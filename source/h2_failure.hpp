
struct h2_fail : h2_libc {
   h2_fail *subling_next{nullptr}, *child_next{nullptr};

   const char* file;
   int line;

   const char* func;
   int seq = -1;

   const char* check_type = "Inner";  // Inner(Mock, AllOf, &&, ||)
   h2_string e_expression, a_expression;
   h2_string explain, user_explain;

   //    expression     expection      represent       value
   //     Ge(var)        Ge(5)          5               5
   //     We(var)        We("abc")      "abc"           abc

   h2_fail(const char* file_, int line_, const char* func_ = nullptr, int seq_ = -1)
     : file(file_), line(line_), func(func_), seq(seq_) {}
   virtual ~h2_fail();

   const char* get_locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);

   static h2_fail* new_normal(const char* file = nullptr, int line = 0, const char* func = nullptr, const char* format = "", ...);
   static h2_fail* new_unexpect(const h2_string& e_represent = "", const h2_string& a_represent = "", const h2_string& expection = "", const h2_string& explain = "", const char* file = nullptr, int line = 0);
   static h2_fail* new_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_string& expection_, const char* file = nullptr, int line = 0);
   static h2_fail* new_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection_, const char* file = nullptr, int line = 0);
   static h2_fail* new_json(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection_, bool caseless_, const char* file = nullptr, int line = 0);
   static h2_fail* new_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, const h2_string& expection_, const h2_string& a_represent_, const h2_string &explain = "", const char* file = nullptr, int line = 0);
   static h2_fail* new_memory_leak(const void* ptr_, int size_, const h2_backtrace& bt_allocate_, const char* where_, const char* file_, int line_);
   static h2_fail* new_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_);
   static h2_fail* new_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_);
   static h2_fail* new_overflow(const void* ptr_, const int size_, const void* addr_, const char* action_, h2_vector<unsigned char> spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_, const char* file = nullptr, int line = 0);
   static h2_fail* new_use_after_free(const void* ptr_, const void* addr_, const char* action_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_use_);
   static h2_fail* new_call(const char* func_, const char* expect, const char* actual, const char* file = nullptr, int line = 0);
};

static inline void h2_fail_g(h2_fail*, bool);
