struct h2_runner {
   h2_singleton(h2_runner);

   h2_suite* current_suite = nullptr;
   h2_case* current_case = nullptr;
   int rounds = 0;
   int lasts = 0;
   h2_list suites;
   h2_list stubs;
   h2_list mocks;
   h2_stats stats;

   void (*global_setups[1024])(){nullptr}, (*global_cleanups[1024])(){nullptr};
   void (*global_suite_setups[1024])(){nullptr}, (*global_suite_cleanups[1024])(){nullptr};
   void (*global_case_setups[1024])(){nullptr}, (*global_case_cleanups[1024])(){nullptr};

   void enumerate();
   void filter();
   void shuffle();
   int main(int argc, const char** argv);

   static void stub(void* srcfp, void* dstfp, const char* srcfn, const char* filine);
   static void unstub(void* srcfp);
   static void mock(void* mocker);
   static void failing(h2_fail* fail);
   static void asserts();
};

#define __H2GlobalCallback(Scope, Q)                               \
   namespace {                                                     \
      static struct Q {                                            \
         Q() { h2_array_append(h2::h2_runner::I().Scope, Scope); } \
         static void Scope();                                      \
      } H2PP_UNIQUE();                                             \
   }                                                               \
   void Q::Scope()

#define H2GlobalSetup(...) __H2GlobalCallback(global_setups, H2PP_UNIQUE())
#define H2GlobalCleanup(...) __H2GlobalCallback(global_cleanups, H2PP_UNIQUE())

#define H2GlobalSuiteSetup(...) __H2GlobalCallback(global_suite_setups, H2PP_UNIQUE())
#define H2GlobalSuiteCleanup(...) __H2GlobalCallback(global_suite_cleanups, H2PP_UNIQUE())

#define H2GlobalCaseSetup(...) __H2GlobalCallback(global_case_setups, H2PP_UNIQUE())
#define H2GlobalCaseCleanup(...) __H2GlobalCallback(global_case_cleanups, H2PP_UNIQUE())
