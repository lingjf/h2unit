
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#include "h2unit.hpp"

#include <cctype>    /* tolower, isspace */
#include <cstdarg>   /* va_list */
#include <errno.h>   /* strerror */
#include <exception> /* std::exception */
#include <iostream>  /* std::cout, std::streambuf */
#include <regex>     /* std::regex */
#include <signal.h>  /* sigaction */
#include <time.h>    /* clock */
#include <typeinfo>  /* std::typeid, std::type_info */

#if !defined _MSC_VER
#include <cxxabi.h> /* abi::__cxa_demangle, abi::__cxa_throw */
#endif

#if defined __linux || defined __APPLE__  // -MSVC -Cygwin -MinGW
#include <execinfo.h>                     /* backtrace, backtrace_symbols */
#endif

#if defined __GLIBC__
#include <malloc.h> /* __malloc_hook */
#endif

#if defined _WIN32  // +MinGW
#ifndef NOMINMAX    // MinGW already defined
#define NOMINMAX    // fix std::min/max conflict with windows::min/max
#endif
#include <winsock2.h> /* socket */
#include <windows.h>
#include <ws2tcpip.h> /* getaddrinfo */
#include <io.h>       /* _write */
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define fileno _fileno
#define socklen_t int
#define ssize_t int
#endif

#if defined __CYGWIN__
#include <windows.h>
#endif

#if defined _WIN32 || defined __CYGWIN__  // +MinGW
#include <dbghelp.h>                      /* CaptureStackBackTrace, SymFromAddr */
#include <shlwapi.h>                      /* StrStrIA */
#define strcasestr StrStrIA
#endif

#if defined _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Dbghelp.lib")
#endif

#if defined __linux || defined __APPLE__ || defined __CYGWIN__
#include <arpa/inet.h>  /* inet_addr, inet_pton */
#include <fcntl.h>      /* fcntl */
#include <fnmatch.h>    /* fnmatch */
#include <netdb.h>      /* getaddrinfo, gethostbyname */
#include <sys/time.h>   /* gettimeofday */
#include <sys/ioctl.h>  /* ioctl */
#include <sys/mman.h>   /* mprotect, mmap */
#include <sys/socket.h> /* sockaddr */
#include <sys/types.h>  /* size_t */
#include <syslog.h>     /* syslog, vsyslog */
#include <unistd.h>     /* sysconf */
#endif

#if defined __APPLE__
#include <AvailabilityMacros.h>
#include <malloc/malloc.h> /* malloc_zone_t */
#endif

#if defined _WIN32  // +MinGW
#define LIBC__write ::_write
#else
#define LIBC__write ::write
#endif

#if defined _MSC_VER
#define h2_stdcall __stdcall
#else
#define h2_stdcall
#endif

#if defined _WIN32 || defined __CYGWIN__  // +MinGW
#if defined __H2UNIT_HPP__ || defined H2UNIT_IMPORT_MAIN
int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#else
int main(int argc, const char** argv);
#endif
#else
__attribute__((weak)) int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#endif

namespace h2 {
// source/utils/h2_list.cpp
inline void add_between(h2_list* thus, h2_list* prev, h2_list* next)
{
   next->prev = thus;
   thus->next = next;
   thus->prev = prev;
   prev->next = thus;
}

h2_inline h2_list& h2_list::add_head(h2_list& entry)
{
   add_between(&entry, this, this->next);
   return *this;
}

h2_inline h2_list& h2_list::add_tail(h2_list& entry)
{
   add_between(&entry, this->prev, this);
   return *this;
}

h2_inline h2_list& h2_list::add_before(h2_list& entry)
{
   add_between(&entry, this->prev, this);
   return *this;
}

h2_inline h2_list& h2_list::add_after(h2_list& entry)
{
   add_between(&entry, this, this->next);
   return *this;
}

h2_inline h2_list& h2_list::out()
{
   prev->next = next;
   next->prev = prev;
   next = prev = this;
   return *this;
}

h2_inline int h2_list::count() const
{
   int c = 0;
   for (auto p = next; p != this; p = p->next) ++c;
   return c;
}

h2_inline void h2_list::sort(int (*cmp)(h2_list*, h2_list*))
{
   h2_list sorted, *q;

   while (!empty()) {
      h2_list& p = pop();
      for (q = sorted.next; q != &sorted; q = q->next)
         if (0 < cmp(q, &p))
            break;
      q->add_before(p);
   }
   while (!sorted.empty()) {
      push_back(sorted.pop());
   }
}
// source/utils/h2_pattern.cpp
h2_inline bool h2_pattern::regex_match(const char* pattern, const char* subject, bool caseless)
{
   bool result = false;
#if !defined _WIN32 || !defined NDEBUG  // Windows regex suck under release version and memory check
   h2_memory::hook(false);
   try {  // c++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error&) {
      result = false;
   }
   h2_memory::hook();
#endif
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

h2_inline bool h2_pattern::match(const char* pattern, const char* subject, bool caseless)
{
   return wildcard_match(pattern, subject, caseless) || regex_match(pattern, subject, caseless);
}
// source/utils/h2_diff.cpp

struct h2_fuzzy {
   static size_t levenshtein(const char* s1, const char* s2, size_t n1, size_t n2, bool caseless)
   {
      h2_vector<size_t> col(n2 + 1), prevCol(n2 + 1);
      for (h2_vector<size_t>::size_type i = 0; i < prevCol.size(); i++)
         prevCol[i] = (int)i;
      for (size_t i = 0; i < n1; i++) {
         col[0] = i + 1;
         for (size_t j = 0; j < n2; j++) {
            bool eq = caseless ? ::tolower(s1[i]) == ::tolower(s2[j]) : s1[i] == s2[j];
            col[j + 1] = std::min(std::min(1 + col[j], 1 + prevCol[1 + j]), prevCol[j] + (eq ? 0 : 1));
         }
         col.swap(prevCol);
      }
      return prevCol[n2];
   }

   // 1 absolute match, 0 absolute not match
   static double similarity(const char* s1, const char* s2, bool caseless)
   {
      size_t n1 = strlen(s1), n2 = strlen(s2);
      if (!n1 && !n2) return 1;
      return 1.0 - levenshtein(s1, s2, n1, n2, caseless) / ((n1 + n2) * 0.5);
   }
};

// https://en.wikipedia.org/wiki/Longest_common_subsequence_problem
struct h2_LCS {
   h2_vector<h2_string> s1, s2;
   bool caseless;
   struct matrix {
      unsigned e : 1, p : 1, d : 6, c : 24;
      matrix() : e(0), p(0), d(0), c(0) {}
   };
   h2_vector<h2_vector<matrix>> m;

   h2_LCS(h2_vector<h2_string> s1_, h2_vector<h2_string> s2_, bool caseless_ = false) : s1(s1_), s2(s2_), caseless(caseless_) {}

   void LCS_cacluate()
   {
      for (size_t i = 1; i < s1.size() + 1; i++) {
         for (size_t j = 1; j < s2.size() + 1; j++) {
            if (s1[i - 1].equals(s2[j - 1], caseless)) {
               m[i][j].c = m[i - 1][j - 1].c + 1;
               m[i][j].e = 1;
            } else {
               if (m[i - 1][j].c > m[i][j - 1].c) {
                  m[i][j].c = m[i - 1][j].c;
               } else {
                  m[i][j].c = m[i][j - 1].c;
               }
               m[i][j].e = 0;
            }
         }
      }
   }

   void LCS_traceback(size_t i, size_t j)
   {
      if (i == 0 || j == 0) return;

      if (m[i][j].e || (m[i - 1][j - 1].c >= m[i - 1][j].c && m[i - 1][j - 1].c >= m[i][j - 1].c)) {
         m[i][j].d = 10;  // 10:30 upper left
      } else if (m[i - 1][j].c > m[i][j - 1].c) {
         m[i][j].d = 12;  // 12:00 upper
      } else {
         m[i][j].d = 9;  // 9:00 left
      }

      if (m[i][j].d == 10) {  // 10:30
         m[i][j].p = m[i][j].e;
         LCS_traceback(i - 1, j - 1); // i--, j--
      } else if (m[i][j].d == 12) {  // 12:00 upper i--
         LCS_traceback(i - 1, j);
      } else {  // 9:00 left j--
         LCS_traceback(i, j - 1);
      }
   }

   std::pair<h2_vector<unsigned char>, h2_vector<unsigned char>> lcs()
   {
      for (size_t i = 0; i < s1.size() + 1; i++) m.push_back(h2_vector<matrix>(s2.size() + 1));
      LCS_cacluate();
      LCS_traceback(s1.size(), s2.size());

      h2_vector<unsigned char> l1(s1.size()), l2(s2.size());
      for (size_t i = 1; i < s1.size() + 1; i++) {
         l1[i - 1] = 0;
         for (size_t j = 1; j < s2.size() + 1; j++)
            if (m[i][j].p) l1[i - 1] = 1;
      }
      for (size_t j = 1; j < s2.size() + 1; j++) {
         l2[j - 1] = 0;
         for (size_t i = 1; i < s1.size() + 1; i++)
            if (m[i][j].p) l2[j - 1] = 1;
      }
      return {l1, l2};
   }
};
// source/utils/h2_misc.cpp
static inline bool h2_blank(const char* str)
{
   for (; str && *str; ++str)
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

static inline const char* strip_left(const char* left, const char* right = nullptr)  // [left, right)
{
   while ((right ? left < right : true) && *left && ::isspace(*left)) ++left;
   return left;
}

static inline const char* strip_right(const char* left, const char* right = nullptr)  // [left, right)
{
   if (!right) right = left + strlen(left);
   for (--right; left <= right && (!*right || ::isspace(*right));) --right;
   return right + 1;
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

static inline size_t number_strlen(size_t number, int base)
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

#define h2_sprintvf(str, fmt, ap)            \
   va_list bp;                               \
   va_copy(bp, ap);                          \
   int len = vsnprintf(nullptr, 0, fmt, bp); \
   str = (char*)alloca(len + 1);             \
   va_end(bp);                               \
   len = vsnprintf(str, len + 1, fmt, ap);

#define h2_sprintf(str, fmt)  \
   va_list ap;                \
   va_start(ap, fmt);         \
   h2_sprintvf(str, fmt, ap); \
   va_end(ap);
// source/utils/h2_libc.cpp
struct h2_libc_malloc {
   h2_singleton(h2_libc_malloc);

   struct buddy {
      unsigned long long size;
      h2_list x;
      buddy(const unsigned long long size_) : size(size_) {}
      bool join_right(buddy* b) { return ((char*)this) + size == (char*)b; }
      bool join_left(buddy* b) { return ((char*)b) + b->size == (char*)this; }
   };

   struct block {
      unsigned long long bytes;
      block* next = nullptr;
      h2_list buddies;

      block(const unsigned long long bytes_) : bytes(bytes_)
      {
         buddy* b = new ((char*)this + sizeof(block)) buddy(bytes - sizeof(block));
         buddies.add_tail(b->x);
      }

      buddy* malloc(const unsigned long long size)
      {
         h2_list_for_each_entry (p, buddies, buddy, x) {
            if (size + sizeof(p->size) <= p->size) {
               unsigned long long left = p->size - (size + sizeof(p->size));
               if (sizeof(buddy) + 64 <= left) {  // avoid smash buddy for performance
                  buddy* b = new ((char*)p + left) buddy(size + sizeof(b->size));
                  p->size = left;
                  return b;
               } else {
                  p->x.out();
                  return p;
               }
            }
         }
         return nullptr;
      }

      bool free(buddy* b)
      {
         if ((char*)b < (char*)this && (char*)this + bytes <= (char*)b) return false;
         h2_list_for_each_entry (p, buddies, buddy, x) {
            if (p->join_right(b)) {
               p->size += b->size;
               return true;
            }
            if (p->join_left(b)) {
               p->x.add_before(b->x);
               b->size += p->size;
               p->x.out();
               return true;
            }
            if ((char*)b + b->size < (char*)p) {
               p->x.add_before(b->x);
               return true;
            }
         }
         buddies.add_tail(b->x);
         return true;
      }
   };

   block* next = nullptr;

   void batch(const unsigned long long size)
   {
      size_t brk_size = 4 * 1024 * 1024;
      size_t brk_count = (size_t)::ceil(size / (double)brk_size);

#if defined _WIN32
      PVOID ptr = VirtualAlloc(NULL, brk_count * brk_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if (ptr == NULL) ::printf("VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      void* ptr = ::mmap(nullptr, brk_count * brk_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (ptr == MAP_FAILED) ::printf("mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      block* p = new (ptr) block(brk_count * brk_size);
      p->next = next;
      next = p;
   }

   buddy* alloc(const unsigned long long size)
   {
      for (block* p = next; p; p = p->next) {
         buddy* b = p->malloc(size);
         if (b) return b;
      }
      return nullptr;
   }

   void* malloc(const size_t size)
   {
      unsigned long long _size = (size + 7) / 8 * 8;
      buddy* b = alloc(_size);
      if (!b) {
         batch(_size);
         b = alloc(_size);
      }

      return b ? (void*)&b->x : nullptr;
   }

   void free(void* ptr)
   {
      if (ptr) {
         buddy* b = (buddy*)((char*)ptr - sizeof(b->size));
         for (block* p = next; p; p = p->next)
            if (p->free(b)) return;
      }
   }
};

h2_inline void* h2_libc::malloc(size_t size)
{
   if (!O.memory_check) return ::malloc(size);
   return h2_libc_malloc::I().malloc(size + 10);
}

h2_inline void h2_libc::free(void* ptr)
{
   if (!O.memory_check) return ::free(ptr);
   if (ptr) h2_libc_malloc::I().free(ptr);
}
// source/utils/h2_string.cpp
static inline size_t utf8len(const char* s)
{
   if (0xf0 == (0xf8 & *(const unsigned char*)s)) return 4;  // 4-byte utf8 code point (began with 0b11110xxx)
   if (0xe0 == (0xf0 & *(const unsigned char*)s)) return 3;  // 3-byte utf8 code point (began with 0b1110xxxx)
   if (0xc0 == (0xe0 & *(const unsigned char*)s)) return 2;  // 2-byte utf8 code point (began with 0b110xxxxx)
   return 1;                                                 // 1-byte ascii (began with 0b0xxxxxxx)
}

h2_inline h2_string& h2_string::sprintf(const char* fmt, ...)
{
   char* alloca_str;
   h2_sprintf(alloca_str, fmt);
   append(alloca_str);
   return *this;
}

h2_inline h2_string& h2_string::replace_all(const char* from, const char* to)
{
   size_t start_pos = 0, from_length = strlen(from), to_length = strlen(to);
   while ((start_pos = find(from, start_pos)) != h2_string::npos) {
      replace(start_pos, from_length, to);
      start_pos += to_length;  // where 'to' is a substring of 'from'
   }
   return *this;
}

h2_inline size_t h2_string::width(size_t fat) const  // wcwidth()/wcswidth()
{
   size_t w = 0, n = 0;
   for (const char* p = c_str(); *p != '\0'; p += n) {
      n = utf8len(p);
      w += (n == 1 ? 1 : fat);
   }
   return w;
}

h2_inline bool h2_string::equals(const h2_string& fulstr, bool caseless) const
{
   if (!caseless) return *this == fulstr;
   return tolower() == fulstr.tolower();
}

h2_inline bool h2_string::contains(const h2_string& substr, bool caseless) const
{
   if (!caseless) return find(substr) != h2_string::npos;
   return tolower().find(substr.tolower()) != h2_string::npos;
}

h2_inline bool h2_string::startswith(const h2_string& prefix, bool caseless) const
{
   if (size() < prefix.size()) return false;
   if (!caseless) return find(prefix) == 0;
   return tolower().find(prefix.tolower()) == 0;
}

h2_inline bool h2_string::endswith(const h2_string& suffix, bool caseless) const
{
   if (size() < suffix.size()) return false;
   if (!caseless) return rfind(suffix) == size() - suffix.size();
   return tolower().rfind(suffix.tolower()) == size() - suffix.size();
}

h2_inline bool h2_string::enclosed(char left, char right) const
{
   if (right == '\0') right = left;
   return front() == left && back() == right;
}

h2_inline h2_string h2_string::unenclose(char left, char right) const
{
   if (!enclosed(left, right)) return *this;
   return h2_string(size() - 2, c_str() + 1);
}

h2_inline h2_string h2_string::escape(bool utf8) const
{
   h2_string s;
   for (auto& c : *this) {
      switch (c) {
         case '\n': s.append(utf8 ? "␍" : "\\n"); break;
         case '\r': s.append(utf8 ? "␊" : "\\r"); break;
         case '\t': s.append(utf8 ? "␉" : "\\t"); break;
         case '\0': s.append(utf8 ? "␀" : "\0"); break;
         default: s.push_back(c); break;
      }
   }
   return s;
}

h2_inline h2_string h2_string::unescape() const
{
   h2_string s = *this;
   s.replace_all("\\b", "\b");
   s.replace_all("\\f", "\f");
   s.replace_all("\\n", "\n");
   s.replace_all("\\r", "\r");
   s.replace_all("\\t", "\t");
   s.replace_all("\\\"", "\"");
   s.replace_all("\\\\", "\\");
   // todo: escape \u12ab
   return s;
}

h2_inline h2_string h2_string::trim() const
{
   const auto a = find_first_not_of("\t\n ");
   if (a == h2_string::npos) return "";
   const auto b = find_last_not_of("\t\n ");
   return substr(a, b - a + 1).c_str();
}

h2_inline h2_string h2_string::squash(bool quote) const
{
   h2_string s;
   bool quote1 = false, quote2 = false;
   int spaces = 0;
   for (char c : trim()) {
      if (c == '\t' || c == '\n' || c == ' ') c = ' ';
      if (c != ' ') spaces = 0;
      if (!quote && c == '\'') quote1 = !quote1;
      if (!quote && c == '\"') quote2 = !quote2;
      if (!quote1 && !quote2 && c == ' ' && spaces++) continue;
      s.push_back(c);
   }
   return s;
}

h2_inline h2_string h2_string::tolower() const
{
   h2_string s;
   for (auto& c : *this) s.push_back(::tolower(c));
   return s;
}

h2_inline h2_string h2_string::center(size_t width) const
{
   if (width <= size()) return *this;
   size_t left = (width - size()) / 2, right = width - left - size();
   h2_string s;
   s.append(left, ' ');
   s.append(*this);
   s.append(right, ' ');
   return s;
}

h2_inline h2_vector<h2_string> h2_string::disperse() const
{
   h2_vector<h2_string> chars;
   for (const char* p = c_str(); *p != '\0';) {
      auto n = utf8len(p);
      chars.push_back(h2_string(n, p));
      p += n;
   }
   return chars;
}
// source/utils/h2_color.cpp
struct h2_color {
   h2_singleton(h2_color);
   char current[8][32]{{'\0'}};

   void clear_style()
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i) current[i][0] = '\0';
   }
   void push_style(const char* style, size_t length)
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] == '\0') {
            strncpy(current[i], style, length);
            current[i][length] = '\0';
            break;
         }
   }
   void pop_style(const char* style, size_t length)
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (!strncmp(current[i], style, length) && strlen(current[i]) == length)
            current[i][0] = '\0';
   }
   void change()
   {
      char a[256], *p = a;
      p += sprintf(p, "\033[%d;", style2value("reset"));
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            p += sprintf(p, "%d;", style2value(current[i]));
      *(p - 1) = 'm';
      LIBC__write(-21371647, a, (size_t)(p - a));
   }
   void parse(const char* style)
   {
      const char* p = style + 2;
      char s = '+';
      if (*p == '+' || *p == '-') s = *p++;
      for (;;) {
         size_t l = strcspn(p, ",}");
         s == '-' ? pop_style(p, l) : push_style(p, l);
         if (!strncmp("reset", p, l)) clear_style();
         if (*(p + l) == '}' || *(p + l) == '\0') break;
         p += l + 1;
      }
   }
   void print(const char* str)
   {  /* Windows PowerShell works, but CMD not, refer to v5.11 SetConsoleTextAttribute */
      if (isctrl(str)) {
         if (h2_option::I().colorful) {
            I().parse(str);
            I().change();
         }
      } else {
         LIBC__write(-21371647, str, strlen(str));
      }
   }
   int style2value(const char* style)  // https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_sequences
   {
      if (!strcmp(style, "reset")) return 0;
      if (!strcmp(style, "bold")) return 1;
      if (!strcmp(style, "italics")) return 3;
      if (!strcmp(style, "underline")) return 4;
      if (!strcmp(style, "inverse")) return 7;
      if (!strcmp(style, "strikethrough")) return 9;
      if (!strcmp(style, "black")) return 30;
      if (!strcmp(style, "red")) return 31;
      if (!strcmp(style, "green")) return 32;
      if (!strcmp(style, "yellow")) return 33;
      if (!strcmp(style, "blue")) return 34;
      if (!strcmp(style, "purple")) return 35;
      if (!strcmp(style, "cyan")) return 36;
      if (!strcmp(style, "gray")) return 37;
      if (!strcmp(style, "default")) return 39;
      if (!strcmp(style, "dark gray")) return 90;
      if (!strcmp(style, "light red")) return 91;
      if (!strcmp(style, "light green")) return 92;
      if (!strcmp(style, "light yellow")) return 93;
      if (!strcmp(style, "light blue")) return 94;
      if (!strcmp(style, "light purple")) return 95;
      if (!strcmp(style, "light cyan")) return 96;
      if (!strcmp(style, "white")) return 97;
      if (!strcmp(style, "bg_black")) return 40;
      if (!strcmp(style, "bg_red")) return 41;
      if (!strcmp(style, "bg_green")) return 42;
      if (!strcmp(style, "bg_yellow")) return 43;
      if (!strcmp(style, "bg_blue")) return 44;
      if (!strcmp(style, "bg_purple")) return 45;
      if (!strcmp(style, "bg_cyan")) return 46;
      if (!strcmp(style, "bg_white")) return 47;
      return 0;
   }

   static bool isctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
// source/utils/h2_line.cpp
h2_inline size_t h2_line::width(bool ignore_indent) const
{
   size_t w = 0;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         if (!ignore_indent || !h2_blank(word.c_str()))
            w += word.width();
   return w;
}

h2_inline h2_line& h2_line::indent(size_t n, const char c)
{
   if (n) insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::padding(size_t n, const char c)
{
   if (n) push_back(h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_line& h2_line::operator+=(const h2_line& line)
{
   insert(end(), line.begin(), line.end());
   return *this;
}

h2_inline h2_line& h2_line::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
   return *this;
}

h2_inline bool h2_line::enclosed(char left, char right) const
{
   if (right == '\0') right = left;
   bool f = false, ff = false, b = false;
   for (auto& word : *this) {
      if (!h2_color::isctrl(word.c_str())) {
         if (!ff) f = word.front() == left;
         ff = true;
         b = word.back() == right;
      }
   }
   return f && b;
}

h2_inline bool h2_line::has(const char* word) const
{
   for (auto& t : *this)
      if (t == word) return true;
   return false;
}

h2_inline h2_line h2_line::gray_quote() const
{
   if (!enclosed('\"', '\"') && !enclosed('\'', '\'')) return *this;

   h2_line line;
   size_t i = 0, w = width();

   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         line.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i == 0) {
            h.append(c.c_str());
         } else if (i == w - 1) {
            t.append(c.c_str());
         } else {
            m.append(c.c_str());
         }
         i += c.width();
      }
      if (h.size()) line += delta(h, "dark gray");
      if (m.size()) line.push_back(m);
      if (t.size()) line += delta(t, "dark gray");
   }

   return line;
}

h2_inline h2_line h2_line::abbreviate(size_t width, size_t tail) const
{
   h2_line line1, line2;
   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str()))
         line1.push_back(word);
      else
         line1.push_back(word.escape());
   }

   size_t i = 0, line1_width = line1.width();
   if (line1_width <= width) return line1;

   for (auto& word : line1) {
      if (h2_color::isctrl(word.c_str())) {
         line2.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i < width - 3 - tail) {
            h.append(c.c_str());
         } else if (i == width - 3 - tail) {
            m = "...";
         } else if (line1_width - tail <= i) {
            t.append(c.c_str());
         }
         i += c.width();
      }
      if (h.size()) line2.push_back(h);
      if (m.size()) line2 += delta(m, "dark gray");
      if (t.size()) line2.push_back(t);
   }

   return line2;
}

h2_inline h2_string h2_line::string() const
{
   h2_string s;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         s += word;
   return s;
}

h2_inline void h2_line::samesizify(h2_line& a, h2_line& b)
{
   size_t a_w = a.width(), b_w = b.width();
   a.padding(std::max(a_w, b_w) - a_w);
   b.padding(std::max(a_w, b_w) - b_w);
}

///// h2_lines
h2_inline h2_lines& h2_lines::operator+=(const h2_lines& lines)
{
   insert(end(), lines.begin(), lines.end());
   return *this;
}

h2_inline size_t h2_lines::width() const
{
   size_t m = 0;
   for (auto& line : *this)
      m = std::max(m, line.width());
   return m;
}

h2_inline bool h2_lines::foldable(size_t width) const
{
   size_t sum = 0;
   for (auto& line : *this)
      for (auto& word : line)
         if (!h2_blank(word.c_str()) && !h2_color::isctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.size();
   return sum < width;
}

h2_inline h2_line h2_lines::folds() const
{
   h2_line folded_line;
   for (auto& line : *this)
      for (auto& word : line)
         if (!h2_blank(word.c_str()))  // drop indent
            folded_line.push_back(word);
   return folded_line;
}

h2_inline h2_string h2_lines::string() const
{
   h2_string s;
   for (auto& line : *this)
      for (auto& word : line)
         if (!h2_blank(word.c_str()) && !h2_color::isctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_lines::sequence(size_t indent, size_t start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = delta(h2_string("%d. ", (int)(i + start)), "dark gray") + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_lines::samesizify(h2_lines& a, h2_lines& b)
{
   auto size = std::max(a.size(), b.size());
   for (auto i = a.size(); i < size; ++i) a.push_back(h2_line());
   for (auto i = b.size(); i < size; ++i) b.push_back(h2_line());
}
// source/render/h2_console.cpp
struct h2_console {
   static size_t width()
   {
      static size_t s_width = 0;
      if (s_width == 0) {
         s_width = 120;
#if defined _WIN32
         CONSOLE_SCREEN_BUFFER_INFO csbi;
         GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
         auto columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
         if (16 < columns) s_width = columns;
#else
         struct winsize w;
         if (-1 != ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
            if (16 < w.ws_col) s_width = w.ws_col;
#endif
      }
      return s_width;
   }

   static void prints(const char* style, const char* format, ...)
   {
      if (style && strlen(style)) {
         char t[128];
         sprintf(t, "\033{%s}", style);
         h2_color::I().print(t);
      }

      char* alloca_str;
      h2_sprintf(alloca_str, format);
      h2_color::I().print(alloca_str);

      if (style && strlen(style)) h2_color::I().print("\033{reset}");
   }

   static void printl(const h2_line& line, bool cr = true)
   {
      for (auto& word : line) h2_color::I().print(word.c_str());
      if (cr) h2_color::I().print("\n");
   }

   static void printl(const h2_lines& lines, bool cr = true)
   {
      for (auto& line : lines) printl(line, cr);
   }
};
// source/symbol/h2_nm.cpp
#if !defined _MSC_VER
static inline void __nm(h2_list& symbols, bool demangle)
{
   h2_memory::hook(false);
   char cmd[256], line[2048], addr[128], type, symbol[2048];
   sprintf(cmd, "nm --defined-only %s -n %s", demangle ? "-f bsd --demangle" : "", O.path);
   FILE* f = ::popen(cmd, "r");
   if (f) {
      while (::fgets(line, sizeof(line) - 1, f)) {
         if (3 != sscanf(line, "%s %c %[^\n]", addr, &type, symbol)) continue;
         if (strchr("bBcCdDiIuU", type)) continue;  // reject bBcCdDiIuU, accept tTwWsSvV, sS for vtable
         symbols.push_back((new h2_symbol(symbol, strtoull(addr, nullptr, 16)))->x);
      }
      ::pclose(f);
   }
   h2_memory::hook();
}
static inline bool __leading_underscore(h2_list& symbols)
{
   bool _main = false;
   h2_list_for_each_entry (p, symbols, h2_symbol, x) {
      if (!strcmp("main", p->symbol)) return false;
      if (!strcmp("_main", p->symbol)) _main = true;
   }
   return _main;
}
static inline void nm(bool demangle, h2_list& symbols, bool& leading_underscore)
{
   __nm(symbols, demangle);
   static h2_once one;
   if (one) leading_underscore = __leading_underscore(symbols);
}
#endif

h2_inline char* h2_symbol::name()
{
   return h2_nm::I().leading_underscore && symbol[0] == '_' ? symbol + 1 : symbol;  // remove leading underscore '_' in MacOS
}

static inline bool strncmp_reverse(const char* a, const char* ae, const char* b, const char* be, size_t n)  // [a, ae) [b, be)
{
   if (ae < a + n || be < b + n) return false;
   return !strncmp(ae - n, be - n, n);
}

h2_inline int h2_nm::get_by_name(const char* name, h2_symbol* res[], int n)
{
   if (!name) return 0;
   size_t len = strlen(name);
   if (len == 0) return 0;
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(GetCurrentProcess(), name, symbol)) return 0;
   static h2_symbol s_symbol("", 0);
   s_symbol.addr = (unsigned long long)symbol->Address;
   res[0] = &s_symbol;
   return 1;
#else
   if (I().demangle_symbols.empty()) nm(true, I().demangle_symbols, I().leading_underscore);

   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      if (!strcmp(p->name(), name)) {
         res[0] = p;
         return 1;
      }
   }
   int count = 0;
   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      char* parentheses = strchr(p->name(), '(');
      if (!parentheses) continue;
      if (!strncmp_reverse(p->name(), parentheses, name, name + len, len)) continue;  // compare function name
      char* func = parentheses - len;
      if (p->name() < func && (func[-1] != ':' || func[-2] != ':')) continue;  // strip namespace
      if (count < n) res[count++] = p;
   }
   return count;
#endif
}

h2_inline h2_symbol* h2_nm::get_by_addr(unsigned long long addr)
{
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromAddr(GetCurrentProcess(), (DWORD64)(addr), 0, symbol)) return nullptr;
   static h2_symbol s_symbol("", 0);
   strcpy(s_symbol.symbol, symbol->Name);
   s_symbol.addr = (unsigned long long)symbol->Address;
   return &s_symbol;
#else
   return nullptr;
#endif
}

h2_inline unsigned long long h2_nm::get_mangle(const char* name)
{
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(GetCurrentProcess(), name, symbol)) return 0;
   return (unsigned long long)symbol->Address;
#else
   if (!name || strlen(name) == 0) return 0;
   if (I().mangle_symbols.empty()) nm(false, I().mangle_symbols, I().leading_underscore);

   h2_list_for_each_entry (p, I().mangle_symbols, h2_symbol, x)
      if (!strcmp(p->name(), name)) return p->addr;

   return 0;
#endif
}
// source/symbol/h2_load.cpp
static inline long long get_load_text_offset()
{
   h2_symbol* s[16];
   if (h2_nm::get_by_name("main", s, 16) == 0) return 0;
   return (long long)&main - (long long)s[0]->addr;
}

static inline long long get_load_vtable_offset()
{
   char vtable_symbol[256];
   h2_vtable_test t;
   long long absolute_vtable = (long long)*(void***)&t;
   sprintf(vtable_symbol, "_ZTV%s", typeid(h2_vtable_test).name());  // mangled for "vtable for h2::h2_vtable_test"
   long long relative_vtable = (long long)h2_nm::get_mangle(vtable_symbol);
   if (relative_vtable == 0)
      h2_console::prints("yellow", "\nDon't find vtable for h2::h2_vtable_test %s\n", vtable_symbol);
   return absolute_vtable - relative_vtable;
}

h2_inline void* h2_load::vtable_to_ptr(unsigned long long addr)
{
   if (I().vtable_offset == -1) I().vtable_offset = get_load_vtable_offset();
   return (void*)(addr + I().vtable_offset);
}

h2_inline void* h2_load::addr_to_ptr(unsigned long long addr)
{
#if defined _MSC_VER || defined __CYGWIN__
   return (void*)addr;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (void*)(addr + I().text_offset);
#endif
}

h2_inline unsigned long long h2_load::ptr_to_addr(void* ptr)
{
#if defined _MSC_VER || defined __CYGWIN__
   return (unsigned long long)ptr;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (unsigned long long)ptr - I().text_offset;
#endif
}
// source/symbol/h2_backtrace.cpp
#if !defined _WIN32  // -MinGW
static inline char* addr2line(unsigned long long addr)
{
   static char buf[1024];
   char cmd[256], *ret = nullptr;
#if defined __APPLE__
   sprintf(cmd, "atos -o %s 0x%llx", O.path, addr);
#else
   sprintf(cmd, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#endif
#if !defined _MSC_VER
   FILE* f = ::popen(cmd, "r");
   if (f) {
      ret = ::fgets(buf, sizeof(buf), f);
      ::pclose(f);
   }
   if (!ret) return nullptr;
   if (O.os == 'm' ? !memcmp(buf, "0x", 2) : !!strstr(buf, "??:")) return nullptr;
   *(char*)strip_right(buf) = '\0';
#endif
   return buf;
}
#endif

#if !(defined _MSC_VER || defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__)
static inline bool backtrace_extract(const char* line, char* mangle_name, unsigned long long* displacement = nullptr)
{
   unsigned long long _t;
#if defined __APPLE__
   // MAC: `3   a.out  0x000000010e777f3d _ZN2h24unit6mallocEm + 45
   if (2 == ::sscanf(line, "%*s%*s%*s%s + %llu", mangle_name, displacement ? displacement : &_t)) return true;
#else
   static unsigned long long v1 = 0, v2 = 0, once = 0;
   // Linux: `./a.out(_ZN2h24unit7executeEv+0x131)[0x55aa6bb840ef]
   if (2 == ::sscanf(line, "%*[^(]%*[^_a-zA-Z]%1023[^)+]+0x%llx", mangle_name, displacement ? displacement : &_t)) return (bool)++v2;
   // Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   mangle_name[0] = '\0';
   if (1 == ::sscanf(line, "%*[^(]%*[^+]+0x%llx", displacement ? displacement : &_t)) return (bool)++v1;

   if (!v2 && !once++) h2_console::prints("yellow", "\nAdd -rdynamic to linker options\n");
#endif
   return false;
}
#endif

h2_inline bool h2_backtrace::operator==(const h2_backtrace& bt) const
{
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (frames[i] != bt.frames[i])
         return false;
   return true;
}

h2_inline h2_backtrace& h2_backtrace::dump(int shift_)
{
   static h2_backtrace s;
   s.shift = shift_;
#if defined _MSC_VER || defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__
   s.count = CaptureStackBackTrace(0, sizeof(s.frames) / sizeof(s.frames[0]), s.frames, NULL);
#else
   h2_memory::hook(false);
   s.count = ::backtrace(s.frames, sizeof(s.frames) / sizeof(s.frames[0]));
   h2_memory::hook();
#endif
   return s;
}

h2_inline bool h2_backtrace::in(void* fps[]) const
{
   bool ret = false;
#if defined _MSC_VER
   for (int i = shift; !ret && i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      if (SymFromAddr(GetCurrentProcess(), (DWORD64)(frames[i]), 0, symbol))
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)symbol->Address == (unsigned long long)fps[j])
               ret = true;
   }
#elif defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__
   for (int i = shift; !ret && i < count; ++i)
      for (int j = 0; !ret && fps[j]; ++j)
         if ((unsigned long long)fps[j] <= (unsigned long long)frames[i] && (unsigned long long)frames[i] < 100 + (unsigned long long)fps[j])
            ret = true;
#else
   h2_memory::hook(false);
   char** symbols = backtrace_symbols(frames, count);
   for (int i = shift; !ret && i < count; ++i) {
      char _t[1024];
      unsigned long long displacement = 0;
      if (backtrace_extract(symbols[i], _t, &displacement))
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)frames[i] - displacement == (unsigned long long)fps[j])
               ret = true;
   }
   free(symbols);
   h2_memory::hook();
#endif
   return ret;
}

h2_inline void h2_backtrace::print(h2_vector<h2_string>& stacks) const
{
#if defined _WIN32  // +MinGW
   for (int i = shift; i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      h2_string frame;
      if (SymFromAddr(GetCurrentProcess(), (DWORD64)(frames[i]), 0, symbol))
         frame.sprintf("%s %p+%lld ", symbol->Name, symbol->Address, (DWORD64)(frames[i]) - (DWORD64)symbol->Address);
      DWORD dwDisplacement;
      IMAGEHLP_LINE64 fileline;
      SymSetOptions(SYMOPT_LOAD_LINES);
      fileline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
      if (SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)(frames[i]), &dwDisplacement, &fileline))
         frame.sprintf("%s:%d", fileline.FileName, fileline.LineNumber);
      stacks.push_back(frame);
      if (!strcmp("main", symbol->Name)) break;
   }
#elif defined __CYGWIN__
   for (int i = shift; i < count; ++i) {
      char* p = nullptr;
      p = addr2line(h2_load::ptr_to_addr(frames[i]));
      if (p) stacks.push_back(p);
   }
#else
   h2_memory::hook(false);
   char** symbols = backtrace_symbols(frames, count);
   h2_memory::hook();
   for (int i = shift; i < count; ++i) {
      char *p = nullptr, mangle_name[1024] = "", demangle_name[1024] = "";
      backtrace_extract(symbols[i], mangle_name);
      if (O.verbose >= VerboseDetail || O.os != 'm') p = addr2line(h2_load::ptr_to_addr(frames[i])); /* atos is slow */
      if (!p) p = h2_cxa::demangle(mangle_name, demangle_name);
      if (!p || !strlen(p)) p = symbols[i];
      stacks.push_back(p);
      if (!strcmp("main", mangle_name) || !strcmp("__libc_start_main", mangle_name)) break;
   }
   h2_memory::hook(false);
   free(symbols);
   h2_memory::hook();
#endif
}

h2_inline void h2_backtrace::print(size_t pad) const
{
   h2_vector<h2_string> stacks;
   print(stacks);
   h2_lines lines;
   for (auto& c : stacks) lines.push_back(c.startswith("h2::") || c.contains(": h2::") ? color(c, "dark gray") : h2_line(c));
   lines.sequence(pad);
   h2_console::printl(lines);
}
// source/symbol/h2_cxa.cpp
h2_inline char* h2_cxa::demangle(const char* mangle_name, char* demangle_name, size_t length)
{
#if defined _MSC_VER
   return (char*)mangle_name;
#else
   int status = -1;
   if (strlen(mangle_name)) abi::__cxa_demangle(mangle_name, demangle_name, &length, &status);
   return status == 0 ? demangle_name : (char*)mangle_name;
#endif
}

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64

static inline unsigned char* follow_JMP32REL(unsigned char* target)
{
   int relative_offset;
   memcpy(reinterpret_cast<void*>(&relative_offset), reinterpret_cast<void*>(target + 1), 4);
   return target + 5 + relative_offset;
}
static inline unsigned char* follow_JMP8REL(unsigned char* target)
{
   signed char relative_offset;
   memcpy(reinterpret_cast<void*>(&relative_offset), reinterpret_cast<void*>(target + 1), 1);
   return target + 2 + relative_offset;
}
static inline unsigned char* follow_JMP32ABS(unsigned char* target)
{
   void** new_target_p;
   if (sizeof(void*) == 8) {  // In 64-bit mode JMPs are RIP-relative, not absolute
      int target_offset;
      memcpy(reinterpret_cast<void*>(&target_offset), reinterpret_cast<void*>(target + 2), 4);
      new_target_p = reinterpret_cast<void**>(target + target_offset + 6);
   } else {
      memcpy(&new_target_p, reinterpret_cast<void*>(target + 2), 4);
   }
   return reinterpret_cast<unsigned char*>(*new_target_p);
}

h2_inline void* h2_cxa::follow_jmp(void* fp, int n)
{
   unsigned char* p = (unsigned char*)fp;
   while (n--) {
      if (p[0] == 0xE9) {  // ASM_JMP32REL
         p = follow_JMP32REL(p);
      } else if (p[0] == 0xEB) {  //ASM_JMP8REL, Visual Studio 7.1 implements new[] as an 8 bit jump to new
         p = follow_JMP8REL(p);
      } else if (p[0] == 0xFF && p[1] == 0x25) {  // ASM_JMP32ABS_0 ASM_JMP32ABS_1
         p = follow_JMP32ABS(p);
      } else if (sizeof(void*) == 8 && p[0] == 0x48 && p[1] == 0xFF && p[2] == 0x25) {  // in Visual Studio 2012 we're seeing jump like that: rex.W jmpq *0x11d019(%rip)
         p = follow_JMP32ABS(p + 1);
      } else {
         break;
      }
   }
   return (void*)p;
}

#elif defined __arm64__ || defined __aarch64__

static inline long long sign_extend(unsigned long long value, unsigned int bits)
{
   const unsigned int left = 64 - bits;
   const long long m1 = -1;
   const long long wide = (long long)(value << left);
   const long long sign = (wide < 0) ? (m1 << left) : 0;
   return value | sign;
}

static inline unsigned long fetch_opcode(void* fp, int i = 0)
{
   return *(unsigned long*)(((unsigned char*)fp) + i * 4);
}

h2_inline void* h2_cxa::follow_jmp(void* fp, int n)
{
   while (n--) {
      // (gdb) disassemble /r printf
      // Dump of assembler code for function printf@plt:
      //    0x0000000000c1f8a0 <+0>:     90 66 00 b0     adrp    x16, 0x18f0000 <div@got.plt>
      //    0x0000000000c1f8a4 <+4>:     11 ee 41 f9     ldr     x17, [x16, #984]
      //    0x0000000000c1f8a8 <+8>:     10 62 0f 91     add     x16, x16, #0x3d8
      //    0x0000000000c1f8ac <+12>:    20 02 1f d6     br      x17
      if ((fetch_opcode(fp, 0) & 0x9f00001f) == (0x90000000 | 16)) {                                                     // C6.2.10 ADRP
         if ((fetch_opcode(fp, 1) & 0xffe003ff) == (0xf9400000 | 16 << 5 | 17)) {                                        // C6.2.101 LDR (immediate)
            if ((fetch_opcode(fp, 2) & 0xfffffc1f) == 0xd61f0000 || (fetch_opcode(fp, 3) & 0xfffffc1f) == 0xd61f0000) {  // BR X17 0xd61f0000 | 17 << 5
               unsigned long long low2 = (fetch_opcode(fp, 0) >> 29) & 3;
               unsigned long long high19 = (fetch_opcode(fp, 0) >> 5) & ~(~0ULL << 19);
               long long page = sign_extend((high19 << 2) | low2, 21) << 12;
               unsigned long long offset = ((fetch_opcode(fp, 1) >> 10) & ~(~0ULL << 12)) << 3;
               unsigned char* target_p = (unsigned char*)((unsigned long long)fp & 0xfffffffffffff000ULL) + page + offset;
               fp = (void*)*(unsigned char**)target_p;
            }
         }
      } else if ((fetch_opcode(fp, 0) & 0xf8000000) == 0x58000000) {  // LDR X17,[PC+n]
         if ((fetch_opcode(fp, 1) & 0xfffffc1f) == 0xd61f0000) {      // BR X17
            unsigned long n = (fetch_opcode(fp, 0) & 0x07ffffe0) >> 3;
            fp = (void*)*(unsigned long long*)(((unsigned long long)fp) + n);
         }
      }
   }
   return fp;
}

#endif
// source/json/h2_tinyexpr.cpp
// TINYEXPR - Tiny recursive descent parser and evaluation engine in C; Copyright (c) 2015-2018 Lewis Van Winkle
struct tinyexpr {
   static bool evaluate(const char* expression, double& value)
   {
      syntax_tree* st = compile(expression);
      if (!st) return false;
      value = st->evaluate();
      delete st;
      return true;
   }

   struct function_spec {
      int parameters;
      const char* fn;
      const void* fp;
   };

   struct syntax_tree : h2_libc {
      double value = 0;
      const function_spec* function = nullptr;
      syntax_tree* parameters[4]{nullptr};
      syntax_tree(double value_) : value(value_) {}
      syntax_tree(const function_spec* function_, syntax_tree* _0 = nullptr, syntax_tree* _1 = nullptr, syntax_tree* _2 = nullptr, syntax_tree* _3 = nullptr) : function(function_), parameters{_0, _1, _2, _3} {}
      ~syntax_tree()
      {
         for (int i = 0; i < 4; ++i)
            if (parameters[i]) delete parameters[i];
      }
      double evaluate()
      {
         if (!function) return value;
         switch (function->parameters) {
            case 0: return ((double (*)())function->fp)();
            case 1: return ((double (*)(double))function->fp)(parameters[0]->evaluate());
            case 2: return ((double (*)(double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate());
            case 3: return ((double (*)(double, double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate(), parameters[2]->evaluate());
            case 4: return ((double (*)(double, double, double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate(), parameters[2]->evaluate(), parameters[3]->evaluate());
            default: return NAN;
         }
      }
   };

   static constexpr int TOK_NULL = 0, TOK_ERROR = 1, TOK_END = 2, TOK_COMMA = 3, TOK_OPEN = 4, TOK_CLOSE = 5, TOK_NUMBER = 6, TOK_FUNCTION = 7, TOK_ADD = 8, TOK_SUB = 9, TOK_MMD = 10, TOK_POW = 11, TOK_FAC = 12;
   struct parser_state {
      const char *start, *next;
      int type;
      double value;
      const function_spec* function;
   };

   static double _add(double a, double b) { return a + b; }
   static double _sub(double a, double b) { return a - b; }
   static double _mul(double a, double b) { return a * b; }
   static double _div(double a, double b) { return a / b; }
   static double _negate(double a) { return -a; }
   static double _comma(double, double b) { return b; }
   static double __pi() { return 3.14159265358979323846; }
   static double __e() { return 2.71828182845904523536; }
   static double __nan() { return NAN; }
   static double __inf() { return INFINITY; }

   static double _factorial(double n)
   {  // n! = 1*2*3...*n , 0!=1
      if (n < 0.0) return NAN;
      unsigned long result = 1;
      for (unsigned long i = 1; i <= (unsigned long)(n); ++i) result *= i;
      return (double)result;
   }
   static double Pnm(double n, double m)
   {  // P(n,m) = n(n-1)(n-2)...(n-m+1)
      if (n < 0.0 || m < 0.0) return NAN;
      unsigned long result = 1;
      for (unsigned long i = std::max(n, m) - std::min(n, m) + 1; i <= std::max(n, m); ++i) result *= i;
      return result;
   }
   static double Cnm(double n, double m)
   {  // C(n,m) = n(n-1)(n-2)...(n-m+1)/m!
      return Pnm(n, m) / _factorial(std::min(n, m));
   }

#ifdef _MSC_VER
#pragma function(ceil)
#pragma function(floor)
#endif

   static const function_spec* get_function(const char* name, int len = 0)
   {
      static const function_spec functions[] = {
        {2, "+", (const void*)_add},
        {2, "-", (const void*)_sub},
        {2, "*", (const void*)_mul},
        {2, "/", (const void*)_div},
        {1, "negate", (const void*)_negate},
        {2, "comma", (const void*)_comma},
        {0, "pi", (const void*)__pi},
        {0, "e", (const void*)__e},
        {0, "nan", (const void*)__nan},
        {0, "inf", (const void*)__inf},
        {1, "factorial", (const void*)_factorial},
        {2, "C", (const void*)Cnm},
        {2, "P", (const void*)Pnm},
        {2, "A", (const void*)Pnm},
        {1, "abs", (const void*)(double (*)(double))::fabs},
        {1, "sqrt", (const void*)(double (*)(double))::sqrt},
        {1, "ln", (const void*)(double (*)(double))::log},
        {1, "log", (const void*)(double (*)(double))::log},
        {1, "log10", (const void*)(double (*)(double))::log10},
        {1, "log2", (const void*)(double (*)(double))::log2},
        {1, "floor", (const void*)(double (*)(double))::floor},
        {1, "ceil", (const void*)(double (*)(double))::ceil},
        {2, "fmod", (const void*)(double (*)(double, double))::fmod},
        {2, "pow", (const void*)(double (*)(double, double))::pow},
        {1, "exp", (const void*)(double (*)(double))::exp},
        {1, "cos", (const void*)(double (*)(double))::cos},
        {1, "acos", (const void*)(double (*)(double))::acos},
        {1, "cosh", (const void*)(double (*)(double))::cosh},
        {1, "sin", (const void*)(double (*)(double))::sin},
        {1, "asin", (const void*)(double (*)(double))::asin},
        {1, "sinh", (const void*)(double (*)(double))::sinh},
        {1, "tan", (const void*)(double (*)(double))::tan},
        {1, "atan", (const void*)(double (*)(double))::atan},
        {2, "atan2", (const void*)(double (*)(double, double))::atan2},
        {1, "tanh", (const void*)(double (*)(double))::tanh},
      };
      if (len == 0) len = strlen(name);
      for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i)
         if (!strncmp(name, functions[i].fn, len) && '\0' == functions[i].fn[len])
            return &functions[i];
      return nullptr;
   }

   static void lexical_token(parser_state* ps)
   {
      ps->type = TOK_NULL;
      do {
         if (!ps->next[0]) {
            ps->type = TOK_END;
         } else if (::isdigit(ps->next[0]) || ps->next[0] == '.') { /* Reading a number. */
            ps->value = strtod(ps->next, (char**)&ps->next);
            ps->type = TOK_NUMBER;
         } else {
            if (::isalpha(ps->next[0])) { /* Look for a builtin function call. */
               const char* fn = ps->next;
               while (::isalpha(ps->next[0]) || ::isdigit(ps->next[0]) || (ps->next[0] == '_')) ps->next++;
               ps->function = get_function(fn, (int)(ps->next - fn));
               ps->type = TOK_FUNCTION;
               if (!ps->function) ps->type = TOK_ERROR;
            } else { /* Look for an operator or special character. */
               switch (ps->next++[0]) {
                  case '+': ps->type = TOK_ADD, ps->function = get_function("+"); break;
                  case '-': ps->type = TOK_SUB, ps->function = get_function("-"); break;
                  case '*': ps->type = TOK_MMD, ps->function = get_function("*"); break;
                  case '/': ps->type = TOK_MMD, ps->function = get_function("/"); break;
                  case '%': ps->type = TOK_MMD, ps->function = get_function("fmod"); break;
                  case '^': ps->type = TOK_POW, ps->function = get_function("pow"); break;
                  case '!': ps->type = TOK_FAC, ps->function = get_function("factorial"); break;
                  case '(': ps->type = TOK_OPEN; break;
                  case ')': ps->type = TOK_CLOSE; break;
                  case ',': ps->type = TOK_COMMA; break;
                  case ' ':
                  case '\t': break;  //  case '\n': case '\r':
                  default: ps->type = TOK_ERROR; break;
               }
            }
         }
      } while (ps->type == TOK_NULL);
   }

   static syntax_tree* base(parser_state* ps)
   { /* <base> = <constant> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
      syntax_tree* st;
      if (ps->type == TOK_NUMBER) {
         st = new syntax_tree(ps->value);
         lexical_token(ps);
      } else if (ps->type == TOK_OPEN) {
         lexical_token(ps);
         st = list(ps);
         if (ps->type != TOK_CLOSE) {
            ps->type = TOK_ERROR;
         } else {
            lexical_token(ps);
         }
      } else if (ps->type == TOK_FUNCTION) {
         st = new syntax_tree(ps->function);
         lexical_token(ps);
         if (st->function->parameters == 0) {
            if (ps->type == TOK_OPEN) {
               lexical_token(ps);
               if (ps->type != TOK_CLOSE) {
                  ps->type = TOK_ERROR;
               } else {
                  lexical_token(ps);
               }
            }
         } else if (st->function->parameters == 1) {
            st->parameters[0] = power(ps);
         } else {
            if (ps->type != TOK_OPEN) {
               ps->type = TOK_ERROR;
            } else {
               int i = 0;
               while (i < 4) {
                  lexical_token(ps);
                  st->parameters[i++] = expr(ps);
                  if (ps->type != TOK_COMMA) break;
               }
               if (ps->type != TOK_CLOSE || i != st->function->parameters) {
                  ps->type = TOK_ERROR;
               } else {
                  lexical_token(ps);
               }
            }
         }
      } else {
         st = new syntax_tree(NAN);
         ps->type = TOK_ERROR;
      }
      return st;
   }

   static syntax_tree* factorial(parser_state* ps)
   { /* <factorial> = <base> {"!"} */
      syntax_tree* st = base(ps);
      while (ps->type == TOK_FAC) {
         st = new syntax_tree(get_function("factorial"), st);
         lexical_token(ps);
      }
      return st;
   }

   static syntax_tree* power(parser_state* ps)
   { /* <power> = {("-" | "+")} <factorial> */
      int sign = 1;
      while (ps->type == TOK_ADD || ps->type == TOK_SUB) {
         if (ps->type == TOK_SUB) sign = -sign;
         lexical_token(ps);
      }
      syntax_tree* st = factorial(ps);
      if (sign == -1) st = new syntax_tree(get_function("negate"), st);
      return st;
   }

   static syntax_tree* factor(parser_state* ps)
   { /* <factor> = <power> {"^" <power>} */
      syntax_tree* st = power(ps);
      while (ps->type == TOK_POW) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, power(ps));
      }
      return st;
   }

   static syntax_tree* term(parser_state* ps)
   { /* <term> = <factor> {("*" | "/" | "%") <factor>} */
      syntax_tree* st = factor(ps);
      while (ps->type == TOK_MMD) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, factor(ps));
      }
      return st;
   }

   static syntax_tree* expr(parser_state* ps)
   { /* <expr> = <term> {("+" | "-") <term>} */
      syntax_tree* st = term(ps);
      while (ps->type == TOK_ADD || ps->type == TOK_SUB) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, term(ps));
      }
      return st;
   }

   static syntax_tree* list(parser_state* ps)
   { /* <list> = <expr> {"," <expr>} */
      syntax_tree* st = expr(ps);
      while (ps->type == TOK_COMMA) {
         lexical_token(ps);
         st = new syntax_tree(get_function("comma"), st, expr(ps));
      }
      return st;
   }

   static syntax_tree* compile(const char* expression)
   {
      parser_state ps;
      ps.start = ps.next = expression;
      lexical_token(&ps);
      syntax_tree* st = list(&ps);
      if (ps.type != TOK_END) {
         delete st;
         return nullptr;
      }
      return st;
   }
};
// source/json/h2_node.cpp
struct h2_json_node : h2_libc {
   static constexpr int t_absent = 0;
   static constexpr int t_null = 1;
   static constexpr int t_boolean = 2;
   static constexpr int t_number = 3;
   static constexpr int t_string = 4;
   static constexpr int t_pattern = 5;  // regex or wildcard pattern
   static constexpr int t_array = 6;
   static constexpr int t_object = 7;

   int type = t_absent;
   int index = 0;
   h2_string key_string;
   h2_string value_string;
   double value_double = 0;
   bool value_boolean = false;
   h2_list children, x; /* array or object */

   h2_json_node(int index_ = 0) : index(index_) {}
   ~h2_json_node()
   {
      h2_list_for_each_entry (p, children, h2_json_node, x) {
         p->x.out();
         delete p;
      }
   }

   int size() const
   {
      return children.count();
   }

   h2_json_node* get(int index) const
   {
      if (index < 0) index = children.count() + index;
      h2_list_for_each_entry (p, i, children, h2_json_node, x)
         if (i == index)
            return p;
      return nullptr;
   }

   h2_json_node* get(const h2_string& name, bool caseless) const
   {
      h2_list_for_each_entry (p, children, h2_json_node, x)
         if (p->key_string.equals(name, caseless))
            return p;
      return nullptr;
   }

   bool is_null() const { return t_null == type; }
   bool is_bool() const { return t_boolean == type; }
   bool is_number() const { return t_number == type; }
   bool is_string() const { return t_string == type; }
   bool is_pattern() const { return t_pattern == type; }
   bool is_array() const { return t_array == type; }
   bool is_object() const { return t_object == type; }

   h2_string format_value(const char* quote) const
   {
      switch (type) {
         case t_null: return "null";
         case t_boolean: return value_boolean ? "true" : "false";
         case t_number: return (value_double - ::floor(value_double) == 0) ? std::to_string((long long)value_double).c_str() : std::to_string(value_double).c_str();
         case t_string: return quote + value_string + quote;
         case t_pattern: return "/" + value_string + "/";
         case t_array:
         case t_object:
         default: return "";
      }
   }

   void format(int& _type, h2_string& _key, h2_string& _value, const char* quote = "") const
   {
      _type = type;
      if (key_string.size()) _key = quote + key_string + quote;
      _value = format_value(quote);
   }

   h2_lines format(bool fold, const char* quote = "", size_t depth = 0, int next = 0) const
   {
      h2_lines lines;
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back(quote + key_string + quote + ": ");
      if (is_array() || is_object()) {
         h2_lines children_lines;
         h2_list_for_each_entry (p, i, children, h2_json_node, x)
            children_lines += p->format(fold, quote, depth + 1, children.count() - i - 1);
         line.push_back(is_array() ? "[" : "{");
         if (fold && children_lines.foldable()) {
            line += children_lines.folds();
         } else {
            lines.push_back(line), line.clear();
            lines += children_lines;
            line.indent(depth * 2);
         }
         line.push_back(is_array() ? "]" : "}");
      } else {
         line.push_back(format_value(quote));
      }
      if (line.size()) {
         if (next) line.push_back(", ");
         lines.push_back(line), line.clear();
      }
      return lines;
   }
};
// source/json/h2_lexical.cpp
struct h2_json_lexical {
   static void new_lexis(h2_vector<h2_string>& lexical, const char* start, const int size)
   {
      const char* left = strip_left(start, start + size);
      const char* right = strip_right(left, start + size);
      lexical.push_back(h2_string(right - left, left));
   }

   static void parse(h2_vector<h2_string>& lexical, const char* json_string, int json_length = -1)
   {
      enum {
         st_idle,
         st_escape,
         st_single_quote,
         st_double_quote,
         st_pattern,
         st_normal
      };

      const char* pending = nullptr;
      const char* p;
      int state = st_idle, stash_state = st_idle;
      for (p = json_string; *p && json_length--; ++p) {
         switch (state) {
            case st_idle:
               if (::isspace(*p)) {
                  continue;
               } else if (strchr("{:}[,]", *p)) {
                  new_lexis(lexical, p, 1);
               } else {
                  pending = p;
                  state = st_normal;
                  if ('\"' == *p) {
                     state = st_double_quote;
                  } else if ('\'' == *p) {
                     state = st_single_quote;
                  } else if ('/' == *p) {
                     state = st_pattern;
                  } else if ('\\' == *p) {
                     stash_state = state, state = st_escape;
                  }
               }
               break;
            case st_escape:
               state = stash_state;
               break;
            case st_single_quote:
               if ('\'' == *p) {
                  new_lexis(lexical, pending, (int)((p + 1) - pending));
                  pending = nullptr;
                  state = st_idle;
               } else if ('\\' == *p) {
                  stash_state = state, state = st_escape;
               }
               break;
            case st_double_quote:
               if ('\"' == *p) {
                  new_lexis(lexical, pending, (int)((p + 1) - pending));
                  pending = nullptr;
                  state = st_idle;
               } else if ('\\' == *p) {
                  stash_state = state, state = st_escape;
               }
               break;
            case st_pattern:
               if ('/' == *p) {
                  new_lexis(lexical, pending, (int)((p + 1) - pending));
                  pending = nullptr;
                  state = st_idle;
               }
               /* no escape char */
               break;
            case st_normal:
               if (strchr("{:}[,]", *p)) {
                  new_lexis(lexical, pending, (int)(p - pending));
                  pending = nullptr;
                  new_lexis(lexical, p, 1);
                  state = st_idle;
               } else if ('\\' == *p) {
                  stash_state = state, state = st_escape;
               }
               break;
         }
      }
      if (pending) {
         new_lexis(lexical, pending, (int)(p - pending));
      }
   }
};
// source/json/h2_syntax.cpp
struct h2_json_syntax {
   size_t i = 0;
   const h2_vector<h2_string>& lexical;
   h2_json_syntax(const h2_vector<h2_string>& _lexical) : lexical(_lexical) {}

   bool parse(h2_json_node& root_node)
   {
      if (!parse_value(root_node)) return false;
      return lexical.size() <= i;  // nothing more, "{},{}"
   }

   h2_string& filter_string(h2_string& s) const
   {
      if (s.enclosed('\"'))
         s = s.unenclose('\"');
      else if (s.enclosed('\''))
         s = s.unenclose('\'');
      s = s.unescape();
      return s;
   }

   bool desire(const char* s)
   {
      if (lexical.size() <= i || !lexical[i].equals(s)) return false;
      ++i;
      return true;
   }

   bool parse_value(h2_json_node& node)
   {
      if (lexical.size() <= i) return true;

      if (lexical[i].equals("null")) {
         ++i;
         node.type = h2_json_node::t_null;
         return true;
      }
      if (lexical[i].equals("false")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = false;
         return true;
      }
      if (lexical[i].equals("true")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = true;
         return true;
      }
      if (lexical[i].equals("[")) return parse_array(node);
      if (lexical[i].equals("{")) return parse_object(node);
      if (lexical[i].startswith("/")) return parse_pattern(node);

      if (lexical[i].equals(":")) return false;
      if (lexical[i].equals(",")) return false;

      return parse_string_or_number(node);
   }

   bool parse_key(h2_json_node& node)
   {
      node.key_string = lexical[i++];
      filter_string(node.key_string);
      return true;
   }

   bool parse_pattern(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (node.value_string.enclosed('/'))
         node.value_string = node.value_string.unenclose('/');
      node.type = h2_json_node::t_pattern;
      return true;
   }

   bool parse_string_or_number(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (tinyexpr::evaluate(node.value_string.c_str(), node.value_double)) {
         node.type = h2_json_node::t_number;
         node.value_string = "";
         return true;
      }
      node.value_double = 0;
      filter_string(node.value_string);
      node.type = h2_json_node::t_string;
      return true;
   }

   bool parse_array(h2_json_node& node)
   {
      if (!desire("[")) return false;
      int n = 0;
      while (i < lexical.size() && !lexical[i].equals("]")) {
         h2_json_node* new_node = new h2_json_node(n++);
         node.children.push_back(new_node->x);
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            i++;
         else
            break;
      }

      if (!desire("]")) return false;
      node.type = h2_json_node::t_array;
      return true;
   }

   bool parse_object(h2_json_node& node)
   {
      if (!desire("{")) return false;
      int n = 0;
      while (i < lexical.size() && !lexical[i].equals("}")) {
         h2_json_node* new_node = new h2_json_node(n++);
         node.children.push_back(new_node->x);
         if (!parse_key(*new_node)) return false;
         if (!desire(":")) return false;
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            ++i;
         else
            break;
      }

      if (!desire("}")) return false;
      node.type = h2_json_node::t_object;
      return true;
   }
};
// source/json/h2_select.cpp
struct h2_json_select {
   struct value {
      int index;
      h2_string key;
   };

   h2_vector<value> values;

   h2_json_select(const char* selector)
   {
      enum {
         st_idle,
         st_in_dot,
         st_in_bracket
      };
      int state = 0;
      const char *s = nullptr, *p = selector;
      do {
         switch (state) {
            case st_idle:
               if (*p == '.') {
                  state = st_in_dot;
                  s = p + 1;
               } else if (*p == '[') {
                  state = st_in_bracket;
                  s = p + 1;
               }
               break;
            case st_in_dot:
               if (*p == '.') {  // end a part
                  if (s < p) add(s, p, true);
                  // restart a new part
                  state = st_in_dot;
                  s = p + 1;
               } else if (*p == '[') {  // end a part
                  if (s < p) add(s, p, true);
                  // restart a new part
                  state = st_in_bracket;
                  s = p + 1;
               } else if (*p == '\0') {
                  if (s < p) add(s, p, true);
                  state = st_idle;
               }
               break;
            case st_in_bracket:
               if (*p == ']') {
                  if (s < p) add(s, p, false);
                  state = st_idle;
               }
               break;
         }
      } while (*p++);
   }

   void add(const char* start, const char* end, bool only_key)  // [start, end)
   {
      start = strip_left(start, end);
      end = strip_right(start, end);
      if (start < end) {
         if (!only_key) {
            if (strspn(start, "-0123456789") == (size_t)(end - start)) {
               values.push_back({atoi(start), ""});
               return;
            } else if ((*start == '\"' && *(end - 1) == '\"') || (*start == '\'' && *(end - 1) == '\'')) {
               ++start, --end;
            }
         }
         if (start < end) values.push_back({0, h2_string(end - start, start)});
      }
   }
};
// source/json/h2_tree.cpp
struct h2_json_tree : h2_json_node {
   h2_vector<h2_string> lexical;
   h2_json_syntax syntax{lexical};
   bool illformed;

   h2_json_tree(const char* json_string, int json_length = -1)
   {
      h2_json_lexical::parse(lexical, json_string, json_length);
      illformed = !syntax.parse(*this);
   }

   h2_json_node* select(const char* selector, bool caseless)
   {
      h2_json_select select(selector);
      h2_json_node* node = this;
      for (auto& c : select.values)
         node = c.key.size() ? node->get(c.key, caseless) : node->get(c.index);
      if (node) node->key_string = "";
      return node;
   }

   h2_line serialize() const
   {
      h2_line line;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            line.printf("yellow,bold,underline", "%s%s ", comma_if(j, " "), lexical[j].c_str());
         else
            line.push_back(comma_if(j, " ") + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) {
         line.printf("yellow,bold,underline", " ... ");
      }
      return line;
   }
};
// source/json/h2_match.cpp
struct h2_json_match {
   static bool match_array(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      if (e->size() != a->size()) return false;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         if (!match(p, a->get(i), caseless))
            return false;
      return true;
   }

   static bool match_object(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      if (e->size() > a->size()) return false;
      h2_list_for_each_entry (p, e->children, h2_json_node, x)
         if (!match(p, a->get(p->key_string, caseless), caseless))
            return false;
      return true;
   }

   static bool match(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      switch (e->type) {
         case h2_json_node::t_null:
            return a->is_null();
         case h2_json_node::t_boolean:
            return a->is_bool() && e->value_boolean == a->value_boolean;
         case h2_json_node::t_number:
            if (!a->is_number()) return false;
            if (std::isnan(e->value_double) && std::isnan(a->value_double)) return true;
            if (std::isinf(e->value_double) && std::isinf(a->value_double)) return true;
            return ::fabs(e->value_double - a->value_double) < 0.00001;
         case h2_json_node::t_string:
            return a->is_string() && e->value_string.equals(a->value_string, caseless);
         case h2_json_node::t_pattern:
            return a->is_string() && h2_pattern::match(e->value_string.c_str(), a->value_string.c_str(), caseless);
         case h2_json_node::t_array:
            return a->is_array() && match_array(e, a, caseless);
         case h2_json_node::t_object:
            return a->is_object() && match_object(e, a, caseless);
         default: return false;
      }
   }

   static double array_similarity(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      double score = 0.0, sub_score = 0.0;
      if (e->size() == a->size()) score += 0.3;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         sub_score += (sub_score * i + similarity(p, a->get(i), caseless)) / (i + 1);
      return score + 0.7 * sub_score;
   }

   static double object_similarity(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      double score = 0.0, sub_score = 0.0;
      if (e->size() == a->size()) score += 0.3;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         sub_score += (sub_score * i + similarity(p, a->get(p->key_string, false), caseless)) / (i + 1);
      return score + 0.7 * sub_score;
   }

   static double similarity(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return 0;
      double score = 0.0;
      if (e->type == h2_json_node::t_array && a->type == h2_json_node::t_array) {
         score += array_similarity(e, a, caseless);
      } else if (e->type == h2_json_node::t_object && a->type == h2_json_node::t_object) {
         score += object_similarity(e, a, caseless);
      } else if (e->type == a->type) {
         score += h2_fuzzy::similarity(e->value_string.c_str(), a->value_string.c_str(), caseless);
      } else {
      }
      if (e->key_string.size() || a->key_string.size()) {
         score = score * 0.5 + 0.5 * h2_fuzzy::similarity(e->key_string.c_str(), a->key_string.c_str(), caseless);
      }
      return score;
   }

   static h2_json_node* search(const h2_list& haystack, h2_json_node* needle, bool caseless)
   {
      double max_score = 0.2;
      h2_json_node* straw = nullptr;
      h2_list_for_each_entry (p, haystack, h2_json_node, x) {
         double score = similarity(needle, p, caseless);
         if (score > max_score) {
            max_score = score;
            straw = p;
         }
      }
      return straw;
   }
};
// source/json/h2_dual.cpp
struct h2_json_dual : h2_libc {  // Combine two node into a dual
   size_t depth;
   int relationship, index = 0x7fffffff;
   bool key_equal = false, value_match = false;
   bool e_last = true, a_last = true;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_list children, x;

   static int index_cmp(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_json_dual, x)->index - h2_list_entry(b, h2_json_dual, x)->index;
   }

   ~h2_json_dual()
   {
      h2_list_for_each_entry (p, children, h2_json_dual, x) {
         p->x.out();
         delete p;
      }
   }

   void __move_dual(h2_json_node* child_e, h2_json_node* child_a, bool caseless)
   {
      children.push_back((new h2_json_dual(child_e, child_a, caseless, depth + 1))->x);
      child_e->x.out();
      delete child_e;
      child_a->x.out();
      delete child_a;
   }

   void __mark_last()
   {
      int e_count = 0, a_count = 0;
      h2_list_for_each_reverse_entry (p, children, h2_json_dual, x) {
         if (p->e_type != h2_json_node::t_absent) p->e_last = e_count++ == 0;
         if (p->a_type != h2_json_node::t_absent) p->a_last = a_count++ == 0;
      }
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, bool caseless, size_t depth_ = 0, int relationship_ = 0) : depth(depth_), relationship(relationship_)
   {
      if (e) index = e->index;
      if (e) e->format(e_type, e_key, e_value, "\"");
      if (a) a->format(a_type, a_key, a_value, "\"");
      key_equal = e_key.equals(a_key, caseless);
      value_match = h2_json_match::match(e, a, caseless);

      if (e_type == h2_json_node::t_object || a_type == h2_json_node::t_object) {
         if (e && a) {
            h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
               h2_json_node* child_a = a->get(child_e->key_string, false);
               if (!child_a && caseless) child_a = a->get(child_e->key_string, true);
               if (child_a) __move_dual(child_e, child_a, caseless);
            }
            h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
               h2_json_node* child_a = h2_json_match::search(a->children, child_e, caseless);
               if (child_a) __move_dual(child_e, child_a, caseless);
            }
         }
         for (int i = 0; i < std::max(e ? e->size() : 0, a ? a->size() : 0); ++i) {
            children.push_back((new h2_json_dual(e ? e->get(i) : nullptr, a ? a->get(i) : nullptr, caseless, depth + 1, -1))->x);
         }
         children.sort(index_cmp);
      } else if (e_type == h2_json_node::t_array || a_type == h2_json_node::t_array) {
         for (int i = 0; i < std::max(e ? e->size() : 0, a ? a->size() : 0); ++i)
            children.push_back((new h2_json_dual(e ? e->get(i) : nullptr, a ? a->get(i) : nullptr, caseless, depth + 1))->x);
      }
      __mark_last();
   }

   const char* __e_style() const { return a_type == h2_json_node::t_absent || relationship < 0 ? "light blue" : "green"; }
   const char* __a_style() const { return e_type == h2_json_node::t_absent || relationship < 0 ? "light purple" : "red,bold"; }

   void align(h2_lines& e_lines, h2_lines& a_lines) const
   {
      h2_lines e_ls, a_ls;
      h2_line e_line, a_line;
      e_line.indent(depth * 2);
      a_line.indent(depth * 2);

      if (e_type != h2_json_node::t_absent && e_key.size())
         e_line += color(e_key, key_equal ? "" : __e_style()) + ": ";
      if (a_type != h2_json_node::t_absent && a_key.size())
         a_line += color(a_key, key_equal ? "" : __a_style()) + ": ";

      if (e_type != h2_json_node::t_absent && e_value.size())
         e_line += color(e_value, value_match ? "" : __e_style());
      if (a_type != h2_json_node::t_absent && a_value.size())
         a_line += color(a_value, value_match ? "" : __a_style());

      if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) e_line.push_back(e_type == h2_json_node::t_object ? "{" : "[");
      if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) a_line.push_back(a_type == h2_json_node::t_object ? "{" : "[");

      h2_lines e_children_lines, a_children_lines;
      h2_list_for_each_entry (p, children, h2_json_dual, x)
         p->align(e_children_lines, a_children_lines);

      if ((O.fold_json >= FoldSame && key_equal && value_match) || (O.fold_json >= FoldSingle && relationship < 0)) {
         e_line += e_children_lines.foldable() ? e_children_lines.folds() : gray(" ... ");
         a_line += a_children_lines.foldable() ? a_children_lines.folds() : gray(" ... ");
      } else if (O.fold_json && e_children_lines.foldable() && a_children_lines.foldable()) {
         e_line += e_children_lines.folds();
         a_line += a_children_lines.folds();
      } else {
         if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) {
            e_ls.push_back(e_line), e_line.clear();
            e_ls += e_children_lines;
            e_line.indent(depth * 2);
         }
         if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) {
            a_ls.push_back(a_line), a_line.clear();
            a_ls += a_children_lines;
            a_line.indent(depth * 2);
         }
      }
      if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) e_line.push_back(e_type == h2_json_node::t_object ? "}" : "]");
      if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) a_line.push_back(a_type == h2_json_node::t_object ? "}" : "]");

      if (e_type != h2_json_node::t_absent && !e_last) e_line.push_back(", ");
      if (a_type != h2_json_node::t_absent && !a_last) a_line.push_back(", ");
      e_ls.push_back(e_line);
      a_ls.push_back(a_line);
      h2_lines::samesizify(e_ls, a_ls);
      e_lines += e_ls;
      a_lines += a_ls;
   }
};
// source/json/h2_json.cpp
struct h2_json {
   static h2_lines dump(const h2_string& json_string)
   {
      h2_json_tree tree(json_string.c_str());
      if (tree.illformed) return {tree.serialize()};
      h2_lines lines = tree.format(O.fold_json, O.json_source_quote);
      if (!h2_blank(O.json_source_quote)) {
         if (!lines.empty()) {
            lines.front() = "\"" + lines.front();
            lines.back() = lines.back() + "\"";
         }
         size_t max_width = lines.width();
         for (size_t i = 0; i < lines.size(); ++i) {
            lines[i].padding(max_width - lines[i].width() + 3);
            if (i < lines.size() - 1) lines[i].push_back("\\");
         }
      }
      return lines;
   }

   static h2_string select(const h2_string& json_string, const h2_string& selector, bool caseless)
   {
      h2_json_tree tree(json_string.c_str());
      if (tree.illformed) return json_string;
      h2_json_node* node = tree.select(selector.c_str(), caseless);
      if (!node) return "";
      return node->format(O.fold_json, "\"").string();
   }

   static int match(const h2_string& expect, const h2_string& actual, bool caseless)
   {  // < 0 illformed json; = 0 matched; > 0 unmatched
      h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
      if (e_tree.illformed || a_tree.illformed) return -1;
      return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
   }

   static bool diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
   {
      h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
      if (e_tree.illformed || a_tree.illformed) return false;
      h2_json_dual dual(&e_tree, &a_tree, caseless);
      dual.align(e_lines, a_lines);
      return true;
   }
};
// source/matcher/h2_matches_memcmp.cpp
h2_inline bool h2_memcmp_util::bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits)
{
   for (size_t k = 0; k < nbits; ++k) {
      size_t i = k / 8, j = 7 - k % 8;
      if (((b1[i] >> j) & 1) != ((b2[i] >> j) & 1)) return false;
   }
   return true;
}

h2_inline bool h2_memcmp_util::is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; p++)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

h2_inline bool h2_memcmp_util::is_bin_string(const char* s)
{
   for (const char* p = s; *p; p++)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

h2_inline size_t h2_memcmp_util::bin_to_bits(const char* bin, unsigned char* bytes)
{
   memset(bytes, 0, strlen(bin));
   size_t c = 0;
   for (const char* p = bin; *p; p++) {
      if (*p == ' ') continue;
      size_t i = c / 8, j = 7 - c % 8;
      ++c;
      unsigned char ebit = *p == '1' ? 1 : 0;
      bytes[i] = bytes[i] | (ebit << j);
   }
   return c;
}

h2_inline size_t h2_memcmp_util::hex_to_bytes(const char* hex, unsigned char* bytes) { return hex2bytes(hex, bytes); }
// source/matcher/h2_matches_strcmp.cpp
h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::regex_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_regex::expection(h2_mc c) const
{
   return ncsc("Re" + gray("(") + h2_stringify(e) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::wildcard_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_wildcard::expection(h2_mc c) const
{
   return ncsc("We" + gray("(") + h2_stringify(e) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, h2_mc c) const
{
   h2_string _e = e, _a = a;
   if (c.squash_whitespace) _e = e.squash(), _a = a.squash();
   if (c.fit(_a.equals(_e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(_e, a, expection(c));
}
h2_inline h2_line h2_matches_strcmp::expection(h2_mc c) const
{
   return ncsc(h2_stringify(c.squash_whitespace ? e.squash() : e, true), c, "≠");
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.contains(substring, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(substring, a, expection(c));
}
h2_inline h2_line h2_matches_substr::expection(h2_mc c) const
{
   return ncsc("Substr" + gray("(") + h2_stringify(substring, true) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.startswith(prefix_string, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expection(c));
}
h2_inline h2_line h2_matches_startswith::expection(h2_mc c) const
{
   return ncsc("StartsWith" + gray("(") + h2_stringify(prefix_string, true) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.endswith(suffix_string, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expection(c));
}
h2_inline h2_line h2_matches_endswith::expection(h2_mc c) const
{
   return ncsc("EndsWith" + gray("(") + h2_stringify(suffix_string, true) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, c.case_insensitive);
   int ret = h2_json::match(e, _a, c.case_insensitive);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(c), c.case_insensitive, "illformed json");
   if (c.fit(ret == 0)) return nullptr;
   return h2_fail::new_json(e, _a, expection(c), c.case_insensitive);
}
h2_inline h2_line h2_matches_json::expection(h2_mc c) const
{
   return ncsc(h2_stringify(e), c, "≠");
}
// source/memory/h2_piece.cpp
struct h2_piece : h2_libc {
   h2_list x;
   unsigned char *user_ptr, *page_ptr;
   size_t user_size, page_size, page_count;
   // free
   const char* who_allocate;
   h2_backtrace bt_allocate, bt_release;
   int free_times = 0;
   // snowfield
   unsigned char snow_flower;
   // forbidden
   static constexpr const unsigned readable = 1, writable = 1 << 1;
   void* forbidden_page{nullptr};
   size_t forbidden_size = 0;
   int violate_times = 0;
   void* violate_ptr{nullptr};
   const char* violate_action = "";
   bool violate_after_free = false;
   h2_backtrace violate_backtrace;

   h2_piece(size_t size, size_t alignment, const char* who, h2_backtrace& bt) : user_size(size), page_size(h2_page_size()), who_allocate(who), bt_allocate(bt)
   {
      size_t alignment_2n = alignment;
      if (not2n((unsigned)alignment)) alignment_2n = (size_t)mask2n((unsigned)alignment) + 1;
      if (alignment_2n < sizeof(void*)) alignment_2n = sizeof(void*);

      size_t user_size_plus = (user_size + alignment_2n - 1 + alignment_2n) & ~(alignment_2n - 1);
      page_count = (size_t)::ceil(user_size_plus / (double)page_size);

#if defined _WIN32
      page_ptr = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if (page_ptr == NULL) h2_console::prints("yellow", "VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      page_ptr = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (page_ptr == MAP_FAILED) h2_console::prints("yellow", "mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      user_ptr = page_ptr + page_size * page_count - user_size_plus + alignment;

      mark_snowfield();
   }

   ~h2_piece()
   {
#if defined _WIN32
      VirtualFree(page_ptr, 0, MEM_DECOMMIT | MEM_RELEASE);
#else
      ::munmap(page_ptr, page_size * (page_count + 1));
#endif
   }

   void set_forbidden(unsigned permission, void* page = 0, size_t size = 0)
   {
      if (page) forbidden_page = page;
      if (size) forbidden_size = size;

#if defined _WIN32
      DWORD old_permission, new_permission;
      new_permission = PAGE_NOACCESS;
      if (permission & readable)
         new_permission = PAGE_READONLY;
      if (permission & writable)
         new_permission = PAGE_READWRITE;
      if (!VirtualProtect(forbidden_page, forbidden_size, new_permission, &old_permission))
         h2_console::prints("yellow", "VirtualProtect failed %lu\n", GetLastError());
#else
      int new_permission = PROT_NONE;
      if (permission & readable)
         new_permission = PROT_READ;
      if (permission & writable)
         new_permission = PROT_READ | PROT_WRITE;
      if (::mprotect(forbidden_page, forbidden_size, new_permission) != 0)
         h2_console::prints("yellow", "mprotect failed %s\n", strerror(errno));
#endif
   }

#if defined _WIN32
   void violate_forbidden(void* ptr, const char* type)
   {
      if (!violate_times++) { /* 只记录第一犯罪现场 */
         violate_backtrace = h2_backtrace::dump(3);
         violate_ptr = ptr;
         violate_action = type;
         violate_after_free = 0 < free_times;
      }
      set_forbidden(readable | writable);
   }
#else
   void violate_forbidden(void* ptr)
   {
      /* 区分读写犯罪方法(一次或二次进入 segment fault):
         1) 设区域为不可读不可写
         2) 读或写行为触发 segment fault, 并进入handler
         3) 设区域为只可读不可写, 先认为犯罪为读
         4) 重新执行代码, 如果是读行为, 则结束(犯罪已经记录为读)
         5) 写行为再次触发 segment fault, 并再次进入handler
         6) 设区域为可读可写, 修正犯罪为写
         7) 恢复执行代码
       */
      auto bt = h2_backtrace::dump(4);
      if (!violate_times++) { /* 只记录第一犯罪现场 */
         set_forbidden(readable);
         violate_backtrace = bt;
         violate_ptr = ptr;
         violate_action = "read";
         violate_after_free = 0 < free_times;
      } else {
         set_forbidden(readable | writable);
         if (bt == violate_backtrace) /* 是第一犯罪现场 */
            violate_action = "write";
      }
   }
#endif

   void mark_snowfield()
   {
      static unsigned char s_snow_flower = (unsigned char)::rand();
      snow_flower = ++s_snow_flower;
      memset(page_ptr, snow_flower, user_ptr - page_ptr);
      memset(user_ptr + user_size, snow_flower, (page_ptr + page_size * page_count) - (user_ptr + user_size));
      set_forbidden(readable, page_ptr + page_size * page_count, page_size);
   }

   h2_fail* check_snowfield(const unsigned char* start, const unsigned char* end)
   {
      for (const unsigned char* p = start; p < end; ++p) {
         if (*p == snow_flower) continue;
         int n = std::min((int)(end - p), 8);
         for (; 0 < n; --n)
            if (p[n - 1] != snow_flower) break;
         h2_vector<unsigned char> spot(p, p + n);
         return h2_fail::new_overflow(user_ptr, user_size, p, "write", spot, bt_allocate, h2_backtrace());
      }
      return nullptr;
   }

   h2_fail* check_snowfield()
   {
      h2_fail* fail = nullptr;
      fail = check_snowfield(user_ptr + user_size, page_ptr + page_size * page_count);
      if (!fail) fail = check_snowfield(page_ptr, user_ptr);
      return fail;
   }

   h2_fail* violate_fail()
   {
      if (violate_after_free)
         return h2_fail::new_use_after_free(user_ptr, violate_ptr, violate_action, bt_allocate, bt_release, violate_backtrace);
      else
         return h2_fail::new_overflow(user_ptr, user_size, violate_ptr, violate_action, h2_vector<unsigned char>(), bt_allocate, violate_backtrace);
   }

   h2_fail* check_asymmetric_free(const char* who_release)
   {
      if (h2_in(who_allocate, 5, "malloc", "calloc", "realloc", "posix_memalign", "aligned_alloc") && h2_in(who_release, 1, "free")) return nullptr;
      // if (h2_in(who_allocate, 2, "new", "new nothrow") && h2_in(who_release, 2, "delete", "delete nothrow")) return nullptr;
      // if (h2_in(who_allocate, 2, "new[]", "new[] nothrow") && h2_in(who_release, 2, "delete[]", "delete[] nothrow")) return nullptr;
      // some compiler new but delete[]
      if (h2_in(who_allocate, 4, "new", "new nothrow", "new[]", "new[] nothrow") && h2_in(who_release, 4, "delete", "delete nothrow", "delete[]", "delete[] nothrow")) return nullptr;

      if (bt_allocate.in(h2_exempt::I().fps)) return nullptr;
      return h2_fail::new_asymmetric_free(user_ptr, who_allocate, who_release, bt_allocate, bt_release);
   }

   h2_fail* check_double_free(h2_backtrace& bt)
   {
      if (!free_times++) {
         bt_release = bt;
         return nullptr;
      }
      return h2_fail::new_double_free(user_ptr, bt_allocate, bt_release, bt);
   }

   h2_fail* free(const char* who_release)
   {
      h2_fail* fail = check_double_free(h2_backtrace::dump(4));
      if (!fail) fail = check_asymmetric_free(who_release);
      if (!fail) fail = check_snowfield();
      if (!fail) set_forbidden(0, page_ptr, page_size * (page_count + 1));
      return fail;
   }

   bool in_page_range(const unsigned char* p) const
   {
      return page_ptr <= p && p < page_ptr + page_size * (page_count + 1);
   }
};
// source/memory/h2_leaky.cpp
struct h2_leaky {
   struct leak {
      void* ptr;
      h2_backtrace bt;
      h2_vector<std::pair<size_t, size_t>> sizes;

      leak(void* ptr_, const h2_backtrace& bt_) : ptr(ptr_), bt(bt_) {}

      h2_vector<std::pair<size_t, size_t>>::iterator find(size_t size)
      {
         for (auto it = sizes.begin(); it != sizes.end(); it++)
            if (it->first == size)
               return it;
         return sizes.end();
      }

      void add(size_t size)
      {
         if (sizes.end() == find(size)) sizes.push_back({size, 0});
         find(size)->second++;
      }

      h2_fail* check(const char* where, const char* filine)
      {
         size_t s = 0;
         for (auto& p : sizes)
            s += p.first * p.second;
         return h2_fail::new_memory_leak(ptr, s, sizes, bt, where, filine);
      }
   };

   h2_vector<leak> leaks;

   h2_vector<leak>::iterator find(const h2_backtrace& bt)
   {
      for (auto it = leaks.begin(); it != leaks.end(); it++)
         if (it->bt == bt)
            return it;
      return leaks.end();
   }

   void add(void* ptr, size_t size, const h2_backtrace& bt)
   {
      if (leaks.end() == find(bt)) leaks.push_back({ptr, bt});
      find(bt)->add(size);
   }

   h2_fail* check(const char* where, const char* filine)
   {
      h2_fail* fails = nullptr;
      for (auto& p : leaks) h2_fail::append_subling(fails, p.check(where, filine));
      return fails;
   }
};
// source/memory/h2_block.cpp
struct h2_block_attributes {
   unsigned long long limit = 0xffffffffffffull;
   size_t alignment = sizeof(void*);
   size_t n_fill = 0;
   unsigned char s_fill[32];
   bool noleak = false;

   size_t parse_fill(const char* p, unsigned char bytes[])
   {
      if (p[0] == '0' && ::tolower(p[1]) == 'x') {
         return hex2bytes(p + 2, bytes);
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

   h2_block_attributes(const char* attributes)
   {
      const char* p;
      if (get_keyvalue(attributes, "noleak")) noleak = true;
      if ((p = get_keyvalue(attributes, "limit"))) limit = strtod(p, nullptr);
      if ((p = get_keyvalue(attributes, "align"))) alignment = strtod(p, nullptr);
      if ((p = get_keyvalue(attributes, "fill"))) n_fill = parse_fill(p, s_fill);
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list pieces;

   h2_block_attributes attributes;
   unsigned long long footprint = 0, allocated = 0;
   const char* where;
   const char* filine;

   h2_block(const char* attributes_, const char* where_, const char* filine_) : attributes(attributes_), where(where_), filine(filine_) {}

   h2_fail* check()
   {
      h2_fail* fails = nullptr;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->violate_times)
            h2_fail::append_subling(fails, p->violate_fail());

      if (fails) return fails;

      h2_leaky leaky;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (!attributes.noleak && !p->free_times)
            leaky.add(p->user_ptr, p->user_size, p->bt_allocate);

      fails = leaky.check(where, filine);
      if (fails) return fails;

      /* why not chain fails in subling? report one fail ignore more for clean.
         when fail, memory may be in used, don't free and keep it for robust */
      h2_list_for_each_entry (p, pieces, h2_piece, x) {
         p->x.out();
         delete p;
      }
      return nullptr;
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, unsigned char c_fill, bool fill, h2_backtrace& bt)
   {
      if (attributes.limit < allocated + size) return nullptr;
      allocated += size;
      if (footprint < allocated) footprint = allocated;

      // allocate action alignment is prior to block level alignment
      if (alignment == 0) alignment = attributes.alignment;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      // allocate action fill is prior to block level fill
      unsigned char* s_fill = attributes.s_fill;
      size_t n_fill = attributes.n_fill;
      if (fill) {
         s_fill = &c_fill;
         n_fill = 1;
      }
      if (0 < n_fill)
         for (size_t i = 0, j = 0; i < size; ++i, ++j)
            ((unsigned char*)p->user_ptr)[i] = s_fill[j % n_fill];

      pieces.push_back(p->x);
      return p;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      allocated -= p->user_size;
      return p->free(who);
   }

   h2_piece* get_piece(const void* ptr) const
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->user_ptr == ptr) return p;
      return nullptr;
   }

   h2_piece* host_piece(const void* ptr) const
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->in_page_range((const unsigned char*)ptr)) return p;
      return nullptr;
   }
};
// source/memory/h2_stack.cpp
struct h2_stack {
   h2_singleton(h2_stack);
   h2_list blocks;
   bool at_exit = false;

   void push(const char* block_attributes, const char* where, const char* filine)
   {
      h2_block* b = new h2_block(block_attributes, where, filine);
      blocks.push(b->x);
   }

   h2_fail* pop()
   {
      h2_block* b = h2_list_pop_entry(blocks, h2_block, x);
      h2_fail* fail = b->check();
      delete b;
      return fail;
   }

   h2_block* top()
   {
      return h2_list_top_entry(blocks, h2_block, x);
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, const char* fill)
   {
      auto bt = h2_backtrace::dump(2);
      h2_block* b = bt.in(h2_exempt::I().fps) ? h2_list_bottom_entry(blocks, h2_block, x) : h2_list_top_entry(blocks, h2_block, x);
      return b ? b->new_piece(who, size, alignment, fill ? *fill : 0, fill, bt) : nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return p->rel_piece(who, piece);
      }
      if (!at_exit && O.os != 'W')
         if (!h2_backtrace::dump(1).in(h2_exempt::I().fps))
            h2_debug(2, "Warning: %s %p not found!", who, ptr);
      return nullptr;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return piece;
      }
      return nullptr;
   }

   h2_piece* host_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->host_piece(ptr);
         if (piece) return piece;
      }
      return nullptr;
   }
};
// source/memory/h2_override.cpp
struct h2_override {
   static size_t size(const void* ptr)
   {
      h2_piece* p = h2_stack::I().get_piece(ptr);
      return p ? p->user_size : 0;
   }
   static void free(void* ptr)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("free", ptr));
   }
   static void* malloc(size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("malloc", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* calloc(size_t count, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("calloc", size * count, 0, "\0");
      return p ? p->user_ptr : nullptr;
   }
   static void* realloc(void* ptr, size_t size)
   {
      h2_piece *old_p = nullptr, *new_p = nullptr;
      if (size == 0 && O.os == 'm') size = 1;
      if (ptr) old_p = h2_stack::I().get_piece(ptr);
      if (size) new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (old_p && new_p) memcpy(new_p->user_ptr, old_p->user_ptr, std::min(old_p->user_size, size));
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("free", ptr));
      return new_p ? new_p->user_ptr : nullptr;
   }
   static int posix_memalign(void** memptr, size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("posix_memalign", size, alignment, nullptr);
      return p ? (*memptr = p->user_ptr, 0) : ENOMEM;
   }
   static void* aligned_alloc(size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("aligned_alloc", size, alignment, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new(std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new(std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new nothrow", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new[](std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new[]", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new[](std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new[] nothrow", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void operator delete(void* ptr)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete(void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete nothrow", ptr));
   }
   static void operator delete[](void* ptr)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete[]", ptr));
   }
   static void operator delete[](void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete[] nothrow", ptr));
   }
};
// source/memory/h2_override_stdlib.cpp
struct h2_override_stdlib {
   h2_list stubs;

   static char* strdup(char* s)
   {
      char* ret = (char*)h2_override::malloc(strlen(s) + 1);
      if (ret) strcpy(ret, s);
      return ret;
   }

   void set()
   {
      h2_stubs::add(stubs, (void*)::free, (void*)h2_override::free, "free", H2_FILINE);
      h2_stubs::add(stubs, (void*)::malloc, (void*)h2_override::malloc, "malloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::realloc, (void*)h2_override::realloc, "realloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::calloc, (void*)h2_override::calloc, "calloc", H2_FILINE);
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
      h2_stubs::add(stubs, (void*)::posix_memalign, (void*)h2_override::posix_memalign, "posix_memalign", H2_FILINE);
#endif
#if defined _ISOC11_SOURCE
      h2_stubs::add(stubs, (void*)::aligned_alloc, (void*)h2_override::aligned_alloc, "aligned_alloc", H2_FILINE);
#endif
      // valloc pvalloc memalign deprecated
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), "new", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), "new nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), "new[]", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), "new[] nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), "delete", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), "delete nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), "delete[]", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), "delete[] nothrow", H2_FILINE);
   }

   void reset() { h2_stubs::clear(stubs); }
};
#if defined __linux
// source/memory/h2_override_linux.cpp
// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html

struct h2_override_platform {
   static void free_hook(void* __ptr, const void* caller) { h2_override::free(__ptr); }
   static void* malloc_hook(size_t __size, const void* caller) { return h2_override::malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return h2_override::realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return h2_override::aligned_alloc(__alignment, __size); }

   void (*saved__free_hook)(void*, const void*);
   void* (*saved__malloc_hook)(size_t, const void*);
   void* (*saved__realloc_hook)(void*, size_t, const void*);
   void* (*saved__memalign_hook)(size_t, size_t, const void*);

   h2_override_platform()
   {
      saved__free_hook = __free_hook;
      saved__malloc_hook = __malloc_hook;
      saved__realloc_hook = __realloc_hook;
      saved__memalign_hook = __memalign_hook;
   }

   void set()
   {
      __free_hook = free_hook;
      __malloc_hook = malloc_hook;
      __realloc_hook = realloc_hook;
      __memalign_hook = memalign_hook;
   }

   void reset()
   {
      __free_hook = saved__free_hook;
      __malloc_hook = saved__malloc_hook;
      __realloc_hook = saved__realloc_hook;
      __memalign_hook = saved__memalign_hook;
   }
};
#elif defined __APPLE__
// source/memory/h2_override_macos.cpp
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
#endif

struct h2_override_platform {
   static size_t mz_size(malloc_zone_t* zone, const void* ptr) { return h2_override::size(ptr); }
   static void* mz_malloc(malloc_zone_t* zone, size_t size) { return h2_override::malloc(size); }
   static void* mz_calloc(malloc_zone_t* zone, size_t num_items, size_t size) { return h2_override::calloc(num_items, size); }
   static void* mz_valloc(malloc_zone_t* zone, size_t size) { return h2_override::malloc(size); }
   static void mz_free(malloc_zone_t* zone, void* ptr) { h2_override::free(ptr); }
   static void mz_free_sized(malloc_zone_t* zone, void* ptr, size_t size) { h2_override::free(ptr); }
   static void* mz_realloc(malloc_zone_t* zone, void* ptr, size_t size) { return h2_override::realloc(ptr, size); }
   static void* mz_memalign(malloc_zone_t* zone, size_t align, size_t size) { return h2_override::aligned_alloc(align, size); }
   static void mz_destroy(malloc_zone_t* zone) {}

   static kern_return_t mi_enumerator(task_t task, void*, unsigned type_mask, vm_address_t zone_address, memory_reader_t reader, vm_range_recorder_t recorder) { return KERN_FAILURE; }
   static size_t mi_good_size(malloc_zone_t* zone, size_t size) { return size; }
   static boolean_t mi_check(malloc_zone_t* zone) { return true; }
   static void mi_print(malloc_zone_t* zone, boolean_t verbose) {}
   static void mi_log(malloc_zone_t* zone, void* address) {}
   static void mi_force_lock(malloc_zone_t* zone) {}
   static void mi_force_unlock(malloc_zone_t* zone) {}

   static boolean_t mi_zone_locked(malloc_zone_t* zone) { return false; }  // Hopefully unneeded by us!

   static malloc_zone_t* get_default_zone()
   {
      malloc_zone_t** zones = nullptr;
      unsigned int num_zones = 0;

      if (KERN_SUCCESS != malloc_get_all_zones(0, nullptr, (vm_address_t**)&zones, &num_zones)) num_zones = 0;
      if (num_zones) return zones[0];
      return malloc_default_zone();
   }

   malloc_introspection_t mi;
   malloc_zone_t mz;

   h2_override_platform()
   {
      memset(&mi, 0, sizeof(mi));
      mi.enumerator = &mi_enumerator;
      mi.good_size = &mi_good_size;
      mi.check = &mi_check;
      mi.print = &mi_print;
      mi.log = &mi_log;
      mi.force_lock = &mi_force_lock;
      mi.force_unlock = &mi_force_unlock;

      memset(&mz, 0, sizeof(malloc_zone_t));
      mz.version = 4;
      mz.zone_name = "h2unit";
      mz.size = &mz_size;
      mz.malloc = &mz_malloc;
      mz.calloc = &mz_calloc;
      mz.valloc = &mz_valloc;
      mz.free = &mz_free;
      mz.realloc = &mz_realloc;
      mz.destroy = &mz_destroy;
      mz.batch_malloc = nullptr;
      mz.batch_free = nullptr;
      mz.introspect = &mi;

#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      mz.version = 6;
      mz.free_definite_size = &mz_free_sized;
      mz.memalign = &mz_memalign;
      mi.zone_locked = &mi_zone_locked;
      if (malloc_default_purgeable_zone) malloc_default_purgeable_zone();
#endif
   }

   void set()
   {
      malloc_zone_register(&mz);
      malloc_zone_t* default_zone = get_default_zone();
      malloc_zone_unregister(default_zone);
      malloc_zone_register(default_zone);
   }

   void reset()
   {
      malloc_zone_unregister(&mz);
   }
};
#elif defined _MSC_VER
// source/memory/h2_override_windows.cpp
// https://github.com/microsoft/mimalloc
// https://github.com/gperftools/gperftools

struct h2_override_platform {
   h2_list stubs;

   // windows specific free_base, free_dbg called by CRT internal functions or operator delete
   // windows specific _msize, _expand
   // A MS CRT "internal" function, implemented using _calloc_impl
   // obscured: _aligned_offset_malloc, _aligned_realloc, _aligned_recalloc, _aligned_offset_realloc, _aligned_offset_recalloc, _malloca, _freea ,_recalloc

   static void _free_base(void* ptr) { h2_override::free(ptr); }
   static void* _expand(void* memblock, size_t size) { return NULL; }
   // When _DEBUG _CRTDBG_MAP_ALLOC (default undefined) is defined CRT maps all to _*_dbg, bug CRT Debug version enabled.
#ifndef NDEBUG
   static void _free_dbg(void* userData, int blockType)
   {
      h2_override::free(userData);
   }
   static void* _malloc_dbg(size_t size, int blockType, const char* filename, int linenumber) { return h2_override::malloc(size); }
   static void* _realloc_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return h2_override::realloc(userData, newSize); }
   static void* _calloc_dbg(size_t num, size_t size, int blockType, const char* filename, int linenumber) { return h2_override::calloc(num, size); }
   static size_t _msize_dbg(void* userData, int blockType) { return h2_override::size(userData); }
   static void* _expand_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return NULL; }
#endif
   static void* _aligned_malloc(size_t size, size_t alignment)
   {
      return h2_override::aligned_alloc(size, alignment);
   }
   static void _aligned_free(void* memblock) { h2_override::free(memblock); }

   void set()
   {
      h2_stubs::add(stubs, (void*)::_free_base, (void*)_free_base, "_free_base", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_msize, (void*)h2_override::size, "_msize", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_expand, (void*)_expand, "_expand", H2_FILINE);
#ifndef NDEBUG
      h2_stubs::add(stubs, (void*)::_free_dbg, (void*)_free_dbg, "_free_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_malloc_dbg, (void*)_malloc_dbg, "_malloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_realloc_dbg, (void*)_realloc_dbg, "_realloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_calloc_dbg, (void*)_calloc_dbg, "_calloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_expand_dbg, (void*)_expand_dbg, "_expand_dbg", H2_FILINE);
#endif
      //// h2_stubs::add(stubs,(void*)::_calloc_crt, (void*)h2_override::calloc, "_calloc_crt", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_aligned_malloc, (void*)_aligned_malloc, "_aligned_malloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_aligned_free, (void*)_aligned_free, "_aligned_free", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_strdup, (void*)h2_override_stdlib::strdup, "_strdup", H2_FILINE);  // strdup call to _strdup
   }

   void reset() { h2_stubs::clear(stubs); }
};
#else  // +MinGW
// source/memory/h2_override_cygwin.cpp
struct h2_override_platform {
   h2_list stubs;

   static char* strndup(char* s, size_t n)
   {
      char* ret = (char*)h2_override::malloc(n + 1);
      if (ret) {
         strncpy(ret, s, n);
         ret[n] = '\0';
      }
      return ret;
   }

   void set()
   {
      h2_stubs::add(stubs, (void*)::strdup, (void*)h2_override_stdlib::strdup, "strdup", H2_FILINE);
#if defined __CYGWIN__
      h2_stubs::add(stubs, (void*)::strndup, (void*)strndup, "strndup", H2_FILINE);
#endif
   }
   void reset() { h2_stubs::clear(stubs); }
};
#endif
// source/memory/h2_memory.cpp
struct h2_overrides {
   h2_singleton(h2_overrides);
   h2_override_stdlib stdlib;
   h2_override_platform platform;

   void set()
   {
      stdlib.set();
      platform.set();
   }
   void reset()
   {
      platform.reset();
      stdlib.reset();
   }
};

h2_inline void h2_memory::initialize()
{
   if (O.memory_check) h2_exempt::setup();
   hook();
   stack::root();
}
h2_inline void h2_memory::finalize()
{
   if (O.memory_check) h2_stack::I().at_exit = true;
}
h2_inline void h2_memory::hook(bool overrides)
{
   static bool s_overrides = false;
   if (O.memory_check) {
      if (overrides) {
         if (!s_overrides) h2_overrides::I().set();
      } else {
         if (s_overrides) h2_overrides::I().reset();
      }
      s_overrides = overrides;
   }
}

h2_inline void h2_memory::stack::root()
{
   h2_stack::I().push("", "root", H2_FILINE);
}
h2_inline void h2_memory::stack::push(const char* filine)
{
   h2_stack::I().push("", "case", filine);
}
h2_inline h2_fail* h2_memory::stack::pop()
{
   return h2_stack::I().pop();
}
h2_inline long long h2_memory::stack::footprint()
{
   return h2_stack::I().top()->footprint;
}

h2_inline h2_memory::stack::block::block(const char* attributes, const char* filine)
{
   unmem = !!get_keyvalue(attributes, "unmem");
   if (unmem) h2_memory::hook(false);
   h2_stack::I().push(attributes, "block", filine);
}
h2_inline h2_memory::stack::block::~block()
{
   h2_runner::failing(h2_stack::I().pop());
   if (unmem) h2_memory::hook();
}
// source/memory/h2_exempt.cpp
struct h2_exempt_stub {
   static time_t mktime(struct tm* timeptr) { return 1325347200; }
   static char* asctime(const struct tm* timeptr) { return (char*)"SUN JAN 1 00:00:00 2012\n"; }
   static char* asctime_r(const struct tm* timeptr, char* buf) { return strcpy(buf, (char*)"SUN JAN 1 00:00:00 2012\n"); }
   static char* ctime(const time_t* clock) { return (char*)"SUN JAN 1 00:00:00 2012\n"; }
   static char* ctime_r(const time_t* clock, char* buf) { return strcpy(buf, (char*)"SUN JAN 1 00:00:00 2012\n"); }
   static struct tm* localtime(const time_t* clock) { return gmtime(clock); }
   static struct tm* localtime_r(const time_t* timep, struct tm* result) { return gmtime_r(timep, result); }
   static struct tm* gmtime(const time_t* clock) { return gmtime_r(clock, nullptr); }

   static struct tm* gmtime_r(const time_t* clock, struct tm* result)
   {
      static struct tm st;
      if (!result) result = &st;
      memset(result, 0, sizeof(struct tm));
      //// result->tm_sec = 0; result->tm_min = 0; result->tm_hour = 0; result->tm_mon = 0; result->tm_wday = 0; result->tm_yday = 0;
      result->tm_mday = 1;
      result->tm_year = 2012 - 1900;
      return result;
   }
};

h2_inline void h2_exempt::setup()
{
   static h2_list stubs;

   h2_stubs::add(stubs, (void*)::gmtime, (void*)h2_exempt_stub::gmtime, "gmtime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::ctime, (void*)h2_exempt_stub::ctime, "ctime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::asctime, (void*)h2_exempt_stub::asctime, "asctime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::localtime, (void*)h2_exempt_stub::localtime, "localtime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::mktime, (void*)h2_exempt_stub::mktime, "mktime", H2_FILINE);

#if defined _WIN32
   add_by_fp((void*)::_wchdir);
   add_by_fp((void*)::fopen);
   add_by_fp((void*)::fclose);
   add_by_fp((void*)::strftime);
   add_by_fp((void*)::gmtime_s);
   add_by_fp((void*)::_gmtime32);
   add_by_fp((void*)::_gmtime64);
   add_by_fp((void*)::_gmtime32_s);
   add_by_fp((void*)::_gmtime64_s);
   add_by_fp(h2_numberfy<void*>(&std::type_info::name));
#else
   h2_stubs::add(stubs, (void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, "gmtime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, "ctime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, "asctime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, "localtime_r", H2_FILINE);
   add_by_fp((void*)::sscanf);
   add_by_fp((void*)::sprintf);
   add_by_fp((void*)::vsnprintf);

#if defined __APPLE__ && defined __clang__
   add_by_fp((void*)::snprintf);
   add_by_fp((void*)::snprintf_l);
   add_by_fp((void*)::strftime_l);
   add_by_fp((void*)::strtod_l);
   add_by_fp((void*)::strtold);
   add_by_fp((void*)::strtof_l);
#endif
#endif

#if defined __GNUC__
   add_by_fp((void*)abi::__cxa_demangle);
   add_by_fp((void*)abi::__cxa_throw);
#if !defined __clang__
   add_by_fp((void*)::__cxa_allocate_exception);
#endif
#endif

   add_by_fp((void*)h2_pattern::regex_match);
}

h2_inline void h2_exempt::add_by_name(const char* fn)
{
   h2_symbol* res[16];
   int n = h2_nm::get_by_name(fn, res, 16);
   for (int i = 0; i < n; ++i)
      add_by_fp(h2_load::addr_to_ptr(res[i]->addr));
}

h2_inline void h2_exempt::add_by_fp(void* fp)
{
   h2_array_append(I().fps, h2_cxa::follow_jmp(fp));
}
// source/except/h2_debug.cpp
static inline bool in_debugging()
{
   bool ret = false;
#if defined __linux
   char t[1024];
   FILE* f = ::fopen("/proc/self/status", "r");
   if (f) {
      while (::fgets(t, sizeof(t) - 1, f)) {
         if (strncmp(t, "TracerPid:\t", 11) == 0) {
            ret = t[11] != '\0' && t[11] != '0';
            break;
         }
      }
      ::fclose(f);
   }
#elif defined __APPLE__
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", (int)getpid());
   FILE* f = ::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r");
   if (f) {
      while (::fgets(t, sizeof(t) - 1, f)) {
         if (strstr(t, h2_basefile(O.path)) || strstr(t, attach_pid)) {
            ret = true;
            break;
         }
      }
      ::pclose(f);
   }
#endif
   return ret;
}

static inline bool h2_attach_debugger()
{
#if defined __linux || defined __APPLE__
   while (!in_debugging()) {
      static h2_once one;
      if (one) {
         char cmd[512];
         ::printf("\nEnter \033[33mpassword\033[0m for connecting \033[33m%s\033[0m. \n", O.os == 'L' ? "GDB" : "LLDB");
#if defined __linux
         sprintf(cmd, "sudo gdb --silent -ex cont --pid=%d", (int)getpid());
#elif defined __APPLE__
         sprintf(cmd, "sudo lldb -o 'continue' --attach-pid %d", (int)getpid());
#endif
         if (fork() == 0) exit(system(cmd));
      }
      h2_sleep(100);
   }
#endif
   return true;
}

#if defined __linux
#if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#define h2_raise_trap() asm volatile("int $3")
#else
#define h2_raise_trap() raise(SIGTRAP)
#endif
#elif defined __APPLE__
#if defined(__i386__) || defined(__x86_64__)
#define h2_raise_trap() __asm__("int $3\n" : :)
#elif defined(__aarch64__)
#define h2_raise_trap() __asm__(".inst 0xd4200000")
#endif
#else
#define h2_raise_trap() in_debugging()
#endif
// source/except/h2_crash.cpp
struct h2_crash {
#if defined _WIN32
   static LONG segment_fault_handler(_EXCEPTION_POINTERS* ExceptionInfo)
   {
      if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
         h2_piece* piece = h2_stack::I().host_piece((const void*)ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
         if (piece) {
            auto operation = ExceptionInfo->ExceptionRecord->ExceptionInformation[0];
            piece->violate_forbidden((void*)ExceptionInfo->ExceptionRecord->ExceptionInformation[1], operation == 0 ? "read" : (operation == 1 ? "write" : (operation == 8 ? "execute" : "unknown")));
            return EXCEPTION_CONTINUE_EXECUTION;
         }
      }
      h2_debug(0, "");
      return EXCEPTION_EXECUTE_HANDLER;
   }

   static void install()
   {
      SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)segment_fault_handler);
   }
#else
   // https://www.gnu.org/software/libsigsegv/
   static void segment_fault_handler(int sig, siginfo_t* si, void* unused)
   {
      // si->si_code == SEGV_ACCERR
      h2_piece* piece = h2_stack::I().host_piece(si->si_addr);
      if (piece) {
         piece->violate_forbidden(si->si_addr);
         return;
      }
      h2_debug(0, "");
      abort();
   }

   static void install()
   {
      struct sigaction action;
      action.sa_sigaction = segment_fault_handler;
      action.sa_flags = SA_SIGINFO;
      sigemptyset(&action.sa_mask);

      if (sigaction(SIGSEGV, &action, nullptr) == -1) perror("Register SIGSEGV handler failed");
      if (sigaction(SIGBUS, &action, nullptr) == -1) perror("Register SIGBUS handler failed");
   }
#endif
};
// source/except/h2_exception.cpp
struct h2_exception {
   h2_singleton(h2_exception);
   h2_list stubs;
   h2_backtrace last_bt;
   char last_type[1024];

#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
   static void RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments)
   {
      I().last_bt = h2_backtrace::dump(1);
      if (O.exception_as_fail) h2_runner::failing(h2_fail::new_exception("was thrown", "", I().last_bt));
      h2::h2_stub_temporary_restore t((void*)::RaiseException);
      ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
   }
#else
   static void __cxa_throw(void* thrown_exception, std::type_info* tinfo, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      I().last_bt = h2_backtrace::dump(1);
      h2_cxa::demangle(tinfo->name(), I().last_type);
      if (O.exception_as_fail) h2_runner::failing(h2_fail::new_exception("was thrown", I().last_type, I().last_bt));
      h2::h2_stub_temporary_restore t((void*)abi::__cxa_throw);
      abi::__cxa_throw(thrown_exception, tinfo, dest);
   }
#endif

   static void initialize()
   {
#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
      h2_stubs::add(I().stubs, (void*)::RaiseException, (void*)RaiseException, "RaiseException", H2_FILINE);
#else
      h2_stubs::add(I().stubs, (void*)abi::__cxa_throw, (void*)__cxa_throw, "__cxa_throw", H2_FILINE);
#endif
      if (!O.debugger_trap) h2_crash::install();
   }
};
// source/stub/h2_e9.cpp
// https://github.com/microsoft/Detours/blob/master/src/detours.cpp

#if defined __i386__ || defined _M_IX86
static constexpr int h2_e9_size = 1 + 4;
#elif defined __x86_64__ || defined _M_X64
static constexpr int h2_e9_size = 2 + 8 + 2;
#elif defined __arm64__ || defined __aarch64__
static constexpr int h2_e9_size = 4 + 4 + 8;
#endif

static inline bool h2_e9_save(void* srcfp, unsigned char* opcode)
{
#if defined _WIN32
   DWORD t;
   if (!VirtualProtect(srcfp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t)) return false;  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
#else
   static unsigned long long page_size = (unsigned long long)h2_page_size(); // uintptr_t
   unsigned long long page_start = reinterpret_cast<unsigned long long>(srcfp) & ~(page_size - 1);
   int page_count = ::ceil((reinterpret_cast<unsigned long long>(srcfp) + h2_e9_size - page_start) / (double)page_size);
   if (mprotect(reinterpret_cast<void*>(page_start), page_count * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) return false;
#endif
   if (opcode) memcpy(opcode, srcfp, h2_e9_size);
   return true;
}

static inline void h2_e9_set(void* srcfp, void* dstfp)
{
   unsigned char* I = static_cast<unsigned char*>(srcfp);

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
   long long delta = (unsigned char*)dstfp - I - 5;
   if (delta < -0x7fffffff || 0x7fffffff < delta) {  //x86_64 asm("movq $dstfp, %rax; jmpq %rax")
      unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
      memcpy(C + 2, &dstfp, sizeof(void*));
      memcpy(I, C, sizeof(C));
   } else {  //i386 asm("jmp offset")
      unsigned char C[] = {0xE9, 0, 0, 0, 0};
      *(long*)(&C[1]) = (long)delta;
      memcpy(I, C, sizeof(C));
   }
   // ::FlushInstructionCache(GetCurrentProcess(), srcfp, h2_e9_size);

#elif defined __arm64__ || defined __aarch64__

#   pragma pack(push, 1)
   struct ldr_br_dst {
      unsigned int ldr;
      unsigned int br;
      void* dstfp;
   };
   struct b_offset {
      unsigned int b : 8;
      unsigned int offset : 24;
   };
#   pragma pack(pop)
   long long offset = (unsigned char*)dstfp - I;

   if (1 || offset < -8388607 - 1 || 0x7fffff < offset) {  // signed 2^24
      struct ldr_br_dst* p = static_cast<ldr_br_dst*>(srcfp);
      p->ldr = 0x58000000 | ((8 / 4) << 5) | 17;  // x17 register store dstfp
      p->br = 0xD61F0000 | (17 << 5);             // jmp x17
      p->dstfp = dstfp;
   } else {
      struct b_offset* p = static_cast<b_offset*>(srcfp);
      p->b = offset >= 0 ? 0x14 : 0x17;  //b 14 forward 17 backward
      p->offset = (unsigned long)((offset / 4) & 0xffffff);
   }

   __builtin___clear_cache((char*)I, (char*)I + h2_e9_size);
#endif
}

static inline void h2_e9_reset(void* srcfp, unsigned char* opcode)
{
   memcpy(srcfp, opcode, h2_e9_size);
   // ::FlushInstructionCache(GetCurrentProcess(), srcfp, h2_e9_size);
#if defined __arm64__ || defined __aarch64__
   __builtin___clear_cache(static_cast<char*>(srcfp), static_cast<char*>(srcfp) + h2_e9_size);
#endif
}
// source/stub/h2_source.cpp
struct h2_source : h2_libc {
   h2_list x;
   unsigned char origin_opcode[32];
   void* source_fp;
   int reference_count = 0;
   h2_source(void* source_fp_, const char* srcfn, const char* filine) : source_fp(source_fp_)
   {
      if (!h2_e9_save(source_fp, origin_opcode)) {
         h2_console::prints("yellow", "STUB %s by %s() failed %s\n", srcfn, O.os == 'W' ? "VirtualProtect" : "mprotect", filine);
         if (O.os == 'm') h2_console::prints("", "try: "), h2_console::prints("green", "printf '\\x07' | dd of=%s bs=1 seek=160 count=1 conv=notrunc\n", O.path);
         if (O.os == 'L') h2_console::prints("", "try: "), h2_console::prints("green", "objcopy --writable-text %s\n", O.path);
      }
   }
   ~h2_source() { h2_e9_reset(source_fp, origin_opcode); }
   void set(void* dstfp) { h2_e9_set(source_fp, dstfp); }
   void save(unsigned char opcode[32]) { h2_e9_save(source_fp, opcode); }
   void reset(unsigned char opcode[32]) { h2_e9_reset(source_fp, opcode); }
   void reset() { h2_e9_reset(source_fp, origin_opcode); }
};

struct h2_sources {
   h2_singleton(h2_sources);
   h2_list sources;

   h2_source* __find(void* fp)
   {
      h2_list_for_each_entry (p, sources, h2_source, x)
         if (p->source_fp == fp)
            return p;
      return nullptr;
   }

   void* __follow(void* fp)
   {
#if defined __arm64__ || defined __aarch64__
#else
      for (int i = 0; i < 1; ++i) {  // follow PLT(Linux) or ILT (Incremental Link Table /Windows)
         if (__find(fp)) break;
         void* next = h2_cxa::follow_jmp(fp, 1);
         if (next == fp) break;
         fp = next;
      }
#endif
      return fp;
   }

   h2_source* get(void* fp) { return __find(__follow(fp)); }

   h2_source* add(void* fp, const char* srcfn, const char* filine)
   {
      void* source_fp = __follow(fp);
      h2_source* source = __find(source_fp);
      if (!source) {
         source = new h2_source(source_fp, srcfn, filine);
         sources.push(source->x);
      }
      source->reference_count++;
      return source;
   }

   void del(h2_source* source)
   {
      if (source && --source->reference_count == 0) {
         source->x.out();
         delete source;
      }
   }
};
// source/stub/h2_stubs.cpp
struct h2_stub : h2_libc {
   h2_list x;
   unsigned char saved_opcode[32];
   void *srcfp, *dstfp;
   h2_source* source;

   h2_stub(void* srcfp_, const char* srcfn, const char* filine) : srcfp(srcfp_)
   {
      source = h2_sources::I().add(srcfp, srcfn, filine);
      if (source) source->save(saved_opcode);
   }
   ~h2_stub()
   {
      if (source) {
         source->reset(saved_opcode);
         h2_sources::I().del(source);
      }
   }
   void stub(void* dstfp_)
   {
      if (source) source->set((dstfp = dstfp_));
   }
};

static inline h2_stub* h2_stubs_get(h2_list& stubs, void* srcfp)
{
   h2_list_for_each_entry (p, stubs, h2_stub, x)
      if (p->srcfp == srcfp)
         return p;
   return nullptr;
}

h2_inline bool h2_stubs::add(h2_list& stubs, void* srcfp, void* dstfp, const char* srcfn, const char* filine)
{
   h2_stub* stub = h2_stubs_get(stubs, srcfp);
   if (!stub) {
      stub = new h2_stub(srcfp, srcfn, filine);
      stubs.push(stub->x);
   }
   stub->stub(dstfp);
   return true;
}

h2_inline void h2_stubs::clear(h2_list& stubs, void* srcfp)
{
   h2_stub* stub = h2_stubs_get(stubs, srcfp);
   if (stub) {
      stub->x.out();
      delete stub;
   }
}

h2_inline void h2_stubs::clear(h2_list& stubs)
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      p->x.out();
      delete p;
   }
}
// source/stub/h2_temporary.cpp
h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* srcfp_) : srcfp(srcfp_)
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) {
      source->save(current_opcode);
      source->reset();
   }
}

h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) source->reset(current_opcode);
}
// source/mock/h2_checkin.cpp
h2_inline h2_fail* h2_checkin::check(size_t index, size_t total, const char* srcfn) const
{
   if (is_satisfied() || is_saturated()) return nullptr;
   h2_line t = srcfn + gray("()") + " expected " + delta(expect(), "green") + " but actually " + delta(actual(), "red,bold") + " called";
   if (1 < total) t += gray(" when ") + index_th(index) + " " + color(expr, "cyan");
   return h2_fail::new_normal(t);
}

h2_inline const char* h2_checkin::actual() const
{
   static char st[64];
   if (call > 0)
      sprintf(st, "%d times", call);
   else
      sprintf(st, "never");
   return st;
}

h2_inline const char* h2_checkin::expect() const
{
   static char st[128];
   if (least == 0) {
      if (most == 0)
         sprintf(st, "never called");
      else if (most == 0x7fffffff)
         sprintf(st, "any number of times");
      else
         sprintf(st, "at most %d times", most);
   } else if (least == most) {
      sprintf(st, "exactly %d times", least);
   } else if (most == 0x7fffffff) {
      sprintf(st, "at least %d times", least);
   } else {  // 0 < least < most < 0x7fffffff
      sprintf(st, "between %d and %d times", least, most);
   }
   return st;
}
// source/mock/h2_mocker.cpp
h2_inline h2_line h2_mocker_base::argument(int seq, const char* def) const
{
   h2_line t;
   for (int i = 0; i < (int)argument_types.size(); ++i)
      t += gray(comma_if(i)) + color(argument_types[i], seq == i ? "red,bold" : def);
   return gray("(") + t + gray(")");
}

h2_inline h2_line h2_mocker_base::signature() const
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(srcfn, "green") + argument(-1, "cyan") + gray(">");
}

h2_inline void h2_mocker_base::mock()
{
   x.out();
   h2_runner::mock(this);
   h2_runner::stub(srcfp, dstfp, srcfn, filine);
}

h2_inline h2_fail* h2_mocker_base::check() const
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(i, checkin_array.size(), srcfn);
      if (fail) fail->seqno = (int)i;
      h2_fail::append_subling(fails, fail);
      h2_runner::asserts();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), filine);
   h2_fail::append_child(fail, fails);
   return fail;
}

h2_inline void h2_mocker_base::failing(h2_fail* fail, int checkin_offset) const
{
   fail->foreach([this, checkin_offset](h2_fail* f, size_t, size_t) {
      f->explain += gray("on ") + (srcfn + argument(f->seqno));
      if (1 < checkin_array.size()) f->explain += gray(" when ") + index_th((size_t)checkin_offset) + " " + color(checkin_array[checkin_offset].expr, "cyan");
   });
   h2_fail* fails = h2_fail::new_normal(signature(), filine);
   h2_fail::append_child(fails, fail);
   h2_runner::failing(fails);
}
// source/mock/h2_mocks.cpp
h2_inline bool h2_mocks::add(h2_list& mocks, void* mocker)
{
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x)
      if (p == (h2_mocker_base*)mocker) return false;
   mocks.push(((h2_mocker_base*)mocker)->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(h2_list& mocks, bool check)
{
   h2_fail* fails = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x) {
      if (check) h2_fail::append_subling(fails, p->check());
      p->reset();
      p->x.out();
   }
   return fails;
}
// source/stdio/h2_stdio.cpp
struct h2_stdio {
   h2_singleton(h2_stdio);
   h2_string* buffer;
   bool stdout_capturable = false, stderr_capturable = false, syslog_capturable = false;
   long long capture_length = 0;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      if (O.verbose >= VerboseNormal || (fd != fileno(stdout) && fd != fileno(stderr))) {
         h2::h2_stub_temporary_restore _((void*)LIBC__write);
         if ((fd == fileno(stdout) || fd == fileno(stderr)) && h2_report::I().backable) {
            LIBC__write(fd, "\n", 1);  // fall printf/cout into new line from report title
            h2_report::I().backable = false;
         }
         LIBC__write(fd == -21371647 ? fileno(stdout) : fd, buf, count);
         if (fd == fileno(stdout) || fd == fileno(stderr))
            I().capture_length += count;
      }
      if ((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr)))
         I().buffer->append((char*)buf, count);
      return (ssize_t)count;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap)
   {
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      return (int)write(fileno(stream), alloca_str, strlen(alloca_str));
   }

   static int fprintf(FILE* stream, const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stream, format, a);
      va_end(a);
      return ret;
   }

   static int fputc(int c, FILE* stream)
   {
      unsigned char t = c;
      int ret = write(fileno(stream), &t, 1);
      return ret == 1 ? c : EOF;
   }

   static int fputs(const char* s, FILE* stream)
   {
      return write(fileno(stream), s, strlen(s));
   }

   static size_t fwrite(const void* ptr, size_t size, size_t nitems, FILE* stream)
   {
      return write(fileno(stream), ptr, size * nitems);
   }

   static int printf(const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stdout, format, a);
      va_end(a);
      return ret;
   }

   static int vprintf(const char* format, va_list ap)
   {
      return vfprintf(stdout, format, ap);
   }

   static int putchar(int c)
   {
      unsigned char t = c;
      write(fileno(stdout), &t, 1);
      return c;
   }

   static int puts(const char* s)
   {
      write(fileno(stdout), s, strlen(s));
      write(fileno(stdout), "\n", 1);
      return 1;
   }

   static void vsyslog(int priority, const char* format, va_list ap)
   {
      if (!I().syslog_capturable) return;
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      I().buffer->append(alloca_str, strlen(alloca_str));
   }

   static void syslog(int priority, const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      vsyslog(priority, format, a);
      va_end(a);
   }

   static void initialize()
   {
      ::setbuf(stdout, 0);  // unbuffered
      I().buffer = new h2_string();
      static h2_list stubs;

      h2_stubs::add(stubs, (void*)::printf, (void*)printf, "printf", H2_FILINE);
      h2_stubs::add(stubs, (void*)::vprintf, (void*)vprintf, "vprintf", H2_FILINE);
      h2_stubs::add(stubs, (void*)::putchar, (void*)putchar, "putchar", H2_FILINE);
      h2_stubs::add(stubs, (void*)::puts, (void*)puts, "puts", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fprintf, (void*)fprintf, "fprintf", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::vfprintf, (void*)vfprintf,  "vfprintf",H2_FILINE);
      h2_stubs::add(stubs, (void*)::fputc, (void*)fputc, "fputc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::putc, (void*)fputc, "fputc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fputs, (void*)fputs, "fputs", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fwrite, (void*)fwrite, "fwrite", H2_FILINE);
#if defined __GNUC__
      struct streambuf : public std::streambuf {
         FILE* f;
         int sync() override { return 0; }
         int overflow(int c) override { return (c != EOF) && h2_stdio::fputc(c, f), 0; }
         streambuf(FILE* _f) : f(_f) { setp(nullptr, 0); }
      };
      static streambuf sb_out(stdout);
      static streambuf sb_err(stderr);
      std::cout.rdbuf(&sb_out); /* internal fwrite() called, but */
      std::cerr.rdbuf(&sb_err);
      std::clog.rdbuf(&sb_err); /* print to stderr */
#endif
      h2_stubs::add(stubs, (void*)LIBC__write, (void*)write, "write", H2_FILINE);
#if !defined _WIN32
      h2_stubs::add(stubs, (void*)::syslog, (void*)syslog, "syslog", H2_FILINE);
      h2_stubs::add(stubs, (void*)::vsyslog, (void*)vsyslog, "vsyslog", H2_FILINE);
#endif
   }

   void start_capture(bool stdout_capturable_, bool stderr_capturable_, bool syslog_capturable_)
   {
      stdout_capturable = stdout_capturable_;
      stderr_capturable = stderr_capturable_;
      syslog_capturable = syslog_capturable_;
      buffer->clear();
   }

   const char* stop_capture()
   {
      stdout_capturable = stderr_capturable = syslog_capturable = false;
      buffer->push_back('\0');
      return buffer->c_str();
   }
};

h2_inline h2_cout::h2_cout(h2_matcher<const char*> m_, const char* e_, const char* type_, const char* filine_) : filine(filine_), m(m_), e(e_), type(type_)
{
   bool all = !strlen(type);
   h2_stdio::I().start_capture(all || get_keyvalue(type, "out"), all || get_keyvalue(type, "err"), all || get_keyvalue(type, "syslog"));
}

h2_inline h2_cout::~h2_cout()
{
   h2_runner::asserts();
   h2_fail* fail = m.matches(h2_stdio::I().stop_capture(), 0);
   if (fail) {
      fail->filine = filine;
      fail->assert_type = "OK";
      fail->e_expression = e;
      fail->a_expression = "";
      fail->explain = "COUT";
      h2_runner::failing(fail);
   }
}
// source/net/h2_dns.cpp
struct h2_name : h2_libc {
   h2_list x, y;
   h2_string name;
   h2_vector<h2_string> resolves;
   h2_name(const char* _hostname) : name(_hostname) {}
};

struct h2_resolver {
   h2_singleton(h2_resolver);
   h2_list dnses;

   static bool inet_addr(const char* str, struct sockaddr_in* addr)
   {
      struct sockaddr_in a;
      if (!addr) addr = &a;
      return 1 == inet_pton(AF_INET, str, &addr->sin_addr);
   }

   h2_name* find(const char* hostname)
   {
      h2_list_for_each_entry (p, dnses, h2_name, y)
         if (p->name == "*" || p->name == hostname)
            return p;
      return nullptr;
   }

   static int h2_stdcall getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res)
   {
      h2_name* name = I().find(hostname);
      if (!name) return -1;

      static struct addrinfo addrinfos[32];
      static struct sockaddr_in sockaddrs[32];
      memset(addrinfos, 0, sizeof(addrinfos));
      memset(sockaddrs, 0, sizeof(sockaddrs));

      struct addrinfo** pp = res;
      for (size_t i = 0; i < name->resolves.size(); ++i) {
         struct addrinfo* a = &addrinfos[i];
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(name->resolves[i].c_str(), b)) {
            a->ai_addr = (struct sockaddr*)b;
            a->ai_addrlen = sizeof(struct sockaddr_in);
         } else
            a->ai_canonname = (char*)name->resolves[i].c_str();
         if (hints) {
            a->ai_family = hints->ai_family;
            a->ai_socktype = hints->ai_socktype;
            a->ai_protocol = hints->ai_protocol;
         } else {
            a->ai_family = AF_INET;
            a->ai_socktype = SOCK_STREAM;
            a->ai_protocol = IPPROTO_TCP;
         }
         a->ai_next = nullptr;
         *pp = a, pp = &a->ai_next;
      }
      *pp = nullptr;
      return 0;
   }

   static void h2_stdcall freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* h2_stdcall gethostbyname(char* hostname)
   {
      h2_name* name = I().find(hostname);
      if (!name) return nullptr;

      static struct sockaddr_in sockaddrs[32];
      static char* h_aliases[32];
      static char* h_addr_list[32];
      static struct hostent h;

      h.h_name = hostname;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      memset(sockaddrs, 0, sizeof(sockaddrs));
      memset(h_aliases, 0, sizeof(h_aliases));
      memset(h_addr_list, 0, sizeof(h_addr_list));

      for (size_t i = 0, a = 0, c = 0; i < name->resolves.size(); ++i) {
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(name->resolves[i].c_str(), b))
            h_addr_list[a++] = (char*)&b->sin_addr;
         else
            h_aliases[c++] = (char*)name->resolves[i].c_str();
      }
      return &h;
   }

   h2_list stubs;
   h2_resolver()
   {
      h2_stubs::add(stubs, (void*)::getaddrinfo, (void*)getaddrinfo, "getaddrinfo", H2_FILINE);
      h2_stubs::add(stubs, (void*)::freeaddrinfo, (void*)freeaddrinfo, "freeaddrinfo", H2_FILINE);
      h2_stubs::add(stubs, (void*)::gethostbyname, (void*)gethostbyname, "gethostbyname", H2_FILINE);
   }
   ~h2_resolver() { h2_stubs::clear(stubs); }
};

h2_inline void h2_dnses::add(h2_list& dnses, h2_list& name)
{
   dnses.push(name);
}

h2_inline void h2_dnses::clear(h2_list& dnses)
{
   h2_list_for_each_entry (p, dnses, h2_name, x) {
      p->x.out();
      p->y.out();
      delete p;
   }
}

h2_inline void h2_dns::setaddrinfo(int n, ...)
{
   if (n == 0) return;
   va_list a, b;

   const char *hostname = "*", *p;
   va_start(a, n);
   for (int i = 0; i < n; ++i)
      if ((p = va_arg(a, const char*)))
         if (!h2_resolver::inet_addr(p, nullptr))
            if (strlen(hostname) < 2 || strlen(p) < strlen(hostname))
               hostname = p;
   va_end(a);

   h2_name* name = new h2_name(hostname);
   va_start(b, n);
   for (int i = 0; i < n; ++i)
      if ((p = va_arg(b, const char*)))
         if (strcmp(hostname, p))
            name->resolves.push_back(p);
   va_end(b);

   h2_resolver::I().dnses.push(name->y);
   if (h2_runner::I().current_case)
      h2_dnses::add(h2_runner::I().current_case->dnses, name->x);
}

h2_inline void h2_dns::initialize()
{
   setaddrinfo(1, "127.0.0.1");
}
// source/net/h2_socket.cpp
struct h2_socket {
   h2_singleton(h2_socket);

   static bool is_block(int sockfd)
   {
#if defined _WIN32
      return true;
#else
      return !(fcntl(sockfd, F_GETFL) & O_NONBLOCK);
#endif
   }
   static bool set_block(int sockfd, bool block)
   {
#if defined _WIN32
      u_long op = block ? 0 : 1;
      if (ioctlsocket(sockfd, FIONBIO, &op) == SOCKET_ERROR) {
         return false;
      }
      return true;
#else
      int flags = fcntl(sockfd, F_GETFL);
      if (flags < 0) {
         return false;
      }

      if (block) {
         if ((flags & O_NONBLOCK) != 0) {
            flags ^= O_NONBLOCK;
         }
      } else {
         flags |= O_NONBLOCK;
      }
      if (fcntl(sockfd, F_SETFL, flags) < 0) {
         return false;
      }

      return true;
#endif
   }

   static void iport_parse(const char* str, struct sockaddr_in* addr)
   {
      char temp[1024];
      strcpy(temp, str);
      addr->sin_family = AF_INET;
      addr->sin_port = 0;
      char* colon = strchr(temp, ':');
      if (colon) {
         *colon = '\0';
         addr->sin_port = htons(atoi(colon + 1));
      }
      inet_pton(AF_INET, temp, &addr->sin_addr);
   }

   static const char* iport_tostring(struct sockaddr_in* addr, char* str)
   {
      char t[256];
      sprintf(str, "%s:%d", inet_ntop(AF_INET, &addr->sin_addr, t, sizeof(t)), ntohs(addr->sin_port));
      return str;
   }

   struct temporary_noblock : h2_once {
      int sockfd, flags;
      temporary_noblock(int sockfd_) : sockfd(sockfd_)
      {
         flags = is_block(sockfd);
         set_block(sockfd, false);
      }
      ~temporary_noblock() { set_block(sockfd, flags); }
   };

   static const char* getsockname(int sockfd, char* s, struct sockaddr_in* a = nullptr)
   {
      struct sockaddr_in b;
      if (!a) a = &b;
      socklen_t l = sizeof(struct sockaddr_in);
      ::getsockname(sockfd, (struct sockaddr*)a, &l);
      return iport_tostring(a, s);
   }

   static h2_packet* read_incoming(int sockfd)
   {
      bool block = is_block(sockfd);
      const char* local = getsockname(sockfd, (char*)alloca(64));

      do {
         h2_list_for_each_entry (p, I().incoming, h2_packet, x) {
            if (h2_pattern::wildcard_match(p->to.c_str(), local)) {
               p->x.out();
               return p;
            }
         }
         if (block) h2_sleep(100);
      } while (block);
      return nullptr;
   }

   static int h2_stdcall accept(int socket, struct sockaddr* address, socklen_t* address_len)
   {
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      iport_parse(tcp->from.c_str(), (struct sockaddr_in*)address);
      *address_len = sizeof(struct sockaddr_in);

      int fd = (int)::socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in a;
      const char* c = getsockname(socket, (char*)alloca(64), &a);
      ::bind(fd, (struct sockaddr*)&a, sizeof(a));
      I().sockets.push_back({fd, c, tcp->from.c_str()});
      if (tcp->data.size())
         I().incoming.push(tcp->x);
      else
         delete tcp;

      return fd;
   }

   static int h2_stdcall connect(int socket, const struct sockaddr* address, socklen_t address_len)
   {
      I().sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      if (tcp->data.size())
         I().incoming.push(tcp->x);
      else
         delete tcp;
      return 0;
   }

   static ssize_t h2_stdcall send(int socket, const void* buffer, size_t length, int flags)
   {
      I().put_outgoing(socket, (const char*)buffer, length);
      return (ssize_t)length;
   }
   static ssize_t h2_stdcall recv(int socket, void* buffer, size_t length, int flags)
   {
      ssize_t ret = 0;
      h2_packet* tcp = read_incoming(socket);
      if (tcp) {
         ret = (ssize_t)tcp->data.copy((char*)buffer, tcp->data.size(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t h2_stdcall sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len)
   {
      I().put_outgoing(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return (ssize_t)length;
   }
   static ssize_t h2_stdcall recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len)
   {
      ssize_t ret = 0;
      h2_packet* udp = read_incoming(socket);

      if (udp) {
         ret = (ssize_t)udp->data.copy((char*)buffer, udp->data.size(), 0);
         iport_parse(udp->from.c_str(), (struct sockaddr_in*)address);
         *address_len = sizeof(struct sockaddr_in);
         delete udp;
      }
      return ret;
   }
#if !defined _WIN32
   static ssize_t h2_stdcall sendmsg(int socket, const struct msghdr* message, int flags)
   {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
   static ssize_t h2_stdcall recvmsg(int socket, struct msghdr* message, int flags)
   {
      return recvfrom(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, &message->msg_namelen);
   }
#endif

   h2_list stubs;

   struct socket {
      int fd;
      h2_string from, to;
      socket(int fd_, const char* from_, const char* to_) : fd(fd_), from(from_), to(to_) {}
   };

   h2_vector<socket> sockets;

   void put_outgoing(const char* from, const char* to, const char* data, size_t size)
   {
      strcpy(last_to, to);
      outgoing.push_back((new h2_packet(from, to, data, size))->x);
   }
   void put_outgoing(int fd, const char* data, size_t size)
   {
      char from[128] = "", to[128] = "";
      for (auto& t : sockets) {
         if (t.fd == fd) {
            strcpy(from, t.from.c_str());
            strcpy(to, t.to.c_str());
            break;
         }
      }
      put_outgoing(from, to, data, size);
   }
   void put_incoming(const char* from, const char* to, const char* data, size_t size)
   {
      incoming.push_back((new h2_packet(strlen(from) ? from : last_to, to, data, size))->x);
   }

   char last_to[128];
   h2_list incoming, outgoing;

   void start()
   {
      strcpy(last_to, "0.0.0.0:0");

      h2_stubs::add(stubs, (void*)::sendto, (void*)sendto, "sendto", H2_FILINE);
      h2_stubs::add(stubs, (void*)::recvfrom, (void*)recvfrom, "recvfrom", H2_FILINE);
#if !defined _WIN32
      h2_stubs::add(stubs, (void*)::sendmsg, (void*)sendmsg, "sendmsg", H2_FILINE);
      h2_stubs::add(stubs, (void*)::recvmsg, (void*)recvmsg, "recvmsg", H2_FILINE);
#endif
      h2_stubs::add(stubs, (void*)::send, (void*)send, "send", H2_FILINE);
      h2_stubs::add(stubs, (void*)::recv, (void*)recv, "recv", H2_FILINE);
      h2_stubs::add(stubs, (void*)::accept, (void*)accept, "accept", H2_FILINE);
      h2_stubs::add(stubs, (void*)::connect, (void*)connect, "connect", H2_FILINE);
   }
   void stop()
   {
      h2_stubs::clear(stubs);
      h2_list_for_each_entry (p, incoming, h2_packet, x) {
         p->x.out();
         delete p;
      }
      h2_list_for_each_entry (p, outgoing, h2_packet, x) {
         p->x.out();
         delete p;
      }
   }
};

static inline void parse_iport(const char* s, char* iport)
{
   for (const char* p = s; p && *p; ++p) {
      if (::isdigit(*p) || *p == '.' || *p == ':' || *p == '*' || *p == '?') {
         *iport++ = *p;
         *iport = '\0';
      } else {
         if (!(::isspace(*p) || *p == '\"')) break;
      }
   }
}

h2_inline void h2_sock::inject(const void* packet, size_t size, const char* attributes)
{
   char from[256] = "", to[256] = "*";
   parse_iport(get_keyvalue(attributes, "from"), from);
   parse_iport(get_keyvalue(attributes, "to"), to);
   h2_socket::I().put_incoming(from, to, (const char*)packet, size);
}

h2_inline void h2_sock::clear()
{
   h2_socket::I().stop();
}

h2_inline h2_sock::h2_sock()
{
   h2_socket::I().start();
}

h2_inline h2_sock::~h2_sock()
{
   h2_socket::I().stop();
}

template <typename M1, typename M2, typename M3, typename M4>
h2_inline void h2_sock::check(const char* filine, const char* e, M1 from, M2 to, M3 payload, M4 size)
{
   h2_runner::asserts();
   h2_packet* p = h2_list_pop_entry(h2_socket::I().outgoing, h2_packet, x);;
   if (!p) {
      h2_line t = "Outgoing packet miss Ptx(";
      t.printf("green", "%s", e).printf("", ")");
      h2_runner::failing(h2_fail::new_normal(t, filine));
      return;
   }
   h2_fail* fails = nullptr;
   h2_fail* fail_from = h2_matcher_cast<const char*>(We(from)).matches(p->from.c_str());
   if (fail_from) {
      fail_from->explain = "from address";
      h2_fail::append_subling(fails, fail_from);
   }
   h2_fail* fail_to = h2_matcher_cast<const char*>(We(to)).matches(p->to.c_str());
   if (fail_to) {
      fail_to->explain = "to address";
      h2_fail::append_subling(fails, fail_to);
   }
   h2_fail* fail_payload = h2_matcher_cast<const unsigned char*>(payload).matches((unsigned char*)p->data.data());
   if (fail_payload) {
      fail_payload->explain = "payload";
      h2_fail::append_subling(fails, fail_payload);
   }
   h2_fail* fail_size = h2_matcher_cast<const int>(size).matches(p->data.size());
   if (fail_size) {
      fail_size->explain = "payload length";
      h2_fail::append_subling(fails, fail_size);
   }

   if (fails) {
      h2_line t = "Outgoing packet unexpected Ptx(";
      t.printf("green", "%s", e).printf("", ")");
      h2_fail* fail = h2_fail::new_normal(t, filine);
      h2_fail::append_child(fail, fails);
      h2_runner::failing(fail);
   }
}
// source/core/h2_test.cpp
static inline void __split_describe(const char* describe, char* name_buf, char* tags_buf)
{
   strcpy(name_buf, describe);
   strcpy(tags_buf, "");

   const char* p = describe;
   for (bool quote = false; *p; ++p) {
      if (*p == '\"') quote = !quote;
      if (!quote && *p == '[') break;
   }
   if (*p != '[') return;  // left brace
   const char* q = p;
   for (bool quote = false; *q; ++q) {
      if (*q == '\"') quote = !quote;
      if (!quote && *q == ']') break;
   }
   if (*q != ']') return;  // right brace

   strncpy(tags_buf, p + 1, (size_t)(q - p - 1));
   tags_buf[(size_t)(q - p - 1)] = '\0';

   strcpy(&name_buf[(size_t)(p - describe)], q + 1);
}

h2_inline h2_test::h2_test(const char* filine_, const char* file_, int line_, const char* describe_) : filine(filine_), file(h2_basefile(file_)), line(line_), describe(describe_)
{
   if (describe) {
      __split_describe(describe, name_buf, tags_buf);
      name = strip_left(name_buf);
      for (char* t = strtok(tags_buf, " ,"); t; t = strtok(nullptr, " ,"))
         h2_array_append(tags, t);
   }
}

h2_inline bool h2_test::tagged(const char* tag_pattern) const
{
   for (int i = 0; tags[i]; ++i) {
      if (!strcasecmp(tags[i], tag_pattern)) return true;
      if (strcspn(tag_pattern, "?*[]") < strlen(tag_pattern) && h2_pattern::wildcard_match(tag_pattern, tags[i], true)) return true;
   }
   return false;
}
// source/core/h2_case.cpp
h2_inline void h2_case::clear()
{
   h2_sock::clear();
   h2_memory::hook();
   if (fails) delete fails;
   fails = nullptr;
   stats.clear();
}

h2_inline void h2_case::prev_setup()
{
   failed = false;
   h2_memory::stack::push(filine);
}

h2_inline void h2_case::post_cleanup()
{
   stats.footprint = h2_memory::stack::footprint();
   h2_dnses::clear(dnses);
   h2_stubs::clear(stubs);
   failing(h2_mocks::clear(mocks, true), true, O.continue_assert);
   failing(h2_memory::stack::pop(), true, O.continue_assert);
}

h2_inline void h2_case::failing(h2_fail* fail, bool defer, bool append)
{
   if (fail) {
      failed = true;
      if (fails && !append)
         delete fail;
      else
         h2_fail::append_subling(fails, fail);
      if (!defer) ::longjmp(fail_hole, 1);
   }
}
// source/core/h2_suite.cpp
h2_inline h2_suite::h2_suite(const char* filine_, const char* file_, int line_, const char* describe_, void (*test_code_)(h2_suite*, h2_case*)) : h2_test(filine_, file_, line_, describe_), test_code(test_code_)
{
   memset(cleanup_hole, 0, sizeof(jmp_buf));
   h2_runner::I().suites.push_back(x);
}

h2_inline void h2_suite::clear()
{
   stats.clear();
}

h2_inline void h2_suite::setup()
{
   h2_memory::stack::push(filine);
}

h2_inline void h2_suite::cleanup()
{
   h2_stubs::clear(stubs);
   h2_mocks::clear(mocks, false);
   stats.footprint = h2_memory::stack::footprint();
   h2_memory::stack::pop();
}

h2_inline void h2_suite::enumerate()
{
   test_code(this, nullptr); /* enumerate case by static local h2_case variable inside of h2_suite_test_CmLn() */
}

h2_inline void h2_suite::test(h2_case* c)
{
   c->prev_setup();
   try {
      test_code(this, c); /* include Setup(); c->post_setup() and c->prev_cleanup(); Cleanup() */
   } catch (...) {
      c->failing(h2_fail::new_exception("was thrown but uncaught", h2_exception::I().last_type, h2_exception::I().last_bt), true, O.continue_assert);
   }
   c->post_cleanup();
}

h2_inline h2_suite::registor::registor(h2_suite* s, h2_case* c)
{
   static int seq = 0x0fffffff;
   s->cases.push_back(c->x);
   s->seq = c->seq = ++seq;
}

h2_inline h2_suite::cleaner::~cleaner()
{
   static const unsigned char zero[sizeof(jmp_buf)] = {0};
   if (memcmp((const void*)thus->cleanup_hole, (const void*)zero, sizeof(jmp_buf)))
      ::longjmp(thus->cleanup_hole, 1);
}
// source/core/h2_filter.cpp
static inline bool match_names(const char* const patterns[], const char* subject)
{
   for (int i = 0; patterns[i]; ++i) {
      if (strcasestr(subject, patterns[i])) return true;
      if (strcspn(patterns[i], "?*[]") < strlen(patterns[i]) && h2_pattern::wildcard_match(patterns[i], subject, true)) return true;
   }
   return false;
}

static inline bool match_tags(const char* const patterns[], const h2_test* test)
{
   for (int i = 0; patterns[i]; ++i)
      if (test->tagged(patterns[i])) return true;
   return false;
}

static inline bool __filter(const h2_test* s, const h2_test* c, const char* const includes[], const char* const excludes[], bool tags_filter)
{
   if (tags_filter) {
      if (includes[0])
         if (!(match_tags(includes, s) || c && match_tags(includes, c)))  // priority && > ||
            return true;
      if (excludes[0])
         if (match_tags(excludes, s) || c && match_tags(excludes, c))
            return true;
   } else {
      if (includes[0])
         if (!(match_names(includes, s->name) || c && match_names(includes, c->name) || match_names(includes, c ? c->filine : s->filine)))
            return true;
      if (excludes[0])
         if (match_names(excludes, s->name) || c && match_names(excludes, c->name) || match_names(excludes, c ? c->filine : s->filine))
            return true;
   }
   return false;
}

static inline void h2_filter_suite(h2_suite* s)
{
   s->filtered = __filter(s, nullptr, O.includes, O.excludes, O.tags_filter);
}

static inline void h2_filter_case(h2_suite* s, h2_case* c)
{
   c->filtered = __filter(s, c, O.includes, O.excludes, O.tags_filter);
   if (!c->filtered) s->filtered = false;
}
// source/core/h2_runner.cpp
static inline void drop_last_order() { ::remove(".last_order"); }

static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, " file: %s\nsuite: %s\n case: %s\n test: %s\n\n", c->file, s->name, c->name, c->failed ? "failed" : "passed");
   ::fclose(f);
}

static inline void __find_case(h2_list& suites, char* filename, char* suitename, char* casename, bool failed)
{
   static int seq = 0;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, s->name))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name) && !strcmp(filename, c->file)) {
               s->seq = c->seq = ++seq;
               if (failed) c->last_failed = true;
            }
}

static inline bool fgets(FILE* f, char* b)
{
   if (!::fgets(b, 512, f)) return false;
   b[strlen(b) - 1] = '\0'; /* remove \n in save_last_order */
   return true;
}

static inline int sequence_last_order(h2_list& suites)
{
   int count = 0;
   char filename[512], suitename[512], casename[512], test[512], br[512];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (fgets(f, filename) && fgets(f, suitename) && fgets(f, casename) && fgets(f, test) && fgets(f, br)) {
      __find_case(suites, filename + 7, suitename + 7, casename + 7, !strcmp("failed", test + 7));
      count++;
   }
   ::fclose(f);
   return count;
}

h2_inline void h2_runner::enumerate()
{
   double dots = 0, dps = (h2_console::width() - 11) / (suites.count() + 0.000001);
   if (O.progressing) h2_console::prints("dark gray", "Collecting ");
   h2_list_for_each_entry (s, i, suites, h2_suite, x) {
      if (O.progressing)
         for (; dots <= i * dps; dots++) h2_console::prints("dark gray", ".");

      for (int i = 0; global_suite_setups[i]; ++i) global_suite_setups[i]();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (int i = 0; global_suite_cleanups[i]; ++i) global_suite_cleanups[i]();
   }
   if (O.progressing) h2_console::prints("", "\33[2K\r");
}

h2_inline void h2_runner::filter()
{
   h2_list_for_each_entry (s, i, suites, h2_suite, x) {
      h2_filter_suite(s);
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         h2_filter_case(s, c);
   }
}

template <typename T>
struct shuffle_comparison {
   static int seq(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, T, x)->seq - h2_list_entry(b, T, x)->seq;
   }
   static int name(h2_list* a, h2_list* b)
   {
      return strcasecmp(h2_list_entry(a, T, x)->name, h2_list_entry(b, T, x)->name);
   }
   static int file(h2_list* a, h2_list* b)
   {
      int t = strcasecmp(h2_list_entry(a, T, x)->file, h2_list_entry(b, T, x)->file);
      return t != 0 ? t : h2_list_entry(a, T, x)->line - h2_list_entry(b, T, x)->line;
   }
   static int cmp(h2_list* a, h2_list* b)
   {
      int reverse = O.shuffles & ShuffleReverse ? -1 : 1;
      if (O.shuffles & ShuffleName) return name(a, b) * reverse;
      if (O.shuffles & ShuffleFile) return file(a, b) * reverse;
      return seq(a, b) * reverse;  // Random or Code Declare
   }
};

h2_inline void h2_runner::shuffle()
{
   if ((lasts = sequence_last_order(suites))) {
      suites.sort(shuffle_comparison<h2_suite>::seq);
      h2_list_for_each_entry (s, suites, h2_suite, x)
         s->cases.sort(shuffle_comparison<h2_case>::seq);
      return;  // run in last order if last failed
   }

   if (O.shuffles) {
      ::srand(::clock());
      if (O.shuffles & ShuffleRandom)
         h2_list_for_each_entry (s, suites, h2_suite, x)
            h2_list_for_each_entry (c, s->cases, h2_case, x)
               s->seq = c->seq = ::rand();

      suites.sort(shuffle_comparison<h2_suite>::cmp);
      h2_list_for_each_entry (s, suites, h2_suite, x)
         s->cases.sort(shuffle_comparison<h2_case>::cmp);
   }
}

h2_inline int h2_runner::main(int argc, const char** argv)
{
#if defined _MSC_VER
   SetConsoleOutputCP(65001);  // set console code page to utf-8
   SymInitialize(GetCurrentProcess(), NULL, TRUE);
#endif
   h2_option::I().parse(argc, argv);
   h2_report::initialize();
   h2_memory::initialize();
   h2_exception::initialize();
   h2_stdio::initialize();
   h2_dns::initialize();

   for (int i = 0; global_setups[i]; ++i) global_setups[i]();
   enumerate();
   filter();

   h2_report::I().on_runner_start(this);
   for (rounds = 0; rounds < O.run_rounds; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (int i = 0; global_suite_setups[i]; ++i) global_suite_setups[i]();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if ((0 < O.break_after_fails && O.break_after_fails <= stats.failed) || (O.only_last_failed && !c->last_failed))
               c->ignored = true;
            if (c->ignored)
               stats.ignored++, s->stats.ignored++;
            else if (c->filtered)
               stats.filtered++, s->stats.filtered++;
            else if (c->todo)
               stats.todo++, s->stats.todo++;

            current_case = c;
            h2_report::I().on_case_start(s, c);
            if (!O.lists && !c->todo && !c->filtered && !c->ignored) {
               for (int i = 0; global_case_setups[i]; ++i) global_case_setups[i]();
               s->test(c);
               for (int i = 0; global_case_cleanups[i]; ++i) global_case_cleanups[i]();
               c->failed ? (stats.failed++, s->stats.failed++) : (stats.passed++, s->stats.passed++);
            }
            h2_report::I().on_case_endup(s, c);
            c->clear();
         }
         s->cleanup();
         for (int i = 0; global_suite_cleanups[i]; ++i) global_suite_cleanups[i]();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      if (stats.failed == 0)
         drop_last_order();
      else if (lasts == 0)
         save_last_order(suites);
   }
   h2_report::I().on_runner_endup(this);
   for (int i = 0; global_cleanups[i]; i++) global_cleanups[i]();

   h2_stubs::clear(stubs);
   h2_mocks::clear(mocks, false);
   h2_memory::finalize();
   return O.quit_exit_code ? stats.failed : 0;
}

h2_inline void h2_runner::stub(void* srcfp, void* dstfp, const char* srcfn, const char* filine)
{
   if (!srcfp || !dstfp) return;
   if (h2_runner::I().current_case)
      h2_stubs::add(h2_runner::I().current_case->stubs, srcfp, dstfp, srcfn, filine);
   else if (h2_runner::I().current_suite)
      h2_stubs::add(h2_runner::I().current_suite->stubs, srcfp, dstfp, srcfn, filine);
   else
      h2_stubs::add(h2_runner::I().stubs, srcfp, dstfp, srcfn, filine);
}

h2_inline void h2_runner::unstub(void* srcfp)
{
   if (!srcfp) return;
   if (h2_runner::I().current_case)
      h2_stubs::clear(h2_runner::I().current_case->stubs, srcfp);
   else if (h2_runner::I().current_suite)
      h2_stubs::clear(h2_runner::I().current_suite->stubs, srcfp);
   else
      h2_stubs::clear(h2_runner::I().stubs, srcfp);
}

h2_inline void h2_runner::mock(void* mocker)
{
   if (h2_runner::I().current_case)
      h2_mocks::add(h2_runner::I().current_case->mocks, mocker);
   else if (h2_runner::I().current_suite)
      h2_mocks::add(h2_runner::I().current_suite->mocks, mocker);
   else
      h2_mocks::add(h2_runner::I().mocks, mocker);
}

h2_inline void h2_runner::failing(h2_fail* fail)
{
   if (!fail) return;
   if (O.debugger_trap && h2_attach_debugger()) h2_raise_trap();
   if (h2_runner::I().current_case) h2_runner::I().current_case->failing(fail, O.continue_assert, true);
}

h2_inline void h2_runner::asserts()
{
   if (h2_runner::I().current_case) h2_runner::I().current_case->stats.asserts += 1;
   if (h2_runner::I().current_suite) h2_runner::I().current_suite->stats.asserts += 1;
   h2_runner::I().stats.asserts += 1;
}
// source/assert/h2_assert.cpp
static inline const char* __find_compare(const char* expression, const char* op)
{
   bool quote = false;
   if (strlen(op) == 2) {
      for (const char* p = expression; *p; ++p) {
         if (*p == '\"') quote = !quote;
         if (!quote && !strncmp(op, p, 2)) return p;
      }
   } else {
      int stacks = 0;
      if (*op == '>')
         for (const char* p = expression; *p; ++p) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '<') ++stacks;
            if (!quote && *p == '>' && 0 == stacks--) return p;
         }
      if (*op == '<')
         for (const char* p = expression + strlen(expression); expression <= p; --p) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '>') ++stacks;
            if (!quote && *p == '<' && 0 == stacks--) return p;
         }
   }
   return nullptr;
}

static inline void __split_compare(const char* expression, const char* op, h2_string& e_expression, h2_string& a_expression)
{
   const char* p = __find_compare(expression, op);
   if (p) {
      e_expression.assign(expression, strip_right(expression, p) - expression);
      a_expression = strip_left(p + strlen(op));
   }
}

h2_inline h2_ostringstream& h2_assert::stash(h2_fail* fail, const char* assert_type, const char* assert_op)
{
   h2_runner::asserts();
   fails = fail;
   if (fail && fail->subling_next) {
      fails = h2_fail::new_unexpect();
      h2_fail::append_child(fails, fail);
   }
   if (fails) {
      fails->assert_type = assert_type;
      fails->assert_op = assert_op;
   }
   return oss;
}

h2_inline void h2_assert::failing(const char* e_expression, const char* a_expression, const char* filine)
{
   if (fails) {
      fails->user_explain = oss.str().c_str();
      fails->filine = filine;
      fails->e_expression = e_expression;
      fails->a_expression = a_expression;
      if (*fails->assert_op != ',') __split_compare(a_expression, fails->assert_op, fails->e_expression, fails->a_expression);
   }
   h2_runner::failing(fails);
}
// source/assert/h2_timer.cpp
h2_inline h2_timer::h2_timer(int ms_, const char* filine_) : filine(filine_), cpu_ms(ms_)
{
   start = ::clock();
}

h2_inline h2_timer::~h2_timer()
{
   h2_runner::asserts();
   double delta = (::clock() - start) * 1000.0 / CLOCKS_PER_SEC;
   if (cpu_ms < delta) {
      h2_line line = "performance expect < ";
      line.printf("green", "%d", cpu_ms).printf("", " ms, but actually cost ").printf("red", "%d", (int)delta).printf("", " ms");
      h2_runner::failing(h2_fail::new_normal(line, filine));
   }
}
// source/render/h2_layout.cpp
struct h2_layout {
   static h2_lines line_break(const h2_line& line, size_t width)
   {
      h2_lines lines;
      h2_string current_style;
      h2_line wrap;

      for (auto& word : line) {
         if (h2_color::isctrl(word.c_str())) {  // + - style , issue
            wrap.push_back(word);
            current_style = word;
            continue;
         }
         for (auto& c : word.disperse()) {
            if (width < wrap.width() + c.width()) {
               lines.push_back(wrap.padding(width - wrap.width()));
               wrap.clear();
               if (current_style.size()) wrap.push_back(current_style);
            }
            wrap.push_back(c);
         }
      }
      lines.push_back(wrap.padding(width - wrap.width()));
      return lines;
   }

   static h2_lines lines_merge(const h2_lines& left_lines, const h2_lines& right_lines, size_t left_width, size_t right_width, int step, char scale, size_t seq_width)
   {
      h2_lines lines;
      char seq_fmt[32];
      sprintf(seq_fmt, "%%%d%c┊ ", (int)seq_width, scale);
      const h2_string left_empty(left_width, ' '), right_empty(right_width, ' ');
      for (size_t i = 0; i < std::max(left_lines.size(), right_lines.size()); ++i) {
         auto left_wrap_lines = line_break(i < left_lines.size() ? left_lines[i] : left_empty, left_width);
         auto right_wrap_lines = line_break(i < right_lines.size() ? right_lines[i] : right_empty, right_width);
         for (size_t j = 0; j < std::max(left_wrap_lines.size(), right_wrap_lines.size()); ++j) {
            h2_line line;
            if (step) {
               if (j == 0)
                  line.printf("dark gray", seq_fmt, step * i);
               else
                  line.indent(seq_width + 2);
            }
            line += j < left_wrap_lines.size() ? left_wrap_lines[j].brush("reset") : color(left_empty, "reset");
            line.printf("dark gray", j < left_wrap_lines.size() - 1 ? "\\│ " : " │ ");
            line += j < right_wrap_lines.size() ? right_wrap_lines[j].brush("reset") : color(right_empty, "reset");
            line.printf("dark gray", j < right_wrap_lines.size() - 1 ? "\\" : " ");
            lines.push_back(line);
         }
      }
      return lines;
   }

   static h2_lines prefix_break(const h2_line& line, const h2_line& title, size_t width)
   {
      h2_lines lines = line_break(line, width - title.width());

      for (size_t i = 0; i < lines.size(); ++i) {
         if (i == 0)
            lines[i] = title + lines[i];
         else
            lines[i].indent(title.width());
      }
      return lines;
   }

   static h2_lines split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, size_t step, char scale, size_t width)
   {
      size_t seq_width = number_strlen(step * std::max(left_lines.size(), right_lines.size()), scale == 'x' ? 16 : 10);
      size_t valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

      size_t left_width = std::max(left_lines.width(), strlen(left_title));
      size_t right_width = std::max(right_lines.width(), strlen(right_title));

      if (left_width < valid_width / 2)
         right_width = std::min(valid_width - left_width, right_width);
      else if (right_width < valid_width / 2)
         left_width = std::min(valid_width - right_width, left_width);
      else
         left_width = right_width = valid_width / 2;

      h2_line title = (step ? h2_string(seq_width + 2, ' ') : "") + h2_string(left_title).center(left_width) + "   " + h2_string(right_title).center(right_width);
      h2_lines lines = {title.brush("dark gray")};

      return lines += lines_merge(left_lines, right_lines, left_width, right_width, step, scale, seq_width);
   }

   static h2_lines unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
   {
      h2_lines lines;
      h2_line up_title_line = gray(up_title) + color("> ", "green");
      h2_line down_title_line = gray(down_title) + color("> ", "red");

      h2_lines up_lines = line_break(up_line, width - up_title_line.width());
      h2_lines down_lines = line_break(down_line, width - down_title_line.width());

      for (size_t i = 0; i < std::max(up_lines.size(), down_lines.size()); ++i) {
         if (i < up_lines.size()) lines.push_back(up_title_line + up_lines[i]);
         if (i < down_lines.size()) lines.push_back(down_title_line + down_lines[i]);
      }
      return lines;
   }

   static h2_lines seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
   {
      h2_line up_title_line = gray(up_title) + color("> ", "green");
      h2_line down_title_line = gray(down_title) + color("> ", "red");
      h2_lines lines = prefix_break(up_line, up_title_line, width);
      return lines += prefix_break(down_line, down_title_line, width);
   }
};
// source/render/h2_failure.cpp
h2_inline void h2_fail::append_subling(h2_fail*& fail, h2_fail* nf)
{
   if (!fail) {
      fail = nf;
   } else {
      h2_fail** pp = &fail->subling_next;
      while (*pp) pp = &(*pp)->subling_next;
      *pp = nf;
   }
}

h2_inline void h2_fail::append_child(h2_fail*& fail, h2_fail* nf)
{
   if (!fail) {
      fail = nf;
   } else {
      h2_fail** pp = &fail->child_next;
      while (*pp) pp = &(*pp)->child_next;
      *pp = nf;
   }
}

h2_inline h2_fail::~h2_fail()
{
   if (child_next) delete child_next;
   if (subling_next) delete subling_next;
}

h2_inline h2_line h2_fail::locate()
{
   if (h2_blank(filine)) return {};
   return gray(" at ") + h2_basefile(filine);
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, size_t, size_t)> cb, size_t si, size_t ci)
{
   cb(this, si, ci);
   if (child_next) child_next->foreach(cb, 0, ci + 1);
   if (subling_next) subling_next->foreach(cb, si + 1, ci);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_line& explain_ = {}, const char* filine_ = nullptr) : h2_fail(explain_, filine_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      line += explain;
      h2_console::printl(line + locate());
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   h2_string _a = a.escape(), _b = b.escape();
   if (_a == _b) return true;

#define H2_NULL_SYNONYM "NULL", "nullptr", "null", "__null", "(null)", "(nil)", "((void *)0)", "0", "0x0", "00000000", "0000000000000000"
#define H2_NOTNULL_SYNONYM "!NULL", "!nullptr", "Not(NULL)", "Not(nullptr)", "Nq(NULL)", "Nq(nullptr)", "Not(0)", "Nq(0)"
#define H2_TRUE_SYNONYM "true", "TRUE", "True", "1"
#define H2_FALSE_SYNONYM "false", "FALSE", "False", "0"

   if (h2_in(_a.c_str(), 11, H2_NULL_SYNONYM) && h2_in(_b.c_str(), 11, H2_NULL_SYNONYM)) return true;
   if (h2_in(_a.c_str(), 8, H2_NOTNULL_SYNONYM) && h2_in(_b.c_str(), 8, H2_NOTNULL_SYNONYM)) return true;
   if (h2_in(_a.c_str(), 4, H2_TRUE_SYNONYM) && h2_in(_b.c_str(), 4, H2_TRUE_SYNONYM)) return true;
   if (h2_in(_a.c_str(), 4, H2_FALSE_SYNONYM) && h2_in(_b.c_str(), 4, H2_FALSE_SYNONYM)) return true;

   if (_a == "Eq(" + _b + ")") return true;
   if (_a == "ListOf(" + _b.unenclose('[', ']') + ")") return true;

   return false;
}

struct h2_fail_unexpect : h2_fail {
   h2_line expection, represent;
   int c = 0;
   h2_fail_unexpect(const h2_line& expection_ = {}, const h2_line& represent_ = {}, const h2_line& explain_ = {}, const char* file_ = nullptr) : h2_fail(explain_, file_), expection(expection_), represent(represent_) {}
   void print_OK1(h2_line& line)
   {
      h2_line a = h2_line(a_expression).gray_quote().brush("cyan");
      line += "OK" + gray("(") + a + gray(")") + " is " + color("false", "bold,red");
   }
   void print_OK2(h2_line& line)
   {
      h2_line e, a;
      if (!expection.width()) {
         e = h2_line(e_expression).abbreviate(10000, 3).gray_quote().brush("green");
      } else if (is_synonym(e_expression, expection.string())) {
         e = h2_line(e_expression).abbreviate(10000, 3).brush("green");
      } else {
         e = h2_line(e_expression).abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).gray_quote().brush("cyan") + gray("==>") + expection.abbreviate(10000, 3).brush("green");
      }  // https://unicode-table.com/en/sets/arrow-symbols/ (← →) (← →) (⇐ ⇒) (⟵ ⟶) ⟸ ⟹

      if (!represent.width()) {
         a = h2_line(a_expression).abbreviate(10000, 3).gray_quote().brush("bold,red");
      } else if (is_synonym(a_expression, represent.string()) || !a_expression.length()) {
         a = represent.abbreviate(10000, 3).brush("bold,red");
      } else {
         a = represent.abbreviate(10000, 3).brush("bold,red") + gray("<==") + h2_line(a_expression).abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).gray_quote().brush("cyan");
      }

      line += "OK" + gray("(") + e + " " + assert_op + " " + a + gray(")");
   }
   void print_JE(h2_line& line)
   {
      h2_line e = h2_line(e_expression.unenclose('\"').unenclose('\'')).abbreviate(O.verbose >= VerboseDetail ? 10000 : 30, 2).brush("cyan");
      h2_line a = h2_line(a_expression.unenclose('\"').unenclose('\'')).abbreviate(O.verbose >= VerboseDetail ? 10000 : 30, 2).brush("bold,red");
      line += "JE" + gray("(") + e + ", " + a + gray(")");
   }
   void print_Inner(h2_line& line)
   {
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      if (expection.width()) {
         line.printf("", "%sexpect is ", comma_if(c++));
         line += expection.abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).brush("green");
      }
      if (represent.width()) {
         line.printf("", "%sactual is ", comma_if(c++));
         line += represent.abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).brush("bold,red");
      }
   }

   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (!strcmp("Inner", assert_type)) print_Inner(line);
      if (!strcmp("OK1", assert_type)) print_OK1(line);
      if (!strcmp("OK2", assert_type)) print_OK2(line);
      if (!strcmp("JE", assert_type)) print_JE(line);
      if (explain.width()) line += comma_if(c++, ", ", " ") + explain;
      if (user_explain.size()) line += {comma_if(c++, ", ", " "), user_explain};
      h2_console::printl(line + locate());
   }
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   const h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_line& expection, const h2_line& explain = {}) : h2_fail_unexpect(expection, h2_stringify(a_value_, true), explain), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   h2_line fmt_char(h2_string& c, bool eq, const char* style)
   {
      if (c.equals(" ") && O.colorful) return gray("‧");
      if (eq) return c.escape();
      return color(c.escape(), style);
   }
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (O.verbose >= VerboseDetail || 8 < e_value.width() || 8 < a_value.width()) {
         h2_line e_line, a_line;
         h2_vector<h2_string> e_chars = e_value.disperse(), a_chars = a_value.disperse();
         auto lcs = h2_LCS(e_chars, a_chars, caseless).lcs();
         for (size_t i = 0; i < lcs.first.size(); ++i) e_line += fmt_char(e_chars[i], lcs.first[i], "green");
         for (size_t i = 0; i < lcs.second.size(); ++i) a_line += fmt_char(a_chars[i], lcs.second[i], "red");
         h2_console::printl(h2_layout::unified(e_line, a_line, "expect", "actual", h2_console::width()));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   const h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_line& expection, const h2_line& explain) : h2_fail_unexpect(expection, h2_stringify(a_value_, true), explain), e_value(e_value_), a_value(a_value_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (16 < e_value.width() || 16 < a_value.width()) {
         h2_line e_line = e_value.escape(), a_line = a_value.escape();
         h2_console::printl(h2_layout::seperate(e_line, a_line, "expect", "actual", h2_console::width()));
      }
   }
};

struct h2_fail_json : h2_fail_unexpect {
   const bool caseless;
   const h2_string e_value, a_value;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_line& expection_, bool caseless_, const h2_line& explain_) : h2_fail_unexpect(expection_, a_value_.squash(), explain_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      e_expression = e_expression.squash(true);
      a_expression = a_expression.squash(true);

      h2_lines e_lines, a_lines;
      h2_fail_unexpect::print(si, ci);
      if (!h2_blank(O.json_source_quote) || !h2_json::diff(e_value, a_value, e_lines, a_lines, caseless)) {
         e_lines = h2_json::dump(e_value);
         a_lines = h2_json::dump(a_value);
         for (size_t i = 0; i < e_lines.size(); ++i)
            if (i) e_lines[i].indent(8);
         for (size_t i = 0; i < a_lines.size(); ++i)
            if (i) a_lines[i].indent(8);
         h2_console::prints("dark gray", "expect");
         h2_console::prints("green", "> ");
         h2_console::printl(e_lines);
         h2_console::prints("dark gray", "actual");
         h2_console::prints("red", "> ");
         h2_console::printl(a_lines);
      } else {
         h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual", 0, 'd', h2_console::width() - 1);
         for (auto& line : lines) line.indent(1);
         h2_console::printl(lines);
      }
   }
};

struct h2_fail_memcmp : h2_fail_unexpect {
   const h2_vector<unsigned char> e_value, a_value;
   const size_t length, width;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, const size_t length_, const size_t width_) : h2_fail_unexpect({}, {}, {}), e_value(e_value_, e_value_ + (length_ * width_ + 7) / 8), a_value(a_value_, a_value_ + (length_ * width_ + 7) / 8), length(length_), width(width_) {}
   void print(size_t si, size_t ci) override
   {
      expection.printf("", "memcmp %d %s", (int)length, format_width());
      h2_fail_unexpect::print(si, ci);
      h2_lines e_lines, a_lines;
      size_t bytes_per_row = 0;
      switch (width) {
         case 1: print_bits(e_lines, a_lines, bytes_per_row = 4); break;
         case 8: print_ints<unsigned char>(e_lines, a_lines, bytes_per_row = (h2_console::width() < 108 ? 8 : 16)); break;
         case 16: print_ints<unsigned short>(e_lines, a_lines, bytes_per_row = 16); break;
         case 32: print_ints<unsigned int>(e_lines, a_lines, bytes_per_row = 16); break;
         case 64: print_ints<unsigned long long>(e_lines, a_lines, bytes_per_row = 16); break;
         default: break;
      }
      h2_console::printl(h2_layout::split(e_lines, a_lines, "expect", "actual", bytes_per_row * 8 / width, 'x', h2_console::width()));
   }

   const char* format_width()
   {
      switch (width) {
         case 1: return "bits";
         case 8: return "bytes";
         case 16: return "uint16";
         case 32: return "uint32";
         case 64: return "uint64";
      }
      return "";
   }

   void print_bits(h2_lines& e_lines, h2_lines& a_lines, size_t bytes_per_row)
   {
      size_t rows = (size_t)::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (size_t i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (size_t j = 0; j < bytes_per_row; ++j) {
            if (j) e_line.push_back(" ");
            if (j) a_line.push_back(" ");
            for (size_t k = 0; k < 8; ++k) {
               if ((i * bytes_per_row + j) * 8 + k < length) {
                  unsigned char e_val = (e_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  unsigned char a_val = (a_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  if (e_val == a_val) {
                     e_line.push_back(h2_string(e_val ? "1" : "0"));
                     a_line.push_back(h2_string(a_val ? "1" : "0"));
                  } else {
                     e_line.printf("green", e_val ? "1" : "0");
                     a_line.printf("bold,red", a_val ? "1" : "0");
                  }
               }
            }
         }
         e_lines.push_back(e_line);
         a_lines.push_back(a_line);
      }
   }

   template <typename T>
   void print_ints(h2_lines& e_lines, h2_lines& a_lines, size_t bytes_per_row)
   {
      char fmt[32];
      sprintf(fmt, "%%s%%0%dX", (int)sizeof(T) * 2);

      size_t rows = (size_t)::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (size_t i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (size_t j = 0; j < bytes_per_row; j += sizeof(T)) {
            if (i * bytes_per_row + j < e_value.size()) {
               T e_val = *(T*)(e_value.data() + (i * bytes_per_row + j));
               T a_val = *(T*)(a_value.data() + (i * bytes_per_row + j));
               char e_str[32], a_str[32];
               sprintf(e_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", e_val);
               sprintf(a_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", a_val);
               if (e_val == a_val) {
                  e_line.push_back(e_str);
                  a_line.push_back(a_str);
               } else {
                  e_line.printf("green", e_str);
                  a_line.printf("bold,red", a_str);
               }
            }
         }
         e_lines.push_back(e_line);
         a_lines.push_back(a_line);
      }
   }
};

struct h2_fail_memory : h2_fail {
   const void* ptr;
   const size_t size;
   const h2_backtrace bt_allocate, bt_release;
   h2_fail_memory(const void* ptr_, const size_t size_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const char* file_ = nullptr) : h2_fail({}, file_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   const h2_vector<std::pair<size_t, size_t>> sizes;
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, const size_t size_, const h2_vector<std::pair<size_t, size_t>>& sizes_, const h2_backtrace& bt_allocate_, const char* where_, const char* file_) : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_), sizes(sizes_), where(where_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line = h2_stringify(ptr) + color(" memory leak ", "bold,red") + h2_stringify(size).brush("red") + " ";
      size_t i = 0, c = 0, n = 3;
      h2_line sl;
      for (auto& p : sizes) {
         sl += gray(comma_if(i++));
         if (O.verbose <= VerboseCompactPassed && n < i) {
            sl += color("..." + h2_stringify(sizes.size() - n), "dark gray");
            break;
         }
         sl += h2_stringify(p.first);
         if (1 < p.second) sl += gray("x") + h2_stringify(p.second);
         c += p.second;
      }
      if (1 < c) line += gray("[") + sl + gray("] ");
      h2_console::printl(" " + line + "bytes in " + where + " totally" + locate());
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::prints("", " %p", ptr);
      h2_console::prints("bold,red", " double free");
      h2_console::prints("", " at backtrace:\n", ptr), bt_double_free.print(2);
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_console::prints("", "  already free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::prints("", " %p allocate with ", ptr);
      h2_console::prints("bold,red", "%s", who_allocate);
      h2_console::prints("", ", release by ");
      h2_console::prints("bold,red", "%s", who_release);
      h2_console::prints("", " asymmetrically at backtrace:\n"), bt_release.print(2);
      if (0 < bt_allocate.count) h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_overflow : h2_fail_memory {
   const void* violate_ptr;             /* 犯罪地点 */
   const char* action;                  /* 犯罪行为 */
   const h2_vector<unsigned char> spot; /* 犯罪现场 */
   const h2_backtrace bt_trample;       /* 犯罪过程 */
   h2_fail_overflow(const void* ptr_, const size_t size_, const void* violate_ptr_, const char* action_, const h2_vector<unsigned char>& spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_) : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace()), violate_ptr(violate_ptr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      long long offset = ptr < violate_ptr ? (long long)violate_ptr - ((long long)ptr + size) : (long long)violate_ptr - (long long)ptr;
      h2_line t = h2_stringify(ptr) + " " + color(h2_string("%+d", (int)offset), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + " " + (offset >= 0 ? "overflow" : "underflow") + " ";
      for (size_t i = 0; i < spot.size(); ++i) t.printf("bold,red", "%02X ", spot[i]);
      h2_console::printl(" " + t + locate() + (bt_trample.count ? " at backtrace:" : ""));
      if (bt_trample.count) bt_trample.print(3);
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_use_after_free : h2_fail_memory {
   const void* violate_ptr;   /* 犯罪地点 */
   const char* action;        /* 犯罪行为 */
   const h2_backtrace bt_use; /* 犯罪过程 */
   h2_fail_use_after_free(const void* ptr_, const void* violate_ptr_, const char* action_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_use_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), violate_ptr(violate_ptr_), action(action_), bt_use(bt_use_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line t = h2_stringify(ptr) + " " + color(h2_string("%+d", (long long)violate_ptr - (long long)ptr), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + color(" after free", "bold,red");
      h2_console::printl(" " + t + " at backtrace:"), bt_use.print(2);
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_console::prints("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_exception : h2_fail {
   const char* type;
   const h2_backtrace bt_throw;
   h2_fail_exception(const h2_line& explain_, const char* type_, const h2_backtrace& bt_throw_) : h2_fail(explain_, nullptr), type(type_), bt_throw(bt_throw_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::printl(" exception " + color(type, "red") + " " + explain + " at backtrace:");
      bt_throw.print(3);
   }
};

struct h2_fail_symbol : h2_fail {
   const h2_string symbol;
   const h2_vector<h2_string> candidates;
   h2_fail_symbol(const h2_string& symbol_, const h2_vector<h2_string>& candidates_, const h2_line& explain_) : h2_fail(explain_, nullptr), symbol(symbol_), candidates(candidates_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::printl(color(candidates.size() ? " Find multiple " : " Not found ", "yellow") + color(symbol, "bold,red"));
      for (size_t i = 0; i < candidates.size(); ++i)
         h2_console::printl("  " + color(h2_stringify(i) + ". ", "dark gray") + color(candidates[i], "yellow"));
      if (explain.width()) h2_console::printl(explain);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_line& explain_, const char* file_) { return new h2_fail_normal(explain_, file_); }
h2_inline h2_fail* h2_fail::new_unexpect(const h2_line& expection_, const h2_line& represent_, const h2_line& explain_) { return new h2_fail_unexpect(expection_, represent_, explain_); }
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strcmp(e_value, a_value, caseless, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strfind(e_value, a_value, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, bool caseless, const h2_line& explain_) { return new h2_fail_json(e_value, a_value, expection_, caseless, explain_); }
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t length, const size_t width) { return new h2_fail_memcmp(e_value, a_value, length, width); }
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* file_) { return new h2_fail_memory_leak(ptr, size, sizes, bt_allocate, where, file_); }
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free) { return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free); }
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release) { return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release); }
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample) { return new h2_fail_overflow(ptr, size, violate_ptr, action, spot, bt_allocate, bt_trample); }
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use) { return new h2_fail_use_after_free(ptr, violate_ptr, action, bt_allocate, bt_release, bt_use); }
h2_inline h2_fail* h2_fail::new_exception(const char* explain_, const char* type, const h2_backtrace& bt_throw) { return new h2_fail_exception(explain_, type, bt_throw); }
h2_inline h2_fail* h2_fail::new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_line& explain_) { return new h2_fail_symbol(symbol, candidates, explain_); };
// source/render/h2_report.cpp
struct h2_report_impl {
   h2_list x;
   int runner_cases = 0, runner_case_index = 0;

   virtual void on_runner_start(h2_runner* r)
   {
      h2_list_for_each_entry (s, r->suites, h2_suite, x)
         runner_cases += s->cases.count();
      r->stats.timecost = h2_now();
   }
   virtual void on_runner_endup(h2_runner* r)
   {
      r->stats.timecost = h2_now() - r->stats.timecost;
   }
   virtual void on_suite_start(h2_suite* s)
   {
      s->stats.timecost = h2_now();
   }
   virtual void on_suite_endup(h2_suite* s)
   {
      s->stats.timecost = h2_now() - s->stats.timecost;
   }
   virtual void on_case_start(h2_suite* s, h2_case* c)
   {
      ++runner_case_index;
      c->stats.timecost = h2_now();
   }
   virtual void on_case_endup(h2_suite* s, h2_case* c)
   {
      c->stats.timecost = h2_now() - c->stats.timecost;
   }
};

#define H2_UNITS(count, unit) ((count > 1) ? (unit "s") : unit)

struct h2_report_list : h2_report_impl {
   int suites = 0, cases = 0, todos = 0;
   int unfiltered_suites = 0, unfiltered_cases = 0, unfiltered_todos = 0;
   int suite_cases = 0, suite_todos = 0;

   struct tag {
      const char* name;
      int suites, cases;
   };

   int tagc = 0, unfiltered_tagc = 0;
   struct tag tags[4096], unfiltered_tags[4096];

   static tag* get_tag(tag tags[], int& tagc, const char* name)
   {
      for (int i = 0; i < tagc; ++i)
         if (!strcmp(name, tags[i].name)) return &tags[i];
      return nullptr;
   }
   static tag* add_tag(tag tags[], int& tagc, const char* name)
   {
      tag* t = get_tag(tags, tagc, name);
      if (t) return t;
      if (tagc >= 4096) return nullptr;
      t = &tags[tagc++];
      t->name = name;
      t->suites = t->cases = 0;
      return t;
   }
   static void add_suite_tag(tag tags[], int& tagc, const char* name)
   {
      auto t = add_tag(tags, tagc, name);
      if (t) t->suites++;
   }
   static void add_case_tag(tag tags[], int& tagc, const char* name)
   {
      auto t = add_tag(tags, tagc, name);
      if (t) t->cases++;
   }

   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      if (O.lists & ListTag) {
         for (int i = 0; i < unfiltered_tagc; ++i) {
            h2_line line;
            line.printf("dark gray", "TAG-%d. ", i).printf("bold,light purple", "%s ", unfiltered_tags[i].name);
            if (unfiltered_tags[i].suites) line.printf("", " %d ", unfiltered_tags[i].suites).printf("dark gray", H2_UNITS(unfiltered_tags[i].suites, "suite"));
            if (unfiltered_tags[i].cases) line.printf("", " %d ", unfiltered_tags[i].cases).printf("dark gray", H2_UNITS(unfiltered_tags[i].cases, "case"));
            h2_console::printl(line);
         }
      }

      h2_line line;
      if (O.lists & ListSuite) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_suites), "green") + " " + gray(H2_UNITS(unfiltered_suites, "suite"));
      if (O.lists & ListCase) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_cases), "green") + " " + gray(H2_UNITS(unfiltered_cases, "case"));
      if (O.lists & ListTodo) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_todos), "green") + " " + gray(H2_UNITS(unfiltered_todos, "todo"));
      if (O.lists & ListTag) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_tagc), "green") + " " + gray(H2_UNITS(unfiltered_tagc, "tag"));
      if (O.lists & ListSuite && suites > unfiltered_suites) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), suites - unfiltered_suites, H2_UNITS(suites - unfiltered_suites, "suite"));
      if (O.lists & ListCase && cases > unfiltered_cases) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), cases - unfiltered_cases, H2_UNITS(cases - unfiltered_cases, "case"));
      if (O.lists & ListTodo && todos > unfiltered_todos) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), todos - unfiltered_todos, H2_UNITS(todos - unfiltered_todos, "todo"));
      if (O.lists & ListTag && tagc > unfiltered_tagc) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), tagc - unfiltered_tagc, H2_UNITS(tagc - unfiltered_tagc, "tag"));
      h2_console::printl("Listing " + line);
   }
   h2_line format_tags(const char* tags[])
   {
      h2_line line;
      if (O.lists & ListTag)
         for (int i = 0; tags[i]; ++i) line.printf("purple", " %s", tags[i]);
      return line;
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      suite_cases = 0;
      suite_todos = 0;
      if (s->absent()) return;  // CASE
      for (int i = 0; s->tags[i]; ++i) add_suite_tag(tags, tagc, s->tags[i]);
      if (!s->filtered)
         for (int i = 0; s->tags[i]; ++i) add_suite_tag(unfiltered_tags, unfiltered_tagc, s->tags[i]);
      ++suites;
      if (!s->filtered && O.lists & ListSuite) {
         ++unfiltered_suites;
         h2_line line;
         line.printf("dark gray", "SUITE-%d. ", unfiltered_suites);
         h2_console::printl(line + color(s->name, "bold,blue") + " " + gray(h2_basefile(s->filine)) + format_tags(s->tags));
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      for (int i = 0; c->tags[i]; ++i) add_case_tag(tags, tagc, c->tags[i]);
      if (!c->filtered)
         for (int i = 0; c->tags[i]; ++i) add_case_tag(unfiltered_tags, unfiltered_tagc, c->tags[i]);

      const char* type = nullptr;
      if (c->todo) {
         ++todos;
         if (!c->filtered && O.lists & ListTodo) {
            type = s->absent() ? "TODO" : "Todo";
            ++unfiltered_todos, ++suite_todos;
         }
      } else {
         ++cases;
         if (!c->filtered && O.lists & ListCase) {
            type = s->absent() ? "CASE" : "Case";
            ++unfiltered_cases, ++suite_cases;
         }
      }

      if (type) {
         h2_line line;
         if (O.lists & ListSuite)
            line.printf("dark gray", " %s/%d-%d. ", type, suite_cases + suite_todos, unfiltered_cases + unfiltered_todos);
         else
            line.printf("dark gray", " %s-%d. ", type, unfiltered_cases + unfiltered_todos);

         h2_console::printl(line + color(c->name, "cyan") + " " + gray(h2_basefile(c->filine)) + format_tags(c->tags));
      }
   }
};

struct h2_report_console : h2_report_impl {
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   h2_line format_title(const char* suite_name, const char* case_name, const char* file_line)
   {
      h2_line title;
      title.printf("dark gray", "┊ ");
      if (strlen(case_name))
         title.printf("", "%s ", case_name);
      else
         title.printf("dark gray", "case ");
      if (suite_name) {
         title.printf("dark gray", "┊ ");
         if (strlen(suite_name))
            title.printf("", "%s ", suite_name);
         else
            title.printf("dark gray", "suite ");
      }
      if (file_line) {
         title.printf("dark gray", "┊ ");
         title.printf("", "%s ", file_line);
      }
      return title;
   }
   void format_percentage(h2_line& bar)
   {
      bar.printf("dark gray", "[");
      bar.printf("", "%3d%%", runner_cases ? (int)(runner_case_index * 100 / runner_cases) : 100);
      bar.printf("dark gray", "] ");
   }
   static const char* format_volume(long long footprint, char* s = (char*)alloca(128))
   {
      if (footprint < 1024LL)
         sprintf(s, "%lld", footprint);
      else if (footprint < 1024LL * 1024LL)
         sprintf(s, "%.2gKB", footprint / 1024.0);
      else if (footprint < 1024LL * 1024LL * 1024LL)
         sprintf(s, "%.2gMB", footprint / (1024.0 * 1024.0));
      else
         sprintf(s, "%.2gGB", footprint / (1024.0 * 1024.0 * 1024.0));
      return s;
   }
   static const char* format_duration(long long ms, char* s = (char*)alloca(128))
   {
      if (ms < 100)
         sprintf(s, "%lld milliseconds", ms);
      else if (ms < 1000 * 60)
         sprintf(s, "%.2g second%s", ms / 1000.0, ms == 1000 ? "" : "s");
      else if (ms < 1000 * 60 * 60)
         sprintf(s, "%.2g minute%s", ms / 60000.0, ms == 60000 ? "" : "s");
      else
         sprintf(s, "%.2g hour%s", ms / 3600000.0, ms == 3600000 ? "" : "s");
      return s;
   }
   static const char* format_units(int count, const char* unit1, const char* unit2 = nullptr, char* s = (char*)alloca(128))
   {
      bool multiple = count > 1;
      sprintf(s, "%d%s%s", count, multiple && unit2 ? unit2 : unit1, multiple && !unit2 ? "s" : "");
      return s;
   }
   void print_bar(bool percentage, const char* status_style, const char* status, h2_suite* s, h2_case* c, bool backable)
   {
      static long long last_capture_length = 0;
      if (last_capture_length == h2_stdio::I().capture_length)
         h2_console::prints("", "\33[2K\r"); /* clear line */
      else
         h2_console::prints("", "\n"); /* user output, new line */
      last_capture_length = h2_stdio::I().capture_length;
      h2_report::I().backable = O.progressing && backable;

      if (!O.progressing && backable) return;
      h2_line bar;
      if (percentage && O.progressing) format_percentage(bar);
      if (status && status_style) bar.printf(status_style, "%s", status);
      if (s && c) bar += format_title(s->name, c->name, backable ? nullptr : h2_basefile(c->filine));
      if (backable) {
         if (h2_console::width() > bar.width())
            bar.padding(h2_console::width() - bar.width());
         else
            bar = bar.abbreviate(h2_console::width());
      }
      h2_console::printl(bar, false);
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      print_bar(false, nullptr, nullptr, nullptr, nullptr, false);

      int n = nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored);
      h2_line line = (0 < r->stats.failed) ? color("Failure ", "bold,red") : color("Success ", "bold,green");
      if (0 < n) line += gray("(");
      line += color(h2_stringify(r->stats.passed), "green") + " passed";  // always print
      if (r->stats.failed) line += gray(", ") + color(h2_stringify(r->stats.failed), "red") + " failed";
      if (r->stats.todo) line += gray(", ") + color(h2_stringify(r->stats.todo), "yellow") + " todo";
      if (r->stats.filtered) line += gray(", ") + color(h2_stringify(r->stats.filtered), "blue") + " filtered";
      if (r->stats.ignored) line += gray(", ") + color(h2_stringify(r->stats.ignored), "blue") + " ignored";
      if (0 < n) line += gray(") ") + h2_stringify(runner_cases);
      line += H2_UNITS(runner_cases, " case") + gray(", ") + format_units(r->stats.asserts, " assert");
      if (1 < r->rounds) line += gray(", ") + format_units(r->rounds, " round");
      line += gray(", ") + format_duration(r->stats.timecost);

      h2_console::printl(line);
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.verbose >= 9 && !(O.includes[0] || O.excludes[0])) {
         print_bar(true, nullptr, nullptr, nullptr, nullptr, false);

         int n = nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored);
         h2_line line;
         if (s->stats.passed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.passed) + " passed";
         if (s->stats.failed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.failed) + " failed";
         if (s->stats.todo) line += gray(comma_if(line.width())) + h2_stringify(s->stats.todo) + " todo";
         if (s->stats.filtered) line += gray(comma_if(line.width())) + h2_stringify(s->stats.filtered) + " filtered";
         if (s->stats.ignored) line += gray(comma_if(line.width())) + h2_stringify(s->stats.ignored) + " ignored";
         line = gray("suite ") + s->name + gray(1 < n ? " (" : " - ") + line + gray(1 < n ? ")" : "");
         if (0 < s->cases.count()) line += H2_UNITS(s->cases.count(), " case");
         if (0 < s->stats.asserts) line += gray(", ") + format_units(s->stats.asserts, " assert");
         if (0 < s->stats.footprint) line += gray(", ") + format_volume(s->stats.footprint) + " footprint";
         if (1 < s->stats.timecost) line += gray(", ") + format_duration(s->stats.timecost);

         h2_console::printl(line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (c->filtered || c->ignored || c->todo) return;
      print_bar(true, "light blue", "Testing", s, c, true);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (c->filtered || c->ignored) return;
      if (c->todo) {
         if (O.verbose >= VerboseDetail) print_bar(true, "yellow", s->absent() ? "TODO   " : "Todo   ", s, c, false);
      } else if (c->failed) {
         if (O.verbose >= VerboseCompactFailed) {
            print_bar(true, "bold,red", "Failed ", s, c, false);
            h2_console::prints("", "\n");
            if (O.verbose >= VerboseNormal && c->fails) {
               c->fails->foreach([](h2_fail* fail, size_t si, size_t ci) { fail->print(si, ci); });
               h2_console::prints("", "\n");
            }
         }
      } else {  // Passed
         if (O.verbose >= VerboseDetail || O.verbose == VerboseCompactPassed) {
            print_bar(true, "green", "Passed ", s, c, false);
            if (O.verbose >= VerboseDetail) {
               h2_line line;
               if (0 < c->stats.asserts) line += gray(comma_if(line.width())) + format_units(c->stats.asserts, " assert");
               if (0 < c->stats.footprint) line += gray(comma_if(line.width())) + format_volume(c->stats.footprint) + " footprint";
               if (0 < c->stats.timecost) line += gray(comma_if(line.width())) + format_duration(c->stats.timecost);
               if (line.width()) h2_console::printl(gray("- ") + line, false);
            }
            h2_console::prints("", "\n");
         }
      }
   }
};

struct h2_report_junit : h2_report_impl {
   FILE* f;
   void on_runner_start(h2_runner* r) override
   {
      h2_report_impl::on_runner_start(r);
      f = fopen(O.junit_path, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats.failed, s->name, s->stats.todo + s->stats.filtered, s->cases.count(), 0, "");
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, c->todo ? "TODO" : (c->filtered ? "Filtered" : (c->ignored ? "Ignored" : (c->failed ? "Failed" : "Passed"))), c->stats.timecost / 1000.0);
      if (c->failed) {
         fprintf(f, "<failure message=\"%s:", c->filine);
         if (c->fails) c->fails->foreach([&](h2_fail* fail, size_t si, size_t ci) {fprintf(f, "{newline}"); fail->print(f); });
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "<system-out></system-out><system-err></system-err>\n");
      fprintf(f, "</testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_report_tap : h2_report_impl {
   /* */
};

h2_inline void h2_report::initialize()
{
   static h2_report_list list_report;
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   if (O.lists) {
      I().reports.push_back(list_report.x);
   } else {
      I().reports.push_back(console_report.x);
      if (strlen(O.junit_path)) I().reports.push_back(junit_report.x);
      if (strlen(O.tap_path)) I().reports.push_back(tap_report.x);
   }
}

/* clang-format off */
h2_inline void h2_report::on_runner_start(h2_runner* r) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_runner_start(r); }
h2_inline void h2_report::on_runner_endup(h2_runner* r) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_runner_endup(r); }
h2_inline void h2_report::on_suite_start(h2_suite* s) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_suite_start(s); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_suite_endup(s); }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_case_start(s, c); }
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c) { h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_case_endup(s, c); }
// source/render/h2_option.cpp
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
            "\033[90m│\033[0m" " -\033[36mF\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mn=max\033[90m]\033[0m  "     "\033[90m│\033[0m" " \033[36mF\033[0mold json print, 0:unfold 1:short 2:same 3:single          "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mi\033[0m\033[90m/\033[0m\033[36me\033[0m" "\033[90m│\033[0m" "\033[90m[\033[0mpattern .\033[90m]\033[0m"     "\033[90m│\033[0m" " \033[36mi\033[0mnclude\033[90m/\033[0m\033[36me\033[0mxclude case suite or file by substr wildcard      " "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36ml\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mtype .\033[90m]\033[0m "     "\033[90m│\033[0m" " \033[36ml\033[0mist suites cases and tags, type [suite case todo tag]     "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mm\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Test cases without \033[36mm\033[0memory check                            "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mo\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mpath\033[90m]\033[0m   "     "\033[90m│\033[0m" " \033[36mo\033[0mutput junit report, default is <executable>.junit.xml     "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mp\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Disable test percentage \033[36mp\033[0mrogressing bar                    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mq\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36mq\033[0muit exit code as failed cases count                       "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mr\033[0m  "                               "\033[90m│\033[0m" "   \033[90m[\033[0mn=2\033[90m]\033[0m   "     "\033[90m│\033[0m" " Repeat test n (default 2) \033[36mr\033[0mounds                           "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36ms\033[0m  "                               "\033[90m│\033[0m" "\033[90m[\033[0mtype=rand\033[90m]\033[0m"     "\033[90m│\033[0m" " \033[36ms\033[0mhuffle cases random/alphabet/reverse if no last failed    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mS\033[0m  "                               "\033[90m│\033[0m" " \033[90m[\033[0mtype=\\\"\033[90m]\033[0m "   "\033[90m│\033[0m" " JSON C/C++ \033[36mS\033[0mource code, type [\\\'/single \\\"/double \\\\\\\"]    "                       "\033[90m│\033[0m\n" H2_USAGE_BR
            "\033[90m│\033[0m" " -\033[36mt\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36mt\033[0mags include/exclude filter                                "                               "\033[90m│\033[0m\n" H2_USAGE_BR
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
            while ((t = get.extract_string())) h2_array_append(excludes, t);
            break;
         case 'f': only_last_failed = true; break;
         case 'F': get.extract_number(fold_json = 0); break;
         case 'i':
            while ((t = get.extract_string())) h2_array_append(includes, t);
            break;
         case 'l':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 4, "suite", "case", "todo", "tag");
               if (!r)
                  ::printf("-l invalid argument: %s, availables: suite | case | todo | tag\n", t);
               else if (!strcmp("suite", r))
                  lists |= ListSuite;
               else if (!strcmp("case", r))
                  lists |= ListCase;
               else if (!strcmp("todo", r))
                  lists |= ListTodo;
               else if (!strcmp("tag", r))
                  lists |= ListTag;
               else
                  ::printf("-l %s\n", r);
            }
            if (!lists) lists = ListSuite | ListCase | ListTodo;
            break;
         case 'm': memory_check = !memory_check; break;
         case 'o':
            sprintf(junit_path, "%s.junit.xml", path);
            if ((t = get.extract_string())) strcpy(junit_path, t);
            break;
         case 'p': progressing = !progressing; break;
         case 'q': quit_exit_code = true; break;
         case 'r': get.extract_number(run_rounds = 2); break;
         case 's':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 4, "random", "name", "file", "reverse");
               if (!r)
                  ::printf("-s invalid argument: %s, availables: random | name | file | reverse\n", t);
               else if (!strcmp("random", r))
                  shuffles |= ShuffleRandom;
               else if (!strcmp("name", r))
                  shuffles |= ShuffleName;
               else if (!strcmp("file", r))
                  shuffles |= ShuffleFile;
               else if (!strcmp("reverse", r))
                  shuffles |= ShuffleReverse;
               else
                  ::printf("-s %s\n", r);
            }
            if (!shuffles) shuffles = ShuffleRandom;
            break;
         case 'S':
            json_source_quote = "\\\"";
            if ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 5, "\'", "single", "\"", "double", "\\\"");
               if (!r)
                  ::printf("-S invalid argument: %s, availables: \' | single | \" | double | \\\"\n", t);
               else if (!strcmp("\'", r) || !strcmp("single", r))
                  json_source_quote = "\'";
               else if (!strcmp("\"", r) || !strcmp("double", r))
                  json_source_quote = "\"";
               else if (!strcmp("\\\"", r))
                  json_source_quote = "\\\"";
               else
                  ::printf("-S %s\n", r);

               if (!h2_in(json_source_quote, 3, "\'", "\"", "\\\"")) json_source_quote = "\\\"";
            }
            break;
         case 't': tags_filter = true; break;
         case 'v': get.extract_number(verbose = 8); break;
         case 'w': colorful = !colorful; break;
         case 'x': exception_as_fail = true; break;
         case 'h':
         case '?': usage(); exit(0);
      }
   }
}
}
