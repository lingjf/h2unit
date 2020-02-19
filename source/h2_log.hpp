
struct h2_log {
   int total_cases, done_cases, percentage;
   h2_log() : total_cases(0), done_cases(0), percentage(0) {}
   virtual void on_task_start(int cases) { total_cases = cases; };
   virtual void on_task_endup(int status_stats[8], long long duration){};
   virtual void on_case_start(h2_case* c){};
   virtual void on_case_endup(h2_case* c) { percentage = ++done_cases * 100 / total_cases; };
};

struct h2_log_console : public h2_log {
   void on_task_endup(int status_stats[8], long long duration) {
      h2_log::on_task_endup(status_stats, duration);
      printf("\n[%3d%%] ", percentage);
      if (0 < status_stats[h2_case::FAILED]) {
         printf("%s", S("bold,red"));
         printf("Failed <%d failed, %d passed, %d todo, %d filtered, %lld ms>\n", status_stats[h2_case::FAILED], status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], duration);
      } else {
         printf("%s", S("bold,green"));
         printf("Passed <%d passed, %d todo, %d filtered, %d cases, %lld ms>\n", status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], total_cases, duration);
      }
      printf("%s", S("reset"));
   }
   void on_case_endup(h2_case* c) {
      h2_log::on_case_endup(c);
      switch (c->status) {
      case h2_case::INITED:
      case h2_case::TODOED:
      case h2_case::FILTED:
         if (O().verbose)
            printf("[%3d%%] CASE(%s // %s): %s at %s:%d\n", percentage, c->suite->name, c->name, status2string[c->status], c->file, c->line);
         break;
      case h2_case::PASSED:
         if (O().verbose) {
            printf("[%3d%%] ", percentage);
            printf("%s", S("light blue"));
            printf("CASE(%s // %s): Passed - %lld ms\n", c->suite->name, c->name, c->t_end - c->t_start);
            printf("%s", S("reset"));
         } else
            printf("\r[%3d%%] (%d/%d)\r", percentage, done_cases, total_cases);
         break;
      case h2_case::FAILED:
         printf("[%3d%%] ", percentage);
         printf("%s", S("bold,purple"));
         printf("CASE(%s // %s): Failed at %s:%d\n", c->suite->name, c->name, c->file, c->line);
         printf("%s", S("reset"));
         H2_FOREACH_FAIL(fail, c->fails) { fail->print(); }
         printf("\n");
         break;
      }
   }
};

struct h2_log_xml : public h2_log {
   void on_task_endup(int status_stats[8], long long duration) {
      h2_log::on_task_endup(status_stats, duration);
      h2_with f(fopen(O().junit, "w"));
      if (!f.f) return;

      fprintf(f.f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f.f, "<testsuites>\n");

      for (auto& s : h2_suite::suites()) {
         fprintf(f.f, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
                 s->status_stats[h2_case::FAILED], s->name, s->status_stats[h2_case::TODOED] + s->status_stats[h2_case::FILTED], (int)s->cases().size(), 0, "");

         for (auto& c : s->cases()) {
            fprintf(f.f, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
                    c->suite->name, c->name, status2string[c->status], (c->t_end - c->t_start) / 1000.0);

            if (c->status == h2_case::FAILED) {
               fprintf(f.f, "      <failure message=\"%s:%d:", c->file, c->line);
               H2_FOREACH_FAIL(fail, c->fails) {
                  fprintf(f.f, "{newline}");
                  fail->print(f.f);
               }
               fprintf(f.f, "\" type=\"AssertionFailedError\"></failure>\n");
            }
            fprintf(f.f, "      <system-out></system-out><system-err></system-err>\n");
            fprintf(f.f, "    </testcase>\n");
         }
         fprintf(f.f, "  </testsuite>\n");
      }
      fprintf(f.f, "</testsuites>\n");
   }
};

struct h2_logs {
   h2_log_console console_log;
   h2_log_xml xml_log;
   std::vector<h2_log*> logs;
   void init() {
      logs.push_back(&console_log);
      if (strlen(O().junit)) logs.push_back(&xml_log);
   }
   /* clang-format off */
   void on_task_start(int cases) { for (auto log : logs) log->on_task_start(cases); }
   void on_task_endup(int status_stats[8], long long duration) { for (auto log : logs) log->on_task_endup(status_stats, duration); }
   void on_case_start(h2_case* c) { for (auto log : logs) log->on_case_start(c); }
   void on_case_endup(h2_case* c) { for (auto log : logs) log->on_case_endup(c); }
   /* clang-format on */
};
