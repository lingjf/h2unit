struct h2_fail : h2_libc {
   h2_fail *subling_next = nullptr, *child_next = nullptr;

   const char* assert_type = "In";  // In(Mock, AllOf, &&, ||)
   const char* assert_op = ",";
   h2_string e_expression, a_expression;
   h2_line explain;
   h2_string user_explain;

   //           expression     expection      represent       value
   // number     Ge(var)        Ge(5)          5               5
   // string     We(var)        We("abc")      "abc"           abc
   // char       We(var)        We('a')        'a'             a

   int seqno = -1;
   const char* filine;

   h2_fail(const h2_line& explain_, const char* filine_) : explain(explain_), filine(filine_) {}
   virtual ~h2_fail();

   h2_line locate();

   virtual void print(size_t si = 0, size_t ci = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, size_t, size_t)> cb, size_t si = 0, size_t ci = 0);
   static void append_subling(h2_fail*& fails, h2_fail* fail);
   static void append_child(h2_fail*& fails, h2_fail* fail);

   static h2_fail* new_normal(const h2_line& explain, const char* filine = nullptr);
   static h2_fail* new_unexpect(const h2_line& expection = {}, const h2_line& represent = {}, const h2_line& explain = {});
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection, const h2_line& explain = {});
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, const h2_line& explain = {});
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, bool caseless, const h2_line& explain = {});
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t length, const size_t width);
   static h2_fail* new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* filine);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample);
   static h2_fail* new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
   static h2_fail* new_exception(const h2_line& explain, const char* type, const h2_backtrace& bt_throw, const char* filine = nullptr);
   static h2_fail* new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_line& explain = {});
};
