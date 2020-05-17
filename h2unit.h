/* v5.3  2020-05-17 18:59:41 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */
#ifndef __H2UNIT_H__
#define __H2UNIT_H__
#define H2UNIT_VERSION "5.3"
#ifndef ___H2UNIT_H___
#define ___H2UNIT_H___

#include <cstdio>      /* printf */
#include <cstdlib>     /* malloc */
#include <cstring>     /* strcpy, memcpy */
#include <cstddef>     /* ptrdiff_t */
#include <climits>     /* INT_MAX */
#include <cmath>       /* fabs */
#include <csetjmp>     /* setjmp, longjmp */
#include <sstream>     /* std::basic_ostringstream */
#include <string>      /* std::string */
#include <vector>      /* std::vector */
#include <tuple>       /* std::tuple */
#include <functional>  /* std::function */
#include <utility>     /* std::forward_as_tuple */
#include <type_traits> /* std::true_type */

#if defined _WIN32
#   define WIN32_LEAN_AND_MEAN /* fix winsock.h winsock2.h conflict */
#   define NOMINMAX            /* fix std::min/max conflict with windows::min/max */
#   include <windows.h>
#   include <malloc.h> /* alloca */
#   define alloca _alloca
#   define ssize_t int
#else
#   include <alloca.h> /* alloca */
#endif

#if defined __GNUC__
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#   pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#   pragma GCC diagnostic ignored "-Wparentheses"
#   pragma GCC diagnostic ignored "-Wsign-compare"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wwrite-strings"
#elif defined __clang__
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#   pragma clang diagnostic ignored "-Wint-to-pointer-cast"
#   pragma clang diagnostic ignored "-Wparentheses"
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wwritable-strings"
#elif defined _WIN32
#   pragma warning(disable : 4005)  // macro-redefine
#   pragma warning(disable : 4018)  // -Wsign-compare
#   pragma warning(disable : 4244)  //
#   pragma warning(disable : 4819)  // Unicode
#   define _CRT_SECURE_NO_WARNINGS
#   define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#if defined __H2UNIT_HPP__
#   define h2_inline
#else
#   define h2_inline inline
#endif

namespace h2 {
/* clang-format off */

#define H2PP__CAT2(_1, _2) _1##_2
#define H2PP_CAT2(_1, _2) H2PP__CAT2(_1, _2)
#define H2PP__CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define H2PP_CAT5(_1, _2, _3, _4, _5) H2PP__CAT5(_1, _2, _3, _4, _5)

#define H2PP_COMMA(...) ,
#define H2PP_EAT(...)
#define H2PP_ID(_1) _1
#define H2PP_DEFER(...) __VA_ARGS__ H2PP_EAT()

#define H2PP_EVAL(...) H2PP_EVAL128(__VA_ARGS__)
#define H2PP_EVAL1024(...) H2PP_EVAL512(H2PP_EVAL512(__VA_ARGS__))
#define H2PP_EVAL512(...) H2PP_EVAL256(H2PP_EVAL256(__VA_ARGS__))
#define H2PP_EVAL256(...) H2PP_EVAL128(H2PP_EVAL128(__VA_ARGS__))
#define H2PP_EVAL128(...) H2PP_EVAL64(H2PP_EVAL64(__VA_ARGS__))
#define H2PP_EVAL64(...) H2PP_EVAL32(H2PP_EVAL32(__VA_ARGS__))
#define H2PP_EVAL32(...) H2PP_EVAL16(H2PP_EVAL16(__VA_ARGS__))
#define H2PP_EVAL16(...) H2PP_EVAL8(H2PP_EVAL8(__VA_ARGS__))
#define H2PP_EVAL8(...) H2PP_EVAL4(H2PP_EVAL4(__VA_ARGS__))
#define H2PP_EVAL4(...) H2PP_EVAL2(H2PP_EVAL2(__VA_ARGS__))
#define H2PP_EVAL2(...) H2PP_EVAL1(H2PP_EVAL1(__VA_ARGS__))
#define H2PP_EVAL1(...) __VA_ARGS__

#define H2PP_1st(_1, ...) _1
#define H2PP_2nd(_1, _2, ...) _2
#define H2PP_3rd(_1, _2, _3, ...) _3
#define H2PP_4th(_1, _2, _3, _4, ...) _4

// #define H2PP_IS_PROBE(...) H2PP_2nd(__VA_ARGS__, 0, )
#define H2PP_IS_PROBE(...) _H2PP_MSVC_VA_ARGS_WORKAROUND(H2PP_2nd, (__VA_ARGS__, 0))
#define _H2PP_MSVC_VA_ARGS_WORKAROUND(_Define, _Args) _Define _Args
#define H2PP_PROBE() ~, 1,

#define H2PP_EQ(x, y) H2PP_IS_PROBE(H2PP_CAT5(_H2PP_EQ, __, x, __, y))
#define _H2PP_EQ__0__0 H2PP_PROBE()
#define _H2PP_EQ__1__1 H2PP_PROBE()
#define _H2PP_EQ__2__2 H2PP_PROBE()
#define _H2PP_EQ__3__3 H2PP_PROBE()
#define _H2PP_EQ__4__4 H2PP_PROBE()
#define _H2PP_EQ__5__5 H2PP_PROBE()
#define _H2PP_EQ__6__6 H2PP_PROBE()
#define _H2PP_EQ__7__7 H2PP_PROBE()
#define _H2PP_EQ__8__8 H2PP_PROBE()
#define _H2PP_EQ__9__9 H2PP_PROBE()

#define H2PP_NOT(_Cond) H2PP_IS_PROBE(H2PP__CAT2(_H2PP_NOT_, _Cond))
#define _H2PP_NOT_0 H2PP_PROBE()

#define H2PP_COMPL(_Value) H2PP__CAT2(_H2PP_COMPL_, _Value)
#define _H2PP_COMPL_0 1
#define _H2PP_COMPL_1 0

#define H2PP_BOOL(_Cond) H2PP_COMPL(H2PP_NOT(_Cond))  // non-zero and thus evaluate to 1

#define H2PP_AND(x, y) H2PP_CAT2(_H2PP_AND_, H2PP_BOOL(x))(H2PP_BOOL(y))
#define _H2PP_AND_0(y) 0
#define _H2PP_AND_1(y) y

#define H2PP_OR(x, y) H2PP_CAT2(_H2PP_OR_, H2PP_BOOL(x))(H2PP_BOOL(y))
#define _H2PP_OR_0(y) y
#define _H2PP_OR_1(y) 1

#define H2PP_IF_ELSE(_Cond, _Then, _Else) H2PP_CAT2(_H2PP_IF_ELSE_, _Cond) (_Then, _Else)
#define _H2PP_IF_ELSE_1(_Then, _Else) _Then
#define _H2PP_IF_ELSE_0(_Then, _Else) _Else

#define H2PP_IF(_Cond) H2PP_CAT2(_H2PP_IF_, H2PP_BOOL(_Cond))
#define _H2PP_IF_1(...) __VA_ARGS__
#define _H2PP_IF_0(...)

#define H2PP_IS_BEGIN_PARENTHESES(...) H2PP_IS_PROBE(_H2PP_IS_BEGIN_PARENTHESES_PROBE __VA_ARGS__)
#define _H2PP_IS_BEGIN_PARENTHESES_PROBE(...) H2PP_PROBE()

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define H2PP_IS_EMPTY(...)                                  \
  _H2PP_IS_EMPTY(_H2PP_HAS_COMMA(__VA_ARGS__),              \
                 _H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__),   \
                 _H2PP_HAS_COMMA(__VA_ARGS__()),            \
                 _H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__()))
#define _H2PP_IS_EMPTY(_1, _2, _3, _4) _H2PP_HAS_COMMA(H2PP__CAT5(_H2PP_IS_EMPTY_CASE_, _1, _2, _3, _4))
#define _H2PP_IS_EMPTY_CASE_0001 ,
#define _H2PP_HAS_COMMA(...) _H2PP_100TH((__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))

#define __H2PP_100TH(_100, _99, _98, _97, _96, _95, _94, _93, _92, _91, _90, _89, _88, _87, _86, _85, _84, _83, _82, _81, _80, _79, _78, _77, _76, _75, _74, _73, _72, _71, _70, _69, _68, _67, _66, _65, _64, _63, _62, _61, _60, _59, _58, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, _41, _40, _39, _38, _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _x, ...) _x
#define _H2PP_100TH(_Args) H2PP_ID(__H2PP_100TH _Args) // workaround for msvc

#define H2PP_NARG(...) H2PP_IF_ELSE(H2PP_IS_EMPTY(__VA_ARGS__), 0, _H2PP_NARG(__VA_ARGS__))
#define _H2PP_NARG(...) _H2PP_100TH((__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define H2PP_HEAD(...) H2PP_CAT2(_H2PP_HEAD_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_HEAD_0(_1, ...) _1
#define _H2PP_HEAD_1(...)

#define H2PP_TAIL(...) H2PP_CAT2(_H2PP_TAIL_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_TAIL_0(_1, ...) __VA_ARGS__
#define _H2PP_TAIL_1(...)

#define H2PP_LAST(...) H2PP_CAT2(_H2PP_LAST_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__) //is empty?
#define _H2PP_LAST_1(...) // empty
#define _H2PP_LAST_0(...) H2PP_EVAL(_H2PP_LAST_(__VA_ARGS__)) //at least 1 argument
#define _H2PP_LAST_(...) H2PP_CAT2(_H2PP_LAST__, H2PP_EQ(1, H2PP_NARG(__VA_ARGS__))) (__VA_ARGS__) // one and only one?
#define _H2PP_LAST__0(...) H2PP_DEFER(_H2PP_LAST_I)()(H2PP_TAIL(__VA_ARGS__)) // shift first, and again
#define _H2PP_LAST__1(...) __VA_ARGS__ // only 1 argument
#define _H2PP_LAST_I() _H2PP_LAST_

#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_ID(H2PP_CAT2(_Macro, H2PP_NARG(__VA_ARGS__))(__VA_ARGS__))

#define H2Q(_Prefix) H2PP_CAT5(_Prefix, _C, __COUNTER__, L, __LINE__)

template <typename U, typename = void>
struct h2_decay_impl {
   typedef U type;
};
template <>
struct h2_decay_impl<char*> {
   typedef const char* type;
};
template <typename U>
struct h2_decay_impl<U, typename std::enable_if<std::is_enum<U>::value>::type> {
   typedef int type;
};

template <typename T>
struct h2_decay {
   using type = typename h2_decay_impl<typename std::decay<T>::type>::type;
};

struct h2_once {
   int bcc;
   h2_once() : bcc(0) {}
   operator bool() { return 0 == bcc++; }
};

struct h2_with {
   FILE* f;
   int (*c)(FILE*);
   h2_with(FILE* file, int (*close)(FILE*) = ::fclose) : f(file), c(close) {}
   ~h2_with() { (f && c) && c(f); }
};

/* clang-format off */
#define h2_singleton(_Class) static _Class& I() { static _Class i; return i; }

#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))
#define h2_list_for_each_entry(p, head, type, link) for (type* p = h2_list_entry((head)->next, type, link), *t = h2_list_entry(p->link.next, type, link); &p->link != (head); p = t, t = h2_list_entry(t->link.next, type, link))

#define h2_list_pop_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->pop(), type, link))
#define h2_list_top_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_first(), type, link))
#define h2_list_bottom_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_last(), type, link))

struct h2_list {
   struct h2_list *next, *prev;

   static void __add_between(h2_list* _new, h2_list* prev, h2_list* next);

   h2_list() : next(this), prev(this) {}

   void add_head(h2_list* entry) { __add_between(entry, this, this->next); }
   void add_tail(h2_list* entry) { __add_between(entry, this->prev, this); }
   void add_before(h2_list* entry) { __add_between(entry, this->prev, this); }
   void add_after(h2_list* entry) { __add_between(entry, this, this->next); }

   void push_back(h2_list* entry) { add_tail(entry); }
   void push(h2_list* entry) { add_head(entry); }
   h2_list* pop() { return empty() ? nullptr : next->out(); }
   h2_list* pop_back() { return empty() ? nullptr : prev->out(); }

   bool is_first(h2_list* entry) const { return next == entry; }
   bool is_last(h2_list* entry) const { return prev == entry; }

   h2_list* get_first() const { return empty() ? nullptr : next; }
   h2_list* get_last() const { return empty() ? nullptr : prev; }

   h2_list* out();

   bool empty() const { return next == this; }
   int count() const;
};

struct h2_option {
   h2_singleton(h2_option);

#if defined __linux__
   static constexpr const int os = 1;
#elif defined __APPLE__
   static constexpr const int os = 2;
#elif defined _WIN32
   static constexpr const int os = 3;
#endif

   const char *path, *debug;
   int verbose, breakable, randomize, times;
   bool colorable, memory_check, listing;
   char junit[256], args[256];
   std::vector<const char*> includes, excludes;

   h2_option() : debug(nullptr), verbose(0), breakable(0), randomize(0), times(1), colorable(true), memory_check(true), listing(false), junit{0} {}

   void parse(int argc, const char** argv);

   int isLinux() const { return 1 == os; }
   int isMAC() const { return 2 == os; }
   int isWindows() const { return 3 == os; }

   bool filter(const char* suitename, const char* casename, const char* filename) const;
};

static const h2_option& O = h2_option::I();  // for pretty

struct h2_libc {
   static void* malloc(size_t size);
   static void free(void* ptr);
   static ssize_t write(int fd, const void* buf, size_t count);
   static void* operator new(std::size_t size) { return malloc(size); }
   static void operator delete(void* ptr) { free(ptr); }
};

template <typename T>
class h2_allocator {
 public:
   typedef size_t size_type;
   typedef ptrdiff_t difference_type;
   typedef T* pointer;
   typedef const T* const_pointer;
   typedef T& reference;
   typedef const T& const_reference;
   typedef T value_type;

   h2_allocator() {}
   h2_allocator(const h2_allocator&) {}

   pointer allocate(size_type n, const void* = 0) { return (T*)h2_libc::malloc(n * sizeof(T)); }
   void deallocate(void* p, size_type) { h2_libc::free(p); }

   pointer address(reference x) const { return &x; }
   const_pointer address(const_reference x) const { return &x; }
   h2_allocator<T>& operator=(const h2_allocator&) { return *this; }
   void construct(pointer p, const T& val) { new ((T*)p) T(val); }
   void destroy(pointer p) { p->~T(); }
   size_type max_size() const { return size_t(-1); }
   template <typename U>
   struct rebind {
      typedef h2_allocator<U> other;
   };
   template <typename U>
   h2_allocator(const h2_allocator<U>&) {}
   template <typename U>
   h2_allocator& operator=(const h2_allocator<U>&) { return *this; }
};

template <typename T>
inline bool operator==(const h2_allocator<T>&, const h2_allocator<T>&) { return true; }
template <typename T>
inline bool operator!=(const h2_allocator<T>&, const h2_allocator<T>&) { return false; }

template <typename T>
using h2_vector = std::vector<T, h2_allocator<T>>;

using h2_ostringstream = std::basic_ostringstream<char, std::char_traits<char>, h2_allocator<char>>;

struct h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
   h2_string() : basic_string() {}
   h2_string(const h2_string& str) : basic_string(str.c_str()) {}
   h2_string(const std::string& str) : basic_string(str.c_str()) {}
   h2_string(const char* s) : basic_string(s) {}
   h2_string(const char* s, size_t n) : basic_string(s, n) {}
   h2_string(size_t n, char c) : basic_string(n, c) {}
   h2_string(const unsigned char* b) : basic_string((const char*)b) {}

   h2_string& operator=(const h2_string& str) { return assign(str.c_str()), *this; }
   h2_string& operator=(const std::string& str) { return assign(str.c_str()), *this; }
   h2_string& operator=(const char* s) { return assign(s), *this; }
   h2_string& operator=(char c) { return assign(1, c), *this; }
   h2_string& operator=(const unsigned char* b) { return assign((const char*)b), *this; }

   h2_string& operator+=(const h2_string& str) { return append(str.c_str()), *this; }
   h2_string& operator+=(const std::string& str) { return append(str.c_str()), *this; }
   h2_string& operator+=(const char* s) { return append(s), *this; }
   h2_string& operator+=(char c) { return push_back(c), *this; }

   bool equals(h2_string str, bool caseless = false) const;
   bool contains(h2_string substr, bool caseless = false) const;
   bool startswith(h2_string prefix, bool caseless = false) const;
   bool endswith(h2_string suffix, bool caseless = false) const;

   bool isspace() const;

   h2_string& tolower();
   static h2_string tolower(h2_string from) { return from.tolower(); }
   h2_string acronym(int width = 16, int tail = 0) const;
   h2_string& center(int width);
   h2_string& sprintf(const char* format, ...);
};

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }

template <typename T>
h2_string h2_stringify(T a)
{
   h2_ostringstream os;
   os << std::boolalpha;
   os << a;
   return h2_string(os.str().c_str());
}
template <>
inline h2_string h2_stringify(std::nullptr_t a)
{
   return "nullptr";
}
struct h2_line : public h2_vector<h2_string> {
   h2_line() {}
   h2_line(h2_string a) : h2_vector<h2_string>({a}) {}
   h2_line(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   int length() const;
   void indent(int n_space);
   void padding(int n_space);

   void printf(const char* style, const char* format, ...);
   void concat_back(const char* style, h2_line& line);
   void concat_front(const char* style, h2_line& line);

   void fold(h2_vector<h2_line>& lines);
   void samesizify(h2_line& b);
};

struct h2_lines : public h2_vector<h2_line> {
   h2_lines() {}
   h2_lines(std::initializer_list<h2_line> il) : h2_vector<h2_line>(il) {}

   void concat_back(h2_lines& lines);
   void concat_front(h2_lines& lines);

   int max_length() const;
   void samesizify(h2_lines& b);
   bool foldable();

   void sequence(int indent, int start = 0);
};
struct h2_layout {
   static h2_lines split(h2_lines& left_lines, h2_lines& right_lines, const char* left_title, const char* right_title);
   static h2_lines unified(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title);
   static h2_lines seperate(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title);
};

struct h2_color {
   static void printf(const char* style, const char* format, ...);
   static void printf(h2_line& line);
   static void printf(h2_lines& lines);

   static bool is_ctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};

template <typename T>
class h2_shared_ptr : h2_libc {
 public:
   h2_shared_ptr() : px(nullptr), pn(nullptr) {}
   explicit h2_shared_ptr(T* p) { acquire(p, nullptr); }
   h2_shared_ptr(const h2_shared_ptr& that) { acquire(that.px, that.pn); }
   ~h2_shared_ptr() { release(); }
   h2_shared_ptr& operator=(h2_shared_ptr that)
   {
      std::swap(px, that.px);
      std::swap(pn, that.pn);
      return *this;
   }
   operator bool() const { return pn && 0 < *pn; }
   T& operator*() const { return *px; }
   T* operator->() const { return px; }

 private:
   void acquire(T* p, long* n)
   {
      pn = n;
      if (p) {
         if (!pn)
            pn = new (h2_libc::malloc(sizeof(long))) long(1);
         else
            ++(*pn);
      }
      px = p;
   }
   void release()
   {
      if (pn && !--(*pn)) {
         delete px;
         h2_libc::free(pn);
      }
   }

   T* px;
   long* pn;
};

struct h2_debugger {
   static void trap();
};

#define h2_debug(...)                                                                               \
   do {                                                                                             \
      if (!O.debug) {                                                                               \
         h2_color::printf("", "%s %s : %d = %s\n", #__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace bt(0);                                                                        \
         bt.print(3);                                                                               \
      }                                                                                             \
   } while (0)

struct tinyexpr {
   static double eval(const char* expression, int* error);
};

struct h2_json {
   static bool match(const h2_string& expect, const h2_string& actual);
   static void diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines);
};

struct h2_backtrace {
   int count, shift;
   void* array[100];

   h2_backtrace() : count(0), shift(0) {}
   h2_backtrace(int shift_);

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(h2_backtrace&);

   bool has(void* func, int size) const;
   void print(h2_vector<h2_string>& stacks) const;
   void print(int pad) const;
};

struct h2_fail : h2_libc {
   h2_fail *subling_next, *child_next;

   const char* file;
   int line;

   const char* func;
   int argi;

   int usage;  // 0 is Inner(Mock, AllOf, &&||); 1 is OK(condition); 2 is OK(expect, actual); 3 is JE
   h2_string e_expression, a_expression;
   h2_string user_explain;

   //    expression     explain      represent       value
   //     Ge(var)        Ge(5)        5               5
   //     We(var)        We("abc")    "abc"           abc

   h2_fail(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1)
     : subling_next(nullptr), child_next(nullptr), file(file_), line(line_), func(func_), argi(argi_), usage(0) {}
   virtual ~h2_fail();

   void set_locate(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1);
   const char* get_locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach (std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);
};

struct h2_fail_unexpect : h2_fail {
   h2_string e_represent, a_represent, e_explain;
   h2_fail_unexpect(h2_string e_represent_, h2_string a_represent_, bool dont_ = false, bool caseless_ = false, h2_string pre = "", h2_string post = "", const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e_represent(e_represent_), a_represent(a_represent_), e_explain((dont_ ? "!" : "") + ((caseless_ ? "~" : "") + (pre + e_represent_ + post))) {}

   void print_OK1(h2_line& line);
   void print_OK2(h2_line& line);
   void print_JE(h2_line& line);
   void print_Inner(h2_line& line);
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool dont_, bool caseless_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("\"" + e_value_ + "\"", "\"" + a_value_ + "\"", dont_, caseless_, "", "", file_, line_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_strfind : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, bool dont_, bool caseless_, h2_string find_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("\"" + e_value_ + "\"", "\"" + a_value_ + "\"", dont_, caseless_, find_ + "(", ")", file_, line_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_json : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, bool dont_, bool caseless_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(e_value_, a_value_, dont_, caseless_, "Je(", ")", file_, line_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_memcmp : h2_fail_unexpect {
   static constexpr const int npr_1b = 4;
   static constexpr const int npr_8b = 16;
   static constexpr const int npr_16b = 8;
   static constexpr const int npr_32b = 4;
   static constexpr const int npr_64b = 2;
   h2_vector<unsigned char> e_value, a_value;
   const int width, nbits;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, bool dont, const char* e_ext, h2_string a_represent_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("", a_represent_, dont, false, e_ext, "", file_, line_), e_value(e_value_, e_value_ + (nbits_ + 7) / 8), a_value(a_value_, a_value_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}
   void print(int subling_index = 0, int child_index = 0);
   void print_bits(h2_lines& e_lines, h2_lines& a_lines);
   void print_bytes(h2_lines& e_lines, h2_lines& a_lines);
   void print_int16(h2_lines& e_lines, h2_lines& a_lines);
   void print_int32(h2_lines& e_lines, h2_lines& a_lines);
   void print_int64(h2_lines& e_lines, h2_lines& a_lines);
};

struct h2_fail_memoverflow : h2_fail {
   const unsigned char* ptr;
   const int offset;
   const unsigned char* magic;
   const h2_vector<unsigned char> spot;
   const h2_backtrace bt0, bt1;
   h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_), ptr((unsigned char*)ptr_), offset(offset_), magic(magic_), spot(((unsigned char*)ptr_) + offset_, ((unsigned char*)ptr_) + offset_ + size), bt0(bt0_), bt1(bt1_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_memleak : h2_fail {
   const char* where;
   struct P {
      void *ptr, *ptr2;
      long long size, size2, bytes, times;
      h2_backtrace bt;
      P(void* ptr_, int size_, h2_backtrace& bt_) : ptr(ptr_), ptr2(nullptr), size(size_), size2(0), bytes(size_), times(1), bt(bt_) {}
   };
   h2_vector<P> places;
   long long bytes, times;

   h2_fail_memleak(const char* file_ = nullptr, int line_ = 0, const char* where_ = "") : h2_fail(file_, line_), where(where_), bytes(0), times(0) {}

   void add(void* ptr, int size, h2_backtrace& bt);
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_double_free : h2_fail {
   void* ptr;
   const h2_backtrace bt_allocate, bt_release1, bt_release2;
   h2_fail_double_free(void* ptr_, h2_backtrace& bt_allocate_, h2_backtrace& bt_release1_, h2_backtrace& bt_release2_) : h2_fail(nullptr, 0), ptr(ptr_), bt_allocate(bt_allocate_), bt_release1(bt_release1_), bt_release2(bt_release2_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_access_after_free : h2_fail {
   const unsigned char* ptr;
   int offset;
   const h2_backtrace bt_allocate, bt_release, bt_access;
   h2_fail_access_after_free(const unsigned char* ptr_, int offset_, h2_backtrace& bt_allocate_, h2_backtrace& bt_release_, h2_backtrace& bt_access_) : h2_fail(nullptr, 0), ptr(ptr_), offset(offset_), bt_allocate(bt_allocate_), bt_release(bt_release_), bt_access(bt_access_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_asymmetric_free : h2_fail {
   void* ptr;
   const char *who_allocate, *who_release;
   const h2_backtrace bt_allocate, bt_release;
   h2_fail_asymmetric_free(void* ptr_, const char* who_allocate_, const char* who_release_, h2_backtrace& bt_allocate_, h2_backtrace& bt_release_) : h2_fail(nullptr, 0), ptr(ptr_), who_allocate(who_allocate_), who_release(who_release_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_call : h2_fail {
   h2_string e_who, e_call, a_call;
   h2_fail_call(const char* func_, const char* expect, const char* actual, const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_), e_who(func_), e_call(expect), a_call(actual) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_fail_instantiate : h2_fail {
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   const bool why_abstract;
   h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_abstract_, const char* file_, int line_) : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why_abstract(why_abstract_) {}
   void print(int subling_index = 0, int child_index = 0);
};

struct h2_stubs {
   h2_list stubs;
   bool add(void* befp, void* tofp, const char* befn = "", const char* tofn = "", const char* file = nullptr, int line = 0);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   void* befp;
   char current[64];
   h2_stub_temporary_restore(void* befp_);
   ~h2_stub_temporary_restore();
};

#define __H2STUB3(BeFunc, ToFunc, _1)                                                    \
   do {                                                                                  \
      h2::h2_stub_g((void*)BeFunc, (void*)ToFunc, #BeFunc, #ToFunc, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB4(Return, BeFunc, Args, Qt)                                         \
   struct {                                                                         \
      void operator=(Return(*toF) Args)                                             \
      {                                                                             \
         Return(*beF) Args = BeFunc;                                                \
         h2::h2_stub_g((void*)beF, (void*)(toF), #BeFunc, "~", __FILE__, __LINE__); \
      }                                                                             \
   } Qt;                                                                            \
   Qt = [] Args -> Return

#define __H2STUB50(Return, Class, Method, Args, Qt)                                                                                                                                                 \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that))                                                                                                                                                   \
      {                                                                                                                                                                                             \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Qt;                                                                                                                                                                                            \
   Qt = [](Class * that) -> Return

#define __H2STUB51(Return, Class, Method, Args, Qt)                                                                                                                                                 \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args)))                                                                                                                    \
      {                                                                                                                                                                                             \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Qt;                                                                                                                                                                                            \
   Qt = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB5(Return, Class, Method, Args, Qt) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Args, __H2STUB50(Return, Class, Method, Args, Qt), __H2STUB51(Return, Class, Method, Args, Qt))

#define __H2STUB60(Return, Class, Method, Args, Instance, Qt)                                                                                    \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that))                                                                                                \
      {                                                                                                                                          \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Qt;                                                                                                                                         \
   Qt = [](Class * that) -> Return

#define __H2STUB61(Return, Class, Method, Args, Instance, Qt)                                                                                    \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args)))                                                                 \
      {                                                                                                                                          \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Qt;                                                                                                                                         \
   Qt = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB6(Return, Class, Method, Args, Instance, Qt) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Args, __H2STUB60(Return, Class, Method, Args, Instance, Qt), __H2STUB61(Return, Class, Method, Args, Instance, Qt))

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__, H2Q(t_stub))

static inline void h2_fail_g(h2_fail* fail);

struct h2_heap {
   static void initialize();
   static void dohook();
   static void unhook();

   static void stack_push_block(const char* file, int line, const char* where, long long limited, const char* fill);
   static h2_fail* stack_pop_block();

   struct stack {
      static void root() { stack_push_block(__FILE__, __LINE__, "root", LLONG_MAX >> 9, nullptr); }
      static void push(const char* file, int line, long long limited = LLONG_MAX >> 9, const char* fill = nullptr) { stack_push_block(file, line, "case", limited, fill); }
      static h2_fail* pop() { return stack_pop_block(); }

      struct block : h2_once {
         block(const char* file, int line, long long limited = LLONG_MAX >> 9, const char* fill = nullptr) { stack_push_block(file, line, "block", limited, fill); }
         ~block() { h2_fail_g(stack_pop_block()); }
      };
   };
};

/* clang-format off */

template <typename...> using h2_void_t = void;

template <typename T, int I> struct h2_constructible_error {
   static T* O(void* m) { return static_cast<T*>(m = (void*)I); }
};

template <typename, typename> struct h2_constructible0_impl : std::false_type {};
template <typename T>
struct h2_constructible0_impl<h2_void_t<decltype(T())>, T> : std::true_type {
   static T* O(void* m) { return new (m) T(); }
};

template <typename, typename> struct h2_constructible1_impl : std::false_type {};
template <typename T>
struct h2_constructible1_impl<h2_void_t<decltype(T({}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}); }
};

template <typename, typename> struct h2_constructible2_impl : std::false_type {};
template <typename T>
struct h2_constructible2_impl<h2_void_t<decltype(T({}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}); }
};

template <typename, typename> struct h2_constructible3_impl : std::false_type {};
template <typename T>
struct h2_constructible3_impl<h2_void_t<decltype(T({}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}); }
};

template <typename, typename> struct h2_constructible4_impl : std::false_type {};
template <typename T>
struct h2_constructible4_impl<h2_void_t<decltype(T({}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}); }
};

template <typename, typename> struct h2_constructible5_impl : std::false_type {};
template <typename T>
struct h2_constructible5_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}); }
};

template <typename, typename> struct h2_constructible6_impl : std::false_type {};
template <typename T>
struct h2_constructible6_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}); }
};

template <typename, typename> struct h2_constructible7_impl : std::false_type {};
template <typename T>
struct h2_constructible7_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}); }
};

template <typename, typename> struct h2_constructible8_impl : std::false_type {};
template <typename T>
struct h2_constructible8_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}); }
};

template <typename, typename> struct h2_constructible9_impl : std::false_type {};
template <typename T>
struct h2_constructible9_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}, {}); }
};

template <typename T> using h2_constructible0 = h2_constructible0_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible1 = h2_constructible1_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible2 = h2_constructible2_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible3 = h2_constructible3_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible4 = h2_constructible4_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible5 = h2_constructible5_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible6 = h2_constructible6_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible7 = h2_constructible7_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible8 = h2_constructible8_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible9 = h2_constructible9_impl<h2_void_t<>, T>;

template <typename T>
using h2_constructible = 
   typename std::conditional<std::is_abstract<T>::value,
      h2_constructible_error<T, 1>,
      typename std::conditional<std::is_default_constructible<T>::value,
         h2_constructible0<T>,
         typename std::conditional<h2_constructible1<T>::value,
            h2_constructible1<T>,
            typename std::conditional<h2_constructible2<T>::value,
               h2_constructible2<T>,
               typename std::conditional<h2_constructible3<T>::value,
                  h2_constructible3<T>,
                  typename std::conditional<h2_constructible4<T>::value,
                     h2_constructible4<T>,
                     typename std::conditional<h2_constructible5<T>::value,
                        h2_constructible5<T>,
                        typename std::conditional<h2_constructible6<T>::value,
                           h2_constructible6<T>,
                           typename std::conditional<h2_constructible7<T>::value,
                              h2_constructible7<T>,
                              typename std::conditional<h2_constructible8<T>::value,
                                 h2_constructible8<T>,
                                 typename std::conditional<h2_constructible9<T>::value,
                                    h2_constructible9<T>,
                                    h2_constructible_error<T, 2>
                                 >::type
                              >::type
                           >::type
                        >::type
                     >::type
                  >::type
               >::type
            >::type
         >::type
      >::type
   >::type;

/* clang-format on */

//  https://itanium-cxx-abi.github.io/cxx-abi/

//  &Class::Method has separate representations for non-virtual and virtual functions.
//  For non-virtual functions, it is the address of the function.
//  For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
//  The least-significant bit therefore discriminates between virtual and non-virtual functions.

template <typename Class, typename F>
struct h2_mfp;

template <typename Class, typename Return, typename... Args>
struct h2_mfp<Class, Return(Args...)> {
   typedef Return (Class::*F)(Args...);
   typedef union {
      F f;
      void* p;
      intptr_t v;
   } U;

   static inline bool is_virtual(U& u)
   {
      return (u.v & 1) && (u.v - 1) % sizeof(void*) == 0 && (u.v - 1) / sizeof(void*) < 1000;
   }

   static inline void* get_vmfp(U& u, Class* obj)
   {
      void** vtable = *(void***)obj;
      return vtable[(u.v - 1) / sizeof(void*)];
   }

   static void* A(F f, const char* action_type, const char* return_type, const char* class_type, const char* method_name, const char* return_args, const char* file, int line)
   {
      U u{f};
      if (!is_virtual(u)) return u.p;
      Class* o = h2_constructible<Class>::O(alloca(sizeof(Class)));
      if (1 == (intptr_t)o || 2 == (intptr_t)o)
         h2_fail_g(new h2_fail_instantiate(action_type, return_type, class_type, method_name, return_args, 1 == (intptr_t)o, file, line));
      return get_vmfp(u, o);
   }

   template <typename Derived>
   static void* A(F f, Derived obj)
   {
      U u{f};
      if (!is_virtual(u)) return u.p;
      return get_vmfp(u, dynamic_cast<Class*>(&obj));
   }
};

template <typename T>
struct h2_matcher_impl {
   virtual h2_fail* matches(T a, bool caseless = false, bool dont = false) const = 0;
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher_base {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher_base() {}
   explicit h2_matcher_base(const h2_matcher_impl<const T&>* impl_, const int placeholder) : impl(impl_) {}
   h2_matcher_base(const h2_matcher_base&) = default;
   h2_matcher_base& operator=(const h2_matcher_base&) = default;
   virtual ~h2_matcher_base() {}
   h2_fail* matches(const T& a, bool caseless = false, bool dont = false) const { return impl->matches(a, caseless, dont); }
};

template <typename T>
struct h2_matcher : h2_matcher_base<T> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl, const int placeholder) : h2_matcher_base<T>(impl, placeholder) {}
   h2_matcher(T value);  // Converting constructor
};

template <>
struct h2_matcher<const char*> : h2_matcher_base<const char*> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const char* const&>* impl, const int placeholder) : h2_matcher_base<const char*>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<const std::string&> : h2_matcher_base<const std::string&> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* impl, const int placeholder) : h2_matcher_base<const std::string&>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<std::string> : h2_matcher_base<std::string> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* impl, const int placeholder) : h2_matcher_base<std::string>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <typename Matches>
struct h2_polymorphic_matcher {
   const Matches m;
   explicit h2_polymorphic_matcher(const Matches& matches_) : m(matches_) {}

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(m), 0); }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      explicit internal_impl(const Matches& matches_) : m(matches_) {}
      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override { return m.matches(a, caseless, dont); }
   };
};

struct h2_string_equal_matches {
   const h2_string e;
   const bool r;
   h2_string_equal_matches(const h2_string& _e, bool _r = false) : e(_e), r(_r) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_float_equal_matches {
   const long double e;
   const bool r;
   explicit h2_float_equal_matches(const long double _e, bool _r) : e(_e), r(_r) {}
   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const;
};

template <typename E>
struct h2_equal_matches {
   const E e;
   const bool r;
   explicit h2_equal_matches(const E& _e, bool _r = false) : e(_e), r(_r) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if (r) dont = !dont;
      if ((a == e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), dont, false, "");
   }
};

template <>
struct h2_equal_matches<char*> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<char* const> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<const char*> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<const char* const> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<h2_string> : h2_string_equal_matches {
   explicit h2_equal_matches(const h2_string e, bool r = false) : h2_string_equal_matches(e, r) {}
};
template <>
struct h2_equal_matches<std::string> : h2_string_equal_matches {
   explicit h2_equal_matches(const std::string e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};

template <>
struct h2_equal_matches<float> : h2_float_equal_matches {
   explicit h2_equal_matches(const float e, bool r = false) : h2_float_equal_matches(e, r) {}
};
template <>
struct h2_equal_matches<double> : h2_float_equal_matches {
   explicit h2_equal_matches(const double e, bool r = false) : h2_float_equal_matches(e, r) {}
};
template <>
struct h2_equal_matches<long double> : h2_float_equal_matches {
   explicit h2_equal_matches(const long double e, bool r = false) : h2_float_equal_matches(e, r) {}
};

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = h2_polymorphic_matcher<h2_equal_matches<T>>(h2_equal_matches<T>(value)); }

inline h2_matcher<const char*>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<const char*>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }

template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from) { return do_cast(from, std::is_convertible<M, h2_matcher<T>>{}, std::is_convertible<M, T>{}); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type) { return h2_polymorphic_matcher<h2_equal_matches<M>>(h2_equal_matches<M>(from)); }
};

template <typename T, typename U>
struct h2_matcher_cast_impl<T, h2_matcher<U>> {
   static h2_matcher<T> cast(const h2_matcher<U>& from) { return h2_matcher<T>(new internal_impl(from)); }

   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}

      // Delegate the matching logic to the source h2_matcher.
      h2_fail* matches(T a, bool caseless, bool dont) const override
      {
         using FromType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::type;
         using ToType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<U>::type>::type>::type;
         // Do not allow implicitly converting base*/& to derived*/&.
         static_assert((std::is_pointer<typename std::remove_reference<T>::type>::value != std::is_pointer<typename std::remove_reference<U>::type>::value) || std::is_same<FromType, ToType>::value || !std::is_base_of<FromType, ToType>::value, "Can't implicitly convert from <base> to <derived>");

         return from.matches(static_cast<U>(a), caseless, dont);
      }
      const h2_matcher<U> from;
      void operator=(internal_impl const&) = delete;
   };
};

template <typename T>
struct h2_matcher_cast_impl<T, h2_matcher<T>> {
   static h2_matcher<T> cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> h2_matcher_cast(const M& from) { return h2_matcher_cast_impl<T, M>::cast(from); }

template <typename Matcher>
struct h2_not_matches {
   const Matcher m;
   explicit h2_not_matches(Matcher matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      return h2_matcher_cast<A>(m).matches(a, caseless, !dont);
   }
};

template <typename M1, typename M2>
struct h2_and_matches {
   const M1 m1;
   const M2 m2;
   explicit h2_and_matches(M1 _m1, M2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m1).matches(a, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m2).matches(a, caseless, false));
      if (!fail == !dont) return nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", h2_stringify(a), dont);
      }
      return fail;
   }
};

template <typename M1, typename M2>
struct h2_or_matches {
   const M1 m1;
   const M2 m2;
   explicit h2_or_matches(M1 _m1, M2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, caseless, false);
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, caseless, false);
      bool result = !f1 || !f2;
      if (result == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), dont);
   }
};

struct h2_any_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
};

struct h2_null_matches {
   const bool reverse;
   explicit h2_null_matches(bool _reverse) : reverse(_reverse) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if (reverse) dont = !dont;
      if ((nullptr == (const void*)a) == !dont) return nullptr;
      return new h2_fail_unexpect(dont ? "NotNull" : "IsNull", h2_stringify((const void*)a), false, false);
   }
};

struct h2_boolean_matches {
   const bool e;
   explicit h2_boolean_matches(bool _e) : e(_e) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if (!e) dont = !dont;
      if (((bool)a) == !dont) return nullptr;
      return new h2_fail_unexpect(dont ? "false" : "true", a ? "true" : "false");
   }
};

template <typename E>
struct h2_ge_matches {
   const E e;
   explicit h2_ge_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a >= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), dont, false, ">=");
   }
};

template <typename E>
struct h2_gt_matches {
   const E e;
   explicit h2_gt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a > e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), dont, false, ">");
   }
};

template <typename E>
struct h2_le_matches {
   const E e;
   explicit h2_le_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a <= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), dont, false, "<=");
   }
};

template <typename E>
struct h2_lt_matches {
   const E e;
   explicit h2_lt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a < e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), dont, false, "<");
   }
};

struct h2_memcmp_matches {
   const int width;
   const void* e;
   const int nbits;
   explicit h2_memcmp_matches(const int _width, const void* _e, const int _nbits) : width(_width), e(_e), nbits(_nbits) {}
   h2_fail* matches(const void* a, bool caseless = false, bool dont = false) const;
};

template <typename Matcher>
struct h2_pointee_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher matcher_) : m(matcher_) {}

   template <typename SmartPointer>
   struct PointeeOf {
      typedef typename SmartPointer::element_type type;
   };
   template <typename T>
   struct PointeeOf<T*> {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, caseless, dont);
   }
};

struct h2_regex_matches {
   const h2_string e;
   explicit h2_regex_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_wildcard_matches {
   const h2_string e;
   explicit h2_wildcard_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_strcmp_matches {
   const h2_string e;
   explicit h2_strcmp_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_contains_matches {
   const h2_string substring;
   explicit h2_contains_matches(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_startswith_matches {
   const h2_string prefix_string;
   explicit h2_startswith_matches(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_endswith_matches {
   const h2_string suffix_string;
   explicit h2_endswith_matches(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_json_matches {
   const h2_string e;
   explicit h2_json_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return m.matches(a, true, dont); }
};

#define H2_MATCHER_T2V(t_matchers)                                                                                                                          \
   template <typename T, size_t I>                                                                                                                          \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), h2_vector<h2_matcher<T>> v_matchers = {}) const \
   {                                                                                                                                                        \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                                                                    \
      return t2v<T>(std::integral_constant<size_t, I + 1>(), v_matchers);                                                                                   \
   }                                                                                                                                                        \
   template <typename T>                                                                                                                                    \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, sizeof...(Matchers)>, h2_vector<h2_matcher<T>> v_matchers = {}) const { return v_matchers; }

template <typename... Matchers>
struct h2_allof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* f = v_matchers[i].matches(a, caseless, false);
         h2_fail::append_subling(fails, f);
      }

      if (!fails == !dont) return nullptr;
      h2_fail* fail = (h2_fail*)new h2_fail_unexpect("", h2_stringify(a), dont);
      if (!dont) {
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* f = v_matchers[i].matches(a, caseless, false);
         if (!f) s++;
         h2_fail::append_subling(fails, f);
      }

      if ((0 < s) == !dont) return nullptr;
      h2_fail* fail = (h2_fail*)new h2_fail_unexpect("", h2_stringify(a), dont);
      if (!dont) {
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* f = m.matches(a, caseless, false);
         if (!f) ++s;
      }

      if ((s == 0) == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), dont);
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_listof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<decltype(a[0]), 0>();
      h2_fail* fail = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i)
         h2_fail::append_subling(fail, v_matchers[i].matches(a[i], caseless, dont));
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

const h2_polymorphic_matcher<h2_any_matches> _{h2_any_matches()};
const h2_polymorphic_matcher<h2_any_matches> Any{h2_any_matches()};
const h2_polymorphic_matcher<h2_null_matches> IsNull{h2_null_matches(false)};
const h2_polymorphic_matcher<h2_null_matches> NotNull{h2_null_matches(true)};
const h2_polymorphic_matcher<h2_boolean_matches> IsTrue{h2_boolean_matches(true)};
const h2_polymorphic_matcher<h2_boolean_matches> IsFalse{h2_boolean_matches(false)};

template <typename E>
inline h2_polymorphic_matcher<h2_equal_matches<E>> Eq(const E expect) { return h2_polymorphic_matcher<h2_equal_matches<E>>(h2_equal_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_equal_matches<E>> Nq(const E expect) { return h2_polymorphic_matcher<h2_equal_matches<E>>(h2_equal_matches<E>(expect, true)); }
template <typename E>
inline h2_polymorphic_matcher<h2_ge_matches<E>> Ge(const E expect) { return h2_polymorphic_matcher<h2_ge_matches<E>>(h2_ge_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_gt_matches<E>> Gt(const E expect) { return h2_polymorphic_matcher<h2_gt_matches<E>>(h2_gt_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_le_matches<E>> Le(const E expect) { return h2_polymorphic_matcher<h2_le_matches<E>>(h2_le_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_lt_matches<E>> Lt(const E expect) { return h2_polymorphic_matcher<h2_lt_matches<E>>(h2_lt_matches<E>(expect)); }

inline h2_polymorphic_matcher<h2_memcmp_matches> Me(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(len ? 8 : 0, (const void*)ptr, len * 8)); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M1e(const char* str) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(0, (const void*)str, strlen(str))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M8e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(8, (const void*)ptr, len ? len * 8 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M16e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(16, (const void*)ptr, len ? len * 16 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M32e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(32, (const void*)ptr, len ? len * 32 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M64e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(64, (const void*)ptr, len ? len * 64 : strlen((const char*)ptr))); }

inline h2_polymorphic_matcher<h2_regex_matches> Re(const h2_string& regex_pattern) { return h2_polymorphic_matcher<h2_regex_matches>(h2_regex_matches(regex_pattern)); }
inline h2_polymorphic_matcher<h2_wildcard_matches> We(const h2_string& wildcard_pattern) { return h2_polymorphic_matcher<h2_wildcard_matches>(h2_wildcard_matches(wildcard_pattern)); }
inline h2_polymorphic_matcher<h2_strcmp_matches> Se(const h2_string& expect) { return h2_polymorphic_matcher<h2_strcmp_matches>(h2_strcmp_matches(expect)); }
inline h2_polymorphic_matcher<h2_contains_matches> Contains(const h2_string& substring) { return h2_polymorphic_matcher<h2_contains_matches>(h2_contains_matches(substring)); }
inline h2_polymorphic_matcher<h2_startswith_matches> StartsWith(const h2_string& prefix_string) { return h2_polymorphic_matcher<h2_startswith_matches>(h2_startswith_matches(prefix_string)); }
inline h2_polymorphic_matcher<h2_endswith_matches> EndsWith(const h2_string& suffix_string) { return h2_polymorphic_matcher<h2_endswith_matches>(h2_endswith_matches(suffix_string)); }
inline h2_polymorphic_matcher<h2_json_matches> Je(const h2_string& expect) { return h2_polymorphic_matcher<h2_json_matches>(h2_json_matches(expect)); }

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(M m) { return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(h2_matcher<h2_string>(m))); }
#ifndef _WIN32
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> operator~(M m)
{
   return CaseLess(m);
}
#endif
template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher m)
{
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(m));
}
template <typename Matches>
inline h2_polymorphic_matcher<h2_not_matches<h2_polymorphic_matcher<Matches>>> operator!(const h2_polymorphic_matcher<Matches>& m) { return Not(m); }

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> AllOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>>(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> AnyOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>>(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> NoneOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>>(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator&&(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator||(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

struct h2_callexp {
   int call, least, most;

   h2_callexp(int _least, int _most) : call(0), least(_least), most(_most) {}

   void operator++() { call += 1; }

   bool is_not_enough() const { return call < least; }
   bool is_satisfied() const { return least <= call && call <= most; }
   bool is_saturated() const { return call == most; }
   bool is_overmuch() const { return most < call; }

   h2_fail* check(const char* func, const char* file, int line);
   const char* actual();
   const char* expect();
};

template <typename Class, typename F>
struct h2_routine;

template <typename Class, typename Return, typename... Args>
struct h2_routine<Class, Return(Args...)> {
   Return _r;
   std::function<Return(Args...)> _f1;
   std::function<Return(Class*, Args...)> _f2;

   h2_routine() : _r(), _f1(), _f2() {}
   h2_routine(Return r) : _r(r), _f1(), _f2() {}
   h2_routine(std::function<Return(Args...)> f) : _f1(f) {}
   h2_routine(std::function<Return(Class*, Args...)> f) : _f2(f) {}

   Return operator()(Class* that, Args... args)
   {
      if (_f2)
         return _f2(that, args...);
      else if (_f1)
         return _f1(args...);
      else
         return _r;
   }
};

template <typename Class, typename... Args>
struct h2_routine<Class, void(Args...)> {
   std::function<void(Args...)> _f1;
   std::function<void(Class*, Args...)> _f2;

   h2_routine() : _f1(), _f2() {}
   h2_routine(std::function<void(Args...)> f) : _f1(f) {}
   h2_routine(std::function<void(Class*, Args...)> f) : _f2(f) {}

   void operator()(Class* that, Args... args)
   {
      if (_f2)
         _f2(that, args...);
      else if (_f1)
         _f1(args...);
   }
};

struct h2_mock : h2_libc {
   h2_list x;
   void *befp, *tofp;
   const char* befn;
   const char* file;
   int line;

   h2_vector<h2_callexp> c_array;
   int c_index;

   h2_mock(void* befp_, void* tofp_, const char* befn_, const char* file_, int line_)
     : befp(befp_), tofp(tofp_), befn(befn_), file(file_), line(line_), c_index(0) {}

   virtual void reset() = 0;

   h2_fail* times_check()
   {
      h2_fail* fail = nullptr;
      for (auto& c : c_array) h2_fail::append_subling(fail, c.check(befn, file, line));
      return fail;
   }
};

static inline void h2_mock_g(h2_mock*);

template <size_t I, typename T, typename... Args>
struct __nth_type_impl {
   using type = typename __nth_type_impl<I - 1, Args...>::type;
};
template <typename T, typename... Args>
struct __nth_type_impl<0, T, Args...> {
   using type = T;
};
template <size_t Index, typename... Args>
struct h2_nth_type {
   using type = typename __nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;
};

template <size_t Index, typename... Args>
using h2_nth_decay = typename h2_decay<typename h2_nth_type<Index, Args...>::type>::type;

template <size_t N>
struct h2_tuple_match {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args, const char* file, int line, const char* func)
   {
      h2_fail* fail = h2_tuple_match<N - 1>::matches(matchers, args, file, line, func);
      h2_fail* f = std::get<N - 1>(matchers).matches(std::get<N - 1>(args));
      if (f) f->set_locate(file, line, func, N - 1);
      h2_fail::append_subling(fail, f);
      return fail;
   }
};

template <>
struct h2_tuple_match<0> {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args, const char* file, int line, const char* func)
   {
      return nullptr;
   }
};

template <int Counter, int Lineno, typename Class, typename F>
class h2_mocker;

template <int Counter, int Lineno, typename Class, typename Return, typename... Args>
class h2_mocker<Counter, Lineno, Class, Return(Args...)> : h2_mock {
 private:
   h2_mocker(void* befp, const char* befn, const char* file, int line)
     : h2_mock(befp, std::is_same<std::false_type, Class>::value ? (void*)normal_function_stub : (void*)member_function_stub, befn, file, line) {}

   /* clang-format off */
#define MATCHER_Any_0_1_2_3_4_5_6_7_8_9           \
   h2_matcher<h2_nth_decay<0, Args...>> _0 = Any, \
   h2_matcher<h2_nth_decay<1, Args...>> _1 = Any, \
   h2_matcher<h2_nth_decay<2, Args...>> _2 = Any, \
   h2_matcher<h2_nth_decay<3, Args...>> _3 = Any, \
   h2_matcher<h2_nth_decay<4, Args...>> _4 = Any, \
   h2_matcher<h2_nth_decay<5, Args...>> _5 = Any, \
   h2_matcher<h2_nth_decay<6, Args...>> _6 = Any, \
   h2_matcher<h2_nth_decay<7, Args...>> _7 = Any, \
   h2_matcher<h2_nth_decay<8, Args...>> _8 = Any, \
   h2_matcher<h2_nth_decay<9, Args...>> _9 = Any
   /* clang-format on */

   using argument_tuple = std::tuple<Args..., int>;
   using matcher_tuple = std::tuple<h2_matcher<h2_nth_decay<0, Args...>>,
                                    h2_matcher<h2_nth_decay<1, Args...>>,
                                    h2_matcher<h2_nth_decay<2, Args...>>,
                                    h2_matcher<h2_nth_decay<3, Args...>>,
                                    h2_matcher<h2_nth_decay<4, Args...>>,
                                    h2_matcher<h2_nth_decay<5, Args...>>,
                                    h2_matcher<h2_nth_decay<6, Args...>>,
                                    h2_matcher<h2_nth_decay<7, Args...>>,
                                    h2_matcher<h2_nth_decay<8, Args...>>,
                                    h2_matcher<h2_nth_decay<9, Args...>>>;

   h2_vector<matcher_tuple> m_array;
   h2_vector<h2_routine<Class, Return(Args...)>> r_array;

   static Return normal_function_stub(Args... args)
   {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](nullptr, std::move(args)...);
   }

   static Return member_function_stub(Class* that, Args... args)
   {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](that, std::move(args)...);
   }

   h2_fail* matches(matcher_tuple& matchers, argument_tuple& args)
   {
      if (1 == std::tuple_size<argument_tuple>::value) return nullptr;
      return h2_tuple_match<std::tuple_size<argument_tuple>::value>::matches(matchers, args, file, line, befn);
   }

   int matches(Args... args)
   {
      argument_tuple a_tuple = std::make_tuple(args..., 0);
      int c_offset = -1;
      for (int i = c_index; i < c_array.size(); ++i) {
         h2_fail* fail = matches(m_array[i], a_tuple);
         if (fail) {
            if (c_array[i].is_not_enough()) h2_fail_g(fail);
            if (c_array[i].is_satisfied()) delete fail; /* continue; try next h2_callexp */
         } else {
            ++c_array[c_offset = i];
            if (c_array[i].is_saturated()) c_index += 1;
            break;
         }
      }
      if (-1 == c_offset) {
         h2_fail_g(new h2_fail_call(befn, "", "exceed", file, line));
      }
      return c_offset;
   }

   void reset() override { c_array.clear(), m_array.clear(), r_array.clear(), c_index = 0; }

 public:
   static h2_mocker& I(void* befp = nullptr, const char* befn = nullptr, const char* file = nullptr, int line = 0)
   {
      static h2_mocker* i = nullptr;
      if (!i) i = new h2_mocker(befp, befn, file, line);
      if (befp && file) {
         i->reset();
         h2_mock_g(i);
      }
      return *i;
   }

   h2_mocker& once(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      c_array.push_back(h2_callexp(1, 1));
      m_array.push_back(std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& twice(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      c_array.push_back(h2_callexp(2, 2));
      m_array.push_back(std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& times(int count)
   {
      c_array.push_back(h2_callexp(count, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& any(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      c_array.push_back(h2_callexp(0, INT_MAX));
      m_array.push_back(std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atleast(int count)
   {
      c_array.push_back(h2_callexp(count, INT_MAX));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atmost(int count)
   {
      c_array.push_back(h2_callexp(0, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& between(int left, int right)
   {
      c_array.push_back(h2_callexp(left, right));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& with(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      if (!m_array.empty()) m_array.back() = std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9);
      return *this;
   }

   /* clang-format off */
   h2_mocker& th1(h2_matcher<h2_nth_decay<0, Args...>> e = Any) { if (!m_array.empty()) std::get<0>(m_array.back()) = e; return *this; }
   h2_mocker& th2(h2_matcher<h2_nth_decay<1, Args...>> e = Any) { if (!m_array.empty()) std::get<1>(m_array.back()) = e; return *this; }
   h2_mocker& th3(h2_matcher<h2_nth_decay<2, Args...>> e = Any) { if (!m_array.empty()) std::get<2>(m_array.back()) = e; return *this; }
   h2_mocker& th4(h2_matcher<h2_nth_decay<3, Args...>> e = Any) { if (!m_array.empty()) std::get<3>(m_array.back()) = e; return *this; }
   h2_mocker& th5(h2_matcher<h2_nth_decay<4, Args...>> e = Any) { if (!m_array.empty()) std::get<4>(m_array.back()) = e; return *this; }
   h2_mocker& th6(h2_matcher<h2_nth_decay<5, Args...>> e = Any) { if (!m_array.empty()) std::get<5>(m_array.back()) = e; return *this; }
   h2_mocker& th7(h2_matcher<h2_nth_decay<6, Args...>> e = Any) { if (!m_array.empty()) std::get<6>(m_array.back()) = e; return *this; }
   h2_mocker& th8(h2_matcher<h2_nth_decay<7, Args...>> e = Any) { if (!m_array.empty()) std::get<7>(m_array.back()) = e; return *this; }
   h2_mocker& th9(h2_matcher<h2_nth_decay<8, Args...>> e = Any) { if (!m_array.empty()) std::get<8>(m_array.back()) = e; return *this; }
   /* clang-format on */

   h2_mocker& returns(h2_routine<Class, Return(Args...)> r)
   {
      if (!r_array.empty()) r_array.back() = r;
      return *this;
   }

   h2_mocker& does(std::function<Return(Args...)> f)
   {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return *this;
   }

   h2_mocker& does(std::function<Return(Class*, Args...)> f)
   {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return *this;
   }

   h2_mocker& operator=(std::function<Return(Args...)> f) { return does(f); }
   h2_mocker& operator=(std::function<Return(Class*, Args...)> f) { return does(f); }
};

struct h2_mocks {
   h2_list mocks;
   bool add(h2_mock* mock);
   h2_fail* clear();
};

#ifdef _WIN32
#   define __H2_LINE__ 0
#else
#   define __H2_LINE__ __LINE__
#endif

#define __H2MOCK2(BeFunc, Signature) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, std::false_type, Signature>::I((void*)BeFunc, #BeFunc, __FILE__, __LINE__)

#define __H2MOCK3(Class, Method, Signature) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, Class, Signature>::I(h2::h2_mfp<Class, Signature>::A(&Class::Method, "MOCK", "", #Class, #Method, #Signature, __FILE__, __LINE__), #Class "::" #Method, __FILE__, __LINE__)

#define __H2MOCK4(Class, Method, Signature, Instance) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, Class, Signature>::I(h2::h2_mfp<Class, Signature>::A(&Class::Method, Instance), #Class "::" #Method, __FILE__, __LINE__)

#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)

struct h2_stdio {
   static void initialize();
   static const char* capture_cout(const char* type = nullptr);
};

struct h2_dns : h2_libc {
   h2_list x, y;
   const char* name;
   int count;
   char array[32][128];
   h2_dns(const char* name_) : name(name_), count(0) {}
   static void setaddrinfo(int count, ...);
   static void initialize();
};

struct h2_dnses {
   h2_list dnses;
   void add(h2_dns* dns);
   void clear();
};

struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(data_, size_){};
};

struct h2_sock : h2_libc {
   h2_list x, y;
   h2_stubs stubs;

   struct socket {
      int fd;
      h2_string from, to;
      socket(int fd_, const char* from_, const char* to_) : fd(fd_), from(from_), to(to_) {}
   };

   h2_vector<socket> sockets;

   h2_sock();
   ~h2_sock();

   void put_outgoing(const char* from, const char* to, const char* data, size_t size);
   void put_outgoing(int fd, const char* data, size_t size);
   void put_incoming(const char* from, const char* to, const char* data, size_t size);

   char last_to[128];
   h2_list incoming, outgoing;
};

template <typename M1, typename M2, typename M3, typename M4>
struct h2_packet_matches {
   const M1 from;
   const M2 to;
   const M3 data;
   const M4 size;
   explicit h2_packet_matches(M1 from_, M2 to_, M3 data_, M4 size_) : from(from_), to(to_), data(data_), size(size_) {}

   h2_fail* matches(h2_packet* a, bool caseless = false, bool dont = false) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(from).matches(a->from.c_str(), caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(to).matches(a->to.c_str(), caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const unsigned char*>(data).matches((unsigned char*)a->data.data(), caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const int>(size).matches(a->data.length(), caseless, dont));
      return fails;
   }
};

template <typename M1, typename M2, typename M3, typename M4>
inline h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>> PktEq(M1 from = Any, M2 to = Any, M3 data = Any, M4 size = Any)
{
   return h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>>(h2_packet_matches<M1, M2, M3, M4>(from, to, data, size));
}

struct h2_socket {
   static h2_packet* start_and_fetch();
   static void inject_received(const void* packet, size_t size, const char* from, const char* to);
};

static constexpr const char* CSS[] = {"init", "Passed", "Failed", "TODO", "Filtered"};

struct h2_case {
   static constexpr const int INITED = 0, PASSED = 1, FAILED = 2, TODOED = 3, FILTED = 4;
   h2_list registered, sorted;
   const char* name;
   const char* file;
   int line;
   long long seq;
   int status;
   jmp_buf jump;
   h2_fail* fails;
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;
   h2_sock* sock;

   h2_case(const char* name_, int todo, const char* file_, int line_);

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() {}
   void post_cleanup();

   void do_fail(h2_fail* fail);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};

struct h2_suite {
   h2_list registered, sorted;
   const char* name;
   const char* file;
   int line;
   long long seq;
   h2_stubs stubs;
   h2_mocks mocks;
   int status_stats[8];
   jmp_buf jump;
   bool jumpable;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list registered_cases, sorted_cases;

   h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const char* file_, int line_);

   void enumerate();
   void execute(h2_case* c);

   void setup() {}
   void cleanup();

   struct installer {
      installer(h2_suite* s, h2_case* c)
      {
         static long long seq = INT_MAX;
         s->registered_cases.push_back(&c->registered);
         s->seq = c->seq = ++seq;
      }
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s) : thus(s) {}
      ~cleaner()
      {
         if (thus->jumpable) ::longjmp(thus->jump, 1);
      }
   };
};

struct h2_report {
   int total_cases, done_cases, percentage, ss, cs;
   long long tt, ts, tc;
   h2_report();
   virtual void on_task_start(int cases);
   virtual void on_task_endup(int status_stats[8], int round);
   virtual void on_suite_start(h2_suite* s);
   virtual void on_suite_endup(h2_suite* s);
   virtual void on_case_start(h2_suite* s, h2_case* c);
   virtual void on_case_endup(h2_suite* s, h2_case* c);
};

struct h2_reports {
   std::vector<h2_report*> reports;
   void initialize();
   void on_task_start(int cases);
   void on_task_endup(int status_stats[8], int round);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};

#define __Matches_Common(message)                                                                     \
   template <typename A>                                                                              \
   bool __matches(const A& a) const;                                                                  \
   template <typename A>                                                                              \
   h2::h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const                   \
   {                                                                                                  \
      h2::h2_fail_unexpect* fail = new h2::h2_fail_unexpect("", h2::h2_stringify(a), dont, caseless); \
      if (__matches(a) == !dont) return nullptr;                                                      \
      if (dont) {                                                                                     \
      } else {                                                                                        \
         h2::h2_ostringstream osm;                                                                    \
         osm << std::boolalpha << H2PP_REMOVE_PARENTHESES(message);                                   \
         fail->user_explain = osm.str().c_str();                                                      \
      }                                                                                               \
      return fail;                                                                                    \
   }

#define H2MATCHER0(name, message)                                                     \
   struct h2_##name##_matches {                                                       \
      explicit h2_##name##_matches() {}                                               \
      __Matches_Common(message)                                                       \
   };                                                                                 \
   const h2::h2_polymorphic_matcher<h2_##name##_matches> name{h2_##name##_matches()}; \
   template <typename A>                                                              \
   bool h2_##name##_matches::__matches(const A& a) const

#define H2MATCHER1(name, e1, message)                                                           \
   template <typename E1>                                                                       \
   struct h2_##name##_matches {                                                                 \
      const E1 e1;                                                                              \
      explicit h2_##name##_matches(const E1& _e1) : e1(_e1) {}                                  \
      __Matches_Common(message)                                                                 \
   };                                                                                           \
   template <typename E1>                                                                       \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1>> name(const E1 _e1)                \
   {                                                                                            \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1>>(h2_##name##_matches<E1>(_e1)); \
   }                                                                                            \
   template <typename E1>                                                                       \
   template <typename A>                                                                        \
   bool h2_##name##_matches<E1>::__matches(const A& a) const

#define H2MATCHER2(name, e1, e2, message)                                                                    \
   template <typename E1, typename E2>                                                                       \
   struct h2_##name##_matches {                                                                              \
      const E1 e1;                                                                                           \
      const E2 e2;                                                                                           \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2) : e1(_e1), e2(_e2) {}                       \
      __Matches_Common(message)                                                                              \
   };                                                                                                        \
   template <typename E1, typename E2>                                                                       \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>> name(const E1 _e1, const E2 _e2)           \
   {                                                                                                         \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>>(h2_##name##_matches<E1, E2>(_e1, _e2)); \
   }                                                                                                         \
   template <typename E1, typename E2>                                                                       \
   template <typename A>                                                                                     \
   bool h2_##name##_matches<E1, E2>::__matches(const A& a) const

#define H2MATCHER3(name, e1, e2, e3, message)                                                                             \
   template <typename E1, typename E2, typename E3>                                                                       \
   struct h2_##name##_matches {                                                                                           \
      const E1 e1;                                                                                                        \
      const E2 e2;                                                                                                        \
      const E3 e3;                                                                                                        \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3) : e1(_e1), e2(_e2), e3(_e3) {}            \
      __Matches_Common(message)                                                                                           \
   };                                                                                                                     \
   template <typename E1, typename E2, typename E3>                                                                       \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>> name(const E1 _e1, const E2 _e2, const E3 _e3)      \
   {                                                                                                                      \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>>(h2_##name##_matches<E1, E2, E3>(_e1, _e2, _e3)); \
   }                                                                                                                      \
   template <typename E1, typename E2, typename E3>                                                                       \
   template <typename A>                                                                                                  \
   bool h2_##name##_matches<E1, E2, E3>::__matches(const A& a) const

#define H2MATCHER4(name, e1, e2, e3, e4, message)                                                                                      \
   template <typename E1, typename E2, typename E3, typename E4>                                                                       \
   struct h2_##name##_matches {                                                                                                        \
      const E1 e1;                                                                                                                     \
      const E2 e2;                                                                                                                     \
      const E3 e3;                                                                                                                     \
      const E4 e4;                                                                                                                     \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4) : e1(_e1), e2(_e2), e3(_e3), e4(_e4) {} \
      __Matches_Common(message)                                                                                                        \
   };                                                                                                                                  \
   template <typename E1, typename E2, typename E3, typename E4>                                                                       \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4) \
   {                                                                                                                                   \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>>(h2_##name##_matches<E1, E2, E3, E4>(_e1, _e2, _e3, _e4)); \
   }                                                                                                                                   \
   template <typename E1, typename E2, typename E3, typename E4>                                                                       \
   template <typename A>                                                                                                               \
   bool h2_##name##_matches<E1, E2, E3, E4>::__matches(const A& a) const

#define H2MATCHER5(name, e1, e2, e3, e4, e5, message)                                                                                                          \
   template <typename E1, typename E2, typename E3, typename E4, typename E5>                                                                                  \
   struct h2_##name##_matches {                                                                                                                                \
      const E1 e1;                                                                                                                                             \
      const E2 e2;                                                                                                                                             \
      const E3 e3;                                                                                                                                             \
      const E4 e4;                                                                                                                                             \
      const E5 e5;                                                                                                                                             \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4, const E5& _e5) : e1(_e1), e2(_e2), e3(_e3), e4(_e4), e5(_e5) {} \
      __Matches_Common(message)                                                                                                                                \
   };                                                                                                                                                          \
   template <typename E1, typename E2, typename E3, typename E4, typename E5>                                                                                  \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4, const E5 _e5)       \
   {                                                                                                                                                           \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>>(h2_##name##_matches<E1, E2, E3, E4, E5>(_e1, _e2, _e3, _e4, _e5));            \
   }                                                                                                                                                           \
   template <typename E1, typename E2, typename E3, typename E4, typename E5>                                                                                  \
   template <typename A>                                                                                                                                       \
   bool h2_##name##_matches<E1, E2, E3, E4, E5>::__matches(const A& a) const

/* clang-format off */

#define _H2MATCHER1(name) H2MATCHER0(name, (""))

#define _H2MATCHER20(name, e1) H2MATCHER1(name, e1, (""))
#define _H2MATCHER21(name, message) H2MATCHER0(name, message)
#define _H2MATCHER2(name, t) H2PP_CAT2(_H2MATCHER2, H2PP_IS_BEGIN_PARENTHESES(t)) (name, t)

#define _H2MATCHER30(name, e1, e2) H2MATCHER2(name, e1, e2, (""))
#define _H2MATCHER31(name, e1, message) H2MATCHER1(name, e1, message)
#define _H2MATCHER3(name, e1, t) H2PP_CAT2(_H2MATCHER3, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, t)

#define _H2MATCHER40(name, e1, e2, e3) H2MATCHER3(name, e1, e2, e3, (""))
#define _H2MATCHER41(name, e1, e2, message) H2MATCHER2(name, e1, e2, message)
#define _H2MATCHER4(name, e1, e2, t) H2PP_CAT2(_H2MATCHER4, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, t)

#define _H2MATCHER50(name, e1, e2, e3, e4) H2MATCHER4(name, e1, e2, e3, e4, (""))
#define _H2MATCHER51(name, e1, e2, e3, message) H2MATCHER3(name, e1, e2, e3, message)
#define _H2MATCHER5(name, e1, e2, e3, t) H2PP_CAT2(_H2MATCHER5, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, e3, t)

#define _H2MATCHER60(name, e1, e2, e3, e4, e5) H2MATCHER5(name, e1, e2, e3, e4, e5, (""))
#define _H2MATCHER61(name, e1, e2, e3, e4, message) H2MATCHER4(name, e1, e2, e3, e4, message)
#define _H2MATCHER6(name, e1, e2, e3, e4, t) H2PP_CAT2(_H2MATCHER6, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, e3, e4, t)

#define _H2MATCHER70(name, e1, e2, e3, e4, e5, e6)
#define _H2MATCHER71(name, e1, e2, e3, e4, e5, message) H2MATCHER5(name, e1, e2, e3, e4, e5, message)
#define _H2MATCHER7(name, e1, e2, e3, e4, e5, t) H2PP_CAT2(_H2MATCHER7, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, e3, e4, e5, t)

/* clang-format on */

#define H2MATCHER(...) H2PP_VARIADIC_CALL(_H2MATCHER, __VA_ARGS__)

struct h2_directory {
   h2_singleton(h2_directory);

   h2_list registered_suites, sorted_suites;

   static void drop_last_order();
   static int sort();
};

struct h2_defer_fail : h2_once {
   int usage;
   const char *e_expression, *a_expression;
   const char* file;
   int line;
   h2_fail* fail;
   h2_ostringstream oss;

   h2_defer_fail(int usage_, const char* e_expression_, const char* a_expression_, const char* file_, int line_)
     : usage(usage_), e_expression(e_expression_), a_expression(a_expression_), file(file_), line(line_), fail(nullptr) {}

   ~h2_defer_fail()
   {
      if (fail) {
         fail->file = file;
         fail->line = line;
         fail->usage = usage;
         fail->e_expression = e_expression;
         fail->a_expression = a_expression;
         fail->user_explain = oss.str().c_str();
         h2_fail_g(fail);
      }
   }
};

static inline h2_ostringstream& h2_OK1(bool a, h2_defer_fail* d)
{
   if (!a) d->fail = new h2_fail_unexpect("true", "false");
   return d->oss;
}

template <typename E, typename A>
static inline h2_ostringstream& h2_OK2(E e, A a, h2_defer_fail* d)
{
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   h2_fail* fail = m.matches((typename h2_decay<A>::type)a);
   d->fail = fail;
   if (fail && fail->subling_next) {
      d->fail = new h2_fail_unexpect("", "");
      h2_fail::append_child(d->fail, fail);
   }
   return d->oss;
}

static inline h2_ostringstream& h2_JE(h2_string e, h2_string a, h2_defer_fail* d)
{
   h2::h2_matcher<h2_string> m = Je(e);
   d->fail = m.matches(a);
   return d->oss;
}

#define __H2OK2(condition, Qt) \
   for (h2::h2_defer_fail Qt(1, "", #condition, __FILE__, __LINE__); Qt;) h2::h2_OK1(condition, &Qt)

#define __H2OK3(expect, actual, Qt) \
   for (h2::h2_defer_fail Qt(2, #expect, #actual, __FILE__, __LINE__); Qt;) h2::h2_OK2(expect, actual, &Qt)

#define __H2JE(expect, actual, Qt) \
   for (h2::h2_defer_fail Qt(3, #expect, #actual, __FILE__, __LINE__); Qt;) h2::h2_JE(expect, actual, &Qt)

#define H2OK(...) H2PP_VARIADIC_CALL(__H2OK, __VA_ARGS__, H2Q(t_defer_fail))

#define H2JE(expect, actual) __H2JE(expect, actual, H2Q(t_defer_fail))

/* clang-format off */

#define _H2_Macro_II(_Func, _Args, ...) _Func(_Args, __VA_ARGS__)
#define _H2_Macro_I(...) _H2_Macro_II(__VA_ARGS__)


#define _H2_ForEachv(_Func, _Args, ...) H2PP_CAT2(_H2_ForEachv_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, __VA_ARGS__)
#define _H2_ForEachv_I() _H2_ForEachv
#define _H2_ForEachv_0(_Func, _Args, ...) _H2_Macro_I(_Func, _Args, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachv_I)()(_Func, _Args, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachv_1(...)
#define H2ForEach(_Func, _Args, ...) H2PP_EVAL(_H2_ForEachv(_Func, _Args, __VA_ARGS__))

#define _H2_ForEachy(_Func, _Args, x, ...) H2PP_CAT2(_H2_ForEachy_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, x, __VA_ARGS__)
#define _H2_ForEachy_I() _H2_ForEachy
#define _H2_ForEachy_0(_Func, _Args, x, ...) _H2_Macro_I(_Func, _Args, x, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachy_I)()(_Func, _Args, x, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachy_1(...)
#define _H2_ForEachx(_Func, _Args, _Tupley, ...) H2PP_CAT2(_H2_ForEachx_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, _Tupley, __VA_ARGS__)
#define _H2_ForEachx_I() _H2_ForEachx
#define _H2_ForEachx_0(_Func, _Args, _Tupley, ...) _H2_ForEachy(_Func, _Args, H2PP_HEAD(__VA_ARGS__), H2PP_REMOVE_PARENTHESES(_Tupley)) H2PP_DEFER(_H2_ForEachx_I)()(_Func, _Args, _Tupley, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachx_1(...)
#define _H2_Fullmesh_Impl(_Func, _Args, _Tuplex, _Tupley) H2PP_EVAL(_H2_ForEachx(_Func, _Args, _Tupley, H2PP_REMOVE_PARENTHESES(_Tuplex)))
#define __H2_Fullmesh_Prev0(_Func, _Args, ...) _H2_Fullmesh_Impl(_Func, _Args, (__VA_ARGS__), (__VA_ARGS__))
#define _H2_Fullmesh_Prev0(...) __H2_Fullmesh_Prev0(__VA_ARGS__)
#define _H2_Fullmesh_Prev1(...) _H2_Fullmesh_Impl(__VA_ARGS__)
#define _H2_Fullmesh_Prev(_Func, _Args, t, ...) H2PP_CAT2(_H2_Fullmesh_Prev, H2PP_AND(H2PP_IS_BEGIN_PARENTHESES(t), H2PP_EQ(1, H2PP_NARG(__VA_ARGS__)))) (_Func, _Args, t, __VA_ARGS__)
#define H2Fullmesh(_Func, _Args, ...) _H2_Fullmesh_Prev(_Func, _Args, __VA_ARGS__)


#define ForForEach_Adapter(_Args, x) H2PP_REMOVE_PARENTHESES(_Args)(x)
#define ForForEach(_Macro_x, ...) H2ForEach(ForForEach_Adapter, (_Macro_x), __VA_ARGS__)

#define ForFullmesh_Adapter(_Args, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)
#define ForFullmesh(_Macro_x_y, ...) H2Fullmesh(ForFullmesh_Adapter, (_Macro_x_y), __VA_ARGS__)


#define H2List_An(...) H2PP_CAT2(_H2List_An_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2List_An_0(_1, ...) _1
#define _H2List_An_1(...) 0

#define H2ForEach_An(...) H2PP_CAT2(_H2ForEach_An_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2ForEach_An_1(...) 0
#define _H2ForEach_An_0(_1, ...) H2PP_CAT2(_H2ForEach_An_0_, H2PP_IS_BEGIN_PARENTHESES(_1))(_1)
#define _H2ForEach_An_0_0(_1) H2List_An(_1)
#define _H2ForEach_An_0_1(_1) H2List_An(H2PP_REMOVE_PARENTHESES(_1))

#define H2Fullmesh_Ax(...) H2ForEach_An(H2PP_HEAD(__VA_ARGS__))
#define H2Fullmesh_Ay(...) H2ForEach_An(H2PP_LAST(__VA_ARGS__))

/* clang-format on */

#define ___ForEach_CASE_Macro(name, Qc, x) \
   CASE(name x) { Qc(x); }
#define __ForEach_CASE_Macro(...) ___ForEach_CASE_Macro(__VA_ARGS__)
#define _ForEach_CASE_Macro(Args, x) __ForEach_CASE_Macro(H2PP_REMOVE_PARENTHESES(Args), x)
#define _ForEach_CASE_Impl(name, Qc, ...)                   \
   template <typename T>                                    \
   void Qc(T x);                                            \
   H2ForEach(_ForEach_CASE_Macro, (name, Qc), __VA_ARGS__); \
   template <typename T>                                    \
   void Qc(T x)
#define H2CASES(name, ...) _ForEach_CASE_Impl(name, H2Q(f), __VA_ARGS__)

#define ___Fullmesh_CASE_Macro(name, Qc, x, y) \
   CASE(name x, y) { Qc(x, y); }
#define __Fullmesh_CASE_Macro(...) ___Fullmesh_CASE_Macro(__VA_ARGS__)
#define _Fullmesh_CASE_Macro(Args, x, y) __Fullmesh_CASE_Macro(H2PP_REMOVE_PARENTHESES(Args), x, y)
#define _Fullmesh_CASE_Impl(name, Qc, ...)                    \
   template <typename T, typename U>                          \
   void Qc(T x, U y);                                         \
   H2Fullmesh(_Fullmesh_CASE_Macro, (name, Qc), __VA_ARGS__); \
   template <typename T, typename U>                          \
   void Qc(T x, U y)
#define H2CASESS(name, ...) _Fullmesh_CASE_Impl(name, H2Q(f), __VA_ARGS__)

#define ___ForEach_Case_Macro(name, Qj, Qb, Qx, Ql, x) \
   Case(name x)                                        \
   {                                                   \
      if (::setjmp(Qj) == 0) {                         \
         Qx = x;                                       \
         Qb = true;                                    \
         goto Ql;                                      \
      } else {                                         \
      }                                                \
   };                                                  \
   Qb = false;
#define __ForEach_Case_Macro(...) ___ForEach_Case_Macro(__VA_ARGS__)
#define _ForEach_Case_Macro(Args, x) __ForEach_Case_Macro(H2PP_REMOVE_PARENTHESES(Args), x)
#define _ForEach_Case_Impl(name, Qj, Qb, Qx, Ql, ...)                   \
   jmp_buf Qj;                                                          \
   bool Qb = false;                                                     \
   auto Qx = H2ForEach_An(__VA_ARGS__);                                 \
   H2ForEach(_ForEach_Case_Macro, (name, Qj, Qb, Qx, Ql), __VA_ARGS__); \
   Ql:                                                                  \
   for (auto x = Qx; Qb; Qb = false, ::longjmp(Qj, 1))
#define H2Cases(name, ...) _ForEach_Case_Impl(name, H2Q(j), H2Q(b), H2Q(v), H2Q(l), __VA_ARGS__)

#define ___Fullmesh_Case_Macro(name, Qj, Qb, Ql, Qx, Qy, x, y) \
   Case(name x, y)                                             \
   {                                                           \
      if (::setjmp(Qj) == 0) {                                 \
         Qx = x;                                               \
         Qy = y;                                               \
         Qb = true;                                            \
         goto Ql;                                              \
      } else {                                                 \
      }                                                        \
   };                                                          \
   Qb = false;
#define __Fullmesh_Case_Macro(...) ___Fullmesh_Case_Macro(__VA_ARGS__)
#define _Fullmesh_Case_Macro(Args, x, y) __Fullmesh_Case_Macro(H2PP_REMOVE_PARENTHESES(Args), x, y)
#define _Fullmesh_Case_Impl(name, Qj, Qb, Ql, Qx, Qy, ...)                    \
   jmp_buf Qj;                                                                \
   bool Qb = false;                                                           \
   auto Qx = H2Fullmesh_Ax(__VA_ARGS__);                                      \
   auto Qy = H2Fullmesh_Ay(__VA_ARGS__);                                      \
   H2Fullmesh(_Fullmesh_Case_Macro, (name, Qj, Qb, Ql, Qx, Qy), __VA_ARGS__); \
   Ql:                                                                        \
   for (auto x = Qx; Qb; ::longjmp(Qj, 1))                                    \
      for (auto y = Qy; Qb; Qb = false)
#define H2Casess(name, ...) _Fullmesh_Case_Impl(name, H2Q(j), H2Q(b), H2Q(l), H2Q(x), H2Q(y), __VA_ARGS__)

struct h2_patch {
   static void initialize();
};

struct h2_task {
   h2_singleton(h2_task);

   h2_reports reports;
   h2_stubs stubs;
   h2_mocks mocks;
   int state, round, status_stats[8];
   h2_suite* current_suite;
   h2_case* current_case;
   std::vector<void (*)()> global_setups, global_teardowns;
   std::vector<void (*)()> global_suite_setups, global_suite_teardowns;
   std::vector<void (*)()> global_case_setups, global_case_teardowns;

   h2_task();
   int execute();
};

static inline void h2_stub_g(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line)
{
   if (20 <= h2_task::I().state) {
      if (h2_task::I().current_case)
         h2_task::I().current_case->stubs.add(befp, tofp, befn, tofn, file, line);
      else if (h2_task::I().current_suite)
         h2_task::I().current_suite->stubs.add(befp, tofp, befn, tofn, file, line);
      else
         h2_task::I().stubs.add(befp, tofp, befn, tofn, file, line);
   }
}

static inline void h2_mock_g(h2_mock* mock)
{
   if (20 <= h2_task::I().state) {
      if (h2_task::I().current_case)
         h2_task::I().current_case->mocks.add(mock) && h2_task::I().current_case->stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      else if (h2_task::I().current_suite)
         h2_task::I().current_suite->mocks.add(mock) && h2_task::I().current_suite->stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      else
         h2_task::I().mocks.add(mock) && h2_task::I().stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
   }
}

static inline void h2_fail_g(h2_fail* fail)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail(fail);
}
}  // namespace h2

/* ======> Interface <====== */

#define GlobalSetup() H2GlobalSetup()
#define GlobalTeardown() H2GlobalTeardown()

#define GlobalSuiteSetup() H2GlobalSuiteSetup()
#define GlobalSuiteTeardown() H2GlobalSuiteTeardown()

#define GlobalCaseSetup() H2GlobalCaseSetup()
#define GlobalCaseTeardown() H2GlobalCaseTeardown()

#define SUITE(...) H2SUITE(#__VA_ARGS__)
#define CASE(...) H2CASE(#__VA_ARGS__)
#define TODO(...) H2TODO(#__VA_ARGS__)

#define Cleanup() H2Cleanup()

#define Case(...) H2Case(#__VA_ARGS__)
#define Todo(...) H2Todo(#__VA_ARGS__)

#define OK(...) H2OK(__VA_ARGS__)
#define JE(e, a) H2JE(e, a)
#define MOCK(...) H2MOCK(__VA_ARGS__)
#define STUB(...) H2STUB(__VA_ARGS__)
#define BLOCK(...) H2BLOCK(__VA_ARGS__)
#define DNS(...) H2DNS(__VA_ARGS__)
#define SOCK(...) H2SOCK(__VA_ARGS__)
#define COUT(...) H2COUT(__VA_ARGS__)

#define MATCHER(...) H2MATCHER(__VA_ARGS__)

#define CASES(name, ...) H2CASES(#name, __VA_ARGS__)
#define CASESS(name, ...) H2CASESS(#name, __VA_ARGS__)
#define Cases(name, ...) H2Cases(#name, __VA_ARGS__)
#define Casess(name, ...) H2Casess(#name, __VA_ARGS__)

/* clang-format off */
using h2::_;
using h2::Any;
using h2::IsNull;
using h2::NotNull;
using h2::IsTrue;
using h2::IsFalse;
using h2::Eq;
using h2::Nq;
using h2::Ge;
using h2::Gt;
using h2::Le;
using h2::Lt;
using h2::Me;
using h2::M1e;
using h2::M8e;
using h2::M16e;
using h2::M32e;
using h2::M64e;
using h2::Re;
using h2::We;
using h2::Je;
using h2::Se;
using h2::Contains;
using h2::StartsWith;
using h2::EndsWith;
using h2::CaseLess;
#ifndef _WIN32
using h2::operator~;
#endif
using h2::Pointee;
using h2::Not;
using h2::operator!;
using h2::operator&&;
using h2::operator||;
using h2::AllOf;
using h2::AnyOf;
using h2::NoneOf;
using h2::ListOf;
/* clang-format on */

/* ==================> implementation <============================= */

#define __H2GlobalCallback(name, Q)                        \
   namespace {                                             \
      static struct Q {                                    \
         Q() { h2::h2_task::I().name##s.push_back(name); } \
         static void name();                               \
      } H2Q(Q);                                            \
   }                                                       \
   void Q::name()

#define H2GlobalSetup() __H2GlobalCallback(global_setup, H2Q(Global_Setup))
#define H2GlobalTeardown() __H2GlobalCallback(global_teardown, H2Q(Global_Teardown))

#define H2GlobalSuiteSetup() __H2GlobalCallback(global_suite_setup, H2Q(Global_Suite_Setup))
#define H2GlobalSuiteTeardown() __H2GlobalCallback(global_suite_teardown, H2Q(Global_Suite_Teardown))

#define H2GlobalCaseSetup() __H2GlobalCallback(global_case_setup, H2Q(Global_Case_Setup))
#define H2GlobalCaseTeardown() __H2GlobalCallback(global_case_teardown, H2Q(Global_Case_Teardown))

#define __H2SUITE(name, QP)                                       \
   static void QP(h2::h2_suite*, h2::h2_case*);                   \
   static h2::h2_suite H2Q(suite)(name, &QP, __FILE__, __LINE__); \
   static void QP(h2::h2_suite* ________suite, h2::h2_case* _________case)

#define H2SUITE(name) __H2SUITE(name, H2Q(h2_suite_test_code))

#define __H2Cleanup()                      \
   if (::setjmp(________suite->jump) == 0) \
      ________suite->jumpable = true;      \
   if (!_________case)

#define H2Cleanup() __H2Cleanup()

#define __H2Case(name, todo, Qc, Q1, Q2)                                         \
   static h2::h2_case Qc(name, todo, __FILE__, __LINE__);                        \
   static h2::h2_suite::installer H2Q(installer)(________suite, &Qc);            \
   if (&Qc == _________case)                                                     \
      for (h2::h2_suite::cleaner Q1(________suite); Q1; _________case = nullptr) \
         for (h2::h2_case::cleaner Q2(&Qc); Q2;)                                 \
            if (::setjmp(Qc.jump) == 0)

#define H2Case(name) __H2Case(name, 0, H2Q(t_case), H2Q(_1), H2Q(_2))
#define H2Todo(name) __H2Case(name, 1, H2Q(t_case), H2Q(_1), H2Q(_2))

#define __H2CASE(name, todo, QR, QP)                                       \
   static void QR();                                                       \
   static void QP(h2::h2_suite* ________suite, h2::h2_case* _________case) \
   {                                                                       \
      static h2::h2_case c(name, todo, __FILE__, __LINE__);                \
      static h2::h2_suite::installer i(________suite, &c);                 \
      if (&c == _________case)                                             \
         for (h2::h2_case::cleaner a(&c); a;)                              \
            if (::setjmp(c.jump) == 0)                                     \
               QR();                                                       \
   }                                                                       \
   static h2::h2_suite H2Q(suite)("Anonymous", &QP, __FILE__, __LINE__);   \
   static void QR()

#define H2CASE(name) __H2CASE(name, 0, H2Q(h2_case_test_code), H2Q(h2_suite_test_code))
#define H2TODO(name) __H2CASE(name, 1, H2Q(h2_case_test_code), H2Q(h2_suite_test_code))

#define __H2BLOCK0(Qb) for (h2::h2_heap::stack::block Qb(__FILE__, __LINE__); Qb;)
#define __H2BLOCK1(Qb, ...) for (h2::h2_heap::stack::block Qb(__FILE__, __LINE__, __VA_ARGS__); Qb;)
#define H2BLOCK(...) H2PP_IF_ELSE(H2PP_IS_EMPTY(__VA_ARGS__), __H2BLOCK0(H2Q(t_block)), __H2BLOCK1(H2Q(t_block), __VA_ARGS__))

#define H2DNS(...) h2::h2_dns::setaddrinfo(H2PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define __H2SOCK0() h2::h2_socket::start_and_fetch()
#define __H2SOCK2(packet, size) h2::h2_socket::inject_received(packet, size, nullptr, "*")
#define __H2SOCK3(packet, size, from) h2::h2_socket::inject_received(packet, size, from, "*")
#define __H2SOCK4(packet, size, from, to) h2::h2_socket::inject_received(packet, size, from, to)
#define H2SOCK(...) H2PP_VARIADIC_CALL(__H2SOCK, __VA_ARGS__)

#define H2COUT(...) h2::h2_stdio::capture_cout(__VA_ARGS__)

#endif

#include <cassert>  /* assert */
#include <cctype>   /* tolower, isspace */
#include <cstdarg>  /* va_list */
#include <cstdint>  /* int32_t */
#include <errno.h>  /* strerror */
#include <iostream> /* cout */
#include <map>      /* std::map */
#include <memory>   /* allocator */
#include <regex>    /* std::regex */
#include <signal.h> /* sigaction */
#include <typeinfo> /* typeid */

#if defined _WIN32
#   include <winsock2.h> /* socket */
#   include <ws2tcpip.h> /* getaddrinfo */
#   include <io.h>       /* _wirte */
#   include <shlwapi.h>  /* PathRemoveFileSpecA, StrStrIA */
#   define fileno _fileno
#   define socklen_t int
#   define strcasestr StrStrIA
#   pragma comment(lib, "Ws2_32.lib")
#   pragma comment(lib, "Shlwapi.lib")
#else
#   include <arpa/inet.h>   /* inet_addr, inet_pton */
#   include <cxxabi.h>      /* demangle */
#   include <execinfo.h>    /* backtrace */
#   include <fcntl.h>       /* fcntl */
#   include <fnmatch.h>     /* fnmatch */
#   include <libgen.h>      /* basename */
#   include <netdb.h>       /* getaddrinfo, gethostbyname */
#   include <sys/ioctl.h>   /* ioctl */
#   include <sys/mman.h>    /* mprotect, mmap */
#   include <sys/socket.h>  /* sockaddr */
#   include <sys/syscall.h> /* syscall */
#   include <sys/types.h>   /* size_t */
#   include <syslog.h>      /* syslog, vsyslog */
#   include <time.h>        /* clock */
#   include <unistd.h>      /* sysconf */
#   if defined __GLIBC__
#      include <malloc.h> /* __malloc_hook */
#   elif defined __APPLE__
#      include <AvailabilityMacros.h>
#      include <malloc/malloc.h> /* malloc_zone_t */
#   endif
#endif

namespace h2 {

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

static inline bool h2_in(const char* x, const char* s[], int n = 0)
{
   n = n ? n : 1000;
   for (int i = 0; s[i] && i < n; ++i)
      if (!strcmp(s[i], x)) return true;
   return false;
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

struct h2__color {
   h2_singleton(h2__color);
   h2__color() { memset(current, 0, sizeof(current)); }

   char current[8][32];

   void clear_style()
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         current[i][0] = '\0';
   }

   void push_style(const char* style)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] == '\0') {
            strcpy(current[i], style);
            break;
         }
   }

   void pop_style(const char* style)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (!strcmp(current[i], style))
            current[i][0] = '\0';
   }

   void change()
   {
#ifdef _WIN32
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

   char* parse(const char* style)
   {
      char* end = strchr((char*)style, '}');
      assert(end);
      char t[128];
      strncpy(t, style, sizeof(t));
      *strchr(t, '}') = '\0';
      for (char* p = strtok(t + 2, ","); p; p = strtok(NULL, ",")) {
         if (p[0] == '-')
            pop_style(p + 1);
         else if (p[0] == '+')
            push_style(p + 1);
         else
            push_style(p);

         if (!strcmp("reset", p)) clear_style();
      }
      return end;
   }

   void print(const char* str)
   {
      for (const char* p = str; *p; p++) {
         if (h2_color::is_ctrl(p)) {
            p = I().parse(p);
            if (h2_option::I().colorable) I().change();
         } else {
            h2_libc::write(fileno(stdout), p, 1);
         }
      }
   }

   static int style2value(const char* style)
   {
      struct st {
         const char* name;
         int value;
      };

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
#endif

      for (int i = 0; i < sizeof(K) / sizeof(K[0]); ++i)
         if (!strcmp(K[i].name, style))
            return K[i].value;

      return K[0].value;  //reset
   }
};

h2_inline void h2_color::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) {
      char t[128];
      sprintf(t, "\033{%s}", style);
      h2__color::I().print(t);
   }

   char* alloca_str;
   h2_sprintf(alloca_str, format);
   h2__color::I().print(alloca_str);

   if (style && strlen(style))
      h2__color::I().print("\033{reset}");
}

h2_inline void h2_color::printf(h2_line& line)
{
   for (auto& word : line)
      h2__color::I().print(word.c_str());
   h2__color::I().print("\n");
}

h2_inline void h2_color::printf(h2_lines& lines)
{
   for (auto& line : lines) printf(line);
}

h2_inline void h2_list::__add_between(h2_list* _new, h2_list* prev, h2_list* next)
{
   next->prev = _new;
   _new->next = next;
   _new->prev = prev;
   prev->next = _new;
}

h2_inline h2_list* h2_list::out()
{
   prev->next = next;
   next->prev = prev;
   next = prev = this;
   return this;
}

h2_inline int h2_list::count() const
{
   int c = 0;
   for (auto p = next; p != this; p = p->next) ++c;
   return c;
}

h2_inline bool h2_string::equals(h2_string str, bool caseless) const
{
   if (!caseless) return *this == str;
   return tolower(c_str()) == tolower(str);
}

h2_inline bool h2_string::contains(h2_string substr, bool caseless) const
{
   if (!caseless) return find(substr) != h2_string::npos;
   return tolower(c_str()).find(tolower(substr)) != h2_string::npos;
}

h2_inline bool h2_string::startswith(h2_string prefix, bool caseless) const
{
   if (size() < prefix.size()) return false;
   if (!caseless) return find(prefix) == 0;
   return tolower(c_str()).find(tolower(prefix)) == 0;
}

h2_inline bool h2_string::endswith(h2_string suffix, bool caseless) const
{
   if (size() < suffix.size()) return false;
   if (!caseless) return rfind(suffix) == length() - suffix.length();
   return tolower(c_str()).rfind(tolower(suffix)) == length() - suffix.length();
}

h2_inline bool h2_string::isspace() const
{
   for (auto& c : *this)
      if (!::isspace(c)) return false;
   return true;
}

h2_inline h2_string& h2_string::tolower()
{
   for (auto& c : *this) c = ::tolower(c);
   return *this;
}

static inline void escape_char(const char c, h2_string& t)
{
   char d = c;
   switch (c) {
   case '\n': d = 'n'; break;
   case '\r': d = 'r'; break;
   case '\t': d = 't'; break;
   }
   if (d != c) t.push_back('\\');
   t.push_back(d);
}

h2_inline h2_string h2_string::acronym(int width, int tail) const
{
   h2_string t;
   if (size() <= width) {
      for (int i = 0; i < size(); ++i) escape_char(c_str()[i], t);
   } else {
      for (int i = 0; i < width - 3 - tail; ++i) escape_char(c_str()[i], t);
      t.append("\033{+dark gray}...\033{-dark gray}");
      for (int i = 0; i < tail; ++i) escape_char(c_str()[size() - tail + i], t);
   }
   return t;
}

h2_inline h2_string& h2_string::center(int width)
{
   int left = (width - length()) / 2, right = width - left - length();
   insert(0, left, ' ');
   append(right, ' ');
   return *this;
}

h2_inline h2_string& h2_string::sprintf(const char* format, ...)
{
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   append(alloca_str);
   return *this;
}

h2_inline int h2_line::length() const
{
   int length = 0;
   for (auto& word : *this)
      if (!h2_color::is_ctrl(word.c_str()))
         length += word.size();
   return length;
}

h2_inline void h2_line::indent(int n_space)
{
   insert(begin(), h2_string(n_space, ' '));
}

h2_inline void h2_line::padding(int n_space)
{
   push_back(h2_string(n_space, ' '));
}

h2_inline void h2_line::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
}

h2_inline void h2_line::concat_back(const char* style, h2_line& line)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   insert(end(), line.begin(), line.end());
   if (style && strlen(style)) push_back("\033{reset}");
}

h2_inline void h2_line::concat_front(const char* style, h2_line& line)
{
   if (style && strlen(style)) insert(begin(), "\033{" + h2_string(style) + "}");
   insert(begin(), line.begin(), line.end());
   if (style && strlen(style)) insert(begin(), "\033{reset}");
}

h2_inline void h2_line::fold(h2_vector<h2_line>& lines)
{
   for (size_t i = 0; i < lines.size(); i++)
      for (auto& word : lines[i])
         if (!word.isspace())  // drop indent
            push_back(word);
}

h2_inline void h2_line::samesizify(h2_line& b)
{
   int len = length(), b_len = b.length();
   padding(std::max(len, b_len) - len);
   b.padding(std::max(len, b_len) - b_len);
}

h2_inline void h2_lines::concat_back(h2_lines& lines)
{
   insert(end(), lines.begin(), lines.end());
}
h2_inline void h2_lines::concat_front(h2_lines& lines)
{
   insert(begin(), lines.begin(), lines.end());
}

h2_inline int h2_lines::max_length() const
{
   int max_length = 0;
   for (size_t i = 0; i < size(); ++i)
      max_length = std::max(max_length, at(i).length());
   return max_length;
}

h2_inline void h2_lines::samesizify(h2_lines& b)
{
   int max_y = std::max(size(), b.size());
   for (size_t i = size(); i < max_y; ++i) push_back(h2_line());
   for (size_t i = b.size(); i < max_y; ++i) b.push_back(h2_line());
}

h2_inline bool h2_lines::foldable()
{
   int sum = 0;
   for (size_t i = 0; i < size(); ++i)
      for (auto& word : at(i))
         if (!word.isspace())  // ignore indent
            sum += word.length();

   return sum < 20;
}

h2_inline void h2_lines::sequence(int indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      h2_line sequence;
      sequence.printf("dark gray", "%lu. ", i + start);
      at(i).concat_front("", sequence);
      at(i).indent(indent);
   }
}
// TINYEXPR - Tiny recursive descent parser and evaluation engine in C
//
// Copyright (c) 2015-2018 Lewis Van Winkle
//
// http://CodePlea.com

/* clang-format off */

#ifndef NAN
#define NAN (0.0/0.0)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif

#define TYPE_MASK(TYPE) ((TYPE)&0x0000001F)

#define IS_PURE(TYPE) (((TYPE) & TE_FLAG_PURE) != 0)
#define IS_FUNCTION(TYPE) (((TYPE) & TE_FUNCTION0) != 0)
#define IS_CLOSURE(TYPE) (((TYPE) & TE_CLOSURE0) != 0)
#define ARITY(TYPE) ( ((TYPE) & (TE_FUNCTION0 | TE_CLOSURE0)) ? ((TYPE) & 0x00000007) : 0 )

struct te
{
    /* Parses the input expression, evaluates it, and frees it. */
    /* Returns NaN on error. */
    static double te_interp(const char *expression, int *error) {
        te_expr *n = te_compile(expression, 0, 0, error);
        double ret;
        if (n) {
            ret = te_eval(n);
            te_free(n);
        } else {
            ret = NAN;
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

    enum {TE_CONSTANT = 1};

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

    static double _fabs(double x) {return fabs(x);}
    static double _cos(double x) {return cos(x);}
    static double _acos(double x) {return acos(x);}
    static double _sin(double x) {return sin(x);}
    static double _asin(double x) {return asin(x);}
    static double _tan(double x) {return tan(x);}
    static double _atan(double x) {return atan(x);}
    static double _sqrt(double x) {return sqrt(x);}
    static double _ln(double x) {return log(x);}
    static double _log10(double x) {return log10(x);}
    static double _log2(double x) {return log2(x);}
    static double _floor(double x) {return floor(x);}
    static double _ceil(double x) {return ceil(x);}
    static double _pow(double x, double y) {return pow(x, y);}
    static double _exp(double x) {return exp(x);}
    static double _fmod(double x, double y) {return fmod(x, y);}

    static double pi(void) {return 3.14159265358979323846;}
    static double e(void) {return 2.71828182845904523536;}
    static double fac(double a) {/* simplest version of fac */
        if (a < 0.0) return NAN;
        if (a > UINT_MAX) return INFINITY;
        unsigned int ua = (unsigned int)(a);
        unsigned long int result = 1, i;
        for (i = 1; i <= ua; i++) {
            if (i > ULONG_MAX / result) return INFINITY;
            result *= i;
        }
        return (double)result;
    }
    static double ncr(double n, double r) {
        if (n < 0.0 || r < 0.0 || n < r) return NAN;
        if (n > UINT_MAX || r > UINT_MAX) return INFINITY;
        unsigned long int un = (unsigned int)(n), ur = (unsigned int)(r), i;
        unsigned long int result = 1;
        if (ur > un / 2) ur = un - ur;
        for (i = 1; i <= ur; i++) {
            if (result > ULONG_MAX / (un - ur + i)) return INFINITY;
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
        //     {"atan2", (const void *)atan2,  TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"ceil", (const void *)_ceil,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"cos", (const void *)_cos,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        //     {"cosh", (const void *)cosh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"e", (const void *)e,          TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"exp", (const void *)_exp,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"fac", (const void *)fac,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"floor", (const void *)_floor,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ln", (const void *)_ln,       TE_FUNCTION1 | TE_FLAG_PURE, 0},
        // #ifdef TE_NAT_LOG
        //     {"log", (const void *)log,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        // #else
        //     {"log", (const void *)log10,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
        // #endif
            {"log10", (const void *)_log10,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log2", (const void *)_log2,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ncr", (const void *)ncr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"npr", (const void *)npr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"pi", (const void *)pi,        TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"pow", (const void *)_pow,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"sin", (const void *)_sin,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        //     {"sinh", (const void *)sinh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"sqrt", (const void *)_sqrt,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"tan", (const void *)_tan,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        //     {"tanh", (const void *)tanh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
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
                ret->value = NAN;
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


#define TE_FUN(...) ((double(*)(__VA_ARGS__))n->function)
#define M(e) te_eval((const te_expr *)n->parameters[e])


    static double te_eval(const te_expr *n) {
        if (!n) return NAN;

        switch(TYPE_MASK(n->type)) {
            case TE_CONSTANT: return n->value;
            case TE_VARIABLE: return *n->bound;

            case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
            case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
                switch(ARITY(n->type)) {
                    case 0: return TE_FUN(void)();
                    case 1: return TE_FUN(double)(M(0));
                    case 2: return TE_FUN(double, double)(M(0), M(1));
                    case 3: return TE_FUN(double, double, double)(M(0), M(1), M(2));
                    case 4: return TE_FUN(double, double, double, double)(M(0), M(1), M(2), M(3));
                    case 5: return TE_FUN(double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4));
                    case 6: return TE_FUN(double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5));
                    case 7: return TE_FUN(double, double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                    default: return NAN;
                }

            case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
            case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
                switch(ARITY(n->type)) {
                    case 0: return TE_FUN(void*)(n->parameters[0]);
                    case 1: return TE_FUN(void*, double)(n->parameters[1], M(0));
                    case 2: return TE_FUN(void*, double, double)(n->parameters[2], M(0), M(1));
                    case 3: return TE_FUN(void*, double, double, double)(n->parameters[3], M(0), M(1), M(2));
                    case 4: return TE_FUN(void*, double, double, double, double)(n->parameters[4], M(0), M(1), M(2), M(3));
                    case 5: return TE_FUN(void*, double, double, double, double, double)(n->parameters[5], M(0), M(1), M(2), M(3), M(4));
                    case 6: return TE_FUN(void*, double, double, double, double, double, double)(n->parameters[6], M(0), M(1), M(2), M(3), M(4), M(5));
                    case 7: return TE_FUN(void*, double, double, double, double, double, double, double)(n->parameters[7], M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                    default: return NAN;
                }

            default: return NAN;
        }

    }

#undef TE_FUN
#undef M

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

/* clang-format on */

h2_inline double tinyexpr::eval(const char* expression, int* error) {
   return te::te_interp(expression, error);
}

#ifndef _WIN32

struct h2_nm {
   h2_singleton(h2_nm);

   std::map<std::string, unsigned long long> symbols;
   unsigned long long main_addr;

   unsigned long long get(const char* name) const
   {
      if (strlen(name) == 0) return 0;
      auto it = symbols.find(name);
      return it != symbols.end() ? it->second : ULLONG_MAX;
   }

   bool in_main(unsigned long long addr) const { return main_addr == ULLONG_MAX ? false : main_addr < addr && addr < main_addr + 256; }

   h2_nm()
   {
      nm_parse();
      main_addr = get("main");
   }

   void nm_parse()
   {
      char nm[256], line[1024], addr[128], type[32], name[1024];
      sprintf(nm, "nm %s", O.path);
      h2_with f(::popen(nm, "r"), ::pclose);
      if (f.f)
         while (::fgets(line, sizeof(line) - 1, f.f))
            if (3 == sscanf(line, "%s%s%s", addr, type, name))
               if (type[0] == 't' || type[0] == 'T' || type[0] == 'w' || type[0] == 'W')
                  symbols.insert(std::make_pair(name + O.isMAC(), (unsigned long long)strtoull(addr, nullptr, 16)));
   }
};

static inline bool demangle(const char* mangled, char* demangled, size_t len)
{
   int status = 0;
   abi::__cxa_demangle(mangled, demangled, &len, &status);
   return status == 0;
}

static inline bool addr2line(unsigned long long addr, char* output, size_t len)
{
   char t[256];
#   if defined __APPLE__
   sprintf(t, "atos -o %s 0x%llx", O.path, addr);
#   else
   sprintf(t, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#   endif
   h2_with f(::popen(t, "r"), ::pclose);
   if (!f.f || !::fgets(output, len, f.f)) return false;
   for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
   return true;
}

static inline bool backtrace_extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long long* offset)
{
   //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
   if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %llu", module, mangled, offset))
      return true;

   //Linux: with '-rdynamic' linker option
   //Linux: module_name(mangled_function_name+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
   if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%llx", module, mangled, offset))
      return true;

   mangled[0] = '\0';

   //Linux: Ubuntu without '-rdynamic' linker option
   //Linux: module_name(+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%llx", module, offset))
      return true;

   //Linux: Redhat/CentOS without '-rdynamic' linker option
   //Linux: module_name()[relative_offset_to_module]
   //Linux: `./a.out() [0x40b960]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^[][0x%llx", module, offset))
      return true;

   //Where?
   //Linux: module_name[relative_offset_to_module]
   //Linux: `./a.out[0x4060e7]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%llx", module, offset))
      return true;

   return false;
}
#endif

h2_inline h2_backtrace::h2_backtrace(int shift_) : shift(shift_)
{
#ifndef _WIN32
   h2_heap::unhook();
   count = ::backtrace(array, sizeof(array) / sizeof(array[0]));
   h2_heap::dohook();
#endif
}

h2_inline bool h2_backtrace::operator==(h2_backtrace& bt)
{
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (array[i] != bt.array[i])
         return false;
   return true;
}

h2_inline bool h2_backtrace::has(void* func, int size) const
{
   for (int i = 0; i < count; ++i)
      if (func <= array[i] && (unsigned char*)array[i] < ((unsigned char*)func) + size)
         return true;
   return false;
}

h2_inline void h2_backtrace::print(h2_vector<h2_string>& stacks) const
{
#ifndef _WIN32
   h2_heap::unhook();
   char** backtraces = backtrace_symbols(array, count);
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
         address = h2_nm::I().get(mangled);
         if (address != ULLONG_MAX)
            if (addr2line(address + offset, addr2lined, sizeof(addr2lined)))
               if (strlen(addr2lined))
                  p = addr2lined;
      }
      stacks.push_back(p);

      if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_nm::I().in_main(address + offset))
         break;
   }
   free(backtraces);
   h2_heap::dohook();
#endif
}

h2_inline void h2_backtrace::print(int pad) const
{
   h2_vector<h2_string> stacks;
   print(stacks);
   h2_lines lines;
   for (auto& c : stacks)
      if (O.verbose || c.find("h2unit.h:") == h2_string::npos && c.find("h2unit.hpp:") == h2_string::npos && c.find("h2unit.cpp:") == h2_string::npos)
         lines.push_back(h2_line(c));
   lines.sequence(pad);
   h2_color::printf(lines);
}

h2_inline h2_fail* h2_callexp::check(const char* func, const char* file, int line)
{
   if (is_satisfied() || is_saturated()) return nullptr;
   return new h2_fail_call(func, expect(), actual(), file, line);
}

h2_inline const char* h2_callexp::actual()
{
   static char st[64];
   if (call > 0)
      sprintf(st, "%d times", call);
   else
      sprintf(st, "never");
   return st;
}

h2_inline const char* h2_callexp::expect()
{
   static char st[128];
   if (least == 0) {
      if (most == 0)
         sprintf(st, "never called");
      else if (most == INT_MAX)
         sprintf(st, "any number of times");
      else
         sprintf(st, "at most %d times", most);
   } else if (least == most)
      sprintf(st, "exactly %d times", least);
   else if (most == INT_MAX)
      sprintf(st, "at least %d times", least);
   else  // 0 < least < most < INT_MAX
      sprintf(st, "between %d and %d times", least, most);

   return st;
}

h2_inline h2_case::h2_case(const char* name_, int todo, const char* file_, int line_)
  : name(name_), file(file_), line(line_), status(todo ? TODOED : INITED), fails(nullptr), sock(nullptr) {}

h2_inline void h2_case::prev_setup()
{
   status = PASSED;
   h2_heap::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup()
{
   h2_stdio::capture_cout(nullptr);
   if (sock) delete sock;
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear();
   h2_fail::append_subling(fail, h2_heap::stack::pop());

   if (!fail) return;
   if (status != FAILED)
      h2_fail::append_subling(fails, fail);
   else
      delete fail;
   status = FAILED;
}

h2_inline void h2_case::do_fail(h2_fail* fail)
{
   status = FAILED;
   h2_fail::append_subling(fails, fail);
   ::longjmp(jump, 1);
}

#if defined __linux__
#   if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#      define h2_raise_trap() asm volatile("int $3")
#   else
#      define h2_raise_trap() raise(SIGTRAP)
#   endif
#elif defined __APPLE__
#   define h2_raise_trap() __asm__("int $3\n" \
                                   :          \
                                   :)
#endif

#if defined __linux__
static inline bool under_debug(int, const char*)
{
   char t[1024];
   h2_with f(::fopen("/proc/self/status", "r"));
   if (f.f)
      while (::fgets(t, sizeof(t) - 1, f.f))
         if (strncmp(t, "TracerPid:\t", 11) == 0)
            return t[11] != '\0' && t[11] != '0';
   return false;
}
#elif defined __APPLE__
static inline bool under_debug(int pid, const char* path)
{
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", pid);
   h2_with f(::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r"), ::pclose);
   if (f.f)
      while (::fgets(t, sizeof(t) - 1, f.f))
         if (strstr(t, basename((char*)path)) || strstr(t, attach_pid))
            return true;
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
#ifndef _WIN32
   int pid = (int)getpid();
   if (!under_debug(pid, O.path)) {
      static h2_once only_one_time;
      if (only_one_time) {
         if (!strcmp("gdb attach", O.debug)) {
            if (fork() == 0) {
               system(get_gdb2((char*)alloca(256), pid));
               exit(0);
            } else
               while (!under_debug(pid, O.path))
                  h2_sleep(100);  // wait for password
         } else {
            system(get_gdb1((char*)alloca(256)));
            exit(0);
         }
      }
   }

   h2_raise_trap();
#endif
}

static inline h2_lines line_break(h2_line& line, int width)
{
   h2_lines lines;
   h2_string current_style;
   h2_line wrap;
   int length = 0;

   for (auto& word : line) {
      if (h2_color::is_ctrl(word.c_str())) {  // + - style , issue
         wrap.push_back(word);
         current_style = word;
      } else {
         for (auto& c : word) {
            if (width <= length) {  // terminate line as later as possible
               lines.push_back(wrap);
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
   lines.push_back(wrap);
   return lines;
}

static inline h2_lines lines_merge(h2_lines& left_lines, h2_lines& right_lines, int width)
{
   left_lines.samesizify(right_lines);
   h2_lines lines;
   for (size_t i = 0; i < std::max(left_lines.size(), right_lines.size()); ++i) {
      left_lines[i].samesizify(right_lines[i]);
      auto left_wrap_lines = line_break(left_lines[i], width - 2);
      auto right_wrap_lines = line_break(right_lines[i], width - 2);
      for (size_t j = 0; j < std::max(left_wrap_lines.size(), right_wrap_lines.size()); ++j) {
         h2_line line;
         line.push_back("\033{reset}");
         line.concat_back("", left_wrap_lines[j]);
         line.push_back("\033{reset}");
         line.printf("dark gray", j == left_wrap_lines.size() - 1 ? "  │ " : " \\│ ");
         line.concat_back("", right_wrap_lines[j]);
         line.push_back("\033{reset}");
         line.printf("dark gray", j == right_wrap_lines.size() - 1 ? "  " : " \\");

         lines.push_back(line);
      }
   }
   return lines;
}

h2_inline h2_lines h2_layout::split(h2_lines& left_lines, h2_lines& right_lines, const char* left_title, const char* right_title)
{
   int max_line_width = std::max(left_lines.max_length(), right_lines.max_length());
   int half_width = std::min(h2_term_size() / 2 - 4, std::max(max_line_width + 3, 30));

   h2_line left_title_line = {"\033{dark gray}", h2_string(left_title).center(half_width - 3), "\033{reset}"};
   h2_line right_title_line = {"\033{dark gray}", h2_string(right_title).center(half_width - 3), "\033{reset}"};

   left_lines.insert(left_lines.begin(), left_title_line);
   right_lines.insert(right_lines.begin(), right_title_line);

   h2_lines lines = lines_merge(left_lines, right_lines, half_width);
   return lines;
}

h2_inline h2_lines h2_layout::unified(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title)
{
   h2_lines lines;

   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines up_lines = line_break(up_line, h2_term_size() - 12);
   h2_lines down_lines = line_break(down_line, h2_term_size() - 12);

   for (size_t i = 0; i < std::max(up_lines.size(), down_lines.size()); ++i) {
      if (i < up_lines.size()) {
         up_lines[i].concat_front("", up_title_line);
         lines.push_back(up_lines[i]);
      }
      if (i < down_lines.size()) {
         down_lines[i].concat_front("", down_title_line);
         lines.push_back(down_lines[i]);
      }
   }

   return lines;
}

h2_inline h2_lines h2_layout::seperate(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title)
{
   h2_lines lines;

   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines up_lines = line_break(up_line, h2_term_size() - 12);
   h2_lines down_lines = line_break(down_line, h2_term_size() - 12);

   for (size_t i = 0; i < up_lines.size(); ++i) {
      if (i == 0)
         up_lines[i].concat_front("", up_title_line);
      else
         up_lines[i].indent(12);
      lines.push_back(up_lines[i]);
   }
   for (size_t i = 0; i < down_lines.size(); ++i) {
      if (i == 0)
         down_lines[i].concat_front("", down_title_line);
      else
         down_lines[i].indent(12);
      lines.push_back(down_lines[i]);
   }
   return lines;
}

static constexpr const char* last_order_file_path = ".last_order";

h2_inline void h2_directory::drop_last_order() { ::remove(last_order_file_path); }

static inline void save_last_order()
{
   h2_with f(fopen(last_order_file_path, "w"));
   if (f.f)
      h2_list_for_each_entry(s, &h2_directory::I().sorted_suites, h2_suite, sorted)
        h2_list_for_each_entry(c, &s->sorted_cases, h2_case, sorted)
          fprintf(f.f, "%s\n%s\n", s->name, c->name);
}

static inline bool mark_sequence(char* suitename, char* casename)
{
   static long long seq = INT_MIN;

   h2_list_for_each_entry(s, &h2_directory::I().registered_suites, h2_suite, registered)
   {
      if (strcmp(suitename, s->name) == 0) {
         h2_list_for_each_entry(c, &s->registered_cases, h2_case, registered)
         {
            if (strcmp(casename, c->name) == 0) {
               s->seq = c->seq = ++seq;
               return true;
            }
         }
      }
   }
   return false;
}

static inline int read_last_order()
{
   int count = 0;
   char suitename[512], casename[512];
   h2_with f(fopen(last_order_file_path, "r"));
   if (f.f)
      while (1 == fscanf(f.f, "%[^\n]\n", suitename) && 1 == fscanf(f.f, "%[^\n]\n", casename))
         if (mark_sequence(suitename, casename))
            count += 1;
   return count;
}

h2_inline void insert_suite_sort(h2_list* suite_list, h2_suite* s)
{
   h2_list_for_each_entry(p, suite_list, h2_suite, sorted)
   {
      if (s->seq < p->seq) {
         p->sorted.add_before(&s->sorted);
         return;
      }
   }
   suite_list->add_tail(&s->sorted);
}

h2_inline void insert_case_sort(h2_list* case_list, h2_case* c)
{
   h2_list_for_each_entry(p, case_list, h2_case, sorted)
   {
      if (c->seq < p->seq) {
         p->sorted.add_before(&c->sorted);
         return;
      }
   }
   case_list->add_tail(&c->sorted);
}

h2_inline int h2_directory::sort()
{
   int count = 0;
   int last = read_last_order();
   srand(clock());
   if (O.randomize && last == 0)
      h2_list_for_each_entry(s, &h2_directory::I().registered_suites, h2_suite, registered)
        h2_list_for_each_entry(c, &s->registered_cases, h2_case, registered)
          s->seq = c->seq = rand();

   h2_list_for_each_entry(s, &h2_directory::I().registered_suites, h2_suite, registered)
   {
      h2_list_for_each_entry(c, &s->registered_cases, h2_case, registered)
      {
         insert_case_sort(&s->sorted_cases, c);
         count += 1;
      }
      insert_suite_sort(&h2_directory::I().sorted_suites, s);
   }

   if (O.randomize && last == 0)
      save_last_order();
   
   return count;
}

struct h2_resolver {
   h2_singleton(h2_resolver);
   h2_list dnses;

   static bool inet_addr(const char* str, struct sockaddr_in* addr)
   {
      struct sockaddr_in a;
      if (!addr) addr = &a;
      return 1 == inet_pton(AF_INET, str, &addr->sin_addr);
   }

   h2_dns* find(const char* hostname)
   {
      h2_list_for_each_entry(p, &dnses, h2_dns, y) if (!strcmp("*", p->name) || !strcmp(hostname, p->name)) return p;
      return nullptr;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res)
   {
      h2_dns* dns = I().find(hostname);
      if (!dns) return -1;

      static struct addrinfo addrinfos[32];
      static struct sockaddr_in sockaddrs[32];
      memset(addrinfos, 0, sizeof(addrinfos));
      memset(sockaddrs, 0, sizeof(sockaddrs));

      struct addrinfo** pp = res;
      for (int i = 0; i < dns->count; ++i) {
         struct addrinfo* a = &addrinfos[i];
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(dns->array[i], b)) {
            a->ai_addr = (struct sockaddr*)b;
            a->ai_addrlen = sizeof(struct sockaddr_in);
         } else
            a->ai_canonname = dns->array[i];
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

   static struct hostent* gethostbyname(char* name)
   {
      h2_dns* dns = I().find(name);
      if (!dns) return nullptr;

      static struct sockaddr_in sockaddrs[32];
      static char* h_aliases[32];
      static char* h_addr_list[32];
      static struct hostent h;

      h.h_name = name;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      memset(sockaddrs, 0, sizeof(sockaddrs));
      memset(h_aliases, 0, sizeof(h_aliases));
      memset(h_addr_list, 0, sizeof(h_addr_list));

      for (int i = 0, a = 0, c = 0; i < dns->count; ++i) {
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(dns->array[i], b))
            h_addr_list[a++] = (char*)&b->sin_addr;
         else
            h_aliases[c++] = dns->array[i];
      }
      return &h;
   }

   h2_stubs stubs;
   h2_resolver()
   {
      stubs.add((void*)::getaddrinfo, (void*)getaddrinfo);
      stubs.add((void*)::freeaddrinfo, (void*)freeaddrinfo);
      stubs.add((void*)::gethostbyname, (void*)gethostbyname);
   }
   ~h2_resolver() { stubs.clear(); }
};

h2_inline void h2_dnses::add(h2_dns* dns) { dnses.push(&dns->x); }
h2_inline void h2_dnses::clear()
{
   h2_list_for_each_entry(p, &dnses, h2_dns, x)
   {
      p->x.out(), p->y.out();
      delete p;
   }
}

h2_inline void h2_dns::setaddrinfo(int n, ...)
{
   if (n == 0) return;
   const char* array[32];
   int count = 0;
   va_list a;
   va_start(a, n);
   for (int i = 0; i < n && i < 32; ++i)
      array[count++] = va_arg(a, const char*);
   va_end(a);

   const char* hostname = "*";
   for (int i = 0; i < count; ++i)
      if (!h2_resolver::inet_addr(array[i], nullptr))
         if (strlen(hostname) < 2 || strlen(array[i]) < strlen(hostname))
            hostname = array[i];

   h2_dns* dns = new h2_dns(hostname);
   for (int i = 0; i < count; ++i)
      if (strcmp(hostname, array[i]))
         strcpy(dns->array[dns->count++], array[i]);

   h2_resolver::I().dnses.push(&dns->y);
   if (h2_task::I().current_case) h2_task::I().current_case->dnses.add(dns);
}

h2_inline void h2_dns::initialize()
{
   setaddrinfo(1, "127.0.0.1");
}

h2_inline void h2_fail::append_subling(h2_fail*& fail, h2_fail* n)
{
   if (!fail)
      fail = n;
   else {
      h2_fail** pp = &fail->subling_next;
      while (*pp) pp = &(*pp)->subling_next;
      *pp = n;
   }
}

h2_inline void h2_fail::append_child(h2_fail*& fail, h2_fail* n)
{
   if (!fail)
      fail = n;
   else {
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

h2_inline void h2_fail::set_locate(const char* file_, int line_, const char* func_, int argi_)
{
   file = file_, line = line_, func = func_, argi = argi_;
}

h2_inline const char* h2_fail::get_locate()
{
   static constexpr const char* a9 = "1st\0002nd\0003rd\0004th\0005th\0006th\0007th\0008th\0009th";
   static char st[1024];
   char* p = st;
   strcpy(p, "");
   if (func && strlen(func)) p += sprintf(p, ", in %s(%s)", func, 0 <= argi && argi < 9 ? a9 + argi * 4 : "");
   if (file && strlen(file) && 0 < line) p += sprintf(p, ", at %s:%d", file, line);
   return st;
}

h2_inline void h2_fail::foreach (std::function<void(h2_fail*, int, int)> cb, int subling_index, int child_index)
{
   cb(this, subling_index, child_index);
   if (child_next) child_next->foreach (cb, 0, child_index + 1);
   if (subling_next) subling_next->foreach (cb, subling_index + 1, child_index);
}

h2_inline void h2_fail_unexpect::print_OK1(h2_line& l)
{
   l.printf("", "OK(%s) is ", a_expression.c_str());
   l.printf("bold,red", "false");
}

static inline bool is_synonym(const char* a, const char* b)
{
   static const char* s_null[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0", nullptr};
   static const char* s_true[] = {"IsTrue", "true", "1", nullptr};
   static const char* s_false[] = {"IsFalse", "false", "0", nullptr};
   static const char** S[] = {s_null, s_true, s_false};

   for (int i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
      if (h2_in(a, S[i]) && h2_in(b, S[i]))
         return true;

   return false;
}

h2_inline void h2_fail_unexpect::print_OK2(h2_line& l)
{
   l.push_back("OK(");
   if (e_expression == e_explain)
      l.printf("green", e_explain.acronym(30, 1).c_str());
   else {
      l.push_back(e_expression.acronym());
      if (e_explain.size() && !is_synonym(e_expression.c_str(), e_explain.c_str())) {
         l.printf("dark gray", "==>");
         l.printf("green", e_explain.acronym(30, 1).c_str());
      }
   }

   l.push_back(", ");

   if (a_expression == a_represent)
      l.printf("bold,red", a_represent.acronym(30, 1).c_str());
   else {
      if (a_represent.size() && !is_synonym(a_expression.c_str(), a_represent.c_str())) {
         l.printf("bold,red", a_represent.acronym(30, 1).c_str());
         l.printf("dark gray", "<==");
      }
      l.push_back(a_expression.acronym());
   }
   l.push_back(")");
}

h2_inline void h2_fail_unexpect::print_JE(h2_line& l)
{
   l.printf("", "JE(%s, %s)", e_expression.acronym().c_str(), a_expression.acronym().c_str());
}

h2_inline void h2_fail_unexpect::print_Inner(h2_line& l)
{
   l.push_back("expect is ");
   l.printf("green", e_explain.acronym(30, 1).c_str());
   l.push_back(", actual is ");
   l.printf("bold,red", a_represent.acronym(30, 1).c_str());
}

h2_inline void h2_fail_unexpect::print(int subling_index, int child_index)
{
   h2_line line;
   line.indent(child_index * 2 + 1);
   if (usage == 0) print_Inner(line);
   if (usage == 1) print_OK1(line);
   if (usage == 2) print_OK2(line);
   if (usage == 3) print_JE(line);
   if (user_explain.size()) line.printf("", ", %s", user_explain.c_str());
   line.push_back(get_locate());
   h2_color::printf(line);
}

static inline void fmt_char(char c, bool eq, const char* style, h2_line& line)
{
   char t_style[32] = "";
   if (!eq) strcpy(t_style, style);
   if (c == '\n') c = 'n', strcat(t_style, ",inverse");
   if (c == '\r') c = 'r', strcat(t_style, ",inverse");
   if (c == '\t') c = 't', strcat(t_style, ",inverse");
   if (c == '\0') c = ' ', eq || strcat(t_style, ",inverse");
   line.printf(t_style, "%c", c);
}

h2_inline void h2_fail_strcmp::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);

   h2_line e_line, a_line;
   for (size_t i = 0; i < e_value.size(); ++i) {
      char ac = i < a_value.size() ? a_value[i] : ' ';
      bool eq = caseless ? ::tolower(e_value[i]) == ::tolower(ac) : e_value[i] == ac;
      fmt_char(e_value[i], eq, "green", e_line);
   }
   for (size_t i = 0; i < a_value.size(); ++i) {
      char ec = i < e_value.size() ? e_value[i] : ' ';
      bool eq = caseless ? ::tolower(a_value[i]) == ::tolower(ec) : a_value[i] == ec;
      fmt_char(a_value[i], eq, "red", a_line);
   }

   h2_lines lines = h2_layout::unified(e_line, a_line, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_strfind::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);

   h2_line e_line, a_line;
   for (size_t i = 0; i < e_value.size(); ++i)
      fmt_char(e_value[i], true, "", e_line);

   for (size_t i = 0; i < a_value.size(); ++i)
      fmt_char(a_value[i], true, "", a_line);

   h2_lines lines = h2_layout::seperate(e_line, a_line, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_json::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);
   h2_lines e_lines, a_lines;
   h2_json::diff(e_value, a_value, e_lines, a_lines);
   h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_memcmp::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);
   h2_lines e_lines, a_lines;
   switch (width) {
   case 1: print_bits(e_lines, a_lines); break;
   case 8: print_bytes(e_lines, a_lines); break;
   case 16: print_int16(e_lines, a_lines); break;
   case 32: print_int32(e_lines, a_lines); break;
   case 64: print_int64(e_lines, a_lines); break;
   default: break;
   }
   h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_memcmp::print_bits(h2_lines& e_lines, h2_lines& a_lines)
{
   int rows = ::ceil(e_value.size() * 1.0 / npr_1b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_1b; ++j) {
         for (int k = 0; k < 8; ++k) {
            if ((i * npr_1b + j) * 8 + k < nbits) {
               unsigned char e_val = (e_value[i * npr_1b + j] >> (7 - k)) & 0x1;
               unsigned char a_val = (a_value[i * npr_1b + j] >> (7 - k)) & 0x1;
               if (e_val == a_val) {
                  e_line.push_back(h2_string(e_val ? "1" : "0"));
                  a_line.push_back(h2_string(a_val ? "1" : "0"));
               } else {
                  e_line.printf("green", e_val ? "1" : "0");
                  a_line.printf("bold,red", a_val ? "1" : "0");
               }
            }
         }
         e_line.push_back(" ");
         a_line.push_back(" ");
      }
      e_lines.push_back(e_line);
      a_lines.push_back(a_line);
   }
}

h2_inline void h2_fail_memcmp::print_bytes(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size(), rows = ::ceil(count * 1.0 / npr_8b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_8b; ++j) {
         if (i * npr_8b + j < count) {
            char e_str[8], a_str[8];
            sprintf(e_str, j < 8 ? "%02X " : " %02X", e_value[i * npr_8b + j]);
            sprintf(a_str, j < 8 ? "%02X " : " %02X", a_value[i * npr_8b + j]);
            if (e_value[i * npr_8b + j] == a_value[i * npr_8b + j]) {
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

h2_inline void h2_fail_memcmp::print_int16(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size() / 2, rows = ::ceil(count * 1.0 / npr_16b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_16b; ++j) {
         if (i * npr_16b + j < count) {
            unsigned short e_val = *(unsigned short*)(e_value.data() + (i * npr_16b + j) * 2);
            unsigned short a_val = *(unsigned short*)(a_value.data() + (i * npr_16b + j) * 2);
            char e_str[32], a_str[32];
            sprintf(e_str, "%04X ", e_val);
            sprintf(a_str, "%04X ", a_val);
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

h2_inline void h2_fail_memcmp::print_int32(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size() / 4, rows = ::ceil(count * 1.0 / npr_32b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_32b; ++j) {
         if (i * npr_32b + j < count) {
            unsigned int e_val = *(unsigned int*)(e_value.data() + (i * npr_32b + j) * 4);
            unsigned int a_val = *(unsigned int*)(a_value.data() + (i * npr_32b + j) * 4);
            char e_str[32], a_str[32];
            sprintf(e_str, "%08X ", e_val);
            sprintf(a_str, "%08X ", a_val);
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

h2_inline void h2_fail_memcmp::print_int64(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size() / 8, rows = ::ceil(count * 1.0 / npr_64b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_64b; ++j) {
         if (i * npr_64b + j < count) {
            unsigned long long e_val = *(unsigned long long*)(e_value.data() + (i * npr_64b + j) * 8);
            unsigned long long a_val = *(unsigned long long*)(a_value.data() + (i * npr_64b + j) * 8);
            char e_str[32], a_str[32];
            sprintf(e_str, "%016llX ", e_val);
            sprintf(a_str, "%016llX ", a_val);
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

h2_inline void h2_fail_memoverflow::print(int subling_index, int child_index)
{
   h2_color::printf("", " Memory overflow malloc %p %+d (%p) ", ptr, offset, ptr + offset);

   for (int i = 0; i < spot.size(); ++i)
      h2_color::printf(magic[i] == spot[i] ? "green" : "bold,red", "%02X ", spot[i]);

   h2_color::printf("", "%s\n", get_locate());
   if (0 < bt1.count) h2_color::printf("", "  %p trampled at backtrace:\n", ptr + offset), bt1.print(3);
   if (0 < bt0.count) h2_color::printf("", "  which allocate at backtrace:\n"), bt0.print(3);
}

h2_inline void h2_fail_memleak::add(void* ptr, int size, h2_backtrace& bt)
{
   bytes += size, times += 1;
   for (auto& c : places) {
      if (c.bt == bt) {
         c.ptr2 = ptr, c.size2 = size, c.bytes += size, c.times += 1;
         return;
      }
   }
   places.push_back(P(ptr, size, bt));
}

h2_inline void h2_fail_memleak::print(int subling_index, int child_index)
{
   h2_color::printf("", " Memory Leak ");
   if (1 < places.size()) {
      h2_color::printf("bold,red", "%d", (int)places.size());
      h2_color::printf("", " places ");
   }
   if (1 < times) {
      h2_color::printf("bold,red", "%d", times);
      h2_color::printf("", " times ");
   }
   if (0 < bytes) {
      h2_color::printf("bold,red", "%d", bytes);
      h2_color::printf("", " bytes ");
   }
   h2_color::printf("", "in %s totally%s\n", where, get_locate());

   for (auto& c : places) {
      if (c.times <= 1) {
         h2_color::printf("", "  %p Leak ", c.ptr);
         h2_color::printf("bold,red", "%lld", c.bytes);
         h2_color::printf("", " bytes, at backtrace:\n");
      } else {
         h2_color::printf("", "  %p, %p ... Leak ", c.ptr, c.ptr2);
         h2_color::printf("bold,red", "%lld", c.times);
         h2_color::printf("", " times ");
         h2_color::printf("bold,red", "%lld", c.bytes);
         h2_color::printf("", " bytes (");
         h2_color::printf("bold,red", "%lld", c.size);
         h2_color::printf("", ", ");
         h2_color::printf("bold,red", "%lld", c.size2);
         h2_color::printf("", " ...), at backtrace:\n");
      }
      c.bt.print(3);
   }
}

h2_inline void h2_fail_double_free::print(int subling_index, int child_index)
{
   h2_color::printf("", " %p double free at backtrace:\n", ptr);
   bt_release2.print(2);
   if (0 < bt_allocate.count) h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   if (0 < bt_release1.count) h2_color::printf("", "  already free at backtrace:\n"), bt_release1.print(3);
}

h2_inline void h2_fail_access_after_free::print(int subling_index, int child_index)
{
   h2_color::printf("", " %p %+d (%p) access after free at backtrace:\n", ptr, offset, ptr + offset);
   bt_access.print(2);
   if (0 < bt_allocate.count) h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   if (0 < bt_release.count) h2_color::printf("", "  and free at backtrace:\n"), bt_release.print(3);
}

h2_inline void h2_fail_asymmetric_free::print(int subling_index, int child_index)
{
   h2_color::printf("", " %p allocate with ", ptr);
   h2_color::printf("bold,red", "%s", who_allocate);
   h2_color::printf("", ", release by ");
   h2_color::printf("bold,red", "%s", who_release);
   h2_color::printf("", " asymmetrically at backtrace:\n");
   bt_release.print(2);
   if (0 < bt_allocate.count) h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
}

h2_inline void h2_fail_call::print(int subling_index, int child_index)
{
   h2_line line = {" ", e_who, "() "};
   if (e_call.size()) {
      line.push_back("expected ");
      line.printf("green", e_call.c_str());
      line.push_back(" calls but ");
   }
   line.printf("red,bold", a_call.c_str());
   line.push_back(" called actually");
   line.push_back(get_locate());
   h2_color::printf(line);
}

h2_inline void h2_fail_instantiate::print(int subling_index, int child_index)
{
   why_abstract ? h2_color::printf("", "Instantiate 'class %s' is a abstract class%s\n", class_type, get_locate()) :
                  h2_color::printf("", "Instantiate 'class %s' don't know initialize arguments%s\n", class_type, get_locate());

   h2_color::printf("", "You may take following solutions to fix it: \n");
   if (why_abstract)
      h2_color::printf("", "1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s, Derived %s(...)) \n",
                        action_type,
                        strlen(return_type) ? return_type : "",
                        strlen(return_type) ? ", " : "",
                        class_type, method_name, return_args, class_type);
   else {
      h2_color::printf("", "1. Define default constructor in class %s, or \n", class_type);
      h2_color::printf("", "2. Add parameterized construction in %s(%s%s%s, %s, %s, %s(...)) \n",
                        action_type,
                        strlen(return_type) ? return_type : "",
                        strlen(return_type) ? ", " : "",
                        class_type, method_name, return_args, class_type);
   }
}

static const unsigned char snowfield[] = {0xbe, 0xaf, 0xca, 0xfe, 0xc0, 0xde, 0xfa, 0xce};

struct h2_piece : h2_libc {
   unsigned char *ptr, *page;
   h2_list x;
   int size, page_size, page_count, free_times;
   const char* who_allocate;
   h2_backtrace bt_allocate, bt_release;

   h2_piece(int size_, int alignment, const char* who, h2_backtrace& bt) : size(size_), free_times(0), who_allocate(who), bt_allocate(bt)
   {
      page_size = h2_page_size();
      if (alignment <= 0) alignment = 8;
      page_count = ::ceil((size + alignment + sizeof(snowfield)) / (double)page_size);

#ifdef _WIN32
      page = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      assert(page);
#else
      page = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      assert(page != MAP_FAILED);
#endif

      ptr = page + page_size * page_count - size;
      ptr = (unsigned char*)(((intptr_t)ptr / alignment) * alignment);

      h2_piece** backward = (h2_piece**)(ptr - sizeof(snowfield) - sizeof(void*));
      *backward = this;

      mark_snowfield();
   }

   ~h2_piece()
   {
#ifdef _WIN32
      VirtualFree(page, 0, MEM_DECOMMIT | MEM_RELEASE);
#else
      ::munmap(page, page_size * (page_count + 1));
#endif
   }

   void mark_forbidden()
   {
#ifdef _WIN32
      DWORD old;
      if (!VirtualProtect(page, page_size * (page_count + 1), PAGE_NOACCESS, &old))
         ::printf("VirtualProtect PAGE_NOACCESS failed %d\n", GetLastError());
#else
      if (::mprotect(page, page_size * (page_count + 1), PROT_NONE) != 0)
         ::printf("mprotect PROT_NONE failed %s\n", strerror(errno));
#endif
   }

   void undo_forbidden()
   {
#ifdef _WIN32
      DWORD old;
      if (!VirtualProtect(page, page_size * (page_count + 1), PAGE_READWRITE, &old))
         ::printf("VirtualProtect PAGE_NOACCESS failed %d\n", GetLastError());
#else
      if (::mprotect(page, page_size * (page_count + 1), PROT_READ | PROT_WRITE) != 0)
         ::printf("mprotect PROT_NONE failed %s\n", strerror(errno));
#endif
   }

   void mark_snowfield()
   {
      memcpy(ptr - sizeof(snowfield), snowfield, sizeof(snowfield));
      memcpy(ptr + size, snowfield, sizeof(snowfield));
#ifdef _WIN32
      DWORD old;
      if (!VirtualProtect(page + page_size * page_count, page_size, PAGE_READONLY, &old))
         ::printf("VirtualProtect PAGE_READONLY failed %d\n", GetLastError());
#else
      if (::mprotect(page + page_size * page_count, page_size, PROT_READ) != 0)
         ::printf("mprotect PROT_READ failed %s\n", strerror(errno));
#endif
   }

   h2_fail* check_snowfield()
   {
      h2_fail* fail = nullptr;
      if (memcmp(ptr + size, snowfield, sizeof(snowfield)))
         h2_fail::append_subling(fail, new h2_fail_memoverflow(ptr, size, snowfield, sizeof(snowfield), bt_allocate, h2_backtrace()));
      if (memcmp(ptr - sizeof(snowfield), snowfield, sizeof(snowfield)))
         h2_fail::append_subling(fail, new h2_fail_memoverflow(ptr, -(int)sizeof(snowfield), snowfield, sizeof(snowfield), bt_allocate, h2_backtrace()));

#ifdef _WIN32
      DWORD old;
      if (!VirtualProtect(page, page_size * (page_count + 1), PAGE_NOACCESS, &old))
         ::printf("VirtualProtect PAGE_NOACCESS failed %d\n", GetLastError());
#else
      if (::mprotect(page, page_size * (page_count + 1), PROT_NONE) != 0)
         ::printf("mprotect PROT_NONE failed %s\n", strerror(errno));
#endif
      return fail;
   }

   h2_fail* check_asymmetric_free(const char* who_release)
   {
      static const char* free_a[] = {"malloc", "calloc", "realloc", "reallocf", "posix_memalign", "memalign", "aligned_alloc", "valloc", "pvalloc", nullptr};
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

      h2_backtrace bt_release(O.isMAC() ? 6 : 5);
      return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release);
   }

   h2_fail* check_double_free()
   {
      h2_fail* fail = nullptr;
      h2_backtrace bt(O.isMAC() ? 6 : 5);
      if (free_times++ == 0)
         bt_release = bt;
      else
         fail = (h2_fail*)new h2_fail_double_free(ptr, bt_allocate, bt_release, bt);
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

      if (!fail) mark_forbidden();

      return fail;
   }

   bool in_range(const void* p)
   {
      const unsigned char* p0 = page;
      const unsigned char* p2 = p0 + page_size * (page_count + 1);
      return p0 <= (const unsigned char*)p && (const unsigned char*)p < p2;
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list using_list, freed_list;

   const char* file;
   int line;
   const char* where;
   long long limited;
   const char* fill;

   h2_block(const char* file_, int line_, const char* where_, long long limited_, const char* fill_)
     : file(file_), line(line_), where(where_), limited(limited_), fill(fill_) {}

   h2_fail* leak_check()
   {
      h2_fail_memleak* fail = nullptr;
      if (!using_list.empty()) {
         fail = new h2_fail_memleak(file, line, where);
         h2_list_for_each_entry(p, &using_list, h2_piece, x) fail->add(p->ptr, p->size, p->bt_allocate);
      }
      h2_list_for_each_entry(p, &freed_list, h2_piece, x)
      {
         p->x.out();
         delete p;
      }
      return fail;
   }

   h2_piece* new_piece(const char* who, int size, int alignment, const char* fill_, h2_backtrace& bt)
   {
      if (limited < size) return nullptr;
      limited -= size;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      if (fill_ ? fill_ : (fill_ = fill))
         for (int i = 0, j = 0, l = strlen(fill_); i < size; ++i, ++j)
            ((char*)p->ptr)[i] = fill_[j % (l ? l : 1)];

      using_list.push(&p->x);
      return p;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->ptr == ptr) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->ptr == ptr) return p;
      return nullptr;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      limited += p->size;

      p->x.out();
      freed_list.push(&p->x);
      return p->free(who);
   }

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->in_range(addr)) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->in_range(addr)) return p;
      return nullptr;
   }
};

struct h2_stack {
   h2_singleton(h2_stack);

   h2_list blocks;

   bool escape(h2_backtrace& bt)
   {
      static struct {
         void* base;
         int size;
      } escape_functions[] = {
        {(void*)sprintf, 300},
        {(void*)vsnprintf, 300},  //  {(void*)sscanf, 300},
        {(void*)localtime, 300},
#ifndef _WIN32
        {(void*)tzset, 300},
#endif
      };

      for (auto& x : escape_functions)
         if (bt.has(x.base, x.size))
            return true;

      return false;
   }

   void push(const char* file, int line, const char* where, long long limited, const char* fill)
   {
      h2_block* b = new h2_block(file, line, where, limited, fill);
      blocks.push(&b->x);
   }

   h2_fail* pop()
   {
      h2_block* b = h2_list_pop_entry(&blocks, h2_block, x);
      h2_fail* fail = b->leak_check();
      delete b;
      return fail;
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, const char* fill)
   {
      h2_backtrace bt(O.isMAC() ? 3 : 2);
      h2_block* b = escape(bt) ? h2_list_bottom_entry(&blocks, h2_block, x) : h2_list_top_entry(&blocks, h2_block, x);
      return b ? b->new_piece(who, size, alignment, fill, bt) : nullptr;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry(p, &blocks, h2_block, x)
      {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return piece;
      }
      return nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry(p, &blocks, h2_block, x)
      {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return p->rel_piece(who, piece);
      }
      h2_debug("Warning: free %p not found!", ptr);
      return nullptr;
   }

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry(p, &blocks, h2_block, x)
      {
         h2_piece* piece = p->host_piece(addr);
         if (piece) return piece;
      }
      return nullptr;
   }
};

// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h
// https://github.com/google/tcmalloc/blob/master/tcmalloc/libc_override.h

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" {
extern malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
}
#endif

struct h2_hook {
   h2_singleton(h2_hook);

   static void free(void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("free", ptr)); /* overflow or double free */
   }
   static void* malloc(size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("malloc", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* calloc(size_t count, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("calloc", size * count, 0, "\0");
      return p ? p->ptr : nullptr;
   }
   static void* realloc(void* ptr, size_t size)
   {
      if (size == 0) {
         free(ptr);
         return nullptr;
      }

      h2_piece* old_p = h2_stack::I().get_piece(ptr);
      if (!old_p) return nullptr;

      h2_piece* new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (!new_p) return nullptr;

      memcpy(new_p->ptr, old_p->ptr, old_p->size);
      h2_fail_g(h2_stack::I().rel_piece("free", ptr));

      return new_p->ptr;
   }
   static int posix_memalign(void** memptr, size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("posix_memalign", size, alignment, nullptr);
      return p ? (*memptr = p->ptr, 0) : ENOMEM;
   }
   static void* aligned_alloc(size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("aligned_alloc", size, alignment, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* _aligned_malloc(size_t size, size_t alignment)
   {
      h2_piece* p = h2_stack::I().new_piece("_aligned_malloc", size, alignment, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void _aligned_free(void* memblock)
   {
      if (memblock) h2_fail_g(h2_stack::I().rel_piece("_aligned_free", memblock));
   }
   static void* operator new(std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* operator new(std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new nothrow", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* operator new[](std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new[]", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* operator new[](std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new[] nothrow", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void operator delete(void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete(void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete[](void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[]", ptr));
   }
   static void operator delete[](void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[]", ptr));
   }

#if defined __GLIBC__

   static void free_hook(void* __ptr, const void* caller)
   {
      free(__ptr);
   }
   static void* malloc_hook(size_t __size, const void* caller) { return malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return aligned_alloc(__alignment, __size); }

   void (*sys__free_hook)(void*, const void*);
   void* (*sys__malloc_hook)(size_t, const void*);
   void* (*sys__realloc_hook)(void*, size_t, const void*);
   void* (*sys__memalign_hook)(size_t, size_t, const void*);

#elif defined __APPLE__
   static size_t mz_size(malloc_zone_t* zone, const void* ptr)
   {
      h2_piece* p = h2_stack::I().get_piece(ptr);
      return p ? p->size : 0;
   }

   static void* mz_malloc(malloc_zone_t* zone, size_t size) { return malloc(size); }
   static void* mz_calloc(malloc_zone_t* zone, size_t num_items, size_t size) { return calloc(num_items, size); }
   static void* mz_valloc(malloc_zone_t* zone, size_t size) { return malloc(size); }
   static void mz_free(malloc_zone_t* zone, void* ptr) { free(ptr); }
   static void mz_free_sized(malloc_zone_t* zone, void* ptr, size_t size) { free(ptr); }
   static void* mz_realloc(malloc_zone_t* zone, void* ptr, size_t size) { return realloc(ptr, size); }
   static void* mz_memalign(malloc_zone_t* zone, size_t align, size_t size) { return aligned_alloc(align, size); }
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

#elif defined _WIN32

#else

#endif

   h2_stubs stubs;

   h2_hook()
   {
#if defined __GLIBC__
      sys__free_hook = __free_hook;
      sys__malloc_hook = __malloc_hook;
      sys__realloc_hook = __realloc_hook;
      sys__memalign_hook = __memalign_hook;

#elif defined __APPLE__

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

#   if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      mz.version = 6;
      mz.free_definite_size = &mz_free_sized;
      mz.memalign = &mz_memalign;
      mi.zone_locked = &mi_zone_locked;

      if (malloc_default_purgeable_zone) malloc_default_purgeable_zone();

#   endif

#elif defined _WIN32
      //TODO
#else
#endif
   }

   void dohook()
   {
#if defined __GLIBC__
      __free_hook = free_hook;
      __malloc_hook = malloc_hook;
      __realloc_hook = realloc_hook;
      __memalign_hook = memalign_hook;
#elif defined __APPLE__
      malloc_zone_register(&mz);
      malloc_zone_t* default_zone = get_default_zone();
      malloc_zone_unregister(default_zone);
      malloc_zone_register(default_zone);
#elif defined _WIN32
      stubs.add((void*)::free, (void*)free);
      stubs.add((void*)::malloc, (void*)malloc);
      stubs.add((void*)::realloc, (void*)realloc);
      stubs.add((void*)::calloc, (void*)calloc);
      stubs.add((void*)::_aligned_malloc, (void*)_aligned_malloc);
#else
#endif
#if defined _ISOC11_SOURCE
      stubs.add((void*)::aligned_alloc, (void*)aligned_alloc);
#endif
      stubs.add((void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t)) operator new));
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&)) operator new));
      stubs.add((void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t)) operator new[]));
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&)) operator new[]));
      stubs.add((void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*)) operator delete));
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&)) operator delete));
      stubs.add((void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*)) operator delete[]));
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&)) operator delete[]));
   }

   void unhook()
   {
#if defined __GLIBC__
      __free_hook = sys__free_hook;
      __malloc_hook = sys__malloc_hook;
      __realloc_hook = sys__realloc_hook;
      __memalign_hook = sys__memalign_hook;
#elif defined __APPLE__
      malloc_zone_unregister(&mz);
#elif defined _WIN32
#else
#endif
      stubs.clear();
   }

#ifndef _WIN32
   // https://www.gnu.org/software/libsigsegv/
   static void segment_fault_handler(int sig, siginfo_t* si, void* unused)
   {
      h2_piece* piece = h2_stack::I().host_piece(si->si_addr);
      if (piece) {
         piece->undo_forbidden();
         h2_backtrace bt_access(3);
         h2_fail_g(new h2_fail_access_after_free(piece->ptr, (intptr_t)si->si_addr - (intptr_t)piece->ptr, piece->bt_allocate, piece->bt_release, bt_access));
      } else {
         h2_debug();
         exit(1);
      }
   }
   void install_segment_fault_handler()
   {
      struct sigaction act;
      act.sa_sigaction = segment_fault_handler;
      act.sa_flags = SA_SIGINFO;
      sigemptyset(&act.sa_mask);
      if (sigaction(SIGSEGV, &act, nullptr) == -1) perror("Register SIGSEGV handler failed");
      if (sigaction(SIGBUS, &act, nullptr) == -1) perror("Register SIGBUS handler failed");
   }
#endif
};

h2_inline void h2_heap::initialize()
{
#ifndef _WIN32
   if (O.memory_check && !O.debug) h2_hook::I().install_segment_fault_handler();
#endif
   stack::root();
}
h2_inline void h2_heap::dohook()
{
   if (O.memory_check) h2_hook::I().dohook();
}
h2_inline void h2_heap::unhook()
{
   if (O.memory_check) h2_hook::I().unhook();
}
h2_inline void h2_heap::stack_push_block(const char* file, int line, const char* where, long long limited, const char* fill)
{
   h2_stack::I().push(file, line, where, limited, fill);
}
h2_inline h2_fail* h2_heap::stack_pop_block()
{
   return h2_stack::I().pop();
}

struct h2_json_parse {
   const char* text;
   int length;
   int offset;

   h2_json_parse(const char* text_, int length_) : text(text_), length(length_), offset(0) {}
   struct h2_json_parse& strip()
   {
      while (offset < length && ::isspace(text[offset])) offset++;
      return *this;
   }
   bool startswith(const char* s, int n)
   {
      if (length - offset < n) return false;
      return ::strncmp(text + offset, s, n) == 0;
   }
   bool startswith(char from, char to = '\0')
   {
      if (length - offset < 1) return false;
      if (to == '\0') to = from;
      return from <= text[offset] && text[offset] <= to;
   }
};

struct h2_json_node : h2_libc {
   static const int t_absent = 0;
   static const int t_null = 1;
   static const int t_boolean = 2;
   static const int t_number = 3;
   static const int t_string = 4;
   static const int t_regexp = 5;
   static const int t_array = 6;
   static const int t_object = 7;

   int type;
   h2_string key_string;
   h2_string value_string;
   double value_double;
   bool value_boolean;
   h2_vector<h2_json_node*> children; /* array or object */

   ~h2_json_node()
   {
      for (auto it : children) delete it;
   }
   h2_json_node() : type(t_absent), value_double(0), value_boolean(false) {}
   h2_json_node(const char* json_string, int length = 0) : h2_json_node()
   {
      if (json_string) {
         if (length == 0) length = strlen(json_string);
         if (length) {
            h2_json_parse x(json_string, length);
            if (!parse_value(x.strip())) type = t_absent;
         }
      }
   }

   int size() { return children.size(); }

   h2_json_node* get(int index) { return 0 <= index && index < children.size() ? children[index] : nullptr; }

   h2_json_node* get(const char* name)
   {
      if (name)
         for (auto node : children)
            if (!node->key_string.compare(name))
               return node;

      return nullptr;
   }

   void del(h2_json_node* child)
   {
      for (auto it = children.begin(); it != children.end(); it++) {
         if (child == *it) {
            children.erase(it);
            delete child;
            return;
         }
      }
   }

   bool is_null() { return t_null == type; }
   bool is_bool() { return t_boolean == type; }
   bool is_number() { return t_number == type; }
   bool is_string() { return t_string == type; }
   bool is_regexp() { return t_regexp == type; }
   bool is_array() { return t_array == type; }
   bool is_object() { return t_object == type; }

   bool parse_number(h2_json_parse& x)
   {
      int i;
      for (i = 0; x.offset + i < x.length; ++i) {
         const char c = x.text[x.offset + i];
         if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0')
            break;
      }

      value_string.assign(x.text + x.offset, i);

      int err = 0;
      value_double = tinyexpr::eval(value_string.c_str(), &err);
      type = t_number;
      x.offset += i;

      return 0 == err;
   }

   bool parse_string(h2_json_parse& x)
   {
      const char bound = x.text[x.offset];
      x.offset++;

      if (x.length <= x.offset) return false;

      const char* src = x.text + x.offset;
      int len = 0;
      for (; x.text[x.offset] != bound; ++len) {
         if (x.text[x.offset++] == '\\') x.offset++;
         if (x.length <= x.offset) return false;
      }

      for (; len > 0; ++src, --len)
         if (*src != '\\')
            value_string.push_back(*src);
         else
            switch (*++src) {
            case 'b': value_string.push_back('\b'); break;
            case 'f': value_string.push_back('\f'); break;
            case 'n': value_string.push_back('\n'); break;
            case 'r': value_string.push_back('\r'); break;
            case 't': value_string.push_back('\t'); break;
            case '\"': value_string.push_back('\"'); break;
            case '\\': value_string.push_back('\\'); break;
            case '/': value_string.push_back('/'); break;
            default: return false;
            }

      type = t_string;
      x.offset++;

      return true;
   }

   bool parse_regexp(h2_json_parse& x)
   {
      bool ret = parse_string(x);
      type = t_regexp;
      return ret;
   }

   bool parse_value(h2_json_parse& x)
   {
      /* t_null */
      if (x.startswith("null", 4)) {
         type = t_null;
         x.offset += 4;
         return true;
      }
      /* false */
      if (x.startswith("false", 5)) {
         type = t_boolean;
         value_boolean = false;
         x.offset += 5;
         return true;
      }
      /* true */
      if (x.startswith("true", 4)) {
         type = t_boolean;
         value_boolean = true;
         x.offset += 4;
         return true;
      }
      /* string */
      if (x.startswith('\"') || x.startswith('\'')) return parse_string(x);
      /* regexp */
      if (x.startswith('/')) return parse_regexp(x);

      /* array */
      if (x.startswith('[')) return parse_array(x);
      /* object */
      if (x.startswith('{')) return parse_object(x);

      /* number */
      if (1 /* x.startswith('-') || x.startswith('0', '9') */) return parse_number(x);

      return false;
   }

   bool parse_array(h2_json_parse& x)
   {
      x.offset++;  //pass [

      while (!x.strip().startswith(']')) {
         h2_json_node* new_node = new h2_json_node();
         if (!new_node) return false;

         children.push_back(new_node);

         if (!new_node->parse_value(x)) return false;

         if (x.strip().startswith(',')) x.offset++;
      }

      type = t_array;
      x.offset++;

      return true;
   }

   bool parse_object(h2_json_parse& x)
   {
      x.offset++;  //pass {

      while (!x.strip().startswith('}')) {
         h2_json_node* new_node = new h2_json_node();
         if (!new_node) return false;

         children.push_back(new_node);

         if (!new_node->parse_string(x)) return false;

         new_node->key_string = new_node->value_string;
         new_node->value_string = "";

         if (!x.strip().startswith(':')) return false;
         x.offset++;

         if (!new_node->parse_value(x.strip())) return false;

         if (x.strip().startswith(',')) x.offset++;
      }

      type = t_object;
      x.offset++;

      return true;
   }

   void dual(int& _type, const char*& _class, h2_string& _key, h2_string& _value)
   {
      _type = t_string;

      if (key_string.size()) _key = "\"" + key_string + "\"";

      switch (type) {
      case t_null:
         _type = t_string;
         _class = "atomic";
         _value = "null";
         break;
      case t_boolean:
         _type = t_string;
         _class = "atomic";
         _value = value_boolean ? "true" : "false";
         break;
      case t_number:
         _type = t_string;
         _class = "atomic";
         _value = h2_stringify(value_double);
         break;
      case t_string:
         _type = t_string;
         _class = "atomic";
         _value = "\"" + value_string + "\"";
         break;
      case t_regexp:
         _type = t_string;
         _class = "atomic";
         _value = value_string;
         break;
      case t_array:
         _type = t_array;
         _class = "array";
         break;
      case t_object:
         _type = t_object;
         _class = "object";
         break;
      }
   }

   void print(h2_lines& lines, int depth = 0, int next = 0, bool fold = false)
   {
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back("\"" + key_string + "\": ");
      if (is_null())
         line.push_back("null");
      else if (is_bool())
         line.push_back(value_boolean ? "true" : "false");
      else if (is_number())
         line.push_back(h2_stringify(value_double));
      else if (is_string())
         line.push_back("\"" + value_string + "\"");
      else if (is_regexp())
         line.push_back("\"/" + value_string + "/\"");
      else if (is_array() || is_object()) {
         line.push_back(is_array() ? "[ " : "{ ");
         h2_lines __lines;
         for (size_t i = 0; i < children.size(); ++i)
            children[i]->print(__lines, depth + 1, children.size() - i - 1, fold);

         if (fold && __lines.foldable()) {
            line.fold(__lines);
            line.push_back(is_array() ? " ]" : " }");
         } else {
            lines.push_back(line), line.clear();
            lines.concat_back(__lines);
            line.indent(depth * 2);
            line.push_back(is_array() ? "]" : "}");
         }
      }
      if (line.size()) {
         if (next) line.push_back(",");
         lines.push_back(line), line.clear();
      }
   }
};

struct h2_json_match {
   static bool match_array(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return false;
      if (e->children.size() != a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->children[i]))
            return false;
      return true;
   }

   static bool match_object(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return false;
      if (e->children.size() > a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->get(e->children[i]->key_string.c_str())))
            return false;
      return true;
   }

   static bool match(h2_json_node* e, h2_json_node* a)
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
         return a->is_string() && e->value_string == a->value_string;
      case h2_json_node::t_regexp:
         return a->is_string() && h2_regex_match(e->value_string.c_str(), a->value_string.c_str());
      case h2_json_node::t_array:
         return a->is_array() && match_array(e, a);
      case h2_json_node::t_object:
         return a->is_object() && match_object(e, a);
      default: return false;
      };
   }

   static h2_json_node* search(h2_vector<h2_json_node*>& haystack, h2_json_node* needle)
   {
      for (auto node : haystack)
         if (match(needle, node)) return node;

      return nullptr;
   }
};

struct h2_json_dual : h2_libc {  // combine 2 Node into a Dual
   bool match;
   int e_type, a_type;
   const char *e_class, *a_class;
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_lines e_blob, a_blob;
   h2_vector<h2_json_dual*> children;
   h2_json_dual* perent;
   int depth;

   ~h2_json_dual()
   {
      for (auto it : children) delete it;
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, h2_json_dual* perent_ = nullptr) : match(false), e_type(h2_json_node::t_absent), a_type(h2_json_node::t_absent), e_class("blob"), a_class("blob"), perent(perent_), depth(perent_ ? perent_->depth + 1 : 0)
   {
      match = h2_json_match::match(e, a);
      if (e) e->dual(e_type, e_class, e_key, e_value);
      if (a) a->dual(a_type, a_class, a_key, a_value);

      if (e_type != a_type) {
         if (e) e->print(e_blob, depth, 0, true);
         if (a) a->print(a_blob, depth, 0, true);
         e_class = a_class = "blob";
      } else if (e_type == h2_json_node::t_object) {
         for (auto i = e->children.begin(); i != e->children.end();) {
            h2_json_node *e1 = *i, *a1 = a->get(e1->key_string.c_str());
            if (!a1) a1 = h2_json_match::search(a->children, e1);
            if (a1) {
               children.push_back(new h2_json_dual(e1, a1, this));
               a->del(a1);
               i = e->children.erase(i);
               delete e1;
            } else
               i++;
         }

         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i)
            children.push_back(new h2_json_dual(e->get(i), a->get(i), this));
      } else if (e_type == h2_json_node::t_array) {
         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i)
            children.push_back(new h2_json_dual(e->get(i), a->get(i), this));
      }
   }

   bool has_next_e(h2_vector<h2_json_dual*>& subling)
   {
      for (size_t i = 0; i < subling.size(); ++i) {
         if (subling[i] == this) {
            for (++i; i < subling.size(); ++i)
               if (subling[i]->e_type != h2_json_node::t_absent)
                  return true;
            break;
         }
      }
      return false;
   }

   bool has_next_a(h2_vector<h2_json_dual*>& subling)
   {
      for (size_t i = 0; i < subling.size(); ++i) {
         if (subling[i] == this) {
            for (++i; i < subling.size(); ++i)
               if (subling[i]->a_type != h2_json_node::t_absent)
                  return true;
            break;
         }
      }
      return false;
   }

   void align(h2_lines& e, h2_lines& a, h2_vector<h2_json_dual*>* subling = nullptr)
   {
      if (!strcmp(e_class, "blob")) {
         e_blob.samesizify(a_blob);
         for (auto& line : e_blob)
            line.insert(line.begin(), "\033{bold,cyan}"), line.push_back("\033{reset}");

         for (auto& line : a_blob)
            line.insert(line.begin(), "\033{yellow}"), line.push_back("\033{reset}");

         e.concat_back(e_blob);
         a.concat_back(a_blob);
         return;
      }

      h2_line e_line, a_line;
      e_line.indent(depth * 2);
      a_line.indent(depth * 2);

      if (e_key.size()) {
         if (!match && e_key != a_key) e_line.push_back("\033{green}");
         e_line.push_back(e_key);
         if (!match && e_key != a_key) e_line.push_back("\033{reset}");
         e_line.push_back(": ");
      }

      if (a_key.size()) {
         if (!match && a_key != e_key) a_line.push_back("\033{red,bold}");
         a_line.push_back(a_key);
         if (!match && a_key != e_key) a_line.push_back("\033{reset}");
         a_line.push_back(": ");
      }

      if (!strcmp(e_class, "atomic")) {
         if (e_value.size()) {
            if (!match && e_value != a_value) e_line.push_back("\033{green}");
            e_line.push_back(e_value);
            if (!match && e_value != a_value) e_line.push_back("\033{reset}");
         }
         if (a_value.size()) {
            if (!match && a_value != e_value) a_line.push_back("\033{red,bold}");
            a_line.push_back(a_value);
            if (!match && a_value != e_value) a_line.push_back("\033{reset}");
         }
      } else if (!strcmp(e_class, "object") || !strcmp(e_class, "array")) {
         e_line.push_back(strcmp(e_class, "object") ? "[ " : "{ ");
         a_line.push_back(strcmp(a_class, "object") ? "[ " : "{ ");

         e.push_back(e_line), e_line.clear();
         a.push_back(a_line), a_line.clear();
         for (size_t i = 0; i < children.size(); ++i)
            children[i]->align(e, a, &children);

         e_line.indent(depth * 2);
         e_line.push_back(strcmp(e_class, "object") ? "]" : "}");
         a_line.indent(depth * 2);
         a_line.push_back(strcmp(a_class, "object") ? "]" : "}");
      }
      if (e_line.size()) {
         if (subling && has_next_e(*subling)) e_line.push_back(",");
         e.push_back(e_line), e_line.clear();
      }
      if (a_line.size()) {
         if (subling && has_next_a(*subling)) a_line.push_back(",");
         a.push_back(a_line), a_line.clear();
      }
   }
};

h2_inline bool h2_json::match(const h2_string& expect, const h2_string& actual)
{
   h2_json_node e(expect.c_str());
   h2_json_node a(actual.c_str());
   return h2_json_match::match(&e, &a);
}

h2_inline void h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines)
{
   h2_json_node e_node(expect.c_str());
   h2_json_node a_node(actual.c_str());
   h2_json_dual dual(&e_node, &a_node);
   dual.align(e_lines, a_lines);
}

struct h2_libc_malloc {
   h2_singleton(h2_libc_malloc);

   struct blob {
      size_t size;
      h2_list link;
   };

   char buffer[1024 * 1024 * 100];
   h2_list blobs;

   h2_libc_malloc()
   {
      blob* b = (blob*)buffer;
      b->size = sizeof(buffer);
      blobs.add_tail(&b->link);
   }

   void merge()
   {
      blob* b = nullptr;
      h2_list_for_each_entry(p, &blobs, blob, link)
      {
         if (b) {
            if (((char*)b) + b->size == (char*)p) {
               b->size += p->size;
               p->link.out();
               continue;
            }
         }
         b = p;
      }
   }

   void insert(blob* b)
   {
      blob* n = nullptr;
      h2_list_for_each_entry(p, &blobs, blob, link)
      {
         if (((char*)b) + b->size <= (char*)p) {
            n = p;
            break;
         }
      }
      if (n)
         n->link.add_before(&b->link);
      else
         blobs.add_tail(&b->link);

      merge();
   }

   void* malloc(size_t size)
   {
      size = (size + 7) / 8 * 8;
      blob* b = nullptr;
      h2_list_for_each_entry(p, &blobs, blob, link)
      {
         if (size <= p->size - sizeof(p->size)) {
            b = p;
            p->link.out();
            break;
         }
      }
      if (!b) {
         int pagesize = h2_page_size();
         int pagecount = ::ceil((size + sizeof(b->size)) / (double)pagesize);

#ifdef _WIN32
         PVOID ptr = VirtualAlloc(NULL, pagecount * pagesize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
         if (ptr == NULL) return nullptr;
#else
         void* ptr = ::mmap(nullptr, pagecount * pagesize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
         if (ptr == MAP_FAILED) return nullptr;
#endif
         b = (blob*)ptr;
         b->size = pagecount * pagesize;
      }
      size_t bz = b->size;
      b->size = size + sizeof(b->size);
      size_t rz = bz - b->size;
      if (sizeof(blob) <= rz) {
         blob* r = (blob*)(((char*)b) + b->size);
         r->size = rz;
         insert(r);
      } else {
         b->size += rz;
      }

      return (void*)&b->link;
   }

   void free(void* ptr)
   {
      blob* b = (blob*)(((char*)ptr) - sizeof(b->size));
      insert(b);
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
#ifdef _WIN32
   return _write(fd, buf, count);
#else
   return ::syscall(SYS_write, fd, buf, count);
#endif
}

h2_inline h2_fail* h2_string_equal_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (r) dont = !dont;
   if (a.equals(e, caseless) == !dont) return nullptr;
   if (h2_wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   if (h2_regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;

   return new h2_fail_strcmp(e, a, dont, caseless);
}

h2_inline h2_fail* h2_float_equal_matches::matches(const long double a, bool caseless, bool dont) const
{
   if (r) dont = !dont;
   // the machine epsilon has to be scaled to the magnitude of the values used
   // and multiplied by the desired precision in ULPs (units in the last place)
   // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
   //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
   bool result = std::fabs(a - e) < 0.00001;
   if (result == !dont) return nullptr;
   return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), dont, false, "");
}

static inline bool __is_bin_string(const char* s)
{
   for (const char* p = s; *p; p++)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

static inline bool __is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; p++)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

static inline int __hex_to_byte(char c)
{
   return '0' <= c && c <= '9' ? c - '0' : ('A' <= c && c <= 'F' ? c - 'A' + 10 : ('a' <= c && c <= 'f' ? c - 'a' + 10 : -1));
}

static inline int __hex_to_bytes(const char* hex, unsigned char* bytes)
{
   memset(bytes, 0, strlen(hex));
   if (hex[0] == '0' && ::tolower(hex[1]) == 'x') hex += 2;
   char b;
   int c = 0;
   for (const char* p = hex; *p; p++) {
      if (::isxdigit(*p)) {
         if (++c % 2 == 0)
            bytes[c / 2 - 1] = (unsigned char)((__hex_to_byte(b) << 4) + __hex_to_byte(*p));
         else
            b = *p;
      }
   }
   return 8 * c / 2;
}

static inline int __bin_to_bytes(const char* bin, unsigned char* bytes)
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

static inline bool __bits_equal(const unsigned char* e, const unsigned char* a, int nbits)
{
   for (int k = 0; k < nbits; ++k) {
      int i = k / 8, j = 7 - k % 8;
      if (((e[i] >> j) & 1) != ((a[i] >> j) & 1)) return false;
   }
   return true;
}

static inline char* __print_size(int width, int nbits)
{
   static char t[64];
   switch (width) {
   case 1: sprintf(t, "%d bit%s", nbits, nbits > 1 ? "s" : ""); break;
   case 8: sprintf(t, "%d byte%s", nbits / 8, nbits / 8 > 1 ? "s" : ""); break;
   case 16: sprintf(t, "%d word%s", nbits / 16, nbits / 16 > 1 ? "s" : ""); break;
   case 32: sprintf(t, "%d dword%s", nbits / 32, nbits / 32 > 1 ? "s" : ""); break;
   case 64: sprintf(t, "%d qword%s", nbits / 64, nbits / 64 > 1 ? "s" : ""); break;
   default: sprintf(t, "?"); break;
   }
   return t;
}

h2_inline h2_fail* h2_memcmp_matches::matches(const void* a, bool caseless, bool dont) const
{
   bool result = false;

   const unsigned char* _e[3];
   int _nbits[3];
   int _width[3];
   int i, gc = 0;

   if (width == 0) /* guess width */ {
      // Case1: binary string descript memory
      if (__is_bin_string((const char*)e)) {
         _e[gc] = (const unsigned char*)alloca(strlen((const char*)e));
         _nbits[gc] = __bin_to_bytes((const char*)e, (unsigned char*)_e[gc]);
         _width[gc] = 1;
         ++gc;
      }
      // Case2: hex string descript memory
      if (__is_hex_string((const char*)e)) {
         _e[gc] = (const unsigned char*)alloca(strlen((const char*)e));
         _nbits[gc] = __hex_to_bytes((const char*)e, (unsigned char*)_e[gc]);
         _width[gc] = 8;
         ++gc;
      }
      // Case3: string filled memory
      _e[gc] = (const unsigned char*)e;
      _nbits[gc] = strlen((const char*)e) * 8;
      _width[gc] = 8;
      ++gc;
   } else {
      _e[gc] = (const unsigned char*)e;
      _nbits[gc] = nbits;
      _width[gc] = width;
      ++gc;
   }

   for (i = 0; i < gc; ++i) {
      if (_width[i] == 1)
         result = __bits_equal((const unsigned char*)_e[i], (const unsigned char*)a, _nbits[i]);
      else
         result = memcmp(_e[i], a, _nbits[i] / 8) == 0;

      if (result) break;
   }

   if (result == !dont) return nullptr;
   int j = result ? i : 0;
   return new h2_fail_memcmp((const unsigned char*)_e[j], (const unsigned char*)a, _width[j], _nbits[j], dont, __print_size(_width[j], _nbits[j]), h2_stringify(a));
}

h2_inline h2_fail* h2_regex_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, dont, caseless, "Re");
}

h2_inline h2_fail* h2_wildcard_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, dont, caseless, "We");
}

h2_inline h2_fail* h2_strcmp_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.equals(e, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, dont, caseless, "");
}

h2_inline h2_fail* h2_contains_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.contains(substring, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(substring, a, dont, caseless, "Contains");
}

h2_inline h2_fail* h2_startswith_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(prefix_string, a, dont, caseless, "StartsWith");
}

h2_inline h2_fail* h2_endswith_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(suffix_string, a, dont, caseless, "EndsWith");
}

h2_inline h2_fail* h2_json_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if ((h2_json::match(e, a)) == !dont) return nullptr;
   return new h2_fail_json(e, a, dont, caseless);
}

h2_inline bool h2_mocks::add(h2_mock* mock)
{
   h2_list_for_each_entry(p, &mocks, h2_mock, x) if (p == mock) return false;
   mocks.push(&mock->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear()
{
   h2_fail* fail = nullptr;
   h2_list_for_each_entry(p, &mocks, h2_mock, x)
   {
      h2_fail::append_subling(fail, p->times_check());
      p->reset();
      p->x.out();
   }
   return fail;
}

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

struct h2__patch {
   h2_singleton(h2__patch);
   h2_stubs stubs;

   static struct tm* localtime(const time_t* clock) { return ::gmtime(clock); }
   static struct tm* localtime_r(const time_t* timep, struct tm* result)
   {
      // memcpy(result, ::gmtime(timep), sizeof(struct tm));
      return result;
   }

   h2__patch()
   {
#if defined __GLIBC__
      stubs.add((void*)::localtime, (void*)localtime);
      stubs.add((void*)::localtime_r, (void*)localtime_r);
#endif
   }
};

h2_inline void h2_patch::initialize()
{
   h2__patch::I();
}

h2_inline h2_report::h2_report() : total_cases(0), done_cases(0), percentage(0), ss(0), cs(0), tt(0), ts(0), tc(0) {}
h2_inline void h2_report::on_task_start(int cases)
{
   total_cases = cases;
   tt = h2_now();
};
h2_inline void h2_report::on_task_endup(int status_stats[8], int round) { tt = h2_now() - tt; };
h2_inline void h2_report::on_suite_start(h2_suite* s) { ts = h2_now(); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { ts = h2_now() - ts; }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { tc = h2_now(); };
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c)
{
   percentage = ++done_cases * 100 / total_cases;
   tc = h2_now() - tc;
};

struct h2_report_console : h2_report {
   void on_task_endup(int status_stats[8], int round) override
   {
      h2_report::on_task_endup(status_stats, round);
      if (O.listing) {
         h2_color::printf("bold,green", "Listing <%d suites, %d cases, %d todo, %d filtered>\n", ss, total_cases, status_stats[h2_case::TODOED], status_stats[h2_case::FILTED]);
      } else {
         h2_color::printf("", "\n[%3d%%] ", percentage);
         char t[62] = "";
         if (1 < round) sprintf(t, " %d rounds,", round);
         if (0 < status_stats[h2_case::FAILED])
            h2_color::printf("bold,red", "Failed <%d failed, %d passed, %d todo, %d filtered,%s %lld ms>\n", status_stats[h2_case::FAILED], status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], t, tt);
         else
            h2_color::printf("bold,green", "Passed <%d passed, %d todo, %d filtered, %d cases,%s %lld ms>\n", status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], total_cases, t, tt);
      }
   }
   void on_suite_start(h2_suite* s) override
   {
      cs = 0;
      if (O.listing) {
         h2_color::printf("", "SUITE%d. ", ++ss);
         h2_color::printf("bold,yellow", "%s", s->name);
         h2_color::printf("", " @ %s:%d\n", s->file, s->line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      if (O.listing) {
         h2_color::printf("", "   CASE%d. ", ++cs);
         h2_color::printf("bold,cyan", "%s", c->name);
         h2_color::printf("", " @ %s:%d\n", basename((char*)c->file), c->line);
      }
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      if (O.listing) return;
      h2_report::on_case_endup(s, c);
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         if (O.verbose) h2_color::printf("", "[%3d%%] (%s // %s): %s at %s:%d\n", percentage, s->name, c->name, CSS[c->status], basename((char*)c->file), c->line);
         break;
      case h2_case::FILTED: break;
      case h2_case::PASSED:
         if (O.verbose) {
            h2_color::printf("", "[%3d%%] ", percentage);
            h2_color::printf("light blue", "(%s // %s): Passed - %lld ms\n", s->name, c->name, tc);
         } else if (!O.debug)
            h2_color::printf("", "\r[%3d%%] (%d/%d) ", percentage, done_cases, total_cases);
         break;
      case h2_case::FAILED:
         h2_color::printf("", "\r[%3d%%] ", percentage);
         h2_color::printf("bold,purple", "(%s // %s): Failed at %s:%d\n", s->name, c->name, basename((char*)c->file), c->line);
         if (c->fails) c->fails->foreach ([](h2_fail* fail, int subling_index, int child_index) { fail->print(subling_index, child_index); });
         h2_color::printf("", "\n");
         break;
      }
   }
};

struct h2_report_junit : h2_report {
   FILE* f;
   void on_task_start(int cases) override
   {
      h2_report::on_task_start(cases);
      f = fopen(O.junit, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   };
   void on_suite_start(h2_suite* s) override
   {
      h2_report::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->status_stats[h2_case::FAILED], s->name, s->status_stats[h2_case::TODOED] + s->status_stats[h2_case::FILTED], s->sorted_cases.count(), 0, "");
   }

   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, CSS[c->status], tc / 1000.0);

      if (c->status == h2_case::FAILED) {
         fprintf(f, "<failure message=\"%s:%d:", c->file, c->line);
         if (c->fails) c->fails->foreach ([=](h2_fail* fail, int subling_index, int child_index) {fprintf(f, "{newline}"); fail->print(f); });
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "<system-out></system-out><system-err></system-err>\n");
      fprintf(f, "</testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_task_endup(int status_stats[8], int round) override
   {
      h2_report::on_task_endup(status_stats, round);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_report_tap : h2_report {
   /* */
};

h2_inline void h2_reports::initialize()
{
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   reports.push_back(&console_report);
   if (strlen(O.junit) && !O.listing) reports.push_back(&junit_report);
}

inline void h2_reports::on_task_start(int cases)
{
   for (auto report : reports) report->on_task_start(cases);
}
inline void h2_reports::on_task_endup(int status_stats[8], int round)
{
   for (auto report : reports) report->on_task_endup(status_stats, round);
}
inline void h2_reports::on_suite_start(h2_suite* s)
{
   for (auto report : reports) report->on_suite_start(s);
}
inline void h2_reports::on_suite_endup(h2_suite* s)
{
   for (auto report : reports) report->on_suite_endup(s);
}
inline void h2_reports::on_case_start(h2_suite* s, h2_case* c)
{
   for (auto report : reports) report->on_case_start(s, c);
}
inline void h2_reports::on_case_endup(h2_suite* s, h2_case* c)
{
   for (auto report : reports) report->on_case_endup(s, c);
}

struct h2__socket {
   h2_singleton(h2__socket);
   h2_list socks;

   static bool is_block(int sockfd)
   {
#ifdef _WIN32
      return true;
#else
      return !(fcntl(sockfd, F_GETFL) & O_NONBLOCK);
#endif
   }
   static bool set_block(int sockfd, bool block)
   {
#ifdef _WIN32
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
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);

      do {
         h2_list_for_each_entry(p, &sock->incoming, h2_packet, x) if (h2_wildcard_match(p->to.c_str(), local))
         {
            p->x.out();
            return p;
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
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      sock->sockets.push_back({fd, c, tcp->from.c_str()});
      if (tcp->data.length())
         sock->incoming.push(&tcp->x);
      else
         delete tcp;

      return fd;
   }

   static int connect(int socket, const struct sockaddr* address, socklen_t address_len)
   {
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      sock->sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      if (tcp->data.length())
         sock->incoming.push(&tcp->x);
      else
         delete tcp;
      return 0;
   }

   static ssize_t send(int socket, const void* buffer, size_t length, int flags)
   {
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      if (sock) sock->put_outgoing(socket, (const char*)buffer, length);
      return length;
   }
#ifndef _WIN32
   static ssize_t sendmsg(int socket, const struct msghdr* message, int flags)
   {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
#endif
   static ssize_t sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len)
   {
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      if (sock) sock->put_outgoing(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return length;
   }
   static ssize_t recv(int socket, void* buffer, size_t length, int flags)
   {
      ssize_t ret = 0;
      h2_packet* tcp = read_incoming(socket);
      if (tcp) {
         ret = tcp->data.copy((char*)buffer, tcp->data.length(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len)
   {
      ssize_t ret = 0;
      h2_packet* udp = read_incoming(socket);
      if (udp) {
         ret = udp->data.copy((char*)buffer, udp->data.length(), 0);
         iport_parse(udp->from.c_str(), (struct sockaddr_in*)address);
         *address_len = sizeof(struct sockaddr_in);
         delete udp;
      }
      return ret;
   }
#ifndef _WIN32
   static ssize_t recvmsg(int socket, struct msghdr* message, int flags)
   {
      return recvfrom(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, &message->msg_namelen);
   }
#endif
};

h2_inline h2_sock::h2_sock()
{
   stubs.add((void*)::sendto, (void*)h2__socket::sendto);
   stubs.add((void*)::recvfrom, (void*)h2__socket::recvfrom);
#ifndef _WIN32
   stubs.add((void*)::sendmsg, (void*)h2__socket::sendmsg);
   stubs.add((void*)::recvmsg, (void*)h2__socket::recvmsg);
#endif
   stubs.add((void*)::send, (void*)h2__socket::send);
   stubs.add((void*)::recv, (void*)h2__socket::recv);
   stubs.add((void*)::accept, (void*)h2__socket::accept);
   stubs.add((void*)::connect, (void*)h2__socket::connect);
   strcpy(last_to, "0.0.0.0:0");
}

h2_inline h2_sock::~h2_sock()
{
   x.out(), y.out();
   stubs.clear();
}

h2_inline void h2_sock::put_outgoing(const char* from, const char* to, const char* data, size_t size)
{
   strcpy(last_to, to);
   outgoing.push_back(&(new h2_packet(from, to, data, size))->x);
}

h2_inline void h2_sock::put_outgoing(int fd, const char* data, size_t size)
{
   char from[128] = "", to[128] = "";
   for (auto& t : sockets)
      if (t.fd == fd) {
         strcpy(from, t.from.c_str());
         strcpy(to, t.to.c_str());
         break;
      }
   put_outgoing(from, to, data, size);
}

h2_inline void h2_sock::put_incoming(const char* from, const char* to, const char* data, size_t size)
{
   incoming.push_back(&(new h2_packet(from ? from : last_to, to, data, size))->x);
}

h2_inline h2_packet* h2_socket::start_and_fetch()
{
   if (!h2_task::I().current_case) return nullptr;

   h2_sock* sock = h2_task::I().current_case->sock;
   if (!sock) {
      sock = h2_task::I().current_case->sock = new h2_sock();
      h2__socket::I().socks.push(&sock->y);
   }
   return h2_list_pop_entry(&sock->outgoing, h2_packet, x);
}

h2_inline void h2_socket::inject_received(const void* packet, size_t size, const char* from, const char* to)
{
   h2_sock* sock = h2_list_top_entry(&h2__socket::I().socks, h2_sock, y);
   if (sock) sock->put_incoming(from, to, (const char*)packet, size);
}

struct h2__stdio {
   h2_singleton(h2__stdio);
   h2_stubs stubs;
   h2_string* buffer;
   bool stdout_capturable, stderr_capturable, syslog_capturable;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      if (!((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr))))
         return h2_libc::write(fd, buf, count);
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

   h2__stdio() : stdout_capturable(false), stderr_capturable(false), syslog_capturable(false)
   {
#ifndef _WIN32
      stubs.add((void*)::write, (void*)write);
#endif
#if defined __APPLE__ || defined _WIN32
      stubs.add((void*)::printf, (void*)printf);
      stubs.add((void*)::vprintf, (void*)vprintf);
      stubs.add((void*)::putchar, (void*)putchar);
      stubs.add((void*)::puts, (void*)puts);
      stubs.add((void*)::fprintf, (void*)fprintf);
      stubs.add((void*)::vfprintf, (void*)vfprintf);
      stubs.add((void*)::fputc, (void*)fputc);
      stubs.add((void*)::putc, (void*)fputc);
      stubs.add((void*)::fputs, (void*)fputs);
      stubs.add((void*)::fwrite, (void*)fwrite);
#endif
#ifndef _WIN32
      stubs.add((void*)::syslog, (void*)syslog);
      stubs.add((void*)::vsyslog, (void*)vsyslog);
#endif
   }

   const char* start_capture(bool _stdout, bool _stderr, bool _syslog)
   {
      stdout_capturable = _stdout;
      stderr_capturable = _stderr;
      syslog_capturable = _syslog;
      buffer->clear();
      return buffer->c_str();
   }

   const char* stop_capture()
   {
      stdout_capturable = stderr_capturable = syslog_capturable = false;
      buffer->push_back('\0');
      return buffer->c_str();
   }
};

h2_inline void h2_stdio::initialize()
{
   ::setbuf(stdout, 0);  // unbuffered
   h2__stdio::I().buffer = new h2_string();
}

h2_inline const char* h2_stdio::capture_cout(const char* type)
{
   if (!type) return h2__stdio::I().stop_capture();
   if (!strlen(type)) return h2__stdio::I().start_capture(true, true, true);
   return h2__stdio::I().start_capture(strcasestr(type, "out"), strcasestr(type, "err"), strcasestr(type, "syslog"));
}
// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_thunk {
#if defined __x86_64__ || defined _M_X64
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined __i386__ || defined _M_IX86
   unsigned char saved_code[sizeof(void*) + 1];
#else

#endif

   void* save(void* befp)
   {
#ifdef _WIN32
      DWORD t;
      if (!VirtualProtect(befp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t)) {  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
         ::printf("STUB: VirtualProtect PAGE_EXECUTE_READWRITE failed %d\n", GetLastError());
         return nullptr;
      }
#else
      uintptr_t pagesize = (uintptr_t)h2_page_size();
      uintptr_t start = reinterpret_cast<uintptr_t>(befp);
      uintptr_t pagestart = start & (~(pagesize - 1));
      int pagecount = ::ceil((start + sizeof(saved_code) - pagestart) / (double)pagesize);

      if (mprotect(reinterpret_cast<void*>(pagestart), pagecount * pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         ::printf("STUB: mprotect PROT_READ | PROT_WRITE | PROT_EXEC failed %s\n", strerror(errno));
         return nullptr;
      }
#endif
      memcpy(saved_code, befp, sizeof(saved_code));
      return befp;
   }

   void set(void* befp, void* tofp)
   {
      unsigned char* I = reinterpret_cast<unsigned char*>(befp);
      ptrdiff_t delta = (unsigned char*)tofp - (unsigned char*)befp - 5;

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
      if (delta < INT_MIN || INT_MAX < delta) {  //x86_64 asm("movq $tofp, %rax; jmpq %rax")
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &tofp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {  //i386 asm("jmp offset")
         unsigned char C[] = {0xE9, 0, 0, 0, 0};
         *(int32_t*)(&C[1]) = delta;
         memcpy(I, C, sizeof(C));
      }
#endif
   }

   void reset(void* befp) { memcpy(befp, saved_code, sizeof(saved_code)); }
};

struct h2_native : h2_thunk {
   h2_list x;
   void* befp;
   int refcount;
   h2_native(void* befp_) : befp(befp_), refcount(0) { save(befp); }
   void restore() { reset(befp); }
};

struct h2_natives {
   h2_singleton(h2_natives);
   h2_list natives;
   h2_native* get(void* befp)
   {
      h2_list_for_each_entry(p, &natives, h2_native, x) if (p->befp == befp) return p;
      return nullptr;
   }
   void dec(void* befp)
   {
      h2_native* native = get(befp);
      if (native)
         if (--native->refcount == 0) {
            native->x.out();
            h2_libc::free(native);
         }
   }
   void inc(void* befp)
   {
      h2_native* native = get(befp);
      if (!native) {
         native = new (h2_libc::malloc(sizeof(h2_native))) h2_native(befp);
         natives.push(&native->x);
      }
      native->refcount++;
   }
};

struct h2_stub : h2_thunk, h2_libc {
   h2_list x;
   void *befp, *tofp;
   const char* file;
   int line;

   h2_stub(void* befp_, const char* file_ = nullptr, int line_ = 0) : file(file_), line(line_)
   {
      h2_natives::I().inc(befp_);
      befp = save(befp_);
   }
   ~h2_stub() { h2_natives::I().dec(befp); }
   void set(void* tofp_)
   {
      tofp = tofp_;
      h2_thunk::set(befp, tofp);
   }
   void reset()
   {
      if (befp) h2_thunk::reset(befp);
   }
};

h2_inline bool h2_stubs::add(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line)
{
   h2_stub* stub = nullptr;
   h2_list_for_each_entry(p, &stubs, h2_stub, x) if (p->befp == befp)
   {
      stub = p;
      break;
   }
   if (!stub) {
      stub = new h2_stub(befp, file, line);
      stubs.push(&stub->x);
   }
   stub->set(tofp);
   return true;
}
h2_inline void h2_stubs::clear()
{
   h2_list_for_each_entry(p, &stubs, h2_stub, x)
   {
      p->reset();
      p->x.out();
      delete p;
   }
}

h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* befp_)
{
   befp = ((h2_thunk*)current)->save(befp_);
   h2_native* native = h2_natives::I().get(befp);
   if (native) native->restore();
}
h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   if (befp) ((h2_thunk*)current)->reset(befp);
}

h2_inline h2_suite::h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const char* file_, int line_)
  : name(name_), file(file_), line(line_), seq(0), status_stats{0}, jumpable(false), test_code(test_code_)
{
   h2_directory::I().registered_suites.push_back(&registered);
}

h2_inline void h2_suite::cleanup()
{
   stubs.clear();
   mocks.clear();
}

h2_inline void h2_suite::enumerate()
{
   /* enumerate case by static local h2_case variable inside of h2_suite_test_code() */
   test_code(this, nullptr);
}

h2_inline void h2_suite::execute(h2_case* c)
{
   c->prev_setup();
   test_code(this, c); /* include setup(); c->post_setup() and c->prev_cleanup(); cleanup() */
   c->post_cleanup();
}

inline h2_task::h2_task() : state(0), round(0), status_stats{0}, current_suite(nullptr), current_case(nullptr) {}

inline int h2_task::execute()
{
   state = 10;
   reports.initialize();
   h2_heap::initialize();
   h2_heap::dohook();
   h2_stdio::initialize();
   h2_dns::initialize();
   h2_patch::initialize();

   state = 20;
   for (auto& setup : global_setups) setup();
   h2_list_for_each_entry(s, &h2_directory::I().registered_suites, h2_suite, registered)
   {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& teardown : global_suite_teardowns) teardown();
   }
   int cases = h2_directory::sort();

   reports.on_task_start(cases);
   for (round = 0; round < O.times && !status_stats[h2::h2_case::FAILED]; ++round) {
      h2_list_for_each_entry(s, &h2_directory::I().sorted_suites, h2_suite, sorted)
      {
         current_suite = s;
         reports.on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry(c, &s->sorted_cases, h2_case, sorted)
         {
            if (0 < O.breakable && O.breakable <= status_stats[h2_case::FAILED]) break;
            current_case = c;
            if (O.filter(s->name, c->name, c->file)) c->status = h2_case::FILTED;
            reports.on_case_start(s, c);
            if (h2_case::INITED == c->status && !O.listing) {
               for (auto& setup : global_case_setups) setup();
               s->execute(c);
               for (auto& teardown : global_case_teardowns) teardown();
            }
            reports.on_case_endup(s, c);
            status_stats[c->status] += 1;
            s->status_stats[c->status] += 1;
         }
         s->cleanup();
         for (auto& teardown : global_suite_teardowns) teardown();
         reports.on_suite_endup(s);
      }
   }
   reports.on_task_endup(status_stats, round);
   for (auto& teardown : global_teardowns) teardown();

   state = 30;
   h2_heap::unhook();
   stubs.clear();
   if (status_stats[h2_case::FAILED] == 0) h2_directory::drop_last_order();

   state = 40;
   return status_stats[h2::h2_case::FAILED];
}
}  // namespace h2

#if defined _WIN32
#   define h2_weak_attribute
#else
#   define h2_weak_attribute __attribute__((weak))
#endif

h2_weak_attribute int main(int argc, const char** argv)
{
   h2::h2_option::I().parse(argc, argv);
   return h2::h2_task::I().execute();
}
#endif
