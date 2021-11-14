
static constexpr int VerboseQuiet = 0;
static constexpr int VerboseCompactFailed = 1;
static constexpr int VerboseCompactPassed = 2;
static constexpr int VerboseNormal = 3;
static constexpr int VerboseDetail = 4;

static constexpr int ShuffleCode = 0x0;
static constexpr int ShuffleRandom = 0x10;
static constexpr int ShuffleName = 0x100;
static constexpr int ShuffleFile = 0x1000;
static constexpr int ShuffleReverse = 0x10000;

static constexpr int ListNone = 0x0;
static constexpr int ListSuite = 0x10;
static constexpr int ListCase = 0x100;
static constexpr int ListTodo = 0x1000;
static constexpr int ListTag = 0x10000;

static constexpr int FoldUnFold = 0;
static constexpr int FoldShort = 1;
static constexpr int FoldSame = 2;
static constexpr int FoldSingle = 3;
static constexpr int FoldMax = 5;

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
   char tap_path[256]{'\0'};
   std::vector<const char*> includes, excludes;

   void parse(int argc, const char** argv);
};

static const h2_option& O = h2_option::I();  // for pretty
