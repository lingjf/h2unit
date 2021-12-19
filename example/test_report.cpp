#include "../h2unit.h"

struct user_report : h2::h2_report_interface {
   FILE* f;
   void on_runner_start(h2::h2_runner* r) override
   {
      f = fopen("user_report.txt", "w");
      if (!f) return;
      fprintf(f, "on_runner_start\n");
   }
   void on_runner_endup(h2::h2_runner*) override
   {
      fprintf(f, "on_runner_endup\n");
      if (!f) fclose(f);
   }
   void on_suite_start(h2::h2_suite* s) override
   {
      fprintf(f, "on_suite_start : %s\n", s->name);
   }
   void on_suite_endup(h2::h2_suite* s) override
   {
      fprintf(f, "on_suite_start : %s\n", s->name);
   }
   void on_case_start(h2::h2_suite* s, h2::h2_case* c) override
   {
      fprintf(f, "on_case_start : %s\n", c->name);
   }
   void on_case_endup(h2::h2_suite* s, h2::h2_case* c) override
   {
      if (c->filtered) {
         fprintf(f, "on_case_endup : [filtered]\n");
         return;
      }

      if (c->ignored) {
         fprintf(f, "on_case_endup : [ignored]\n");
         return;
      }

      if (c->todo) {
         fprintf(f, "on_case_endup : [todo]\n");
         return;
      }

      if (c->failed) {
         fprintf(f, "on_case_endup : [failed]\n");
      } else {
         fprintf(f, "on_case_endup : [passed]\n");
      }
   }
};

H2Report(user_report);
