
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
   bool verbose, colorfull, shuffle, memory_check, listing;
   int breakable, rounds;
   char junit[256], args[256];
   std::vector<const char*> includes, excludes;

   h2_option() : debug(nullptr), verbose(false), colorfull(true), shuffle(false), memory_check(true), listing(false), breakable(0), rounds(1), junit{0} {}

   void parse(int argc, const char** argv);

   int isLinux() const { return 1 == os; }
   int isMAC() const { return 2 == os; }
   int isWindows() const { return 3 == os; }

   bool filter(const char* suitename, const char* casename, const char* filename) const;
};

static const h2_option& O = h2_option::I();  // for pretty
