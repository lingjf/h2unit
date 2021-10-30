struct h2_case {
   h2_list x;
   const char* file;
   const char* name;
   bool todo = false;
   bool filtered = false, ignored = false, failed = false, last_failed = false;
   bool scheduled = false;
   int seq = 0;
   jmp_buf ctx;
   h2_list stubs;
   h2_list mocks;
   h2_dnses dnses;
   h2_stats stats;
   h2_fail* fails = nullptr;

   h2_case(const char* file_, const char* name_, int todo_) : file(file_), name(name_), todo(todo_) {}
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() { scheduled = false; }
   void post_cleanup();

   void failing(h2_fail* fail, bool defer, bool append);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};
