struct h2_report_impl {
   h2_list x;
   int suites = 0, cases = 0;
   int suite_index = 0, suite_case_index = 0, runner_case_index = 0;

   virtual void on_runner_start(h2_runner* r)
   {
      suites = r->suites.count();
      h2_list_for_each_entry (s, r->suites, h2_suite, x)
         cases += s->cases.count();
      r->stats.timecost = h2_now();
   }
   virtual void on_runner_endup(h2_runner* r)
   {
      r->stats.timecost = h2_now() - r->stats.timecost;
   }
   virtual void on_suite_start(h2_suite* s)
   {
      suite_case_index = 0;
      s->stats.timecost = h2_now();
   }
   virtual void on_suite_endup(h2_suite* s)
   {
      s->stats.timecost = h2_now() - s->stats.timecost;
   }
   virtual void on_case_start(h2_suite* s, h2_case* c)
   {
      ++suite_case_index;
      c->stats.timecost = h2_now();
   }
   virtual void on_case_endup(h2_suite* s, h2_case* c)
   {
      ++runner_case_index;
      c->stats.timecost = h2_now() - c->stats.timecost;
   }
};

struct h2_report_list : h2_report_impl {
   int unfiltered_suite_index = 0, unfiltered_suite_case_index = 0, unfiltered_runner_case_index = 0;
   bool option_has(const char* type)
   {
      for (auto t : O.list_cases)
         if (!strcasecmp("all", t) || !strcasecmp(type, t)) return true;
      return false;
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      h2_console::prints("bold,green", "Listing <%d suites, %d cases", unfiltered_suite_index, unfiltered_runner_case_index);
      if (runner_case_index - unfiltered_runner_case_index) h2_console::prints("bold,green", ", %d filtered", runner_case_index - unfiltered_runner_case_index);
      h2_console::prints("bold,green", ">\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      unfiltered_suite_case_index = 0;
      if (!s->name) return;  // CASE
      if (s->filtered) return;
      ++unfiltered_suite_index;
      if (option_has("suite")) {
         h2_console::prints("dark gray", "SUITE-%d. ", unfiltered_suite_index);
         h2_console::prints("bold,blue", "%s", s->name);
         h2_console::prints("dark gray", " %s\n", s->file);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (s->filtered) return;
      const char* type = nullptr;
      if (c->todo) {
         if (option_has("todo")) type = s->name ? "Todo" : "TODO";
      } else {
         if (option_has("case")) type = s->name ? "Case" : "CASE";
      }

      if (type) {
         ++unfiltered_runner_case_index, ++unfiltered_suite_case_index;
         if (option_has("suite"))
            h2_console::prints("dark gray", " %s/%d-%d. ", type, unfiltered_suite_case_index, unfiltered_runner_case_index);
         else
            h2_console::prints("dark gray", " %s-%d. ", type, unfiltered_runner_case_index);
         h2_console::prints("cyan", "%s", c->name);
         h2_console::prints("dark gray", " %s\n", h2_basefile(c->file));
      }
   }
};

struct h2_report_console : h2_report_impl {
   void comma_status(int n, const char* style, const char* name, int& c)
   {
      if (c++) h2_console::prints("dark gray", ", ");
      h2_console::prints(style, "%d", n);
      h2_console::prints("", " %s", name);
   }
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   h2_line format_title(const char* suite_name, const char* case_name, const char* file)
   {
      h2_line title;
      title.printf("dark gray", "┊ ");
      if (strlen(case_name))
         title.printf("", "%s ", case_name);
      else
         title.printf("dark gray", "case ");
      if (suite_name) {
         title.printf("dark gray", "┊ ");
         if (strlen(suite_name))
            title.printf("", "%s ", suite_name);
         else
            title.printf("dark gray", "suite ");
      }
      if (file) {
         title.printf("dark gray", "┊ ");
         title.printf("", "%s ", file);
      }
      return title;
   }
   void format_percentage(h2_line& bar)
   {
      bar.printf("dark gray", "[");
      bar.printf("", "%3d%%", cases ? (int)(runner_case_index * 100 / cases) : 100);
      bar.printf("dark gray", "] ");
   }
   void print_bar(bool percentage, const char* status_style, const char* status, h2_suite* s, h2_case* c, bool backable)
   {
      static long long last_capture_length = 0;
      if (last_capture_length == h2_stdio::I().capture_length)
         h2_console::prints("", "\33[2K\r"); /* clear line */
      else
         h2_console::prints("", "\n"); /* user output, new line */
      last_capture_length = h2_stdio::I().capture_length;
      h2_report::I().backable = O.progressing && backable;

      if (!O.progressing && backable) return;
      h2_line bar;
      if (percentage && O.progressing) format_percentage(bar);
      if (status && status_style) bar.printf(status_style, "%s", status);
      if (s && c) bar += format_title(s->name, c->name, backable ? nullptr : h2_basefile(c->file));
      if (backable) {
         if (h2_console::width() > bar.width())
            bar.padding(h2_console::width() - bar.width());
         else
            bar = bar.abbreviate(h2_console::width());
      }
      h2_console::printl(bar, false);
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      print_bar(false, nullptr, nullptr, nullptr, nullptr, false);
      if (0 < r->stats.failed)
         h2_console::prints("bold,red", "Failure ");
      else
         h2_console::prints("bold,green", "Success ");

      if (0 < nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored))
         h2_console::prints("dark gray", "(");

      int c = 0;
      comma_status(r->stats.passed, "green", "passed", c);
      if (r->stats.failed) comma_status(r->stats.failed, "red", "failed", c);
      if (r->stats.todo) comma_status(r->stats.todo, "yellow", "todo", c);
      if (r->stats.filtered) comma_status(r->stats.filtered, "blue", "filtered", c);
      if (r->stats.ignored) comma_status(r->stats.ignored, "blue", "ignored", c);
      if (0 < nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored)) {
         h2_console::prints("dark gray", ")");
         h2_console::prints("", " %d", cases);
      }
      h2_console::prints("", " case%s", 1 < cases ? "s" : "");
      h2_console::prints("dark gray", ", ");
      h2_console::prints("", "%d assert%s", r->stats.asserts, 1 < r->stats.asserts ? "s" : "");
      if (1 < r->rounds) {
         h2_console::prints("dark gray", ", ");
         h2_console::prints("", "%d rounds", r->rounds);
      }
      h2_console::prints("dark gray", ", ");
      h2_console::prints("", "%s \n", format_duration(r->stats.timecost));
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.verbose >= 9 && O.includes.size() + O.excludes.size() == 0) {
         print_bar(true, nullptr, nullptr, nullptr, nullptr, false);
         h2_console::prints("dark gray", "suite ");
         h2_console::prints("", "%s", ss(s->name));
         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_console::prints("dark gray", " (");
         else
            h2_console::prints("dark gray", " - ");

         int c = 0;
         if (s->stats.passed) comma_status(s->stats.passed, "", "passed", c);
         if (s->stats.failed) comma_status(s->stats.failed, "", "failed", c);
         if (s->stats.todo) comma_status(s->stats.todo, "", "todo", c);
         if (s->stats.filtered) comma_status(s->stats.filtered, "", "filtered", c);
         if (s->stats.ignored) comma_status(s->stats.ignored, "", "ignored", c);

         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_console::prints("dark gray", ")");
         if (0 < s->cases.count())
            h2_console::prints("", " case%s", 1 < s->cases.count() ? "s" : "");

         if (0 < s->stats.asserts) {
            h2_console::prints("dark gray", ", ");
            h2_console::prints("", "%d assert%s", s->stats.asserts, 1 < s->stats.asserts ? "s" : "");
         }
         if (0 < s->stats.footprint) {
            h2_console::prints("dark gray", ", ");
            h2_console::prints("", "%s footprint", format_volume(s->stats.footprint));
         }
         if (1 < s->stats.timecost) {
            h2_console::prints("dark gray", ", ");
            h2_console::prints("", "%s", format_duration(s->stats.timecost));
         }
         h2_console::prints("", "\n");
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (c->filtered || c->ignored || c->todo) return;
      print_bar(true, "light blue", "Testing", s, c, true);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (c->filtered || c->ignored) return;
      if (c->todo) {
         if (O.verbose >= verbose_detail) print_bar(true, "yellow", s->name ? "Todo   " : "TODO   ", s, c, false);
      } else if (c->failed) {
         if (O.verbose >= verbose_compact_failed) {
            print_bar(true, "bold,red", "Failed ", s, c, false);
            h2_console::prints("", "\n");
            if (O.verbose >= verbose_normal && c->fails) {
               c->fails->foreach([](h2_fail* fail, size_t si, size_t ci) { fail->print(si, ci); });
               h2_console::prints("", "\n");
            }
         }
      } else {  // Passed
         if (O.verbose >= verbose_detail || O.verbose == verbose_compact_passed) {
            print_bar(true, "green", "Passed ", s, c, false);
            if (O.verbose >= verbose_detail) {
               h2_line ad;
               if (0 < c->stats.asserts) ad.printf("dark gray", ad.width() ? ", " : "").printf("", "%d assert%s", c->stats.asserts, 1 < c->stats.asserts ? "s" : "");
               if (0 < c->stats.footprint) ad.printf("dark gray", ad.width() ? ", " : "").printf("", "%s footprint", format_volume(c->stats.footprint));
               if (0 < c->stats.timecost) ad.printf("dark gray", ad.width() ? ", " : "").printf("", "%s", format_duration(c->stats.timecost));
               if (ad.width()) h2_console::printl(gray("- ") + ad, false);
            }
            h2_console::prints("", "\n");
         }
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
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, c->todo ? "TODO" : (c->filtered ? "Filtered" : (c->ignored ? "Ignored" : (c->failed ? "Failed" : "Passed"))), c->stats.timecost / 1000.0);
      if (c->failed) {
         fprintf(f, "<failure message=\"%s:", c->file);
         if (c->fails) c->fails->foreach([&](h2_fail* fail, size_t si, size_t ci) {fprintf(f, "{newline}"); fail->print(f); });
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
   if (O.list_cases.size()) {
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
