
#ifndef ___H2_LOG__H___
#define ___H2_LOG__H___

class h2_log
{
 public:
   h2_log()
   {
   }
   virtual ~h2_log()
   {
   }

   virtual void on_task_start() = 0;
   virtual void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration) = 0;
   virtual void on_case_start(h2_case* c) = 0;
   virtual void on_case_endup(h2_case* c) = 0;
};

class h2_logs : public h2_log
{
 public:
   std::vector<h2_log*> logs;

   h2_logs()
   {
   }

   virtual ~h2_logs()
   {
      auto it = logs.begin();
      while (it != logs.end()) {
         it = logs.erase(it);
      }
   }

   void add(h2_log* log)
   {
      logs.push_back(log);
   }

   void on_task_start()
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_task_start();
      }
   }

   void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration)
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_task_endup(failed, passed, todo, filtered, cases, duration);
      }
   }

   void on_case_start(h2_case* c)
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_case_start(c);
      }
   }

   void on_case_endup(h2_case* c)
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_case_endup(c);
      }
   }
};

class h2_log_console : public h2_log
{
 public:
   h2_log_console() {}
   virtual ~h2_log_console() {}
   void on_task_start() {}
   void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration)
   {
      if (failed > 0) {
         printf("%s", h2_cfg::style("bold,red"));
         printf("\nFailed <%d failed, %d passed, %d todo, %d filtered, %ld ms>\n", failed, passed, todo, filtered, duration);
      }
      else {
         printf("%s", h2_cfg::style("bold,green"));
         printf("\nPassed <%d passed, %d todo, %d filtered, %d cases, %ld ms>\n", passed, todo, filtered, cases, duration);
      }
      printf("%s", h2_cfg::style("reset"));
   }
   void on_case_start(h2_case* c) {}
   void on_case_endup(h2_case* c)
   {
      switch (c->_status_) {
      case h2_case::_TODOED_:
         if (!strlen(c->_suite_->name)) {
            printf("H2UNIT_CASE(%s): TODO at %s:%d\n", c->_casename_, c->_casefile_, c->_caseline_);
         }
         else {
            printf("H2CASE(%s, %s): TODO at %s:%d\n", c->_suite_->name, c->_casename_, c->_casefile_, c->_caseline_);
         }
         break;
      case h2_case::_FILTED_:
         break;
      case h2_case::_PASSED_:
         if (h2_cfg::I().verbose) {
            printf("%s", h2_cfg::style("blue"));
            if (!strlen(c->_suite_->name)) {
               printf("H2UNIT_CASE(%s): Passed - %ld ms\n", c->_casename_, c->_endup_ - c->_start_);
            }
            else {
               printf("H2CASE(%s, %s): Passed - %ld ms\n", c->_suite_->name, c->_casename_, c->_endup_ - c->_start_);
            }
            printf("%s", h2_cfg::style("reset"));
         }
         break;
      case h2_case::_FAILED_:
         printf("%s", h2_cfg::style("bold,purple"));
         if (!strlen(c->_suite_->name)) {
            printf("H2UNIT_CASE(%s): Failed at %s:%d\n", c->_casename_, c->_casefile_, c->_caseline_);
         }
         else {
            printf("H2CASE(%s, %s): Failed at %s:%d\n", c->_suite_->name, c->_casename_, c->_casefile_, c->_caseline_);
         }
         printf("%s", h2_cfg::style("reset"));

         for (auto it = c->_fail_list_.begin(); it != c->_fail_list_.end(); it++) {
            H2_FAIL_FOREACH(f, *it)
            {
               f->print();
            }
         }
         ::printf("\n");
         break;
      }

      printf("%s", h2_cfg::style("reset"));
   }
};

class h2_log_xml : public h2_log
{
 private:
   FILE* fp;

 public:
   h2_log_xml() {}
   virtual ~h2_log_xml() {}

   bool set_file(const char* filepath)
   {
      fp = fopen(filepath, "w");
      return fp != NULL;
   }

   void on_task_start() {}
   void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration)
   {
      fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(fp, "<testsuites>\n");

      for (auto i = h2_suite::G()->begin(); i != h2_suite::G()->end(); i++) {
         auto p = *i;
         fprintf(fp, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
                 p->status_count[h2_case::_FAILED_],
                 p->name,
                 p->status_count[h2_case::_TODOED_] + p->status_count[h2_case::_FILTED_],
                 (int)p->case_list.size(),
                 0, "");

         for (auto j = p->case_list.begin(); j != p->case_list.end(); j++) {
            auto c = *j;
            fprintf(fp, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
                    c->_suite_->name, c->_casename_, c->status(), (c->_endup_ - c->_start_) / 1000.0);

            if (c->_status_ == h2_case::_FAILED_) {
               fprintf(fp, "      <failure message=\"%s:%d:", c->_casefile_, c->_caseline_);

               for (auto k = c->_fail_list_.begin(); k != c->_fail_list_.end(); k++) {
                  H2_FAIL_FOREACH(f, *k)
                  {
                     fprintf(fp, "{newline}");
                     f->print(fp);
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

   void on_case_start(h2_case* c) {}
   void on_case_endup(h2_case* c) {}
};

#endif
