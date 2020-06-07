
struct h2_report {
   int total_cases = 0, done_cases = 0, percentage = 0, ss = 0, cs = 0;
   long long tt = 0, ts = 0, tc = 0;
   virtual void on_task_start(int cases);
   virtual void on_task_endup(int stats[h2_case::statuss], int round);
   virtual void on_suite_start(h2_suite* s);
   virtual void on_suite_endup(h2_suite* s);
   virtual void on_case_start(h2_suite* s, h2_case* c);
   virtual void on_case_endup(h2_suite* s, h2_case* c);
};

struct h2_reports {
   std::vector<h2_report*> reports;
   void initialize();
   void on_task_start(int cases);
   void on_task_endup(int stats[h2_case::statuss], int round);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};
