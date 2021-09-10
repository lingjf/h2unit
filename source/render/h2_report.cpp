struct h2_report_impl {
   h2_list x;
   int suites = 0, cases = 0;
   int suite_index = 0, suite_case_index = 0, runner_case_index = 0;
   clock_t runner_cost = 0, suite_cost = 0, case_cost = 0;

   virtual void on_runner_start(h2_runner* r)
   {
      suites = r->suites.count();
      h2_list_for_each_entry (s, r->suites, h2_suite, x)
         cases += s->cases.count();
      runner_cost = ::clock();
   }
   virtual void on_runner_endup(h2_runner* r)
   {
      runner_cost = ::clock() - runner_cost;
   }
   virtual void on_suite_start(h2_suite* s)
   {
      suite_case_index = 0;
      suite_cost = ::clock();
   }
   virtual void on_suite_endup(h2_suite* s)
   {
      suite_cost = ::clock() - suite_cost;
   }
   virtual void on_case_start(h2_suite* s, h2_case* c)
   {
      ++suite_case_index;
      case_cost = ::clock();
   }
   virtual void on_case_endup(h2_suite* s, h2_case* c)
   {
      ++runner_case_index;
      case_cost = ::clock() - case_cost;
   }
};

struct h2_report_list : h2_report_impl {
   int unfiltered_suite_index = 0, unfiltered_suite_case_index = 0, unfiltered_runner_case_index = 0, todo = 0;

   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      h2_color::prints("bold,green", "Listing <%d suites, %d cases", unfiltered_suite_index, unfiltered_runner_case_index);
      if (todo) h2_color::prints("bold,green", ", %d todo", todo);
      if (runner_case_index - unfiltered_runner_case_index) h2_color::prints("bold,green", ", %d filtered", runner_case_index - unfiltered_runner_case_index);
      h2_color::prints("bold,green", ">\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      unfiltered_suite_case_index = 0;
      if (!s->name) return;  // CASE
      if (s->filtered) return;
      ++unfiltered_suite_index;
      h2_color::prints("dark gray", "SUITE-%d. ", unfiltered_suite_index);
      h2_color::prints("bold,blue", "%s", s->name);
      h2_color::prints("dark gray", " %s:%d\n", s->file, s->line);
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (s->filtered) return;
      ++unfiltered_runner_case_index, ++unfiltered_suite_case_index;
      if (c->todo) todo++;
      if (s->name)
         h2_color::prints("dark gray", " %s/%d-%d. ", c->todo ? "Todo" : "Case", unfiltered_suite_case_index, unfiltered_runner_case_index);
      else
         h2_color::prints("dark gray", " %s-%d. ", c->todo ? "TODO" : "CASE", unfiltered_runner_case_index);
      h2_color::prints("cyan", "%s", c->name);
      h2_color::prints("dark gray", " %s:%d\n", h2_basename(c->file), c->line);
   }
};

struct h2_report_console : h2_report_impl {
   void print_perfix(bool percentage)
   {
      static size_t s_last = 0;
      h2_color::prints("", s_last == h2_stdio::I().capture_length ? "\r" : "\n");
      if (percentage && O.percent_progressing) {
         h2_color::prints("dark gray", "[");
         h2_color::prints("", "%3d%%", cases ? (int)(runner_case_index * 100 / cases) : 100);
         h2_color::prints("dark gray", "] ");
      }
      s_last = h2_stdio::I().capture_length;
   }
   const char* format_duration(clock_t ticks)
   {
      double ms = ticks * 1000.0 / CLOCKS_PER_SEC;
      static char st[128];
      if (ms < 100)
         sprintf(st, "%d milliseconds", (int)ceil(ms));
      else if (ms < 1000 * 60)
         sprintf(st, "%.2g seconds", ms / 1000.0);
      else if (ms < 1000 * 60 * 60)
         sprintf(st, "%.2g minutes", ms / 6000.0);
      else
         sprintf(st, "%.2g hours", ms / 36000.0);

      return st;
   }
   const char* format_volume(long long footprint)
   {
      static char st[128];
      if (footprint < 1024)
         sprintf(st, "%lld footprint", footprint);
      else if (footprint < 1024 * 1024LL)
         sprintf(st, "%.2gKB footprint", footprint / (double)1024);
      else if (footprint < 1024 * 1024 * 1024LL)
         sprintf(st, "%.2gMB footprint", footprint / (double)(1024 * 1024LL));
      else
         sprintf(st, "%.2gGB footprint", footprint / (double)(1024 * 1024 * 1024LL));
      return st;
   }
   void comma_status(int n, const char* style, const char* name, int& c)
   {
      if (c++) h2_color::prints("dark gray", ", ");
      h2_color::prints(style, "%d", n);
      h2_color::prints("", " %s", name);
   }
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      print_perfix(false);
      if (O.verbose >= 2)
         h2_color::prints("", "\n");
      if (0 < r->stats.failed)
         h2_color::prints("bold,red", "Failure ");
      else
         h2_color::prints("bold,green", "Success ");

      if (0 < nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored))
         h2_color::prints("dark gray", "(");

      int c = 0;
      comma_status(r->stats.passed, "green", "passed", c);
      if (r->stats.failed) comma_status(r->stats.failed, "red", "failed", c);
      if (r->stats.todo) comma_status(r->stats.todo, "yellow", "todo", c);
      if (r->stats.filtered) comma_status(r->stats.filtered, "blue", "filtered", c);
      if (r->stats.ignored) comma_status(r->stats.ignored, "blue", "ignored", c);
      if (0 < nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored)) {
         h2_color::prints("dark gray", ")");
         h2_color::prints("", " %d", cases);
      }
      h2_color::prints("", " case%s", 1 < cases ? "s" : "");
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%d assert%s", r->stats.asserts, 1 < r->stats.asserts ? "s" : "");
      if (1 < r->rounds) {
         h2_color::prints("dark gray", ", ");
         h2_color::prints("", "%d rounds", r->rounds);
      }
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%s \n", format_duration(runner_cost));
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
   }

   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.verbose >= 2 && O.includes.size() + O.excludes.size() == 0) {
         print_perfix(true);
         h2_color::prints("dark gray", "suite ");
         h2_color::prints("", "%s", ss(s->name));
         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_color::prints("dark gray", " (");
         else
            h2_color::prints("dark gray", " - ");

         int c = 0;
         if (s->stats.passed) comma_status(s->stats.passed, "", "passed", c);
         if (s->stats.failed) comma_status(s->stats.failed, "", "failed", c);
         if (s->stats.todo) comma_status(s->stats.todo, "", "todo", c);
         if (s->stats.filtered) comma_status(s->stats.filtered, "", "filtered", c);
         if (s->stats.ignored) comma_status(s->stats.ignored, "", "ignored", c);

         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_color::prints("dark gray", ")");
         if (0 < s->cases.count())
            h2_color::prints("", " case%s", 1 < s->cases.count() ? "s" : "");

         if (0 < s->stats.asserts) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%d assert%s", s->stats.asserts, 1 < s->stats.asserts ? "s" : "");
         }
         if (0 < s->stats.footprint) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%s", format_volume(s->stats.footprint));
         }
         if (1 < suite_cost) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%s", format_duration(suite_cost));
         }
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      print_perfix(true);
   }
   void print_title(const char* s, const char* c, const char* file, int line)
   {
      if (strlen(c))
         h2_color::prints("", "%s", c);
      else
         h2_color::prints("dark gray", "case");
      if (s) {
         h2_color::prints("dark gray", " | ");
         if (strlen(s))
            h2_color::prints("", "%s", s);
         else
            h2_color::prints("dark gray", "   ");
      }
      h2_color::prints("dark gray", " | ");
      h2_color::prints("", "%s:%d", h2_basename(file), line);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (c->filtered || c->ignored) return;
      if (c->todo) {
         print_perfix(true);
         h2_color::prints("yellow", s->name ? "Todo   " : "TODO   ");
         print_title(s->name, c->name, c->file, c->line);
         return;
      }
      if (c->failed) {
         print_perfix(true);
         h2_color::prints("bold,red", "Failed ");
         print_title(s->name, c->name, c->file, c->line);
         if (O.verbose >= 1) {
            h2_color::prints("", "\n");
            if (c->fails) c->fails->foreach([](h2_fail* fail, int subling_index, int child_index) { fail->print(subling_index, child_index); });
         }
      } else {
         if (O.verbose >= 2) {
            print_perfix(true);
            h2_color::prints("green", "Passed ");
            print_title(s->name, c->name, c->file, c->line);
            if (0 < c->asserts) {
               h2_color::prints("dark gray", " - ");
               h2_color::prints("", "%d assert%s", c->asserts, 1 < c->asserts ? "s" : "");
            }
            if (0 < c->footprint) {
               h2_color::prints("dark gray", ",");
               h2_color::prints("", " %s", format_volume(c->footprint));
            }
            if (1 < case_cost) {
               h2_color::prints("dark gray", ",");
               h2_color::prints("", " %s", format_duration(case_cost));
            }
         } else if (!O.debug)
            print_perfix(true);
      }
   }
};

struct h2_report_junit : h2_report_impl {
   FILE* f;
   void on_runner_start(h2_runner* r) override
   {
      h2_report_impl::on_runner_start(r);
      f = fopen(O.junit_path, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats.failed, s->name, s->stats.todo + s->stats.filtered, s->cases.count(), 0, "");
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, c->todo ? "TODO" : (c->filtered ? "Filtered" : (c->ignored ? "Ignored" : (c->failed ? "Failed" : "Passed"))), case_cost / 1000.0);
      if (c->failed) {
         fprintf(f, "<failure message=\"%s:%d:", c->file, c->line);
         if (c->fails) c->fails->foreach([=](h2_fail* fail, int subling_index, int child_index) {fprintf(f, "{newline}"); fail->print(f); });
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "<system-out></system-out><system-err></system-err>\n");
      fprintf(f, "</testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_report_tap : h2_report_impl {
   /* */
};

h2_inline void h2_report::initialize()
{
   static h2_report_list list_report;
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   if (O.list_cases) {
      I().reports.push_back(list_report.x);
   } else {
      I().reports.push_back(console_report.x);
      if (strlen(O.junit_path)) I().reports.push_back(junit_report.x);
      if (strlen(O.tap_path)) I().reports.push_back(tap_report.x);
   }
}

/* clang-format off */
h2_inline void h2_report::on_runner_start(h2_runner* r) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_runner_start(r); }
h2_inline void h2_report::on_runner_endup(h2_runner* r) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_runner_endup(r); }
h2_inline void h2_report::on_suite_start(h2_suite* s) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_suite_start(s); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_suite_endup(s); }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_case_start(s, c); }
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_case_endup(s, c); }
