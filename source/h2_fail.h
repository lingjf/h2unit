
#ifndef ___H2_FAIL__H___
#define ___H2_FAIL__H___

#define H2_FAIL_FOREACH(f, First)                                 \
   for (h2_fail* x_fail = First; x_fail; x_fail = x_fail->x_next) \
      for (h2_fail* f = x_fail; f; f = f->y_next)

class h2_fail
{
 public:
   h2_fail *x_next, *y_next;

 protected:
   const char* file;
   int line;
   const char* func;

   int _argth;

   h2_string _k;

 public:
   h2_fail(const char* file_, int line_)
     : x_next(nullptr), y_next(nullptr), file(file_), line(line_), func(nullptr), _argth(-1) {}

   virtual ~h2_fail()
   {
      if (y_next) {
         delete y_next;
      }
      if (x_next) {
         delete x_next;
      }
   };

   void appendx(h2_fail* f)
   {
      if (!x_next) {
         x_next = f;
         return;
      }
      h2_fail* p = x_next;
      while (p->x_next) {
         p = p->x_next;
      }
      p->x_next = f;
   }

   void appendy(h2_fail* f)
   {
      if (!y_next) {
         y_next = f;
         return;
      }
      h2_fail* p = y_next;
      while (p->y_next) {
         p = p->y_next;
      }
      p->y_next = f;
   }

   void locate(const char* file_, int line_, const char* func_)
   {
      file = file_;
      line = line_;
      func = func_;

      if (y_next) {
         y_next->locate(file_, line_, func_);
      }
      if (x_next) {
         x_next->locate(file_, line_, func_);
      }
   }

   void argth(int argth_)
   {
      _argth = argth_;
      if (y_next) {
         y_next->argth(argth_);
      }
   }

   void kprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _k = _k + t;
   }

   virtual void print()
   {
      if (0 < _k.size()) {
         ::printf(" %s", _k.c_str());
         print_locate();
         ::printf("\n");
      }
   }

   void print_locate()
   {
      if (func && strlen(func)) {
         ::printf(", in %s(", func);
         if (0 <= _argth) {
            static const char* argz[] = {"1st", "2nd", "3rd", "4th"};
            if (_argth < 4)
               ::printf("%s", argz[_argth]);
            else
               ::printf("%dth", _argth - 1);
         }
         ::printf(")");
      }
      if (file && strlen(file) && 0 < line) {
         ::printf(", at %s:%d", file, line);
      }
   }

   virtual void print(FILE* fp) {}

   static void* operator new(std::size_t sz)
   {
      return h2_alloc::I().malloc(sz);
   }
   static void operator delete(void* ptr)
   {
      h2_alloc::I().free(ptr);
   }
};

class h2_fail_normal : public h2_fail
{
 public:
   h2_fail_normal(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_normal() {}

   void k(h2_string key)
   {
      _k = key;
   }

   void print(FILE* fp)
   {
      fprintf(fp, "normal");
   }
};

class h2_fail_unexpect : public h2_fail
{
 private:
   h2_string _e, _a, _h, _m, _t;

 public:
   h2_fail_unexpect(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_unexpect() {}

   void hamet(h2_string head, h2_string actual, h2_string middle, h2_string expect, h2_string tail)
   {
      _h = head;
      _a = actual;
      _m = middle;
      _e = expect;
      _t = tail;
   }

   void hprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _h = t;
   }

   void eprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _e = t;
   }

   void mprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _m = t;
   }

   void aprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _a = t;
   }

   void tprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _t = t;
   }

   void print()
   {
      h2_fail::print();
      ::printf(" %s%s%s%s %s %s%s%s%s",
               _h.c_str(),
               h2_cfg::style("bold,red"), _a.c_str(), h2_cfg::style("reset"),
               _m.c_str(),
               h2_cfg::style("green"), _e.c_str(), h2_cfg::style("reset"),
               _t.c_str());
      print_locate();
      ::printf("\n");
   }

   void print(FILE* fp)
   {
      fprintf(fp, " %s %s %s %s %s ", _h.c_str(), _a.c_str(), _m.c_str(), _e.c_str(), _t.c_str());
   }
};

class h2_fail_strcmp : public h2_fail
{
 private:
   h2_string e, a;
   bool caseless;

 public:
   h2_fail_strcmp(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_strcmp() {}

   void add(h2_string expect, h2_string actual, bool caseless)
   {
      e = expect;
      a = actual;
      this->caseless = caseless;
      kprintf("String not %sequals", caseless ? "case-insensitive " : "");
   }

   void get(int i, char& e0, char& e1, char& a0, char& a1)
   {
      if (i < (int)e.length())
         e0 = e[i];
      else
         e0 = ' ';
      e1 = ::tolower(e0);

      if (i < (int)a.length())
         a0 = a[i];
      else
         a0 = ' ';
      a1 = ::tolower(a0);
   }

   void print()
   {
      h2_fail::print();

      int terminal_columns = h2_cfg::get_term_columns();
      int columns = terminal_columns - 12;
      int rows = H2_DIV_ROUND_UP((int)std::max(e.length(), a.length()), columns);

      for (int i = 0; i < rows; ++i) {
         ::printf("%sexpect%s> ", h2_cfg::style("dark gray"), h2_cfg::style("reset"));
         for (int j = 0; j < columns; ++j) {
            char e0, e1, a0, a1;
            get(i * columns + j, e0, e1, a0, a1);
            bool eq = caseless ? e1 == a1 : e0 == a0;
            if (!eq) ::printf("%s", h2_cfg::style("green"));
            ::printf("%c", e0);
            if (!eq) ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("\n");
         ::printf("%sactual%s> ", h2_cfg::style("dark gray"), h2_cfg::style("reset"));
         for (int j = 0; j < columns; ++j) {
            char e0, e1, a0, a1;
            get(i * columns + j, e0, e1, a0, a1);
            bool eq = caseless ? e1 == a1 : e0 == a0;
            if (!eq) ::printf("%s", h2_cfg::style("red,bold"));
            ::printf("%c", a0);
            if (!eq) ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("\n");
      }
   }

   void print(FILE* fp)
   {
      fprintf(fp, "strcmp failed");
   }
};

class h2_fail_memcmp : public h2_fail
{
 private:
   const void* _actual;
   int _size;
   std::vector<unsigned char, h2_allocator<unsigned char>> e;
   std::vector<unsigned char, h2_allocator<unsigned char>> a;

 public:
   h2_fail_memcmp(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_memcmp() {}
   void add(const void* expect, const void* actual, const int len)
   {
      _actual = actual;
      _size = len;
      e.assign((unsigned char*)expect, ((unsigned char*)expect) + len);
      a.assign((unsigned char*)actual, ((unsigned char*)actual) + len);
      kprintf("Memory %p binary %d bytes not equal", actual, len);
   }

   void print()
   {
      h2_fail::print();

      ::printf("                     %sexpect%s                       %s│%s                       %sactual%s \n",
               h2_cfg::style("dark gray"), h2_cfg::style("reset"),
               h2_cfg::style("dark gray"), h2_cfg::style("reset"),
               h2_cfg::style("dark gray"), h2_cfg::style("reset"));

      int size = (int)e.size();
      int rows = (size + 15) / 16;  //H2_DIV_ROUND_UP(size, 16);

      for (int i = 0; i < rows; ++i) {
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               ::printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) {
               ::printf("%s", h2_cfg::style("green"));
            }
            ::printf(j < 8 ? "%02X " : " %02X", e[i * 16 + j]);
            ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("  %s│%s  ", h2_cfg::style("dark gray"), h2_cfg::style("reset"));
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               ::printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) {
               ::printf("%s", h2_cfg::style("bold,red"));
            }
            ::printf(j < 8 ? "%02X " : " %02X", a[i * 16 + j]);
            ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("\n");
      }
   }
   void print(FILE* fp)
   {
      fprintf(fp, "Memory %p compare %d bytes failed", _actual, _size);
   }
};

class h2_fail_memover : public h2_fail
{
 private:
   const void* _ptr;
   int _offset;
   std::vector<unsigned char, h2_allocator<unsigned char>> _a;
   const unsigned char* _magic;
   h2_backtrace bt;

 public:
   h2_fail_memover(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_memover() {}

   void add(void* ptr, int offset, const unsigned char* magic, int size, h2_backtrace& bt)
   {
      _ptr = ptr;
      _offset = offset;
      _magic = magic;
      _a.assign(((unsigned char*)ptr) + offset, ((unsigned char*)ptr) + offset + size);
      this->bt = bt;
   }

   void print()
   {
      h2_fail::print();
      ::printf(" Memory overflow malloc %p %+d  ", _ptr, _offset);

      for (int i = 0; i < (int)_a.size(); ++i) {
         if (_magic[i] == _a[i]) {
            ::printf("%s", h2_cfg::style("green"));
         }
         else {
            ::printf("%s", h2_cfg::style("bold,red"));
         }
         ::printf("%02X ", _a[i]);
         ::printf("%s", h2_cfg::style("reset"));
      }
      print_locate();
      ::printf("\n");
      bt.print();
   }

   void print(FILE* fp)
   {
      fprintf(fp, "Memory overflow malloc %p %+d  ", _ptr, _offset);
   }
};

class h2_fail_memleak : public h2_fail
{
 private:
   const char* _where;
   struct V
   {
      void* ptr;
      int size;
      int bytes;
      int times;
      h2_backtrace bt;
      V(void* _ptr, int _size, h2_backtrace& _bt)
        : ptr(_ptr), size(_size), bytes(_size), times(1), bt(_bt)
      {
      }
   };
   std::vector<V, h2_allocator<V>> _leaks;
   long long _bytes;
   int _places;

 public:
   h2_fail_memleak(const char* file = "", int line = -1, const char* where = "")
     : h2_fail(file, line), _where(where), _bytes(0), _places(0) {}
   virtual ~h2_fail_memleak() {}

   void add(void* ptr, int size, h2_backtrace& bt)
   {
      _bytes += size;
      _places += 1;
      for (auto i = _leaks.begin(); i != _leaks.end(); i++) {
         if ((*i).bt == bt) {
            (*i).bytes += size;
            (*i).times += 1;
            return;
         }
      }
      _leaks.push_back(V(ptr, size, bt));
   }

   void print()
   {
      kprintf("Memory Leaked %s%lld bytes in %s totally", str_places(_places), _bytes, _where);
      h2_fail::print();
      for (auto i = _leaks.begin(); i != _leaks.end(); i++) {
         auto v = *i;
         ::printf("  %p", v.ptr);
         if (v.times <= 1)
            ::printf(" ");
         else
            ::printf("... ");
         ::printf("Leaked ");
         if (v.times <= 1)
            /* ::printf("") */;
         else
            ::printf("%d times ", v.times);
         ::printf("%d", v.bytes);
         if (v.times <= 1)
            ::printf(" ");
         else
            ::printf("(%d+...)", v.size);
         ::printf("bytes, at backtrace\n");

         v.bt.print();
      }
   }

   const char* str_places(int places)
   {
      if (places <= 1) return "";
      static char t[32];
      sprintf(t, "%d places ", places);
      return t;
   }

   void print(FILE* fp)
   {
      fprintf(fp, "Memory Leaked %s%lld bytes in %s totally", str_places(_places), _bytes, _where);
   }
};

class h2_fail_json : public h2_fail
{
 private:
   h2_string e, a;

 public:
   h2_fail_json(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_json() {}

   void add(const char* expect, const char* actual)
   {
      e = expect;
      a = actual;
   }

   void print()
   {
      h2_fail::print();

      int terminal_columns = h2_cfg::get_term_columns();

      h2json::diff_print(e.c_str(), a.c_str(), terminal_columns);
   }

   void print(FILE* fp)
   {
      fprintf(fp, "JSON compare failed");
   }
};

class h2_fail_instantiate : public h2_fail
{
 private:
   const char* action_type;
   const char* return_type;
   const char* class_type;
   const char* method_name;
   const char* return_args;
   int why;

 public:
   h2_fail_instantiate(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_instantiate() {}

   void add(
     const char* action_type,
     const char* return_type,
     const char* class_type,
     const char* method_name,
     const char* return_args,
     int why)
   {
      this->action_type = action_type;
      this->return_type = return_type;
      this->class_type = class_type;
      this->method_name = method_name;
      this->return_args = return_args;
      this->why = why;

      if (why == 1)
         kprintf("Instantiate 'class %s' don't know initialize arguments", class_type);
      else if (why == 2)
         kprintf("Instantiate 'class %s' is a abstract class", class_type);
   }

   void print()
   {
      h2_fail::print();

      ::printf("You may take following solutions to fix it: \n");
      if (why == 1) {
         ::printf("1. Define default constructor in class %s, or \n", class_type);
         ::printf("2. Add parameterized construction in %s(%s%s%s, %s, %s%s, %s(...)%s) \n",
                  action_type,
                  strlen(return_type) ? return_type : "",
                  strlen(return_type) ? ", " : "",
                  class_type, method_name, return_args,
                  h2_cfg::style("bold,yellow"),
                  class_type,
                  h2_cfg::style("reset"));
      }
      else if (why == 2) {
         ::printf("1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s%s, Derived_%s(...)%s) \n",
                  action_type,
                  strlen(return_type) ? return_type : "",
                  strlen(return_type) ? ", " : "",
                  class_type, method_name, return_args,
                  h2_cfg::style("bold,yellow"),
                  class_type,
                  h2_cfg::style("reset"));
      }
   }

   void print(FILE* fp)
   {
      fprintf(fp, "Instantiate 'class %s' don't know initialize arguments", class_type);
   }
};

#endif
