
struct h2_log {
   virtual void on_task_start(){};
   virtual void on_task_endup(int status[8], int cases, long long duration){};
   virtual void on_case_start(h2_case* c){};
   virtual void on_case_endup(h2_case* c){};
};

/* clang-format off */
struct h2_logs : public h2_log {
   std::vector<h2_log*> logs;
   void add(h2_log* log) { logs.push_back(log); }
   void on_task_start() { for (auto log : logs) log->on_task_start(); }
   void on_task_endup(int status[8], int cases, long long duration) { for (auto log : logs) log->on_task_endup(status, cases, duration); }
   void on_case_start(h2_case* c) { for (auto log : logs) log->on_case_start(c); }
   void on_case_endup(h2_case* c) { for (auto log : logs) log->on_case_endup(c); }
};
/* clang-format on */

struct h2_log_console : public h2_log {
   void on_task_endup(int status[8], int cases, long long duration) {
      if (0 < status[h2_case::FAILED]) {
         printf("%s", S("bold,red"));
         printf("\nFailed <%d failed, %d passed, %d todo, %d filtered, %lld ms>\n", status[h2_case::FAILED], status[h2_case::PASSED], status[h2_case::TODOED], status[h2_case::FILTED], duration);
      } else {
         printf("%s", S("bold,green"));
         printf("\nPassed <%d passed, %d todo, %d filtered, %d cases, %lld ms>\n", status[h2_case::PASSED], status[h2_case::TODOED], status[h2_case::FILTED], cases, duration);
      }
      printf("%s", S("reset"));
   }
   void on_case_endup(h2_case* c) {
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         printf("CASE(%s, %s): TODO at %s:%d\n", c->suite->name, c->name, c->file, c->line);
         break;
      case h2_case::FILTED: break;
      case h2_case::PASSED:
         if (O().verbose) {
            printf("%s", S("light blue"));
            printf("CASE(%s, %s): Passed - %lld ms\n", c->suite->name, c->name, c->t_end - c->t_start);
            printf("%s", S("reset"));
         }
         break;
      case h2_case::FAILED:
         printf("%s", S("bold,purple"));
         printf("CASE(%s, %s): Failed at %s:%d\n", c->suite->name, c->name, c->file, c->line);
         printf("%s", S("reset"));
         H2_FAIL_FOREACH(fail, c->fails) { fail->print(); }
         ::printf("\n");
         break;
      }

      printf("%s", S("reset"));
   }
};

struct h2_log_xml : public h2_log {
   void on_task_endup(int status[8], int cases, long long duration) {
      h2_with f(fopen(O().junit, "w"));
      if (!f.f) return;

      fprintf(f.f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f.f, "<testsuites>\n");

      for (auto& s : h2_suite::suites()) {
         fprintf(f.f, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
                 s->status[h2_case::FAILED], s->name, s->status[h2_case::TODOED] + s->status[h2_case::FILTED], (int)s->cases().size(), 0, "");

         for (auto& c : s->cases()) {
            fprintf(f.f, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
                    c->suite->name, c->name, case_status[c->status], (c->t_end - c->t_start) / 1000.0);

            if (c->status == h2_case::FAILED) {
               fprintf(f.f, "      <failure message=\"%s:%d:", c->file, c->line);
               H2_FAIL_FOREACH(fail, c->fails) {
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
