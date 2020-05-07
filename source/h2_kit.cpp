
#ifdef _WIN32

static inline char* basename(char* path)
{
   static char t[MAX_PATH + 1];
   strcpy(t, path);
   PathRemoveFileSpecA(t);
   return t;
}

#endif

static inline bool h2_regex_match(const char* pattern, const char* subject, bool caseless = false)
{
   bool result = false;
   try {  // c++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error&) {
      result = false;
   }
   return result;
}

static inline bool h2_wildcard_match(const char* pattern, const char* subject, bool caseless = false)
{
#ifdef _WIN32
   const char *scur = subject, *pcur = pattern;
   const char *sstar = nullptr, *pstar = nullptr;
   while (*scur) {
      if (caseless ? ::tolower(*scur) == ::tolower(*pcur) : *scur == *pcur || *pcur == '?') {
         ++scur;
         ++pcur;
      } else if (*pcur == '*') {
         pstar = pcur++;
         sstar = scur;
      } else if (pstar) {
         pcur = pstar + 1;
         scur = ++sstar;
      } else
         return false;
   }
   while (*pcur == '*') ++pcur;
   return !*pcur;
#else
   return !fnmatch(pattern, subject, caseless ? FNM_CASEFOLD : 0);
#endif
}

static inline long long h2_now()
{
   return clock() * 1000 / CLOCKS_PER_SEC;
}

static inline void h2_sleep(long long milliseconds)
{
#ifdef _WIN32
   Sleep(milliseconds);
#else
   ::usleep(milliseconds * 1000);
#endif
}

static inline int h2_page_size()
{
   static int s_page_size = 0;
   if (s_page_size == 0) {
#ifdef _WIN32
      SYSTEM_INFO si;
      GetSystemInfo(&si);
      s_page_size = (int)si.dwPageSize;
#else
      s_page_size = sysconf(_SC_PAGESIZE);
#endif
   }
   return s_page_size;
}

static inline int h2_term_size()
{
#ifdef _WIN32
   return 80;
#else
   struct winsize w;
   if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) return 80;
   return w.ws_col;
#endif
}

static inline const char* PAD(int n)
{
   static char st[1024];
   memset(st, ' ', n);
   st[n] = '\0';
   return st;
}

static inline char* SF(const char* style, const char* fmt, ...)
{
   static char sb[1024 * 256], *sp;
   if (sp < sb || sb + sizeof(sb) / 2 < sp) sp = sb;
   char *s = sp, *p = s;

   p += sprintf(p, "%s", h2_option::I().style(style));
   va_list a;
   va_start(a, fmt);
   p += vsprintf(p, fmt, a);
   va_end(a);
   p += sprintf(p, "%s", h2_option::I().style("reset"));

   sp = p + 1;
   return s;
}

struct h2_color {
   struct st {
      const char* name;
      int value;
   };
   static struct st* find(struct st* s, int n, const char* name)
   {
      for (int i = 0; i < n; ++i)
         if (!strcmp(s[i].name, name))
            return &s[i];
      return nullptr;
   }
   static void print(char* style)
   {
#ifdef _WIN32
      static HANDLE console_handle = NULL;
      static WORD default_attribute;
      if (console_handle == NULL) {
         console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
         CONSOLE_SCREEN_BUFFER_INFO csbi;
         GetConsoleScreenBufferInfo(console_handle, &csbi);
         default_attribute = csbi.wAttributes;
      }
      static struct st K[] = {
        // normal style
        {"reset", default_attribute},
        {"bold", FOREGROUND_INTENSITY},
        // { "italics", 3 },
        {"underline", COMMON_LVB_UNDERSCORE},
        {"inverse", COMMON_LVB_REVERSE_VIDEO},
        // { "strikethrough", 9 },
        // foreground color
        // { "black", 30 },
        {"red", FOREGROUND_RED},
        {"green", FOREGROUND_GREEN},
        {"yellow", FOREGROUND_RED | FOREGROUND_GREEN},
        {"blue", FOREGROUND_BLUE},
        {"purple", FOREGROUND_RED | FOREGROUND_BLUE},
        {"cyan", FOREGROUND_BLUE | FOREGROUND_GREEN},
        {"white", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"dark gray", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"default", 39},
        // background color
        // { "bg_black", 40 },
        {"bg_red", BACKGROUND_RED},
        {"bg_green", BACKGROUND_GREEN},
        {"bg_yellow", BACKGROUND_RED | BACKGROUND_GREEN},
        {"bg_blue", BACKGROUND_BLUE},
        {"bg_purple", BACKGROUND_RED | BACKGROUND_BLUE},
        {"bg_cyan", BACKGROUND_BLUE | BACKGROUND_GREEN},
        {"bg_white", BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE},
        // { "bg_default", 49 }
      };

      WORD a = 0;
#else
      static struct st K[] = {
        {"reset", 0},
        {"bold", 1},
        {"italics", 3},
        {"underline", 4},
        {"inverse", 7},
        {"strikethrough", 9},
        {"black", 30},
        {"red", 31},
        {"green", 32},
        {"yellow", 33},
        {"blue", 34},
        {"purple", 35},
        {"cyan", 36},
        {"gray", 37},
        {"default", 39},
        {"dark gray", 90},
        {"light red", 91},
        {"light green", 92},
        {"light yellow", 93},
        {"light blue", 94},
        {"light purple", 95},
        {"light cyan", 96},
        {"white", 97},
        {"bg_black", 40},
        {"bg_red", 41},
        {"bg_green", 42},
        {"bg_yellow", 43},
        {"bg_blue", 44},
        {"bg_purple", 45},
        {"bg_cyan", 46},
        {"bg_white", 47},
        // {"bg_default", 49}
      };
      char a[256] = "\033[";
#endif
      for (char* p = strtok(style, ","); p; p = strtok(NULL, ",")) {
         struct st* s = find(K, sizeof(K) / sizeof(K[0]), p);
         if (!s) continue;
#ifdef _WIN32
         a |= s->value;
#else
         sprintf(a + strlen(a), "%d;", s->value);
#endif
      }
#ifdef _WIN32
      SetConsoleTextAttribute(console_handle, a);
#else
      if (!strcmp("\033[", a)) return;  // no style
      a[strlen(a) - 1] = 'm';
      h2_libc::write(1, a, strlen(a));
#endif
   }
};

static inline int h2_printf(const char* format, ...)
{
   int len, ret = 0;
   va_list a, b;
   va_start(a, format);
   len = vsnprintf(nullptr, 0, format, a);
   char* tmp = (char*)alloca(len + 1);
   va_end(a);

   va_start(b, format);
   len = vsnprintf(tmp, len + 1, format, b);
   va_end(b);

   for (char* p = tmp; *p; p++) {
      if (!memcmp("\033[", p, 2)) {
         char* q = strchr(p + 2, ']');
         if (!q) return 0;
         *q = '\0';
         h2_color::print(p + 2);
         p = q;
      } else {
         ret += h2_libc::write(fileno(stdout), p, 1);
      }
   }

   return ret;
}
