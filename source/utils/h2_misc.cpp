static inline const char* h2_basename(const char* path)
{
   const char* p = strrchr(path, '/');
   if (!p) p = strrchr(path, '\\');
   return p ? p + 1 : path;
}

h2_inline bool h2_pattern::regex_match(const char* pattern, const char* subject, bool caseless)
{
   bool result = false;
   h2_memory::hook(false);
   try {  // c++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error&) {
      result = false;
   }
   h2_memory::hook();
   return result;
}

h2_inline bool h2_pattern::wildcard_match(const char* pattern, const char* subject, bool caseless)
{
#if defined _WIN32 || defined __CYGWIN__
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

static inline const char* get_key(const char* subject, const char* key)
{
   return strcasestr(subject, key);
}
static inline const char* get_eq(const char* start)
{
   for (; *start && ::isspace(*start);) start++;  //strip left space
   return *start == '=' ? start : nullptr;
}

h2_inline const char* h2_extract::has(const char* attributes, const char* key)
{
   return strcasestr(attributes, key);
}

h2_inline bool h2_extract::numeric(const char* attributes, const char* key, double& value)
{
   const char* p_key = get_key(attributes, key);
   if (!p_key) return false;
   const char* p_eq = get_eq(p_key + strlen(key));
   if (!p_eq) return false;
   const char* p_numeric = p_eq + 1;
   for (; *p_numeric && ::isspace(*p_numeric);) p_numeric++;  //strip left space
   value = strtod(p_numeric, nullptr);
   return true;
}

h2_inline bool h2_extract::iport(const char* attributes, const char* key, char* str)
{
   const char* p_key = get_key(attributes, key);
   if (!p_key) return false;
   const char* p_eq = get_eq(p_key + strlen(key));
   if (!p_eq) return false;

   for (const char* p = p_eq + 1; *p; p++) {
      if (::isdigit(*p) || *p == '.' || *p == ':' || *p == '*' || *p == '?') {
         *str++ = *p;
         *str = '\0';
      } else {
         if (!(::isspace(*p) || *p == '\"')) break;
      }
   }
   return true;
}

h2_inline int h2_extract::fill(const char* attributes, const char* key, unsigned char bytes[])
{
   const char* p_key = get_key(attributes, key);
   if (!p_key) return 0;
   const char* p_eq = get_eq(p_key + strlen(key));
   if (!p_eq) return 0;
   const char* p = p_eq + 1;
   for (; *p && ::isspace(*p);) p++;  // strip left space
   if (p[0] == '0' && ::tolower(p[1]) == 'x') {
      return hex_to_bytes(p + 2, bytes);
   } else {
      long long v = strtoll(p, nullptr, 10);
      if (v <= 0xFFU)
         return *((unsigned char*)bytes) = (unsigned char)v, 1;
      else if (v <= 0xFFFFU)
         return *((unsigned short*)bytes) = (unsigned short)v, 2;
      else if (v <= 0xFFFFFFFFU)
         return *((unsigned int*)bytes) = (unsigned int)v, 4;
      else
         return *((unsigned long long*)bytes) = (unsigned long long)v, 8;
   }
}

h2_inline bool h2_pattern::match(const char* pattern, const char* subject, bool caseless)
{
   return wildcard_match(pattern, subject, caseless) || regex_match(pattern, subject, caseless);
}

static inline void h2_sleep(long long milliseconds)
{
#if defined _WIN32
   Sleep(milliseconds);
#else
   ::usleep(milliseconds * 1000);
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
