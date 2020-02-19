
struct h2_suite;
static inline void h2_suite_case_g(h2_suite*, void*);
static inline void h2_suite_setup_g(h2_suite*);
static inline void h2_suite_teardown_g(h2_suite*);

static constexpr const char* status2string[] = {"init", "TODO", "Filtered", "Passed", "Failed"};

struct h2_case {
   /* clang-format off */
   enum { INITED = 0, PASSED, FAILED, TODOED, FILTED };
   /* clang-format on */

   const char* name;
   h2_suite* suite;
   int status;
   const char* file;
   int line;
   long long t_start, t_end;

   void prev_setup() {
      t_start = h2_now();
      status = PASSED;
      h2_stack::G().push(file, line, "case");
   }

   void post_setup() { jc = 1; }

   void prev_teardown() { jc = 0; }

   void post_teardown() {
      lambda_code = nullptr;
      undo_dns();
      undo_stub();
      h2_fail* fail = undo_mock();
      h2_append_x_fail(fail, h2_stack::G().pop());

      if (fail)
         if (status == FAILED)
            delete fail;
         else {
            status = FAILED;
            h2_append_x_fail(fails, fail);
         }

      t_end = h2_now();
   }

   void execute() {
      prev_setup();
      h2_suite_setup_g(suite);
      post_setup();

      if (::setjmp(jb) == 0) {
         derive_code();
         if (lambda_code) lambda_code();
      } else
         status = FAILED;

      prev_teardown();
      h2_suite_teardown_g(suite);
      post_teardown();
   }

   h2_list stubs, mocks, dnss;

   void do_stub(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
      h2_stub* stub = nullptr;
      h2_list_for_each_entry(p, &stubs, h2_stub, x) if (p->befp == befp) {
         stub = p;
         break;
      }
      if (!stub) {
         stub = new h2_stub(befp, file, line);
         stubs.push(&stub->x);
      }
      stub->replace(tofp);
   }

   void undo_stub() {
      h2_list_for_each_entry(p, &stubs, h2_stub, x) {
         p->restore();
         p->x.out();
         delete p;
      }
   }

   bool do_mock(h2_mock* mock) {
      h2_list_for_each_entry(p, &mocks, h2_mock, x) if (p == mock) return true;
      do_stub(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      mocks.push(&mock->x);
      return true;
   }

   h2_fail* undo_mock() {
      h2_fail* fail = nullptr;
      h2_list_for_each_entry(p, &mocks, h2_mock, x) {
         h2_append_x_fail(fail, p->times_check());
         p->reset();
         p->x.out();
      }
      return fail;
   }

   void do_dns(h2_dns* dns) { dnss.push(&dns->x); }

   void undo_dns() {
      h2_list_for_each_entry(p, &dnss, h2_dns, x) {
         p->x.out();
         p->y.out();
         delete p;
      }
   }

   int jc;
   jmp_buf jb;
   h2_fail* fails;

   void do_fail(h2_fail* fail) {
      status = FAILED;
      h2_append_x_fail(fails, fail);
      if (0 < jc--) ::longjmp(jb, 1);
   }

   struct H {
      h2_case* _case;
      int count;

      H(h2_case* case_) : _case(case_), count(0) {
         _case->prev_setup();
         h2_suite_setup_g(_case->suite);
         _case->post_setup();
      }
      ~H() {
         _case->prev_teardown();
         h2_suite_teardown_g(_case->suite);
         _case->post_teardown();
      }

      operator bool() { return 0 == count++; }
   };

   h2_case(const char* name_, h2_suite* suite_, int status_, const char* file_, int line_)
     : name(name_), suite(suite_), status(status_), file(file_), line(line_), jc(0), fails(nullptr), lambda_code() {
      h2_suite_case_g(suite, this);
   }

   virtual void derive_code() {}
   std::function<void()> lambda_code;
};
