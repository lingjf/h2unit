struct h2_report {
   h2_singleton(h2_report);
   static void initialize();

   bool backable = false;
   h2_list reports;
   void on_runner_start(h2_runner* r);
   void on_runner_endup(h2_runner* r);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};
