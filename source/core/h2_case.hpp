struct h2_case {
   h2_sz sz;
   const char* name;
   bool todo = false, filtered = false, ignored = false;
   bool failed = false, last_failed = false;
   int seq = 0;
   int asserts = 0;
   long long footprint = 0;
   h2_list x;
   jmp_buf ctx;
   h2_fail* fails = nullptr;
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;

   h2_case(const char* name_, int todo_, const h2_sz& sz_) : sz(sz_), name(name_), todo(todo_) {}
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() {}
   void post_cleanup();

   void do_fail(h2_fail* fail, bool defer, bool append);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};
