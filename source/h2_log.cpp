
h2_inline h2_log::h2_log() : total_cases(0), done_cases(0), percentage(0), tt(0), ts(0), tc(0) {}
h2_inline void h2_log::on_task_start(int cases) {
   total_cases = cases;
   tt = h2_now();
};
h2_inline void h2_log::on_task_endup(int status_stats[8]) { tt = h2_now() - tt; };
h2_inline void h2_log::on_suite_start(h2_suite* s) { ts = h2_now(); }
h2_inline void h2_log::on_suite_endup(h2_suite* s) { ts = h2_now() - ts; }
h2_inline void h2_log::on_case_start(h2_suite* s, h2_case* c) { tc = h2_now(); };
h2_inline void h2_log::on_case_endup(h2_suite* s, h2_case* c) {
   percentage = ++done_cases * 100 / total_cases;
   tc = h2_now() - tc;
};

struct h2_log_console : h2_log {
   void on_task_endup(int status_stats[8]) override {
      h2_log::on_task_endup(status_stats);
      printf("\n[%3d%%] ", percentage);
      if (0 < status_stats[h2_case::FAILED]) {
         printf("%s", S("bold,red"));
         printf("Failed <%d failed, %d passed, %d todo, %d filtered, %lld ms>\n", status_stats[h2_case::FAILED], status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], tt);
      } else {
         printf("%s", S("bold,green"));
         printf("Passed <%d passed, %d todo, %d filtered, %d cases, %lld ms>\n", status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], total_cases, tt);
      }
      printf("%s", S("reset"));
   }
   void on_case_endup(h2_suite* s, h2_case* c) override {
      h2_log::on_case_endup(s, c);
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         if (O.verbose)
            printf("[%3d%%] (%s // %s): %s at %s:%d\n", percentage, s->name, c->name, CSS[c->status], basename((char*)c->file), c->line);
         break;
      case h2_case::FILTED: break;
      case h2_case::PASSED:
         if (O.verbose) {
            printf("[%3d%%] ", percentage);
            printf("%s", S("light blue"));
            printf("(%s // %s): Passed - %lld ms\n", s->name, c->name, tc);
            printf("%s", S("reset"));
         } else if (!O.debug)
            printf("\r[%3d%%] (%d/%d)\r", percentage, done_cases, total_cases);
         break;
      case h2_case::FAILED:
         printf("[%3d%%] ", percentage);
         printf("%s", S("bold,purple"));
         printf("(%s // %s): Failed at %s:%d\n", s->name, c->name, basename((char*)c->file), c->line);
         printf("%s", S("reset"));
         for (h2_fail* x_fail = c->fails; x_fail; x_fail = x_fail->x_next)
            for (h2_fail* fail = x_fail; fail; fail = fail->y_next)
               fail->print();
         printf("\n");
         break;
      }
   }
};

struct h2_log_xml : h2_log {
   FILE* f;

   void on_task_start(int cases) override {
      h2_log::on_task_start(cases);
      f = fopen(O.junit, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   };

   void on_suite_start(h2_suite* s) override {
      h2_log::on_suite_start(s);
      if (!f) return;
      fprintf(f, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
              s->status_stats[h2_case::FAILED], s->name, s->status_stats[h2_case::TODOED] + s->status_stats[h2_case::FILTED], (int)s->cases().size(), 0, "");
   }

   void on_case_endup(h2_suite* s, h2_case* c) override {
      h2_log::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
              s->name, c->name, CSS[c->status], tc / 1000.0);

      if (c->status == h2_case::FAILED) {
         fprintf(f, "      <failure message=\"%s:%d:", c->file, c->line);
         for (h2_fail* x_fail = c->fails; x_fail; x_fail = x_fail->x_next)
            for (h2_fail* fail = x_fail; fail; fail = fail->y_next) {
               fprintf(f, "{newline}");
               fail->print(f);
            }
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "      <system-out></system-out><system-err></system-err>\n");
      fprintf(f, "    </testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override {
      h2_log::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "  </testsuite>\n");
   }

   void on_task_endup(int status_stats[8]) override {
      h2_log::on_task_endup(status_stats);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_log_tap : h2_log {
   /* */
};

h2_inline void h2_logs::init() {
   static h2_log_console console_log;
   static h2_log_xml xml_log;
   static h2_log_tap tap_log;
   logs.push_back(&console_log);
   if (strlen(O.junit)) logs.push_back(&xml_log);
}
