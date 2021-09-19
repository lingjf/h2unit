struct h2_fail : h2_libc {
   h2_fail *subling_next{nullptr}, *child_next{nullptr};

   const char* assert_type = "Inner";  // Inner(Mock, AllOf, &&, ||)
   h2_string e_expression, a_expression;
   h2_sentence explain;
   h2_string user_explain;

   //           expression     expection      represent       value
   // number     Ge(var)        Ge(5)          5               5
   // string     We(var)        We("abc")      "abc"           abc
   // char       We(var)        We('a')        'a'             a

   int seqno = -1;
   const char* file;
   int line;

   h2_fail(const h2_sentence& explain_, const char* file_, int line_) : explain(explain_), file(file_), line(line_) {}
   virtual ~h2_fail();

   h2_sentence locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);

   static h2_fail* new_normal(const h2_sentence& explain, const char* file = nullptr, int line = 0);
   static h2_fail* new_unexpect(const h2_sentence& expection = {}, const h2_sentence& represent = {}, const h2_sentence& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_sentence& expection, const h2_sentence& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_sentence& expection, const h2_sentence& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_sentence& expection, bool caseless, const h2_sentence& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t width, const size_t nbits, const h2_string& represent, const h2_sentence& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* file, int line);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file = nullptr, int line = 0);
   static h2_fail* new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
   static h2_fail* new_exception(const char* explain, const char* type, const h2_backtrace& bt_throw);
   static h2_fail* new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_sentence& explain = {}, const char* file = nullptr, int line = 0);
};

static inline void h2_fail_g(h2_fail*);
