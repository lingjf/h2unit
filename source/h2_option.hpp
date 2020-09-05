
struct h2_option {
   h2_singleton(h2_option);

#if defined __linux__
   static constexpr const char* os = "linux";
#elif defined __APPLE__
   static constexpr const char* os = "macos";
#elif defined _WIN32
   static constexpr const char* os = "windows";
#endif

   unsigned terminal_width;

   char args[256];
   const char* path;
   const char* debug = nullptr;
   bool verbose = false;
   bool colorful = true;
   bool execute_progress = true;
   bool seq = false;
   bool fold_json = true;
   bool copy_paste_json = false;
   bool only_execute_fails = false;
   bool shuffle_order = false;
   bool memory_check = true;
   bool list_cases = false;
   int break_after_fails = 0;
   int rounds = 1;
   char junit[256]{'\0'};
   char tap[256]{'\0'};
   std::vector<const char*> includes, excludes;

   h2_option();
   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int lino) const;
};

static const h2_option& O = h2_option::I();  // for pretty
