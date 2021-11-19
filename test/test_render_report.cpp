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
      OK("1KB", h2::h2_report_console::format_volume(1024));
      OK("1.5KB", h2::h2_report_console::format_volume(1024 + 512));
      OK("1MB", h2::h2_report_console::format_volume(1024 * 1024));
      OK("1.5MB", h2::h2_report_console::format_volume(1024 * 1024 + 1024 * 512));
      OK("1GB", h2::h2_report_console::format_volume(1024 * 1024 * 1024));
      OK("1.5GB", h2::h2_report_console::format_volume(1024 * 1024 * 1024 + 1024 * 1024 * 512));
   }

   Case(format_duration)
   {
      OK("99 milliseconds", h2::h2_report_console::format_duration(99));
      OK("0.8 seconds", h2::h2_report_console::format_duration(799));
      OK("1 second", h2::h2_report_console::format_duration(1000));
      OK("8.2 seconds", h2::h2_report_console::format_duration(1000 * 8 + 200));
      OK("1 minute", h2::h2_report_console::format_duration(1000 * 60));
      OK("1.5 minutes", h2::h2_report_console::format_duration(1000 * 60 + 1000 * 30));
      OK("1 hour", h2::h2_report_console::format_duration(1000 * 60 * 60));
      OK("1.5 hours", h2::h2_report_console::format_duration(1000 * 60 * 60 + 1000 * 60 * 30));
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
