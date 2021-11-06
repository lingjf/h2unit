/* clang-format off */
static inline void usage()
{
   ::printf(" \033[90mhttps://github.com/lingjf/\033[0m\033[32mh2unit\033[0m \033[90mv\033[0m%s \033[90m%s\033[0m\n", H2PP_STR(H2UNIT_VERSION), H2PP_STR(H2UNIT_REVISION));
#define H2_USAGE_BR "\033[90m├─────┼───────────┼────────────────────────────────────────────────────────────┤\033[0m\n"
   ::printf("\033[90m┌─────┬───────────┬────────────────────────────────────────────────────────────┐\033[0m\n"
            "\033[90m│\033[0m" " -\033[36mb\033[0m  "                               "\033[90m│\033[0m" "   \033[90m[\033[0mn=1\033[90m]\033[0m   "     "\033[90m│\033[0m" " \033[36mb\033[0mreak test once n (default 1) cases failed                 "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mc\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36mc\033[0montinue asserts even if failure occurred                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36md\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36md\033[0mebug with gdb once failure occurred                       "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mf\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Only test last \033[36mf\033[0mailed cases                                "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mi\033[0m\033[90m/\033[0m\033[36me\033[0m" "\033[90m│\033[0m" "\033[90m[\033[0mpattern .\033[90m]\033[0m"     "\033[90m│\033[0m" " \033[36mi\033[0mnclude\033[90m/\033[0m\033[36me\033[0mxclude case suite or file by substr wildcard      " "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mj\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mn=max\033[90m]\033[0m  "     "\033[90m│\033[0m" " Fold \033[36mj\033[0mson print, 0:unfold 1:short 2:same 3:single          "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36ml\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mtype .\033[90m]\033[0m "     "\033[90m│\033[0m" " \033[36ml\033[0mist out suites and cases, type [suite case todo]          "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mm\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Test cases without \033[36mm\033[0memory check                            "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mn\033[0m  "                               "\033[90m│\033[0m" "   \033[90m[\033[0mn=2\033[90m]\033[0m   "     "\033[90m│\033[0m" " Repeat test \033[36mn\033[0m (default 2) rounds                           "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mo\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mpath\033[90m]\033[0m   "     "\033[90m│\033[0m" " \033[36mo\033[0mutput junit report, default is <executable>.junit.xml     "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mp\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Disable test percentage \033[36mp\033[0mrogressing bar                    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mq\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36mq\033[0muit exit code as failed cases count                       "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mr\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " test cases in \033[36mr\033[0mandom order if no last failed               "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36ms\033[0m  "                               "\033[90m│\033[0m" "\033[90m[\033[0mtype=\\\\\\\"\033[90m]\033[0m" "\033[90m│\033[0m" " JSON C/C++ \033[36ms\033[0mource code, type [\\\'/single \\\"/double \\\\\\\"]    "                       "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mv\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mn=max\033[90m]\033[0m  "     "\033[90m│\033[0m" " \033[36mv\033[0merbose, 0:quiet 1/2:compact 3:normal 4:details            "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mw\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Console output in black-\033[36mw\033[0mhite color style                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mx\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Thrown e\033[36mx\033[0mception is considered as failure                  "                               "\033[90m│\033[0m\n"
            "\033[90m└─────┴───────────┴────────────────────────────────────────────────────────────┘\033[0m\n");
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
      for (int k = 0; k < argc; ++k) s += sprintf(s, " %s", args[k]);
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
         case 'b': get.extract_number(break_after_fails = 1); break;
         case 'c': continue_assert = true; break;
         case 'd': debugger_trap = true; break;
         case 'e':
            while ((t = get.extract_string())) excludes.push_back(t);
            break;
         case 'f': only_last_failed = true; break;
         case 'i':
            while ((t = get.extract_string())) includes.push_back(t);
            break;
         case 'j': get.extract_number(fold_json = 0); break;
         case 'l':
            while ((t = get.extract_string())) list_cases.push_back(t);
            if (!list_cases.size()) list_cases.push_back("all");
            memory_check = false;
            break;
         case 'm': memory_check = !memory_check; break;
         case 'n': get.extract_number(run_rounds = 2); break;
         case 'o':
            sprintf(junit_path, "%s.junit.xml", path);
            if ((t = get.extract_string())) strcpy(junit_path, t);
            break;
         case 'p': progressing = !progressing; break;
         case 'q': quit_exit_code = true; break;
         case 'r': shuffle_cases = true; break;
         case 's':
            json_source_quote = "\\\"";
            if ((t = get.extract_string())) {
               json_source_quote = t;
               if (!strcasecmp("single", t)) json_source_quote = "\'";
               if (!strcasecmp("double", t)) json_source_quote = "\"";
               if (strcasecmp("\'", t) && strcasecmp("\"", t) && strcasecmp("\\\"", t)) json_source_quote = "\\\"";
            }
            break;
         case 'v': get.extract_number(verbose = 8); break;
         case 'w': colorful = !colorful; break;
         case 'x': exception_as_fail = true; break;
         case 'h':
         case '?': usage(); exit(0);
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

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* fileline) const
{
   if (!includes.empty())
      if (!match3(includes, suitename) && !match3(includes, casename) && !match3(includes, fileline))
         return true;
   if (!excludes.empty())
      if (match3(excludes, suitename) || match3(excludes, casename) || match3(excludes, fileline))
         return true;
   return false;
}
