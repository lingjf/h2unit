struct h2_case : h2_test {
   bool todo = false;
   bool ignored = false, failed = false, last_failed = false, warning = false;
   jmp_buf fail_hole;
   h2_fail* fails = nullptr;

   h2_case(const char* filine, const char* file, int line, const char* describe, int todo_) : h2_test(filine, file, line, describe), todo(todo_) {}
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() {}
   void post_cleanup();

   void failing(h2_fail* fail, bool defer, bool append);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};
