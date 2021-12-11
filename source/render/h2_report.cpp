h2_inline void h2_report::initialize()
{
   if (O.lists) {
      static h2_report_list list_report;
      h2_array_append(I().reports, &list_report);
   } else {
      static h2_report_console console_report;
      h2_array_append(I().reports, &console_report);
      if (strlen(O.junit_path)) {
         static h2_report_junit junit_report;
         h2_array_append(I().reports, &junit_report);
      }
   }
}

/* clang-format off */
h2_inline void h2_report::on_runner_start(h2_runner* r) { for (int i = 0; reports[i]; ++i) reports[i]->on_runner_start(r); }
h2_inline void h2_report::on_runner_endup(h2_runner* r) { for (int i = 0; reports[i]; ++i) reports[i]->on_runner_endup(r); }
h2_inline void h2_report::on_suite_start(h2_suite* s) { for (int i = 0; reports[i]; ++i) reports[i]->on_suite_start(s); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { for (int i = 0; reports[i]; ++i) reports[i]->on_suite_endup(s); }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { for (int i = 0; reports[i]; ++i) reports[i]->on_case_start(s, c); }
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c) { for (int i = 0; reports[i]; ++i) reports[i]->on_case_endup(s, c); }
