
struct h2_report {
   h2_singleton(h2_report);
   static void initialize();

   h2_list reports;
   void on_task_start(h2_task* t);
   void on_task_endup(h2_task* t);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};
