
static constexpr const char* CSS[] = {"init", "Passed", "Failed", "TODO", "Filtered"};

struct h2_case {
   static constexpr const int INITED = 0, PASSED = 1, FAILED = 2, TODOED = 3, FILTED = 4;

   const char* name;
   const char* file;
   int line;
   long long seq;
   int status;
   h2_list stubs, mocks, dnss;
   h2_sock* sock;
   jmp_buf jump;
   h2_fail* fails;

   h2_case(const char* name_, int todo, const char* file_, int line_);

   void prev_setup();
   void post_setup();
   void prev_cleanup();
   void post_cleanup();

   void do_stub(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line);
   void undo_stub();

   bool do_mock(h2_mock* mock);
   h2_fail* undo_mock();

   void do_dns(h2_dns* dns);
   void undo_dns();

   h2_sock* do_sock(h2_sock* sock);
   void undo_sock();

   void do_fail(h2_fail* fail);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};
