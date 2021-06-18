
struct h2_report_impl {
   h2_list x;
   int suites = 0, cases = 0;
   int suite_index = 0, suite_case_index = 0, task_case_index = 0;
   long long task_cost = 0, suite_cost = 0, case_cost = 0;

   virtual void on_task_start(h2_task* t)
   {
      suites = t->suites.count();
      h2_list_for_each_entry (s, t->suites, h2_suite, x)
         cases += s->cases.count();
      task_cost = h2_now();
   }
   virtual void on_task_endup(h2_task* t)
   {
      task_cost = h2_now() - task_cost;
   }
   virtual void on_suite_start(h2_suite* s)
   {
      suite_case_index = 0;
      suite_cost = h2_now();
   }
   virtual void on_suite_endup(h2_suite* s)
   {
      suite_cost = h2_now() - suite_cost;
   }
   virtual void on_case_start(h2_suite* s, h2_case* c)
   {
      ++suite_case_index;
      case_cost = h2_now();
   }
   virtual void on_case_endup(h2_suite* s, h2_case* c)
   {
      ++task_case_index;
      case_cost = h2_now() - case_cost;
   }
};

struct h2_report_list : h2_report_impl {
   int unfiltered_suite_index = 0, unfiltered_suite_case_index = 0, unfiltered_task_case_index = 0, todo = 0;

   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);
      h2_color::prints("bold,green", "Listing <%d suites, %d cases", unfiltered_suite_index, unfiltered_task_case_index);
      if (todo) h2_color::prints("bold,green", ", %d todo", todo);
      if (task_case_index - unfiltered_task_case_index) h2_color::prints("bold,green", ", %d filtered", task_case_index - unfiltered_task_case_index);
      h2_color::prints("bold,green", ">\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      unfiltered_suite_case_index = 0;
      if (!s->name) return;  // CASE
      if ((O.list_cases == 'l' && !s->filtered) || (O.list_cases == 'L' && s->filtered)) {
         h2_color::prints("dark gray", "SUITE-%d. ", ++unfiltered_suite_index);
         h2_color::prints("bold,blue", "%s", s->name);
         h2_color::prints("dark gray", " %s:%d\n", s->file, s->line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if ((O.list_cases == 'l' && !c->filtered) || (O.list_cases == 'L' && c->filtered)) {
         ++unfiltered_task_case_index, ++unfiltered_suite_case_index;
         if (c->todo) todo++;
         h2_color::prints("dark gray", "%s-%d. ", c->todo ? (s->name ? " Todo" : "TODO") : (s->name ? " Case" : "CASE"), unfiltered_suite_case_index);
         h2_color::prints("cyan", "%s", c->name);
         h2_color::prints("dark gray", " %s:%d\n", h2_basename(c->file), c->line);
      }
   }
};

struct h2_report_console : h2_report_impl {
   void print_perfix(bool percentage)
   {
      static size_t last = 0;
      h2_color::prints("", h2_cout::length() == last ? "\r" : "\n");
      if (percentage && O.execute_progress) {
         h2_color::prints("dark gray", "[");
         h2_color::prints("", "%3d%%", cases ? (int)(task_case_index * 100 / cases) : 100);
         h2_color::prints("dark gray", "] ");
      }
      last = h2_cout::length();
   }
   const char* format_duration(long long ms)
   {
      static char st[128];
      if (ms < 100)
         sprintf(st, "%lld milliseconds", ms);
      else if (ms < 1000 * 60)
         sprintf(st, "%.2g seconds", ms / (double)1000.0);
      else if (ms < 1000 * 60 * 60)
         sprintf(st, "%.2g minutes", ms / (double)6000.0);
      else
         sprintf(st, "%.2g hours", ms / (double)36000.0);

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
   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);

      print_perfix(false);
      if (O.verbose)
         h2_color::prints("", "\n");
      if (0 < t->stats.failed)
         h2_color::prints("bold,red", "Failure ");
      else
         h2_color::prints("bold,green", "Success ");

      if (0 < nonzero_count(t->stats.failed, t->stats.todo, t->stats.filtered, t->stats.ignored))
         h2_color::prints("dark gray", "(");

      int c = 0;
      comma_status(t->stats.passed, "green", "passed", c);
      if (t->stats.failed) comma_status(t->stats.failed, "red", "failed", c);
      if (t->stats.todo) comma_status(t->stats.todo, "yellow", "todo", c);
      if (t->stats.filtered) comma_status(t->stats.filtered, "blue", "filtered", c);
      if (t->stats.ignored) comma_status(t->stats.ignored, "blue", "ignored", c);
      if (0 < nonzero_count(t->stats.failed, t->stats.todo, t->stats.filtered, t->stats.ignored)) {
         h2_color::prints("dark gray", ")");
         h2_color::prints("", " %d", cases);
      }
      h2_color::prints("", " case%s", 1 < cases ? "s" : "");
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%d assert%s", t->stats.asserts, 1 < t->stats.asserts ? "s" : "");
      if (1 < t->rounds) {
         h2_color::prints("dark gray", ", ");
         h2_color::prints("", "%d rounds", t->rounds);
      }
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%s \n", format_duration(task_cost));
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
   }

   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.compact) return;
      if (O.verbose && O.includes.size() + O.excludes.size() == 0) {
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
         h2_color::prints("", "\n");
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
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
      if (c->filtered) return;
      if (c->todo) {
         print_perfix(true);
         h2_color::prints("yellow", s->name ? "Todo   " : "TODO   ");
         print_title(s->name, c->name, c->file, c->line);
         h2_color::prints("", "\n");
         return;
      }
      switch (c->status) {
      case h2_case::passed:
         if (O.verbose) {
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
            h2_color::prints("", "\n");
         } else if (!O.debug)
            print_perfix(true);
         break;
      case h2_case::failed:
         print_perfix(true);
         h2_color::prints("bold,red", "Failed ");
         print_title(s->name, c->name, c->file, c->line);
         h2_color::prints("", "\n");
         if (O.compact) break;
         if (c->fails) c->fails->foreach([](h2_fail* fail, int subling_index, int child_index) { fail->print(subling_index, child_index); });
         h2_color::prints("", "\n");
         break;
      }
   }
};

struct h2_report_junit : h2_report_impl {
   FILE* f;
   void on_task_start(h2_task* t) override
   {
      h2_report_impl::on_task_start(t);
      f = fopen(O.junit, "w");
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

      static constexpr const char* CSS[] = {"Initial", "Passed", "Failed", "Ignored"};
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, c->todo ? "TODO" : (c->filtered ? "Filtered" : CSS[c->status]), case_cost / 1000.0);

      if (c->status == h2_case::failed) {
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
   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);
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
      if (strlen(O.junit)) I().reports.push_back(junit_report.x);
      if (strlen(O.tap)) I().reports.push_back(tap_report.x);
   }
}

h2_inline void h2_report::on_task_start(h2_task* t)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_task_start(t);
}
h2_inline void h2_report::on_task_endup(h2_task* t)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_task_endup(t);
}
h2_inline void h2_report::on_suite_start(h2_suite* s)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_suite_start(s);
}
h2_inline void h2_report::on_suite_endup(h2_suite* s)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_suite_endup(s);
}
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_case_start(s, c);
}
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_case_endup(s, c);
}
