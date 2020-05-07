
struct h2_option {
   h2_singleton(h2_option);

#if defined __linux__
   static constexpr const int os = 1;
#elif defined __APPLE__
   static constexpr const int os = 2;
#elif defined _WIN32
   static constexpr const int os = 3;
#endif

   const char *path, *debug;
   int verbose, listing, breakable, randomize;
   bool colorable, memory_check;
   char junit[256], args[256];
   std::vector<const char*> includes, excludes;

   h2_option() : debug(nullptr), verbose(0), listing(0), breakable(0), randomize(0), colorable(true), memory_check(true), junit{0} {}

   void parse(int argc, const char** argv);

   int isLinux() const { return 1 == os; }
   int isMAC() const { return 2 == os; }
   int isWindows() const { return 3 == os; }

   bool filter(const char* suitename, const char* casename, const char* filename) const;
   const char* style(const char* s) const;
};

static const h2_option& O = h2_option::I();  // for pretty
