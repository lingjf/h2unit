
static constexpr unsigned linux = 0x0101;
static constexpr unsigned macos = 0x0102;
static constexpr unsigned winos = 0x0200;

struct h2_option {
   h2_singleton(h2_option);

#if defined linux || defined __linux || defined __linux__
   static constexpr unsigned os = linux;
#elif defined __APPLE__
   static constexpr unsigned os = macos;
#elif defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
   static constexpr unsigned os = winos;
#endif

   unsigned terminal_width;

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

   h2_option();
   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int lino) const;
};

static const h2_option& O = h2_option::I();  // for pretty
