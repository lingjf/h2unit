
struct h2_case {
   static constexpr int initial = 0, passed = 1, failed = 2, ignored = 3;

   const char* name;
   const char* file;
   int line;
   bool todo = false, filtered = false;
   int status = initial, last_status = initial;
   int seq = 0;
   int asserts = 0;
   long long footprint = 0;
   h2_list x;
   jmp_buf ctx;
   h2_fail* fails{nullptr};
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;
   h2_sock* sock{nullptr};

   h2_case(const char* name_, const char* file_, int line_, int todo_) : name(name_), file(file_), line(line_), todo(todo_) {}
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() {}
   void post_cleanup(const h2_string& ex);

   void do_fail(h2_fail* fail, bool defer);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};
