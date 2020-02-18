
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
   const char *path, *include_patterns[9], *exclude_patterns[9];

   h2_option(int argc, const char** argv)
     : listing(0), breakable(0), verbose(false), colorable(true), randomize(false), memory_check(true), dns(true), junit{0}, include_patterns{0}, exclude_patterns{0} {
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
               for (int j = i + 1; j < argc && argv[j][0] != '-'; ++j, ++i) insert(include_patterns, argv[j]);
               break;
            case 'x':
               for (int j = i + 1; j < argc && argv[j][0] != '-'; ++j, ++i) insert(exclude_patterns, argv[j]);
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

   void insert(const char* patterns[9], const char* pattern) {
      for (int i = 0; i < 9; ++i)
         if (!patterns[i]) {
            patterns[i] = pattern;
            break;
         }
   }

   int filter(const char* patterns[9], const char* subject) {
      for (int i = 0; i < 9 && patterns[i]; ++i)
         if (strchr(patterns[i], '?') || strchr(patterns[i], '*')) {
            if (h2_wildcard_match(patterns[i], subject)) return 1;
         } else {
            if (strstr(subject, patterns[i])) return 1;
         }
      return patterns[0] ? 0 : -1;
   }

   bool filter(const char* suitename, const char* casename, const char* filename) {
      if (0 == filter(include_patterns, suitename) && 0 == filter(include_patterns, casename) && 0 == filter(include_patterns, filename))
         return true;
      if (1 == filter(exclude_patterns, suitename) || 1 == filter(exclude_patterns, casename) || 1 == filter(exclude_patterns, filename))
         return true;
      return false;
   }
};

static inline h2_option& O() { return h2_option::I(0, nullptr); }

static inline const char* S(const char* style_str) {
   if (!O().colorable) return "";

   static char shift_buffer[8][128];
   static long shift_index = 0;

   return h2_style(style_str, shift_buffer[++shift_index % 8]);
}
