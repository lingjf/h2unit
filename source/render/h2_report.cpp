struct h2_report_impl {
   h2_list x;
   int runner_cases = 0, runner_case_index = 0;

   virtual void on_runner_start(h2_runner* r)
   {
      h2_list_for_each_entry (s, r->suites, h2_suite, x)
         runner_cases += s->cases.count();
      r->stats.timecost = h2_now();
   }
   virtual void on_runner_endup(h2_runner* r)
   {
      r->stats.timecost = h2_now() - r->stats.timecost;
   }
   virtual void on_suite_start(h2_suite* s)
   {
      s->stats.timecost = h2_now();
   }
   virtual void on_suite_endup(h2_suite* s)
   {
      s->stats.timecost = h2_now() - s->stats.timecost;
   }
   virtual void on_case_start(h2_suite* s, h2_case* c)
   {
      ++runner_case_index;
      c->stats.timecost = h2_now();
   }
   virtual void on_case_endup(h2_suite* s, h2_case* c)
   {
      c->stats.timecost = h2_now() - c->stats.timecost;
   }
};

#define H2_UNITS(count, unit) ((count > 1) ? (unit "s") : unit)

struct h2_report_list : h2_report_impl {
   int suites = 0, cases = 0, todos = 0;
   int unfiltered_suites = 0, unfiltered_cases = 0, unfiltered_todos = 0;
   int suite_cases = 0, suite_todos = 0;
   std::map<std::string, unsigned long> tags;

   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      if (O.lists & ListTag) {
         int i = 0;
         for (auto& tag : tags) {
            h2_line line;
            line.printf("dark gray", "TAG-%d. ", ++i).printf("bold,light purple", "%s ", tag.first.c_str());
            auto suite_count = (tag.second & 0xFFFF0000) >> 16;
            if (suite_count) line.printf("", " %d ", suite_count).printf("dark gray", H2_UNITS(suite_count, "suite"));
            auto case_count = tag.second & 0x0000FFFF;
            if (case_count) line.printf("", " %d ", case_count).printf("dark gray", H2_UNITS(case_count, "case"));
            h2_console::printl(line);
         }
      }

      h2_line line;
      if (O.lists & ListSuite) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_suites), "green") + " " + gray(H2_UNITS(unfiltered_suites, "suite"));
      if (O.lists & ListCase) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_cases), "green") + " " + gray(H2_UNITS(unfiltered_cases, "case"));
      if (O.lists & ListTodo) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_todos), "green") + " " + gray(H2_UNITS(unfiltered_todos, "todo"));
      if (O.lists & ListTag) line += gray(comma_if(line.width())) + color(h2_stringify(tags.size()), "green") + " " + gray(H2_UNITS(tags.size(), "tag"));
      if (O.lists & ListSuite && suites > unfiltered_suites) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), suites - unfiltered_suites, H2_UNITS(suites - unfiltered_suites, "suite"));
      if (O.lists & ListCase && cases > unfiltered_cases) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), cases - unfiltered_cases, H2_UNITS(cases - unfiltered_cases, "case"));
      if (O.lists & ListTodo && todos > unfiltered_todos) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), todos - unfiltered_todos, H2_UNITS(todos - unfiltered_todos, "todo"));
      h2_console::printl("Listing " + line);
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      suite_cases = 0;
      suite_todos = 0;
      if (s->absent()) return;  // CASE
      for (int i = 0; s->describe.tags[i]; ++i) tags[s->describe.tags[i]] += 0x10000;

      ++suites;
      if (!s->filtered && O.lists & ListSuite) {
         ++unfiltered_suites;
         h2_line line;
         line.printf("dark gray", "SUITE-%d. ", unfiltered_suites);
         h2_console::printl(line + color(s->describe.name, "bold,blue") + " " + gray(h2_basefile(s->filine)));
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      for (int i = 0; c->describe.tags[i]; ++i) tags[c->describe.tags[i]] += 0x1;

      const char* type = nullptr;
      if (c->todo) {
         ++todos;
         if (!c->filtered && O.lists & ListTodo) {
            type = s->absent() ? "TODO" : "Todo";
            ++unfiltered_todos, ++suite_todos;
         }
      } else {
         ++cases;
         if (!c->filtered && O.lists & ListCase) {
            type = s->absent() ? "CASE" : "Case";
            ++unfiltered_cases, ++suite_cases;
         }
      }

      if (type) {
         h2_line line;
         if (O.lists & ListSuite)
            line.printf("dark gray", " %s/%d-%d. ", type, suite_cases + suite_todos, unfiltered_cases + unfiltered_todos);
         else
            line.printf("dark gray", " %s-%d. ", type, unfiltered_cases + unfiltered_todos);
         h2_console::printl(line + color(c->describe.name, "cyan") + " " + gray(h2_basefile(c->filine)));
      }
   }
};

struct h2_report_console : h2_report_impl {
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   h2_line format_title(const char* suite_name, const char* case_name, const char* file_line)
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
      if (file_line) {
         title.printf("dark gray", "┊ ");
         title.printf("", "%s ", file_line);
      }
      return title;
   }
   void format_percentage(h2_line& bar)
   {
      bar.printf("dark gray", "[");
      bar.printf("", "%3d%%", runner_cases ? (int)(runner_case_index * 100 / runner_cases) : 100);
      bar.printf("dark gray", "] ");
   }
   static const char* format_volume(long long footprint, char* s = (char*)alloca(128))
   {
      if (footprint < 1024LL)
         sprintf(s, "%lld", footprint);
      else if (footprint < 1024LL * 1024LL)
         sprintf(s, "%.2gKB", footprint / 1024.0);
      else if (footprint < 1024LL * 1024LL * 1024LL)
         sprintf(s, "%.2gMB", footprint / (1024.0 * 1024.0));
      else
         sprintf(s, "%.2gGB", footprint / (1024.0 * 1024.0 * 1024.0));
      return s;
   }
   static const char* format_duration(long long ms, char* s = (char*)alloca(128))
   {
      if (ms < 100)
         sprintf(s, "%lld milliseconds", ms);
      else if (ms < 1000 * 60)
         sprintf(s, "%.2g second%s", ms / 1000.0, ms == 1000 ? "" : "s");
      else if (ms < 1000 * 60 * 60)
         sprintf(s, "%.2g minute%s", ms / 60000.0, ms == 60000 ? "" : "s");
      else
         sprintf(s, "%.2g hour%s", ms / 3600000.0, ms == 3600000 ? "" : "s");
      return s;
   }
   static const char* format_units(int count, const char* unit1, const char* unit2 = nullptr, char* s = (char*)alloca(128))
   {
      bool multiple = count > 1;
      sprintf(s, "%d%s%s", count, multiple && unit2 ? unit2 : unit1, multiple && !unit2 ? "s" : "");
      return s;
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
      if (s && c) bar += format_title(s->describe.name, c->describe.name, backable ? nullptr : h2_basefile(c->filine));
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

      int n = nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored);
      h2_line line = (0 < r->stats.failed) ? color("Failure ", "bold,red") : color("Success ", "bold,green");
      if (0 < n) line += gray("(");
      line += color(h2_stringify(r->stats.passed), "green") + " passed";  // always print
      if (r->stats.failed) line += gray(", ") + color(h2_stringify(r->stats.failed), "red") + " failed";
      if (r->stats.todo) line += gray(", ") + color(h2_stringify(r->stats.todo), "yellow") + " todo";
      if (r->stats.filtered) line += gray(", ") + color(h2_stringify(r->stats.filtered), "blue") + " filtered";
      if (r->stats.ignored) line += gray(", ") + color(h2_stringify(r->stats.ignored), "blue") + " ignored";
      if (0 < n) line += gray(") ") + h2_stringify(runner_cases);
      line += H2_UNITS(runner_cases, " case") + gray(", ") + format_units(r->stats.asserts, " assert");
      if (1 < r->rounds) line += gray(", ") + format_units(r->rounds, " round");
      line += gray(", ") + format_duration(r->stats.timecost);

      h2_console::printl(line);
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

         int n = nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored);
         h2_line line;
         if (s->stats.passed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.passed) + " passed";
         if (s->stats.failed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.failed) + " failed";
         if (s->stats.todo) line += gray(comma_if(line.width())) + h2_stringify(s->stats.todo) + " todo";
         if (s->stats.filtered) line += gray(comma_if(line.width())) + h2_stringify(s->stats.filtered) + " filtered";
         if (s->stats.ignored) line += gray(comma_if(line.width())) + h2_stringify(s->stats.ignored) + " ignored";
         line = gray("suite ") + s->describe.name + gray(1 < n ? " (" : " - ") + line + gray(1 < n ? ")" : "");
         if (0 < s->cases.count()) line += H2_UNITS(s->cases.count(), " case");
         if (0 < s->stats.asserts) line += gray(", ") + format_units(s->stats.asserts, " assert");
         if (0 < s->stats.footprint) line += gray(", ") + format_volume(s->stats.footprint) + " footprint";
         if (1 < s->stats.timecost) line += gray(", ") + format_duration(s->stats.timecost);

         h2_console::printl(line);
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
         if (O.verbose >= VerboseDetail) print_bar(true, "yellow", s->absent() ? "TODO   " : "Todo   ", s, c, false);
      } else if (c->failed) {
         if (O.verbose >= VerboseCompactFailed) {
            print_bar(true, "bold,red", "Failed ", s, c, false);
            h2_console::prints("", "\n");
            if (O.verbose >= VerboseNormal && c->fails) {
               c->fails->foreach([](h2_fail* fail, size_t si, size_t ci) { fail->print(si, ci); });
               h2_console::prints("", "\n");
            }
         }
      } else {  // Passed
         if (O.verbose >= VerboseDetail || O.verbose == VerboseCompactPassed) {
            print_bar(true, "green", "Passed ", s, c, false);
            if (O.verbose >= VerboseDetail) {
               h2_line line;
               if (0 < c->stats.asserts) line += gray(comma_if(line.width())) + format_units(c->stats.asserts, " assert");
               if (0 < c->stats.footprint) line += gray(comma_if(line.width())) + format_volume(c->stats.footprint) + " footprint";
               if (0 < c->stats.timecost) line += gray(comma_if(line.width())) + format_duration(c->stats.timecost);
               if (line.width()) h2_console::printl(gray("- ") + line, false);
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
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats.failed, s->describe.name, s->stats.todo + s->stats.filtered, s->cases.count(), 0, "");
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->describe.name, c->describe.name, c->todo ? "TODO" : (c->filtered ? "Filtered" : (c->ignored ? "Ignored" : (c->failed ? "Failed" : "Passed"))), c->stats.timecost / 1000.0);
      if (c->failed) {
         fprintf(f, "<failure message=\"%s:", c->filine);
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
   if (O.lists) {
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
