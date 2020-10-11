
static inline void usage()
{
   ::printf(" \033[33m╭────────────────────────────────────────────────────────────────────────────╮\033[0m\n");
   ::printf(" \033[33m│\033[0m                                \033[32mh2unit \033[31m%-9g                            \033[33m│\033[0m\n", H2UNIT_VERSION);
   ::printf(" \033[33m│\033[0m          Manual: \033[34;4mhttps://github.com/lingjf/h2unit.git\033[0m \033[0;36mREADME.md            \033[33m│\033[0m\n");
   ::printf(" \033[33m╰────────────────────────────────────────────────────────────────────────────╯\033[0m\n");

   ::printf("\
 ┌────────┬───────────┬───────────────────────────────────────────────────────┐\n\
 │ Option │ Parameter │ Description                                           │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -v     │           │ Verbose output                                        │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -q     │           │ Compact output without failure detail                 │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -l     │           │ List out suites and cases                             │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -s     │           │ Shuffle cases and execute in random order             │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -b[n]  │    [n]    │ Breaking test once n (default 1) failures occurred    │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -o     │           │ Only execute last failed cases                        │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -p     │           │ Hide execute progressing                              │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -r[n]  │    [n]    │ Repeat run n rounds (default 1) when no failure       │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -c     │           │ Output in black-white color style                     │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -f     │           │ Fold simple JSON object or array                      │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -y     │           │ Copy-paste JSON C/C++ source code                     │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -m     │           │ Run cases without memory check                        │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -x     │           │ Throw exception is considered as failure              │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -d     │           │ Debug with gdb once failure occurred                  │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -j     │  [path]   │ Generate junit report, default path is junit.xml      │\n\
 ├────────┼───────────┼────────────────┬──────────────────────────────────────┤\n\
 │ -i     │ patterns  │ include filter │ case, suite or file name             │\n\
 ├────────┤ separated ├────────────────┤ case-insensitive matches patterns    │\n\
 │ -e     │ by space  │ exclude filter │ default include all, exclude none    │\n\
 └────────┴───────────┴────────────────┴──────────────────────────────────────┘\n\
\n");
}

struct getopt {
   int argc;
   const char** args;  // argv + 1
   int i = -1;
   const char* j = nullptr;

   getopt(int argc_, const char** args_) : argc(argc_), args(args_) {}

   const char* extract_next()
   {
      ++i;
      return i < argc ? args[i] : nullptr;
   }

   const char* extract_string()
   {
      if (i + 1 < argc && args[i + 1] && args[i + 1][0] != '-') return args[++i];
      return nullptr;
   }

   const char next_option()
   {
      do {
         for (; j && *++j;) return *j;
         for (; (j = extract_next()) && j[0] != '-';) {}
      } while (j);
      return '\0';
   }

   void extract_number(int& value)
   {
      if (j) {  // j always not null
         int l = strspn(j + 1, "0123456789");
         if (l) {
            value = atoi(j + 1);
            j += l;
            return;
         }
      }

      if (i + 1 < argc) {
         int l = strlen(args[i + 1]);
         if (l && strspn(args[i + 1], "0123456789") == l)
            value = atoi(args[++i]);
      }
   }

   void arguments(char* s)
   {
      for (int k = 0; k < argc; ++k)
         s += sprintf(s, " %s", args[k]);
   }
};

h2_inline h2_option::h2_option()
{
   terminal_width = h2_termimal_width();
#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
   memory_check = false;
#endif
}

h2_inline void h2_option::parse(int argc, const char** argv)
{
   path = argv[0];
   getopt get(argc - 1, argv + 1);
   get.arguments(args);

   for (const char* t;;) {
      switch (get.next_option()) {
      case '\0': return;
      case 'v': verbose = true; break;
      case 'q': compact = !compact; break;
      case 'c': colorful = !colorful; break;
      case 's': shuffle_order = !shuffle_order; break;
      case 'o': only_execute_fails = true; break;
      case 'f': fold_json = !fold_json; break;
      case 'p': execute_progress = !execute_progress; break;
      case 'y': copy_paste_json = true; break;
      case 'm': memory_check = !memory_check; break;
      case 'x': exception_fails = !exception_fails; break;
      case 'l': list_cases = true; break;
      case 'b':
         break_after_fails = 1;
         get.extract_number(break_after_fails);
         break;
      case 'r':
         rounds = 1;
         get.extract_number(rounds);
         break;
      case 'd': debug = "gdb new"; break;
      case 'D': debug = "gdb attach"; break;
      case 'j':
         sprintf(junit, "%s.xml", path);
         if ((t = get.extract_string())) strcpy(junit, t);
         break;
      case 'i':
         while ((t = get.extract_string())) includes.push_back(t);
         break;
      case 'e':
         while ((t = get.extract_string())) excludes.push_back(t);
         break;
      case 'h':
      case '?':
         usage();
         exit(0);
      }
   }
}

static inline bool match3(const std::vector<const char*>& patterns, const char* subject)
{
   for (auto pattern : patterns) {
      if (strcasestr(subject, pattern)) return true;
      if (strcspn(pattern, "?*[]") < strlen(pattern) && h2_pattern::wildcard_match(pattern, subject, true)) return true;
      // if (strcspn(pattern, "?*[]+^$\\.") < strlen(pattern) && h2_pattern::regex_match(pattern, subject, true)) return true;
   }
   return false;
}

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* file, int lino) const
{
   char filename[1024];
   sprintf(filename, "%s:%d", file, lino);
   if (!includes.empty())
      if (!match3(includes, suitename) && !match3(includes, casename) && !match3(includes, filename))
         return true;
   if (!excludes.empty())
      if (match3(excludes, suitename) || match3(excludes, casename) || match3(excludes, filename))
         return true;
   return false;
}
