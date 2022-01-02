#define H2_UNITS(count, unit) ((count > 1) ? (unit "s") : unit)

struct h2_report_console : h2_report_interface {
   size_t cases = 0, index = 0, last_capture_length = 0;

   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   h2_line format_title(const char* suite_name, const char* case_name, const char* file_line)
   {
      h2_line title;
      title.printf("dark gray", "┊ ");
      if (strlen(case_name)) title.printf("", "%s ", case_name);
      else title.printf("dark gray", "case ");
      if (suite_name) {
         title.printf("dark gray", "┊ ");
         if (strlen(suite_name)) title.printf("", "%s ", suite_name);
         else title.printf("dark gray", "suite ");
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
      bar.printf("", "%3d%%", cases ? (int)(index * 100 / cases) : 100);
      bar.printf("dark gray", "] ");
   }
   static const char* format_volume(long long footprint, char* s = (char*)alloca(128))
   {
      if (footprint < 1024LL) sprintf(s, "%lld", footprint);
      else if (footprint < 1024LL * 1024LL) sprintf(s, "%.1fKB", footprint / 1024.0);
      else if (footprint < 1024LL * 1024LL * 1024LL) sprintf(s, "%.2fMB", footprint / (1024.0 * 1024.0));
      else sprintf(s, "%.3fGB", footprint / (1024.0 * 1024.0 * 1024.0));
      return s;
   }
   static const char* format_duration(long long ms, char* s = (char*)alloca(128))
   {
      if (ms < 100) sprintf(s, "%lld milliseconds", ms);
      else if (ms < 1000 * 60) sprintf(s, "%.1f second%s", ms / 1000.0, ms == 1000 ? "" : "s");
      else if (ms < 1000 * 60 * 60) sprintf(s, "%.2f minute%s", ms / 60000.0, ms == 60000 ? "" : "s");
      else sprintf(s, "%.3f hour%s", ms / 3600000.0, ms == 3600000 ? "" : "s");
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
      const char* new_line = last_capture_length == h2_stdio::I().capture_length ? "\33[2K\r" /* clear line */ : /* user output */ "\n";
      h2_console::prints("", new_line);
      last_capture_length = h2_stdio::I().capture_length;
      h2_report::I().backable = O.progressing && backable;

      if (!O.progressing && backable) return;
      h2_line bar;
      if (percentage && O.progressing) format_percentage(bar);
      if (status && status_style) bar.printf(status_style, "%s", status);
      if (s && c) bar += format_title(s->name, c->name, backable ? nullptr : h2_basefile(c->filine));
      if (backable) {
         if (h2_console::width() > bar.width()) bar.padding(h2_console::width() - bar.width());
         else bar = bar.abbreviate(h2_console::width());
      }
      h2_console::printl(bar, false);
   }
   void on_runner_start(h2_runner* r) override
   {
      h2_list_for_each_entry (s, r->suites, h2_suite, x)
         cases += s->cases.count();
   }
   void on_runner_endup(h2_runner* r) override
   {
      print_bar(false, nullptr, nullptr, nullptr, nullptr, false);

      int n = nonzero_count(r->stats.failed, r->stats.warning, r->stats.todo, r->stats.filtered, r->stats.ignored);
      h2_line line = (0 < r->stats.failed) ? color("Failure ", "bold,red") : color("Success ", "bold,green");
      if (0 < n) line += gray("(");
      line += color(h2_stringify(r->stats.passed), "green") + " passed";  // always print
      if (r->stats.failed) line += gray(", ") + color(h2_stringify(r->stats.failed), "red") + " failed";
      if (r->stats.warning) line += gray(", ") + color(h2_stringify(r->stats.warning), "cyan") + " warning";
      if (r->stats.todo) line += gray(", ") + color(h2_stringify(r->stats.todo), "yellow") + " todo";
      if (r->stats.filtered) line += gray(", ") + color(h2_stringify(r->stats.filtered), "blue") + " filtered";
      if (r->stats.ignored) line += gray(", ") + color(h2_stringify(r->stats.ignored), "blue") + " ignored";
      if (0 < n) line += gray(") ") + h2_stringify(cases);
      line += H2_UNITS(cases, " case") + gray(", ") + format_units(r->stats.asserts, " assert");
      if (1 < r->rounds) line += gray(", ") + format_units(r->rounds, " round");
      line += gray(", ") + format_duration(r->stats.timecost);

      h2_console::printl(line);
   }
   void on_suite_start(h2_suite* s) override {}
   void on_suite_endup(h2_suite* s) override
   {
      if (O.verbose >= 9 && !(O.includes[0] || O.excludes[0])) {
         print_bar(true, nullptr, nullptr, nullptr, nullptr, false);

         int n = nonzero_count(s->stats.passed, s->stats.warning, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored);
         h2_line line;
         if (s->stats.passed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.passed) + " passed";
         if (s->stats.failed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.failed) + " failed";
         if (s->stats.warning) line += gray(comma_if(line.width())) + h2_stringify(s->stats.warning) + " warning";
         if (s->stats.todo) line += gray(comma_if(line.width())) + h2_stringify(s->stats.todo) + " todo";
         if (s->stats.filtered) line += gray(comma_if(line.width())) + h2_stringify(s->stats.filtered) + " filtered";
         if (s->stats.ignored) line += gray(comma_if(line.width())) + h2_stringify(s->stats.ignored) + " ignored";
         line = gray("suite ") + s->name + gray(1 < n ? " (" : " - ") + line + gray(1 < n ? ")" : "");
         if (0 < s->cases.count()) line += H2_UNITS(s->cases.count(), " case");
         if (0 < s->stats.asserts) line += gray(", ") + format_units(s->stats.asserts, " assert");
         if (0 < s->stats.footprint) line += gray(", ") + format_volume(s->stats.footprint) + " footprint";
         if (1 < s->stats.timecost) line += gray(", ") + format_duration(s->stats.timecost);

         h2_console::printl(line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      ++index;
      if (c->filtered || c->ignored || c->todo) return;
      print_bar(true, "light blue", "Testing", s, c, true);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
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
      } else if (c->warning) {
         if (O.verbose >= VerboseCompactWarning) {
            print_bar(true, "bold,cyan", "Warning", s, c, false);
            h2_console::prints("", "\n");
            if (O.verbose > VerboseNormal && c->fails) {
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
