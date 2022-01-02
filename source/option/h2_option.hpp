static constexpr int OsLinux = 0x11, OsMacOS = 0x12, OsWindows = 0x20;
static constexpr int VerboseQuiet = 0, VerboseCompactFailed = 1, VerboseCompactWarning = 2, VerboseCompactPassed = 3, VerboseNormal = 4, VerboseDetail = 5;
static constexpr int ShuffleCode = 0x0, ShuffleRandom = 0x10, ShuffleName = 0x100, ShuffleFile = 0x1000, ShuffleReverse = 0x10000;
static constexpr int ListNone = 0x0, ListSuite = 0x10, ListCase = 0x100, ListTodo = 0x1000, ListTags = 0x10000;
static constexpr int FoldUnFold = 0, FoldShort = 1, FoldSame = 2, FoldSingle = 3, FoldMax = 5;

struct h2_option {
   h2_singleton(h2_option);

#if defined __linux
   static constexpr int os = OsLinux;
#elif defined __APPLE__
   static constexpr int os = OsMacOS;
#elif defined _WIN32 || defined __CYGWIN__  // +MinGW
   static constexpr int os = OsWindows;
#endif

   char args[256];
   const char* path;
   bool colorful = true;
   bool progressing = true;
   bool only_last_failed = false;
   bool memory_check = true;
   bool continue_assert = false;
   bool debugger_trap = false;
   bool exit_with_fails = false;
   bool as_waring_exception = false;
   bool as_waring_uncaught = false;
   bool as_waring_memory_leak = false;
   bool as_waring_memory_violate = false;
   bool as_waring_memory_double_free = false;
   bool as_waring_memory_asymmetric_free = false;
   int break_after_fails = 0;
   int run_rounds = 1;
   int fold_json = FoldMax;
   int shuffles = ShuffleCode;
   int lists = ListNone;
   int verbose = VerboseNormal;
   const char* json_source_quote = "";
   char junit_path[256]{'\0'};
   const char *includes[128]{nullptr}, *excludes[128]{nullptr};
   const char *file_includes[128]{nullptr}, *file_excludes[128]{nullptr};
   const char *suite_includes[128]{nullptr}, *suite_excludes[128]{nullptr};
   const char *case_includes[128]{nullptr}, *case_excludes[128]{nullptr};
   const char *tags_includes[128]{nullptr}, *tags_excludes[128]{nullptr};

   void parse(int argc, const char** argv);
};

static const h2_option& O = h2_option::I();  // for pretty
