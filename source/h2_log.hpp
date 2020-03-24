
struct h2_log {
   int total_cases, done_cases, percentage;
   long long tt, ts, tc;
   h2_log();
   virtual void on_task_start(int cases);
   virtual void on_task_endup(int status_stats[8]);
   virtual void on_suite_start(h2_suite* s);
   virtual void on_suite_endup(h2_suite* s);
   virtual void on_case_start(h2_suite* s, h2_case* c);
   virtual void on_case_endup(h2_suite* s, h2_case* c);
};

struct h2_logs {
   std::vector<h2_log*> logs;
   void init();
   void on_task_start(int cases) {
      for (auto log : logs) log->on_task_start(cases);
   }
   void on_task_endup(int status_stats[8]) {
      for (auto log : logs) log->on_task_endup(status_stats);
   }
   void on_suite_start(h2_suite* s) {
      for (auto log : logs) log->on_suite_start(s);
   }
   void on_suite_endup(h2_suite* s) {
      for (auto log : logs) log->on_suite_endup(s);
   }
   void on_case_start(h2_suite* s, h2_case* c) {
      for (auto log : logs) log->on_case_start(s, c);
   }
   void on_case_endup(h2_suite* s, h2_case* c) {
      for (auto log : logs) log->on_case_endup(s, c);
   }
};
