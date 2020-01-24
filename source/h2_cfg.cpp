struct h2_configure {
   char* path;
   const char* platform;
   bool listing;
   bool verbose;
   bool colorable;
   bool randomize;
   bool memory_check;
   long breakable;
   char junit[256];

   std::vector<const char*> include_patterns;
   std::vector<const char*> exclude_patterns;

   h2_configure()
     : path(nullptr),
#if defined(__linux__)
       platform("Linux"),
#elif defined(__APPLE__)
       platform("MAC"),
#else
#endif
       listing(false),
       verbose(false),
       colorable(true),
       randomize(false),
       memory_check(true),
       breakable(0),
       junit{0} {
   }

   /* clang-format off */
   static h2_configure& I() { static h2_configure I; return I; }
   /* clang-format on */

   void usage() {
      printf("Usage: \n"
             "-v                  Make the operation more talkative\n"
             "-l                  List out all suites and cases\n"
             "-b [n]              Breaking test once n (default is 1) failures occurred\n"
             "-c                  Output in black-white color mode\n"
             "-r                  Run cases in random order\n"
             "-m                  Run cases without memory check\n"
             "-j [path]           Generate junit report, default is .xml \n"
             "-i {patterns}       Run cases which case name, suite name or file name matches\n"
             "-x {patterns}       Run cases which case name, suite name and file name not matches\n");
   }

   void opt(int argc, char** argv) {
      path = argv[0];

      for (int i = 1; i < argc; ++i) {
         if (argv[i][0] == '-') {
            for (char* p = argv[i] + 1; *p; p++) {
               switch (*p) {
               case 'v': verbose = true; break;
               case 'l': listing = true; break;
               case 'b':
                  breakable = 1;
                  if (i + 1 < argc && argv[i + 1][0] != '-') {
                     breakable = atoi(argv[i + 1]);
                     ++i;
                  }
                  break;
               case 'c': colorable = !colorable; break;
               case 'r': randomize = true; break;
               case 'm': memory_check = !memory_check; break;
               case 'j':
                  sprintf(junit, "%s.xml", path);
                  if (i + 1 < argc && argv[i + 1][0] != '-') {
                     strcpy(junit, argv[i + 1]);
                     ++i;
                  }
                  break;
               case 'i':
                  for (int j = i + 1; j < argc; ++j) {
                     if (argv[j][0] == '-') break;
                     include_patterns.push_back(argv[j]);
                     ++i;
                  }
                  break;
               case 'x':
                  for (int j = i + 1; j < argc; ++j) {
                     if (argv[j][0] == '-') break;
                     exclude_patterns.push_back(argv[j]);
                     ++i;
                  }
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
   }

   int filter(std::vector<const char*>& patterns, const char* subject) {
      if (patterns.empty()) return -1;

      for (auto it = patterns.begin(); it != patterns.end(); it++)
         if (strchr(*it, '?') || strchr(*it, '*')) {
            if (h2_wildcard_match(*it, subject)) return 1;
         } else {
            if (strstr(subject, *it)) return 1;
         }
      return 0;
   }

   bool filter(const char* suitename, const char* casename, const char* filename) {
      if (0 == filter(include_patterns, suitename) && 0 == filter(include_patterns, casename) && 0 == filter(include_patterns, filename))
         return true;
      if (1 == filter(exclude_patterns, suitename) || 1 == filter(exclude_patterns, casename) || 1 == filter(exclude_patterns, filename))
         return true;
      return false;
   }

   const char* style(const char* style_str) {
      if (!I().colorable) { return ""; }

      static char shift_buffer[8][128];
      static long shift_index = 0;

      shift_index = (shift_index + 1) % 8;
      return h2_style(style_str, shift_buffer[shift_index]);
   }

   int get_term_columns() {
      struct winsize w;
      if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) {
         printf("ioctl failed %s\n", strerror(errno));
         return 80;
      }
      return w.ws_col;
   }
};

static inline h2_configure& cfg() { return h2_configure::I(); }
