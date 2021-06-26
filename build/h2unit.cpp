
/* v5.9 2021-06-26 18:18:20 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#include "h2unit.hpp"

#include <cassert>   /* assert */
#include <cctype>    /* tolower, isspace */
#include <cstdarg>   /* va_list */
#include <errno.h>   /* strerror */
#include <exception> /* std::exception */
#include <iostream>  /* std::cout, std::streambuf */
#include <memory>    /* std::allocator */
#include <regex>     /* std::regex */
#include <signal.h>  /* sigaction */
#include <typeinfo>  /* std::typeid, std::type_info */

#if defined _WIN32
#   include <winsock2.h> /* socket */
#   include <ws2tcpip.h> /* getaddrinfo */
#   include <io.h>       /* _wirte */
#   include <shlwapi.h>  /* PathRemoveFileSpecA, StrStrIA */
#   include <dbghelp.h>  /* SymFromAddr */
#   define fileno _fileno
#   define socklen_t int
#   define strcasestr StrStrIA
#   pragma comment(lib, "Ws2_32.lib")
#   pragma comment(lib, "Shlwapi.lib")
#   pragma comment(lib, "Dbghelp.lib")
#else
#   include <arpa/inet.h>   /* inet_addr, inet_pton */
#   include <cxxabi.h>      /* abi::__cxa_demangle, abi::__cxa_throw */
#   include <execinfo.h>    /* backtrace */
#   include <fcntl.h>       /* fcntl */
#   include <fnmatch.h>     /* fnmatch */
#   include <libgen.h>      /* basename */
#   include <netdb.h>       /* getaddrinfo, gethostbyname */
#   include <sys/ioctl.h>   /* ioctl */
#   include <sys/mman.h>    /* mprotect, mmap */
#   include <sys/socket.h>  /* sockaddr */
#   include <sys/syscall.h> /* syscall */
#   include <sys/time.h>    /* gettimeofday */
#   include <sys/types.h>   /* size_t */
#   include <syslog.h>      /* syslog, vsyslog */
#   include <unistd.h>      /* sysconf */
#   if defined __GLIBC__
#      include <malloc.h> /* __malloc_hook */
#   elif defined __APPLE__
#      include <AvailabilityMacros.h>
#      include <malloc/malloc.h> /* malloc_zone_t */
#   endif
#endif

#if defined _WIN32
int main(int argc, const char** argv);
#   if defined __H2UNIT_HPP__ || defined IMPORT_MAIN
int main(int argc, const char** argv)
{
   h2::h2_option::I().parse(argc, argv);
   h2::h2_task::I().execute();
   return 0;
}
#   endif
#else
__attribute__((weak)) int main(int argc, const char** argv)
{
   h2::h2_option::I().parse(argc, argv);
   h2::h2_task::I().execute();
   return 0;
}
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
// source/utils/h2_misc.cpp

static inline const char* h2_basename(const char* path)
{
   const char* p = strrchr(path, '/');
   if (!p) p = strrchr(path, '\\');
   return p ? p + 1 : path;
}

h2_inline bool h2_pattern::regex_match(const char* pattern, const char* subject, bool caseless)
{
   bool result = false;
   try {  // c++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error&) {
      result = false;
   }
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
// source/utils/h2_numeric.cpp

h2_inline bool h2_numeric::not2n(unsigned x)
{
   return x & (x - 1);
}

h2_inline unsigned h2_numeric::mask2n(unsigned x)
{
   x |= (x >> 1);
   x |= (x >> 2);
   x |= (x >> 4);
   x |= (x >> 8);
   x |= (x >> 16);
   return x;
}

h2_inline int h2_numeric::hex_to_byte(char c)
{
   return '0' <= c && c <= '9' ? c - '0' : ('A' <= c && c <= 'F' ? c - 'A' + 10 : ('a' <= c && c <= 'f' ? c - 'a' + 10 : -1));
}

h2_inline bool h2_numeric::is_bin_string(const char* s)
{
   for (const char* p = s; *p; p++)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

h2_inline bool h2_numeric::is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; p++)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

h2_inline int h2_numeric::bin_to_bits(const char* bin, unsigned char* bytes)
{
   memset(bytes, 0, strlen(bin));
   int c = 0;
   for (const char* p = bin; *p; p++) {
      if (*p == ' ') continue;
      int i = c / 8, j = 7 - c % 8;
      ++c;
      unsigned char ebit = *p == '1' ? 1 : 0;
      bytes[i] = bytes[i] | (ebit << j);
   }
   return c;
}

h2_inline int h2_numeric::hex_to_bits(const char* hex, unsigned char* bytes)
{
   memset(bytes, 0, strlen(hex));
   if (hex[0] == '0' && ::tolower(hex[1]) == 'x') hex += 2;
   char b;
   int c = 0;
   for (const char* p = hex; *p; p++) {
      if (::isxdigit(*p)) {
         if (++c % 2 == 0)
            bytes[c / 2 - 1] = (unsigned char)((hex_to_byte(b) << 4) + hex_to_byte(*p));
         else
            b = *p;
      }
   }
   return 8 * c / 2;
}

h2_inline int h2_numeric::hex_to_bytes(const char* hex, unsigned char* bytes)
{
   char b;
   int i = 0, c = 0;

   for (; ::isxdigit(hex[c]);) ++c;
   if (c % 2 == 1) {
      b = '0';
      i = 1;
      c += 1;
      hex = hex - 1;
   }
   for (; i < c; ++i) {
      if (i % 2 == 1)
         bytes[i / 2] = (unsigned char)((hex_to_byte(b) << 4) + hex_to_byte(hex[i]));
      b = hex[i];
   }
   return c / 2;
}

h2_inline bool h2_numeric::bits_equal(const unsigned char* b1, const unsigned char* b2, int nbits)
{
   for (int k = 0; k < nbits; ++k) {
      int i = k / 8, j = 7 - k % 8;
      if (((b1[i] >> j) & 1) != ((b2[i] >> j) & 1)) return false;
   }
   return true;
}

h2_inline const char* h2_numeric::sequence_number(int sequence, int shift)
{
   static const char* st[] = {"0th", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th"};
   static char ss[64];

   sequence += shift;
   if (sequence < sizeof(st) / sizeof(st[0])) {
      return st[sequence];
   }
   sprintf(ss, "%dth", sequence);
   return ss;
}
// source/utils/h2_libc.cpp

struct h2_libc_malloc {
   h2_singleton(h2_libc_malloc);

   struct buddy {
      long long size;
      h2_list x;
      buddy(long long _size) : size(_size) {}
      bool join_right(buddy* b) { return ((char*)this) + size == (char*)b; }
      bool join_left(buddy* b) { return ((char*)b) + b->size == (char*)this; }
   };

   struct block {
      long long bytes;
      block* next = nullptr;
      h2_list buddies;

      block(long long _bytes) : bytes(_bytes)
      {
         buddy* b = new ((char*)this + sizeof(block)) buddy(bytes - sizeof(block));
         buddies.add_tail(b->x);
      }

      buddy* malloc(const long long size)
      {
         h2_list_for_each_entry (p, buddies, buddy, x) {
            if (size + sizeof(p->size) <= p->size) {
               long long left = p->size - (size + sizeof(p->size));
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
               //TODO: join_left with next buddy
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

   void batch(long long size)
   {
      int page_size = h2_page_size();
      int page_count = ::ceil(size / (double)page_size) + 256;

#if defined _WIN32
      PVOID ptr = VirtualAlloc(NULL, page_count * page_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if (ptr == NULL) ::printf("VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      void* ptr = ::mmap(nullptr, page_count * page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (ptr == MAP_FAILED) h2_color::prints("yellow", "mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      block* p = new (ptr) block(page_count * page_size);
      p->next = next;
      next = p;
   }

   buddy* alloc(long long size)
   {
      for (block* p = next; p; p = p->next) {
         buddy* b = p->malloc(size);
         if (b) return b;
      }
      return nullptr;
   }

   void* malloc(size_t size)
   {
      long long _size = (size + 7) / 8 * 8;
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
   if (!O.memory_check) {
      return ::malloc(size);
   }

   return h2_libc_malloc::I().malloc(size + 10);
}

h2_inline void h2_libc::free(void* ptr)
{
   if (!O.memory_check) {
      ::free(ptr);
      return;
   }

   if (ptr) h2_libc_malloc::I().free(ptr);
}

h2_inline ssize_t h2_libc::write(int fd, const void* buf, size_t count)
{
#if defined _WIN32
   return _write(fd, buf, count);
#else
   return ::syscall(SYS_write, fd, buf, count);
#endif
}
// source/utils/h2_string.cpp

h2_inline h2_string& h2_string::sprintf(const char* format, ...)
{
   char* alloca_str;
   h2_sprintf(alloca_str, format);
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

h2_inline bool h2_string::equals(const h2_string& str, bool caseless) const
{
   if (!caseless) return *this == str;
   return tolower() == str.tolower();
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

h2_inline bool h2_string::isspace() const
{
   for (auto& c : *this)
      if (!::isspace(c)) return false;
   return true;
}

h2_inline bool h2_string::enclosed(const char c) const
{
   return front() == c && back() == c;
}

h2_inline h2_string h2_string::escape() const
{
   h2_string s;
   for (auto& c : *this) {
      switch (c) {
      case '\n': s.append("\\n"); break;
      case '\r': s.append("\\r"); break;
      case '\t': s.append("\\t"); break;
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
   //todo: escape \u12ab
   return s;
}

h2_inline h2_string h2_string::unquote(const char c) const
{
   if (!enclosed(c)) return *this;
   return h2_string(size() - 2, c_str() + 1);
}

h2_inline h2_string h2_string::tolower() const
{
   h2_string s;
   for (auto& c : *this) s.push_back(::tolower(c));
   return s;
}

h2_inline h2_string h2_string::center(int width) const
{
   int left = (width - size()) / 2, right = width - left - size();
   h2_string s;
   s.append(left, ' ');
   s.append(*this);
   s.append(right, ' ');
   return s;
}
// source/utils/h2_row.cpp

h2_inline unsigned h2_row::width(bool ignore_indent) const
{
   unsigned w = 0;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         if (!ignore_indent || !word.isspace())
            w += word.size();
   return w;
}

h2_inline h2_row& h2_row::indent(int n, const char c)
{
   insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_row& h2_row::padding(int n, const char c)
{
   push_back(h2_string(n, c));
   return *this;
}

h2_inline h2_row& h2_row::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_row& h2_row::operator+=(const h2_row& row)
{
   insert(end(), row.begin(), row.end());
   return *this;
}

h2_inline h2_row& h2_row::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
   return *this;
}

h2_inline bool h2_row::enclosed(const char c) const
{
   bool f = false, ff = false, b = false;
   for (auto& word : *this) {
      if (!h2_color::isctrl(word.c_str())) {
         if (!ff) f = word.front() == c;
         ff = true;
         b = word.back() == c;
      }
   }

   return f && b;
}

h2_inline h2_row h2_row::gray_quote() const
{
   if (!enclosed('\"') && !enclosed('\'')) return *this;

   h2_row row;
   unsigned w = width();
   unsigned i = 0;

   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         row.push_back(word);
      } else {
         h2_string h, m, t;
         for (auto& c : word) {
            if (i == 0) {
               h.push_back(c);
            } else if (i == w - 1) {
               t.push_back(c);
            } else {
               m.push_back(c);
            }
            ++i;
         }
         if (h.size()) row += gray(h);
         if (m.size()) row.push_back(m);
         if (t.size()) row += gray(t);
      }
   }

   return row;
}

h2_inline h2_row h2_row::acronym(int width, int tail) const
{
   h2_row r1;
   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         r1.push_back(word);
      } else {
         r1.push_back(word.escape());
      }
   }

   int r1_width = r1.width();
   if (r1_width <= width) return r1;

   h2_row r2;
   int i = 0;
   for (auto& word : r1) {
      if (h2_color::isctrl(word.c_str())) {
         r2.push_back(word);
      } else {
         h2_string h, m, t;
         for (auto& c : word) {
            if (i < width - 3 - tail) {
               h.push_back(c);
            } else if (i == width - 3 - tail) {
               m = "...";
            } else if (r1_width - tail <= i) {
               t.push_back(c);
            }
            ++i;
         }
         if (h.size()) r2.push_back(h);
         if (m.size()) r2 += gray(m);
         if (t.size()) r2.push_back(t);
      }
   }

   return r2;
}

h2_inline h2_string h2_row::string() const
{
   h2_string s;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         s += word;
   return s;
}

h2_inline void h2_row::samesizify(h2_row& b)
{
   int w = width(), b_w = b.width();
   padding(std::max(w, b_w) - w);
   b.padding(std::max(w, b_w) - b_w);
}

h2_inline h2_rows& h2_rows::operator+=(const h2_rows& rows)
{
   insert(end(), rows.begin(), rows.end());
   return *this;
}

h2_inline unsigned h2_rows::width() const
{
   unsigned m = 0;
   for (auto& row : *this)
      m = std::max(m, row.width());
   return m;
}

h2_inline bool h2_rows::foldable(unsigned width)
{
   int sum = 0;
   for (auto& row : *this)
      for (auto& word : row)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.size();

   return sum < width;
}

h2_inline h2_row h2_rows::folds()
{
   h2_row folded_row;
   for (auto& row : *this)
      for (auto& word : row)
         if (!word.isspace())  // drop indent
            folded_row.push_back(word);
   return folded_row;
}

h2_inline h2_string h2_rows::string() const
{
   h2_string s;
   for (auto& row : *this)
      for (auto& word : row)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_rows::sequence(unsigned indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = gray(h2_string("%zu. ", i + start)) + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_rows::samesizify(h2_rows& b)
{
   size_t max_y = std::max(size(), b.size());
   for (size_t i = size(); i < max_y; ++i) push_back(h2_row());
   for (size_t i = b.size(); i < max_y; ++i) b.push_back(h2_row());
}
// source/utils/h2_color.cpp

struct h2_colorful {
   h2_singleton(h2_colorful);

   char current[8][32];
   int default_attribute;
#if defined _WIN32
   HANDLE console_handle;
#else
#   define FOREGROUND_INTENSITY 0
#   define COMMON_LVB_UNDERSCORE 0
#   define COMMON_LVB_REVERSE_VIDEO 0
#   define FOREGROUND_RED 0
#   define FOREGROUND_GREEN 0
#   define FOREGROUND_BLUE 0
#   define BACKGROUND_RED 0
#   define BACKGROUND_GREEN 0
#   define BACKGROUND_BLUE 0
#endif

   h2_colorful()
   {
      memset(current, 0, sizeof(current));
      default_attribute = 0;
#if defined _WIN32
      console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(console_handle, &csbi);
      default_attribute = csbi.wAttributes;
#endif
   }

   void clear_style()
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         current[i][0] = '\0';
   }

   void push_style(const char* style, int length)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] == '\0') {
            strncpy(current[i], style, length);
            current[i][length] = '\0';
            break;
         }
   }

   void pop_style(const char* style, int length)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (!strncmp(current[i], style, length) && strlen(current[i]) == length)
            current[i][0] = '\0';
   }

   void change()
   {
#if defined _WIN32
      SetConsoleTextAttribute(console_handle, style2value("reset"));
      WORD a = 0;
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            a |= style2value(current[i]);
      SetConsoleTextAttribute(console_handle, a);
#else
      char a[256];
      sprintf(a, "\033[%d;", style2value("reset"));
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            sprintf(a + strlen(a), "%d;", style2value(current[i]));
      a[strlen(a) - 1] = 'm';
      h2_libc::write(1, a, strlen(a));
#endif
   }

   void parse(const char* style)
   {
      const char* p = style + 2;
      char s = '+';
      if (*p == '+' || *p == '-') s = *p++;

      for (;;) {
         int l = strcspn(p, ",}");
         s == '-' ? pop_style(p, l) : push_style(p, l);
         if (!strncmp("reset", p, l)) clear_style();
         if (*(p + l) == '}' || *(p + l) == '\0') break;
         p += l + 1;
      }
   }

   void print(const char* str)
   {
      if (h2_color::isctrl(str)) {
         if (h2_option::I().colorful) I().parse(str), I().change();
      } else {
         h2_libc::write(fileno(stdout), str, strlen(str));
      }
   }

   int style2value(const char* style)
   {
      static struct st {
         const char* name;
         int value;
         int attribute;
      } K[] = {
        {"reset", 0, default_attribute},
        {"bold", 1, FOREGROUND_INTENSITY},
        {"italics", 3, 0},
        {"underline", 4, COMMON_LVB_UNDERSCORE},
        {"inverse", 7, COMMON_LVB_REVERSE_VIDEO},
        {"strikethrough", 9, 0},
        {"black", 30, 0},
        {"red", 31, FOREGROUND_RED},
        {"green", 32, FOREGROUND_GREEN},
        {"yellow", 33, FOREGROUND_RED | FOREGROUND_GREEN},
        {"blue", 34, FOREGROUND_BLUE},
        {"purple", 35, FOREGROUND_RED | FOREGROUND_BLUE},
        {"cyan", 36, FOREGROUND_BLUE | FOREGROUND_GREEN},
        {"gray", 37, 0},
        {"default", 39, 0},
        {"dark gray", 90, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"light red", 91, FOREGROUND_RED | FOREGROUND_INTENSITY},
        {"light green", 92, FOREGROUND_GREEN | FOREGROUND_INTENSITY},
        {"light yellow", 93, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY},
        {"light blue", 94, FOREGROUND_BLUE | FOREGROUND_INTENSITY},
        {"light purple", 95, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
        {"light cyan", 96, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE},
        {"white", 97, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"bg_black", 40, 0},
        {"bg_red", 41, BACKGROUND_RED},
        {"bg_green", 42, BACKGROUND_GREEN},
        {"bg_yellow", 43, BACKGROUND_RED | BACKGROUND_GREEN},
        {"bg_blue", 44, BACKGROUND_BLUE},
        {"bg_purple", 45, BACKGROUND_RED | BACKGROUND_BLUE},
        {"bg_cyan", 46, BACKGROUND_BLUE | BACKGROUND_GREEN},
        {"bg_white", 47, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE},
        // {"bg_default", 49}
      };

      for (int i = 0; i < sizeof(K) / sizeof(K[0]); ++i)
         if (!strcmp(K[i].name, style))
#if defined _WIN32
            return K[i].attribute;
#else
            return K[i].value;
#endif

      return default_attribute;
   }
};

h2_inline void h2_color::prints(const char* style, const char* format, ...)
{
   if (style && strlen(style)) {
      char t[128];
      sprintf(t, "\033{%s}", style);
      h2_colorful::I().print(t);
   }

   char* alloca_str;
   h2_sprintf(alloca_str, format);
   h2_colorful::I().print(alloca_str);

   if (style && strlen(style)) h2_colorful::I().print("\033{reset}");
}

h2_inline void h2_color::printl(const h2_row& row)
{
   for (auto& word : row) h2_colorful::I().print(word.c_str());
   h2_colorful::I().print("\n");
}

h2_inline void h2_color::printl(const h2_rows& rows)
{
   for (auto& row : rows) printl(row);
}
// source/utils/h2_nm.cpp

static inline void nm1(std::map<std::string, unsigned long long>*& symbols)
{
#if !defined _WIN32
   h2_memory::restores();
   char nm[256], line[2048], addr[128], type, name[2048];
   symbols = new std::map<std::string, unsigned long long>();
   sprintf(nm, "nm %s", O.path);
#   if defined __APPLE__
   sprintf(nm, "nm -U %s", O.path);
#   else
   sprintf(nm, "nm --defined-only %s", O.path);
#   endif
   FILE* f = ::popen(nm, "r");
   if (!f) return;
   while (::fgets(line, sizeof(line) - 1, f)) {
      if (3 != sscanf(line, "%s %c %s", addr, &type, name)) continue;
      if (strchr("bBcCdDiIuU", type)) continue;  // reject bBcCdDiIuU, accept tTwWsSvV, sS for vtable
      int _ = 0;
      if (O.os == macOS) _ = 1;  // remove prefix '_' in MacOS
      (*symbols)[name + _] = (unsigned long long)strtoull(addr, nullptr, 16);
   }
   ::pclose(f);
   h2_memory::overrides();
#endif
}

static inline void nm2(h2_list& symbols)
{
#if !defined _WIN32
   h2_memory::restores();
   char nm[256], line[2048], addr[128], type, name[2048];
#   if defined __APPLE__
   sprintf(nm, "nm --demangle -U %s", O.path);
#   else
   sprintf(nm, "nm --demangle --defined-only %s", O.path);
#   endif
   FILE* f = ::popen(nm, "r");
   if (!f) return;
   while (::fgets(line, sizeof(line) - 1, f)) {
      if (3 != sscanf(line, "%s %c %[^\n]", addr, &type, name)) continue;
      if (strchr("bBcCdDiIuU", type)) continue;
      int _ = 0;
      if (O.os == macOS && !strchr(name, '(')) _ = 1;
      symbols.push_back((new h2_symbol(name + _, (unsigned long long)strtoull(addr, nullptr, 16)))->x);
   }
   ::pclose(f);
   h2_memory::overrides();
#endif
}

h2_inline int h2_nm::get(const char* name, h2_scope res[], int n)
{
#if defined _WIN32
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(O.hProcess, name, symbol))
      return 0;
   res[0].addr = (unsigned long long)symbol->Address;
   res[0].size = (unsigned long long)symbol->Size;
   return 1;
#else
   return 0;
#endif
}

h2_inline unsigned long long h2_nm::get(const char* name)
{
#if defined _WIN32
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (SymFromName(O.hProcess, name, symbol))
      return (unsigned long long)symbol->Address;
   return 0;
#else
   if (!name || strlen(name) == 0) return 0;
   if (!I().symbols_mangled) nm1(I().symbols_mangled);

   auto it = I().symbols_mangled->find(name);
   return it != I().symbols_mangled->end() ? it->second : 0ULL;
#endif
}

static inline bool strncmp_reverse(const char* a, const char* ae, const char* b, const char* be, int n)  // [a, ae) [b, be)
{
   if (ae < a + n || be < b + n) return false;
   return !strncmp(ae - n, be - n, n);
}

h2_inline int h2_nm::find(const char* name, h2_symbol* res[], int n)
{
   if (!name) return 0;
   int len = strlen(name);
   if (len == 0) return 0;
   if (I().symbols_demangled.empty()) nm2(I().symbols_demangled);

   h2_list_for_each_entry (p, I().symbols_demangled, h2_symbol, x) {
      if (!strcmp(p->name.c_str(), name)) {
         res[0] = p;
         return 1;
      }
   }

   int count = 0;
   h2_list_for_each_entry (p, I().symbols_demangled, h2_symbol, x) {
      char* parentheses = strchr((char*)p->name.c_str(), '(');
      if (!parentheses) continue;
      if (!strncmp_reverse(p->name.c_str(), parentheses, name, name + len, len)) continue;  // compare function name
      char* func = parentheses - len;
      if (p->name.c_str() < func && func[-1] != ':' && func[-2] != ':') continue;  // strip namespace
      if (count < n) res[count++] = p;
   }
   return count;
}

struct h2_offset {
   h2_offset() {}
   virtual ~h2_offset() {}
   virtual void dummy() {}

   static long long vtable_offset()
   {
      static long long s = get_vtable_offset();
      return s;
   }

   static long long get_vtable_offset()
   {
      char vtable_symbol[256];
      h2_offset t;
      long long absolute_vtable = (long long)*(void***)&t;
      sprintf(vtable_symbol, "_ZTV%s", typeid(h2_offset).name());  // mangled for "vtable for h2_offset"
      long long relative_vtable = (long long)h2_nm::get(vtable_symbol);
      return absolute_vtable - relative_vtable;
   }

   static long long text_offset()
   {
      static long long s = get_text_offset();
      return s;
   }

   static long long get_text_offset()
   {
      long long relative_vtable = (long long)h2_nm::get("main");
      return (long long)&main - relative_vtable;
   }
};

h2_inline long long h2_nm::text_offset()
{
   return h2_offset::text_offset();
}

h2_inline long long h2_nm::vtable_offset()
{
   return h2_offset::vtable_offset();
}

h2_inline bool h2_nm::in_main(unsigned long long addr)
{
   static unsigned long long main_addr = get("main");
   if (main_addr == 0) return false;
   /* main() 52~60 bytes code in linux MAC */
   return main_addr < addr && addr < main_addr + 128;
}
// source/utils/h2_backtrace.cpp

static inline bool demangle(const char* mangled, char* demangled, size_t len)
{
   int status = 0;
#if !defined _WIN32
   abi::__cxa_demangle(mangled, demangled, &len, &status);
#endif
   return status == 0;
}

#if !defined _WIN32
static inline bool addr2line(unsigned long long addr, char* output, size_t len)
{
   char t[256];
#   if defined __APPLE__
   sprintf(t, "atos -o %s 0x%llx", O.path, addr);
#   else
   sprintf(t, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#   endif
   FILE* f = ::popen(t, "r");
   if (!f) return false;
   output = ::fgets(output, len, f);
   ::pclose(f);
   if (!output) return false;
   for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
   return true;
}
#endif

static inline bool backtrace_extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long long* offset)
{
   //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
   if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %llu", module, mangled, offset)) return true;

   //Linux: with '-rdynamic' linker option
   //Linux: module_name(mangled_function_name+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
   if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%llx", module, mangled, offset)) return true;

   mangled[0] = '\0';

   //Linux: Ubuntu without '-rdynamic' linker option
   //Linux: module_name(+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%llx", module, offset)) return true;

   //Linux: Redhat/CentOS without '-rdynamic' linker option
   //Linux: module_name()[relative_offset_to_module]
   //Linux: `./a.out() [0x40b960]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^[][0x%llx", module, offset)) return true;

   //Where?
   //Linux: module_name[relative_offset_to_module]
   //Linux: `./a.out[0x4060e7]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%llx", module, offset)) return true;

   return false;
}

h2_inline h2_backtrace::h2_backtrace(int shift_) : shift(shift_)
{
   h2_memory::restores();
#ifdef _WIN32
   count = CaptureStackBackTrace(0, sizeof(frames) / sizeof(frames[0]), frames, NULL);
#else
   count = ::backtrace(frames, sizeof(frames) / sizeof(frames[0]));
#endif
   h2_memory::overrides();
}

h2_inline bool h2_backtrace::operator==(const h2_backtrace& bt)
{
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (frames[i] != bt.frames[i])
         return false;
   return true;
}

h2_inline bool h2_backtrace::has(void* func, int size) const
{
   for (int i = 0; i < count; ++i)
      if (func <= frames[i] && (unsigned char*)frames[i] < ((unsigned char*)func) + size)
         return true;
   return false;
}

h2_inline void h2_backtrace::print(h2_vector<h2_string>& stacks) const
{
#ifdef _WIN32
   for (int i = shift; i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      h2_string frame;
      if (SymFromAddr(O.hProcess, (DWORD64)(frames[i]), 0, symbol))
         frame.sprintf("%s %p+%lld ", symbol->Name, symbol->Address, (DWORD64)(frames[i]) - (DWORD64)symbol->Address);
      DWORD dwDisplacement;
      IMAGEHLP_LINE64 fileline;
      SymSetOptions(SYMOPT_LOAD_LINES);
      fileline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
      if (SymGetLineFromAddr64(O.hProcess, (DWORD64)(frames[i]), &dwDisplacement, &fileline))
         frame.sprintf("%s:%d", fileline.FileName, fileline.LineNumber);
      stacks.push_back(frame);
      if (!strcmp("main", symbol->Name) || h2_nm::in_main(symbol->Address))
         break;
   }
#else
   h2_memory::restores();
   char** backtraces = backtrace_symbols(frames, count);
   h2_memory::overrides();

   for (int i = shift; i < count; ++i) {
      char *p = backtraces[i], module[256] = "", mangled[256] = "", demangled[256] = "", addr2lined[512] = "";
      unsigned long long address = 0, offset = 0;
      if (backtrace_extract(backtraces[i], module, mangled, &offset)) {
         if (strlen(mangled)) {
            p = mangled;
            if (demangle(mangled, demangled, sizeof(demangled)))
               if (strlen(demangled))
                  p = demangled;
         }
         if (O.verbose || O.os != macOS /* for speed atos is slow */) {
            address = h2_nm::get(mangled);
            if (address != ULLONG_MAX)
               if (addr2line(address + offset, addr2lined, sizeof(addr2lined)))
                  if (strlen(addr2lined))
                     p = addr2lined;
         }
      }
      stacks.push_back(p);

      if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_nm::in_main(address + offset))
         break;
   }

   h2_memory::restores();
   free(backtraces);
   h2_memory::overrides();
#endif
}

h2_inline void h2_backtrace::print(int pad) const
{
   h2_vector<h2_string> stacks;
   print(stacks);
   h2_rows rows;
   for (auto& c : stacks)
      rows.push_back(c.startswith("h2::") ? gray(c) : h2_row(c));
   rows.sequence(pad);
   h2_color::printl(rows);
}

// source/json/h2_tinyexpr.cpp
// TINYEXPR - Tiny recursive descent parser and evaluation engine in C
// Copyright (c) 2015-2018 Lewis Van Winkle
// http://CodePlea.com

/* clang-format off */

struct tinyexpr
{
    static int TYPE_MASK(int type) { return type & 0x0000001F; }
    static bool IS_PURE(int type) { return (type & TE_FLAG_PURE) != 0; }
    static bool IS_FUNCTION(int type) { return (type & TE_FUNCTION0) != 0; }
    static bool IS_CLOSURE(int type) { return (type & TE_CLOSURE0) != 0; }
    static int ARITY(int type) { return type & (TE_FUNCTION0 | TE_CLOSURE0) ? (type & 0x00000007) : 0; }

    /* Parses the input expression, evaluates it, and frees it. */
    /* Returns NaN on error. */
    static double te_interp(const char *expression, int *error) {
        te_expr *n = te_compile(expression, 0, 0, error);
        double ret;
        if (n) {
            ret = te_eval(n);
            te_free(n);
        } else {
            ret = (NAN);
        }
        return ret;
    }

    struct te_expr {
        int type;
        union {double value; const double *bound; const void *function;};
        void *parameters[1];
    };

    enum {
        TE_VARIABLE = 0,

        TE_FUNCTION0 = 8, TE_FUNCTION1, TE_FUNCTION2, TE_FUNCTION3,
        TE_FUNCTION4, TE_FUNCTION5, TE_FUNCTION6, TE_FUNCTION7,

        TE_CLOSURE0 = 16, TE_CLOSURE1, TE_CLOSURE2, TE_CLOSURE3,
        TE_CLOSURE4, TE_CLOSURE5, TE_CLOSURE6, TE_CLOSURE7,

        TE_FLAG_PURE = 32
    };

    struct te_variable {
        const char *name;
        const void *address;
        int type;
        void *context;
    };

    typedef double (*te_fun2)(double, double);

    enum { TOK_NULL = TE_CLOSURE7+1, TOK_ERROR, TOK_END, TOK_SEP, TOK_OPEN, TOK_CLOSE, TOK_NUMBER, TOK_VARIABLE, TOK_INFIX };

    enum { TE_CONSTANT = 1 };

    struct state {
        const char *start;
        const char *next;
        int type;
        union {double value; const double *bound; const void *function;};
        void *context;

        const te_variable *lookup;
        int lookup_len;
    };

    static te_expr *new_expr(const int type, const te_expr *parameters[]) {
        const int arity = ARITY(type);
        const int psize = sizeof(void*) * arity;
        const int size = (sizeof(te_expr) - sizeof(void*)) + psize + (IS_CLOSURE(type) ? sizeof(void*) : 0);
        te_expr *ret = (te_expr *)h2_libc::malloc(size);
        memset(ret, 0, size);
        if (arity && parameters) {
            memcpy(ret->parameters, parameters, psize);
        }
        ret->type = type;
        ret->bound = 0;
        return ret;
    }

    static void te_free_parameters(te_expr *n) {
        if (!n) return;
        switch (TYPE_MASK(n->type)) {
            case TE_FUNCTION7: case TE_CLOSURE7: te_free((te_expr *)n->parameters[6]);     /* Falls through. */
            case TE_FUNCTION6: case TE_CLOSURE6: te_free((te_expr *)n->parameters[5]);     /* Falls through. */
            case TE_FUNCTION5: case TE_CLOSURE5: te_free((te_expr *)n->parameters[4]);     /* Falls through. */
            case TE_FUNCTION4: case TE_CLOSURE4: te_free((te_expr *)n->parameters[3]);     /* Falls through. */
            case TE_FUNCTION3: case TE_CLOSURE3: te_free((te_expr *)n->parameters[2]);     /* Falls through. */
            case TE_FUNCTION2: case TE_CLOSURE2: te_free((te_expr *)n->parameters[1]);     /* Falls through. */
            case TE_FUNCTION1: case TE_CLOSURE1: te_free((te_expr *)n->parameters[0]);
        }
    }

    static void te_free(te_expr *n) {
        if (!n) return;
        te_free_parameters(n);
        h2_libc::free((void *)n);
    }

    static double _fabs(double x) {return ::fabs(x);}
    static double _cos(double x) {return ::cos(x);}
    static double _acos(double x) {return ::acos(x);}
    static double _cosh(double x) {return ::cosh(x);}
    static double _sin(double x) {return ::sin(x);}
    static double _asin(double x) {return ::asin(x);}
    static double _sinh(double x) {return ::sinh(x);}
    static double _tan(double x) {return ::tan(x);}
    static double _atan(double x) {return ::atan(x);}
    static double _atan2(double x, double y) {return ::atan2(x, y);}
    static double _tanh(double x) {return ::tanh(x);}
    static double _sqrt(double x) {return ::sqrt(x);}
    static double _ln(double x) {return ::log(x);}
    static double _log(double x) {return ::log(x);}
    static double _log10(double x) {return ::log10(x);}
    static double _log2(double x) {return ::log2(x);}
    static double _floor(double x) {return ::floor(x);}
    static double _ceil(double x) {return ::ceil(x);}
    static double _pow(double x, double y) {return ::pow(x, y);}
    static double _exp(double x) {return ::exp(x);}
    static double _fmod(double x, double y) {return ::fmod(x, y);}

    static double pi(void) {return 3.14159265358979323846;}
    static double e(void) {return 2.71828182845904523536;}
    static double fac(double a) {/* simplest version of fac */
        if (a < 0.0) return (NAN);
        if (a > UINT_MAX) return (NAN);
        unsigned int ua = (unsigned int)(a);
        unsigned long int result = 1, i;
        for (i = 1; i <= ua; i++) {
            if (i > ULONG_MAX / result) return (NAN);
            result *= i;
        }
        return (double)result;
    }
    static double ncr(double n, double r) {
        if (n < 0.0 || r < 0.0 || n < r) return (NAN);
        if (n > UINT_MAX || r > UINT_MAX) return (NAN);
        unsigned long int un = (unsigned int)(n), ur = (unsigned int)(r), i;
        unsigned long int result = 1;
        if (ur > un / 2) ur = un - ur;
        for (i = 1; i <= ur; i++) {
            if (result > ULONG_MAX / (un - ur + i)) return (NAN);
            result *= un - ur + i;
            result /= i;
        }
        return result;
    }
    static double npr(double n, double r) {return ncr(n, r) * fac(r);}

    static const te_variable *find_builtin(const char *name, int len) {
        static const te_variable functions[] = {
            /* must be in alphabetical order */
            {"abs", (const void *)_fabs,     TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"acos", (const void *)_acos,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"asin", (const void *)_asin,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"atan", (const void *)_atan,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"atan2", (const void *)_atan2,  TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"ceil", (const void *)_ceil,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"cos", (const void *)_cos,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"cosh", (const void *)_cosh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"e", (const void *)e,          TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"exp", (const void *)_exp,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"fac", (const void *)fac,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"floor", (const void *)_floor,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ln", (const void *)_ln,       TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log", (const void *)_log,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log10", (const void *)_log10,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log2", (const void *)_log2,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ncr", (const void *)ncr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"npr", (const void *)npr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"pi", (const void *)pi,        TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"pow", (const void *)_pow,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"sin", (const void *)_sin,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"sinh", (const void *)_sinh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"sqrt", (const void *)_sqrt,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"tan", (const void *)_tan,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"tanh", (const void *)_tanh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {0, 0, 0, 0}
        };
        int imin = 0;
        int imax = sizeof(functions) / sizeof(te_variable) - 2;

        /*Binary search.*/
        while (imax >= imin) {
            const int i = (imin + ((imax-imin)/2));
            int c = strncmp(name, functions[i].name, len);
            if (!c) c = '\0' - functions[i].name[len];
            if (c == 0) {
                return functions + i;
            } else if (c > 0) {
                imin = i + 1;
            } else {
                imax = i - 1;
            }
        }

        return 0;
    }

    static const te_variable *find_lookup(const state *s, const char *name, int len) {
        int iters;
        const te_variable *var;
        if (!s->lookup) return 0;

        for (var = s->lookup, iters = s->lookup_len; iters; ++var, --iters) {
            if (strncmp(name, var->name, len) == 0 && var->name[len] == '\0') {
                return var;
            }
        }
        return 0;
    }

    static double add(double a, double b) {return a + b;}
    static double sub(double a, double b) {return a - b;}
    static double mul(double a, double b) {return a * b;}
    static double divide(double a, double b) {return a / b;}
    static double negate(double a) {return -a;}
    static double comma(double a, double b) {(void)a; return b;}

    static void next_token(state *s) {
        s->type = TOK_NULL;

        do {

            if (!*s->next){
                s->type = TOK_END;
                return;
            }

            /* Try reading a number. */
            if ((s->next[0] >= '0' && s->next[0] <= '9') || s->next[0] == '.') {
                s->value = strtod(s->next, (char**)&s->next);
                s->type = TOK_NUMBER;
            } else {
                /* Look for a variable or builtin function call. */
                if (s->next[0] >= 'a' && s->next[0] <= 'z') {
                    const char *start;
                    start = s->next;
                    while ((s->next[0] >= 'a' && s->next[0] <= 'z') || (s->next[0] >= '0' && s->next[0] <= '9') || (s->next[0] == '_')) s->next++;

                    const te_variable *var = find_lookup(s, start, s->next - start);
                    if (!var) var = find_builtin(start, s->next - start);

                    if (!var) {
                        s->type = TOK_ERROR;
                    } else {
                        switch(TYPE_MASK(var->type))
                        {
                            case TE_VARIABLE:
                                s->type = TOK_VARIABLE;
                                s->bound = (const double *)var->address;
                                break;

                            case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:         /* Falls through. */
                            case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:         /* Falls through. */
                                s->context = var->context;                                                  /* Falls through. */

                            case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:     /* Falls through. */
                            case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:     /* Falls through. */
                                s->type = var->type;
                                s->function = var->address;
                                break;
                        }
                    }

                } else {
                    /* Look for an operator or special character. */
                    switch (s->next++[0]) {
                        case '+': s->type = TOK_INFIX; s->function = (const void *)add; break;
                        case '-': s->type = TOK_INFIX; s->function = (const void *)sub; break;
                        case '*': s->type = TOK_INFIX; s->function = (const void *)mul; break;
                        case '/': s->type = TOK_INFIX; s->function = (const void *)divide; break;
                        case '^': s->type = TOK_INFIX; s->function = (const void *)_pow; break;
                        // case '%': s->type = TOK_INFIX; s->function = (const void *)fmod; break;
                        case '(': s->type = TOK_OPEN; break;
                        case ')': s->type = TOK_CLOSE; break;
                        case ',': s->type = TOK_SEP; break;
                        case ' ': case '\t': case '\n': case '\r': break;
                        default: s->type = TOK_ERROR; break;
                    }
                }
            }
        } while (s->type == TOK_NULL);
    }

    static te_expr *base(state *s) {
        /* <base>      =    <constant> | <variable> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
        te_expr *ret;
        int arity;

        switch (TYPE_MASK(s->type)) {
            case TOK_NUMBER:
                ret = new_expr(TE_CONSTANT, 0);
                ret->value = s->value;
                next_token(s);
                break;

            case TOK_VARIABLE:
                ret = new_expr(TE_VARIABLE, 0);
                ret->bound = s->bound;
                next_token(s);
                break;

            case TE_FUNCTION0:
            case TE_CLOSURE0:
                ret = new_expr(s->type, 0);
                ret->function = s->function;
                if (IS_CLOSURE(s->type)) ret->parameters[0] = s->context;
                next_token(s);
                if (s->type == TOK_OPEN) {
                    next_token(s);
                    if (s->type != TOK_CLOSE) {
                        s->type = TOK_ERROR;
                    } else {
                        next_token(s);
                    }
                }
                break;

            case TE_FUNCTION1:
            case TE_CLOSURE1:
                ret = new_expr(s->type, 0);
                ret->function = s->function;
                if (IS_CLOSURE(s->type)) ret->parameters[1] = s->context;
                next_token(s);
                ret->parameters[0] = power(s);
                break;

            case TE_FUNCTION2: case TE_FUNCTION3: case TE_FUNCTION4:
            case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
            case TE_CLOSURE2: case TE_CLOSURE3: case TE_CLOSURE4:
            case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
                arity = ARITY(s->type);

                ret = new_expr(s->type, 0);
                ret->function = s->function;
                if (IS_CLOSURE(s->type)) ret->parameters[arity] = s->context;
                next_token(s);

                if (s->type != TOK_OPEN) {
                    s->type = TOK_ERROR;
                } else {
                    int i;
                    for(i = 0; i < arity; i++) {
                        next_token(s);
                        ret->parameters[i] = expr(s);
                        if(s->type != TOK_SEP) {
                            break;
                        }
                    }
                    if(s->type != TOK_CLOSE || i != arity - 1) {
                        s->type = TOK_ERROR;
                    } else {
                        next_token(s);
                    }
                }

                break;

            case TOK_OPEN:
                next_token(s);
                ret = list(s);
                if (s->type != TOK_CLOSE) {
                    s->type = TOK_ERROR;
                } else {
                    next_token(s);
                }
                break;

            default:
                ret = new_expr(0, 0);
                s->type = TOK_ERROR;
                ret->value = (NAN);
                break;
        }

        return ret;
    }

    static te_expr *power(state *s) {
        /* <power>     =    {("-" | "+")} <base> */
        int sign = 1;
        while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
            if (s->function == sub) sign = -sign;
            next_token(s);
        }

        te_expr *ret;

        if (sign == 1) {
            ret = base(s);
        } else {
            const te_expr*_t[] = {base(s)};
            ret = new_expr(TE_FUNCTION1 | TE_FLAG_PURE, _t);
            ret->function = (const void *)negate;
        }

        return ret;
    }

    static te_expr *factor(state *s) {
        /* <factor>    =    <power> {"^" <power>} */
        te_expr *ret = power(s);

        while (s->type == TOK_INFIX && (s->function == _pow)) {
            te_fun2 t = (te_fun2)s->function;
            next_token(s);

            const te_expr*_t[] = {ret, power(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)t;
        }

        return ret;
    }

    static te_expr *term(state *s) {
        /* <term>      =    <factor> {("*" | "/" | "%") <factor>} */
        te_expr *ret = factor(s);

        while (s->type == TOK_INFIX && (s->function == mul || s->function == divide || s->function == (const void *)_fmod)) {
            te_fun2 t = (te_fun2)s->function;
            next_token(s);
            const te_expr*_t[] = {ret, factor(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)t;
        }

        return ret;
    }

    static te_expr *expr(state *s) {
        /* <expr>      =    <term> {("+" | "-") <term>} */
        te_expr *ret = term(s);

        while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
            te_fun2 t = (te_fun2)s->function;
            next_token(s);
            const te_expr*_t[] = {ret, term(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)t;
        }

        return ret;
    }

    static te_expr *list(state *s) {
        /* <list>      =    <expr> {"," <expr>} */
        te_expr *ret = expr(s);

        while (s->type == TOK_SEP) {
            next_token(s);
            const te_expr*_t[] = {ret, expr(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)comma;
        }

        return ret;
    }

#define TE_F(...) ((double(*)(__VA_ARGS__))n->function)
#define TE_M(e) te_eval((const te_expr *)n->parameters[e])

    static double te_eval(const te_expr *n) {
        if (!n) return (NAN);

        switch(TYPE_MASK(n->type)) {
            case TE_CONSTANT: return n->value;
            case TE_VARIABLE: return *n->bound;

            case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
            case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
                switch(ARITY(n->type)) {
                    case 0: return TE_F(void)();
                    case 1: return TE_F(double)(TE_M(0));
                    case 2: return TE_F(double, double)(TE_M(0), TE_M(1));
                    case 3: return TE_F(double, double, double)(TE_M(0), TE_M(1), TE_M(2));
                    case 4: return TE_F(double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3));
                    case 5: return TE_F(double, double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4));
                    case 6: return TE_F(double, double, double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5));
                    case 7: return TE_F(double, double, double, double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5), TE_M(6));
                    default: return (NAN);
                }

            case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
            case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
                switch(ARITY(n->type)) {
                    case 0: return TE_F(void*)(n->parameters[0]);
                    case 1: return TE_F(void*, double)(n->parameters[1], TE_M(0));
                    case 2: return TE_F(void*, double, double)(n->parameters[2], TE_M(0), TE_M(1));
                    case 3: return TE_F(void*, double, double, double)(n->parameters[3], TE_M(0), TE_M(1), TE_M(2));
                    case 4: return TE_F(void*, double, double, double, double)(n->parameters[4], TE_M(0), TE_M(1), TE_M(2), TE_M(3));
                    case 5: return TE_F(void*, double, double, double, double, double)(n->parameters[5], TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4));
                    case 6: return TE_F(void*, double, double, double, double, double, double)(n->parameters[6], TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5));
                    case 7: return TE_F(void*, double, double, double, double, double, double, double)(n->parameters[7], TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5), TE_M(6));
                    default: return (NAN);
                }

            default: return (NAN);
        }

    }

#undef TE_F
#undef TE_M

    static void optimize(te_expr *n) {
        /* Evaluates as much as possible. */
        if (n->type == TE_CONSTANT) return;
        if (n->type == TE_VARIABLE) return;

        /* Only optimize out functions flagged as pure. */
        if (IS_PURE(n->type)) {
            const int arity = ARITY(n->type);
            int known = 1;
            int i;
            for (i = 0; i < arity; ++i) {
                optimize((te_expr*)n->parameters[i]);
                if (((te_expr*)(n->parameters[i]))->type != TE_CONSTANT) {
                    known = 0;
                }
            }
            if (known) {
                const double value = te_eval(n);
                te_free_parameters(n);
                n->type = TE_CONSTANT;
                n->value = value;
            }
        }
    }

    static te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error) {
        state s;
        s.start = s.next = expression;
        s.lookup = variables;
        s.lookup_len = var_count;

        next_token(&s);
        te_expr *root = list(&s);

        if (s.type != TOK_END) {
            te_free(root);
            if (error) {
                *error = (s.next - s.start);
                if (*error == 0) *error = 1;
            }
            return 0;
        } else {
            optimize(root);
            if (error) *error = 0;
            return root;
        }
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
   h2_string key_string;
   h2_string value_string;
   double value_double = 0;
   bool value_boolean = false;
   h2_list children, x; /* array or object */

   ~h2_json_node()
   {
      h2_list_for_each_entry (p, children, h2_json_node, x) {
         p->x.out();
         delete p;
      }
   }

   int size()
   {
      return children.count();
   }

   h2_json_node* get(int index)
   {
      if (index < 0) index = children.count() + index;
      h2_list_for_each_entry (p, i, children, h2_json_node, x)
         if (i == index)
            return p;
      return nullptr;
   }

   h2_json_node* get(const h2_string& name, bool caseless)
   {
      h2_list_for_each_entry (p, children, h2_json_node, x)
         if (p->key_string.equals(name, caseless))
            return p;
      return nullptr;
   }

   bool is_null() { return t_null == type; }
   bool is_bool() { return t_boolean == type; }
   bool is_number() { return t_number == type; }
   bool is_string() { return t_string == type; }
   bool is_pattern() { return t_pattern == type; }
   bool is_array() { return t_array == type; }
   bool is_object() { return t_object == type; }

   void dual(int& _type, const char*& _class, h2_string& _key, h2_string& _value)
   {
      if (key_string.size()) _key = "\"" + key_string + "\"";
      _type = type;
      switch (type) {
      case t_null:
         _class = "atomic";
         _value = "null";
         break;
      case t_boolean:
         _class = "atomic";
         _value = value_boolean ? "true" : "false";
         break;
      case t_number:
         _class = "atomic";
         if (value_double - ::floor(value_double) == 0)
            _value = std::to_string((long long)value_double).c_str();
         else
            _value = std::to_string(value_double).c_str();
         break;
      case t_string:
         _class = "atomic";
         _value = "\"" + value_string + "\"";
         break;
      case t_pattern:
         _class = "atomic";
         _value = value_string;
         break;
      case t_array:
         _class = "array";
         break;
      case t_object:
         _class = "object";
         break;
      }
   }

   h2_string slash_if(bool slash) { return slash ? "\\" : ""; }

   void print(h2_rows& rows, bool fold = false, bool slash = false, int depth = 0, int next = 0)
   {
      h2_row row;
      row.indent(depth * 2);
      if (key_string.size())
         row.push_back(slash_if(slash) + "\"" + key_string + slash_if(slash) + "\": ");
      if (is_null())
         row.push_back("null");
      else if (is_bool())
         row.push_back(value_boolean ? "true" : "false");
      else if (is_number()) {
         if (value_double - ::floor(value_double) == 0)
            row.push_back(std::to_string((long long)value_double).c_str());
         else
            row.push_back(std::to_string(value_double).c_str());
      } else if (is_string())
         row.push_back(slash_if(slash) + "\"" + value_string + slash_if(slash) + "\"");
      else if (is_pattern())
         row.push_back(slash_if(slash) + "\"/" + value_string + "/" + slash_if(slash) + "\"");
      else if (is_array() || is_object()) {
         h2_rows children_rows;
         h2_list_for_each_entry (p, i, children, h2_json_node, x)
            p->print(children_rows, fold, slash, depth + 1, children.count() - i - 1);

         row.push_back(is_array() ? "[" : "{");
         if (fold && children_rows.foldable()) {
            row += children_rows.folds();
         } else {
            rows.push_back(row), row.clear();
            rows += children_rows;
            row.indent(depth * 2);
         }
         row.push_back(is_array() ? "]" : "}");
      }
      if (row.size()) {
         if (next) row.push_back(", ");
         rows.push_back(row), row.clear();
      }
   }
};
// source/json/h2_lexical.cpp

struct h2_json_lexical {
   static void new_lexis(h2_vector<h2_string>& lexical, const char* start, int size)
   {
      const char *left = start, *right = start + size;
      for (; left < right && *left && ::isspace(*left);) left++;
      for (; left < right - 1 && ::isspace(*(right - 1));) right--;
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
      for (p = json_string; *p && json_length--; p++) {
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
               new_lexis(lexical, pending, (p + 1) - pending);
               pending = nullptr;
               state = st_idle;
            } else if ('\\' == *p) {
               stash_state = state, state = st_escape;
            }
            break;
         case st_double_quote:
            if ('\"' == *p) {
               new_lexis(lexical, pending, (p + 1) - pending);
               pending = nullptr;
               state = st_idle;
            } else if ('\\' == *p) {
               stash_state = state, state = st_escape;
            }
            break;
         case st_pattern:
            if ('/' == *p) {
               new_lexis(lexical, pending, (p + 1) - pending);
               pending = nullptr;
               state = st_idle;
            }
            /* no escape char */
            break;
         case st_normal:
            if (strchr("{:}[,]", *p)) {
               new_lexis(lexical, pending, p - pending);
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
         new_lexis(lexical, pending, p - pending);
      }
   }
};
// source/json/h2_syntax.cpp

struct h2_json_syntax {
   int i = 0;
   const h2_vector<h2_string>& lexical;
   h2_json_syntax(const h2_vector<h2_string>& _lexical) : lexical(_lexical) {}

   bool parse(h2_json_node& root_node)
   {
      if (!parse_value(root_node)) return false;
      return lexical.size() <= i;  // nothing more, "{},{}"
   }

   h2_string& filter_string(h2_string& s)
   {
      if (s.enclosed('\"'))
         s = s.unquote('\"');
      else if (s.enclosed('\''))
         s = s.unquote('\'');
      s = s.unescape();
      return s;
   }

   bool interpret_number(const h2_string& s, double& value)
   {
      int err = 0;
      value = tinyexpr::te_interp(s.c_str(), &err);
      return 0 == err;
   }

   bool requires(const char* s)
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
         node.value_string = node.value_string.unquote('/');
      node.type = h2_json_node::t_pattern;
      return true;
   }

   bool parse_string_or_number(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (interpret_number(node.value_string, node.value_double)) {
         node.type = h2_json_node::t_number;
         return true;
      }
      filter_string(node.value_string);
      node.type = h2_json_node::t_string;
      return true;
   }

   bool parse_array(h2_json_node& node)
   {
      if (!requires("[")) return false;
      while (i < lexical.size() && !lexical[i].equals("]")) {
         h2_json_node* new_node = new h2_json_node();
         node.children.push_back(new_node->x);
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            i++;
         else
            break;
      }

      if (!requires("]")) return false;
      node.type = h2_json_node::t_array;
      return true;
   }

   bool parse_object(h2_json_node& node)
   {
      if (!requires("{")) return false;
      while (i < lexical.size() && !lexical[i].equals("}")) {
         h2_json_node* new_node = new h2_json_node();
         node.children.push_back(new_node->x);
         if (!parse_key(*new_node)) return false;
         if (!requires(":")) return false;
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            ++i;
         else
            break;
      }

      if (!requires("}")) return false;
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
      const int st_idle = 0;
      const int st_in_dot = 1;
      const int st_in_bracket = 2;
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
               if (s < p) add(s, p - 1, true);
               // restart a new part
               state = st_in_dot;
               s = p + 1;
            } else if (*p == '[') {  // end a part
               if (s < p) add(s, p - 1, true);
               // restart a new part
               state = st_in_bracket;
               s = p + 1;
            } else if (*p == '\0') {
               if (s < p) add(s, p - 1, true);
               state = st_idle;
            }
            break;
         case st_in_bracket:
            if (*p == ']') {
               if (s < p) add(s, p - 1, false);
               state = st_idle;
            }
            break;
         }
      } while (*p++);
   }

   void add(const char* start, const char* end, bool only_key)
   {
      for (; start <= end && ::isspace(*start);) start++;  //strip left space
      for (; start <= end && ::isspace(*end);) end--;      //strip right space
      if (start <= end) {
         if (!only_key) {
            if (strspn(start, "-0123456789") == end - start + 1) {
               values.push_back({atoi(start), ""});
               return;
            } else if ((*start == '\"' && *end == '\"') || (*start == '\'' && *end == '\'')) {
               start++, end--;
            }
         }
         if (start <= end) values.push_back({0, h2_string(end - start + 1, start)});
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
      node->key_string = "";
      return node;
   }

   h2_row serialize()
   {
      h2_row row;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            row.printf("yellow,bold,underline", "%s%s ", comma_if(j, " "), lexical[j].c_str());
         else
            row.push_back(comma_if(j, " ") + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) {
         row.printf("yellow,bold,underline", " ... ");
      }
      return row;
   }

   h2_rows format()
   {
      h2_rows rows;
      print(rows, O.fold_json, O.copy_paste_json);
      return rows;
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
         return a->is_number() && ::fabs(e->value_double - a->value_double) < 0.00001;
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

   static h2_json_node* search(const h2_list& haystack, h2_json_node* needle, bool caseless)
   {
      h2_list_for_each_entry (p, haystack, h2_json_node, x)
         if (match(needle, p, caseless))
            return p;
      return nullptr;
   }
};
// source/json/h2_dual.cpp

struct h2_json_dual : h2_libc {  // combine two node into a dual
   bool key_equal = false, match = false;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   const char *e_class = "blob", *a_class = "blob";
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_rows e_blob, a_blob;
   h2_list children, x;
   h2_json_dual* perent;
   int depth;

   ~h2_json_dual()
   {
      h2_list_for_each_entry (p, children, h2_json_dual, x) {
         p->x.out();
         delete p;
      }
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, bool caseless, h2_json_dual* perent_ = nullptr) : perent(perent_), depth(perent_ ? perent_->depth + 1 : 0)
   {
      match = h2_json_match::match(e, a, caseless);
      if (e) e->dual(e_type, e_class, e_key, e_value);
      if (a) a->dual(a_type, a_class, a_key, a_value);
      key_equal = e_key.equals(a_key, caseless);

      if (strcmp(e_class, a_class)) {
         if (e) e->print(e_blob, O.fold_json, false, depth);
         if (a) a->print(a_blob, O.fold_json, false, depth);
         e_class = a_class = "blob";
      } else if (!strcmp("object", e_class)) {
         h2_list_for_each_entry (_e, e->children, h2_json_node, x) {
            h2_json_node* _a = a->get(_e->key_string, false);
            if (!_a) _a = a->get(_e->key_string, caseless);
            if (!_a) _a = h2_json_match::search(a->children, _e, caseless);
            if (_a) {
               children.push_back((new h2_json_dual(_e, _a, caseless, this))->x);
               _e->x.out();
               delete _e;
               _a->x.out();
               delete _a;
            }
         }

         for (int i = 0; i < std::max(e->size(), a->size()); ++i)
            children.push_back((new h2_json_dual(e->get(i), a->get(i), caseless, this))->x);
      } else if (!strcmp("array", e_class)) {
         for (int i = 0; i < std::max(e->size(), a->size()); ++i)
            children.push_back((new h2_json_dual(e->get(i), a->get(i), caseless, this))->x);
      }
   }

   bool has_next(h2_list* subling, bool expect) const
   {
      if (subling) {
         for (h2_list* p = x.next; p != subling; p = p->next) {
            h2_json_dual* d = h2_list_entry(p, h2_json_dual, x);
            if ((expect ? d->e_type : d->a_type) != h2_json_node::t_absent)
               return true;
         }
      }
      return false;
   }

   void align(h2_rows& e_rows, h2_rows& a_rows, h2_list* subling = nullptr)
   {
      if (!strcmp(e_class, "blob")) {
         e_blob.samesizify(a_blob);
         for (auto& row : e_blob) row.brush("cyan");
         for (auto& row : a_blob) row.brush("yellow");

         e_rows += e_blob;
         a_rows += a_blob;
         return;
      }

      h2_row e_row, a_row;
      e_row.indent(depth * 2);
      a_row.indent(depth * 2);

      if (e_key.size()) {
         if (!key_equal) e_row.push_back("\033{green}");
         e_row.push_back(e_key);
         if (!key_equal) e_row.push_back("\033{reset}");
         e_row.push_back(": ");
      }

      if (a_key.size()) {
         if (!key_equal) a_row.push_back("\033{red,bold}");
         a_row.push_back(a_key);
         if (!key_equal) a_row.push_back("\033{reset}");
         a_row.push_back(": ");
      }

      if (!strcmp(e_class, "atomic")) {
         if (e_value.size()) {
            if (!match) e_row.push_back("\033{green}");
            e_row.push_back(e_value);
            if (!match) e_row.push_back("\033{reset}");
         }
         if (a_value.size()) {
            if (!match) a_row.push_back("\033{red,bold}");
            a_row.push_back(a_value);
            if (!match) a_row.push_back("\033{reset}");
         }
      } else if (!strcmp(e_class, "object") || !strcmp(e_class, "array")) {
         h2_rows e_children_rows, a_children_rows;
         h2_list_for_each_entry (p, children, h2_json_dual, x)
            p->align(e_children_rows, a_children_rows, &children);

         e_row.push_back(strcmp(e_class, "object") ? "[" : "{");
         a_row.push_back(strcmp(a_class, "object") ? "[" : "{");
         if (O.fold_json && e_children_rows.foldable() && a_children_rows.foldable()) {
            e_row += e_children_rows.folds();
            a_row += a_children_rows.folds();
         } else {
            e_rows.push_back(e_row), e_row.clear();
            e_rows += e_children_rows;
            e_row.indent(depth * 2);
            a_rows.push_back(a_row), a_row.clear();
            a_rows += a_children_rows;
            a_row.indent(depth * 2);
         }
         e_row.push_back(strcmp(e_class, "object") ? "]" : "}");
         a_row.push_back(strcmp(a_class, "object") ? "]" : "}");
      }
      if (e_row.size()) {
         if (has_next(subling, true)) e_row.push_back(", ");
         e_rows.push_back(e_row), e_row.clear();
      }
      if (a_row.size()) {
         if (has_next(subling, false)) a_row.push_back(", ");
         a_rows.push_back(a_row), a_row.clear();
      }
   }
};
// source/json/h2_json.cpp

h2_inline h2_rows h2_json::format(const h2_string& json_string)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return {tree.serialize()};
   h2_rows rows = tree.format();
   if (O.copy_paste_json) {
      if (!rows.empty()) {
         rows.front() = "\"" + rows.front();
         rows.back() = rows.back() + "\"";
      }
      unsigned max_width = rows.width();
      for (size_t i = 0; i < rows.size(); ++i) {
         rows[i].padding(max_width - rows[i].width() + 3);
         if (i < rows.size() - 1) rows[i].push_back("\\");
      }
   }
   return rows;
}

h2_inline h2_string h2_json::select(const h2_string& json_string, const h2_string& selector, bool caseless)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return json_string;
   h2_json_node* node = tree.select(selector.c_str(), caseless);
   if (!node) return "";
   h2_rows rows;
   node->print(rows, O.fold_json, false);
   return rows.string();
}

h2_inline int h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return -1;
   return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_rows& e_lines, h2_rows& a_lines, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return false;
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_lines, a_lines);
   return true;
}

// source/matcher/h2_strcmp.cpp

h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expection(caseless, dont));
}
h2_inline h2_row h2_matches_regex::expection(bool caseless, bool dont) const
{
   return CD("Re" + gray("(") + h2_stringify(e) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expection(caseless, dont));
}
h2_inline h2_row h2_matches_wildcard::expection(bool caseless, bool dont) const
{
   return CD("We" + gray("(") + h2_stringify(e) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.equals(e, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expection(caseless, dont));
}
h2_inline h2_row h2_matches_strcmp::expection(bool caseless, bool dont) const
{
   return CD(h2_representify(e), caseless, dont, "≠");
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.contains(substring, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(substring, a, expection(caseless, dont));
}
h2_inline h2_row h2_matches_substr::expection(bool caseless, bool dont) const
{
   return CD("Substr" + gray("(") + h2_representify(substring) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expection(caseless, dont));
}
h2_inline h2_row h2_matches_startswith::expection(bool caseless, bool dont) const
{
   return CD("StartsWith" + gray("(") + h2_representify(prefix_string) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expection(caseless, dont));
}
h2_inline h2_row h2_matches_endswith::expection(bool caseless, bool dont) const
{
   return CD("EndsWith" + gray("(") + h2_representify(suffix_string) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, int, bool caseless, bool dont) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, caseless);
   int ret = h2_json::match(e, _a, caseless);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(caseless, dont), caseless, "illformed json");
   if ((ret == 0) == !dont) return nullptr;
   return h2_fail::new_json(e, _a, expection(caseless, dont), caseless, DS(dont));
}
h2_inline h2_row h2_matches_json::expection(bool caseless, bool dont) const
{
   return CD(h2_stringify(e), caseless, dont, "≠");
}
// source/matcher/h2_memcmp.cpp

static inline h2_string readable_size(int width, int nbits)
{
   char t[64];
   switch (width) {
   case 1: sprintf(t, "%d bit%s", nbits, nbits > 1 ? "s" : ""); break;
   case 8: sprintf(t, "%d byte%s", nbits / 8, nbits / 8 > 1 ? "s" : ""); break;
   case 16: sprintf(t, "%d word%s", nbits / 16, nbits / 16 > 1 ? "s" : ""); break;
   case 32: sprintf(t, "%d dword%s", nbits / 32, nbits / 32 > 1 ? "s" : ""); break;
   case 64: sprintf(t, "%d qword%s", nbits / 64, nbits / 64 > 1 ? "s" : ""); break;
   default: sprintf(t, "?"); break;
   }
   return h2_string(t);
}

h2_inline h2_fail* h2_matches_bytecmp::matches(const void* a, int n, bool caseless, bool dont) const
{
   bool result = false;
   int _nbytes;
   if (isstring) {
      if (h2_numeric::is_hex_string((const char*)e)) {
         unsigned char* _e = (unsigned char*)alloca(strlen((const char*)e));
         int max_length = h2_numeric::hex_to_bytes((const char*)e, _e);
         _nbytes = nbytes;
         if (nbytes == 0) _nbytes = max_length;
         if (_nbytes <= max_length) {
            result = memcmp(_e, a, _nbytes) == 0;
         }
      } else {
         _nbytes = strlen((const char*)e);
      }
   } else {
      if (nbytes == 0) {
         return h2_fail::new_normal("length required");
      }
   }
   if (!result) {
      _nbytes = nbytes;
      result = memcmp(e, a, _nbytes) == 0;
   }
   if (result == !dont) return nullptr;
   return h2_fail::new_memcmp((const unsigned char*)e, (const unsigned char*)a, width, _nbytes * 8, h2_stringify(a).string(), "memcmp " + readable_size(width, _nbytes * 8));
}

h2_inline h2_row h2_matches_bytecmp::expection(bool caseless, bool dont) const
{
   return CD("Me()", caseless, dont);
}

h2_inline h2_fail* h2_matches_bitcmp::matches(const void* a, int n, bool caseless, bool dont) const
{
   int max_length = INT_MAX;
   unsigned char* _e = (unsigned char*)e;
   if (isstring) {
      unsigned char* t = (unsigned char*)alloca(strlen((const char*)e));
      if (h2_numeric::is_bin_string((const char*)e)) {
         max_length = h2_numeric::bin_to_bits((const char*)e, t);
         _e = t;
      } else if (h2_numeric::is_hex_string((const char*)e)) {
         max_length = h2_numeric::hex_to_bits((const char*)e, t);
         _e = t;
      } else {
         max_length = strlen((const char*)e) * 8;
      }
   } else {
      if (nbits == 0) {
         return h2_fail::new_normal("length required");
      }
   }
   int _nbits = nbits;
   if (nbits == 0) _nbits = max_length;
   if (max_length < _nbits) {
      return h2_fail::new_normal("length too loog");
   }
   bool result = h2_numeric::bits_equal(_e, (const unsigned char*)a, _nbits);
   if (result == !dont) return nullptr;
   return h2_fail::new_memcmp(_e, (const unsigned char*)a, 1, _nbits, h2_stringify(a).string(), "memcmp " + readable_size(1, _nbits));
}

h2_inline h2_row h2_matches_bitcmp::expection(bool caseless, bool dont) const
{
   return CD("Me()", caseless, dont);
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
   void* violate_address{nullptr};
   const char* violate_action = "";
   bool violate_after_free = false;
   h2_backtrace violate_backtrace;

   h2_piece(size_t size, size_t alignment, const char* who, h2_backtrace& bt)
     : user_size(size), page_size(h2_page_size()), who_allocate(who), bt_allocate(bt)
   {
      size_t alignment_2n = alignment;
      if (h2_numeric::not2n(alignment)) alignment_2n = h2_numeric::mask2n(alignment) + 1;
      if (alignment_2n < sizeof(void*)) alignment_2n = sizeof(void*);

      size_t user_size_plus = (user_size + alignment_2n - 1 + alignment_2n) & ~(alignment_2n - 1);
      page_count = ::ceil(user_size_plus / (double)page_size);

#if defined _WIN32
      page_ptr = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if (page_ptr == NULL) ::printf("VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      page_ptr = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (page_ptr == MAP_FAILED) h2_color::prints("yellow", "mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
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
         ::printf("VirtualProtect failed %d\n", GetLastError());
#else
      int new_permission = PROT_NONE;
      if (permission & readable)
         new_permission = PROT_READ;
      if (permission & writable)
         new_permission = PROT_READ | PROT_WRITE;
      if (::mprotect(forbidden_page, forbidden_size, new_permission) != 0)
         h2_color::prints("yellow", "mprotect failed %s\n", strerror(errno));
#endif
   }

   void violate_forbidden(void* addr)
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
      h2_backtrace bt(3);
      if (!violate_times++) { /* 只记录第一犯罪现场 */
         set_forbidden(readable);
         violate_backtrace = bt;
         violate_address = addr;
         violate_action = "read";
         violate_after_free = 0 < free_times;
      } else {
         set_forbidden(readable | writable);
         if (bt == violate_backtrace) /* 是第一犯罪现场 */
            violate_action = "write";
      }
   }

   void mark_snowfield()
   {
      static unsigned char s_snow_flower = 0;
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
         return h2_fail::new_use_after_free(user_ptr, violate_address, violate_action, bt_allocate, bt_release, violate_backtrace);
      else
         return h2_fail::new_overflow(user_ptr, user_size, violate_address, violate_action, h2_vector<unsigned char>(), bt_allocate, violate_backtrace);
   }

   h2_fail* check_asymmetric_free(const char* who_release)
   {
      static const char* free_a[] = {"malloc", "calloc", "realloc", "strdup", "reallocf", "posix_memalign", "memalign", "aligned_alloc", "valloc", "pvalloc", nullptr};
      static const char* free_r[] = {"free", nullptr};
      static const char* new_a[] = {"new", "new nothrow", nullptr};
      static const char* new_r[] = {"delete", nullptr};
      static const char* news_a[] = {"new[]", "new[] nothrow", nullptr};
      static const char* news_r[] = {"delete[]", nullptr};
      static const char* _aligned_a[] = {"_aligned_malloc", "_aligned_realloc", "_aligned_recalloc", "_aligned_offset_malloc", "_aligned_offset_realloc", "_aligned_offset_recalloc", nullptr};
      static const char* _aligned_r[] = {"_aligned_free", nullptr};
      static const char* HeapAlloc_a[] = {"HeapAlloc", nullptr};
      static const char* HeapFree_r[] = {"HeapFree", nullptr};
      static const char* VirtualAlloc_a[] = {"VirtualAlloc", nullptr};
      static const char* VirtualFree_r[] = {"VirtualFree", nullptr};
      static struct {
         const char **a, **r;
      } S[] = {{free_a, free_r}, {new_a, new_r}, {news_a, news_r}, {_aligned_a, _aligned_r}, {HeapAlloc_a, HeapFree_r}, {VirtualAlloc_a, VirtualFree_r}};

      for (int i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
         if (h2_in(who_allocate, S[i].a) && h2_in(who_release, S[i].r))
            return nullptr;

      h2_backtrace bt(O.os == macOS ? 6 : 5);
      return h2_fail::new_asymmetric_free(user_ptr, who_allocate, who_release, bt_allocate, bt);
   }

   h2_fail* check_double_free()
   {
      h2_fail* fail = nullptr;
      h2_backtrace bt(O.os == macOS ? 6 : 5);
      if (free_times++ == 0)
         bt_release = bt;
      else
         fail = h2_fail::new_double_free(user_ptr, bt_allocate, bt_release, bt);
      return fail;
   }

   h2_fail* free(const char* who_release)
   {
      h2_fail* fail = nullptr;
      if (!fail)
         fail = check_double_free();
      if (!fail)
         fail = check_asymmetric_free(who_release);
      if (!fail)
         fail = check_snowfield();

      if (!fail) set_forbidden(0, page_ptr, page_size * (page_count + 1));

      return fail;
   }

   bool in_page_range(const unsigned char* p)
   {
      return page_ptr <= p && p < page_ptr + page_size * (page_count + 1);
   }
};
// source/memory/h2_leaky.cpp

struct h2_leaky {
   struct leak {
      void* ptr;
      h2_backtrace bt;
      h2_vector<std::pair<int, int>> sizes;

      leak(void* _ptr, const h2_backtrace& _bt) : ptr(_ptr), bt(_bt) {}

      h2_vector<std::pair<int, int>>::iterator find(int size)
      {
         for (auto it = sizes.begin(); it != sizes.end(); it++)
            if (it->first == size)
               return it;
         return sizes.end();
      }

      void add(int size)
      {
         if (sizes.end() == find(size)) sizes.push_back({size, 0});
         find(size)->second++;
      }

      h2_fail* check(const char* where, const char* file, int line)
      {
         int s = 0;
         for (auto& p : sizes)
            s += p.first * p.second;
         return h2_fail::new_memory_leak(ptr, s, sizes, bt, where, file, line);
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

   void add(void* ptr, int size, const h2_backtrace& bt)
   {
      if (leaks.end() == find(bt)) leaks.push_back({ptr, bt});
      find(bt)->add(size);
   }

   h2_fail* check(const char* where, const char* file, int line)
   {
      h2_fail* fails = nullptr;
      for (auto& p : leaks) h2_fail::append_subling(fails, p.check(where, file, line));
      return fails;
   }
};
// source/memory/h2_block.cpp

struct h2_block : h2_libc {
   h2_list x;
   h2_list pieces;

   long long footprint = 0, allocated = 0;
   long long limit;
   size_t align;
   unsigned char s_fill[32];
   int n_fill;
   bool noleak;  // ignore leak check
   const char* where;
   const char* file;
   int line;

   h2_block(long long _limit, size_t _align, unsigned char _s_fill[32], int _n_fill, bool _noleak, const char* _where, const char* _file, int _line)
     : limit(_limit), align(_align), n_fill(_n_fill), noleak(_noleak), where(_where), file(_file), line(_line) { memcpy(s_fill, _s_fill, _n_fill); }

   h2_fail* check()
   {
      h2_fail* fails = nullptr;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->violate_times)
            h2_fail::append_subling(fails, p->violate_fail());

      if (fails) return fails;

      h2_leaky leaky;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (!noleak && !p->free_times)
            leaky.add(p->user_ptr, p->user_size, p->bt_allocate);

      fails = leaky.check(where, file, line);
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
      if (limit < allocated + size) return nullptr;
      allocated += size;
      if (footprint < allocated) footprint = allocated;

      // allocate action alignment is prior to block level alignment
      if (alignment == 0) alignment = align;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      // allocate action fill is prior to block level fill
      unsigned char* s_filling = s_fill;
      int n_filling = n_fill;
      if (fill) {
         s_filling = &c_fill;
         n_filling = 1;
      }
      if (0 < n_filling)
         for (int i = 0, j = 0; i < size; ++i, ++j)
            ((unsigned char*)p->user_ptr)[i] = s_filling[j % n_filling];

      pieces.push_back(p->x);
      return p;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      allocated -= p->user_size;
      return p->free(who);
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->user_ptr == ptr) return p;
      return nullptr;
   }

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->in_page_range((const unsigned char*)addr)) return p;
      return nullptr;
   }
};
// source/memory/h2_stack.cpp

struct h2_stack {
   h2_singleton(h2_stack);
   h2_list blocks;

   void push(long long limit, size_t align, unsigned char s_fill[32], int n_fill, bool noleak, const char* where, const char* file, int line)
   {
      h2_block* b = new h2_block(limit, align, s_fill, n_fill, noleak, where, file, line);
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
      h2_backtrace bt(O.os == macOS ? 3 : 2);
      h2_block* b = h2_exempt::in(bt) ? h2_list_bottom_entry(blocks, h2_block, x) : h2_list_top_entry(blocks, h2_block, x);
      return b ? b->new_piece(who, size, alignment, fill ? *fill : 0, fill, bt) : nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return p->rel_piece(who, piece);
      }
      h2_backtrace bt(O.os == macOS ? 3 : 2);
      if (!h2_exempt::in(bt))
         h2_debug("Warning: %s %p not found!", who, ptr);
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

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->host_piece(addr);
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
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("free", ptr), false);
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
      if (size == 0 && O.os == macOS) size = 1;
      if (ptr) old_p = h2_stack::I().get_piece(ptr);
      if (size) new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (old_p && new_p) memcpy(new_p->user_ptr, old_p->user_ptr, std::min(old_p->user_size, size));
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("free", ptr), false);
      return new_p ? new_p->user_ptr : nullptr;
   }
   static char* strdup(char* s)
   {
      h2_piece* p = h2_stack::I().new_piece("strdup", strlen(s) + 1, 0, nullptr);
      char* ret = p ? (char*)p->user_ptr : nullptr;
      if (ret) strcpy(ret, s);
      return ret;
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
   static void* _aligned_malloc(size_t size, size_t alignment)
   {
      h2_piece* p = h2_stack::I().new_piece("_aligned_malloc", size, alignment, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void _aligned_free(void* memblock)
   {
      if (memblock) h2_fail_g(h2_stack::I().rel_piece("_aligned_free", memblock), false);
   }
   static void* valloc(size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("valloc", size, h2_page_size(), nullptr);
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
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete", ptr), false);
   }
   static void operator delete(void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete", ptr), false);
   }
   static void operator delete[](void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[]", ptr), false);
   }
   static void operator delete[](void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[]", ptr), false);
   }
};
#if defined __GLIBC__
// source/memory/h2_wrapper_linux.cpp

// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html

struct h2_wrapper_specific {
   static void free_hook(void* __ptr, const void* caller) { h2_override::free(__ptr); }
   static void* malloc_hook(size_t __size, const void* caller) { return h2_override::malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return h2_override::realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return h2_override::aligned_alloc(__alignment, __size); }

   void (*saved__free_hook)(void*, const void*);
   void* (*saved__malloc_hook)(size_t, const void*);
   void* (*saved__realloc_hook)(void*, size_t, const void*);
   void* (*saved__memalign_hook)(size_t, size_t, const void*);

   h2_wrapper_specific()
   {
      saved__free_hook = __free_hook;
      saved__malloc_hook = __malloc_hook;
      saved__realloc_hook = __realloc_hook;
      saved__memalign_hook = __memalign_hook;
   }

   void overrides()
   {
      __free_hook = free_hook;
      __malloc_hook = malloc_hook;
      __realloc_hook = realloc_hook;
      __memalign_hook = memalign_hook;
   }

   void restores()
   {
      __free_hook = saved__free_hook;
      __malloc_hook = saved__malloc_hook;
      __realloc_hook = saved__realloc_hook;
      __memalign_hook = saved__memalign_hook;
   }
};
#elif defined __APPLE__
// #   include "memory/h2_wrapper_macos1.cpp"
// source/memory/h2_wrapper_macos2.cpp

// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
#endif

struct h2_wrapper_specific {
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

   h2_wrapper_specific()
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

   void overrides()
   {
      malloc_zone_register(&mz);
      malloc_zone_t* default_zone = get_default_zone();
      malloc_zone_unregister(default_zone);
      malloc_zone_register(default_zone);
   }

   void restores()
   {
      malloc_zone_unregister(&mz);
   }
};
// #   include "memory/h2_wrapper_macos3.cpp"
#elif defined _WIN32
// source/memory/h2_wrapper_windows.cpp

struct h2_wrapper_specific {
   h2_stubs stubs;

   void overrides()
   {
      stubs.add((void*)::_aligned_malloc, (void*)h2_override::_aligned_malloc, "_aligned_malloc", __FILE__, __LINE__);
      stubs.add((void*)::strdup, (void*)h2_override::strdup, "strdup", __FILE__, __LINE__);
   }

   void restores()
   {
      stubs.clear();
   }
};
#endif
// source/memory/h2_wrapper.cpp

struct h2_wrapper {
   h2_singleton(h2_wrapper);
   h2_wrapper_specific specific;
   h2_stubs stubs;

   void overrides()
   {
      stubs.add((void*)::free, (void*)h2_override::free, "free", __FILE__, __LINE__);
      stubs.add((void*)::malloc, (void*)h2_override::malloc, "malloc", __FILE__, __LINE__);
      stubs.add((void*)::realloc, (void*)h2_override::realloc, "realloc", __FILE__, __LINE__);
      stubs.add((void*)::calloc, (void*)h2_override::calloc, "calloc", __FILE__, __LINE__);
#if !defined _WIN32
      // _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
      stubs.add((void*)::posix_memalign, (void*)h2_override::posix_memalign, "posix_memalign", __FILE__, __LINE__);
#   if defined _ISOC11_SOURCE
      stubs.add((void*)::aligned_alloc, (void*)h2_override::aligned_alloc, "aligned_alloc", __FILE__, __LINE__);
#   endif
#endif
      // deprecated valloc pvalloc memalign
      stubs.add((void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), "new", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), "new", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), "new[]", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), "new[]", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), "delete", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), "delete", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), "delete[]", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), "delete[]", __FILE__, __LINE__);

      specific.overrides();
   }

   void restores()
   {
      stubs.clear();
      specific.restores();
   }
};
// source/memory/h2_crash.cpp

struct h2_crash {
#ifndef _WIN32
   // https://www.gnu.org/software/libsigsegv/
   static void segment_fault_handler(int sig, siginfo_t* si, void* unused)
   {
      h2_piece* piece = h2_stack::I().host_piece(si->si_addr);
      if (piece) {
         piece->violate_forbidden(si->si_addr);
      } else {
         h2_debug("");
         abort();
      }
   }
#endif

   static void install_segment_fault_handler()
   {
#ifndef _WIN32
      struct sigaction action;
      action.sa_sigaction = segment_fault_handler;
      action.sa_flags = SA_SIGINFO;
      sigemptyset(&action.sa_mask);

      if (sigaction(SIGSEGV, &action, nullptr) == -1) perror("Register SIGSEGV handler failed");
#   ifdef __APPLE__
      if (sigaction(SIGBUS, &action, nullptr) == -1) perror("Register SIGBUS handler failed");
#   endif
#endif
   }
};
// source/memory/h2_memory.cpp

h2_inline void h2_memory::initialize()
{
   if (O.memory_check && !O.debug) h2_crash::install_segment_fault_handler();
   stack::root();
   if (O.memory_check) h2_wrapper::I().overrides();
   if (O.memory_check) h2_exempt::setup();
}
h2_inline void h2_memory::finalize()
{
   if (O.memory_check) h2_wrapper::I().restores();
}
h2_inline void h2_memory::overrides()
{
   if (O.memory_check) h2_wrapper::I().overrides();
}
h2_inline void h2_memory::restores()
{
   if (O.memory_check) h2_wrapper::I().restores();
}

h2_inline void h2_memory::stack::root()
{
   h2_stack::I().push(LLONG_MAX / 2, sizeof(void*), nullptr, 0, false, "root", __FILE__, __LINE__);
}
h2_inline void h2_memory::stack::push(const char* file, int line)
{
   h2_stack::I().push(LLONG_MAX / 2, sizeof(void*), nullptr, 0, false, "case", file, line);
}
h2_inline h2_fail* h2_memory::stack::pop()
{
   return h2_stack::I().pop();
}
h2_inline long long h2_memory::stack::footprint()
{
   return h2_stack::I().top()->footprint;
}

static inline void parse_block_attributes(const char* attributes, long long& n_limit, int& n_align, unsigned char s_fill[32], int& n_fill, bool& noleak)
{
   n_limit = LLONG_MAX / 2;
   n_align = sizeof(void*);
   n_fill = 0;
   noleak = false;

   const char* p_noleak = strcasestr(attributes, "noleak");
   if (p_noleak) {
      noleak = true;
   }

   const char* p_limit = strcasestr(attributes, "limit");
   if (p_limit) {
      const char* p = strchr(p_limit, '=');
      if (p) {
         n_limit = (long long)strtod(p + 1, nullptr);
      }
   }

   const char* p_align = strcasestr(attributes, "align");
   if (p_align) {
      const char* p = strchr(p_align, '=');
      if (p) {
         n_align = (int)strtod(p + 1, nullptr);
      }
   }

   const char* p_fill = strcasestr(attributes, "fill");
   if (p_fill) {
      const char* p = strchr(p_fill, '=');
      if (p) {
         for (p += 1; *p && ::isspace(*p);) p++;  // strip left space

         if (p[0] == '0' && ::tolower(p[1]) == 'x') {
            n_fill = h2_numeric::hex_to_bytes(p + 2, s_fill);
         } else {
            long long v = strtoll(p, (char**)NULL, 10);
            if (v <= 0xFFU)
               n_fill = 1, *((unsigned char*)s_fill) = (unsigned char)v;
            else if (v <= 0xFFFFU)
               n_fill = 2, *((unsigned short*)s_fill) = (unsigned short)v;
            else if (v <= 0xFFFFFFFFU)
               n_fill = 4, *((unsigned int*)s_fill) = (unsigned int)v;
            else
               n_fill = 8, *((unsigned long long*)s_fill) = (unsigned long long)v;
         }
      }
   }
}

h2_inline h2_memory::stack::block::block(const char* attributes, const char* file, int line)
{
   long long n_limit;
   int n_align;
   unsigned char s_fill[32];
   int n_fill;
   bool noleak;

   parse_block_attributes(attributes, n_limit, n_align, s_fill, n_fill, noleak);

   h2_stack::I().push(n_limit, n_align, s_fill, n_fill, noleak, "block", file, line);
}

h2_inline h2_memory::stack::block::~block()
{
   h2_fail_g(h2_stack::I().pop(), false);
}
// source/memory/h2_exempt.cpp

struct h2_exempt_stub {  // allocate memory inside asymmetrically
   static char* asctime(const struct tm* timeptr)
   {
      static char st[256];
      asctime_r(timeptr, st);
      return st;
   }

   static char* asctime_r(const struct tm* timeptr, char* buf)
   {
      h2_memory::restores();
#if defined _WIN32
      asctime_s(buf, 256, timeptr);
#else
      for (h2::h2_stub_temporary_restore t((void*)::asctime_r); t;) buf = ::asctime_r(timeptr, buf);
#endif
      h2_memory::overrides();
      return buf;
   }

   static char* ctime(const time_t* clock)
   {
      static char st[256];
      ctime_r(clock, st);
      return st;
   }

   static char* ctime_r(const time_t* clock, char* buf)
   {
      struct tm t;
      return asctime_r(gmtime_r(clock, &t), buf);
   }

   static struct tm* localtime(const time_t* clock)
   {
      return gmtime(clock);
   }

   static struct tm* localtime_r(const time_t* timep, struct tm* result)
   {
      return gmtime_r(timep, result);
   }

   static struct tm* gmtime_r(const time_t* clock, struct tm* result)
   {
      h2_memory::restores();
#if defined _WIN32
      ::gmtime_s(result, clock);
#else
      for (h2::h2_stub_temporary_restore t((void*)::gmtime_r); t;) result = ::gmtime_r(clock, result);
#endif
      h2_memory::overrides();
      return result;
   }

   static struct tm* gmtime(const time_t* clock)
   {
      static struct tm st;
      gmtime_r(clock, &st);
      return &st;
   }

   static time_t mktime(struct tm* timeptr)
   {
      time_t ret = 0;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::mktime); t;) ret = ::mktime(timeptr);
      h2_memory::overrides();
      return ret;
   }

   static double strtod(const char* nptr, char** endptr)
   {
      double ret = 0;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::strtod); t;) ret = ::strtod(nptr, endptr);
      h2_memory::overrides();
      return ret;
   }

   static long double strtold(const char* nptr, char** endptr)
   {
      double ret = 0;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::strtold); t;) ret = ::strtold(nptr, endptr);
      h2_memory::overrides();
      return ret;
   }
};

struct h2_exemption : h2_libc {
   h2_list x;
   void* base;
   int size;

   h2_exemption(void* _base, int _size = 0) : base(_base), size(_size)
   {
      if (!size) size = function_size((unsigned char*)base);
   }

   int function_size(unsigned char* func)
   {
#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
      // e8/e9/ff15/ff25   call/jmp PLT
      if (*func == 0xE8 || *func == 0xE9) return 1;
      if (*func == 0xFF && (*(func + 1) == 0x15 || *(func + 1) == 0x25)) return 2;
      for (unsigned char* p = func + 1;; p++) {
         // 5d c3   pop %ebp; ret;
         // 5b c3   pop %ebx; ret;
         // c9 c3   leave; ret;
         if (*p == 0xC3 && ((*(p - 1) == 0x5D) || (*(p - 1) == 0x5B) || (*(p - 1) == 0xC9))) {
            return p - func;
         }
      }
#endif
      return 300;
   }
};

h2_inline void h2_exempt::setup()
{
   static h2_stubs stubs;

   stubs.add((void*)::gmtime, (void*)h2_exempt_stub::gmtime, "gmtime", __FILE__, __LINE__);
   stubs.add((void*)::ctime, (void*)h2_exempt_stub::ctime, "ctime", __FILE__, __LINE__);
   stubs.add((void*)::asctime, (void*)h2_exempt_stub::asctime, "asctime", __FILE__, __LINE__);
   stubs.add((void*)::localtime, (void*)h2_exempt_stub::localtime, "localtime", __FILE__, __LINE__);
#if !defined _WIN32
   stubs.add((void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, "gmtime_r", __FILE__, __LINE__);
   stubs.add((void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, "ctime_r", __FILE__, __LINE__);
   stubs.add((void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, "asctime_r", __FILE__, __LINE__);
   stubs.add((void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, "localtime_r", __FILE__, __LINE__);
#endif
   if (O.os == Linux) stubs.add((void*)::mktime, (void*)h2_exempt_stub::mktime, "mktime", __FILE__, __LINE__);
   if (O.os == macOS) stubs.add((void*)::strtod, (void*)h2_exempt_stub::strtod, "strtod", __FILE__, __LINE__);
   if (O.os == macOS) stubs.add((void*)::strtold, (void*)h2_exempt_stub::strtold, "strtold", __FILE__, __LINE__);

   add((void*)::sscanf);
   add((void*)::sprintf);
   add((void*)::vsnprintf);
#if defined _WIN32
   h2_scope s[16];
   if (h2_nm::get("h2::h2_defer_failure::~h2_defer_failure", s, 1))
      add((void*)s[0].addr, s[0].size);

#endif
#ifdef __APPLE__
   add((void*)::vsnprintf_l);
   add((void*)abi::__cxa_throw);
#endif
   add((void*)h2_pattern::regex_match);  // linux is 0xcb size, MAC is 0x100 (gap to next symbol)
}

h2_inline void h2_exempt::add(void* func, unsigned long size)
{
   I().exempts.push_back((new h2_exemption((void*)func, size))->x);
}

h2_inline bool h2_exempt::in(const h2_backtrace& bt)
{
   h2_list_for_each_entry (p, I().exempts, h2_exemption, x)
      if (bt.has(p->base, p->size)) return true;
   return false;
}

// source/exception/h2_exception.cpp

struct h2_exception_stub {
   static void __cxa_throw(void* thrown_exception, std::type_info* ti, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      h2_fail_g(h2_fail::new_normal({"A exception was thrown : ", ti->name()}), false);
   }
};

h2_inline void h2_exception::initialize()
{
#if !defined _WIN32
   static h2_stubs stubs;
   if (O.exception_fails) stubs.add((void*)abi::__cxa_throw, (void*)h2_exception_stub::__cxa_throw, "__cxa_throw", __FILE__, __LINE__);
#endif
}

// source/stub/h2_e9.cpp
// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_e9 {
   static constexpr int size = sizeof(void*) == 8 ? 2 + 8 + 2 : 5;

   static bool save(void* origin_fp, unsigned char* saved)
   {
#if defined _WIN32
      DWORD t;
      if (!VirtualProtect(origin_fp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t))  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
         return false;
#else
      /* uintptr_t is favourite, but is optional type in c++ std, using unsigned long long for portable */
      unsigned long long page_size = (unsigned long long)h2_page_size();
      unsigned long long origin_start = reinterpret_cast<unsigned long long>(origin_fp);
      unsigned long long page_start = origin_start & ~(page_size - 1);
      int page_count = ::ceil((origin_start + size - page_start) / (double)page_size);

      if (mprotect(reinterpret_cast<void*>(page_start), page_count * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
         return false;
#endif
      if (saved) memcpy(saved, origin_fp, size);
      return true;
   }

   static void set(void* origin_fp, void* substitute_fp)
   {
      unsigned char* I = reinterpret_cast<unsigned char*>(origin_fp);
      long long delta = (unsigned char*)substitute_fp - (unsigned char*)origin_fp - 5;

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
      if (delta < INT_MIN || INT_MAX < delta) {  //x86_64 asm("movq $substitute_fp, %rax; jmpq %rax")
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &substitute_fp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {  //i386 asm("jmp offset")
         unsigned char C[] = {0xE9, 0, 0, 0, 0};
         *(long*)(&C[1]) = delta;
         memcpy(I, C, sizeof(C));
      }
#endif
   }

   static void reset(void* origin_fp, unsigned char* saved)
   {
      memcpy(origin_fp, saved, size);
   }
};
// source/stub/h2_native.cpp

struct h2_native : h2_libc {
   h2_list x;
   unsigned char saved[32];
   void* origin_fp;
   int reference_count = 0;
   h2_native(void* _origin_fp) : origin_fp(_origin_fp) { h2_e9::save(origin_fp, saved); }
   ~h2_native() { restore(); }
   void restore() { h2_e9::reset(origin_fp, saved); }
};

struct h2_natives {
   h2_singleton(h2_natives);
   h2_list natives;

   h2_native* get(void* origin_fp)
   {
      h2_list_for_each_entry (p, natives, h2_native, x)
         if (p->origin_fp == origin_fp)
            return p;
      return nullptr;
   }

   void operator+=(void* origin_fp)
   {
      h2_native* native = get(origin_fp);
      if (!native) {
         native = new h2_native(origin_fp);
         natives.push(native->x);
      }
      native->reference_count++;
   }

   void operator-=(void* origin_fp)
   {
      h2_native* native = get(origin_fp);
      if (native) {
         if (--native->reference_count == 0) {
            native->x.out();
            delete native;
         }
      }
   }
};
// source/stub/h2_stub.cpp

struct h2_stub : h2_libc {
   h2_list x;
   unsigned char saved[32];
   void *origin_fp, *substitute_fp;

   h2_stub(void* _origin_fp) : origin_fp(_origin_fp)
   {
      h2_natives::I() += origin_fp;
      h2_e9::save(origin_fp, saved);
   }
   ~h2_stub()
   {
      h2_e9::reset(origin_fp, saved);
      h2_natives::I() -= origin_fp;
   }
   void stub(void* _substitute_fp)
   {
      substitute_fp = _substitute_fp;
      h2_e9::set(origin_fp, substitute_fp);
   }
};

h2_inline bool h2_stubs::add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line)
{
   h2_stub* stub = nullptr;
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      if (p->origin_fp == origin_fp) {
         stub = p;
         break;
      }
   }
   if (!stub) {
      if (!h2_e9::save(origin_fp, nullptr)) {
         h2_color::prints("yellow", "STUB %s by %s() failed %s:%d\n", origin_fn, O.os == windows ? "VirtualProtect" : "mprotect", file, line);
         if (O.os == macOS) ::printf("try: "), h2_color::prints("green", "printf '\\x07' | dd of=%s bs=1 seek=160 count=1 conv=notrunc\n", O.path);
         if (O.os == Linux) ::printf("try: "), h2_color::prints("green", "objcopy --writable-text %s\n", O.path);
         return false;
      }
      stub = new h2_stub(origin_fp);
      stubs.push(stub->x);
   }
   stub->stub(substitute_fp);
   return true;
}

h2_inline void h2_stubs::clear(void* origin_fp)
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      if (p->origin_fp == origin_fp) {
         p->x.out();
         delete p;
      }
   }
}

h2_inline void h2_stubs::clear()
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      p->x.out();
      delete p;
   }
}

h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* _origin_fp) : origin_fp(_origin_fp)
{
   h2_native* native = h2_natives::I().get(origin_fp);
   if (native) {
      h2_e9::save(origin_fp, saved);
      native->restore();
   }
}

h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   h2_native* native = h2_natives::I().get(origin_fp);
   if (native) {
      h2_e9::reset(origin_fp, saved);
   }
}

// source/mock/h2_checkin.cpp

h2_inline h2_fail* h2_checkin::check(const char* func, int index, int total, const char* file, int line)
{
   if (is_satisfied() || is_saturated()) return nullptr;
   h2_row t = func + gray("()") + " expected " + delta(expect(), "green") + " but actually " + delta(actual(), "red,bold") + " called";
   if (1 < total) t += gray(" when ") + h2_numeric::sequence_number(index) + " checkin " + color(expr, "cyan");
   return h2_fail::new_normal(t, file, line);
}

h2_inline const char* h2_checkin::actual()
{
   static char st[64];
   if (call > 0)
      sprintf(st, "%d times", call);
   else
      sprintf(st, "never");
   return st;
}

h2_inline const char* h2_checkin::expect()
{
   static char st[128];
   if (least == 0) {
      if (most == 0)
         sprintf(st, "never called");
      else if (most == INT_MAX)
         sprintf(st, "any number of times");
      else
         sprintf(st, "at most %d times", most);
   } else if (least == most) {
      sprintf(st, "exactly %d times", least);
   } else if (most == INT_MAX) {
      sprintf(st, "at least %d times", least);
   } else {  // 0 < least < most < INT_MAX
      sprintf(st, "between %d and %d times", least, most);
   }
   return st;
}
// source/mock/h2_mock.cpp

h2_inline h2_row h2_mockee::arguments(int seq)
{
   h2_row row;
   row += gray("(");
   for (int i = 0; i < argument_type.size(); ++i) {
      if (i) row += gray(", ");
      if (seq == i)
         row += color(argument_type[i], "red,bold");
      else
         row.push_back(argument_type[i]);
   }
   row += gray(")");
   return row;
}

h2_inline h2_row h2_mockee::signature()
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(class_function, "green") + arguments() + gray(", ") + color(inspects, "cyan") + gray(">") + " fails";
}

h2_inline void h2_mockee::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(origin_fp, substitute_fp, class_function, file, line);
}

h2_inline h2_fail* h2_mockee::times_check()
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(class_function, i, checkin_array.size(), nullptr, 0);
      if (fail) fail->seqno = i;
      h2_fail::append_subling(fails, fail);
      h2_assert_g();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), file, line);
   h2_fail::append_child(fail, fails);
   return fail;
}
// source/mock/h2_mocks.cpp

h2_inline bool h2_mocks::add(void* mock)
{
   h2_mockee* mockee = (h2_mockee*)mock;
   h2_list_for_each_entry (p, mocks, h2_mockee, x)
      if (p == mockee) return false;
   mocks.push(mockee->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(bool check)
{
   h2_fail* fails = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mockee, x) {
      if (check) h2_fail::append_subling(fails, p->times_check());
      p->reset();
      p->x.out();
   }
   return fails;
}

// source/extension/h2_dns.cpp

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

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res)
   {
      h2_name* name = I().find(hostname);
      if (!name) return -1;

      static struct addrinfo addrinfos[32];
      static struct sockaddr_in sockaddrs[32];
      memset(addrinfos, 0, sizeof(addrinfos));
      memset(sockaddrs, 0, sizeof(sockaddrs));

      struct addrinfo** pp = res;
      for (int i = 0; i < name->resolves.size(); ++i) {
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

   static void freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* gethostbyname(char* hostname)
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

      for (int i = 0, a = 0, c = 0; i < name->resolves.size(); ++i) {
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(name->resolves[i].c_str(), b))
            h_addr_list[a++] = (char*)&b->sin_addr;
         else
            h_aliases[c++] = (char*)name->resolves[i].c_str();
      }
      return &h;
   }

   h2_stubs stubs;
   h2_resolver()
   {
      stubs.add((void*)::getaddrinfo, (void*)getaddrinfo, "getaddrinfo", __FILE__, __LINE__);
      stubs.add((void*)::freeaddrinfo, (void*)freeaddrinfo, "freeaddrinfo", __FILE__, __LINE__);
      stubs.add((void*)::gethostbyname, (void*)gethostbyname, "gethostbyname", __FILE__, __LINE__);
   }
   ~h2_resolver() { stubs.clear(); }
};

h2_inline void h2_dnses::add(h2_list& name)
{
   dnses.push(name);
}

h2_inline void h2_dnses::clear()
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
   if (h2_task::I().current_case) h2_task::I().current_case->dnses.add(name->x);
}

h2_inline void h2_dns::initialize()
{
   setaddrinfo(1, "127.0.0.1");
}
// source/extension/h2_socket.cpp

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

   static int accept(int socket, struct sockaddr* address, socklen_t* address_len)
   {
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      iport_parse(tcp->from.c_str(), (struct sockaddr_in*)address);
      *address_len = sizeof(struct sockaddr_in);

      int fd = ::socket(AF_INET, SOCK_STREAM, 0);
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

   static int connect(int socket, const struct sockaddr* address, socklen_t address_len)
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

   static ssize_t send(int socket, const void* buffer, size_t length, int flags)
   {
      I().put_outgoing(socket, (const char*)buffer, length);
      return length;
   }
   static ssize_t recv(int socket, void* buffer, size_t length, int flags)
   {
      ssize_t ret = 0;
      h2_packet* tcp = read_incoming(socket);
      if (tcp) {
         ret = tcp->data.copy((char*)buffer, tcp->data.size(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len)
   {
      I().put_outgoing(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return length;
   }
   static ssize_t recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len)
   {
      ssize_t ret = 0;
      h2_packet* udp = read_incoming(socket);

      if (udp) {
         ret = udp->data.copy((char*)buffer, udp->data.size(), 0);
         iport_parse(udp->from.c_str(), (struct sockaddr_in*)address);
         *address_len = sizeof(struct sockaddr_in);
         delete udp;
      }
      return ret;
   }
#if !defined _WIN32
   static ssize_t sendmsg(int socket, const struct msghdr* message, int flags)
   {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
   static ssize_t recvmsg(int socket, struct msghdr* message, int flags)
   {
      return recvfrom(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, &message->msg_namelen);
   }
#endif

   h2_stubs stubs;

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

      stubs.add((void*)::sendto, (void*)sendto, "sendto", __FILE__, __LINE__);
      stubs.add((void*)::recvfrom, (void*)recvfrom, "recvfrom", __FILE__, __LINE__);
#if !defined _WIN32
      stubs.add((void*)::sendmsg, (void*)sendmsg, "sendmsg", __FILE__, __LINE__);
      stubs.add((void*)::recvmsg, (void*)recvmsg, "recvmsg", __FILE__, __LINE__);
#endif
      stubs.add((void*)::send, (void*)send, "send", __FILE__, __LINE__);
      stubs.add((void*)::recv, (void*)recv, "recv", __FILE__, __LINE__);
      stubs.add((void*)::accept, (void*)accept, "accept", __FILE__, __LINE__);
      stubs.add((void*)::connect, (void*)connect, "connect", __FILE__, __LINE__);
   }
   void stop()
   {
      stubs.clear();
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

static inline void extract_iport_after_equal(const char* s, char* o)
{
   const char* p = strchr(s, '=');
   if (!p) return;
   for (p += 1; *p; p++) {
      if (::isdigit(*p) || *p == '.' || *p == ':' || *p == '*' || *p == '?') {
         *o++ = *p;
      } else {
         if (!(::isspace(*p) || *p == '\"')) return;
      }
   }
}

static inline void parse_sock_attributes(const char* attributes, char from[256], char to[256])
{
   memset(from, 0, 256);
   memset(to, 0, 256);
   const char* p_from = strcasestr(attributes, "from");
   if (p_from) {
      extract_iport_after_equal(p_from + strlen("from"), from);
   }
   const char* p_to = strcasestr(attributes, "to");
   if (p_to) {
      extract_iport_after_equal(p_to + strlen("to"), to);
   } else {
      strcpy(to, "*");
   }
}

h2_inline void h2_sock::inject(const void* packet, size_t size, const char* attributes)
{
   char from[256], to[256];
   parse_sock_attributes(attributes, from, to);
   h2_socket::I().put_incoming(from, to, (const char*)packet, size);
}

h2_inline h2_packet* h2_sock::fetch()
{
   return h2_list_pop_entry(h2_socket::I().outgoing, h2_packet, x);
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
// source/extension/h2_stdio.cpp

struct h2_stdio {
   h2_singleton(h2_stdio);
   h2_stubs stubs;
   h2_string* buffer;
   bool stdout_capturable = false, stderr_capturable = false, syslog_capturable = false;
   size_t capture_length = 0;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      h2_libc::write(fd, buf, count);
      if (fd == fileno(stdout) || fd == fileno(stderr))
         I().capture_length += count;
      if ((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr)))
         I().buffer->append((char*)buf, count);
      return count;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap)
   {
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      return write(fileno(stream), alloca_str, strlen(alloca_str));
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

   h2_stdio()
   {
#if !defined _WIN32
      stubs.add((void*)::write, (void*)write, "write", __FILE__, __LINE__);
      stubs.add((void*)::syslog, (void*)syslog, "syslog", __FILE__, __LINE__);
      stubs.add((void*)::vsyslog, (void*)vsyslog, "vsyslog", __FILE__, __LINE__);
#endif

#if defined __GNUC__ && __GNUC__ > 5
#else  // MACOS && WIN32 && GCC<=5
      stubs.add((void*)::printf, (void*)printf, "printf", __FILE__, __LINE__);
      stubs.add((void*)::vprintf, (void*)vprintf, "vprintf", __FILE__, __LINE__);
      stubs.add((void*)::putchar, (void*)putchar, "putchar", __FILE__, __LINE__);
      stubs.add((void*)::puts, (void*)puts, "puts", __FILE__, __LINE__);
      stubs.add((void*)::fprintf, (void*)fprintf, "fprintf", __FILE__, __LINE__);
      stubs.add((void*)::vfprintf, (void*)vfprintf, "vfprintf", __FILE__, __LINE__);
      stubs.add((void*)::fputc, (void*)fputc, "fputc", __FILE__, __LINE__);
      stubs.add((void*)::putc, (void*)fputc, "fputc", __FILE__, __LINE__);
      stubs.add((void*)::fputs, (void*)fputs, "fputs", __FILE__, __LINE__);
      stubs.add((void*)::fwrite, (void*)fwrite, "fwrite", __FILE__, __LINE__);
#   if defined __GNUC__ && __GNUC__ <= 5
      struct streambuf : public std::streambuf {
         FILE* f;
         int sync() override { return 0; }
         int overflow(int c) override { return h2_stdio::fputc(c, f); }
         streambuf(FILE* _f) : f(_f) { setp(nullptr, 0); }
      };
      static streambuf sb_out(stdout);
      static streambuf sb_err(stderr);
      std::cout.rdbuf(&sb_out); /* internal fwrite() called, but */
      std::cerr.rdbuf(&sb_err);
      std::clog.rdbuf(&sb_err); /* print to stderr */
#   endif
#endif


   }

   static void initialize()
   {
      ::setbuf(stdout, 0);  // unbuffered
      I().buffer = new h2_string();
   }

   void start_capture(bool _stdout, bool _stderr, bool _syslog)
   {
      stdout_capturable = _stdout;
      stderr_capturable = _stderr;
      syslog_capturable = _syslog;
      buffer->clear();
   }

   const char* stop_capture()
   {
      stdout_capturable = stderr_capturable = syslog_capturable = false;
      buffer->push_back('\0');
      return buffer->c_str();
   }
};

h2_inline h2_cout::h2_cout(h2_matcher<const char*> m_, const char* e_, const char* type_, const char* file_, int line_) : file(file_), line(line_), m(m_), e(e_), type(type_)
{
   bool all = !strlen(type);
   h2_stdio::I().start_capture(all || strcasestr(type, "out"), all || strcasestr(type, "err"), all || strcasestr(type, "syslog"));
}

h2_inline h2_cout::~h2_cout()
{
   h2_assert_g();
   h2_fail* fail = m.matches(h2_stdio::I().stop_capture(), 0);
   if (fail) {
      fail->file = file;
      fail->line = line;
      fail->assert_type = "OK2";
      fail->e_expression = e;
      fail->a_expression = "";
      fail->explain = "COUT";
      h2_fail_g(fail, false);
   }
}

h2_inline size_t h2_cout::length()
{
   return h2_stdio::I().capture_length;
}
// source/extension/h2_perf.cpp

h2_inline h2_perf::h2_perf(long long ms_, const char* file_, int line_) : file(file_), line(line_), ms(ms_)
{
   start = ::clock();
}

h2_inline h2_perf::~h2_perf()
{
   h2_assert_g();
   long long delta = (::clock() - start) * 1000 / CLOCKS_PER_SEC;
   if (ms < delta) {
      h2_row row = "performance expect < ";
      row.printf("green", "%lld", ms).printf("", " ms, but actually cost ").printf("red", "%lld", delta).printf("", " ms");
      h2_fail_g(h2_fail::new_normal(row, file, line), false);
   }
}

// source/core/h2_case.cpp

h2_inline void h2_case::clear()
{
   h2_sock::clear();
   if (fails) delete fails;
   fails = nullptr;
   asserts = 0;
}

h2_inline void h2_case::prev_setup()
{
   status = passed;
   h2_memory::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup(const h2_string& ex)
{
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear(true);
   footprint = h2_memory::stack::footprint();
   h2_fail::append_subling(fail, h2_memory::stack::pop());
   // should memory assert stats into assert count ?

   if (status == failed) {
      if (fail) delete fail;
      return;
   }
   if (!ex.empty()) {
      h2_fail::append_subling(fails, h2_fail::new_normal({"Uncaught Exception : ", ex}));
      if (fail) delete fail;
      status = failed;
      return;
   }
   if (fail) {
      h2_fail::append_subling(fails, fail);
      status = failed;
      return;
   }
}

h2_inline void h2_case::do_fail(h2_fail* fail, bool defer)
{
   status = failed;
   h2_fail::append_subling(fails, fail);
   if (!defer) ::longjmp(ctx, 1);
}
// source/core/h2_suite.cpp

h2_inline h2_suite::h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const char* file_, int line_) : name(name_), file(file_), line(line_), test_code(test_code_)
{
   memset(ctx, 0, sizeof(jmp_buf));
   h2_task::I().suites.push_back(x);
}

h2_inline void h2_suite::clear()
{
   stats.clear();
}

h2_inline void h2_suite::setup()
{
   h2_memory::stack::push(file, line);
}

h2_inline void h2_suite::cleanup()
{
   stubs.clear();
   mocks.clear(false);
   stats.footprint = h2_memory::stack::footprint();
   h2_memory::stack::pop();
}

h2_inline void h2_suite::enumerate()
{
   test_code(this, nullptr); /* enumerate case by static local h2_case variable inside of h2_suite_test_CmLn() */
}

h2_inline void h2_suite::execute(h2_case* c)
{
   h2_string ex;
   c->prev_setup();
   try {
      test_code(this, c); /* include Setup(); c->post_setup() and c->prev_cleanup(); Cleanup() */
   } catch (std::exception& e) {
      ex = e.what();
   } catch (std::string& m) {
      ex = m.c_str();
   } catch (const char* m) {
      ex = m;
   } catch (...) {
      ex = "Unknown Exception";
   }
   c->post_cleanup(ex);
}

h2_inline h2_suite::registor::registor(h2_suite* s, h2_case* c)
{
   static int s_auto_increment = INT_MAX / 4;
   s->cases.push_back(c->x);
   s->seq = c->seq = ++s_auto_increment;
}

h2_inline h2_suite::cleaner::~cleaner()
{
   static const jmp_buf zero = {0};
   if (memcmp((const void*)thus->ctx, (const void*)zero, sizeof(jmp_buf)))
      ::longjmp(thus->ctx, 1);
}
// source/core/h2_task.cpp

static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, "%s\n%s\n%d\n", ss(s->name), c->name, c->status);
   ::fclose(f);
}

static inline void __mark(h2_list& suites, char* suitename, char* casename, int status)
{
   static int seq = INT_MIN / 4;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, ss(s->name)))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name))
               s->seq = c->seq = ++seq, c->last_status = status;
}

static inline int mark_last_order(h2_list& suites)
{
   int count = 0;
   char suitename[1024], casename[1024], status[32];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (::fgets(suitename, sizeof(suitename), f) && ::fgets(casename, sizeof(casename), f) && ::fgets(status, sizeof(status), f)) {
      suitename[strlen(suitename) - 1] = '\0';  /* remove \n in save_last_order */
      casename[strlen(casename) - 1] = '\0';
      status[strlen(status) - 1] = '\0';
      __mark(suites, suitename, casename, atoi(status));
      count++;
   }
   ::fclose(f);
   return count;
}

static inline void drop_last_order()
{
   ::remove(".last_order");
}

static int __suite_cmp(h2_list* a, h2_list* b)
{
   return h2_list_entry(a, h2_suite, x)->seq - h2_list_entry(b, h2_suite, x)->seq;
}

static int __case_cmp(h2_list* a, h2_list* b)
{
   return h2_list_entry(a, h2_case, x)->seq - h2_list_entry(b, h2_case, x)->seq;
}

h2_inline void h2_task::shuffle()
{
   last = mark_last_order(suites);
   srand(h2_now());
   if (O.shuffle_order && last == 0)
      h2_list_for_each_entry (s, suites, h2_suite, x)
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            s->seq = c->seq = rand();

   suites.sort(__suite_cmp);
   h2_list_for_each_entry (s, suites, h2_suite, x)
      s->cases.sort(__case_cmp);
}

h2_inline void h2_task::shadow()
{
   if (stats.failed == 0)
      drop_last_order();
   else if (last == 0)
      save_last_order(suites);
}

h2_inline void h2_task::enumerate()
{
   h2_list_for_each_entry (s, suites, h2_suite, x) {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& cleanup : global_suite_cleanups) cleanup();
      int unfiltered = 0;
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         if (!(c->filtered = O.filter(ss(s->name), c->name, c->file, c->line)))
            unfiltered++;
      if (unfiltered == 0) s->filtered = O.filter(ss(s->name), "", s->file, s->line);
   }
}

h2_inline void h2_task::execute()
{
   h2_report::initialize();
   h2_memory::initialize();
   h2_exception::initialize();
   h2_stdio::initialize();
   h2_dns::initialize();

   for (auto& setup : global_setups) setup();
   enumerate();

   h2_report::I().on_task_start(this);
   for (rounds = 0; rounds < O.rounds && !stats.failed; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if (0 < O.break_after_fails && O.break_after_fails <= stats.failed) break;
            current_case = c;
            h2_report::I().on_case_start(s, c);
            if (!O.list_cases) {
               if (c->todo) {
                  stats.todo++, s->stats.todo++;
               } else if (c->filtered) {
                  stats.filtered++, s->stats.filtered++;
               } else {
                  if (O.only_execute_fails && c->last_status != h2_case::failed)
                     c->status = h2_case::ignored;
                  if (c->status != h2_case::ignored) {
                     for (auto& setup : global_case_setups) setup();
                     s->execute(c);
                     for (auto& cleanup : global_case_cleanups) cleanup();
                  }
                  if (c->status == h2_case::passed) stats.passed++, s->stats.passed++;
                  if (c->status == h2_case::failed) stats.failed++, s->stats.failed++;
                  if (c->status == h2_case::ignored) stats.ignored++, s->stats.ignored++;
               }
            }
            h2_report::I().on_case_endup(s, c);
            c->clear();
         }
         s->cleanup();
         for (auto& cleanup : global_suite_cleanups) cleanup();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      shadow();
   }
   h2_report::I().on_task_endup(this);
   for (auto& cleanup : global_cleanups) cleanup();

   stubs.clear();
   mocks.clear(false);
   h2_memory::finalize();
}

// source/assert/h2_assert.cpp

static inline const char* find_outer_comma(const char* expression)
{
   char stack[1024] = {'\0'};
   int top = 1;
   int len = strlen(expression);
   for (int i = 0; i < len; ++i) {
      switch (expression[i]) {
      case '\\':
         if (expression[i + 1]) ++i;
         break;
      case '\"':
         if (stack[top - 1] == '\"')
            top--;
         else
            stack[top++] = '\"';
         break;
      case '\'':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] == '\'')
               top--;
            else
               stack[top++] = '\'';
         }
         break;
      case '(':
         if (stack[top - 1] != '\"') {
            stack[top++] = '(';
         }
         break;
      case ')':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] != '(') return nullptr;
            top--;
         }
         break;
      case '<':
         if (stack[top - 1] != '\"') {
            stack[top++] = '<';
         }
         break;
      case '>':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] != '<') return nullptr;
            top--;
         }
         break;
      case '{':
         if (stack[top - 1] != '\"') {
            stack[top++] = '{';
         }
         break;
      case '}':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] != '{') return nullptr;
            top--;
         }
         break;
      case ',':
         if (top == 1) return expression + i;
         break;
      default: break;
      }
   }
   return nullptr;
}

static inline void split_expression(h2_string& e_expression, h2_string& a_expression, const char* expression)
{
   const char *p = nullptr, *q = nullptr, *comma = nullptr;

   comma = find_outer_comma(expression);
   if (comma) {
      for (p = comma - 1; expression <= p && ::isspace(*p);) p--;
      e_expression.assign(expression, (p + 1) - expression);

      for (q = comma + 1; ::isspace(*q);) q++;
      a_expression.assign(q, (expression + strlen(expression)) - q);
   }
}

h2_inline h2_defer_failure::~h2_defer_failure()
{
   if (fails) {
      fails->file = file;
      fails->line = line;
      fails->assert_type = assert_type;
      if (!strcmp("OK1", assert_type)) {
         fails->e_expression = e_expression;
         fails->a_expression = expression;
      } else if (!strcmp("OK2", assert_type)) {
         const char* comma = find_outer_comma(expression);
         if (comma) {
            const char *p, *q;
            for (p = comma - 1; expression <= p && ::isspace(*p);) p--;
            fails->e_expression.assign(expression, (p + 1) - expression);
            for (q = comma + 1; ::isspace(*q);) q++;
            fails->a_expression.assign(q, (expression + strlen(expression)) - q);
         } else {
            fails->e_expression = e_expression;
            fails->a_expression = a_expression;
         }
      } else {
         fails->e_expression = e_expression;
         fails->a_expression = a_expression;
      }
      fails->user_explain = oss.str().c_str();
      h2_fail_g(fails, false);
   }
}

// source/other/h2_debug.cpp

#if defined __linux__
#   if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#      define h2_raise_trap() asm volatile("int $3")
#   else
#      define h2_raise_trap() raise(SIGTRAP)
#   endif
#elif defined __APPLE__
/* clang-format off */
#   define h2_raise_trap() __asm__("int $3\n" : :)
/* clang-format on */
#endif

#if defined __linux__
static inline bool under_debug(int, const char*)
{
   char t[1024];
   FILE* f = ::fopen("/proc/self/status", "r");
   if (!f) return false;
   bool ret = false;
   while (::fgets(t, sizeof(t) - 1, f)) {
      if (strncmp(t, "TracerPid:\t", 11) == 0) {
         ret = t[11] != '\0' && t[11] != '0';
         break;
      }
   }
   ::fclose(f);
   return ret;
}
#elif defined __APPLE__
static inline bool under_debug(int pid, const char* path)
{
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", pid);
   FILE* f = ::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r");
   if (!f) return false;
   bool ret = false;
   while (::fgets(t, sizeof(t) - 1, f)) {
      if (strstr(t, h2_basename(path)) || strstr(t, attach_pid)) {
         ret = true;
         break;
      }
   }
   ::pclose(f);
   return false;
}
#endif

static inline char* get_gdb1(char* s)
{
#if defined __linux__
   sprintf(s, "gdb --quiet --args %s %s", O.path, O.args);
#elif defined __APPLE__
   sprintf(s, "lldb %s -- %s", O.path, O.args);
#endif
   return s;
}

static inline char* get_gdb2(char* s, int pid)
{
#if defined __linux__
   sprintf(s, "sudo gdb --pid=%d", pid);
#elif defined __APPLE__
   sprintf(s, "sudo lldb --attach-pid %d", pid);
#endif
   return s;
}

h2_inline void h2_debugger::trap()
{
#if !defined _WIN32
   int pid = (int)getpid();
   if (!under_debug(pid, O.path)) {
      static h2_once only_one_time;
      if (only_one_time) {
         if (!strcmp("gdb attach", O.debug)) {
            if (fork() == 0) {
               system(get_gdb2((char*)alloca(256), pid));
               exit(0);
            } else {
               while (!under_debug(pid, O.path))
                  h2_sleep(100);  // wait for password
            }
         } else {
            system(get_gdb1((char*)alloca(256)));
            exit(0);
         }
      }
   }

   h2_raise_trap();
#endif
}
// source/other/h2_failure.cpp

h2_inline void h2_fail::append_subling(h2_fail*& fail, h2_fail* n)
{
   if (!fail) {
      fail = n;
   } else {
      h2_fail** pp = &fail->subling_next;
      while (*pp) pp = &(*pp)->subling_next;
      *pp = n;
   }
}

h2_inline void h2_fail::append_child(h2_fail*& fail, h2_fail* n)
{
   if (!fail) {
      fail = n;
   } else {
      h2_fail** pp = &fail->child_next;
      while (*pp) pp = &(*pp)->child_next;
      *pp = n;
   }
}

h2_inline h2_fail::~h2_fail()
{
   if (child_next) delete child_next;
   if (subling_next) delete subling_next;
}

h2_inline h2_row h2_fail::locate()
{
   if (file && strlen(file) && line)
      return gray(" at ") + h2_string("%s:%d", h2_basename(file), line);
   return {};
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index, int child_index)
{
   cb(this, subling_index, child_index);
   if (child_next) child_next->foreach(cb, 0, child_index + 1);
   if (subling_next) subling_next->foreach(cb, subling_index + 1, child_index);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_row& explain_ = {}, const char* file_ = nullptr, int line_ = 0) : h2_fail(explain_, file_, line_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row;
      row.indent(child_index * 2 + 1);
      if (0 <= seqno) row.printf("dark gray", "%d. ", seqno);
      row += explain;
      h2_color::printl(row + locate());
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   static const char* s_null[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0", nullptr};
   static const char* s_true[] = {"IsTrue", "true", "1", nullptr};
   static const char* s_false[] = {"IsFalse", "false", "0", nullptr};
   static const char** S[] = {s_null, s_true, s_false};

   if (a == b) return true;
   for (int i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
      if (h2_in(a.c_str(), S[i]) && h2_in(b.c_str(), S[i]))
         return true;
   return false;
}

struct h2_fail_unexpect : h2_fail {
   h2_row expection, represent;
   int c = 0;
   h2_fail_unexpect(const h2_row& expection_ = {}, const h2_row& represent_ = {}, const h2_row& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(explain_, file_, line_), expection(expection_), represent(represent_) {}

   void print_OK1(h2_row& row)
   {
      h2_row a = h2_row(a_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("cyan");
      row += "OK" + gray("(") + a + gray(")") + " is " + color("false", "bold,red");
   }
   void print_OK2(h2_row& row)
   {
      h2_row e, a;
      if (!expection.width()) {
         e = h2_row(e_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("green");
      } else if (is_synonym(e_expression, expection.string())) {
         e = expection.acronym(O.verbose ? 10000 : 30, 3).brush("green");
      } else {
         e = h2_row(e_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("cyan") + gray("==>") + expection.acronym(O.verbose ? 10000 : 30, 3).brush("green");
      }

      if (!represent.width()) {
         a = h2_row(a_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("bold,red");
      } else if (is_synonym(a_expression, represent.string()) || !a_expression.length()) {
         a = represent.acronym(O.verbose ? 10000 : 30, 3).brush("bold,red");
      } else {
         a = represent.acronym(O.verbose ? 10000 : 30, 3).brush("bold,red") + gray("<==") + h2_row(a_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("cyan");
      }

      row += "OK" + gray("(") + e + ", " + a + gray(")");
   }
   void print_JE(h2_row& row)
   {
      h2_row e = h2_row(e_expression.unquote('\"').unquote('\'')).acronym(O.verbose ? 10000 : 30, 2).brush("cyan");
      h2_row a = h2_row(a_expression.unquote('\"').unquote('\'')).acronym(O.verbose ? 10000 : 30, 2).brush("bold,red");
      row += "JE" + gray("(") + e + ", " + a + gray(")");
   }
   void print_Inner(h2_row& row)
   {
      if (0 <= seqno) row.printf("dark gray", "%d. ", seqno);
      if (expection.width()) {
         row.printf("", "%sexpect is ", comma_if(c++));
         row += expection.acronym(O.verbose ? 10000 : 30, 3).brush("green");
      }
      if (represent.width()) {
         row.printf("", "%sactual is ", comma_if(c++));
         row += represent.acronym(O.verbose ? 10000 : 30, 3).brush("bold,red");
      }
   }

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row;
      row.indent(child_index * 2 + 1);
      if (!strcmp("Inner", assert_type)) print_Inner(row);
      if (!strcmp("OK1", assert_type)) print_OK1(row);
      if (!strcmp("OK2", assert_type)) print_OK2(row);
      if (!strcmp("JE", assert_type)) print_JE(row);
      if (explain.width()) row += comma_if(c++, ", ", " ") + explain;
      if (user_explain.size()) row += {comma_if(c++, ", ", " "), user_explain};
      h2_color::printl(row + locate());
   }
};

static inline void fmt_char(char c, bool eq, const char* style, h2_row& row)
{
   char t_style[32] = "";
   if (!eq) strcpy(t_style, style);
   if (c == '\n') c = 'n', strcat(t_style, ",inverse");
   if (c == '\r') c = 'r', strcat(t_style, ",inverse");
   if (c == '\t') c = 't', strcat(t_style, ",inverse");
   if (c == '\0') c = ' ', eq || strcat(t_style, ",inverse");
   row.printf(t_style, "%c", c);
}

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_row& expection, const h2_row& explain = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(expection, h2_representify(a_value_), explain, file_, line_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
         h2_row e_row, a_row;
         for (size_t i = 0; i < e_value.size(); ++i) {
            char ac = i < a_value.size() ? a_value[i] : ' ';
            bool eq = caseless ? ::tolower(e_value[i]) == ::tolower(ac) : e_value[i] == ac;
            fmt_char(e_value[i], eq, "green", e_row);
         }
         for (size_t i = 0; i < a_value.size(); ++i) {
            char ec = i < e_value.size() ? e_value[i] : ' ';
            bool eq = caseless ? ::tolower(a_value[i]) == ::tolower(ec) : a_value[i] == ec;
            fmt_char(a_value[i], eq, "red", a_row);
         }

         h2_color::printl(h2_layout::unified(e_row, a_row, "expect", "actual", O.terminal_width));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_row& expection, const h2_row& explain, const char* file = nullptr, int line = 0)
     : h2_fail_unexpect(expection, h2_representify(a_value_), explain, file, line), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
         h2_row e_row, a_row;
         for (size_t i = 0; i < e_value.size(); ++i) fmt_char(e_value[i], true, "", e_row);
         for (size_t i = 0; i < a_value.size(); ++i) fmt_char(a_value[i], true, "", a_row);
         h2_color::printl(h2_layout::seperate(e_row, a_row, "expect", "actual", O.terminal_width));
      }
   }
};

struct h2_fail_json : h2_fail_unexpect {
   h2_string e_value, a_value;
   const bool caseless;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_row& expection_, bool caseless_, const h2_row& explain_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(expection_, a_value_, explain_, file_, line_), e_value(e_value_), a_value(a_value_), caseless(caseless_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_rows e_rows, a_rows;
      h2_fail_unexpect::print(subling_index, child_index);
      if (O.copy_paste_json || !h2_json::diff(e_value, a_value, e_rows, a_rows, caseless)) {
         e_rows = h2_json::format(e_value);
         a_rows = h2_json::format(a_value);
         for (size_t i = 0; i < e_rows.size(); ++i)
            if (i) e_rows[i].indent(8);
         for (size_t i = 0; i < a_rows.size(); ++i)
            if (i) a_rows[i].indent(8);
         h2_color::prints("dark gray", "expect");
         h2_color::prints("green", "> ");
         h2_color::printl(e_rows);
         h2_color::prints("dark gray", "actual");
         h2_color::prints("red", "> ");
         h2_color::printl(a_rows);
      } else {
         h2_rows rows = h2_layout::split(e_rows, a_rows, "expect", "actual", 0, 'd', O.terminal_width - 1);
         for (auto& row : rows) row.indent(1);
         h2_color::printl(rows);
      }
   }
};

struct h2_fail_memcmp : h2_fail_unexpect {
   h2_vector<unsigned char> e_value, a_value;
   const int width, nbits;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, const h2_string& represent_, const h2_row& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect({}, represent_, explain_, file_, line_), e_value(e_value_, e_value_ + (nbits_ + 7) / 8), a_value(a_value_, a_value_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}

   void print(int subling_index, int child_index) override
   {
      h2_fail_unexpect::print(subling_index, child_index);
      h2_rows e_rows, a_rows;
      int bytes_per_row = 0;
      switch (width) {
      case 1: print_bits(e_rows, a_rows, bytes_per_row = 4); break;
      case 8: print_ints<unsigned char>(e_rows, a_rows, bytes_per_row = (O.terminal_width < 108 ? 8 : 16)); break;
      case 16: print_ints<unsigned short>(e_rows, a_rows, bytes_per_row = 16); break;
      case 32: print_ints<unsigned int>(e_rows, a_rows, bytes_per_row = 16); break;
      case 64: print_ints<unsigned long long>(e_rows, a_rows, bytes_per_row = 16); break;
      default: break;
      }
      h2_color::printl(h2_layout::split(e_rows, a_rows, "expect", "actual", bytes_per_row * 8 / width, 'x', O.terminal_width));
   }

   void print_bits(h2_rows& e_rows, h2_rows& a_rows, int bytes_per_row)
   {
      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_row e_row, a_row;
         for (int j = 0; j < bytes_per_row; ++j) {
            if (j) e_row.push_back(" ");
            if (j) a_row.push_back(" ");
            for (int k = 0; k < 8; ++k) {
               if ((i * bytes_per_row + j) * 8 + k < nbits) {
                  unsigned char e_val = (e_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  unsigned char a_val = (a_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  if (e_val == a_val) {
                     e_row.push_back(h2_string(e_val ? "1" : "0"));
                     a_row.push_back(h2_string(a_val ? "1" : "0"));
                  } else {
                     e_row.printf("green", e_val ? "1" : "0");
                     a_row.printf("bold,red", a_val ? "1" : "0");
                  }
               }
            }
         }
         e_rows.push_back(e_row);
         a_rows.push_back(a_row);
      }
   }

   template <typename T>
   void print_ints(h2_rows& e_rows, h2_rows& a_rows, int bytes_per_row)
   {
      char fmt[32];
      sprintf(fmt, "%%s%%0%dX", (int)sizeof(T) * 2);

      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_row e_row, a_row;
         for (int j = 0; j < bytes_per_row; j += sizeof(T)) {
            if (i * bytes_per_row + j < e_value.size()) {
               T e_val = *(T*)(e_value.data() + (i * bytes_per_row + j));
               T a_val = *(T*)(a_value.data() + (i * bytes_per_row + j));
               char e_str[32], a_str[32];
               sprintf(e_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", e_val);
               sprintf(a_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", a_val);
               if (e_val == a_val) {
                  e_row.push_back(e_str);
                  a_row.push_back(a_str);
               } else {
                  e_row.printf("green", e_str);
                  a_row.printf("bold,red", a_str);
               }
            }
         }
         e_rows.push_back(e_row);
         a_rows.push_back(a_row);
      }
   }
};

struct h2_fail_memory : h2_fail {
   const void* ptr;
   const int size;
   const h2_backtrace bt_allocate, bt_release;

   h2_fail_memory(const void* ptr_, const int size_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail({}, file_, line_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   h2_vector<std::pair<int, int>> sizes;
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, int size_, const h2_vector<std::pair<int, int>>& sizes_, const h2_backtrace& bt_allocate_, const char* where_, const char* file_, int line_)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), sizes(sizes_), where(where_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row = h2_stringify(ptr) + color(" memory leak ", "bold,red") + h2_stringify(size).brush("red") + " ";
      int i = 0, c = 0, n = 3;
      h2_row l;
      for (auto& p : sizes) {
         l += gray(comma_if(i++));
         if (!O.verbose && n < i) {
            l += gray("..." + h2_stringify(sizes.size() - n));
            break;
         }
         l += h2_stringify(p.first);
         if (1 < p.second) l += gray("x") + h2_stringify(p.second);
         c += p.second;
      }
      if (1 < c) row += gray("[") + l + gray("] ");
      h2_color::printl(" " + row + "bytes in " + where + " totally" + locate());
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::prints("", " %p", ptr);
      h2_color::prints("bold,red", " double free");
      h2_color::prints("", " at backtrace:\n", ptr), bt_double_free.print(2);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::prints("", "  already free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::prints("", " %p allocate with ", ptr);
      h2_color::prints("bold,red", "%s", who_allocate);
      h2_color::prints("", ", release by ");
      h2_color::prints("bold,red", "%s", who_release);
      h2_color::prints("", " asymmetrically at backtrace:\n"), bt_release.print(2);
      if (0 < bt_allocate.count) h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_overflow : h2_fail_memory {
   const void* addr;                    /* 犯罪地点 */
   const char* action;                  /* 犯罪行为 */
   const h2_vector<unsigned char> spot; /* 犯罪现场 */
   const h2_backtrace bt_trample;       /* 犯罪过程 */
   h2_fail_overflow(const void* ptr_, const int size_, const void* addr_, const char* action_, const h2_vector<unsigned char>& spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), addr(addr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      int offset = ptr < addr ? (long long)addr - ((long long)ptr + size) : (long long)addr - (long long)ptr;
      h2_row row = h2_stringify(ptr) + " " + color(h2_string("%+d", offset), "bold,red") + " " + gray("(") + h2_stringify(addr) + gray(")") + " " + color(action, "bold,red") + " " + (offset >= 0 ? "overflow" : "underflow") + " ";
      for (int i = 0; i < spot.size(); ++i) row.printf("bold,red", "%02X ", spot[i]);
      h2_color::printl(" " + row + locate());
      if (bt_trample.count) h2_color::prints("", "  trampled at backtrace:\n"), bt_trample.print(3);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_use_after_free : h2_fail_memory {
   const void* addr;          /* 犯罪地点 */
   const char* action;        /* 犯罪行为 */
   const h2_backtrace bt_use; /* 犯罪过程 */
   h2_fail_use_after_free(const void* ptr_, const void* addr_, const char* action_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_use_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), addr(addr_), action(action_), bt_use(bt_use_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row = h2_stringify(ptr) + " " + color(h2_string("%+d", (long long)addr - (long long)ptr), "bold,red") + " " + gray("(") + h2_stringify(addr) + gray(")") + " " + color(action, "bold,red") + color(" after free", "bold,red");
      h2_color::printl(" " + row + " at backtrace:"), bt_use.print(2);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::prints("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_row& explain, const char* file, int line)
{
   return new h2_fail_normal(explain, file, line);
}
h2_inline h2_fail* h2_fail::new_unexpect(const h2_row& expection, const h2_row& represent, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_unexpect(expection, represent, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_row& expection, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_strcmp(e_value, a_value, caseless, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_row& expection, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_strfind(e_value, a_value, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_row& expection, bool caseless, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_json(e_value, a_value, expection, caseless, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& represent, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_memcmp(e_value, a_value, width, nbits, represent, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, int size, const h2_vector<std::pair<int, int>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* file, int line)
{
   return new h2_fail_memory_leak(ptr, size, sizes, bt_allocate, where, file, line);
}
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free)
{
   return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free);
}
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release)
{
   return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release);
}
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const int size, const void* addr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file, int line)
{
   return new h2_fail_overflow(ptr, size, addr, action, spot, bt_allocate, bt_trample, file, line);
}
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* addr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use)
{
   return new h2_fail_use_after_free(ptr, addr, action, bt_allocate, bt_release, bt_use);
}
// source/other/h2_report.cpp

struct h2_report_impl {
   h2_list x;
   int suites = 0, cases = 0;
   int suite_index = 0, suite_case_index = 0, task_case_index = 0;
   long long task_cost = 0, suite_cost = 0, case_cost = 0;

   virtual void on_task_start(h2_task* t)
   {
      suites = t->suites.count();
      h2_list_for_each_entry (s, t->suites, h2_suite, x)
         cases += s->cases.count();
      task_cost = h2_now();
   }
   virtual void on_task_endup(h2_task* t)
   {
      task_cost = h2_now() - task_cost;
   }
   virtual void on_suite_start(h2_suite* s)
   {
      suite_case_index = 0;
      suite_cost = h2_now();
   }
   virtual void on_suite_endup(h2_suite* s)
   {
      suite_cost = h2_now() - suite_cost;
   }
   virtual void on_case_start(h2_suite* s, h2_case* c)
   {
      ++suite_case_index;
      case_cost = h2_now();
   }
   virtual void on_case_endup(h2_suite* s, h2_case* c)
   {
      ++task_case_index;
      case_cost = h2_now() - case_cost;
   }
};

struct h2_report_list : h2_report_impl {
   int unfiltered_suite_index = 0, unfiltered_suite_case_index = 0, unfiltered_task_case_index = 0, todo = 0;

   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);
      h2_color::prints("bold,green", "Listing <%d suites, %d cases", unfiltered_suite_index, unfiltered_task_case_index);
      if (todo) h2_color::prints("bold,green", ", %d todo", todo);
      if (task_case_index - unfiltered_task_case_index) h2_color::prints("bold,green", ", %d filtered", task_case_index - unfiltered_task_case_index);
      h2_color::prints("bold,green", ">\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      unfiltered_suite_case_index = 0;
      if (!s->name) return;  // CASE
      if ((O.list_cases == 'l' && !s->filtered) || (O.list_cases == 'L' && s->filtered)) {
         h2_color::prints("dark gray", "SUITE-%d. ", ++unfiltered_suite_index);
         h2_color::prints("bold,blue", "%s", s->name);
         h2_color::prints("dark gray", " %s:%d\n", s->file, s->line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if ((O.list_cases == 'l' && !c->filtered) || (O.list_cases == 'L' && c->filtered)) {
         ++unfiltered_task_case_index, ++unfiltered_suite_case_index;
         if (c->todo) todo++;
         h2_color::prints("dark gray", "%s-%d. ", c->todo ? (s->name ? " Todo" : "TODO") : (s->name ? " Case" : "CASE"), unfiltered_suite_case_index);
         h2_color::prints("cyan", "%s", c->name);
         h2_color::prints("dark gray", " %s:%d\n", h2_basename(c->file), c->line);
      }
   }
};

struct h2_report_console : h2_report_impl {
   void print_perfix(bool percentage)
   {
      static size_t last = 0;
      h2_color::prints("", h2_cout::length() == last ? "\r" : "\n");
      if (percentage && O.execute_progress) {
         h2_color::prints("dark gray", "[");
         h2_color::prints("", "%3d%%", cases ? (int)(task_case_index * 100 / cases) : 100);
         h2_color::prints("dark gray", "] ");
      }
      last = h2_cout::length();
   }
   const char* format_duration(long long ms)
   {
      static char st[128];
      if (ms < 100)
         sprintf(st, "%lld milliseconds", ms);
      else if (ms < 1000 * 60)
         sprintf(st, "%.2g seconds", ms / (double)1000.0);
      else if (ms < 1000 * 60 * 60)
         sprintf(st, "%.2g minutes", ms / (double)6000.0);
      else
         sprintf(st, "%.2g hours", ms / (double)36000.0);

      return st;
   }
   const char* format_volume(long long footprint)
   {
      static char st[128];
      if (footprint < 1024)
         sprintf(st, "%lld footprint", footprint);
      else if (footprint < 1024 * 1024LL)
         sprintf(st, "%.2gKB footprint", footprint / (double)1024);
      else if (footprint < 1024 * 1024 * 1024LL)
         sprintf(st, "%.2gMB footprint", footprint / (double)(1024 * 1024LL));
      else
         sprintf(st, "%.2gGB footprint", footprint / (double)(1024 * 1024 * 1024LL));

      return st;
   }
   void comma_status(int n, const char* style, const char* name, int& c)
   {
      if (c++) h2_color::prints("dark gray", ", ");
      h2_color::prints(style, "%d", n);
      h2_color::prints("", " %s", name);
   }
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);

      print_perfix(false);
      if (O.verbose)
         h2_color::prints("", "\n");
      if (0 < t->stats.failed)
         h2_color::prints("bold,red", "Failure ");
      else
         h2_color::prints("bold,green", "Success ");

      if (0 < nonzero_count(t->stats.failed, t->stats.todo, t->stats.filtered, t->stats.ignored))
         h2_color::prints("dark gray", "(");

      int c = 0;
      comma_status(t->stats.passed, "green", "passed", c);
      if (t->stats.failed) comma_status(t->stats.failed, "red", "failed", c);
      if (t->stats.todo) comma_status(t->stats.todo, "yellow", "todo", c);
      if (t->stats.filtered) comma_status(t->stats.filtered, "blue", "filtered", c);
      if (t->stats.ignored) comma_status(t->stats.ignored, "blue", "ignored", c);
      if (0 < nonzero_count(t->stats.failed, t->stats.todo, t->stats.filtered, t->stats.ignored)) {
         h2_color::prints("dark gray", ")");
         h2_color::prints("", " %d", cases);
      }
      h2_color::prints("", " case%s", 1 < cases ? "s" : "");
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%d assert%s", t->stats.asserts, 1 < t->stats.asserts ? "s" : "");
      if (1 < t->rounds) {
         h2_color::prints("dark gray", ", ");
         h2_color::prints("", "%d rounds", t->rounds);
      }
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%s \n", format_duration(task_cost));
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
   }

   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.compact) return;
      if (O.verbose && O.includes.size() + O.excludes.size() == 0) {
         print_perfix(true);
         h2_color::prints("dark gray", "suite ");
         h2_color::prints("", "%s", ss(s->name));
         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_color::prints("dark gray", " (");
         else
            h2_color::prints("dark gray", " - ");

         int c = 0;
         if (s->stats.passed) comma_status(s->stats.passed, "", "passed", c);
         if (s->stats.failed) comma_status(s->stats.failed, "", "failed", c);
         if (s->stats.todo) comma_status(s->stats.todo, "", "todo", c);
         if (s->stats.filtered) comma_status(s->stats.filtered, "", "filtered", c);
         if (s->stats.ignored) comma_status(s->stats.ignored, "", "ignored", c);

         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_color::prints("dark gray", ")");
         if (0 < s->cases.count())
            h2_color::prints("", " case%s", 1 < s->cases.count() ? "s" : "");

         if (0 < s->stats.asserts) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%d assert%s", s->stats.asserts, 1 < s->stats.asserts ? "s" : "");
         }
         if (0 < s->stats.footprint) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%s", format_volume(s->stats.footprint));
         }
         if (1 < suite_cost) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%s", format_duration(suite_cost));
         }
         h2_color::prints("", "\n");
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
   }
   void print_title(const char* s, const char* c, const char* file, int line)
   {
      if (strlen(c))
         h2_color::prints("", "%s", c);
      else
         h2_color::prints("dark gray", "case");
      if (s) {
         h2_color::prints("dark gray", " | ");
         if (strlen(s))
            h2_color::prints("", "%s", s);
         else
            h2_color::prints("dark gray", "   ");
      }
      h2_color::prints("dark gray", " | ");
      h2_color::prints("", "%s:%d", h2_basename(file), line);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (c->filtered) return;
      if (c->todo) {
         print_perfix(true);
         h2_color::prints("yellow", s->name ? "Todo   " : "TODO   ");
         print_title(s->name, c->name, c->file, c->line);
         h2_color::prints("", "\n");
         return;
      }
      switch (c->status) {
      case h2_case::passed:
         if (O.verbose) {
            print_perfix(true);
            h2_color::prints("green", "Passed ");
            print_title(s->name, c->name, c->file, c->line);
            if (0 < c->asserts) {
               h2_color::prints("dark gray", " - ");
               h2_color::prints("", "%d assert%s", c->asserts, 1 < c->asserts ? "s" : "");
            }
            if (0 < c->footprint) {
               h2_color::prints("dark gray", ",");
               h2_color::prints("", " %s", format_volume(c->footprint));
            }
            if (1 < case_cost) {
               h2_color::prints("dark gray", ",");
               h2_color::prints("", " %s", format_duration(case_cost));
            }
            h2_color::prints("", "\n");
         } else if (!O.debug)
            print_perfix(true);
         break;
      case h2_case::failed:
         print_perfix(true);
         h2_color::prints("bold,red", "Failed ");
         print_title(s->name, c->name, c->file, c->line);
         h2_color::prints("", "\n");
         if (O.compact) break;
         if (c->fails) c->fails->foreach([](h2_fail* fail, int subling_index, int child_index) { fail->print(subling_index, child_index); });
         h2_color::prints("", "\n");
         break;
      }
   }
};

struct h2_report_junit : h2_report_impl {
   FILE* f;
   void on_task_start(h2_task* t) override
   {
      h2_report_impl::on_task_start(t);
      f = fopen(O.junit, "w");
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

      static constexpr const char* CSS[] = {"Initial", "Passed", "Failed", "Ignored"};
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, c->todo ? "TODO" : (c->filtered ? "Filtered" : CSS[c->status]), case_cost / 1000.0);

      if (c->status == h2_case::failed) {
         fprintf(f, "<failure message=\"%s:%d:", c->file, c->line);
         if (c->fails) c->fails->foreach([=](h2_fail* fail, int subling_index, int child_index) {fprintf(f, "{newline}"); fail->print(f); });
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
   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);
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
   if (O.list_cases) {
      I().reports.push_back(list_report.x);
   } else {
      I().reports.push_back(console_report.x);
      if (strlen(O.junit)) I().reports.push_back(junit_report.x);
      if (strlen(O.tap)) I().reports.push_back(tap_report.x);
   }
}

h2_inline void h2_report::on_task_start(h2_task* t)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_task_start(t);
}
h2_inline void h2_report::on_task_endup(h2_task* t)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_task_endup(t);
}
h2_inline void h2_report::on_suite_start(h2_suite* s)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_suite_start(s);
}
h2_inline void h2_report::on_suite_endup(h2_suite* s)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_suite_endup(s);
}
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_case_start(s, c);
}
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c)
{
   h2_list_for_each_entry (p, reports, h2_report_impl, x)
      p->on_case_endup(s, c);
}

// source/other/h2_layout.cpp

static inline h2_rows row_break(const h2_row& row, unsigned width)
{
   h2_rows rows;
   h2_string current_style;
   h2_row wrap;
   unsigned length = 0;

   for (auto& word : row) {
      if (h2_color::isctrl(word.c_str())) {  // + - style , issue
         wrap.push_back(word);
         current_style = word;
      } else {
         for (auto& c : word) {
            if (width <= length) {  // terminate line as later as possible
               rows.push_back(wrap);
               wrap.clear();
               length = 0;
               if (current_style.size()) wrap.push_back(current_style);
            }
            wrap.push_back(h2_string(1, c));
            ++length;
         }
      }
   }
   if (length < width) wrap.push_back(h2_string(width - length, ' '));
   rows.push_back(wrap);
   return rows;
}

static inline unsigned int_width(int maximum, char scale)
{
   char t1[64], t2[64];
   sprintf(t1, "%%%c", scale);
   sprintf(t2, t1, maximum);
   return strlen(t2);
}

static inline void rows_merge(h2_rows& rows, const h2_rows& left_rows, const h2_rows& right_rows, unsigned left_width, unsigned right_width, int step, char scale, int seq_width)
{
   char seq_fmt[32];
   sprintf(seq_fmt, "%%%d%c│ ", seq_width, scale);
   const h2_string left_empty(left_width, ' '), right_empty(right_width, ' ');
   for (size_t i = 0; i < std::max(left_rows.size(), right_rows.size()); ++i) {
      auto left_wrap_rows = row_break(i < left_rows.size() ? left_rows[i] : left_empty, left_width);
      auto right_wrap_rows = row_break(i < right_rows.size() ? right_rows[i] : right_empty, right_width);
      for (size_t j = 0; j < std::max(left_wrap_rows.size(), right_wrap_rows.size()); ++j) {
         h2_row row;
         if (step) {
            if (j == 0)
               row.printf("dark gray", seq_fmt, step * i);
            else
               row.indent(seq_width + 2);
         }
         row += j < left_wrap_rows.size() ? left_wrap_rows[j].brush("reset") : color(left_empty, "reset");
         row.printf("dark gray", j < left_wrap_rows.size() - 1 ? "\\│ " : " │ ");
         row += j < right_wrap_rows.size() ? right_wrap_rows[j].brush("reset") : color(right_empty, "reset");
         row.printf("dark gray", j < right_wrap_rows.size() - 1 ? "\\" : " ");
         rows.push_back(row);
      }
   }
}

h2_inline h2_rows h2_layout::split(const h2_rows& left_rows, const h2_rows& right_rows, const char* left_title, const char* right_title, int step, char scale, unsigned width)
{
   unsigned seq_width = int_width(step * std::max(left_rows.size(), right_rows.size()), scale);
   unsigned valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

   unsigned left_width = std::max(left_rows.width(), 8u); /* at least title width */
   unsigned right_width = std::max(right_rows.width(), 8u);

   if (left_width < valid_width / 2)
      right_width = std::min(valid_width - left_width, right_width);
   else if (right_width < valid_width / 2)
      left_width = std::min(valid_width - right_width, left_width);
   else
      left_width = right_width = valid_width / 2;

   h2_rows rows;
   h2_row title_row = {"\033{reset}", "\033{dark gray}", (step ? h2_string(seq_width + 2, ' ') : ""), h2_string(left_title).center(left_width), "   ", h2_string(right_title).center(right_width), "\033{reset}"};
   rows.push_back(title_row);

   rows_merge(rows, left_rows, right_rows, left_width, right_width, step, scale, seq_width);
   return rows;
}

h2_inline h2_rows h2_layout::unified(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width)
{
   h2_rows rows;

   h2_row up_title_row = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_row down_title_row = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_rows up_rows = row_break(up_row, width - up_title_row.width());
   h2_rows down_rows = row_break(down_row, width - down_title_row.width());

   for (size_t i = 0; i < std::max(up_rows.size(), down_rows.size()); ++i) {
      if (i < up_rows.size())
         rows.push_back(up_title_row + up_rows[i]);
      if (i < down_rows.size())
         rows.push_back(down_title_row + down_rows[i]);
   }

   return rows;
}

static inline h2_rows prefix_break(const h2_row& row, const h2_row& title, unsigned width)
{
   h2_rows rows = row_break(row, width - title.width());

   for (size_t i = 0; i < rows.size(); ++i) {
      if (i == 0)
         rows[i] = title + rows[i];
      else
         rows[i].indent(title.width());
   }
   return rows;
}

h2_inline h2_rows h2_layout::seperate(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width)
{
   h2_row up_title_row = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_row down_title_row = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_rows rows = prefix_break(up_row, up_title_row, width);
   return rows += prefix_break(down_row, down_title_row, width);
}
// source/other/h2_option.cpp

static inline void usage()
{
   ::printf(" \033[33m╭────────────────────────────────────────────────────────────────────────────╮\033[0m\n");
   ::printf(" \033[33m│\033[0m               \033[32mh2unit \033[31m%-5g \033[34;4mhttps://github.com/lingjf/h2unit\033[0m \033[0;36m               \033[33m│\033[0m\n", H2UNIT_VERSION);
   ::printf(" \033[33m╰────────────────────────────────────────────────────────────────────────────╯\033[0m\n");

   ::printf("\
 ┌────────┬───────────┬───────────────────────────────────────────────────────┐\n\
 │ Option │ Parameter │ Description                                           │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -v     │           │ Verbose output                                        │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -q     │           │ Compact output without failure detail                 │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -l     │           │ List out suites and cases                             │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -s     │           │ Shuffle cases and execute in random order             │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -b[n]  │    [n]    │ Breaking test once n (default 1) failures occurred    │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -o     │           │ Only execute last failed cases                        │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -p     │           │ Hide execute progressing                              │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -r[n]  │    [n]    │ Repeat run n rounds (default 2) when no failure       │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -c     │           │ Output in black-white color style                     │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -f     │           │ Fold simple JSON object or array                      │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -y     │           │ Copy-paste JSON C/C++ source code                     │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -m     │           │ Run cases without memory check                        │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -x     │           │ Throw exception is considered as failure              │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -d     │           │ Debug with gdb once failure occurred                  │\n\
 ├────────┼───────────┼───────────────────────────────────────────────────────┤\n\
 │ -j     │  [path]   │ Generate junit report, default path is junit.xml      │\n\
 ├────────┼───────────┼─────────┬─────────────────────────────────────────────┤\n\
 │ -i     │           │ include │ Filter case, suite or file by matching      │\n\
 ├────────┤  patterns ├─────────┤ case-insensitive name with substr/wildcard  │\n\
 │ -e     │           │ exclude │ patterns which separated by space           │\n\
 └────────┴───────────┴─────────┴─────────────────────────────────────────────┘\n");
}

struct getopt {
   int argc;
   const char** args;  // argv + 1
   int i = -1;
   const char* j = nullptr;

   getopt(int argc_, const char** args_) : argc(argc_), args(args_) {}

   const char* extract_next()
   {
      return ++i < argc ? args[i] : nullptr;
   }

   const char* extract_string()
   {
      return (i + 1 < argc && args[i + 1] && args[i + 1][0] != '-') ? args[++i] : nullptr;
   }

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
         int l = strspn(j + 1, "0123456789");
         if (l) {
            value = atoi(j + 1);
            j += l;
            return;
         }
      }

      if (i + 1 < argc) {
         int l = strlen(args[i + 1]);
         if (l && strspn(args[i + 1], "0123456789") == l)
            value = atoi(args[++i]);
      }
   }

   void arguments(char* s)
   {
      for (int k = 0; k < argc; ++k)
         s += sprintf(s, " %s", args[k]);
   }
};

h2_inline h2_option::h2_option()
{
   terminal_width = h2_termimal_width();
#if defined _WIN32
   // memory_check = false;
   hProcess = GetCurrentProcess();
   SymInitialize(hProcess, NULL, TRUE);
#endif
}

h2_inline void h2_option::parse(int argc, const char** argv)
{
   path = argv[0];
   getopt get(argc - 1, argv + 1);
   get.arguments(args);

   for (const char* t;;) {
      switch (get.next_option()) {
      case '\0': return;
      case 'v': verbose = true; break;
      case 'q': compact = !compact; break;
      case 'c': colorful = !colorful; break;
      case 's': shuffle_order = !shuffle_order; break;
      case 'o': only_execute_fails = true; break;
      case 'f': fold_json = !fold_json; break;
      case 'p': execute_progress = !execute_progress; break;
      case 'y': copy_paste_json = true; break;
      case 'm': memory_check = !memory_check; break;
      case 'x': exception_fails = !exception_fails; break;
      case 'l': list_cases = 'l'; break;
      case 'L': list_cases = 'L'; break;
      case 'b':
         break_after_fails = 1;
         get.extract_number(break_after_fails);
         break;
      case 'r':
         rounds = 2;
         get.extract_number(rounds);
         break;
      case 'd': debug = "gdb new"; break;
      case 'D': debug = "gdb attach"; break;
      case 'j':
         sprintf(junit, "%s.xml", path);
         if ((t = get.extract_string())) strcpy(junit, t);
         break;
      case 'i':
         while ((t = get.extract_string())) includes.push_back(t);
         break;
      case 'e':
         while ((t = get.extract_string())) excludes.push_back(t);
         break;
      case 'h':
      case '?':
         usage();
         exit(0);
      }
   }
   if (list_cases) memory_check = false;
}

static inline bool match3(const std::vector<const char*>& patterns, const char* subject)
{
   for (auto pattern : patterns) {
      if (strcasestr(subject, pattern)) return true;
      if (strcspn(pattern, "?*[]") < strlen(pattern) && h2_pattern::wildcard_match(pattern, subject, true)) return true;
      // if (strcspn(pattern, "?*[]+^$\\.") < strlen(pattern) && h2_pattern::regex_match(pattern, subject, true)) return true;
   }
   return false;
}

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* file, int line) const
{
   char filename[1024];
   sprintf(filename, "%s:%d", file, line);
   if (!includes.empty())
      if (!match3(includes, suitename) && !match3(includes, casename) && !match3(includes, filename))
         return true;
   if (!excludes.empty())
      if (match3(excludes, suitename) || match3(excludes, casename) || match3(excludes, filename))
         return true;
   return false;
}

}  // namespace h2
