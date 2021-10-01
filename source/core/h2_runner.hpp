struct h2_runner {
   h2_singleton(h2_runner);

   h2_stats stats;
   int rounds = 0;
   int last = 0;
   h2_list suites;
   h2_suite* current_suite = nullptr;
   h2_case* current_case = nullptr;
   h2_stubs stubs;
   h2_mocks mocks;
   std::vector<void (*)()> global_setups, global_cleanups;
   std::vector<void (*)()> global_suite_setups, global_suite_cleanups;
   std::vector<void (*)()> global_case_setups, global_case_cleanups;

   void shuffle();
   void shadow();
   void enumerate();
   int main(int argc, const char** argv);
};

static inline void h2_stub_g(void* srcfp, void* dstfp, const h2_sz& sz)
{
   if (!srcfp || !dstfp) return;
   if (h2_runner::I().current_case)
      h2_runner::I().current_case->stubs.add(srcfp, dstfp, sz);
   else if (h2_runner::I().current_suite)
      h2_runner::I().current_suite->stubs.add(srcfp, dstfp, sz);
   else
      h2_runner::I().stubs.add(srcfp, dstfp, sz);
}

static inline void h2_unstub_g(void* srcfp)
{
   if (!srcfp) return;
   if (h2_runner::I().current_case)
      h2_runner::I().current_case->stubs.clear(srcfp);
   else if (h2_runner::I().current_suite)
      h2_runner::I().current_suite->stubs.clear(srcfp);
   else
      h2_runner::I().stubs.clear(srcfp);
}

static inline void h2_mock_g(void* mock)
{
   if (h2_runner::I().current_case)
      h2_runner::I().current_case->mocks.add(mock);
   else if (h2_runner::I().current_suite)
      h2_runner::I().current_suite->mocks.add(mock);
   else
      h2_runner::I().mocks.add(mock);
}

static inline void h2_assert_g()
{
   if (h2_runner::I().current_case) h2_runner::I().current_case->asserts += 1;
   if (h2_runner::I().current_suite) h2_runner::I().current_suite->stats.asserts += 1;
   h2_runner::I().stats.asserts += 1;
}

static inline void h2_fail_g(h2_fail* fail)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_runner::I().current_case) h2_runner::I().current_case->do_fail(fail, O.verbose >= 5, true);
}
