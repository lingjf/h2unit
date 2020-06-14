
struct h2_task {
   h2_singleton(h2_task);

   int stats[h2_case::statuss]{0};
   int suites = 0, cases = 0;
   int checks = 0;
   int rounds = 0;
   h2_suite* current_suite = nullptr;
   h2_case* current_case = nullptr;
   h2_stubs stubs;
   h2_mocks mocks;
   std::vector<void (*)()> global_setups, global_teardowns;
   std::vector<void (*)()> global_suite_setups, global_suite_teardowns;
   std::vector<void (*)()> global_case_setups, global_case_teardowns;

   int execute();
};

static inline void h2_stub_g(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* substitute_fn, const char* file, int line)
{
   if (h2_task::I().current_case)
      h2_task::I().current_case->stubs.add(origin_fp, substitute_fp, origin_fn, substitute_fn, file, line);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->stubs.add(origin_fp, substitute_fp, origin_fn, substitute_fn, file, line);
   else
      h2_task::I().stubs.add(origin_fp, substitute_fp, origin_fn, substitute_fn, file, line);
}

static inline void h2_mock_g(h2_mock* mock)
{
   if (h2_task::I().current_case)
      h2_task::I().current_case->mocks.add(mock) && h2_task::I().current_case->stubs.add(mock->origin_fp, mock->substitute_fp, mock->origin_fn, "", mock->file, mock->line);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->mocks.add(mock) && h2_task::I().current_suite->stubs.add(mock->origin_fp, mock->substitute_fp, mock->origin_fn, "", mock->file, mock->line);
   else
      h2_task::I().mocks.add(mock) && h2_task::I().stubs.add(mock->origin_fp, mock->substitute_fp, mock->origin_fn, "", mock->file, mock->line);
}

static inline void h2_check_g()
{
   if (h2_task::I().current_case) h2_task::I().current_case->checks += 1;
   if (h2_task::I().current_suite) h2_task::I().current_suite->checks += 1;
   h2_task::I().checks += 1;
}

static inline void h2_fail_g(h2_fail* fail, bool defer)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail(fail, defer);
}
