
struct h2_option {
#if defined __linux__
   static constexpr const int os = 1;
#elif defined __APPLE__
   static constexpr const int os = 2;
#elif defined _WIN32
   static constexpr const int os = 3;
#endif

   int listing, breakable;
   bool verbose, colorable, randomize, memory_check, dns;
   char junit[256];
   const char* path;
   std::vector<const char*> include_patterns, exclude_patterns;

   h2_option(int argc, const char** argv)
     : listing(0), breakable(0), verbose(false), colorable(true), randomize(false), memory_check(true), dns(true), junit{0} {
      path = argv[0];
      for (int i = 1; i < argc; ++i) {
         if (argv[i][0] != '-') continue;
         for (const char* p = argv[i] + 1; *p; p++) {
            switch (*p) {
            case 'v': verbose = true; break;
            case 'l':
               listing = 'A';
               if (i + 1 < argc && argv[i + 1][0] != '-') listing = argv[++i][0];
               break;
            case 'b':
               breakable = 1;
               if (i + 1 < argc && argv[i + 1][0] != '-') breakable = atoi(argv[++i]);
               break;
            case 'c': colorable = !colorable; break;
            case 'r': randomize = !randomize; break;
            case 'm': memory_check = !memory_check; break;
            case 'd': dns = !dns; break;
            case 'j':
               sprintf(junit, "%s.xml", path);
               if (i + 1 < argc && argv[i + 1][0] != '-') strcpy(junit, argv[++i]);
               break;
            case 'i':
               for (int j = i + 1; j < argc && argv[j][0] != '-'; ++j, ++i) include_patterns.push_back(argv[j]);
               break;
            case 'x':
               for (int j = i + 1; j < argc && argv[j][0] != '-'; ++j, ++i) exclude_patterns.push_back(argv[j]);
               break;
            case '-': break;
            case 'h':
            case '?':
               usage();
               exit(0);
            default:
               printf("unknown option: -%c, -h for help\n", *p);
               exit(0);
            }
         }
      }
   }

   int isLinux() const { return 1 == os; }
   int isMAC() const { return 2 == os; }
   int isWindows() const { return 3 == os; }

   /* clang-format off */
   static h2_option& I(int argc, const char** argv) { static h2_option __(argc, argv); return __; }
   /* clang-format on */

   void usage() {
      printf("Usage:\n"
             "-v                  Make the operation more talkative\n"
             "-l [sc]             List out all suites and cases\n"
             "-b [n]              Breaking test once n (default is 1) failures occurred\n"
             "-c                  Output in black-white color mode\n"
             "-r                  Run cases in random order\n"
             "-m                  Run cases without memory check\n"
             "-d                  Hook DNS resolver\n"
             "-j [path]           Generate junit report, default is .xml\n"
             "-i {patterns}       Run cases which case name, suite name or file name matches\n"
             "-x {patterns}       Run cases which case name, suite name and file name not matches\n");
   }

   bool match(std::vector<const char*>& patterns, const char* subject) {
      for (auto pattern : patterns)
         if (strcspn(pattern, "?*+^$\\.[]") < strlen(pattern)) {
            if (h2_regex_match(pattern, subject)) return true;
            if (strcspn(pattern, "+^$\\.[]") == strlen(pattern))
               if (h2_wildcard_match(pattern, subject)) return true;
         } else {
            if (strstr(subject, pattern)) return true;
         }
      return false;
   }

   bool filter(const char* suitename, const char* casename, const char* filename) {
      if (include_patterns.size())
         if (!match(include_patterns, suitename) && !match(include_patterns, casename) && !match(include_patterns, filename))
            return true;
      if (exclude_patterns.size())
         if (match(exclude_patterns, suitename) || match(exclude_patterns, casename) || match(exclude_patterns, filename))
            return true;
      return false;
   }
};

static inline h2_option& O() { return h2_option::I(0, nullptr); }

static inline const char* S(const char* style_str) {
   if (!O().colorable) return "";

   static char shift_buffer[32][128];
   static long shift_index = 0;

   return h2_style(style_str, shift_buffer[++shift_index % 32]);
}
