
static inline void usage()
{
   ::printf("  \033[33m╭─────────────────────────────────────────────────────────────────────────╮\033[0m\n");
   ::printf("  \033[33m│\033[0m                                                                         \033[33m│\033[0m\n");
   ::printf("  \033[33m│\033[0m                       Current version \033[32mh2unit \033[31m%-9s                  \033[33m│\033[0m\n", H2UNIT_VERSION);
   ::printf("  \033[33m│\033[0m         Manual: \033[34;4mhttps://github.com/lingjf/h2unit.git \033[0;36mREADME.md          \033[33m│\033[0m\n");
   ::printf("  \033[33m│\033[0m                                                                         \033[33m│\033[0m\n");
   ::printf("  \033[33m╰─────────────────────────────────────────────────────────────────────────╯\033[0m\n");

   ::printf("\
  ┌────────┬───────────┬────────────────────────────────────────────────────┐\n\
  │ Option │ Parameter │ Description                                        │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -v     │           │ Verbose output                                     │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -l     │           │ List out suites and cases                          │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -r     │           │ Run suite and cases in random order.               │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -b[n]  │    [n]    │ Breaking test once n (default 1) failures occurred │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -s[n]  │    [n]    │ Repeat run n times (default 1) when no failure     │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -c     │           │ Output in black-white color style                  │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -m     │           │ Run cases without memory check                     │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -d     │           │ Debug with gdb once failure occurred               │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -j     │  [path]   │ Generate junit report, default path is junit.xml   │\n\
  ├────────┼───────────┼────────────────┬───────────────────────────────────┤\n\
  │ -i     │ patterns  │ include filter │ case, suite or file name          │\n\
  ├────────┤ separated ├────────────────┤ case-insensitive matches patterns │\n\
  │ -x     │ by space  │ exclude filter │ default include all, exclude none │\n\
  └────────┴───────────┴────────────────┴───────────────────────────────────┘\n\
\n");
}

struct getopt {
   int argc;
   const char* argv[100];
   int offset;
   getopt(int argc_, const char** argv_) : argc(argc_), offset(0)
   {
      for (int i = 0; i < argc; ++i)
         argv[i] = argv_[i];
   }
   const char* next()
   {
      for (; offset < argc; ++offset)
         if (argv[offset])
            return argv[offset++];
      return nullptr;
   }
   const char* extract()
   {
      const char **pp = nullptr, *p = nullptr;
      for (int i = offset; i < argc; ++i)
         if (argv[i]) {
            pp = argv + i;
            break;
         }

      if (pp && *pp[0] != '-') p = *pp, *pp = nullptr;
      return p;
   }
   const char* parseint(const char* p, int& value) const
   {
      for (value = 0; ::isdigit(*p); p++) value = value * 10 + (*p - '0');
      return p - 1;
   }
   void args(char* s)
   {
      for (int i = 0; i < argc; ++i)
         s += sprintf(s, " %s", argv[i]);
   }
};

h2_inline void h2_option::parse(int argc, const char** argv)
{
   path = argv[0];
   getopt get(argc - 1, argv + 1);
   get.args(args);

   for (const char* p; p = get.next();) {
      if (p[0] != '-') continue;
      for (const char* t; *p; p++) {
         switch (*p) {
         case 'v': verbose = 1; break;
         case 'l': listing = true; break;
         case 'b':
            breakable = 1;
            if (::isdigit(*(p + 1)))
               p = get.parseint(p + 1, breakable);
            else if ((t = get.extract()))
               breakable = atoi(t);
            break;
         case 's':
            times = 1;
            if (::isdigit(*(p + 1)))
               p = get.parseint(p + 1, times);
            else if ((t = get.extract()))
               times = atoi(t);
            break;
         case 'c': colorable = !colorable; break;
         case 'r':
            randomize = 'A';
            if ((t = get.extract())) randomize = t[0];
            break;
         case 'm': memory_check = !memory_check; break;
         case 'd': debug = "gdb new"; break;
         case 'D': debug = "gdb attach"; break;
         case 'j':
            sprintf(junit, "%s.xml", path);
            if ((t = get.extract())) strcpy(junit, t);
            break;
         case 'i':
            while ((t = get.extract())) includes.push_back(t);
            break;
         case 'x':
            while ((t = get.extract())) excludes.push_back(t);
            break;
         case '-': break;
         case 'h':
         case '?':
            usage();
            exit(0);
         default:
            ::printf("unknown option: -%c, -h for help\n", *p);
            exit(0);
         }
      }
   }
#ifdef _WIN32
   memory_check = false;
#endif
}

static inline bool match3(const std::vector<const char*>& patterns, const char* subject)
{
   for (auto pattern : patterns)
      if (strcspn(pattern, "?*+^$\\.[]") < strlen(pattern)) {
         if (h2_regex_match(pattern, subject, true)) return true;
         if (strcspn(pattern, "+^$\\.[]") == strlen(pattern))
            if (h2_wildcard_match(pattern, subject, true)) return true;
      } else {
         if (strcasestr(subject, pattern)) return true;
      }
   return false;
}

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* filename) const
{
   if (includes.size())
      if (!match3(includes, suitename) && !match3(includes, casename) && !match3(includes, filename))
         return true;
   if (excludes.size())
      if (match3(excludes, suitename) || match3(excludes, casename) || match3(excludes, filename))
         return true;
   return false;
}
