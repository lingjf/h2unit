struct h2_option {
   h2_singleton(h2_option);

#if defined __linux
   static constexpr char os = 'L';
#elif defined __APPLE__
   static constexpr char os = 'm';
#elif defined _WIN32 || defined __CYGWIN__ // +MinGW
   static constexpr char os = 'W';
#endif

   char args[256];
   const char* path;
   const char* debug = nullptr;
   bool colorful = true;
   bool progressing = true;
   bool only_previous_failed = false;
   bool shuffle_cases = false;
   bool memory_check = true;
   bool exception_as_fail = false;
   int list_cases = 0;
   int break_after_fails = 0;
   int run_rounds = 1;
   int fold_json = 9; // 0 unfold, 1 fold simple, 2 fold same, 3 fold peer-miss
   int copy_paste_json = 0; // 0 no quote, 1 quote by ', 2 quote by ", 3 quote by \"
   int verbose = 2;
   char junit_path[256]{'\0'};
   char tap_path[256]{'\0'};
   std::vector<const char*> includes, excludes;

   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int line) const;
};

static const h2_option& O = h2_option::I();  // for pretty
