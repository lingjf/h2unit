
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
   std::vector<void (*)()> global_setups, global_cleanups;
   std::vector<void (*)()> global_suite_setups, global_suite_cleanups;
   std::vector<void (*)()> global_case_setups, global_case_cleanups;

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

#define __H2GlobalCallback(Scope, Q)                           \
   namespace {                                                 \
      static struct Q {                                        \
         Q() { h2::h2_runner::I().Scope##s.push_back(Scope); } \
         static void Scope();                                  \
      } H2PP_UNIQUE();                                         \
   }                                                           \
   void Q::Scope()

#define H2GlobalSetup() __H2GlobalCallback(global_setup, H2PP_UNIQUE())
#define H2GlobalCleanup() __H2GlobalCallback(global_cleanup, H2PP_UNIQUE())

#define H2GlobalSuiteSetup() __H2GlobalCallback(global_suite_setup, H2PP_UNIQUE())
#define H2GlobalSuiteCleanup() __H2GlobalCallback(global_suite_cleanup, H2PP_UNIQUE())

#define H2GlobalCaseSetup() __H2GlobalCallback(global_case_setup, H2PP_UNIQUE())
#define H2GlobalCaseCleanup() __H2GlobalCallback(global_case_cleanup, H2PP_UNIQUE())
