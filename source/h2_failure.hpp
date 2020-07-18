
struct h2_fail : h2_libc {
   h2_fail *subling_next{nullptr}, *child_next{nullptr};

   const char* file;
   int line;

   const char* func;
   int seq = -1;

   const char* check_type = "Inner";  // Inner(Mock, AllOf, &&, ||)
   h2_string e_expression, a_expression;
   h2_line explain;
   h2_string user_explain;

   //    expression     expection      represent       value
   //     Ge(var)        Ge(5)          5               5
   //     We(var)        We("abc")      "abc"           abc

   h2_fail(const h2_line& explain_, const char* file_, int line_, const char* func_ = nullptr, int seq_ = -1)
     : file(file_), line(line_), func(func_), seq(seq_), explain(explain_) {}
   virtual ~h2_fail();

   const char* locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);

   static h2_fail* new_normal(const h2_line& explain, const char* func = nullptr, const char* file = nullptr, int line = 0);
   static h2_fail* new_unexpect(const h2_string& e_represent = "", const h2_string& a_represent = "", const h2_string& expection = "", const h2_line& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_string& expection, const char* file = nullptr, int line = 0);
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, const char* file = nullptr, int line = 0);
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, bool caseless, const char* file = nullptr, int line = 0);
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& expection, const h2_string& a_represent, const h2_line& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_memory_leak(const void* ptr, int size, const h2_backtrace& bt_allocate, const char* where, const char* file, int line);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const int size, const void* addr, const char* action, h2_vector<unsigned char> spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file = nullptr, int line = 0);
   static h2_fail* new_use_after_free(const void* ptr, const void* addr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
   static h2_fail* new_call(const char* func, const char* expect, const char* actual, const h2_line& explain = {}, const char* file = nullptr, int line = 0);
};

static inline void h2_fail_g(h2_fail*, bool);
