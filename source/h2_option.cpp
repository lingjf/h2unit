
struct getopt {
   int argc;
   const char* argv[100];
   int offset;
   getopt(int argc_, const char** argv_) : argc(argc_), offset(0) {
      for (int i = 0; i < argc; ++i)
         argv[i] = argv_[i];
   }
   const char* next() {
      for (; offset < argc; ++offset)
         if (argv[offset])
            return argv[offset++];
      return nullptr;
   }
   const char* extract() {
      const char **pp = nullptr, *p = nullptr;
      for (int i = offset; i < argc; ++i)
         if (argv[i]) {
            pp = argv + i;
            break;
         }

      if (pp && *pp[0] != '-') p = *pp, *pp = nullptr;
      return p;
   }
   const char* parseint(const char* p, int& value) const {
      for (value = 0; ::isdigit(*p); p++) value = value * 10 + (*p - '0');
      return p - 1;
   }

   void args(char* s) {
      for (int i = 0; i < argc; ++i)
         s += sprintf(s, " %s", argv[i]);
   }
};

h2_inline void h2_option::parse(int argc_, const char** argv_) {
   path = argv_[0];
   getopt get(argc_ - 1, argv_ + 1);
   get.args(args);

   for (const char* p; p = get.next();) {
      if (p[0] != '-') continue;
      for (const char* t; *p; p++) {
         switch (*p) {
         case 'v': verbose = 1; break;
         case 'l':
            listing = 'A';
            if ((t = get.extract())) listing = t[0];
            break;
         case 'b':
            breakable = 1;
            if (::isdigit(*(p + 1)))
               p = get.parseint(p + 1, breakable);
            else if ((t = get.extract()))
               breakable = atoi(t);
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
            while ((t = get.extract())) include_patterns.push_back(t);
            break;
         case 'x':
            while ((t = get.extract())) exclude_patterns.push_back(t);
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

h2_inline void h2_option::usage() {
   printf("Usage:\n"
          "-v                  Make the operation more talkative\n"
          "-l [sca]            List out suites and cases\n"
          "-b [n]              Breaking test once n (default is 1) failures occurred\n"
          "-c                  Output in black-white color mode\n"
          "-r [sca]            Run cases in random order\n"
          "-m                  Run cases without memory check\n"
          "-d/D                Debug mode, -D for gdb attach but sudo requires password\n"
          "-j [path]           Generate junit report, default is .xml\n"
          "-i {patterns}       Run cases which case name, suite name or file name matches\n"
          "-x {patterns}       Run cases which case name, suite name and file name not matches\n");
}

static inline bool match3(const std::vector<const char*>& patterns, const char* subject) {
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

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* filename) const {
   if (include_patterns.size())
      if (!match3(include_patterns, suitename) && !match3(include_patterns, casename) && !match3(include_patterns, filename))
         return true;
   if (exclude_patterns.size())
      if (match3(exclude_patterns, suitename) || match3(exclude_patterns, casename) || match3(exclude_patterns, filename))
         return true;
   return false;
}

h2_inline const char* h2_option::style(const char* s) const {
   static char shift_buffer[32][128];
   static long shift_index = 0;
   if (!colorable) return "";
   return h2_style(s, shift_buffer[++shift_index % 32]);
}
