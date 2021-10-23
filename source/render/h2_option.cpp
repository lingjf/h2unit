/* clang-format off */
static inline void usage()
{
#define H2_USAGE_SP "\033[90m│\033[0m"
#define H2_USAGE_BR "\033[90m├─────┼───────────┼────────────────────────────────────────────────────────────┤\033[0m\n"
   char b[] = "\033[90m┌─────┬───────────┬────────────────────────────────────────────────────────────┐\033[0m\n"
              H2_USAGE_SP " -\033[36ma\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36ma\033[0mttend to last failed cases                                " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mb\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mb\033[0mreak test once n (default 1) cases failed                 " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mc\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Output in black-white \033[36mc\033[0molor style                          " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36md\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36md\033[0mebug with gdb once failure occurred                       " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mf\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mf\033[0mold JSON object or array, bigger n more folded            " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mi\033[0m/\033[36me\033[0m" H2_USAGE_SP "\033[90m[\033[0mpattern .\033[90m]\033[0m" H2_USAGE_SP " \033[36mi\033[0mnclude/\033[36me\033[0mxclude case, suite or file by substr/wildcard     " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mj\033[0m  " H2_USAGE_SP "  \033[90m[\033[0mpath\033[90m]\033[0m   " H2_USAGE_SP " Generate \033[36mj\033[0munit report, default is <executable>.junit.xml   " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36ml\033[0m  " H2_USAGE_SP "  \033[90m[\033[0mtype .\033[90m]\033[0m " H2_USAGE_SP " \033[36ml\033[0mist out suites and cases, type [suite case todo]          " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mm\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Test cases without \033[36mm\033[0memory check                            " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mp\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Disable test percentage \033[36mp\033[0mrogressing bar                    " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mr\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " Repeat test n (default 2) \033[36mr\033[0mounds                           " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36ms\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36ms\033[0mhuffle cases then test in random order if no last failed  " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mv\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mv\033[0merbose, 0:compact 2:normal 4:abbreviate default:all       " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mx\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Thrown e\033[36mx\033[0mception is considered as failure                  " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36my\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " Cop\033[36my\033[0m-paste-able C/C++ source code formatted JSON           " H2_USAGE_SP "\n"
              "\033[90m└─────┴───────────┴────────────────────────────────────────────────────────────┘\033[0m\n";

   ::printf(" \033[90mhttps://github.com/lingjf/\033[0m\033[32mh2unit\033[0m \033[90mv\033[0m%-5s \n%s", H2PP_STR(H2UNIT_VERSION), b);
}
/* clang-format on */

struct getopt {
   int argc;
   const char** args;  // argv + 1
   int i = -1;
   const char* j = nullptr;

   getopt(int argc_, const char** args_) : argc(argc_), args(args_) {}

   const char* extract_next() { return ++i < argc ? args[i] : nullptr; }
   const char* extract_string() { return (i + 1 < argc && args[i + 1] && args[i + 1][0] != '-') ? args[++i] : nullptr; }

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
         auto l = strspn(j + 1, "0123456789");
         if (l) {
            value = atoi(j + 1);
            j += l;
            return;
         }
      }

      if (i + 1 < argc) {
         auto l = strlen(args[i + 1]);
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

h2_inline void h2_option::parse(int argc, const char** argv)
{
   path = argv[0];
   getopt get(argc - 1, argv + 1);
   get.arguments(args);

   for (const char* t;;) {
      switch (get.next_option()) {
         case '\0': return;
         case 'a': last_failed = true; break;
         case 'b': break_after_fails = 1, get.extract_number(break_after_fails); break;
         case 'c': colorful = !colorful; break;
         case 'd': debug = "gdb new"; break;
         case 'D': debug = "gdb attach"; break;
         case 'e':
            while ((t = get.extract_string())) excludes.push_back(t);
            break;
         case 'f': fold_json = 0, get.extract_number(fold_json); break;
         case 'i':
            while ((t = get.extract_string())) includes.push_back(t);
            break;
         case 'j':
            sprintf(junit_path, "%s.junit.xml", path);
            if ((t = get.extract_string())) strcpy(junit_path, t);
            break;
         case 'l':
            while ((t = get.extract_string())) {
               if (!strcasecmp("suite", t)) list_cases |= 1;
               if (!strcasecmp("case", t)) list_cases |= 2;
               if (!strcasecmp("todo", t)) list_cases |= 4;
            }
            if (!list_cases) list_cases = 1 | 2 | 4;
            break;
         case 'm': memory_check = !memory_check; break;
         case 'p': progressing = !progressing; break;
         case 'r': run_rounds = 2, get.extract_number(run_rounds); break;
         case 's': shuffle_cases = true; break;
         case 'v': verbose = 8, get.extract_number(verbose); break;
         case 'x': exception_as_fail = true; break;
         case 'y': copy_paste_json = 3, get.extract_number(copy_paste_json); break;
         case 'h':
         case '?': usage(); exit(0);
      }
   }
   if (list_cases) memory_check = false;
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

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* file, int line) const
{
   char filename[1024];
   sprintf(filename, "%s:%d", file, line);
   if (!includes.empty())
      if (!match3(includes, suitename) && !match3(includes, casename) && !match3(includes, filename))
         return true;
   if (!excludes.empty())
      if (match3(excludes, suitename) || match3(excludes, casename) || match3(excludes, filename))
         return true;
   return false;
}
