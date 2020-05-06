
h2_inline h2_report::h2_report() : total_cases(0), done_cases(0), percentage(0), tt(0), ts(0), tc(0) {}
h2_inline void h2_report::on_task_start(int cases) {
   total_cases = cases;
   tt = h2_now();
};
h2_inline void h2_report::on_task_endup(int status_stats[8]) { tt = h2_now() - tt; };
h2_inline void h2_report::on_suite_start(h2_suite* s) { ts = h2_now(); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { ts = h2_now() - ts; }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { tc = h2_now(); };
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c) {
   percentage = ++done_cases * 100 / total_cases;
   tc = h2_now() - tc;
};

struct h2_report_console : h2_report {
   void on_task_endup(int status_stats[8]) override {
      h2_report::on_task_endup(status_stats);
      h2_printf("\n[%3d%%] ", percentage);
      if (0 < status_stats[h2_case::FAILED])
         h2_printf("%s", SF("bold,red", "Failed <%d failed, %d passed, %d todo, %d filtered, %lld ms>\n", status_stats[h2_case::FAILED], status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], tt));
      else
         h2_printf("%s", SF("bold,green", "Passed <%d passed, %d todo, %d filtered, %d cases, %lld ms>\n", status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], total_cases, tt));
   }
   void on_case_endup(h2_suite* s, h2_case* c) override {
      h2_report::on_case_endup(s, c);
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         if (O.verbose) h2_printf("[%3d%%] (%s // %s): %s at %s:%d\n", percentage, s->name, c->name, CSS[c->status], basename((char*)c->file), c->line);
         break;
      case h2_case::FILTED: break;
      case h2_case::PASSED:
         if (O.verbose)
            h2_printf("[%3d%%] %s", percentage, SF("light blue", "(%s // %s): Passed - %lld ms\n", s->name, c->name, tc));
         else if (!O.debug)
            h2_printf("\r[%3d%%] (%d/%d)\r", percentage, done_cases, total_cases);
         break;
      case h2_case::FAILED:
         h2_printf("[%3d%%] %s", percentage, SF("bold,purple", "(%s // %s): Failed at %s:%d\n", s->name, c->name, basename((char*)c->file), c->line));
         for (h2_fail* x_fail = c->fails; x_fail; x_fail = x_fail->x_next)
            for (h2_fail* fail = x_fail; fail; fail = fail->y_next)
               fail->print();
         h2_printf("\n");
         break;
      }
   }
};

struct h2_report_junit : h2_report {
   FILE* f;
   void on_task_start(int cases) override {
      h2_report::on_task_start(cases);
      f = fopen(O.junit, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   };
   void on_suite_start(h2_suite* s) override {
      h2_report::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->status_stats[h2_case::FAILED], s->name, s->status_stats[h2_case::TODOED] + s->status_stats[h2_case::FILTED], (int)s->cases().size(), 0, "");
   }

   void on_case_endup(h2_suite* s, h2_case* c) override {
      h2_report::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, CSS[c->status], tc / 1000.0);

      if (c->status == h2_case::FAILED) {
         fprintf(f, "<failure message=\"%s:%d:", c->file, c->line);
         for (h2_fail* x_fail = c->fails; x_fail; x_fail = x_fail->x_next)
            for (h2_fail* fail = x_fail; fail; fail = fail->y_next) {
               fprintf(f, "{newline}");
               fail->print(f);
            }
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "<system-out></system-out><system-err></system-err>\n");
      fprintf(f, "</testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override {
      h2_report::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_task_endup(int status_stats[8]) override {
      h2_report::on_task_endup(status_stats);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_report_tap : h2_report {
   /* */
};

h2_inline void h2_reports::initialize() {
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   reports.push_back(&console_report);
   if (strlen(O.junit)) reports.push_back(&junit_report);
}

inline void h2_reports::on_task_start(int cases) {
   for (auto report : reports) report->on_task_start(cases);
}
inline void h2_reports::on_task_endup(int status_stats[8]) {
   for (auto report : reports) report->on_task_endup(status_stats);
}
inline void h2_reports::on_suite_start(h2_suite* s) {
   for (auto report : reports) report->on_suite_start(s);
}
inline void h2_reports::on_suite_endup(h2_suite* s) {
   for (auto report : reports) report->on_suite_endup(s);
}
inline void h2_reports::on_case_start(h2_suite* s, h2_case* c) {
   for (auto report : reports) report->on_case_start(s, c);
}
inline void h2_reports::on_case_endup(h2_suite* s, h2_case* c) {
   for (auto report : reports) report->on_case_endup(s, c);
}
