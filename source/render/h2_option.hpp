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
   bool percent_progressing = true;
   bool fold_json = true;
   bool copy_paste_json = false;
   bool only_previous_failed = false;
   bool shuffle_cases = false;
   bool memory_check = true;
   bool exception_as_fail = false;
   bool list_cases = false;
   int break_after_fails = 0;
   int run_rounds = 1;
   int verbose = 1;
   char junit_path[256]{'\0'};
   char tap_path[256]{'\0'};
   std::vector<const char*> includes, excludes;

   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int line) const;
};

static const h2_option& O = h2_option::I();  // for pretty
