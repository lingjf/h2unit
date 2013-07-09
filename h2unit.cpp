/* Jeff Ling , lingjf@gmail.com */

#include "h2unit.h"
#include <cctype>
#include <ctime>

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef strdup
#undef strndup
#undef new
#undef delete

using namespace std;

static inline void h2unit_list_null(h2unit_list* node) {
   node->next = node->prev = (h2unit_list*)0;
}

static inline void h2unit_list_init(h2unit_list* node) {
   node->next = node->prev = node;
}

static inline void __h2unit_list_add(h2unit_list *newl, h2unit_list *prev, h2unit_list *next)
{
   next->prev = newl;
   newl->next = next;
   newl->prev = prev;
   prev->next = newl;
}

static inline void h2unit_list_add_head(h2unit_list *newl, h2unit_list *head)
{
   __h2unit_list_add(newl, head, head->next);
}

static inline void h2unit_list_add_tail(h2unit_list *newl, h2unit_list *head)
{
   __h2unit_list_add(newl, head->prev, head);
}

static inline void h2unit_list_del(h2unit_list *node)
{
   node->next->prev = node->prev;
   node->prev->next = node->next;
   node->next = node->prev = node;
}

static inline bool h2unit_list_empty(h2unit_list *head)
{
   return head->next == head;
}

static inline h2unit_list* h2unit_list_get_head(h2unit_list *head)
{
   return (h2unit_list_empty(head)?((h2unit_list*)0):(head)->next);
}

static inline h2unit_list* h2unit_list_get_tail(h2unit_list *head)
{
   return (h2unit_list_empty(head)?((h2unit_list*)0):(head)->prev);
}

#define h2unit_list_entry(ptr, type, field)  \
   ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->field)))

#define h2unit_list_for_each(iter, head)  \
   for( (iter) = (head)->next; (iter) != (head); (iter) = (iter)->next )

static int __pattern_cmp(char* pattern, char* source)
{
   int negate, found;
   for (;;) {
      switch (*pattern) {
      case '\0':
         return *source ? 1 : 0;
      case '?':
         if (!*source++) return 1;
         pattern++;
         break;
      case '*':
         while (*++pattern == '*') {
         }
         if (!*pattern) return 0;
         for (; *source; source++) {
            if (!__pattern_cmp(pattern, source)) return 0;
         }
         return 1;
      case '[':
         if (!*source) return 1;
         pattern++;
         negate = *pattern == '!' || *pattern == '^';
         if (negate) pattern++;
         for (found = 0; *pattern != ']'; pattern++) {
            if (!*pattern) return 1;
            if (*pattern == '-') {
               char a = *(pattern - 1);
               char z = *(pattern + 1);
               if (a == '[' || a == '!' || a == '^' || a == '-') a = 0x00;
               if (z == ']' || z == '!' || z == '^' || z == '-') z = 0x7f;
               if (a <= *source && *source <= z) found = 1;
            } else {
               if (*pattern == *source) found = 1;
            }
         }
         if ((negate && found) || (!negate && !found)) return 1;
         pattern++, source++;
         break;
      default:
         if (*pattern++ != *source++) return 1;
         break;
      }
   }
   return 1;
}

class h2unit_config
{
public:
   bool _colored;
   bool _verbose;
   bool _random;
   char* _include;
   char* _exclude;

   h2unit_config()
   {
      _verbose = false;
      _colored = true;
      _random = false;
      _include = NULL;
      _exclude = NULL;
   }
} cfg;

#ifdef _WIN32
#include <windows.h>
static long __milliseconds()
{
   return timeGetTime() / 1000;
}
#define srandom srand
#define random rand
#define strcasecmp _stricmp
#else
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
static long __milliseconds()
{
   struct timeval tv;
   struct timezone tz;
   gettimeofday(&tv, &tz);
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif


class h2unit_fail
{
public:
   h2unit_fail() throw ()
   {
   }
   ~h2unit_fail() throw ()
   {
   }
} _fail;

typedef struct h2unit_blob
{
   h2unit_list queue;
   h2unit_list stack;

   void* ptr;
   unsigned char* data;
   size_t size;
   const char* file;
   int line;
} h2unit_blob;

static const int BLOB_GUARD_SIZE = 8;
static const unsigned BLOB_MAGIC_CODE = 0xbeafcafe;

h2unit_blob* h2unit_blob_new(size_t size, size_t alignment, unsigned char c, const char* file, int line)
{
   void* data = malloc(size + BLOB_GUARD_SIZE * sizeof(unsigned) * 2 + alignment);
   if (!data) return NULL;
   h2unit_blob* new_blob = (h2unit_blob*) malloc(sizeof(h2unit_blob));
   if (!new_blob) {
      free(data);
      return NULL;
   }
   new_blob->data = (unsigned char*)data;
   new_blob->ptr = new_blob->data + BLOB_GUARD_SIZE * sizeof(unsigned);
   new_blob->ptr = (void*)(((unsigned long)(new_blob->ptr)+((alignment)-1))&(~((alignment)-1)));
   new_blob->size = size;
   new_blob->file = file;
   new_blob->line = line;

   h2unit_list_init(&new_blob->queue);
   h2unit_list_init(&new_blob->stack);

   for (int l = 0; l < (int)size; l++) {
      ((unsigned char*)new_blob->ptr)[l] = c;
   }

   unsigned* p = (unsigned*)(new_blob->ptr) - 1;
   unsigned* q = (unsigned*)((unsigned char*)(new_blob->ptr) + new_blob->size);
   for (int i = 0; i < BLOB_GUARD_SIZE; i++) {
      p[-i] = BLOB_MAGIC_CODE;
      q[i] = BLOB_MAGIC_CODE;
   }

   return new_blob;
}

void h2unit_blob_del(h2unit_blob* blob)
{
   h2unit_list_init(&blob->queue);
   h2unit_list_init(&blob->stack);

   /* overflow and under-flow checking */
   bool overed = false;
   unsigned* p = (unsigned*)(blob->ptr) - 1;
   unsigned* q = (unsigned*)((unsigned char*)(blob->ptr) + blob->size);
   for (int i = 0; i < BLOB_GUARD_SIZE; i++) {
      if (p[-i] != BLOB_MAGIC_CODE || q[i] != BLOB_MAGIC_CODE) {
         if (h2unit_case::_current_) {
            h2unit_case::_current_->_vmsg_(&h2unit_case::_current_->_errormsg_, "bold,red", q[i] == BLOB_MAGIC_CODE ? "Memory OverFlow" : "Memory UnderFlow");
            h2unit_case::_current_->_vmsg_(&h2unit_case::_current_->_errormsg_, "", " at %s:%d", blob->file, blob->line);
            overed = true;
            break;
         }
      }
   }

   free(blob->data);
   free(blob);

   if (overed) {
      throw _fail;
   }
}

typedef struct h2unit_leak
{
   h2unit_list stack;
   h2unit_list blobs;

   const char* file;
   int line;
} h2unit_leak;

typedef struct h2unit_symb
{
   char* name;
   void* addr;
   struct h2unit_symb* next;
} h2unit_symb;

typedef struct h2unit_stub
{
   struct h2unit_stub* next;
   void* orig;
   unsigned char code[sizeof(void*) + 4];
} h2unit_stub;

typedef struct h2unit_unit
{
   const char* name;
   int case_count;
   int case_passed;
   int case_failed;
   int case_ignored;
   int case_filtered;
   h2unit_case* cases;
   struct h2unit_unit* next;
} h2unit_unit;

class h2unit_listen
{
public:
   class h2unit_listen* next;
public:
   h2unit_listen()
   {
      next = NULL;
   }
   virtual ~h2unit_listen()
   {
   }
   virtual void on_task_start()
   {
   }
   virtual void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration, h2unit_unit* unit_list)
   {
      for (h2unit_unit* p = unit_list; p; p = p->next) {
         for (h2unit_case* c = p->cases; c; c = c->_next_) {
            p->case_count++;
            switch (c->_status_) {
            case h2unit_case::_IGNORE_:
               p->case_ignored++;
               break;
            case h2unit_case::_FILTED_:
               p->case_filtered++;
               break;
            case h2unit_case::_PASSED_:
               p->case_passed++;
               break;
            case h2unit_case::_FAILED_:
               p->case_failed++;
               break;
            }
         }
      }
   }
   virtual void on_case_start()
   {
   }
   virtual void on_case_endup(double percentage)
   {
   }
};

class h2unit_listens: public h2unit_listen
{
public:
   h2unit_listens()
   {
   }
   void attach(h2unit_listen* listener)
   {
      listener->next = next;
      next = listener;
   }
   void on_task_start()
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_task_start();
      }
   }
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration, h2unit_unit* unit_list)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_task_endup(failed, passed, ignored, filtered, cases, checks, duration, unit_list);
      }
   }
   void on_case_start()
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_start();
      }
   }
   void on_case_endup(double percentage)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_endup(percentage);
      }
   }
};

class h2unit_listen_text: public h2unit_listen
{
private:
   FILE *filp;
   void print_string(h2unit_string* s)
   {
      for (h2unit_string* p = s; p; p = p->next) {
         fprintf(filp, "%s", p->data);
      }
   }
public:
   h2unit_listen_text()
   {
   }
   virtual ~h2unit_listen_text()
   {
   }
   void on_task_start()
   {
      filp = fopen("h2unit_text.log", "w");
   }
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration, h2unit_unit* unit_list)
   {
      if (failed > 0) {
         fprintf(filp, "\nFailed <%d failed, %d passed, %d ignored, %d filtered, %d checks, %ld ms>\n", failed, passed, ignored, filtered, checks, duration);
      } else {
         fprintf(filp, "\nPassed <%d passed, %d ignored, %d filtered, %d cases, %d checks, %ld ms>\n", passed, ignored, filtered, cases, checks, duration);
      }
      fclose(filp);
   }
   void on_case_endup(double percentage)
   {
      h2unit_case* p = h2unit_case::_current_;
      switch (p->_status_) {
      case h2unit_case::_IGNORE_:
         fprintf(filp, "H2CASE(%s, %s): Ignored at %s:%d\n", p->_unitname_, p->_casename_, p->_casefile_, p->_caseline_);
         break;
      case h2unit_case::_FILTED_:
         break;
      case h2unit_case::_PASSED_:
         if (cfg._verbose) {
            fprintf(filp, "H2CASE(%s, %s): Passed - %ld ms     \n", p->_unitname_, p->_casename_, p->_endup_ - p->_start_);
         }
         break;
      case h2unit_case::_FAILED_:
         fprintf(filp, "H2CASE(%s, %s): Failed at %s:%d\n", p->_unitname_, p->_casename_, p->_checkfile_, p->_checkline_);
         if (p->_errormsg_) {
            fprintf(filp, "  ");
            print_string(h2unit_case::_current_->_errormsg_);
            fprintf(filp, "\n");
         }
         if (p->_expected_) {
            fprintf(filp, "  expected<");
            print_string(h2unit_case::_current_->_expected_);
            fprintf(filp, ">\n");
         }
         if (p->_actually_) {
            fprintf(filp, "  actually<");
            print_string(h2unit_case::_current_->_actually_);
            fprintf(filp, ">\n");
         }
         if (p->_addition_) {
            for (int i = 0; h2unit_case::_current_->_addition_[i]; i++) {
               fprintf(filp, "   ");
               print_string(h2unit_case::_current_->_addition_[i]);
               fprintf(filp, "\n");
            }
         }
         break;
      }
   }

};

class h2unit_listen_console: public h2unit_listen
{
private:

   const char* color(const char *style)
   {
      if (!cfg._colored) return "";

      struct st
      {
         const char *name;
         const int value;
      };

      static struct st t[] = {
      // normal style
            { "reset", 0 }, { "bold", 1 }, { "italics", 3 }, { "underline", 4 }, { "inverse", 7 }, { "strikethrough", 9 },
            // foreground color
            { "black", 30 }, { "red", 31 }, { "green", 32 }, { "yellow", 33 }, { "blue", 34 }, { "purple", 35 }, { "cyan", 36 }, { "white", 37 }, { "default", 39 },
            // background color
            { "bg_black", 40 }, { "bg_red", 41 }, { "bg_green", 42 }, { "bg_yellow", 43 }, { "bg_blue", 44 }, { "bg_purple", 45 }, { "bg_cyan", 46 }, { "bg_white", 47 }, { "bg_default", 49 }

      };

      static char copied[512];
      strncpy(copied, style, sizeof(copied));

      static char buffer[128];
      char *p = buffer;
      p += sprintf(p, "\033[");

      for (char* opt = strtok(copied, ","); opt; opt = strtok(NULL, ",")) {
         for (unsigned i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
            if (strcmp(t[i].name, opt) == 0) {
               p += sprintf(p, "%d;", t[i].value);
               break;
            }
         }
      }

      *(p - 1) = 'm';
      return (const char*) buffer;
   }

   void print_string(h2unit_string* s)
   {
      for (h2unit_string* p = s; p; p = p->next) {
         printf("%s", color(p->style));
         printf("%s", p->data);
         printf("%s", color("reset"));
      }
   }

public:
   h2unit_listen_console()
   {
   }
   virtual ~h2unit_listen_console()
   {
   }
   void on_task_start()
   {
   }
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration, h2unit_unit* unit_list)
   {
      printf("\r                                                    \n");
      if (failed > 0) {
         printf("%s", color("bold,red"));
         printf("Failed <%d failed, %d passed, %d ignored, %d filtered, %d checks, %ld ms>\n", failed, passed, ignored, filtered, checks, duration);
      } else {
         printf("%s", color("bold,green"));
         printf("Passed <%d passed, %d ignored, %d filtered, %d cases, %d checks, %ld ms>\n", passed, ignored, filtered, cases, checks, duration);
      }
      printf("%s\n", color("reset"));
   }
   void on_case_endup(double percentage)
   {
      h2unit_case* p = h2unit_case::_current_;
      switch (p->_status_) {
      case h2unit_case::_IGNORE_:
         printf("\rH2CASE(%s, %s): Ignored at %s:%d\n", p->_unitname_, p->_casename_, p->_casefile_, p->_caseline_);
         break;
      case h2unit_case::_FILTED_:
         break;
      case h2unit_case::_PASSED_:
         if (cfg._verbose) {
            printf("%s", color("blue"));
            printf("\rH2CASE(%s, %s): Passed - %ld ms     \n", p->_unitname_, p->_casename_, p->_endup_ - p->_start_);
            printf("%s", color("reset"));
         }
         break;
      case h2unit_case::_FAILED_:
         printf("%s", color("bold,purple"));
         printf("\rH2CASE(%s, %s): Failed at %s:%d\n", p->_unitname_, p->_casename_, p->_checkfile_, p->_checkline_);
         printf("%s", color("reset"));
         if (p->_errormsg_) {
            printf("  ");
            print_string(h2unit_case::_current_->_errormsg_);
            printf("\n");
         }
         if (p->_expected_) {
            printf("  expected<");
            print_string(h2unit_case::_current_->_expected_);
            printf(">\n");
         }
         if (p->_actually_) {
            printf("  actually<");
            print_string(h2unit_case::_current_->_actually_);
            printf(">\n");
         }
         if (p->_addition_) {
            for (int i = 0; h2unit_case::_current_->_addition_[i]; i++) {
               printf("   ");
               print_string(h2unit_case::_current_->_addition_[i]);
               printf("\n");
            }
         }
         break;
      }

      printf("%s", color("bold,blue"));
      printf("\rH2UNIT running ... %d%% completed.", (int) (percentage));
      printf("%s", color("reset"));
   }
};

class h2unit_listen_html: public h2unit_listen
{
private:
   FILE *filp;
public:
   h2unit_listen_html()
   {
   }
   virtual ~h2unit_listen_html()
   {
   }
   void on_task_start()
   {
      filp = fopen("h2unit_html.html", "w");
      fprintf(filp, "<html>");
      fprintf(filp, "<head>");
      fprintf(filp, "<title></title>");
      fprintf(filp, "<style></style>");
      fprintf(filp, "</head>");
      fprintf(filp, "<body>");
      fprintf(filp, "<table>");

   }
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration, h2unit_unit* unit_list)
   {
      for (h2unit_unit* p = unit_list; p; p = p->next) {
         fprintf(filp, "<tr>");
         fprintf(filp, "<td> %s </td>", p->name);
         fprintf(filp, "<td><table>");
         for (h2unit_case* c = p->cases; c; c = c->_next_) {
            fprintf(filp, "<tr>");
            const char* status;
            switch (c->_status_) {
            case h2unit_case::_IGNORE_:
               status = "Ignored";
               break;
            case h2unit_case::_FILTED_:
               status = "Filtered";
               break;
            case h2unit_case::_PASSED_:
               status = "Passed";
               break;
            case h2unit_case::_FAILED_:
               status = "Failed";
               break;
            }
            fprintf(filp, "<td> %s </td>", c->_casename_);
            fprintf(filp, "<td> %s </td>", status);
            fprintf(filp, "</tr>");
         }
         fprintf(filp, "</table></td>");
         fprintf(filp, "</tr>");
      }
      fprintf(filp, "</table>");

      if (failed > 0) {
         fprintf(filp, "Failed <%d failed, %d passed, %d ignored, %d filtered, %d checks, %ld ms>\n", failed, passed, ignored, filtered, checks, duration);
      } else {
         fprintf(filp, "Passed <%d passed, %d ignored, %d filtered, %d cases, %d checks, %ld ms>\n", passed, ignored, filtered, cases, checks, duration);
      }

      fprintf(filp, "</body>");
      fprintf(filp, "</html>");
      fclose(filp);
   }
};

class h2unit_listen_xml: public h2unit_listen
{
private:
   FILE *filp;

   void print_string(h2unit_string* s)
   {
      for (h2unit_string* p = s; p; p = p->next) {
         fprintf(filp, "%s", p->data);
      }
   }
public:
   h2unit_listen_xml()
   {
   }
   virtual ~h2unit_listen_xml()
   {
   }
   void on_task_start()
   {
   }
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration, h2unit_unit* unit_list)
   {
      h2unit_listen::on_task_endup(failed, passed, ignored, filtered, cases, checks, duration, unit_list);
      filp = fopen("h2unit_junit.xml", "w");
      fprintf(filp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(filp, "<testsuites>\n");
      for (h2unit_unit* p = unit_list; p; p = p->next) {
         fprintf(filp, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", p->case_failed, p->name, p->case_ignored + p->case_filtered, p->case_count, 0, "");
         for (h2unit_case* c = p->cases; c; c = c->_next_) {
            const char* status;
            switch (c->_status_) {
            case h2unit_case::_IGNORE_:
               status = "Ignored";
               break;
            case h2unit_case::_FILTED_:
               status = "Filtered";
               break;
            case h2unit_case::_PASSED_:
               status = "Passed";
               break;
            case h2unit_case::_FAILED_:
               status = "Failed";
               break;
            }
            fprintf(filp, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", c->_unitname_, c->_casename_, status, (c->_endup_ - c->_start_) / 1000.0);
            if (c->_status_ == h2unit_case::_FAILED_) {
               fprintf(filp, "      <failure message=\"Failed at %s:%d\"></failure>\n", c->_checkfile_, c->_checkline_);
            }
            if (c->_errormsg_) {
               fprintf(filp, "      <failure message=\"");
               print_string(c->_errormsg_);
               fprintf(filp, "\"></failure>\n");
            }
            if (c->_expected_) {
               fprintf(filp, "      <failure message=\"expected<");
               print_string(c->_expected_);
               printf("n");
               fprintf(filp, ">\"></failure>\n");
            }
            if (c->_actually_) {
               fprintf(filp, "      <failure message=\"actually<");
               print_string(c->_actually_);
               fprintf(filp, ">\"></failure>\n");
            }
            if (c->_addition_) {
               for (int i = 0; c->_addition_[i]; i++) {
                  fprintf(filp, "      <failure message=\"");
                  print_string(c->_addition_[i]);
                  fprintf(filp, "\"></failure>\n");
               }
            }
            fprintf(filp, "      <system-out></system-out><system-err></system-err>\n");
            fprintf(filp, "    </testcase>\n");
         }
         fprintf(filp, "  </testsuite>\n");
      }
      fprintf(filp, "</testsuites>\n");
      fclose(filp);
   }
};


class h2unit_task
{
public:
   int unit_count, case_count, case_excuted_count, check_count;
   int case_failed, case_passed, case_ignore, case_filter;
   h2unit_symb* symb_list;
   h2unit_list blob_list;
   h2unit_stub* stub_list;
   h2unit_unit* unit_list;
   h2unit_case* case_chain;
   unsigned limited;

   h2unit_listen_text text_listener;
   h2unit_listen_console console_listener;
   h2unit_listen_html html_listener;
   h2unit_listen_xml xml_listener;
   h2unit_listens listener;

   h2unit_task()
   {
      case_failed = case_passed = case_ignore = case_filter = 0;
      unit_count = case_count = case_excuted_count = check_count = 0;
      case_chain = NULL;

      unit_list = NULL;
      symb_list = NULL;
      stub_list = NULL;
      h2unit_list_init(&blob_list);
      limited = 0x7fffffff;

      listener.attach(&console_listener);
      listener.attach(&text_listener);
      listener.attach(&xml_listener);
      listener.attach(&html_listener);
   }

   static h2unit_task* O()
   {
      static h2unit_task _instance;
      return &_instance;
   }

   void build_symbols(char* path)
   {
#ifndef _WIN32
      char buf[512];
      char* line = buf;
      size_t len = sizeof(buf);
      int n;
      const char* symb_file = "h2unit_sym.txt";
      /**
       * TODO: for windows using dumpbin.exe or BFD (Binary File Descriptor Library)
       *
       * dumpbin.exe should be invoked from Visual Studio Command Prompt.
       * How to invoke dumpbin.exe with system() in code ?
       *
       * BFD is a very heavy library, it will break h2unit's principle : Light-Weight just like hydrogen !
       *
       * http://support.microsoft.com/kb/177429
       * http://sourceware.org/binutils/docs-2.23.1/bfd/index.html
       */
      sprintf(buf, "nm %s > %s", path, symb_file);
      system(buf);

      FILE* filp = fopen(symb_file, "r");
      if (filp == NULL) {
         return;
      }

      while ((n = getline(&line, &len, filp)) != -1) {
         char *t;
         t = strtok(line, " ");
         if (t && t[0] == '0' && strlen(t) > 4) {
            long a = strtol(t, NULL, 16);
            t = strtok(NULL, " ");
            if (t && tolower((int)t[0]) == 't' && strlen(t) == 1) {
               t = strtok(NULL, " ");
               h2unit_symb* s = (h2unit_symb*) malloc(sizeof(h2unit_symb));
               s->name = strdup(t);
               for (int l = strlen(s->name); l > 0 && isspace (s->name[l - 1]); l--) {
                  s->name[l - 1] = '\0';
               }
               s->addr = (void*)a;
               s->next = symb_list;
               symb_list = s;

            }
         }
      }

      fclose(filp);
      sprintf(buf, "unlink %s", symb_file);
      system(buf);
#endif
   }

   void* get_symbol_address(const char* symb)
   {
      for (h2unit_symb* p = symb_list; p != NULL; p = p->next) {
         if (!strcmp(p->name, symb)) {
            return p->addr;
         }
         /* some compiler prefix _ to all symbols */
         if (p->name[0] == '_') {
            if (!strcmp(p->name + 1, symb)) {
               return p->addr;
            }
         }
      }
      return NULL;
   }

   void install_testcase(h2unit_case* testcase)
   {
      testcase->_chain_ = case_chain;
      case_chain = testcase;
      case_count++;

      h2unit_unit** p;
      for (p = &unit_list; *p; p = &(*p)->next) {
         if (strcmp((*p)->name, testcase->_unitname_) == 0) {
            break;
         }
      }
      if (*p == NULL) {
         *p = (h2unit_unit*) malloc(sizeof(h2unit_unit));
         memset(*p, 0, sizeof(h2unit_unit));
         (*p)->name = testcase->_unitname_;
         unit_count++;
      }

      h2unit_case** c;
      for (c = &(*p)->cases; *c; c = &(*c)->_next_) {
         if ((*c)->_caseline_ > testcase->_caseline_) {
            break;
         }
      }
      testcase->_next_ = *c;
      *c = testcase;
   }

   void random_sequence()
   {
      h2unit_case* p = case_chain;
      case_chain = NULL;
      srandom(__milliseconds());
      for (int i = 1; p; i++) {
         h2unit_case* n = p->_chain_;

         int rpos = random() % i;
         h2unit_case** ip = &case_chain;
         for (int j = 0; j < rpos; j++) {
            ip = &((*ip)->_chain_);
         }
         p->_chain_ = *ip;
         *ip = p;

         p = n;
      }
   }

   void run()
   {
      long start = __milliseconds();
      if (cfg._random) random_sequence();
      listener.on_task_start();
      for (h2unit_case* p = case_chain; (h2unit_case::_current_ = p); p = p->_chain_) {
         listener.on_case_start();
         p->_execute_();
         case_excuted_count += 1;
         check_count += p->_checkcount_;

         listener.on_case_endup(100.0 * case_excuted_count / case_count);

         switch (p->_status_) {
         case h2unit_case::_IGNORE_:
            case_ignore++;
            break;
         case h2unit_case::_FILTED_:
            case_filter++;
            break;
         case h2unit_case::_PASSED_:
            case_passed++;
            break;
         case h2unit_case::_FAILED_:
            case_failed++;
            break;
         }
      }
      listener.on_task_endup(case_failed, case_passed, case_ignore, case_filter, case_count, check_count, __milliseconds() - start, unit_list);
   }

   h2unit_blob* get_blob(void* ptr)
   {
      h2unit_list* p;
      h2unit_list_for_each(p, &blob_list) {
         h2unit_blob* b = h2unit_list_entry(p, h2unit_blob, queue);
         if (b->ptr == ptr) return b;
      }
      return NULL;
   }

   h2unit_blob* add_blob(void* old, size_t size, size_t alignment, unsigned char c, const char* file, int line)
   {
      h2unit_blob* old_blob = get_blob(old);
      size_t old_size = old_blob ? old_blob->size : 0;

      if (size > limited + old_size) return NULL;

      h2unit_blob* new_blob = h2unit_blob_new(size, alignment, c, file, line);

      h2unit_list_add_head(&new_blob->queue, &blob_list);

      if (h2unit_case::_current_) {
         h2unit_case::_current_->_blob_add_(&new_blob->stack);
      }

      if (old_blob) {
         memcpy(new_blob->ptr, old_blob->ptr, old_size);
         del_blob(old_blob);
      }
      limited -= size;

      return new_blob;
   }

   void del_blob(h2unit_blob* blob)
   {
      limited += blob->size;
      h2unit_list_del(&blob->stack);
      h2unit_list_del(&blob->queue);
      h2unit_blob_del(blob);
   }

   h2unit_stub* add_stub(void* orig)
   {
      h2unit_stub* stub = (h2unit_stub*) malloc(sizeof(h2unit_stub));
      if (stub) {
         stub->orig = orig;
         stub->next = stub_list;
         stub_list = stub;
         memcpy(stub->code, orig, sizeof(stub->code));
      }
      return stub;
   }

   h2unit_stub* get_stub(void* orig)
   {
      h2unit_stub* stub;
      for (stub = stub_list; stub; stub = stub->next) {
         if (stub->orig == orig) return stub;
      }
      return NULL;
   }

   void del_stubs()
   {
      while (stub_list) {
         h2unit_stub* stub = stub_list;
         stub_list = stub->next;
         memcpy(stub->orig, stub->code, sizeof(stub->code));
         free(stub);
      }
   }
};

h2unit_auto::h2unit_auto(const char* file, int line)
{
   done = false;
   h2unit_case::_current_->_leak_push_(file, line);
}

h2unit_auto::~h2unit_auto()
{
   if (!h2unit_case::_current_->_leak_pop_()) {
      throw _fail;
   }
}

h2unit_case* h2unit_case::_current_ = NULL;

h2unit_case::h2unit_case()
{
}

h2unit_case::~h2unit_case()
{
}

void h2unit_case::_init_(const char* unitname, const char* casename, bool ignored, const char* file, int line)
{
   _status_ = _INITED_;
   _unitname_ = unitname;
   _casename_ = casename;
   _casefile_ = file;
   _caseline_ = line;

   _checkfile_ = file;
   _checkline_ = line;
   _checkcount_ = 0;

   _errormsg_ = NULL;
   _expected_ = NULL;
   _actually_ = NULL;
   _addition_ = NULL;

   h2unit_list_init(&_leak_stack_);

   if (ignored) _status_ = _IGNORE_;

   h2unit_task::O()->install_testcase(this);
}

void h2unit_case::_prev_setup_()
{
   _leak_push_(NULL, 0);
}

void h2unit_case::_post_teardown_()
{
   /* balance test environment automatically */
   _limit_(0x7fffffff);
   h2unit_task::O()->del_stubs();

   /* memory leak detection */
   _leak_pop_();
}

void h2unit_case::setup()
{
}

void h2unit_case::teardown()
{
}

void h2unit_case::_execute_()
{
   _start_ = __milliseconds();
   if (cfg._include != NULL && (__pattern_cmp(cfg._include, (char*) _unitname_) != 0 && __pattern_cmp(cfg._include, (char*) _casename_) != 0)) {
      _status_ = _FILTED_;
      return;
   }
   if (cfg._exclude != NULL && (__pattern_cmp(cfg._exclude, (char*) _unitname_) == 0 || __pattern_cmp(cfg._exclude, (char*) _casename_) == 0)) {
      _status_ = _FILTED_;
      return;
   }

   if (_status_ != _IGNORE_) {
      _status_ = _PASSED_;
      _prev_setup_();
      setup();
      try {
         _testcase_();
      } catch (class h2unit_fail) {
         _status_ = _FAILED_;
      }
      teardown();
      _post_teardown_();
   }

   _endup_ = __milliseconds();
}

void h2unit_case::_limit_(unsigned long bytes)
{
   h2unit_task::O()->limited = bytes;
}

void h2unit_case::_leak_push_(const char* file, int line)
{
   h2unit_leak* leak = (h2unit_leak*) malloc(sizeof(h2unit_leak));
   memset(leak, 0, sizeof(h2unit_leak));
   leak->file = file;
   leak->line = line;
   h2unit_list_init(&leak->stack);
   h2unit_list_init(&leak->blobs);
   h2unit_list_add_head(&leak->stack, &_leak_stack_);
}

void h2unit_case::_blob_add_(h2unit_list* blob)
{
   h2unit_list* head = h2unit_list_get_head(&_leak_stack_);
   h2unit_leak* leak = h2unit_list_entry(head, h2unit_leak, stack);
   h2unit_list_add_head(blob, &leak->blobs);
}

void h2unit_case::_blob_del_(h2unit_list* blob)
{
   h2unit_list_del(blob);
}

bool h2unit_case::_leak_pop_()
{
   h2unit_list* head = h2unit_list_get_head(&_leak_stack_);
   h2unit_list_del(head);

   h2unit_leak* leak = h2unit_list_entry(head, h2unit_leak, stack);
   if (h2unit_list_empty(&leak->blobs)) {
      free(leak);
      return true;
   }

   if (_status_ == _FAILED_) { /* other failure already happen, ignore memory leak failure */
      return true;
   }

   _status_ = _FAILED_;

   int count = 0;
   h2unit_list* p;
   h2unit_list_for_each(p, &leak->blobs) {
      count++;
   }

   _addition_ = (h2unit_string**) malloc((count + 1) * sizeof(h2unit_string*));
   memset(_addition_, 0, (count + 1) * sizeof(h2unit_string*));

   int i = 0, leaked = 0;
   h2unit_list_for_each(p, &leak->blobs) {
      h2unit_blob* b = h2unit_list_entry(p, h2unit_blob, stack);
      _vmsg_(&_addition_[i], "bold,red", "Leaked %d bytes", b->size);
      _vmsg_(&_addition_[i], "", " at %s:%d", b->file, b->line);
      i++;
      leaked += b->size;
   }

   if (leak->file == NULL) {
      _vmsg_(&_errormsg_, "bold,red", "Memory Leaked %d bytes in case totally", leaked);
   } else {
      _vmsg_(&_errormsg_, "bold,red", "Memory Leaked %d bytes in block", leaked);
      _vmsg_(&_errormsg_, "", " at %s:%d", leak->file, leak->line);
   }

   return false;
}

void h2unit_case::_vmsg_(h2unit_string** typed, const char *style, const char* format, ...)
{
   h2unit_string* p = (h2unit_string*) malloc(sizeof(h2unit_string));
   while (*typed) typed = &(*typed)->next;
   *typed = p;

   va_list args;
   va_start(args, format);
   static char t[1024 * 8];
   int sz = vsprintf(t, format, args);
   va_end(args);

   p->next = NULL;
   p->style = style;
   p->data = (char*) malloc(sz + 1);

   va_start(args, format);
   vsprintf(p->data, format, args);
   va_end(args);
}

void* h2unit_case::_addr_(const char* native, const char* native_name, const char* fake_name)
{
   void *address = h2unit_task::O()->get_symbol_address(native);
   if (address == NULL) {
      _vmsg_(&_errormsg_, "", "H2STUB(");
      _vmsg_(&_errormsg_, "bold,red", "%s", native_name);
      _vmsg_(&_errormsg_, "", " <-- ");
      _vmsg_(&_errormsg_, "bold,red", "%s", fake_name);
      _vmsg_(&_errormsg_, "", ")");
      _vmsg_(&_errormsg_, "bold,purple", " %s not found", native_name);

      throw _fail;
   }
   return address;
}

void h2unit_case::_stub_(void* native, void* fake, const char* native_name, const char* fake_name)
{
   char reason[128];
   unsigned char *I = (unsigned char*) native;
#ifdef _WIN32
   DWORD saved;
   if (!VirtualProtect(native, sizeof(void*) + 4, PAGE_WRITECOPY, &saved)) { //PAGE_EXECUTE_WRITECOPY
      sprintf(reason, "VirtualProtect:%d", GetLastError());
      goto failure;
   }
#else
   int pagesize = sysconf(_SC_PAGE_SIZE);
   if (mprotect((void*) ((unsigned long) native & (~(pagesize - 1))), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
      sprintf(reason, "mprotect:%s", strerror(errno));
      goto failure;
   }
#endif
   if (!h2unit_task::O()->get_stub(native)) {
      if (!h2unit_task::O()->add_stub(native)) {
         sprintf(reason, "out of memory");
         goto failure;
      }
   }
   //x86 __asm("jmp $fake") : 0xE9 {fake-orig-5}
   //x86 __asm("movl $fake, %eax; jmpl %eax") : 0xB8 {fake} 0xFF 0xE0
   //x86_64 __asm("movq $fake, %rax; jmpq %rax") : 0x48 0xB8 {fake} 0xFF 0xE0
#ifdef __x86_64__
   *I++ = 0x48;
   *I++ = 0xB8;
   memcpy(I, &fake, sizeof(void*));
   I += sizeof(void*);
   *I++ = 0xFF;
   *I++ = 0xE0;
#else
   *I++ = 0xE9;
   fake = (void*) ((unsigned long) fake - (unsigned long) native - (sizeof(void*) + 1));
   memcpy(I, &fake, sizeof(void*));
#endif

   return;

   failure:

   _vmsg_(&_errormsg_, "", "H2STUB(");
   _vmsg_(&_errormsg_, "bold,red", "%s", native_name);
   _vmsg_(&_errormsg_, "", " <-- ");
   _vmsg_(&_errormsg_, "bold,red", "%s", fake_name);
   _vmsg_(&_errormsg_, "", ")");
   _vmsg_(&_errormsg_, "bold,red", " %s", reason);

   throw _fail;
}

void h2unit_case::_enter_check_(const char* file, int line)
{
   _checkfile_ = file;
   _checkline_ = line;
   _checkcount_++;
}

void h2unit_case::_check_equal_(bool result)
{
   if (!result) {
      _vmsg_(&_expected_, "bold,red", "true");
      _vmsg_(&_actually_, "bold,red", "false");

      throw _fail;
   }
}

void h2unit_case::_check_equal_(int expected, int actually)
{
   if (expected != actually) {
      _vmsg_(&_expected_, "bold,red", "%d 0x%x", expected, expected);
      _vmsg_(&_actually_, "bold,red", "%d 0x%x", actually, actually);

      throw _fail;
   }
}

void h2unit_case::_check_equal_(unsigned long expected, unsigned long actually)
{
   if (expected != actually) {
      _vmsg_(&_expected_, "bold,red", "%ld 0x%lx", (long)expected, expected);
      _vmsg_(&_actually_, "bold,red", "%ld 0x%lx", (long)actually, actually);

      throw _fail;
   }
}

void h2unit_case::_check_equal_(unsigned long long expected, unsigned long long actually)
{
   if (expected != actually) {
#ifdef _WIN32
      _vmsg_(&_expected_, "bold,red", "%I64d 0x%I64x", (long long)expected, expected);
      _vmsg_(&_actually_, "bold,red", "%I64d 0x%I64x", (long long)actually, actually);
#else
      _vmsg_(&_expected_, "bold,red", "%lld 0x%llx", (long long)expected, expected);
      _vmsg_(&_actually_, "bold,red", "%lld 0x%llx", (long long)actually, actually);
#endif

      throw _fail;
   }
}

void h2unit_case::_check_equal_(double expected, double actually)
{
   double delta = expected - actually;
   if (delta < 0) delta = -delta;
   if (delta > 0.00001) { /* 0.00001 is epsilon value */
      _vmsg_(&_expected_, "bold,red", "%f", expected);
      _vmsg_(&_actually_, "bold,red", "%f", actually);

      throw _fail;
   }
}

void h2unit_case::_check_equal_(char* expected, char* actually)
{
   if (strcmp(expected, actually) != 0) {
      int actually_length = strlen(actually);
      int expected_length = strlen(expected);

      for (int i = 0; i < expected_length; i++) {
         _vmsg_(&_expected_, i <= actually_length && expected[i] == actually[i] ? "bold,green" : "bold,red", "%c", expected[i]);
      }
      for (int j = 0; j < actually_length; j++) {
         _vmsg_(&_actually_, j <= expected_length && actually[j] == expected[j] ? "bold,green" : "bold,red", "%c", actually[j]);
      }

      throw _fail;
   }
}

void h2unit_case::_check_range_(double from, double to, double actually)
{
   if (actually < from - 0.00001 || actually > to + 0.00001) {
      _vmsg_(&_expected_, "bold,red", "[%f, %f]", from, to);
      _vmsg_(&_actually_, "bold,red", "%f", actually);

      throw _fail;
   }
}

void h2unit_case::_check_inset_(double *inset, int count, double actually)
{
   for (int i = 0; i < count; i++) {
      double delta = inset[i] - actually;
      if (delta < 0) delta = -delta;
      if (delta < 0.00001) {
         return;
      }

   }
   if (count > 0) {
      _vmsg_(&_expected_, "bold,red", "{%f, ...}", inset[0]);
   } else {
      _vmsg_(&_expected_, "bold,red", "{}");
   }
   _vmsg_(&_actually_, "bold,red", "%f", actually);

   throw _fail;
}

void h2unit_case::_check_equal_strcmp_nocase_(char* expected, char* actually)
{
   if (strcasecmp(expected, actually) != 0) {
      int actually_length = strlen(actually);
      int expected_length = strlen(expected);

      for (int i = 0; i < expected_length; i++) {
         _vmsg_(&_expected_, i <= actually_length && tolower((int) expected[i]) == tolower((int) actually[i]) ? "bold,green" : "bold,red", "%c", expected[i]);
      }
      for (int j = 0; j < actually_length; j++) {
         _vmsg_(&_actually_, j <= expected_length && tolower((int) actually[j]) == tolower((int) expected[j]) ? "bold,green" : "bold,red", "%c", actually[j]);
      }

      throw _fail;
   }
}

void h2unit_case::_check_regex_(char* express, char* actually)
{
   if (__pattern_cmp(express, actually) != 0) {
      _vmsg_(&_expected_, "bold,red", "%s", express);
      _vmsg_(&_actually_, "bold,red", "%s", actually);

      throw _fail;
   }
}

void h2unit_case::_check_equal_(unsigned char* expected, unsigned char* actually, int length)
{
   if (memcmp(expected, actually, length) != 0) {
      for (int i = 0; i < length; i++) {
         _vmsg_(&_expected_, expected[i] == actually[i] ? "bold,green" : "bold,red", i % 16 < 8 ? "%02X " : " %02X", expected[i]);
         _vmsg_(&_actually_, actually[i] == expected[i] ? "bold,green" : "bold,red", i % 16 < 8 ? "%02X " : " %02X", actually[i]);
      }

      throw _fail;
   }
}

void h2unit_case::_check_catch_(const char* expected, const char* actually, const char* exceptype)
{
   if (expected != actually) {
      _vmsg_(&_expected_, "bold,red", "%s %s", expected, exceptype);
      _vmsg_(&_actually_, "bold,red", "%s %s", actually, exceptype);

      throw _fail;
   }
}

void* h2unit_alloc(void* ptr, size_t size, size_t alignment, unsigned char c, const char* file, int line)
{
   if (size == 0) {
      h2unit_free(ptr, file, line);
      return NULL;
   }
   h2unit_blob* b = h2unit_task::O()->add_blob(ptr, size, alignment, c, file, line);
   return b ? b->ptr : NULL;
}

void h2unit_free(void* ptr, const char* file, int line)
{
   h2unit_blob* b = h2unit_task::O()->get_blob(ptr);
   if (b) h2unit_task::O()->del_blob(b);
}

int h2unit_posix_memalign(void** ptr, size_t alignment, size_t size, const char* file, int line)
{
   alignment = 4;
   h2unit_blob* b = h2unit_task::O()->add_blob(NULL, size, alignment, 0xED, file, line);
   if (b) {
      *ptr = b->ptr;
      return 0;
   }
#ifndef ENOMEM
#define ENOMEM -1
#endif
   return ENOMEM;
}

char* h2unit_strdup(const char* s, const char* file, int line)
{
   size_t size = strlen(s) + 1;
   h2unit_blob* b = h2unit_task::O()->add_blob(NULL, size, 4, 0xED, file, line);
   if (b) {
      memcpy(b->ptr, (const void*)s, size);
   }
   return (char*) (b ? b->ptr : NULL);
}

char* h2unit_strndup(const char* s, size_t n, const char* file, int line)
{
   size_t size = strlen(s);
   size = (size > n ? n : size) + 1;
   h2unit_blob* b = h2unit_task::O()->add_blob(NULL, size, 4, 0xED, file, line);
   if (b) {
      memcpy(b->ptr, (const void*)s, size - 1);
      ((char*)b->ptr)[size - 1] = '\0';
   }
   return (char*) (b ? b->ptr : NULL);
}

void* operator new(size_t size, const char* file, int line)
{
   return h2unit_alloc(NULL, size, 4, 0x0, file, line);
}

void* operator new[](size_t size, const char* file, int line)
{
   return h2unit_alloc(NULL, size, 4, 0x0, file, line);
}

void operator delete(void* object)
{
   h2unit_free(object, "", 0);
}

void operator delete[](void* object)
{
   h2unit_free(object, "", 0);
}

int main(int argc, char** argv)
{
   h2unit_task::O()->build_symbols(argv[0]);
   if (argc > 1) {
      if (strstr(argv[1], "v")) cfg._verbose = true;
      if (strstr(argv[1], "b")) cfg._colored = false;
      if (strstr(argv[1], "r")) cfg._random = true;
      if (strstr(argv[1], "i")) cfg._include = argv[2];
      if (strstr(argv[1], "x")) cfg._exclude = argv[2];
   }
   h2unit_task::O()->run();
   return 0;
}

