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

static inline bool not2n(unsigned x)
{
   return x & (x - 1);
}

static inline unsigned mask2n(unsigned x)
{
   x |= (x >> 1);
   x |= (x >> 2);
   x |= (x >> 4);
   x |= (x >> 8);
   x |= (x >> 16);
   return x;
}

static inline const char* strip_left(const char* s)
{
   for (; s && *s && ::isspace(*s);) s++;  // strip left space
   return s;
}

static inline const char* get_keyvalue(const char* attributes, const char* key)
{
   const char* p_key = strcasestr(attributes, key);
   if (!p_key) return nullptr;
   const char* p_eq = strip_left(p_key + strlen(key));
   if (*p_eq != '=') return "";
   return strip_left(p_eq + 1);
}

static inline int hex2byte(char c)
{
   return '0' <= c && c <= '9' ? c - '0' : ('A' <= c && c <= 'F' ? c - 'A' + 10 : ('a' <= c && c <= 'f' ? c - 'a' + 10 : -1));
}

static inline size_t hex2bytes(const char* hex, unsigned char* bytes)
{
   char b;
   size_t i = 0, c = 0;

   for (; ::isxdigit(hex[c]);) ++c;
   if (c % 2 == 1) {
      b = '0';
      i = 1;
      c += 1;
      hex = hex - 1;
   }
   for (; i < c; ++i) {
      if (i % 2 == 1)
         bytes[i / 2] = (unsigned char)((hex2byte(b) << 4) + hex2byte(hex[i]));
      b = hex[i];
   }
   return c / 2;
}

static inline const char* index_th(size_t sequence, size_t shift = 1)
{
   static const char* st[] = {"0th", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th"};
   static char ss[64];

   sequence += shift;
   if (sequence < sizeof(st) / sizeof(st[0])) return st[sequence];
   sprintf(ss, "%dth", (int)sequence);
   return ss;
}

static inline size_t number_strlen(unsigned long long number, int base)
{
   unsigned long long _10000000 = 1;
   for (size_t i = 1;; ++i) {
      _10000000 *= base;
      if (number < _10000000) return i;
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

static inline const char* comma_if(bool a, const char* t = ", ", const char* f = "") { return a ? t : f; }

static inline bool h2_in(const char* a, int n, ...)
{
   va_list ap;
   va_start(ap, n);
   for (int i = 0; i < n; ++i)
      if (!strcmp(va_arg(ap, const char*), a)) return true;
   va_end(ap);
   return false;
}

static inline const char* h2_candidate(const char* a, int n, ...)
{
   int count = 0;
   const char* matches[32];

   va_list ap;
   va_start(ap, n);
   for (int i = 0; i < n; ++i) {
      const char* b = va_arg(ap, const char*);
      if (!strncasecmp(b, a, strlen(a))) matches[count++] = b;
   }
   va_end(ap);

   if (count == 0) return nullptr;
   if (count == 1) return matches[0];

   static char ss[1024];
   sprintf(ss, "ambiguous argument: %s, candidates: ", a);
   for (int i = 0; i < count; ++i)
      sprintf(ss + strlen(ss), "%s%s", comma_if(i, " | "), matches[i]);
   return ss;
}

#define h2_append(Array, Size, a)   \
   for (int i = 0; i < Size; ++i) { \
      if (!Array[i]) {              \
         Array[i] = a;              \
         break;                     \
      }                             \
   }

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
