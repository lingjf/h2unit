
struct h2_task {
   h2_logs logs;
   h2_log_console console_log;
   h2_log_xml xml_log;

   int status[8];
   h2_case* current_case;
   std::vector<h2_case*> cases;

   h2_task() : status{0}, current_case(nullptr) {}

   /* clang-format off */
   static h2_task& I() { static h2_task __; return __; }
   /* clang-format on */

   void prepare() {
      h2_sihook_g();
      if (O().listing) h2_directory::list_then_exit();

      logs.add(&console_log);
      if (strlen(O().junit)) logs.add(&xml_log);

      cases = h2_directory::cases();

      if (O().dns) h2_ns::I().init();
      h2_stack::G().push(__FILE__, __LINE__, "root");
      h2_dohook_g();
   }

   void cleanup() {
      h2_unhook_g();
      if (O().dns) h2_ns::I().exit();
      if (status[h2_case::FAILED] == 0) h2_directory::drop_last_order();
   }

   void execute() {
      long long t_start = h2_now();
      logs.on_task_start();
      for (auto c : cases) {
         current_case = c;
         logs.on_case_start(c);
         if (O().filter(c->suite->name, c->name, c->file)) c->status = h2_case::FILTED;
         if (h2_case::INITED == c->status) c->suite->p(c->suite, c);
         logs.on_case_endup(c);
         status[c->status] += 1;
         c->suite->status[c->status] += 1;
         if (0 < O().breakable && O().breakable <= status[h2_case::FAILED]) break;
      }
      logs.on_task_endup(status, cases.size(), h2_now() - t_start);
   }
};

static inline void h2_stub_g(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   if (h2_task::I().current_case) h2_task::I().current_case->do_stub(befp, tofp, befn, tofn, file, line);
}

static inline bool h2_mock_g(h2_mock* mock) {
   return h2_task::I().current_case ? h2_task::I().current_case->do_mock(mock) : false;
}

static inline void h2_dns_g(h2_dns* dns) {
   if (h2_task::I().current_case) h2_task::I().current_case->do_dns(dns);
}

static inline void h2_fail_g(void* fail) {
   if (h2_task::I().current_case && fail) h2_task::I().current_case->do_fail((h2_fail*)fail);
}
