struct h2_runner;
struct h2_suite;
struct h2_case;

struct h2_report_interface {
   virtual void on_runner_start(h2_runner*) = 0;
   virtual void on_runner_endup(h2_runner*) = 0;
   virtual void on_suite_start(h2_suite*) = 0;
   virtual void on_suite_endup(h2_suite*) = 0;
   virtual void on_case_start(h2_suite*, h2_case*) = 0;
   virtual void on_case_endup(h2_suite*, h2_case*) = 0;
};

struct h2_report : h2_report_interface {
   h2_singleton(h2_report);
   static void initialize();

   bool backable = false;
   h2_report_interface* reports[8]{nullptr};

   virtual void on_runner_start(h2_runner* r) override;
   virtual void on_runner_endup(h2_runner* r) override;
   virtual void on_suite_start(h2_suite* s) override;
   virtual void on_suite_endup(h2_suite* s) override;
   virtual void on_case_start(h2_suite* s, h2_case* c) override;
   virtual void on_case_endup(h2_suite* s, h2_case* c) override;
};
