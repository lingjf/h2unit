
struct h2_fail : h2_nohook {
   h2_fail *x_next, *y_next;

   const char* file;
   int line;

   const char* func;
   int argi;

   h2_string _k, _h, _m, _u;
   h2_string e_expr, _e, a_expr, _a;
   int w_type;  // 0 is MOCK; 1 is OK(condition); 2 is OK(expect, actual); 3 is JE

   h2_fail(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1);
   virtual ~h2_fail();

   void locate(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1);
   void set_w_type(int w_type_);
   void set_e_expr(const char* e_expr_);
   void set_a_expr(const char* a_expr_);

   void kprintf(const char* format, ...);
   void hprintf(const char* format, ...);
   void eprintf(const char* format, ...);
   void aprintf(const char* format, ...);
   void mprintf(const char* format, ...);
   void uprintf(const char* format, ...);
   void mprintf() {}

   void print_locate();
   virtual void print();
   virtual void print(FILE* fp);
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

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const char* file_ = nullptr, int line_ = 0, const char* func_ = nullptr, const char* format = "", ...);
   void print();
};

struct h2_fail_unexpect : h2_fail {
   h2_fail_unexpect(const char* file_ = nullptr, int line_ = 0);
   void print_OK1();
   void print_OK2();
   void print_OK3();
   void print_MOCK();
   void print();
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const h2_string expect, actual;
   const bool caseless;
   h2_fail_strcmp(const h2_string& expect_, const h2_string& actual_, bool caseless_, const char* file_ = nullptr, int line_ = 0);
   void print();
   char* fmt_char(char c, bool eq);
};

struct h2_fail_json : h2_fail_unexpect {
   const h2_string expect, actual;
   h2_fail_json(const h2_string& expect_, const h2_string& actual_, const char* file_ = nullptr, int line_ = 0);
   void print();
};

struct h2_fail_memcmp : h2_fail_unexpect {
   h2_vector<unsigned char> expect, actual;
   h2_fail_memcmp(const unsigned char* expect_, const unsigned char* actual_, int len, const char* file_ = nullptr, int line_ = 0);
   void print();
   char* fmt_byte(unsigned char c, unsigned char t, int j, const char* style, char* p);
};

struct h2_fail_memoverflow : h2_fail {
   const unsigned char* ptr;
   const int offset;
   const unsigned char* magic;
   const h2_vector<unsigned char> spot;
   const h2_backtrace bt0, bt1;
   h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0);
   void print();
};

struct h2_fail_memleak : h2_fail {
   const char* where;
   struct P {
      void *ptr, *ptr2;
      long long size, size2, bytes, times;
      h2_backtrace bt;
      P(void* ptr_, int size_, h2_backtrace& bt_) : ptr(ptr_), ptr2(nullptr), size(size_), size2(0), bytes(size_), times(1), bt(bt_) {}
   };
   h2_vector<P> places;
   long long bytes, times;

   h2_fail_memleak(const char* file_ = nullptr, int line_ = 0, const char* where_ = "");

   void add(void* ptr, int size, h2_backtrace& bt);
   void print();
};

struct h2_fail_doublefree : h2_fail {
   const h2_backtrace bt0, bt1;
   h2_fail_doublefree(void* ptr_, h2_backtrace& bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0);
   void print();
};

struct h2_fail_instantiate : h2_fail {
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   const bool why_abstract;
   h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_abstract_, const char* file_, int line_);
   void print();
};