static inline void usage()
{
   ::printf("\033[33m╭─────────────────────────────────────────────────────────────────────────────╮\033[0m\n");
   ::printf("\033[33m│\033[0m                \033[32mh2unit \033[31m%-5s \033[34;4mhttps://github.com/lingjf/h2unit\033[0m \033[0;36m               \033[33m│\033[0m\n", H2PP_STR(H2UNIT_VERSION));
   ::printf("\033[33m╰─────────────────────────────────────────────────────────────────────────────╯\033[0m\n");

#define H2_USAGE_SP "\033[90m│\033[0m"
#define H2_USAGE_BR "\033[90m├─────┼───────────┼───────────────────────────────────────────────────────────┤\033[0m\n"
   char b[] = "\033[90m┌─────┬───────────┬───────────────────────────────────────────────────────────┐\033[0m\n"
              H2_USAGE_SP " -\033[36ml\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36ml\033[0mist out suites and cases                                 " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mm\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Test cases without \033[36mm\033[0memory check                           " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36ms\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36ms\033[0mhuffle cases then test in random order                   " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mb\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mb\033[0mreak test once n (default 1) cases failed                " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mf\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Only test previous \033[36mf\033[0mailed cases                           " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mp\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Hide test percentage \033[36mp\033[0mrogressing                          " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mr\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " Repeat test n (default 2) \033[36mr\033[0mounds                          " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mc\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Output in black-white \033[36mc\033[0molor style                         " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mF\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36mF\033[0mold simple JSON object or array                          " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36my\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Cop\033[36my\033[0m-paste JSON C/C++ source code                         " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mx\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Thrown e\033[36mx\033[0mception is considered as failure                 " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mv\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mv\033[0merbose output, 0:compact 1:normal 2:check all 3:default  " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36md\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36md\033[0mebug with gdb once failure occurred                      " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mj\033[0m  " H2_USAGE_SP "  \033[90m[\033[0mpath\033[90m]\033[0m   " H2_USAGE_SP " Generate \033[36mj\033[0munit report, default is <executable>.junit.xml  " H2_USAGE_SP "\n"
              "\033[90m├─────┼───────────┼─────────┬─────────────────────────────────────────────────┤\033[0m\n"
              H2_USAGE_SP " -\033[36mi\033[0m  " H2_USAGE_SP "  pattern  " H2_USAGE_SP " \033[36mi\033[0mnclude " H2_USAGE_SP " Filter case, suite or file by substr/wildcard   " H2_USAGE_SP "\n"
              H2_USAGE_SP " -\033[36me\033[0m  " H2_USAGE_SP " \033[90m[\033[0m pattern\033[90m]\033[0m" H2_USAGE_SP " \033[36me\033[0mxclude " H2_USAGE_SP " matching name with patterns case-insensitively  " H2_USAGE_SP "\n"
              "\033[90m└─────┴───────────┴─────────┴─────────────────────────────────────────────────┘\033[0m\n";
   ::printf("%s", b);
}

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

h2_inline void h2_option::parse(int argc, const char** argv)
{
   path = argv[0];
   getopt get(argc - 1, argv + 1);
   get.arguments(args);

   for (const char* t;;) {
      switch (get.next_option()) {
      case '\0': return;
      case 'c': colorful = !colorful; break;
      case 's': shuffle_cases = true; break;
      case 'f': only_previous_failed = true; break;
      case 'F': fold_json = !fold_json; break;
      case 'p': percent_progressing = !percent_progressing; break;
      case 'y': copy_paste_json = true; break;
      case 'm': memory_check = !memory_check; break;
      case 'x': exception_as_fail = true; break;
      case 'l': list_cases = true; break;
      case 'b': break_after_fails = 1, get.extract_number(break_after_fails); break;
      case 'r': run_rounds = 2, get.extract_number(run_rounds); break;
      case 'v': verbose = 9, get.extract_number(verbose); break;
      case 'd': debug = "gdb new"; break;
      case 'D': debug = "gdb attach"; break;
      case 'j':
         sprintf(junit_path, "%s.junit.xml", path);
         if ((t = get.extract_string())) strcpy(junit_path, t);
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
