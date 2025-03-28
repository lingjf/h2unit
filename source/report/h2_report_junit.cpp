struct h2_report_junit : h2_report_interface {
   FILE* f;
   char* xml_escape(const char* str, char* escaped_str) {
      char* p = escaped_str;
      for (const char* s = str; *s != '\0'; s++) {
         switch (*s) {
            case '&': strcpy(p, "&amp;"); p += 5; break;
            case '<': strcpy(p, "&lt;"); p += 4; break;
            case '>': strcpy(p, "&gt;"); p += 4; break;
            case '"': strcpy(p, "&quot;"); p += 6; break;
            case '\'': strcpy(p, "&apos;"); p += 6; break;
            default: *p++ = *s; break;
         }
      }
      *p = '\0';
      return escaped_str;
   }
   void on_runner_start(h2_runner* r) override
   {
      f = fopen(O.junit_path, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      if (!f) return;
      char temp1[1024];
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats.failed, xml_escape(s->name, temp1), s->stats.todo + s->stats.filtered, s->cases.count(), 0, "");
   }
   void on_case_start(h2_suite* s, h2_case* c) override {}
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      if (!f) return;
      char temp1[1024], temp2[1024];
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", xml_escape(s->name, temp1), xml_escape(c->name, temp2), c->todo ? "TODO" : (c->filtered ? "Filtered" : (c->ignored ? "Ignored" : (c->failed ? "Failed" : (c->warning ? "Warning" : "Passed")))), c->stats.timecost / 1000.0);
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
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_runner_endup(h2_runner* r) override
   {
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};
