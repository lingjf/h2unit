
struct h2_task {
   h2_singleton(h2_task);

   h2_reports reports;
   h2_stubs stubs;
   h2_mocks mocks;
   int state, round, status_stats[8];
   h2_suite* current_suite;
   h2_case* current_case;
   std::vector<void (*)()> global_setups, global_teardowns;
   std::vector<void (*)()> global_suite_setups, global_suite_teardowns;
   std::vector<void (*)()> global_case_setups, global_case_teardowns;

   h2_task();
   int execute();
};

static inline void h2_stub_g(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line)
{
   if (20 <= h2_task::I().state) {
      if (h2_task::I().current_case)
         h2_task::I().current_case->stubs.add(befp, tofp, befn, tofn, file, line);
      else if (h2_task::I().current_suite)
         h2_task::I().current_suite->stubs.add(befp, tofp, befn, tofn, file, line);
      else
         h2_task::I().stubs.add(befp, tofp, befn, tofn, file, line);
   }
}

static inline void h2_mock_g(h2_mock* mock)
{
   if (20 <= h2_task::I().state) {
      if (h2_task::I().current_case)
         h2_task::I().current_case->mocks.add(mock) && h2_task::I().current_case->stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      else if (h2_task::I().current_suite)
         h2_task::I().current_suite->mocks.add(mock) && h2_task::I().current_suite->stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      else
         h2_task::I().mocks.add(mock) && h2_task::I().stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
   }
}

static inline void h2_fail_g(h2_fail* fail)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail(fail);
}
