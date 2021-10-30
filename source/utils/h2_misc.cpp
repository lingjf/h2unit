static inline bool h2_blank(const char* str)
{
   for (; str && *str; str++)
      if (!::isspace(*str)) return false;
   return true;
}

static inline const char* h2_basefile(const char* path)
{
   if (path)
      for (const char* p = path + strlen(path); path <= p; --p)
         if (*p == '/' || *p == '\\')
            return p + 1;
   return path;
}

static inline const char* get_key(const char* subject, const char* key)
{
   return strcasestr(subject, key);
}
static inline const char* get_eq(const char* start)
{
   for (; *start && ::isspace(*start);) start++;  // strip left space
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
   for (; *p_numeric && ::isspace(*p_numeric);) p_numeric++;  // strip left space
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
      return (int)h2_numeric::hex_to_bytes(p + 2, bytes);
   } else {
      unsigned long long v = strtoull(p, nullptr, 10);
      if (v <= 0xFFULL)
         return *((unsigned char*)bytes) = (unsigned char)v, 1;
      else if (v <= 0xFFFFULL)
         return *((unsigned short*)bytes) = (unsigned short)v, 2;
      else if (v <= 0xFFFFFFFFULL)
         return *((unsigned int*)bytes) = (unsigned int)v, 4;
      else
         return *((unsigned long long*)bytes) = (unsigned long long)v, 8;
   }
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
   Sleep((DWORD)milliseconds);
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
