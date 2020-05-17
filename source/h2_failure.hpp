
struct h2_fail : h2_libc {
   h2_fail *subling_next, *child_next;

   const char* file;
   int line;

   const char* func;
   int argi;

   int usage;  // 0 is Inner(Mock, AllOf, &&||); 1 is OK(condition); 2 is OK(expect, actual); 3 is JE
   h2_string e_expression, a_expression;
   h2_string user_explain;

   //    expression     explain      represent       value
   //     Ge(var)        Ge(5)        5               5
   //     We(var)        We("abc")    "abc"           abc

   h2_fail(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1)
     : subling_next(nullptr), child_next(nullptr), file(file_), line(line_), func(func_), argi(argi_), usage(0) {}
   virtual ~h2_fail();

   void set_locate(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1);
   const char* get_locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach (std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);
};

struct h2_fail_unexpect : h2_fail {
   h2_string e_represent, a_represent, e_explain;
   h2_fail_unexpect(h2_string e_represent_, h2_string a_represent_, bool dont_ = false, bool caseless_ = false, h2_string pre = "", h2_string post = "", const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e_represent(e_represent_), a_represent(a_represent_), e_explain((dont_ ? "!" : "") + ((caseless_ ? "~" : "") + (pre + e_represent_ + post))) {}

   void print_OK1(h2_line& line);
   void print_OK2(h2_line& line);
   void print_JE(h2_line& line);
   void print_Inner(h2_line& line);
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool dont_, bool caseless_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("\"" + e_value_ + "\"", "\"" + a_value_ + "\"", dont_, caseless_, "", "", file_, line_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_strfind : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, bool dont_, bool caseless_, h2_string find_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("\"" + e_value_ + "\"", "\"" + a_value_ + "\"", dont_, caseless_, find_ + "(", ")", file_, line_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_json : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, bool dont_, bool caseless_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(e_value_, a_value_, dont_, caseless_, "Je(", ")", file_, line_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_memcmp : h2_fail_unexpect {
   static constexpr const int npr_1b = 4;
   static constexpr const int npr_8b = 16;
   static constexpr const int npr_16b = 8;
   static constexpr const int npr_32b = 4;
   static constexpr const int npr_64b = 2;
   h2_vector<unsigned char> e_value, a_value;
   const int width, nbits;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, bool dont, const char* e_ext, h2_string a_represent_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("", a_represent_, dont, false, e_ext, "", file_, line_), e_value(e_value_, e_value_ + (nbits_ + 7) / 8), a_value(a_value_, a_value_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}
   void print(int subling_index = 0, int child_index = 0);
   void print_bits(h2_lines& e_lines, h2_lines& a_lines);
   void print_bytes(h2_lines& e_lines, h2_lines& a_lines);
   void print_int16(h2_lines& e_lines, h2_lines& a_lines);
   void print_int32(h2_lines& e_lines, h2_lines& a_lines);
   void print_int64(h2_lines& e_lines, h2_lines& a_lines);
};

struct h2_fail_memoverflow : h2_fail {
   const unsigned char* ptr;
   const int offset;
   const unsigned char* magic;
   const h2_vector<unsigned char> spot;
   const h2_backtrace bt0, bt1;
   h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_), ptr((unsigned char*)ptr_), offset(offset_), magic(magic_), spot(((unsigned char*)ptr_) + offset_, ((unsigned char*)ptr_) + offset_ + size), bt0(bt0_), bt1(bt1_) {}
   void print(int subling_index = 0, int child_index = 0);
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

   h2_fail_memleak(const char* file_ = nullptr, int line_ = 0, const char* where_ = "") : h2_fail(file_, line_), where(where_), bytes(0), times(0) {}

   void add(void* ptr, int size, h2_backtrace& bt);
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_double_free : h2_fail {
   void* ptr;
   const h2_backtrace bt_allocate, bt_release1, bt_release2;
   h2_fail_double_free(void* ptr_, h2_backtrace& bt_allocate_, h2_backtrace& bt_release1_, h2_backtrace& bt_release2_) : h2_fail(nullptr, 0), ptr(ptr_), bt_allocate(bt_allocate_), bt_release1(bt_release1_), bt_release2(bt_release2_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_access_after_free : h2_fail {
   const unsigned char* ptr;
   int offset;
   const h2_backtrace bt_allocate, bt_release, bt_access;
   h2_fail_access_after_free(const unsigned char* ptr_, int offset_, h2_backtrace& bt_allocate_, h2_backtrace& bt_release_, h2_backtrace& bt_access_) : h2_fail(nullptr, 0), ptr(ptr_), offset(offset_), bt_allocate(bt_allocate_), bt_release(bt_release_), bt_access(bt_access_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_asymmetric_free : h2_fail {
   void* ptr;
   const char *who_allocate, *who_release;
   const h2_backtrace bt_allocate, bt_release;
   h2_fail_asymmetric_free(void* ptr_, const char* who_allocate_, const char* who_release_, h2_backtrace& bt_allocate_, h2_backtrace& bt_release_) : h2_fail(nullptr, 0), ptr(ptr_), who_allocate(who_allocate_), who_release(who_release_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_call : h2_fail {
   h2_string e_who, e_call, a_call;
   h2_fail_call(const char* func_, const char* expect, const char* actual, const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_), e_who(func_), e_call(expect), a_call(actual) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_instantiate : h2_fail {
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   const bool why_abstract;
   h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_abstract_, const char* file_, int line_) : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why_abstract(why_abstract_) {}
   void print(int subling_index = 0, int child_index = 0);
};
