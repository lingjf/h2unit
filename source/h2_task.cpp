
struct h2_task {
   std::vector<h2_case*> case_list;

   h2_logs logs;
   h2_log_console console_log;
   h2_log_xml xml_log;

   int status[8];

   h2_task() : status{0}, current_suite(nullptr), current_case(nullptr) {}

   h2_suite* current_suite;
   h2_case* current_case;

   /* clang-format off */
   static h2_task& I() { static h2_task I; return I; }
   /* clang-format on */

   void prepare(int argc, const char** argv) {
      h2_cfg().opt(argc, argv);

      if (h2_cfg().listing) {
         h2_order::print_list();
         exit(0);
      }

      logs.add(&console_log);
      if (strlen(h2_cfg().junit)) {
         if (!xml_log.set_file(h2_cfg().junit)) {
            printf("Can't open file %s\n", h2_cfg().junit);
            exit(1);
         }
         logs.add(&xml_log);
      }

      h2_nm::I(); /* invoke system nm */

      h2_sihook_g();

      case_list = h2_order::case_list();

      h2_extra::I().dohook();
      addrinfos(1, "127.0.0.1");

      h2_stack::G().push(__FILE__, __LINE__, "root");
      h2_dohook_g();
   }

   void cleanup() {
      h2_unhook_g();

      h2_extra::I().unhook();

      if (status[h2_case::FAILED] == 0) h2_order::drop_last_order();
   }

   void execute() {
      long long t_start = h2_milliseconds();

      logs.on_task_start();

      for (auto it = case_list.begin(); it != case_list.end(); it++) {
         current_case = *it;
         current_suite = current_case->suite;

         logs.on_case_start(current_case);

         if (h2_cfg().filter(current_suite->name, current_case->name, current_case->file)) current_case->status = h2_case::FILTED;

         if (h2_case::INITED == current_case->status) current_suite->p(current_suite, current_case);

         logs.on_case_endup(current_case);

         status[current_case->status] += 1;
         current_suite->status[current_case->status] += 1;

         if (0 < h2_cfg().breakable && h2_cfg().breakable <= status[h2_case::FAILED]) break;
      }

      logs.on_task_endup(status, case_list.size(), h2_milliseconds() - t_start);
   }
};

static inline void h2_stub_g(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   if (h2_task::I().current_case) h2_task::I().current_case->do_stub(befp, tofp, befn, tofn, file, line);
}

static inline bool h2_mock_g(h2_mock* mock) {
   return h2_task::I().current_case ? h2_task::I().current_case->do_mock(mock) : false;
}

static inline void h2_fail_g(void* fail) {
   if (h2_task::I().current_case && fail) h2_task::I().current_case->do_fail((h2_fail*)fail);
}
