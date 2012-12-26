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
   char* _filter;

   h2unit_config()
   {
      _verbose = false;
      _colored = false;
      _random = false;
      _filter = NULL;
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
static long __milliseconds()
{
   struct timeval tv;
   struct timezone tz;
   gettimeofday(&tv, &tz);
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif

typedef struct h2unit_leak
{
   struct h2unit_leak* next;
   void* data;
   int length;
   const char* file;
   int line;
   h2unit_case* owner;
} h2unit_leak;

typedef struct h2unit_stub
{
   struct h2unit_stub* next;
   void* orig;
   unsigned char code[sizeof(void*) + 4];
} h2unit_stub;

class h2unit_listen
{
protected:
   const char* unitname;
   const char* casename;
   const char* casefile;
   int caseline;
   const char* checkfile;
   int checkline;
public:
   class h2unit_listen* next;
public:
   h2unit_listen()
   {
      next = NULL;
      unitname = "";
      casename = "";
      casefile = "";
      caseline = 0;
      checkfile = "";
      checkline = 0;
   }
   virtual ~h2unit_listen()
   {
   }
   virtual void on_task_start() = 0;
   virtual void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration) = 0;
   virtual void on_case_start(const char* unitname, const char* casename, const char* casefile, int caseline)
   {
      this->unitname = unitname;
      this->casename = casename;
      this->casefile = casefile;
      this->caseline = caseline;
   }
   virtual void on_case_endup(double percentage)
   {
      this->unitname = "";
      this->casename = "";
      this->casefile = "";
      this->caseline = 0;
   }
   virtual void on_check_enter(const char* checkfile, int checkline)
   {
      this->checkfile = checkfile;
      this->checkline = checkline;
   }
   virtual void on_case_ignored() = 0;
   virtual void on_case_filtered() = 0;
   virtual void on_case_failed() = 0;
   virtual void on_case_passed(long duration) = 0;
   virtual void on_case_leaked(int leakbytes) = 0;
   virtual void on_case_leakat(int leakbytes, const char* leakfile, int leakline) = 0;
   virtual void on_case_failed_check(const char* condition, bool result) = 0;
   virtual void on_case_failed_integer(int expected, int actual) = 0;
   virtual void on_case_failed_double(double expected, double actual) = 0;
   virtual void on_case_failed_strcmp(char* expected, char* actual) = 0;
   virtual void on_case_failed_strcmp_nocase(char* expected, char* actual) = 0;
   virtual void on_case_failed_regex(char* express, char* actual) = 0;
   virtual void on_case_failed_memcmp(unsigned char* expected, unsigned char* actual, int length) = 0;
   virtual void on_case_failed_catch(const char* expected, const char* actual, const char* exceptype) = 0;
   virtual void on_case_failed_stub(const char* express, const char* reason) = 0;
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
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_task_endup(failed, passed, ignored, filtered, cases, checks, duration);
      }
   }
   void on_case_start(const char* unitname, const char* casename, const char* casefile, int caseline)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_start(unitname, casename, casefile, caseline);
      }
   }
   void on_case_endup(double percentage)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_endup(percentage);
      }
   }
   void on_check_enter(const char* checkfile, int checkline)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_check_enter(checkfile, checkline);
      }
   }
   void on_case_ignored()
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_ignored();
      }
   }
   void on_case_filtered()
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_filtered();
      }
   }
   void on_case_failed()
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed();
      }
   }
   void on_case_passed(long duration)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_passed(duration);
      }
   }
   void on_case_leaked(int leakbytes)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_leaked(leakbytes);
      }
   }
   void on_case_leakat(int leakbytes, const char* leakfile, int leakline)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_leakat(leakbytes, leakfile, leakline);
      }
   }
   void on_case_failed_check(const char* condition, bool result)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_check(condition, result);
      }
   }
   void on_case_failed_integer(int expected, int actual)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_integer(expected, actual);
      }
   }
   void on_case_failed_double(double expected, double actual)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_double(expected, actual);
      }
   }
   void on_case_failed_strcmp(char* expected, char* actual)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_strcmp(expected, actual);
      }
   }
   void on_case_failed_strcmp_nocase(char* expected, char* actual)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_strcmp_nocase(expected, actual);
      }
   }
   void on_case_failed_regex(char* express, char* actual)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_regex(express, actual);
      }
   }
   void on_case_failed_memcmp(unsigned char* expected, unsigned char* actual, int length)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_memcmp(expected, actual, length);
      }
   }
   void on_case_failed_catch(const char* expected, const char* actual, const char* exceptype)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_catch(expected, actual, exceptype);
      }
   }
   void on_case_failed_stub(const char* express, const char* reason)
   {
      for (class h2unit_listen *p = next; p; p = p->next) {
         p->on_case_failed_stub(express, reason);
      }
   }
};

class h2unit_listen_text: public h2unit_listen
{
private:
   FILE *filp;

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
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration)
   {
      if (failed > 0) {
         fprintf(filp, "Failed <%d failed, %d passed, %d ignored, %d filtered, %d checks, %ld ms>\n", failed, passed, ignored, filtered, checks, duration);
      } else {
         fprintf(filp, "Passed <%d passed, %d ignored, %d filtered, %d cases, %d checks, %ld ms>\n", passed, ignored, filtered, cases, checks, duration);
      }
      fclose(filp);
   }
   void on_case_ignored()
   {
      fprintf(filp, "H2CASE(%s, %s): Ignored at %s:%d\n", unitname, casename, casefile, caseline);
   }
   void on_case_filtered()
   {
   }
   void on_case_passed(long duration)
   {
      fprintf(filp, "H2CASE(%s, %s): Passed - %ld ms     \n", unitname, casename, duration);
   }
   void on_case_leaked(int leakbytes)
   {
      fprintf(filp, "H2CASE(%s, %s): Memory Leaked %d bytes totally at %s:%d\n", unitname, casename, leakbytes, casefile, caseline);
   }
   void on_case_leakat(int leakbytes, const char* leakfile, int leakline)
   {
      fprintf(filp, "  Leaked %d bytes at %s:%d\n", leakbytes, leakfile, leakline);
   }
   void on_case_failed()
   {
      fprintf(filp, "H2CASE(%s, %s): Failed at %s:%d\n", unitname, casename, checkfile, checkline);
   }
   void on_case_failed_check(const char* condition, bool result)
   {
      on_case_failed();
      fprintf(filp, "  H2CHECK(%s)\n", condition);
   }
   void on_case_failed_integer(int expected, int actual)
   {
      on_case_failed();
      fprintf(filp, "  expected is <%d 0x%x>\n", expected, expected);
      fprintf(filp, "  actually is <%d 0x%x>\n", actual, actual);
   }
   void on_case_failed_double(double expected, double actual)
   {
      on_case_failed();
      fprintf(filp, "  expected is <%f>\n", expected);
      fprintf(filp, "  actually is <%f>\n", actual);
   }
   void on_case_failed_strcmp(char* expected, char* actual)
   {
      on_case_failed();
      fprintf(filp, "  expected is <%s>\n", expected);
      fprintf(filp, "  actually is <%s>\n", actual);
   }
   void on_case_failed_strcmp_nocase(char* expected, char* actual)
   {
      on_case_failed();
      fprintf(filp, "  expected is <%s>\n", expected);
      fprintf(filp, "  actually is <%s>\n", actual);
   }
   void on_case_failed_regex(char* express, char* actual)
   {
      on_case_failed();
      fprintf(filp, "  expected is <%s>\n", express);
      fprintf(filp, "  actually is <%s>\n", actual);
   }
   void on_case_failed_memcmp(unsigned char* expected, unsigned char* actual, int length)
   {
      on_case_failed();
      fprintf(filp, "  expected is [");
      for (int i = 0; i < length; i++) {
         fprintf(filp, "%02X", expected[i]);
      }
      fprintf(filp, "]\n");

      fprintf(filp, "  actually is [");
      for (int i = 0; i < length; i++) {
         fprintf(filp, "%02X", actual[i]);
      }
      fprintf(filp, "]\n");
   }
   void on_case_failed_catch(const char* expected, const char* actual, const char* exceptype)
   {
      on_case_failed();
      fprintf(filp, "  expected is <%s %s>\n", expected, exceptype);
      fprintf(filp, "  actually is <%s %s>\n", actual, exceptype);
   }
   void on_case_failed_stub(const char* express, const char* reason)
   {
      on_case_failed();
      fprintf(filp, "  H2STUB(%s) : %s\n", express, reason);
   }
};

class h2unit_listen_console: public h2unit_listen
{
private:
   char _color_string[128];
   static const int RESET = 0;
   static const int BOLD_ON = 1;
   static const int ITALICS_ON = 3;
   static const int UNDERLINE_ON = 4;
   static const int INVERSE_ON = 7;
   static const int STRIKETHROUGH_ON = 9;
   static const int BOLD_OFF = 22;
   static const int ITALICS_OFF = 23;
   static const int UNDERLINE_OFF = 24;
   static const int INVERSE_OFF = 27;
   static const int STRIKETHROUGH_OFF = 29;
   // foreground color
   static const int BLACK = 30;
   static const int RED = 31;
   static const int GREEN = 32;
   static const int YELLOW = 33;
   static const int BLUE = 34;
   static const int PURPLE = 35;
   static const int CYAN = 36;
   static const int WHITE = 37;
   static const int DEFAULT = 39;
   // background color
   static const int BG_BLACK = 40;
   static const int BG_RED = 41;
   static const int BG_GREEN = 42;
   static const int BG_YELLOW = 43;
   static const int BG_BLUE = 44;
   static const int BG_PURPLE = 45;
   static const int BG_CYAN = 46;
   static const int BG_WHITE = 47;
   static const int BG_DEFAULT = 49;

   const char* color(int code1, int code2 = -1, int code3 = -1, int code4 = -1, int code5 = -1, int code6 = -1, int code7 = -1, int code8 = -1)
   {
      if (!cfg._colored) return "";
      char* p = _color_string;

      p += sprintf(p, "\033[%d", code1);
      if (code2 != -1) {
         p += sprintf(p, ";%d", code2);
         if (code3 != -1) {
            p += sprintf(p, ";%d", code3);
            if (code4 != -1) {
               p += sprintf(p, ";%d", code4);
               if (code5 != -1) {
                  p += sprintf(p, ";%d", code5);
                  if (code6 != -1) {
                     p += sprintf(p, ";%d", code6);
                     if (code7 != -1) {
                        p += sprintf(p, ";%d", code7);
                        if (code8 != -1) {
                           p += sprintf(p, ";%d", code8);
                        }
                     }
                  }
               }
            }
         }
      }
      sprintf(p, "m");
      return (const char*) _color_string;
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
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration)
   {
      printf("\r                                                    \n");
      if (failed > 0) {
         printf("%s", color(BOLD_ON, RED));
         printf("Failed <%d failed, %d passed, %d ignored, %d filtered, %d checks, %ld ms>\n", failed, passed, ignored, filtered, checks, duration);
      } else {
         printf("%s", color(BOLD_ON, GREEN));
         printf("Passed <%d passed, %d ignored, %d filtered, %d cases, %d checks, %ld ms>\n", passed, ignored, filtered, cases, checks, duration);
      }
      printf("%s\n", color(RESET));
   }
   void on_case_endup(double percentage)
   {
      h2unit_listen::on_case_endup(percentage);
      printf("%s", color(BOLD_ON, BLUE));
      printf("\rH2UNIT running ... %d%% completed.", (int) (percentage));
      printf("%s", color(RESET));
   }
   void on_case_ignored()
   {
      printf("%s", color(BOLD_ON, PURPLE));
      printf("\rH2CASE(%s, %s): Ignored at %s:%d\n", unitname, casename, casefile, caseline);
      printf("%s", color(RESET));
   }
   void on_case_filtered()
   {
   }
   void on_case_passed(long duration)
   {
      if (cfg._verbose) {
         printf("%s", color(BLUE));
         printf("\rH2CASE(%s, %s): Passed - %ld ms     \n", unitname, casename, duration);
         printf("%s", color(RESET));
      }
   }
   void on_case_leaked(int leakbytes)
   {
      printf("%s", color(BOLD_ON, PURPLE));
      printf("\rH2CASE(%s, %s): ", unitname, casename);
      printf("%s", color(BOLD_ON, RED));
      printf("Memory Leaked %d bytes totally ", leakbytes);
      printf("%s", color(BOLD_ON, PURPLE));
      printf("at %s:%d\n", casefile, caseline);
      printf("%s", color(RESET));
   }
   void on_case_leakat(int leakbytes, const char* leakfile, int leakline)
   {
      printf("%s", color(BOLD_ON, RED));
      printf("  Leaked %d bytes ", leakbytes);
      printf("%s", color(BOLD_ON, PURPLE));
      printf("at %s:%d\n", leakfile, leakline);
      printf("%s", color(RESET));
   }
   void on_case_failed()
   {
      printf("%s", color(BOLD_ON, PURPLE));
      printf("\rH2CASE(%s, %s): Failed at %s:%d\n", unitname, casename, checkfile, checkline);
      printf("%s", color(RESET));
   }
   void on_case_failed_check(const char* condition, bool result)
   {
      on_case_failed();
      printf("%s", color(BOLD_ON, PURPLE));
      printf("  H2CHECK(");
      printf("%s", color(RED));
      printf("%s", condition);
      printf("%s", color(PURPLE));
      printf(")\n");
      printf("%s", color(RESET));
   }
   void on_case_failed_integer(int expected, int actual)
   {
      on_case_failed();
      printf("%s", color(BOLD_ON, PURPLE));
      printf("  expected is <");
      printf("%s", color(RED));
      printf("%d 0x%x", expected, expected);
      printf("%s", color(PURPLE));
      printf(">\n  actually is <");
      printf("%s", color(RED));
      printf("%d 0x%x", actual, actual);
      printf("%s", color(PURPLE));
      printf(">\n");
      printf("%s", color(RESET));
   }
   void on_case_failed_double(double expected, double actual)
   {
      on_case_failed();
      printf("%s", color(BOLD_ON, PURPLE));
      printf("  expected is <");
      printf("%s", color(RED));
      printf("%f", expected);
      printf("%s", color(PURPLE));
      printf(">\n  actually is <");
      printf("%s", color(RED));
      printf("%f", actual);
      printf("%s", color(PURPLE));
      printf(">\n");
      printf("%s", color(RESET));
   }
   void on_case_failed_strcmp(char* expected, char* actual)
   {
      on_case_failed();
      int i, j, n = 64;
      int actual_length = strlen(actual);
      int expected_length = strlen(expected);
      int length = actual_length > expected_length ? actual_length : expected_length;
      printf("%s", color(BOLD_ON, PURPLE));
      for (i = 0; i < length; i += n) {
         printf("  expected is \"");
         for (j = 0; j < n && i + j < expected_length; j++) {
            if (expected[i + j] == actual[i + j])
               printf("%s", color(BLUE));
            else
               printf("%s", color(RED));
            printf("%c", expected[i + j]);
            printf("%s", color(BOLD_ON, PURPLE));
         }
         printf("\"\n");
         printf("  actually is \"");
         for (j = 0; j < n && i + j < actual_length; j++) {
            if (expected[i + j] == actual[i + j])
               printf("%s", color(BLUE));
            else
               printf("%s", color(RED));
            printf("%c", actual[i + j]);
            printf("%s", color(BOLD_ON, PURPLE));
         }
         printf("\"\n");
      }
      printf("%s", color(RESET));
   }
   void on_case_failed_strcmp_nocase(char* expected, char* actual)
   {
      on_case_failed();
      int i, j, n = 64;
      int actual_length = strlen(actual);
      int expected_length = strlen(expected);
      int length = actual_length > expected_length ? actual_length : expected_length;
      printf("%s", color(BOLD_ON, PURPLE));
      for (i = 0; i < length; i += n) {
         printf("  expected is \"");
         for (j = 0; j < n && i + j < expected_length; j++) {
            if (tolower((int) expected[i + j]) == tolower((int) actual[i + j]))
               printf("%s", color(BLUE));
            else
               printf("%s", color(RED));
            printf("%c", expected[i + j]);
            printf("%s", color(BOLD_ON, PURPLE));
         }
         printf("\"\n");
         printf("  actually is \"");
         for (j = 0; j < n && i + j < actual_length; j++) {
            if (tolower((int) expected[i + j]) == tolower((int) actual[i + j]))
               printf("%s", color(BLUE));
            else
               printf("%s", color(RED));
            printf("%c", actual[i + j]);
            printf("%s", color(BOLD_ON, PURPLE));
         }
         printf("\"\n");
      }
      printf("%s", color(RESET));
   }
   void on_case_failed_regex(char* express, char* actual)
   {
      on_case_failed();
      printf("%s", color(BOLD_ON, PURPLE));
      printf("  wildcard is <");
      printf("%s", color(RED));
      printf("%s", express);
      printf("%s", color(PURPLE));
      printf(">\n  actually is \"");
      printf("%s", color(RED));
      printf("%s", actual);
      printf("%s", color(PURPLE));
      printf("\"\n");
      printf("%s", color(RESET));
   }
   void on_case_failed_memcmp(unsigned char* expected, unsigned char* actual, int length)
   {
      on_case_failed();
      int i, j, n = 16;
      printf("%s", color(BOLD_ON, PURPLE));
      for (i = 0; i < length; i += n) {
         printf("  expected is [");
         for (j = 0; j < n && i + j < length; j++) {
            if (expected[i + j] == actual[i + j])
               printf("%s", color(BLUE));
            else
               printf("%s", color(RED));
            printf(j < n / 2 ? "%02X " : " %02X", expected[i + j]);
            printf("%s", color(BOLD_ON, PURPLE));
         }
         printf("]\n");
         printf("  actually is [");
         for (j = 0; j < n && i + j < length; j++) {
            if (expected[i + j] == actual[i + j])
               printf("%s", color(BLUE));
            else
               printf("%s", color(RED));
            printf(j < n / 2 ? "%02X " : " %02X", actual[i + j]);
            printf("%s", color(BOLD_ON, PURPLE));
         }
         printf("]\n");
      }
      printf("%s", color(RESET));
   }
   void on_case_failed_catch(const char* expected, const char* actual, const char* exceptype)
   {
      on_case_failed();
      printf("%s", color(BOLD_ON, PURPLE));
      printf("  expected is <");
      printf("%s", color(RED));
      printf("%s %s", expected, exceptype);
      printf("%s", color(PURPLE));
      printf(">\n  actually is <");
      printf("%s", color(RED));
      printf("%s %s", actual, exceptype);
      printf("%s", color(PURPLE));
      printf(">\n");
      printf("%s", color(RESET));
   }
   void on_case_failed_stub(const char* express, const char* reason)
   {
      on_case_failed();
      printf("%s", color(BOLD_ON, PURPLE));
      printf("  H2STUB(");
      printf("%s", color(RED));
      printf("%s", express);
      printf("%s", color(PURPLE));
      printf(") : ");
      printf("%s", color(RED));
      printf("%s\n", reason);
      printf("%s", color(RESET));
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
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration)
   {
      if (failed > 0) {
         fprintf(filp, "Failed <%d failed, %d passed, %d ignored, %d filtered, %d checks, %ld ms>\n", failed, passed, ignored, filtered, checks, duration);
      } else {
         fprintf(filp, "Passed <%d passed, %d ignored, %d filtered, %d cases, %d checks, %ld ms>\n", passed, ignored, filtered, cases, checks, duration);
      }

      fprintf(filp, "</table>");
      fprintf(filp, "</body>");
      fprintf(filp, "</html>");
      fclose(filp);
   }
   void on_case_start(const char* unitname, const char* casename, const char* casefile, int caseline)
   {
      h2unit_listen::on_case_start(unitname, casename, casefile, caseline);
      fprintf(filp, "<tr>");
   }
   void on_case_endup(double percentage)
   {
      h2unit_listen::on_case_endup(percentage);
      fprintf(filp, "</tr>");
   }
   void on_case_ignored()
   {
      fprintf(filp, "<td>%s</td><td>%s</td><td>Ignored</td><td>%s:%d</td>", unitname, casename, casefile, caseline);
   }
   void on_case_filtered()
   {
   }
   void on_case_passed(long duration)
   {
      fprintf(filp, "<td>%s</td><td>%s</td><td>Passed</td><td> %ld</td>", unitname, casename, duration);
   }
   void on_case_leaked(int leakbytes)
   {
      fprintf(filp, "<td>%s</td><td>%s</td><td>Memory Leaked %d bytes</td><td>%s:%d</td>", unitname, casename, leakbytes, casefile, caseline);
   }
   void on_case_leakat(int leakbytes, const char* leakfile, int leakline)
   {
      //fprintf(filp, "  Leaked %d bytes at %s:%d\n", leakbytes, leakfile, leakline);
   }
   void on_case_failed()
   {
      fprintf(filp, "<td>%s</td><td>%s</td><td>Failed</td><td>%s:%d</td>", unitname, casename, casefile, caseline);
   }
   void on_case_failed_check(const char* condition, bool result)
   {
      on_case_failed();
      fprintf(filp, "<td>H2CHECK(%s)</td>", condition);
   }
   void on_case_failed_integer(int expected, int actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "expected is <%d 0x%x>", expected, expected);
      fprintf(filp, "actually is <%d 0x%x>", actual, actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_double(double expected, double actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%f>\n", expected);
      fprintf(filp, "  actually is <%f>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_strcmp(char* expected, char* actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s>\n", expected);
      fprintf(filp, "  actually is <%s>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_strcmp_nocase(char* expected, char* actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s>\n", expected);
      fprintf(filp, "  actually is <%s>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_regex(char* express, char* actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s>\n", express);
      fprintf(filp, "  actually is <%s>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_memcmp(unsigned char* expected, unsigned char* actual, int length)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is [");
      for (int i = 0; i < length; i++) {
         fprintf(filp, "%02X", expected[i]);
      }
      fprintf(filp, "]\n");

      fprintf(filp, "  actually is [");
      for (int i = 0; i < length; i++) {
         fprintf(filp, "%02X", actual[i]);
      }
      fprintf(filp, "]\n");
      fprintf(filp, "</td>");
   }
   void on_case_failed_catch(const char* expected, const char* actual, const char* exceptype)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s %s>\n", expected, exceptype);
      fprintf(filp, "  actually is <%s %s>\n", actual, exceptype);
      fprintf(filp, "</td>");
   }
   void on_case_failed_stub(const char* express, const char* reason)
   {

   }
};

class h2unit_listen_xml: public h2unit_listen
{
private:
   FILE *filp;
public:
   h2unit_listen_xml()
   {
   }
   virtual ~h2unit_listen_xml()
   {
   }
   void on_task_start()
   {
      filp = fopen("h2unit_xml.xml", "w");
      fprintf(filp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
   }
   void on_task_endup(int failed, int passed, int ignored, int filtered, int cases, int checks, long duration)
   {
      if (failed > 0) {
         fprintf(filp, "Failed <%d failed, %d passed, %d ignored, %d filtered, %d checks, %ld ms>\n", failed, passed, ignored, filtered, checks, duration);
      } else {
         fprintf(filp, "Passed <%d passed, %d ignored, %d filtered, %d cases, %d checks, %ld ms>\n", passed, ignored, filtered, cases, checks, duration);
      }
      fclose(filp);
   }
   void on_case_start(const char* unitname, const char* casename, const char* casefile, int caseline)
   {
      h2unit_listen::on_case_start(unitname, casename, casefile, caseline);
      fprintf(filp, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" tests=\"%d\" time=\"%d.%03d\" timestamp=\"%s\">\n", 1, unitname, 1, 0, 0, "");
      fprintf(filp, "<properties></properties>");
      fprintf(filp, "<testcase classname=\"%s\" name=\"%s\" time=\"%d.%03d\">\n", unitname, casename, 0, 0);
   }
   void on_case_endup(double percentage)
   {
      h2unit_listen::on_case_endup(percentage);
      fprintf(filp, "</testcase><system-out></system-out><system-err></system-err></testsuite>");
   }
   void on_case_ignored()
   {
      fprintf(filp, "<td>%s</td><td>%s</td><td>Ignored</td><td>%s:%d</td>", unitname, casename, casefile, caseline);
   }
   void on_case_filtered()
   {
   }
   void on_case_passed(long duration)
   {
      fprintf(filp, "<td>%s</td><td>%s</td><td>Passed</td><td> %ld</td>", unitname, casename, duration);
   }
   void on_case_leaked(int leakbytes)
   {
      fprintf(filp, "<td>%s</td><td>%s</td><td>Memory Leaked %d bytes</td><td>%s:%d</td>", unitname, casename, leakbytes, casefile, caseline);
   }
   void on_case_leakat(int leakbytes, const char* leakfile, int leakline)
   {
      //fprintf(filp, "  Leaked %d bytes at %s:%d\n", leakbytes, leakfile, leakline);
   }
   void on_case_failed()
   {
      fprintf(filp, "<failure message=\"%s\"> </failure>", "");
   }
   void on_case_failed_check(const char* condition, bool result)
   {
      on_case_failed();
      fprintf(filp, "<td>H2CHECK(%s)</td>", condition);
   }
   void on_case_failed_integer(int expected, int actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "expected is <%d 0x%x>", expected, expected);
      fprintf(filp, "actually is <%d 0x%x>", actual, actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_double(double expected, double actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%f>\n", expected);
      fprintf(filp, "  actually is <%f>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_strcmp(char* expected, char* actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s>\n", expected);
      fprintf(filp, "  actually is <%s>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_strcmp_nocase(char* expected, char* actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s>\n", expected);
      fprintf(filp, "  actually is <%s>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_regex(char* express, char* actual)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s>\n", express);
      fprintf(filp, "  actually is <%s>\n", actual);
      fprintf(filp, "</td>");
   }
   void on_case_failed_memcmp(unsigned char* expected, unsigned char* actual, int length)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is [");
      for (int i = 0; i < length; i++) {
         fprintf(filp, "%02X", expected[i]);
      }
      fprintf(filp, "]\n");

      fprintf(filp, "  actually is [");
      for (int i = 0; i < length; i++) {
         fprintf(filp, "%02X", actual[i]);
      }
      fprintf(filp, "]\n");
      fprintf(filp, "</td>");
   }
   void on_case_failed_catch(const char* expected, const char* actual, const char* exceptype)
   {
      on_case_failed();
      fprintf(filp, "<td>");
      fprintf(filp, "  expected is <%s %s>\n", expected, exceptype);
      fprintf(filp, "  actually is <%s %s>\n", actual, exceptype);
      fprintf(filp, "</td>");
   }
   void on_case_failed_stub(const char* express, const char* reason)
   {

   }
};

class h2unit_task
{
public:
   int case_count, check_count;
   int case_failed, case_passed, case_ignore, case_filter;
   h2unit_leak* leak_list;
   h2unit_stub* stub_list;
   h2unit_case* case_list;
   unsigned limited;

   h2unit_listen_text text_listener;
   h2unit_listen_console console_listener;
   h2unit_listen_html html_listener;
   h2unit_listen_xml xml_listener;
   h2unit_listens listener;
   h2unit_task()
   {
      case_failed = case_passed = case_ignore = case_filter = 0;
      case_count = check_count = 0;
      case_list = NULL;
      stub_list = NULL;
      leak_list = NULL;
      limited = 0x7fffffff;

      listener.attach(&console_listener);
   }

   static h2unit_task* O()
   {
      static h2unit_task _instance;
      return &_instance;
   }

   void install(h2unit_case* testcase)
   {
      testcase->_next_ = case_list;
      case_list = testcase;
      case_count++;
   }

   void reorder()
   {
      h2unit_case* p = case_list;
      case_list = NULL;
      srandom(__milliseconds());
      for (int i = 1; p; i++) {
         h2unit_case* n = p->_next_;

         int rpos = random() % i;
         h2unit_case** ip = &case_list;
         for (int j = 0; j < rpos; j++) {
            ip = &((*ip)->_next_);
         }
         p->_next_ = *ip;
         *ip = p;

         p = n;
      }
   }

   void run()
   {
      long total_starts = __milliseconds();
      if (cfg._random) reorder();
      listener.on_task_start();
      for (h2unit_case* p = case_list; (h2unit_case::_current_ = p); p = p->_next_) {
         listener.on_case_start(p->_unitname_, p->_casename_, p->_casefile_, p->_caseline_);
         long case_starts = __milliseconds();
         p->_execute_();
         switch (p->_status_) {
         case h2unit_case::_IGNORE_:
            case_ignore++;
            listener.on_case_ignored();
            break;
         case h2unit_case::_FILTER_:
            case_filter++;
            break;
         case h2unit_case::_PASSED_:
            case_passed++;
            listener.on_case_passed(__milliseconds() - case_starts);
            break;
         case h2unit_case::_FAILED_:
            case_failed++;
            break;
         case h2unit_case::_LEAKED_:
            case_failed++;
            listener.on_case_leaked(p->_leakbytes_);
            for (h2unit_leak* leak = leak_list; leak; leak = leak->next) {
               if (leak->owner == p) {
                  listener.on_case_leakat(leak->length, leak->file, leak->line);
               }
            }
            break;
         }
         listener.on_case_endup(100.0 * (case_passed + case_failed + case_ignore + case_filter) / case_count);
      }
      listener.on_task_endup(case_failed, case_passed, case_ignore, case_filter, case_count, check_count, __milliseconds() - total_starts);
   }

   bool enough(size_t size)
   {
      return (size <= limited);
   }

   h2unit_leak* add_leak(void* data, int length, const char* file, int line, h2unit_case* testcase)
   {
      h2unit_leak* leak = (h2unit_leak*) malloc(sizeof(h2unit_leak));
      if (leak) {
         leak->data = data;
         leak->length = length;
         leak->file = file;
         leak->line = line;
         leak->owner = testcase;
         leak->next = leak_list;
         leak_list = leak;
         limited -= length;
      }
      return leak;
   }

   h2unit_leak* get_leak(void* ptr)
   {
      h2unit_leak* leak;
      for (leak = leak_list; leak; leak = leak->next) {
         if (leak->data == ptr) return leak;
      }
      return NULL;
   }

   void del_leak(h2unit_leak* leak)
   {
      h2unit_leak** ip;
      for (ip = &leak_list; *ip; ip = &((*ip)->next)) {
         if (*ip == leak) {
            *ip = (*ip)->next;
            limited += leak->length;
            free(leak);
            return;
         }
      }
   }

   h2unit_stub* add_stub(void* orig)
   {
      h2unit_stub* stub = (h2unit_stub*) malloc(sizeof(h2unit_stub));
      if (stub) {
         stub->orig = orig;
         stub->next = stub_list;
         stub_list = stub;
         memcpy(stub->code, orig, sizeof(void*) + 4);
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
         memcpy(stub->orig, stub->code, sizeof(void*) + 4);
         free(stub);
      }
   }
};

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

h2unit_case* h2unit_case::_current_ = NULL;

h2unit_case::h2unit_case()
{
   _unitname_ = "";
   _casename_ = "";
   _casefile_ = "";
   _caseline_ = 0;
   _status_ = _INITED_;
   _leakbytes_ = 0;

   h2unit_task::O()->install(this);
}

h2unit_case::~h2unit_case()
{
}

void h2unit_case::_init_(const char* unitname, const char* casename, bool ignored, const char* file, int line)
{
   _unitname_ = unitname;
   _casename_ = casename;
   _casefile_ = file;
   _caseline_ = line;
   if (ignored) _status_ = _IGNORE_;
}

void h2unit_case::_prev_setup_()
{
   _leakbytes_ = 0;
   h2unit_leak* leak;
   for (leak = h2unit_task::O()->leak_list; leak; leak = leak->next) {
      _leakbytes_ += leak->length;
   }
}

void h2unit_case::_post_teardown_()
{
   /* balance test environment automatically */
   _limit_(0x7fffffff);
   h2unit_task::O()->del_stubs();

   /* memory leak detection */
   long after = 0;
   h2unit_leak* leak;
   for (leak = h2unit_task::O()->leak_list; leak; leak = leak->next) {
      after += leak->length;
   }
   _leakbytes_ = after - _leakbytes_;
   if (_leakbytes_) _status_ = _LEAKED_;
}

void h2unit_case::setup()
{
}

void h2unit_case::teardown()
{
}

void h2unit_case::_execute_()
{
   if (cfg._filter != NULL && __pattern_cmp(cfg._filter, (char*) _unitname_) != 0) {
      _status_ = _FILTER_;
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
}

void h2unit_case::_limit_(unsigned long bytes)
{
   h2unit_task::O()->limited = bytes;
}

void h2unit_case::_stub_(void* orig, void* fake, const char* express)
{
   char reason[128];
   unsigned char *p = (unsigned char*) orig;
#ifdef _WIN32
   DWORD saved;
   if (!VirtualProtect(orig, sizeof(void*) + 4, PAGE_WRITECOPY, &saved)) { //PAGE_EXECUTE_WRITECOPY
      sprintf(reason, "VirtualProtect:%d", GetLastError());
      goto failure;
   }
#else
   int pagesize = sysconf(_SC_PAGE_SIZE);
   if (mprotect((void*) ((unsigned long) orig & (~(pagesize - 1))), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
      sprintf(reason, "mprotect:%s", strerror(errno));
      goto failure;
   }
#endif
   if (!h2unit_task::O()->get_stub(orig)) {
      if (!h2unit_task::O()->add_stub(orig)) {
         sprintf(reason, "out of memory");
         goto failure;
      }
   }
   //I32 __asm("jmp $fake") : 0xE9 {fake-orig-5}
   //I32 __asm("movl $fake, %eax; jmpl %eax") : 0xB8 {fake} 0xFF 0xE0
   //I32_64 __asm("movq $fake, %rax; jmpq %rax") : 0x48 0xB8 {fake} 0xFF 0xE0
#ifdef __x86_64__
   *p++ = 0x48;
   *p++ = 0xB8;
   memcpy(p, &fake, sizeof(void*));
   p += sizeof(void*);
   *p++ = 0xFF;
   *p++ = 0xE0;
#else
   *p++ = 0xE9;
   fake = (void*) ((unsigned long) fake - (unsigned long) orig - (sizeof(void*) + 1));
   memcpy(p, &fake, sizeof(void*));
#endif

   return;

   failure: h2unit_task::O()->listener.on_case_failed_stub(express, reason);
   throw _fail;
}

void h2unit_case::_enter_check_(const char* file, int line)
{
   h2unit_task::O()->check_count++;
   h2unit_task::O()->listener.on_check_enter(file, line);
}

void h2unit_case::_check_true_(const char* condition, bool result)
{
   if (!result) {
      h2unit_task::O()->listener.on_case_failed_check(condition, result);
      throw _fail;
   }
}

void h2unit_case::_check_equal_(int expected, int actual)
{
   if (expected != actual) {
      h2unit_task::O()->listener.on_case_failed_integer(expected, actual);
      throw _fail;
   }
}

void h2unit_case::_check_equal_(double expected, double actual, double threshold)
{
   double delta = expected - actual;
   if (delta < 0) delta = -delta;
   if (delta > threshold) {
      h2unit_task::O()->listener.on_case_failed_double(expected, actual);
      throw _fail;
   }
}

void h2unit_case::_check_equal_(char* expected, char* actual)
{
   if (strcmp(expected, actual) != 0) {
      h2unit_task::O()->listener.on_case_failed_strcmp(expected, actual);
      throw _fail;
   }
}

void h2unit_case::_check_equal_strcmp_nocase_(char* expected, char* actual)
{
   if (strcasecmp(expected, actual) != 0) {
      h2unit_task::O()->listener.on_case_failed_strcmp_nocase(expected, actual);
      throw _fail;
   }
}

void h2unit_case::_check_regex_(char* express, char* actual)
{
   if (__pattern_cmp(express, actual) != 0) {
      h2unit_task::O()->listener.on_case_failed_regex(express, actual);
      throw _fail;
   }
}

void h2unit_case::_check_equal_(unsigned char* expected, unsigned char* actual, int length)
{
   if (memcmp(expected, actual, length) != 0) {
      h2unit_task::O()->listener.on_case_failed_memcmp(expected, actual, length);
      throw _fail;
   }
}

void h2unit_case::_check_catch_(const char* expected, const char* actual, const char* exceptype)
{
   if (expected != actual) {
      h2unit_task::O()->listener.on_case_failed_catch(expected, actual, exceptype);
      throw _fail;
   }
}

void* h2unit_malloc(size_t size, const char* file, int line)
{
   if (!h2unit_task::O()->enough(size)) return NULL;
   void* data = malloc(size);
   if (data == NULL) return NULL;
   h2unit_task::O()->add_leak(data, size, file, line, h2unit_case::_current_);
   return data;
}

void* h2unit_calloc(size_t nmemb, size_t size, const char* file, int line)
{
   if (!h2unit_task::O()->enough(nmemb * size)) return NULL;
   void* data = calloc(nmemb, size);
   if (data == NULL) return NULL;
   h2unit_task::O()->add_leak(data, nmemb * size, file, line, h2unit_case::_current_);
   return data;
}

void* h2unit_realloc(void* ptr, size_t size, const char* file, int line)
{
   if (!ptr) {
      return h2unit_malloc(size, file, line);
   }
   if (!size) {
      h2unit_free(ptr, file, line);
      return NULL;
   }
   h2unit_leak* leak = h2unit_task::O()->get_leak(ptr);
   if (leak) {
      if (!h2unit_task::O()->enough(size)) return NULL;
      void* data = realloc(leak->data, size);
      if (data == NULL) return NULL;
      h2unit_task::O()->add_leak(data, size, file, line, h2unit_case::_current_);
      h2unit_task::O()->del_leak(leak);
      return data;
   }
   return NULL;
}

void h2unit_free(void* ptr, const char* file, int line)
{
   h2unit_leak* leak = h2unit_task::O()->get_leak(ptr);
   if (leak) h2unit_task::O()->del_leak(leak);
}

char* h2unit_strdup(const char* s, const char* file, int line)
{
   size_t size = strlen(s) + 1;
   if (!h2unit_task::O()->enough(size)) return NULL;
   char* data = strdup(s);
   if (data == NULL) return NULL;
   h2unit_task::O()->add_leak(data, size, file, line, h2unit_case::_current_);
   return data;
}

char* h2unit_strndup(const char* s, size_t n, const char* file, int line)
{
   size_t len = strlen(s);
   len = (len > n ? n : len) + 1;
   if (!h2unit_task::O()->enough(len)) return NULL;
   char* data = (char*) malloc(len);
   if (data == NULL) return NULL;
   data[len - 1] = '\0';
   memcpy(data, s, len - 1);
   h2unit_task::O()->add_leak(data, len, file, line, h2unit_case::_current_);
   return data;
}

void* operator new(size_t size, const char* file, int line)
{
   return h2unit_malloc(size, file, line);
}

void* operator new[](size_t size, const char* file, int line)
{
   return h2unit_malloc(size, file, line);
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
   if (argc > 1) {
      if (strstr(argv[1], "v")) cfg._verbose = true;
      if (strstr(argv[1], "c")) cfg._colored = true;
      if (strstr(argv[1], "r")) cfg._random = true;
      if (strstr(argv[1], "u")) cfg._filter = argv[2];
      if (strstr(argv[1], "t")) h2unit_task::O()->listener.attach(&h2unit_task::O()->text_listener);
      if (strstr(argv[1], "h")) h2unit_task::O()->listener.attach(&h2unit_task::O()->html_listener);
      if (strstr(argv[1], "x")) h2unit_task::O()->listener.attach(&h2unit_task::O()->xml_listener);
   }
   h2unit_task::O()->run();
   return 0;
}

