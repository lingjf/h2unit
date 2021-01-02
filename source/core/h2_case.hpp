
struct h2_case {
   static constexpr int initial = 0, passed = 1, failed = 2, todo = 3, filtered = 4, ignored = 5, n_st = 6;

   const char* name;
   int status;
   const char* file;
   int lino;
   h2_list x;
   int seq = 0;
   int last_status = initial;
   int asserts = 0;
   long long footprint = 0;
   jmp_buf ctx;
   h2_fail* fails{nullptr};
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;
   h2_sock* sock{nullptr};

   h2_case(const char* name_, int status_, const char* file_, int lino_) : name(name_), status(status_), file(file_), lino(lino_) {}
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
