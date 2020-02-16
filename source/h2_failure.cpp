
#define H2_FAIL_FOREACH(f, First)                                 \
   for (h2_fail* x_fail = First; x_fail; x_fail = x_fail->x_next) \
      for (h2_fail* f = x_fail; f; f = f->y_next)

#define H2_XPRINTF(format, value)            \
   do {                                      \
      char t[1024 * 8];                      \
      va_list args;                          \
      va_start(args, format);                \
      vsnprintf(t, sizeof(t), format, args); \
      va_end(args);                          \
      value = value + t;                     \
   } while (0)

struct h2_fail {
   h2_fail *x_next, *y_next;

   const char* file;
   int line;

   const char* func;
   int argi;

   h2_string _k;

   static constexpr const char* A9 = "1st\0002nd\0003rd\0004th\0005th\0006th\0007th\0008th\0009th";

   h2_fail(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1)
     : x_next(nullptr), y_next(nullptr), file(file_), line(line_), func(func_), argi(argi_) {}

   virtual ~h2_fail() {
      if (y_next) delete y_next;
      if (x_next) delete x_next;
   };

   void locate(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1) {
      file = file_, line = line_, func = func_, argi = argi_;
      if (y_next) y_next->locate(file_, line_, func_, argi_);
   }

   void kprintf(const char* format, ...) { H2_XPRINTF(format, _k); }

   virtual void print() { _k.size() && printf(" %s", _k.c_str()); }

   void print_locate() {
      if (func && strlen(func)) printf(", in %s(%s)", func, 0 <= argi && argi < 9 ? A9 + argi * 4 : "?");
      if (file && strlen(file) && 0 < line) printf(", at %s:%d", file, line);
      printf("\n");
   }

   virtual void print(FILE* fp) { fprintf(fp, "%s", _k.c_str()); }

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
};

static inline void h2_append_x_fail(h2_fail*& fail, h2_fail* n) {
   if (!fail) {
      fail = n;
   } else {
      h2_fail** pp = &fail->x_next;
      while (*pp) pp = &(*pp)->x_next;
      *pp = n;
   }
}

static inline void h2_append_y_fail(h2_fail*& fail, h2_fail* n) {
   if (!fail) {
      fail = n;
   } else {
      h2_fail** pp = &fail->y_next;
      while (*pp) pp = &(*pp)->y_next;
      *pp = n;
   }
}

struct h2_fail_normal : public h2_fail {
   h2_fail_normal(const char* file_ = nullptr, int line_ = 0, const char* func_ = nullptr, const char* format = "", ...)
     : h2_fail(file_, line_, func_) { H2_XPRINTF(format, _k); }

   void print() { h2_fail::print(), print_locate(); }
};

struct h2_fail_unexpect : public h2_fail {
   h2_string _e, _a, _h, _m, _t;

   h2_fail_unexpect(const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_) {}

   void hprintf(const char* format, ...) { H2_XPRINTF(format, _h); }
   void eprintf(const char* format, ...) { H2_XPRINTF(format, _e); }
   void mprintf(const char* format, ...) { H2_XPRINTF(format, _m); }
   void aprintf(const char* format, ...) { H2_XPRINTF(format, _a); }
   void tprintf(const char* format, ...) { H2_XPRINTF(format, _t); }

   void print() {
      h2_fail::print(); /* nothing */
      printf(" %s%s%s%s %s %s%s%s%s",
             _h.c_str(),
             h2cfg().style("bold,red"), _a.c_str(), h2cfg().style("reset"),
             _m.c_str(),
             h2cfg().style("green"), _e.c_str(), h2cfg().style("reset"),
             _t.c_str());
      print_locate();
   }

   void print(FILE* fp) {
      h2_fail::print(fp);
      fprintf(fp, " %s %s %s %s %s ", _h.c_str(), _a.c_str(), _m.c_str(), _e.c_str(), _t.c_str());
   }
};

struct h2_fail_strcmp : public h2_fail {
   const h2_string e, a;
   const bool caseless;

   h2_fail_strcmp(const char* expect, const char* actual, bool caseless_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e(expect), a(actual), caseless(caseless_) { kprintf("String not %sequals", caseless_ ? "case-insensitive " : ""); }

   void print() {
      h2_fail::print(), print_locate();

      int columns = h2cfg().get_term_columns() - 12;
      int rows = H2_DIV_ROUND_UP(std::max(e.length(), a.length()), columns);

      for (int i = 0; i < rows; ++i) {
         char eline[1024], aline[1024], *ep = eline, *ap = aline;
         ep += sprintf(ep, "%sexpect%s>%s ", h2cfg().style("dark gray"), h2cfg().style("green"), h2cfg().style("reset"));
         ap += sprintf(ap, "%sactual%s> ", h2cfg().style("dark gray"), h2cfg().style("reset"));
         for (int j = 0; j < columns; ++j) {
            char _e = i * columns + j < (int)e.length() ? e[i * columns + j] : ' ';
            char _a = i * columns + j < (int)a.length() ? a[i * columns + j] : ' ';

            bool eq = caseless ? ::tolower(_e) == ::tolower(_a) : _e == _a;
            ep += sprintf(ep, "%s%c%s", eq ? "" : h2cfg().style("green"), _e, eq ? "" : h2cfg().style("reset"));
            ap += sprintf(ap, "%s%c%s", eq ? "" : h2cfg().style("red,bold"), _a, eq ? "" : h2cfg().style("reset"));
         }
         printf("%s\n%s\n", eline, aline);
      }
   }
};

struct h2_fail_memcmp : public h2_fail {
   h2_vector<unsigned char> e, a;
   const void* p;

   h2_fail_memcmp(const void* expect, const void* actual, int len, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e((unsigned char*)expect, ((unsigned char*)expect) + len), a((unsigned char*)actual, ((unsigned char*)actual) + len), p(actual) {
      kprintf("Memory %p binary %d bytes not equal", p, len);
   }

   void print() {
      h2_fail::print(), print_locate();

      printf("                     %sexpect%s                       %s│%s                       %sactual%s \n",
             h2cfg().style("dark gray"), h2cfg().style("reset"), h2cfg().style("dark gray"), h2cfg().style("reset"), h2cfg().style("dark gray"), h2cfg().style("reset"));

      int size = e.size();
      int rows = H2_DIV_ROUND_UP(size, 16);

      for (int i = 0; i < rows; ++i) {
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) printf("%s", h2cfg().style("green"));
            printf(j < 8 ? "%02X " : " %02X", e[i * 16 + j]);
            printf("%s", h2cfg().style("reset"));
         }
         printf("  %s│%s  ", h2cfg().style("dark gray"), h2cfg().style("reset"));
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) printf("%s", h2cfg().style("bold,red"));
            printf(j < 8 ? "%02X " : " %02X", a[i * 16 + j]);
            printf("%s", h2cfg().style("reset"));
         }
         printf("\n");
      }
   }
};

struct h2_fail_memoverflow : public h2_fail {
   const unsigned char* ptr;
   const int offset;
   const unsigned char* magic;
   const h2_vector<unsigned char> spot;
   const h2_backtrace bt0, bt1;

   h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), ptr((unsigned char*)ptr_), offset(offset_), magic(magic_), spot(((unsigned char*)ptr_) + offset_, ((unsigned char*)ptr_) + offset_ + size), bt0(bt0_), bt1(bt1_) {
      kprintf(" Memory overflow malloc %p %+d (%p) ", ptr, offset, ptr + offset);
   }

   void print() {
      h2_fail::print();

      for (size_t i = 0; i < spot.size(); ++i)
         printf("%s%02X %s", magic[i] == spot[i] ? h2cfg().style("green") : h2cfg().style("bold,red"), spot[i], h2cfg().style("reset"));

      print_locate();
      if (0 < bt1.count) printf("  %p trampled at backtrace:\n", ptr + offset), bt1.print();
      if (0 < bt0.count) printf("  which allocated at backtrace:\n"), bt0.print();
   }
};

struct h2_fail_memleak : public h2_fail {
   const char* where;
   struct A {
      void *ptr, *ptr2;
      int size, size2, bytes, times;
      h2_backtrace bt;
      A(void* ptr_, int size_, h2_backtrace& bt_) : ptr(ptr_), ptr2(nullptr), size(size_), size2(0), bytes(size_), times(1), bt(bt_) {}
   };
   h2_vector<A> leaks;
   long long bytes, times, places;

   h2_fail_memleak(const char* file_ = nullptr, int line_ = 0, const char* where_ = "")
     : h2_fail(file_, line_), where(where_), bytes(0), times(0), places(0) {}

   void add(void* ptr, int size, h2_backtrace& bt) {
      bytes += size;
      times += 1;
      for (auto c : leaks)
         if (c.bt == bt) {
            c.ptr2 = ptr;
            c.size2 = size;
            c.bytes += size;
            c.times += 1;
            return;
         }
      places += 1;
      leaks.push_back(A(ptr, size, bt));
   }

   void print() {
      char t1[64] = "", t2[64] = "";
      if (1 < places) sprintf(t1, "%lld places ", places);
      if (1 < times) sprintf(t2, "%lld times ", times);

      kprintf("Memory Leaked %s%s%lld bytes in %s totally", t1, t2, bytes, where);
      h2_fail::print(), print_locate();
      for (auto c : leaks) {
         char t3[64] = " ", t4[64] = "", t5[64] = "";
         if (1 < c.times) sprintf(t3, ", %p ... ", c.ptr2);
         if (1 < c.times) sprintf(t4, "%d times ", c.times);
         if (1 < c.times) sprintf(t5, " (%d, %d ...)", c.size, c.size2);
         printf("  %p%sLeaked %s%d bytes%s, at backtrace\n", c.ptr, t3, t4, c.bytes, t5);
         c.bt.print();
      }
   }
};

struct h2_fail_doublefree : public h2_fail {
   const h2_backtrace bt0, bt1;

   h2_fail_doublefree(void* ptr_, h2_backtrace& bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), bt0(bt0_), bt1(bt1_) { kprintf("%p double freed", ptr_); }

   void print() {
      h2_fail::print(), printf(" at backtrace:\n");
      bt1.print();
      if (0 < bt0.count) printf("  which allocated at backtrace:\n"), bt0.print();
   }
};

struct h2_fail_json : public h2_fail {
   const h2_string e, a;

   h2_fail_json(const char* k, const char* expect, const char* actual, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e(expect), a(actual) { _k = k; }

   void print() {
      h2_fail::print(), print_locate();

      int terminal_columns = h2cfg().get_term_columns();
      if (terminal_columns < 10) terminal_columns = 80;
      h2_json::diff_print(e.c_str(), a.c_str(), terminal_columns);
   }
};

struct h2_fail_instantiate : public h2_fail {
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   const int why;

   h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_, const char* file_, int line_)
     : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why(why_) {
      if (why == 1)
         kprintf("Instantiate 'class %s' is a abstract class", class_type);
      else if (why == 2)
         kprintf("Instantiate 'class %s' don't know initialize arguments", class_type);
   }

   void print() {
      h2_fail::print(), print_locate();

      printf("You may take following solutions to fix it: \n");
      if (why == 1) {
         printf("1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s%s, Derived_%s(...)%s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                h2cfg().style("bold,yellow"),
                class_type,
                h2cfg().style("reset"));
      } else if (why == 2) {
         printf("1. Define default constructor in class %s, or \n", class_type);
         printf("2. Add parameterized construction in %s(%s%s%s, %s, %s%s, %s(...)%s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                h2cfg().style("bold,yellow"),
                class_type,
                h2cfg().style("reset"));
      }
   }
};
