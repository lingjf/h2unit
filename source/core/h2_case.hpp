
struct h2_case {
   enum { initial = 0,
          passed,
          failed,
          todo,
          filtered,
          ignored,
          statuss };

   const char* name;
   const char* file;
   int lino;
   h2_list x;
   int seq = 0;
   int last_status = initial;
   int status = initial;
   int asserts = 0;
   long long footprint = 0;
   jmp_buf jump;
   h2_fail* fails{nullptr};
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;
   h2_sock* sock{nullptr};

   h2_case(const char* name, int status, const char* file, int lino);
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
