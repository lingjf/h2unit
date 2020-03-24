
struct h2_task {
   h2_singleton(h2_task);

   h2_logs logs;
   int status_stats[8];
   h2_case* current_case;
   std::vector<void (*)()> global_setups, global_teardowns;
   std::vector<void (*)()> global_suite_setups, global_suite_teardowns;
   std::vector<void (*)()> global_case_setups, global_case_teardowns;

   h2_task() : status_stats{0}, current_case(nullptr) {}

   void prepare();
   void postpare();
   void execute();
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

static inline h2_sock* h2_sock_g(h2_sock* sock) {
   if (h2_task::I().current_case) return h2_task::I().current_case->do_sock(sock);
   return nullptr;
}

static inline void h2_fail_g(void* fail) {
   if (fail && O.debug) h2_debugger::trap();
   if (h2_task::I().current_case && fail) h2_task::I().current_case->do_fail((h2_fail*)fail);
}
