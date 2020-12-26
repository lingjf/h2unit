
struct h2_task {
   h2_singleton(h2_task);

   int stats[h2_case::n_st]{0};
   int asserts = 0;
   int rounds = 0;
   int last = 0;
   h2_list suites;
   h2_suite* current_suite = nullptr;
   h2_case* current_case = nullptr;
   h2_stubs stubs;
   h2_mocks mocks;
   std::vector<void (*)()> global_setups, global_teardowns;
   std::vector<void (*)()> global_suite_setups, global_suite_teardowns;
   std::vector<void (*)()> global_case_setups, global_case_teardowns;

   void shuffle();
   void shadow();
   void enumerate();
   void execute();
};

static inline void h2_stub_g(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line)
{
   if (!origin_fp || !substitute_fp) return;
   if (h2_task::I().current_case)
      h2_task::I().current_case->stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
   else
      h2_task::I().stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
}

static inline void h2_unstub_g(void* origin_fp)
{
   if (!origin_fp) return;
   if (h2_task::I().current_case)
      h2_task::I().current_case->stubs.clear(origin_fp);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->stubs.clear(origin_fp);
   else
      h2_task::I().stubs.clear(origin_fp);
}

static inline void h2_mock_g(void* mock)
{
   if (h2_task::I().current_case)
      h2_task::I().current_case->mocks.add(mock);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->mocks.add(mock);
   else
      h2_task::I().mocks.add(mock);
}

static inline void h2_assert_g()
{
   if (h2_task::I().current_case) h2_task::I().current_case->asserts += 1;
   if (h2_task::I().current_suite) h2_task::I().current_suite->asserts += 1;
   h2_task::I().asserts += 1;
}

static inline void h2_fail_g(h2_fail* fail, bool defer)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail(fail, defer);
}
