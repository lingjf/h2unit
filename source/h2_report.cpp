
h2_inline h2_report::h2_report() : total_cases(0), done_cases(0), percentage(0), ss(0), cs(0), tt(0), ts(0), tc(0) {}
h2_inline void h2_report::on_task_start(int cases)
{
   total_cases = cases;
   tt = h2_now();
}
h2_inline void h2_report::on_task_endup(int status_stats[8], int round) { tt = h2_now() - tt; };
h2_inline void h2_report::on_suite_start(h2_suite* s) { ts = h2_now(); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { ts = h2_now() - ts; }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { tc = h2_now(); };
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c)
{
   percentage = ++done_cases * 100 / total_cases;
   tc = h2_now() - tc;
}

struct h2_report_console : h2_report {
   void print_percentage(bool percent = true, bool number = false)
   {
      static size_t last = 0;
      h2_color::printf("", h2_stdio::get_length() == last ? "\r" : "\n");
      if (percent) {
         h2_color::printf("dark gray", "[");
         h2_color::printf("", "%3d%%", percentage);
         h2_color::printf("dark gray", "] ");
      }
      if (number) {
         h2_color::printf("dark gray", "(");
         h2_color::printf("", "%d", done_cases);
         h2_color::printf("dark gray", "/");
         h2_color::printf("", "%d", total_cases);
         h2_color::printf("dark gray", ") ");
      }
      last = h2_stdio::get_length();
   }

   const char* format_duration(long long ms)
   {
      static char st[64];
      if (ms < 1000 * 60)
         sprintf(st, "%.2g seconds", ms / (double)1000.0);
      else if (ms < 1000 * 60 * 60)
         sprintf(st, "%.2g minutes", ms / (double)6000.0);
      else
         sprintf(st, "%.2g hours", ms / (double)36000.0);

      return st;
   }

   void print_status(int n, const char* style, const char* name)
   {
      if (0 < n) {
         h2_color::printf("dark gray", ", ");
         h2_color::printf(style, "%d", n);
         h2_color::printf("", " %s", name);
      }
   }

   void on_task_endup(int status_stats[8], int round) override
   {
      h2_report::on_task_endup(status_stats, round);
      if (O.listing) {
         h2_color::printf("bold,green", "Listing <%d suites, %d cases, %d todo>\n", ss, total_cases, status_stats[h2_case::TODOED]);
      } else {
         print_percentage(false, false);
         if (0 < status_stats[h2_case::FAILED])
            h2_color::printf("bold,red", "Failure");
         else
            h2_color::printf("bold,green", "Success");
         h2_color::printf("dark gray", " (");
         h2_color::printf("green", "%d", status_stats[h2_case::PASSED]);
         h2_color::printf("", " passed");
         print_status(status_stats[h2_case::FAILED], "red", "failed");
         print_status(status_stats[h2_case::TODOED], "yellow", "todo");
         print_status(status_stats[h2_case::FILTED], "blue", "filtered");
         h2_color::printf("dark gray", ") ");
         if (1 < round) h2_color::printf("", "%d rounds ", round);
         h2_color::printf("", "%s \n", format_duration(tt));
      }
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report::on_suite_start(s);
      cs = 0;
      if (O.listing) {
         h2_color::printf("", "SUITE%d. ", ++ss);
         h2_color::printf("bold,blue", "%s", s->name);
         h2_color::printf("", " %s:%d\n", s->file, s->line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report::on_case_start(s, c);
      if (O.listing) {
         h2_color::printf("", "   %s%d. ", c->status == h2_case::TODOED ? "TODO" : "CASE", ++cs);
         h2_color::printf("cyan", "%s", c->name);
         h2_color::printf("", " %s:%d\n", basename((char*)c->file), c->line);
      }
   }

   void print_suite_case(const char* s, const char* c)
   {
      h2_color::printf("", "%s", s);
      h2_color::printf("dark gray", " | ");
      h2_color::printf("", "%s", c);
   }

   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report::on_case_endup(s, c);
      if (O.listing) return;
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         print_percentage();
         print_suite_case(s->name, c->name);
         h2_color::printf("yellow", " Todo");
         h2_color::printf("", " at %s:%d\n", basename((char*)c->file), c->line);
         break;
      case h2_case::FILTED: break;
      case h2_case::PASSED:
         if (O.verbose) {
            print_percentage();
            print_suite_case(s->name, c->name);
            h2_color::printf("green", " Passed");
            h2_color::printf("", " %lld ms\n", tc);
         } else if (!O.debug)
            print_percentage(true, true);
         break;
      case h2_case::FAILED:
         print_percentage();
         print_suite_case(s->name, c->name);
         h2_color::printf("bold,red", " Failed");
         h2_color::printf("", " at %s:%d\n", basename((char*)c->file), c->line);
         if (c->fails) c->fails->foreach ([](h2_fail* fail, int subling_index, int child_index) { fail->print(subling_index, child_index); });
         h2_color::printf("", "\n");
         break;
      }
   }
};

struct h2_report_junit : h2_report {
   FILE* f;
   void on_task_start(int cases) override
   {
      h2_report::on_task_start(cases);
      f = fopen(O.junit, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   };
   void on_suite_start(h2_suite* s) override
   {
      h2_report::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->status_stats[h2_case::FAILED], s->name, s->status_stats[h2_case::TODOED] + s->status_stats[h2_case::FILTED], s->cases.count(), 0, "");
   }

   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, CSS[c->status], tc / 1000.0);

      if (c->status == h2_case::FAILED) {
         fprintf(f, "<failure message=\"%s:%d:", c->file, c->line);
         if (c->fails) c->fails->foreach ([=](h2_fail* fail, int subling_index, int child_index) {fprintf(f, "{newline}"); fail->print(f); });
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "<system-out></system-out><system-err></system-err>\n");
      fprintf(f, "</testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_task_endup(int status_stats[8], int round) override
   {
      h2_report::on_task_endup(status_stats, round);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_report_tap : h2_report {
   /* */
};

h2_inline void h2_reports::initialize()
{
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   reports.push_back(&console_report);
   if (strlen(O.junit) && !O.listing) reports.push_back(&junit_report);
}

inline void h2_reports::on_task_start(int cases)
{
   for (auto report : reports) report->on_task_start(cases);
}
inline void h2_reports::on_task_endup(int status_stats[8], int round)
{
   for (auto report : reports) report->on_task_endup(status_stats, round);
}
inline void h2_reports::on_suite_start(h2_suite* s)
{
   for (auto report : reports) report->on_suite_start(s);
}
inline void h2_reports::on_suite_endup(h2_suite* s)
{
   for (auto report : reports) report->on_suite_endup(s);
}
inline void h2_reports::on_case_start(h2_suite* s, h2_case* c)
{
   for (auto report : reports) report->on_case_start(s, c);
}
inline void h2_reports::on_case_endup(h2_suite* s, h2_case* c)
{
   for (auto report : reports) report->on_case_endup(s, c);
}
