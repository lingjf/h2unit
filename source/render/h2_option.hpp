
static constexpr int VerboseQuiet = 0, VerboseCompactFailed = 1, VerboseCompactPassed = 2, VerboseNormal = 3, VerboseDetail = 4;
static constexpr int ShuffleCode = 0x0, ShuffleRandom = 0x10, ShuffleName = 0x100, ShuffleFile = 0x1000, ShuffleReverse = 0x10000;
static constexpr int ListNone = 0x0, ListSuite = 0x10, ListCase = 0x100, ListTodo = 0x1000, ListTag = 0x10000;
static constexpr int FoldUnFold = 0, FoldShort = 1, FoldSame = 2, FoldSingle = 3, FoldMax = 5;

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
   int fold_json = FoldMax;
   int shuffles = ShuffleCode;
   int lists = ListNone;
   int verbose = VerboseNormal;
   const char* json_source_quote = "";
   char junit_path[256]{'\0'};
   const char *includes[128]{nullptr}, *excludes[128]{nullptr};

   void parse(int argc, const char** argv);
};

static const h2_option& O = h2_option::I();  // for pretty
