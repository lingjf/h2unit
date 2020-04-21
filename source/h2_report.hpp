
struct h2_report {
   int total_cases, done_cases, percentage;
   long long tt, ts, tc;
   h2_report();
   virtual void on_task_start(int cases);
   virtual void on_task_endup(int status_stats[8]);
   virtual void on_suite_start(h2_suite* s);
   virtual void on_suite_endup(h2_suite* s);
   virtual void on_case_start(h2_suite* s, h2_case* c);
   virtual void on_case_endup(h2_suite* s, h2_case* c);
};

struct h2_reports {
   std::vector<h2_report*> reports;
   void initialize();
   void on_task_start(int cases);
   void on_task_endup(int status_stats[8]);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};
