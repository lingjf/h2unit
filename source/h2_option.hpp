
struct h2_option {
   h2_singleton(h2_option);

#if defined __linux__
   static constexpr const char* os = "linux";
#elif defined __APPLE__
   static constexpr const char* os = "macos";
#elif defined _WIN32
   static constexpr const char* os = "windows";
#endif

   char args[256];
   const char* path;
   const char* debug = nullptr;
   bool verbose = false;
   bool colorfull = true;
   bool shuffle = false;
   bool memory_check = true;
   bool listing = false;
   int breakable = 0;
   int rounds = 1;
   char junit[256]{'\0'};
   char tap[256]{'\0'};
   std::vector<const char*> includes, excludes;

   void parse(int argc, const char** argv);

   bool filter(const char* suitename, const char* casename, const char* filename) const;
};

static const h2_option& O = h2_option::I();  // for pretty
