
static constexpr int verbose_quiet = 0;
static constexpr int verbose_compact_failed = 1;
static constexpr int verbose_compact_passed = 2;
static constexpr int verbose_normal = 3;
static constexpr int verbose_detail = 4;

static constexpr int ShuffleCode = 0x0;
static constexpr int ShuffleRandom = 0x10;
static constexpr int ShuffleName = 0x100;
static constexpr int ShuffleFile = 0x1000;
static constexpr int ShuffleReverse = 0x10000;

struct h2_option {
   h2_singleton(h2_option);

#if defined __linux
   static constexpr char os = 'L';
#elif defined __APPLE__
   static constexpr char os = 'm';
#elif defined _WIN32 || defined __CYGWIN__  // +MinGW
   static constexpr char os = 'W';
#endif

   char args[256];
   const char* path;
   bool colorful = true;
   bool progressing = true;
   bool only_last_failed = false;
   bool memory_check = true;
   bool continue_assert = false;
   bool exception_as_fail = false;
   bool debugger_trap = false;
   bool quit_exit_code = false;
   bool tags_filter = false;
   int break_after_fails = 0;
   int run_rounds = 1;
   int fold_json = 5;  // 0 unfold, 1 fold short, 2 fold same, 3 fold single
   int shuffle_cases = ShuffleCode;
   int verbose = verbose_normal;
   const char* json_source_quote = "";
   char junit_path[256]{'\0'};
   char tap_path[256]{'\0'};
   std::vector<const char*> list_cases;
   std::vector<const char*> includes, excludes;

   void parse(int argc, const char** argv);
};

static const h2_option& O = h2_option::I();  // for pretty
