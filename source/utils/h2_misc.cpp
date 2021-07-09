
static inline const char* h2_basename(const char* path)
{
   const char* p = strrchr(path, '/');
   if (!p) p = strrchr(path, '\\');
   return p ? p + 1 : path;
}

h2_inline bool h2_pattern::regex_match(const char* pattern, const char* subject, bool caseless)
{
   bool result = false;
   h2_memory::restores();
   try {  // c++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error&) {
      result = false;
   }
   h2_memory::overrides();
   return result;
}

h2_inline bool h2_pattern::wildcard_match(const char* pattern, const char* subject, bool caseless)
{
#if defined _WIN32
   const char *scur = subject, *pcur = pattern;
   const char *sstar = nullptr, *pstar = nullptr;
   while (*scur) {
      if (*pcur == '?') {
         ++scur;
         ++pcur;
      } else if (*pcur == '*') {
         pstar = pcur++;
         sstar = scur;
      } else if (caseless ? ::tolower(*scur) == ::tolower(*pcur) : *scur == *pcur) {
         ++scur;
         ++pcur;
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

h2_inline bool h2_pattern::match(const char* pattern, const char* subject, bool caseless)
{
   return wildcard_match(pattern, subject, caseless) || regex_match(pattern, subject, caseless);
}

static inline long long h2_now()
{
#if defined _WIN32
   return GetTickCount();
#else
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
#endif
}

static inline void h2_sleep(long long milliseconds)
{
#if defined _WIN32
   Sleep(milliseconds);
#else
   ::usleep(milliseconds * 1000);
#endif
}

static inline unsigned h2_termimal_width()
{
#if defined _WIN32
   return 80;
#else
   struct winsize w;
   if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) return 80;
   return w.ws_col < 16 || 256 < w.ws_col ? 80 : w.ws_col;
#endif
}

static inline unsigned h2_page_size()
{
#if defined _WIN32
   SYSTEM_INFO si;
   GetSystemInfo(&si);
   return (unsigned)si.dwPageSize;
#else
   return sysconf(_SC_PAGESIZE);
#endif
}

static inline bool h2_in(const char* x, const char* s[], int n = 0)
{
   for (int i = 0; s[i] && i < (n ? n : 1000); ++i)
      if (!strcmp(s[i], x)) return true;
   return false;
}

static inline const char* comma_if(bool a, const char* t = ", ", const char* f = "") { return a ? t : f; };
static inline const char* ss(const char* a) { return a ? a : ""; };

#define h2_sprintvf(str, fmt, ap)               \
   do {                                         \
      va_list bp;                               \
      va_copy(bp, ap);                          \
      int len = vsnprintf(nullptr, 0, fmt, bp); \
      str = (char*)alloca(len + 1);             \
      va_end(bp);                               \
      len = vsnprintf(str, len + 1, fmt, ap);   \
   } while (0)

#define h2_sprintf(str, fmt)     \
   do {                          \
      va_list ap;                \
      va_start(ap, fmt);         \
      h2_sprintvf(str, fmt, ap); \
      va_end(ap);                \
   } while (0)
