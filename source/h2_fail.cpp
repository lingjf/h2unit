#define H2_FAIL_FOREACH(f, First)                                 \
   for (h2_fail* x_fail = First; x_fail; x_fail = x_fail->x_next) \
      for (h2_fail* f = x_fail; f; f = f->y_next)

#define H2_XPRINTF(format, value)         \
   char t[1024 * 8];                      \
   va_list args;                          \
   va_start(args, format);                \
   vsnprintf(t, sizeof(t), format, args); \
   va_end(args);                          \
   value = value + t;

class h2_fail {
 public:
   h2_fail *x_next, *y_next;

 protected:
   const char* file;
   int line;

   const char* func;
   int argi;

   h2_string _k;

 public:
   h2_fail(const char* file_, int line_) : x_next(nullptr), y_next(nullptr), file(file_), line(line_), func(nullptr), argi(-1) {}
   virtual ~h2_fail() {
      if (y_next) delete y_next;
      if (x_next) delete x_next;
   };

   void locate(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1) {
      file = file_, line = line_, func = func_, argi = argi_;
      if (y_next) y_next->locate(file_, line_, func_, argi_);
   }

   void kprintf(const char* format, ...) { H2_XPRINTF(format, _k); }

   virtual void print() {
      if (0 < _k.size()) {
         printf(" %s", _k.c_str());
         print_locate();
         printf("\n");
      }
   }

   void print_locate() {
      if (func && strlen(func)) printf(", in %s(%s)", func, 0 <= argi && argi < 9 ? (const char*)"1st\0002nd\0003rd\0004th\0005th\0006th\0007th\0008th\0009th" + argi * 4 : "?");
      if (file && strlen(file) && 0 < line) printf(", at %s:%d", file, line);
   }

   virtual void print(FILE* fp) {}

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

class h2_fail_normal : public h2_fail {
 public:
   h2_fail_normal(const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_) {}

   void k(h2_string key) { _k = key; }

   void print(FILE* fp) {
      fprintf(fp, "normal");
   }
};

class h2_fail_unexpect : public h2_fail {
 private:
   h2_string _e, _a, _h, _m, _t;

 public:
   h2_fail_unexpect(const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_) {}

   void hamet(h2_string head, h2_string actual, h2_string middle, h2_string expect, h2_string tail) {
      _h = head, _a = actual, _m = middle, _e = expect, _t = tail;
   }

   void hprintf(const char* format, ...) { H2_XPRINTF(format, _h); }
   void eprintf(const char* format, ...) { H2_XPRINTF(format, _e); }
   void mprintf(const char* format, ...) { H2_XPRINTF(format, _m); }
   void aprintf(const char* format, ...) { H2_XPRINTF(format, _a); }
   void tprintf(const char* format, ...) { H2_XPRINTF(format, _t); }

   void print() {
      h2_fail::print();
      printf(" %s%s%s%s %s %s%s%s%s",
             _h.c_str(),
             cfg().style("bold,red"), _a.c_str(), cfg().style("reset"),
             _m.c_str(),
             cfg().style("green"), _e.c_str(), cfg().style("reset"),
             _t.c_str());
      print_locate();
      printf("\n");
   }

   void print(FILE* fp) {
      fprintf(fp, " %s %s %s %s %s ", _h.c_str(), _a.c_str(), _m.c_str(), _e.c_str(), _t.c_str());
   }
};

class h2_fail_strcmp : public h2_fail {
 private:
   h2_string e, a;
   bool caseless;

 public:
   h2_fail_strcmp(const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_) {}

   void add(const char* expect, const char* actual, bool caseless) {
      e = expect;
      a = actual;
      this->caseless = caseless;
      kprintf("String not %sequals", caseless ? "case-insensitive " : "");
   }

   void get(int i, char& e0, char& e1, char& a0, char& a1) {
      e0 = i < (int)e.length() ? e[i] : ' ';
      e1 = ::tolower(e0);
      a0 = i < (int)a.length() ? a[i] : ' ';
      a1 = ::tolower(a0);
   }

   void print() {
      h2_fail::print();

      int columns = cfg().get_term_columns() - 12;
      int rows = H2_DIV_ROUND_UP((int)std::max(e.length(), a.length()), columns);

      for (int i = 0; i < rows; ++i) {
         printf("%sexpect%s>%s ", cfg().style("dark gray"), cfg().style("green"), cfg().style("reset"));
         for (int j = 0; j < columns; ++j) {
            char e0, e1, a0, a1;
            get(i * columns + j, e0, e1, a0, a1);
            bool eq = caseless ? e1 == a1 : e0 == a0;
            if (!eq) printf("%s", cfg().style("green"));
            printf("%c", e0);
            if (!eq) printf("%s", cfg().style("reset"));
         }
         printf("\n");
         printf("%sactual%s> ", cfg().style("dark gray"), cfg().style("reset"));
         for (int j = 0; j < columns; ++j) {
            char e0, e1, a0, a1;
            get(i * columns + j, e0, e1, a0, a1);
            bool eq = caseless ? e1 == a1 : e0 == a0;
            if (!eq) printf("%s", cfg().style("red,bold"));
            printf("%c", a0);
            if (!eq) printf("%s", cfg().style("reset"));
         }
         printf("\n");
      }
   }

   void print(FILE* fp) {
      fprintf(fp, "strcmp failed");
   }
};

class h2_fail_memcmp : public h2_fail {
 private:
   const void* p;
   h2_vector<unsigned char> e, a;

 public:
   h2_fail_memcmp(const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_) {}

   void add(const void* expect, const void* actual, const int len) {
      p = actual;
      e.assign((unsigned char*)expect, ((unsigned char*)expect) + len);
      a.assign((unsigned char*)actual, ((unsigned char*)actual) + len);
      kprintf("Memory %p binary %d bytes not equal", p, len);
   }

   void print() {
      h2_fail::print();

      printf("                     %sexpect%s                       %s│%s                       %sactual%s \n",
             cfg().style("dark gray"), cfg().style("reset"),
             cfg().style("dark gray"), cfg().style("reset"),
             cfg().style("dark gray"), cfg().style("reset"));

      int size = (int)e.size();
      int rows = H2_DIV_ROUND_UP(size, 16);

      for (int i = 0; i < rows; ++i) {
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) printf("%s", cfg().style("green"));
            printf(j < 8 ? "%02X " : " %02X", e[i * 16 + j]);
            printf("%s", cfg().style("reset"));
         }
         printf("  %s│%s  ", cfg().style("dark gray"), cfg().style("reset"));
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) printf("%s", cfg().style("bold,red"));
            printf(j < 8 ? "%02X " : " %02X", a[i * 16 + j]);
            printf("%s", cfg().style("reset"));
         }
         printf("\n");
      }
   }
   void print(FILE* fp) {
      fprintf(fp, "Memory %p compare failed", p);
   }
};

class h2_fail_memoverflow : public h2_fail {
 private:
   const unsigned char* ptr;
   int offset;
   const unsigned char* magic;
   h2_vector<unsigned char> spot;
   h2_backtrace bt0, bt1;

 public:
   h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace* bt0_, h2_backtrace* bt1_, const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_), ptr((const unsigned char*)ptr_), offset(offset_), magic(magic_) {
      spot.assign(((unsigned char*)ptr_) + offset_, ((unsigned char*)ptr_) + offset_ + size);
      if (bt0_) bt0 = *bt0_;
      if (bt1_) bt1 = *bt1_;
   }

   void print() {
      h2_fail::print();
      printf(" Memory overflow malloc %p %+d (%p) ", ptr, offset, ptr + offset);

      for (int i = 0; i < (int)spot.size(); ++i) {
         printf("%s", magic[i] == spot[i] ? cfg().style("green") : cfg().style("bold,red"));
         printf("%02X ", spot[i]);
         printf("%s", cfg().style("reset"));
      }
      print_locate();
      printf("\n");
      if (0 < bt1.count) printf("  %p trampled at backtrace:\n", ptr + offset), bt1.print();
      if (0 < bt0.count) printf("  which allocated at backtrace:\n"), bt0.print();
   }

   void print(FILE* fp) {
      fprintf(fp, "Memory overflow malloc %p %+d  ", ptr, offset);
   }
};

class h2_fail_memleak : public h2_fail {
 private:
   const char* where;
   struct A {
      void* ptr;
      int size, bytes, times;
      h2_backtrace bt;
      A(void* ptr_, int size_, h2_backtrace& bt_) : ptr(ptr_), size(size_), bytes(size_), times(1), bt(bt_) {}
   };
   h2_vector<A> leaks;
   long long bytes;
   int places;

 public:
   h2_fail_memleak(const char* file_ = nullptr, int line_ = 0, const char* where_ = "") : h2_fail(file_, line_), where(where_), bytes(0), places(0) {}

   void add(void* ptr, int size, h2_backtrace& bt) {
      bytes += size;
      places += 1;
      for (auto i = leaks.begin(); i != leaks.end(); i++) {
         if ((*i).bt == bt) {
            (*i).bytes += size;
            (*i).times += 1;
            return;
         }
      }
      leaks.push_back(A(ptr, size, bt));
   }

   void print() {
      kprintf("Memory Leaked %s%lld bytes in %s totally", str_places(places), bytes, where);
      h2_fail::print();
      for (auto i = leaks.begin(); i != leaks.end(); i++) {
         auto a = *i;
         printf("  %p", a.ptr);
         a.times <= 1 ? printf(" ") : printf("... ");
         printf("Leaked ");
         if (a.times > 1) printf("%d times ", a.times);
         printf("%d", a.bytes);
         a.times <= 1 ? printf(" ") : printf("(%d+...)", a.size);
         printf("bytes, at backtrace\n");
         a.bt.print();
      }
   }

   const char* str_places(int n) {
      if (n <= 1) return "";
      static char t[32];
      sprintf(t, "%d places ", n);
      return t;
   }

   void print(FILE* fp) {
      fprintf(fp, "Memory Leaked %s%lld bytes in %s totally", str_places(places), bytes, where);
   }
};

class h2_fail_doublefree : public h2_fail {
 private:
   void* ptr;
   h2_backtrace bt0, bt1;

 public:
   h2_fail_doublefree(void* ptr_, h2_backtrace& bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_), ptr(ptr_), bt0(bt0_), bt1(bt1_) {}

   void print() {
      kprintf("%p double freed at backtrace:", ptr);
      h2_fail::print();
      bt1.print();
      if (0 < bt0.count) printf("  which allocated at backtrace:\n"), bt0.print();
   }

   void print(FILE* fp) {
      fprintf(fp, "%p double free", ptr);
   }
};

class h2_fail_json : public h2_fail {
 private:
   h2_string e, a;

 public:
   h2_fail_json(const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_) {}

   void add(const char* expect, const char* actual) {
      e = expect;
      a = actual;
   }

   void print() {
      h2_fail::print();

      int terminal_columns = cfg().get_term_columns();
      if (terminal_columns < 10) terminal_columns = 80;
      h2_json::diff_print(e.c_str(), a.c_str(), terminal_columns);
   }

   void print(FILE* fp) {
      fprintf(fp, "JSON compare failed");
   }
};

class h2_fail_instantiate : public h2_fail {
 private:
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   int why;

 public:
   h2_fail_instantiate(const char* file_, int line_, const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_)
     : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why(why_) {
      if (why == 1)
         kprintf("Instantiate 'class %s' is a abstract class", class_type);
      else if (why == 2)
         kprintf("Instantiate 'class %s' don't know initialize arguments", class_type);
   }

   void print() {
      h2_fail::print();

      printf("You may take following solutions to fix it: \n");
      if (why == 1) {
         printf("1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s%s, Derived_%s(...)%s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                cfg().style("bold,yellow"),
                class_type,
                cfg().style("reset"));
      } else if (why == 2) {
         printf("1. Define default constructor in class %s, or \n", class_type);
         printf("2. Add parameterized construction in %s(%s%s%s, %s, %s%s, %s(...)%s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                cfg().style("bold,yellow"),
                class_type,
                cfg().style("reset"));
      }
   }

   void print(FILE* fp) {
      if (why == 1)
         fprintf(fp, "Instantiate 'class %s' is a abstract class", class_type);
      else if (why == 2)
         fprintf(fp, "Instantiate 'class %s' don't know initialize arguments", class_type);
   }
};
