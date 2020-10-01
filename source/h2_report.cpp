
struct h2_report_impl {
   h2_list x;
   int suites = 0, cases = 0;
   int suite_index = 0, suite_case_index = 0, task_case_index = 0;
   long long task_cost = 0, suite_cost = 0, case_cost = 0;

   virtual void on_task_start(h2_task* t)
   {
      suites = t->suites.count();
      h2_list_for_each_entry (s, t->suites, h2_suite, x) {
         cases += s->cases.count();
      }
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

struct h2_report_console : h2_report_impl {
   void print_perfix(bool percentage)
   {
      static size_t last = 0;
      h2_color::printf("", h2_stdio::capture_length() == last ? "\r" : "\n");
      if (percentage && O.execute_progress) {
         h2_color::printf("dark gray", "[");
         h2_color::printf("", "%3d%%", cases ? (int)(task_case_index * 100 / cases) : 100);
         h2_color::printf("dark gray", "] ");
      }
      last = h2_stdio::capture_length();
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
      if (c++) h2_color::printf("dark gray", ", ");
      h2_color::printf(style, "%d", n);
      h2_color::printf("", " %s", name);
   }
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);
      if (O.list_cases) {
         h2_color::printf("bold,green", "Listing <%d suites, %d cases, %d todo>\n", suites, cases - t->stats[h2_case::todo], t->stats[h2_case::todo]);
      } else {
         print_perfix(false);
         if (O.verbose)
            h2_color::printf("", "\n");
         if (0 < t->stats[h2_case::failed])
            h2_color::printf("bold,red", "Failure ");
         else
            h2_color::printf("bold,green", "Success ");

         if (0 < nonzero_count(t->stats[h2_case::failed], t->stats[h2_case::todo], t->stats[h2_case::filtered], t->stats[h2_case::ignored]))
            h2_color::printf("dark gray", "(");

         int c = 0;
         comma_status(t->stats[h2_case::passed], "green", "passed", c);
         if (t->stats[h2_case::failed]) comma_status(t->stats[h2_case::failed], "red", "failed", c);
         if (t->stats[h2_case::todo]) comma_status(t->stats[h2_case::todo], "yellow", "todo", c);
         if (t->stats[h2_case::filtered]) comma_status(t->stats[h2_case::filtered], "blue", "filtered", c);
         if (t->stats[h2_case::ignored]) comma_status(t->stats[h2_case::ignored], "blue", "ignored", c);
         if (0 < nonzero_count(t->stats[h2_case::failed], t->stats[h2_case::todo], t->stats[h2_case::filtered], t->stats[h2_case::ignored])) {
            h2_color::printf("dark gray", ")");
            h2_color::printf("", " %d", cases);
         }
         h2_color::printf("", " case%s", 1 < cases ? "s" : "");
         h2_color::printf("dark gray", ", ");
         h2_color::printf("", "%d assert%s", t->asserts, 1 < t->asserts ? "s" : "");
         if (1 < t->rounds) {
            h2_color::printf("dark gray", ", ");
            h2_color::printf("", "%d rounds", t->rounds);
         }
         h2_color::printf("dark gray", ", ");
         h2_color::printf("", "%s \n", format_duration(task_cost));
      }
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      if (O.list_cases) {
         h2_color::printf("", "SUITE-%d. ", ++suite_index);
         h2_color::printf("bold,blue", "%s", s->name);
         h2_color::printf("", " %s:%d\n", s->file, s->lino);
      }
   }

   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.compact) return;
      if (O.verbose && O.includes.size() + O.excludes.size() == 0) {
         print_perfix(true);
         h2_color::printf("dark gray", "suite ");
         h2_color::printf("", "%s", s->name);
         if (1 < nonzero_count(s->stats[h2_case::passed], s->stats[h2_case::failed], s->stats[h2_case::todo], s->stats[h2_case::filtered], s->stats[h2_case::ignored]))
            h2_color::printf("dark gray", " (");
         else
            h2_color::printf("dark gray", " - ");

         int c = 0;
         if (s->stats[h2_case::passed]) comma_status(s->stats[h2_case::passed], "", "passed", c);
         if (s->stats[h2_case::failed]) comma_status(s->stats[h2_case::failed], "", "failed", c);
         if (s->stats[h2_case::todo]) comma_status(s->stats[h2_case::todo], "", "todo", c);
         if (s->stats[h2_case::filtered]) comma_status(s->stats[h2_case::filtered], "", "filtered", c);
         if (s->stats[h2_case::ignored]) comma_status(s->stats[h2_case::ignored], "", "ignored", c);

         if (1 < nonzero_count(s->stats[h2_case::passed], s->stats[h2_case::failed], s->stats[h2_case::todo], s->stats[h2_case::filtered], s->stats[h2_case::ignored]))
            h2_color::printf("dark gray", ")");
         if (0 < s->cases.count())
            h2_color::printf("", " case%s", 1 < s->cases.count() ? "s" : "");

         if (0 < s->asserts) {
            h2_color::printf("dark gray", ", ");
            h2_color::printf("", "%d assert%s", s->asserts, 1 < s->asserts ? "s" : "");
         }
         if (0 < s->footprint) {
            h2_color::printf("dark gray", ", ");
            h2_color::printf("", "%s", format_volume(s->footprint));
         }
         if (1 < suite_cost) {
            h2_color::printf("dark gray", ", ");
            h2_color::printf("", "%s", format_duration(suite_cost));
         }
         h2_color::printf("", "\n");
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (O.list_cases) {
         h2_color::printf("", " %s-%d. ", c->status == h2_case::todo ? "TODO" : "CASE", suite_case_index);
         h2_color::printf("cyan", "%s", c->name);
         h2_color::printf("", " %s:%d\n", basename((char*)c->file), c->lino);
      }
   }
   void print_title(const char* s, const char* c, const char* file, int lino)
   {
      if (strlen(c))
         h2_color::printf("", "%s", c);
      else
         h2_color::printf("dark gray", "case");
      h2_color::printf("dark gray", " | ");
      if (strlen(s))
         h2_color::printf("", "%s", s);
      else
         h2_color::printf("dark gray", "suite");
      h2_color::printf("dark gray", " | ");
      h2_color::printf("", "%s:%d", basename((char*)file), lino);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (O.list_cases) return;
      switch (c->status) {
      case h2_case::initial: break;
      case h2_case::todo:
         print_perfix(true);
         h2_color::printf("yellow", "Todo   ");
         print_title(s->name, c->name, c->file, c->lino);
         h2_color::printf("", "\n");
         break;
      case h2_case::filtered: break;
      case h2_case::passed:
         if (O.verbose) {
            print_perfix(true);
            h2_color::printf("green", "Passed ");
            print_title(s->name, c->name, c->file, c->lino);
            if (0 < c->asserts) {
               h2_color::printf("dark gray", " - ");
               h2_color::printf("", "%d assert%s", c->asserts, 1 < c->asserts ? "s" : "");
            }
            if (0 < c->footprint) {
               h2_color::printf("dark gray", ",");
               h2_color::printf("", " %s", format_volume(c->footprint));
            }
            if (1 < case_cost) {
               h2_color::printf("dark gray", ",");
               h2_color::printf("", " %s", format_duration(case_cost));
            }
            h2_color::printf("", "\n");
         } else if (!O.debug)
            print_perfix(true);
         break;
      case h2_case::failed:
         print_perfix(true);
         h2_color::printf("bold,red", "Failed ");
         print_title(s->name, c->name, c->file, c->lino);
         h2_color::printf("", "\n");
         if (O.compact) break;
         if (c->fails) c->fails->foreach([](h2_fail* fail, int subling_index, int child_index) { fail->print(subling_index, child_index); });
         h2_color::printf("", "\n");
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
   };
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats[h2_case::failed], s->name, s->stats[h2_case::todo] + s->stats[h2_case::filtered], s->cases.count(), 0, "");
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (!f) return;

      static constexpr const char* CSS[] = {"Initial", "Passed", "Failed", "TODO", "Filtered", "Ignored"};
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, CSS[c->status], case_cost / 1000.0);

      if (c->status == h2_case::failed) {
         fprintf(f, "<failure message=\"%s:%d:", c->file, c->lino);
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
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   I().reports.push_back(console_report.x);
   if (strlen(O.junit) && !O.list_cases) I().reports.push_back(junit_report.x);
   if (strlen(O.tap) && !O.list_cases) I().reports.push_back(tap_report.x);
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
