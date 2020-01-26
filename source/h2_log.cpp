
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
   void on_task_start() { for (auto it = logs.begin(); it != logs.end(); it++) (*it)->on_task_start(); }
   void on_task_endup(int status[8], int cases, long long duration) { for (auto it = logs.begin(); it != logs.end(); it++) (*it)->on_task_endup(status, cases, duration); }
   void on_case_start(h2_case* c) { for (auto it = logs.begin(); it != logs.end(); it++) (*it)->on_case_start(c); }
   void on_case_endup(h2_case* c) { for (auto it = logs.begin(); it != logs.end(); it++) (*it)->on_case_endup(c); }
};
/* clang-format on */

struct h2_log_console : public h2_log {
   void on_task_endup(int status[8], int cases, long long duration) {
      if (0 < status[h2_case::FAILED]) {
         printf("%s", h2_cfg().style("bold,red"));
         printf("\nFailed <%d failed, %d passed, %d todo, %d filtered, %lld ms>\n", status[h2_case::FAILED], status[h2_case::PASSED], status[h2_case::TODOED], status[h2_case::FILTED], duration);
      } else {
         printf("%s", h2_cfg().style("bold,green"));
         printf("\nPassed <%d passed, %d todo, %d filtered, %d cases, %lld ms>\n", status[h2_case::PASSED], status[h2_case::TODOED], status[h2_case::FILTED], cases, duration);
      }
      printf("%s", h2_cfg().style("reset"));
   }
   void on_case_endup(h2_case* c) {
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         printf("CASE(%s, %s): TODO at %s:%d\n", c->suite->name, c->name, c->file, c->line);
         break;
      case h2_case::FILTED:
         break;
      case h2_case::PASSED:
         if (h2_cfg().verbose) {
            printf("%s", h2_cfg().style("light blue"));
            printf("CASE(%s, %s): Passed - %lld ms\n", c->suite->name, c->name, c->t_end - c->t_start);
            printf("%s", h2_cfg().style("reset"));
         }
         break;
      case h2_case::FAILED:
         printf("%s", h2_cfg().style("bold,purple"));
         printf("CASE(%s, %s): Failed at %s:%d\n", c->suite->name, c->name, c->file, c->line);
         printf("%s", h2_cfg().style("reset"));

         for (auto it = c->fails.begin(); it != c->fails.end(); it++) {
            H2_FAIL_FOREACH(fail, *it) {
               fail->print();
            }
         }
         ::printf("\n");
         break;
      }

      printf("%s", h2_cfg().style("reset"));
   }
};

struct h2_log_xml : public h2_log {
   FILE* fp;

   bool set_file(const char* filepath) { return !!(fp = fopen(filepath, "w")); }

   void on_task_endup(int status[8], int cases, long long duration) {
      fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(fp, "<testsuites>\n");

      for (auto i = h2_suite::G().begin(); i != h2_suite::G().end(); i++) {
         h2_suite* s = *i;
         fprintf(fp, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
                 s->status[h2_case::FAILED], s->name, s->status[h2_case::TODOED] + s->status[h2_case::FILTED], (int)s->cases().size(), 0, "");

         for (auto j = s->cases().begin(); j != s->cases().end(); j++) {
            h2_case* c = *j;
            fprintf(fp, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
                    c->suite->name, c->name, h2_cs[c->status], (c->t_end - c->t_start) / 1000.0);

            if (c->status == h2_case::FAILED) {
               fprintf(fp, "      <failure message=\"%s:%d:", c->file, c->line);

               for (auto k = c->fails.begin(); k != c->fails.end(); k++) {
                  H2_FAIL_FOREACH(fail, *k) {
                     fprintf(fp, "{newline}");
                     fail->print(fp);
                  }
               }
               fprintf(fp, "\" type=\"AssertionFailedError\"></failure>\n");
            }
            fprintf(fp, "      <system-out></system-out><system-err></system-err>\n");
            fprintf(fp, "    </testcase>\n");
         }
         fprintf(fp, "  </testsuite>\n");
      }
      fprintf(fp, "</testsuites>\n");

      fclose(fp);
   }
};