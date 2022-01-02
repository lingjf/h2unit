static inline int usage(const char* option)
{
   if (option && (!strcmp("i", option) || !strcmp("e", option))) {
      return ::printf("\033[36m-i/e pattern\033[0m: include in file:line, suite name, case name, suite tags, case tags\n"
                      "\033[36m-i/e file=pattern\033[0m: include in file:line\n"
                      "\033[36m-i/e suite=pattern\033[0m: include in suite name\n"
                      "\033[36m-i/e case=pattern\033[0m: include in case name\n"
                      "\033[36m-i/e tags=pattern\033[0m: include in suite tags or case tags\n");
   }
   if (option && !strcmp("W", option)) {
      return ::printf("\033[36mexception\033[0m: consider thrown exception as warning\n"
                      "\033[36muncaught\033[0m: consider uncaught exception as warning\n"
                      "\033[36mleak\033[0m: consider memory leak as warning\n"
                      "\033[36mviolate\033[0m: consider memory violate access as warning\n"
                      "\033[36mdouble_free\033[0m: consider memory double free as warning\n"
                      "\033[36masymmetric_free\033[0m: consider memory asymmetric free as warning\n");
   }
   ::printf(" \033[32mh₂unit\033[0m \033[90mv\033[0m%s  \033[90m%s\033[0m  \033[90m%s\033[0m\n", H2PP_STR(H2UNIT_VERSION), H2PP_STR(H2UNIT_DATE), H2PP_STR(H2UNIT_REVISION));
   /* clang-format off */
#define H2_USAGE_BR "\033[90m├─────┼───────────┼────────────────────────────────────────────────────────────┤\033[0m\n"
   return ::printf("\033[90m┌─────┬───────────┬────────────────────────────────────────────────────────────┐\033[0m\n"
                   "\033[90m│\033[0m" " -\033[36mb\033[0m  "                               "\033[90m│\033[0m" "   \033[90m[\033[0mn=1\033[90m]\033[0m   "     "\033[90m│\033[0m" " \033[36mb\033[0mreak test once n (default 1) cases failed                 "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mc\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36mc\033[0montinue asserts even if failure occurred                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mC\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Console output in black-white \033[36mC\033[0molor style                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36md\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36md\033[0mebug with gdb/lldb once failure occurred                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mf\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Only test last \033[36mf\033[0mailed cases                                "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mF\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mn=max\033[90m]\033[0m  "     "\033[90m│\033[0m" " \033[36mF\033[0mold json print, 0:unfold 1:short 2:same 3:single          "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mi\033[0m\033[90m/\033[0m\033[36me\033[0m" "\033[90m│\033[0m" "\033[90m[\033[0mpattern .\033[90m]\033[0m"     "\033[90m│\033[0m" " \033[36mi\033[0mnclude\033[90m/\033[0m\033[36me\033[0mxclude by case suite file or tags, -h i...        " "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36ml\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mtype .\033[90m]\033[0m "     "\033[90m│\033[0m" " \033[36ml\033[0mist suites cases and tags, type [suite case todo tags]    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mm\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Test cases without \033[36mm\033[0memory check                            "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mo\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mpath\033[90m]\033[0m   "     "\033[90m│\033[0m" " \033[36mo\033[0mutput junit report, default is <executable>.junit.xml     "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mp\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Disable test percentage \033[36mp\033[0mrogressing bar                    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mr\033[0m  "                               "\033[90m│\033[0m" "   \033[90m[\033[0mn=2\033[90m]\033[0m   "     "\033[90m│\033[0m" " Repeat test n (default 2) \033[36mr\033[0mounds                           "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36ms\033[0m  "                               "\033[90m│\033[0m" "\033[90m[\033[0mtype=rand\033[90m]\033[0m"     "\033[90m│\033[0m" " \033[36ms\033[0mhuffle cases random/alphabet/reverse if no last failed    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mS\033[0m  "                               "\033[90m│\033[0m" " \033[90m[\033[0mtype=\\\"\033[90m]\033[0m "   "\033[90m│\033[0m" " JSON C/C++ \033[36mS\033[0mource code, type [\\\'/single \\\"/double \\\\\\\"]    "                       "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mv\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mn=max\033[90m]\033[0m  "     "\033[90m│\033[0m" " \033[36mv\033[0merbose, 0:quiet 1/2/3:compact 4:normal 5:details          "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mx\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " e\033[36mx\033[0mit code as failed cases count                            "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mW\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mtype .\033[90m]\033[0m "     "\033[90m│\033[0m" " Configure failure as \033[36mW\033[0marning, exception, leak, -h W...     "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mh\033[0m\033[90m/\033[0m\033[36m?\033[0m" "\033[90m│\033[0m" "  \033[90m[\033[0moption\033[90m]\033[0m "     "\033[90m│\033[0m" " https://github.com/lingjf/h2unit/.../document/option.md    "                                              "\033[90m│\033[0m\n"
                   "\033[90m└─────┴───────────┴────────────────────────────────────────────────────────────┘\033[0m\n");
   /* clang-format on */
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

   char t2[64];
   for (const char* t;;) {
      switch (get.next_option()) {
         case '\0': return;
         case 'b': get.extract_number(break_after_fails = 1); break;
         case 'c': continue_assert = true; break;
         case 'C': colorful = !colorful; break;
         case 'd': debugger_trap = true; break;
         case 'e':
            while ((t = get.extract_string())) {
               const char* eq = strchr(t, '=');
               if (eq) {
                  const char* r = h2_candidate(strcpyn(t2, t, eq - t), 4, "file", "suite", "case", "tags");
                  if (!strcmp("file", r)) h2_array_append(file_excludes, eq + 1);
                  else if (!strcmp("suite", r)) h2_array_append(suite_excludes, eq + 1);
                  else if (!strcmp("case", r)) h2_array_append(case_excludes, eq + 1);
                  else if (!strcmp("tags", r)) h2_array_append(tags_excludes, eq + 1);
                  else ::printf("-e %s\n", r), exit(-1);
               } else h2_array_append(excludes, t);
            }
            break;
         case 'f': only_last_failed = true; break;
         case 'F': get.extract_number(fold_json = 0); break;
         case 'i':
            while ((t = get.extract_string())) {
               const char* eq = strchr(t, '=');
               if (eq) {
                  const char* r = h2_candidate(strcpyn(t2, t, eq - t), 4, "file", "suite", "case", "tags");
                  if (!strcmp("file", r)) h2_array_append(file_includes, eq + 1);
                  else if (!strcmp("suite", r)) h2_array_append(suite_includes, eq + 1);
                  else if (!strcmp("case", r)) h2_array_append(case_includes, eq + 1);
                  else if (!strcmp("tags", r)) h2_array_append(tags_includes, eq + 1);
                  else ::printf("-i %s\n", r), exit(-1);
               } else h2_array_append(includes, t);
            }
            break;
         case 'l':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 4, "suite", "case", "todo", "tags");
               if (!strcmp("suite", r)) lists |= ListSuite;
               else if (!strcmp("case", r)) lists |= ListCase;
               else if (!strcmp("todo", r)) lists |= ListTodo;
               else if (!strcmp("tags", r)) lists |= ListTags;
               else ::printf("-l %s\n", r), exit(-1);
            }
            if (!lists) lists = ListSuite | ListCase | ListTodo;
            break;
         case 'm': memory_check = !memory_check; break;
         case 'o':
            sprintf(junit_path, "%s.junit.xml", path);
            if ((t = get.extract_string())) strcpy(junit_path, t);
            break;
         case 'p': progressing = !progressing; break;
         case 'r': get.extract_number(run_rounds = 2); break;
         case 's':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 4, "random", "name", "file", "reverse");
               if (!strcmp("random", r)) shuffles |= ShuffleRandom;
               else if (!strcmp("name", r)) shuffles |= ShuffleName;
               else if (!strcmp("file", r)) shuffles |= ShuffleFile;
               else if (!strcmp("reverse", r)) shuffles |= ShuffleReverse;
               else ::printf("-s %s\n", r), exit(-1);
            }
            if (!shuffles) shuffles = ShuffleRandom;
            break;
         case 'S':
            json_source_quote = "\\\"";
            if ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 5, "\'", "single", "\"", "double", "\\\"");
               if (!strcmp("\'", r) || !strcmp("single", r)) json_source_quote = "\'";
               else if (!strcmp("\"", r) || !strcmp("double", r)) json_source_quote = "\"";
               else if (!strcmp("\\\"", r)) json_source_quote = "\\\"";
               else ::printf("-S %s\n", r), exit(-1);

               if (!h2_in(json_source_quote, 3, "\'", "\"", "\\\"")) json_source_quote = "\\\"";
            }
            break;
         case 'v': get.extract_number(verbose = 8); break;
         case 'x': exit_with_fails = true; break;
         case 'W':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 5, "exception", "uncaught", "leak", "violate", "asymmetric_free");
               if (!strcmp("exception", r)) as_waring_exception = true;
               else if (!strcmp("uncaught", r)) as_waring_uncaught = true;
               else if (!strcmp("leak", r)) as_waring_memory_leak = true;
               else if (!strcmp("violate", r)) as_waring_memory_violate = true;
               else if (!strcmp("double_free", r)) as_waring_memory_double_free = true;
               else if (!strcmp("asymmetric_free", r)) as_waring_memory_asymmetric_free = true;
               else ::printf("-W %s\n", r), exit(-1);
            }
            break;
         case 'h':
         case '?': usage(get.extract_string()); exit(0);
      }
   }
}
