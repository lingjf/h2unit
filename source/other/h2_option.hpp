
static constexpr unsigned Linux = 0x0101;
static constexpr unsigned macOS = 0x0102;
static constexpr unsigned windows = 0x0200;

struct h2_option {
   h2_singleton(h2_option);

#if defined linux || defined __linux || defined __linux__
   static constexpr unsigned os = Linux;
#elif defined __APPLE__
   static constexpr unsigned os = macOS;
#elif defined _WIN32
   static constexpr unsigned os = windows;
#endif

   char args[256];
   const char* path;
   const char* debug = nullptr;
   bool verbose = false;
   bool compact = false;
   bool colorful = true;
   bool execute_progress = true;
   bool fold_json = true;
   bool copy_paste_json = false;
   bool only_execute_fails = false;
   bool shuffle_order = false;
   bool memory_check = true;
   bool exception_fails = false;
   char list_cases = '\0';
   int break_after_fails = 0;
   int rounds = 1;
   char junit[256]{'\0'};
   char tap[256]{'\0'};
   std::vector<const char*> includes, excludes;
   unsigned terminal_width;

   h2_option();
   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int line) const;
};

static const h2_option& O = h2_option::I();  // for pretty
