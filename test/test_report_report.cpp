#include "../source/h2_unit.cpp"

CASE(H2_UNITS)
{
   OK("case", H2_UNITS(0, "case"));
   OK("case", H2_UNITS(1, "case"));
   OK("cases", H2_UNITS(3, "case"));
}

SUITE(report utils)
{
   Case(format_volume)
   {
      OK("101", h2::h2_report_console::format_volume(101));
      OK("1.0KB", h2::h2_report_console::format_volume(1024));
      OK("1.5KB", h2::h2_report_console::format_volume(1024 + 512));
      OK("1.00MB", h2::h2_report_console::format_volume(1024 * 1024));
      OK("1.50MB", h2::h2_report_console::format_volume(1024 * 1024 + 1024 * 512));
      OK("1.000GB", h2::h2_report_console::format_volume(1024 * 1024 * 1024));
      OK("1.500GB", h2::h2_report_console::format_volume(1024 * 1024 * 1024 + 1024 * 1024 * 512));
   }

   Case(format_duration)
   {
      OK("99 milliseconds", h2::h2_report_console::format_duration(99));
      OK("0.8 seconds", h2::h2_report_console::format_duration(799));
      OK("1.0 second", h2::h2_report_console::format_duration(1000));
      OK("8.2 seconds", h2::h2_report_console::format_duration(1000 * 8 + 200));
      OK("1.00 minute", h2::h2_report_console::format_duration(1000 * 60));
      OK("1.50 minutes", h2::h2_report_console::format_duration(1000 * 60 + 1000 * 30));
      OK("1.000 hour", h2::h2_report_console::format_duration(1000 * 60 * 60));
      OK("1.500 hours", h2::h2_report_console::format_duration(1000 * 60 * 60 + 1000 * 60 * 30));
   }

   Case(format_units single unit)
   {
      OK("0 time", h2::h2_report_console::format_units(0, " time"));
      OK("1 time", h2::h2_report_console::format_units(1, " time"));
      OK("2 times", h2::h2_report_console::format_units(2, " time"));
      OK("3 times", h2::h2_report_console::format_units(3, " time"));
   }

   Case(format_units multiple unit)
   {
      OK("0 time", h2::h2_report_console::format_units(0, " time", " timex"));
      OK("1 time", h2::h2_report_console::format_units(1, " time", " timex"));
      OK("2 timex", h2::h2_report_console::format_units(2, " time", " timex"));
      OK("3 timex", h2::h2_report_console::format_units(3, " time", " timex"));
   }
}

struct a_report : h2::h2_report_interface {
   FILE* f;
   void on_runner_start(h2::h2_runner* r) override
   {
      f = fopen("a.out.report.txt", "w");
      if (!f) return;
      fprintf(f, "on_runner_start\n");
   }
   void on_runner_endup(h2::h2_runner*) override
   {
      fprintf(f, "on_runner_endup\n");
      if (!f) fclose(f);
   }
   void on_suite_start(h2::h2_suite* s) override
   {
      fprintf(f, "on_suite_start : %s\n", s->name);
   }
   void on_suite_endup(h2::h2_suite* s) override
   {
      fprintf(f, "on_suite_start : %s\n", s->name);
   }
   void on_case_start(h2::h2_suite* s, h2::h2_case* c) override
   {
      fprintf(f, "on_case_start : %s\n", c->name);
   }
   void on_case_endup(h2::h2_suite* s, h2::h2_case* c) override
   {
      fprintf(f, "on_case_endup : %s\n", c->name);
   }
};

H2Report(a_report);
