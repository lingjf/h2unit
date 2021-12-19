h2_inline void h2_report::initialize()
{
   if (O.lists) {
      h2_array_append(I().reports, new h2_report_list);
   } else {
      h2_array_append(I().reports, new h2_report_console);
      if (strlen(O.junit_path)) h2_array_append(I().reports, new h2_report_junit);
   }
}

h2_inline h2_report::registor::registor(h2_report_interface* report) { h2_array_append(h2_report::I().reports, report); }

/* clang-format off */
h2_inline void h2_report::on_runner_start(h2_runner* r) { for (int i = 0; reports[i]; ++i) reports[i]->on_runner_start(r); }
h2_inline void h2_report::on_runner_endup(h2_runner* r) { for (int i = 0; reports[i]; ++i) reports[i]->on_runner_endup(r); }
h2_inline void h2_report::on_suite_start(h2_suite* s) { for (int i = 0; reports[i]; ++i) reports[i]->on_suite_start(s); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { for (int i = 0; reports[i]; ++i) reports[i]->on_suite_endup(s); }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { for (int i = 0; reports[i]; ++i) reports[i]->on_case_start(s, c); }
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c) { for (int i = 0; reports[i]; ++i) reports[i]->on_case_endup(s, c); }
