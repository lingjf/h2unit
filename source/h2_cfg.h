
#ifndef ___H2_CFG__H___
#define ___H2_CFG__H___

#include <getopt.h>
#include <sys/ioctl.h>

#ifdef _WIN32

static inline int getopt(int argc, char** argv, char* opts)
{
   static int sp = 1;
   register int c;
   register char* cp;

   int opterr = 1;
   int optind = 1;
   int optopt;
   char* optarg;

   if (sp == 1)
      if (optind >= argc ||
          argv[optind][0] != '-' || argv[optind][1] == '\0') {
         return (-1);
      }
      else if (strcmp(argv[optind], "--") == NULL) {
         optind++;
         return (-1);
      }
   optopt = c = argv[optind][sp];
   if (c == ':' || (cp = strchr(opts, c)) == NULL) {
      if (argv[optind][++sp] == '\0') {
         optind++;
         sp = 1;
      }
      return ('?');
   }
   if (*++cp == ':') {
      if (argv[optind][sp + 1] != '\0') {
         optarg = &argv[optind++][sp + 1];
      }
      else if (++optind >= argc) {
         sp = 1;
         return ('?');
      }
      else {
         optarg = argv[optind++];
      }
      sp = 1;
   }
   else {
      if (argv[optind][++sp] == '\0') {
         sp = 1;
         optind++;
      }
      optarg = NULL;
   }
   return (c);
}

#endif

struct h2_cfg
{
   char* path;
   bool verbose;
   bool colorable;
   bool randomize;
   bool memory_check;
   char* junit;

   std::vector<const char*> include_patterns;
   std::vector<const char*> exclude_patterns;

   h2_cfg()
     : path(nullptr), verbose(false), colorable(true), randomize(false), memory_check(true), junit(nullptr) {}
   ~h2_cfg() {}

   static h2_cfg& I()
   {
      static h2_cfg I;
      return I;
   }

   void configure(int argc, char** argv)
   {
      path = argv[0];

      int c;
      while (-1 != (c = getopt(argc, argv, "vcrmj:i:x:h?"))) {
         switch (c) {
         case 'v': verbose = true; break;
         case 'c': colorable = !colorable; break;
         case 'r': randomize = true; break;
         case 'm': memory_check = !memory_check; break;
         case 'j': junit = optarg; break;
         case 'i': include_patterns.push_back(optarg); break;
         case 'x': exclude_patterns.push_back(optarg); break;
         case 'h':
         case '?':
         default:
            printf("Usage: \n"
                   "-v                  Make the operation more talkative\n"
                   "-c                  Output in black-white color mode\n"
                   "-r                  Run cases in random order\n"
                   "-m                  Run cases without memory check\n"
                   "-j {path}           Generate junit report\n"
                   "-i {pattern}        Run cases which case name or unit name matches\n"
                   "-x {pattern}        Run cases which case name and unit name not matches\n");
            exit(0);
         }
      }
   }

   int filter(std::vector<const char*>& patterns, const char* subject)
   {
      if (patterns.empty()) {
         return -1;
      }
      for (auto it = patterns.begin(); it != patterns.end(); it++) {
         const char* pattern = *it;
         if (strchr(pattern, '?') || strchr(pattern, '*')) {
            if (h2_wildcard_match(pattern, subject)) return 1;
         }
         else {
            if (strstr(subject, pattern)) return 1;
         }
      }
      return 0;
   }

   bool filter(const char* suitename, const char* casename)
   {
      if (0 == filter(include_patterns, suitename) && 0 == filter(include_patterns, casename)) {
         return true;
      }
      if (1 == filter(exclude_patterns, suitename) || 1 == filter(exclude_patterns, casename)) {
         return true;
      }
      return false;
   }

   static const char* style(const char* style_str)
   {
      if (!I().colorable) { return ""; }

      static char shift_buffer[8][128];
      static long shift_index = 0;

      shift_index = (shift_index + 1) % 8;

#if defined(_WIN32)

#else

      return h2_style(style_str, shift_buffer[shift_index]);
#endif
   }

   static int get_term_columns()
   {
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      return w.ws_col;
   }
};

#endif
