/* v5.4  2020-05-31 10:24:08 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */
#ifndef __H2UNIT_HPP__
#define __H2UNIT_HPP__
#define H2UNIT_VERSION "5.4"
#ifndef ___H2UNIT_H___
#define ___H2UNIT_H___

#include <cstdio>        /* printf */
#include <cstdlib>       /* malloc */
#include <cstring>       /* strcpy, memcpy */
#include <cstddef>       /* ptrdiff_t */
#include <climits>       /* INT_MAX */
#include <cmath>         /* fabs */
#include <csetjmp>       /* setjmp, longjmp */
#include <sstream>       /* std::basic_ostringstream */
#include <string>        /* std::string */
#include <vector>        /* std::vector */
#include <deque>         /* std::deque */
#include <array>         /* std::array */
#include <list>          /* std::list */
#include <forward_list>  /* std::forward_list */
#include <set>           /* std::set */
#include <unordered_set> /* std::unordered_set */
#include <map>           /* std::map */
#include <unordered_map> /* std::unordered_map */
#include <tuple>         /* std::tuple */
#include <functional>    /* std::function */
#include <utility>       /* std::forward_as_tuple, std::pair */
#include <type_traits>   /* std::true_type */

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
#define h2_list_for_each_entry(p, head, type, link) for (int i = 0; i == 0; ++i) for (type* p = h2_list_entry((head)->next, type, link), *t = h2_list_entry(p->link.next, type, link); &p->link != (head); p = t, t = h2_list_entry(t->link.next, type, link), ++i)

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
   bool verbose, colorfull, shuffle, memory_check, listing;
   int breakable, rounds;
   char junit[256], args[256];
   std::vector<const char*> includes, excludes;

   h2_option() : debug(nullptr), verbose(false), colorfull(true), shuffle(false), memory_check(true), listing(false), breakable(0), rounds(1), junit{0} {}

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
/* clang-format on */

template <typename T>
h2_string h2_stringify(T a)
{
   h2_ostringstream os;
   os << std::boolalpha;
   os << a;
   return h2_string(os.str().c_str());
}

template <typename A, size_t N>
h2_string h2_stringify(const std::array<A, N> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::vector<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::deque<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::list<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::forward_list<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::set<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::multiset<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::unordered_set<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::unordered_multiset<T> a) { return ""; }

template <>
inline h2_string h2_stringify(std::nullptr_t a) { return "nullptr"; }

struct h2_line : public h2_vector<h2_string> {
   h2_line() {}
   h2_line(h2_string a) : h2_vector<h2_string>({a}) {}
   h2_line(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   int width() const;
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

   int max_width() const;
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
   h2_string no, explain, user_explain;

   //    expression     expection      represent       value
   //     Ge(var)        Ge(5)          5               5
   //     We(var)        We("abc")      "abc"           abc

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

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const char* file_ = nullptr, int line_ = 0, const char* func_ = nullptr, const char* format = "", ...);
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_unexpect : h2_fail {
   h2_string e_represent, a_represent;
   h2_string expection;
   h2_fail_unexpect(h2_string e_represent_ = "", h2_string a_represent_ = "", h2_string expection_ = "", h2_string explain_ = "", const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e_represent(e_represent_), a_represent(a_represent_), expection(expection_) { explain = explain_; }

   void print_OK1(h2_line& line);
   void print_OK2(h2_line& line);
   void print_JE(h2_line& line);
   void print_Inner(h2_line& line);
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_string& expection_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("\"" + e_value_ + "\"", "\"" + a_value_ + "\"", expection_, "", file_, line_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_strfind : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("\"" + e_value_ + "\"", "\"" + a_value_ + "\"", expection_, "", file_, line_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_json : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(e_value_, a_value_, expection_, "", file_, line_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_memcmp : h2_fail_unexpect {
   static constexpr const int npr_1b = 4;
   static constexpr const int npr_8b = 16;
   static constexpr const int npr_16b = 8;
   static constexpr const int npr_32b = 4;
   static constexpr const int npr_64b = 2;
   h2_vector<unsigned char> e_value, a_value;
   const int width, nbits;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, const h2_string& expection_, h2_string a_represent_, h2_string explain_ = "", const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("", a_represent_, expection_, explain_, file_, line_), e_value(e_value_, e_value_ + (nbits_ + 7) / 8), a_value(a_value_, a_value_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}
   void print(int subling_index = 0, int child_index = 0) override;
   void print_bits(h2_lines& e_lines, h2_lines& a_lines);
   void print_bytes(h2_lines& e_lines, h2_lines& a_lines);
   void print_int16(h2_lines& e_lines, h2_lines& a_lines);
   void print_int32(h2_lines& e_lines, h2_lines& a_lines);
   void print_int64(h2_lines& e_lines, h2_lines& a_lines);
};

struct h2_fail_memory : h2_fail {
   const void* ptr;
   const int size;
   const h2_backtrace bt_allocate, bt_release;

   h2_fail_memory(const void* ptr_, const int size_, h2_backtrace bt_allocate_, h2_backtrace bt_release_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, int size_, h2_backtrace bt_allocate_, const char* where_, const char* file_, int line_)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), where(where_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, h2_backtrace bt_allocate_, h2_backtrace bt_release_, h2_backtrace bt_double_free_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, h2_backtrace bt_allocate_, h2_backtrace bt_release_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_overflow : h2_fail_memory {
   const void* addr;                    /* 犯罪地点 */
   const char* action;                  /* 犯罪行为 */
   const h2_vector<unsigned char> spot; /* 犯罪现场 */
   const h2_backtrace bt_trample;       /* 犯罪过程 */
   h2_fail_overflow(const void* ptr_, const int size_, const void* addr_, const char* action_, h2_vector<unsigned char> spot_, h2_backtrace bt_allocate_, h2_backtrace bt_trample_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), addr(addr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_use_after_free : h2_fail_memory {
   const void* addr;          /* 犯罪地点 */
   const char* action;        /* 犯罪行为 */
   const h2_backtrace bt_use; /* 犯罪过程 */
   h2_fail_use_after_free(const void* ptr_, const void* addr_, const char* action_, h2_backtrace bt_allocate_, h2_backtrace bt_release_, h2_backtrace bt_use_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), addr(addr_), action(action_), bt_use(bt_use_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_call : h2_fail {
   h2_string e_who, e_call, a_call;
   h2_fail_call(const char* func_, const char* expect, const char* actual, const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_), e_who(func_), e_call(expect), a_call(actual) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

struct h2_fail_instantiate : h2_fail {
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   const bool why_abstract;
   h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_abstract_, const char* file_, int line_) : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why_abstract(why_abstract_) {}
   void print(int subling_index = 0, int child_index = 0) override;
};

static inline void h2_fail_g(h2_fail*, bool);

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

struct h2_heap {
   static void initialize();
   static void dohook();
   static void unhook();

   struct stack {
      static void root();
      static void push(const char* file, int line, long long limited = LLONG_MAX >> 9, const char* fill = nullptr);
      static h2_fail* pop();

      struct block : h2_once {
         block(const char* file, int line, long long limited = LLONG_MAX >> 9, const char* fill = nullptr);
         ~block();
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
         h2_fail_g(new h2_fail_instantiate(action_type, return_type, class_type, method_name, return_args, 1 == (intptr_t)o, file, line), false);
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

static inline h2_string CD(h2_string s, bool caseless = false, bool dont = false)
{
   h2_string z = s;
   if (dont) z = "!" + z;
   if (caseless) z = "~" + z;
   return z;
}

struct h2_equals_string {
   const h2_string e;
   h2_equals_string(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_equals_float {
   const long double e;
   explicit h2_equals_float(const long double _e) : e(_e) {}
   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const;
   h2_string expects(const long double a, bool caseless = false, bool dont = false) const;
};

template <typename E>
struct h2_equals {
   const E e;
   explicit h2_equals(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a == e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }

   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(h2_stringify(e), caseless, dont);
   }
};

template <>
struct h2_equals<char*> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<char* const> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<const char*> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<const char* const> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<h2_string> : h2_equals_string {
   explicit h2_equals(const h2_string e) : h2_equals_string(e) {}
};
template <>
struct h2_equals<std::string> : h2_equals_string {
   explicit h2_equals(const std::string e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<float> : h2_equals_float {
   explicit h2_equals(const float e) : h2_equals_float(e) {}
};
template <>
struct h2_equals<double> : h2_equals_float {
   explicit h2_equals(const double e) : h2_equals_float(e) {}
};
template <>
struct h2_equals<long double> : h2_equals_float {
   explicit h2_equals(const long double e) : h2_equals_float(e) {}
};

struct h2_matchee_any {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return "Any"; }
};

struct h2_matchee_null {
   const bool reverse;
   explicit h2_matchee_null(bool _reverse) : reverse(_reverse) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      bool _dont = reverse ? !dont : dont;
      if ((nullptr == (const void*)a) == !_dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify((const void*)a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return (reverse ? !dont : dont) ? "NotNull" : "IsNull"; }
};

template <bool E>
struct h2_matchee_boolean {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      bool _dont = E ? dont : !dont;
      if (((bool)a) == !_dont) return nullptr;
      return new h2_fail_unexpect("", a ? "true" : "false", expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return (E ? dont : !dont) ? "false" : "true"; }
};

template <typename E>
struct h2_matchee_ge {
   const E e;
   explicit h2_matchee_ge(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a >= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(">=" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matchee_gt {
   const E e;
   explicit h2_matchee_gt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a > e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(">" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matchee_le {
   const E e;
   explicit h2_matchee_le(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a <= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD("<=" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matchee_lt {
   const E e;
   explicit h2_matchee_lt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a < e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD("<" + h2_stringify(e), caseless, dont);
   }
};

struct h2_matchee_regex {
   const h2_string e;
   explicit h2_matchee_regex(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_wildcard {
   const h2_string e;
   explicit h2_matchee_wildcard(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_strcmp {
   const h2_string e;
   explicit h2_matchee_strcmp(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_substr {
   const h2_string substring;
   explicit h2_matchee_substr(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_startswith {
   const h2_string prefix_string;
   explicit h2_matchee_startswith(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_endswith {
   const h2_string suffix_string;
   explicit h2_matchee_endswith(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_json {
   const h2_string e;
   explicit h2_matchee_json(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_memcmp {
   const int width;
   const void* e;
   const int nbits;
   explicit h2_matchee_memcmp(const int _width, const void* _e, const int _nbits) : width(_width), e(_e), nbits(_nbits) {}
   h2_fail* matches(const void* a, bool caseless = false, bool dont = false) const;
   h2_string expects(const void* a, bool caseless = false, bool dont = false) const;
};

template <typename T>
struct h2_matcher_impl {
   virtual h2_fail* matches(T a, bool caseless = false, bool dont = false) const = 0;
   virtual h2_string expects(T a, bool caseless = false, bool dont = false) const { return ""; }
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
   h2_string expects(const T& a, bool caseless = false, bool dont = false) const { return impl->expects(a, caseless, dont); };
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
      h2_string expects(T a, bool caseless = false, bool dont = false) const override { return m.expects(a, caseless, dont); }
   };
};

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = h2_polymorphic_matcher<h2_equals<T>>(h2_equals<T>(value)); }

inline h2_matcher<const char*>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_equals_string>(h2_equals_string(value)); }
inline h2_matcher<const char*>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_equals_string>(h2_equals_string(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_equals_string>(h2_equals_string(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_equals_string>(h2_equals_string(value)); }
inline h2_matcher<std::string>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_equals_string>(h2_equals_string(value)); }
inline h2_matcher<std::string>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_equals_string>(h2_equals_string(value)); }

template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from) { return do_cast(from, std::is_convertible<M, h2_matcher<T>>{}, std::is_convertible<M, T>{}); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type) { return h2_polymorphic_matcher<h2_equals<M>>(h2_equals<M>(from)); }
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
   explicit h2_not_matches(Matcher _m) : m(_m) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      return h2_matcher_cast<A>(m).matches(a, caseless, !dont);
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return h2_matcher_cast<A>(m).expects(a, caseless, !dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_and_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_and_matches(Matcher1 _m1, Matcher2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m1).matches(a, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m2).matches(a, caseless, false));
      if (!fail == !dont) return nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
      }
      return fail;
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<A>(m1).expects(a, caseless, false);
      h2_string s2 = h2_matcher_cast<A>(m2).expects(a, caseless, false);
      return CD(s1 + "&&" + s1, caseless, dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_or_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_or_matches(Matcher1 _m1, Matcher2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, caseless, false);
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, caseless, false);
      bool result = !f1 || !f2;
      if (result == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<A>(m1).expects(a, caseless, false);
      h2_string s2 = h2_matcher_cast<A>(m2).expects(a, caseless, false);
      return CD(s1 + "||" + s1, caseless, dont);
   }
};

template <typename Matcher>
struct h2_pointee_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher _m) : m(_m) {}

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
   template <typename A>
   h2_string expects(const A a, bool caseless = false, bool dont = false) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).expects(*a, caseless, dont);
   }
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return m.matches(a, true, dont); }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return m.expects(a, true, dont); }
};

template <typename KMatcher, typename VMatcher>
struct h2_pair_matches {
   const KMatcher k;
   const VMatcher v;
   explicit h2_pair_matches(KMatcher k_, VMatcher v_) : k(k_), v(v_) {}

   template <typename AK, typename AV>
   operator h2_pair_matches<AK, AV>();

   template <typename AK, typename AV>
   h2_fail* matches(const std::pair<AK, AV>& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<AK>(k).matches(a.first, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<AV>(v).matches(a.second, caseless, false));
      if (!fail == !dont) return nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", "{" + h2_stringify(a.first) + ", " + h2_stringify(a.second) + "}", expects(a, caseless, dont));
      }
      return fail;
   }
   template <typename AK, typename AV>
   h2_string expects(const std::pair<AK, AV>& a, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<AK>(k).expects(a.first, caseless, false);
      h2_string s2 = h2_matcher_cast<AV>(v).expects(a.second, caseless, false);
      return CD("{" + s1 + ", " + s2 + "}", false, dont);
   }
};

template <typename KMatcher, typename VMatcher>
template <typename AK, typename AV>
inline h2_pair_matches<KMatcher, VMatcher>::operator h2_pair_matches<AK, AV>()
{
   return h2_pair_matches<AK, AV>(h2_matcher_cast<AK>(k), h2_matcher_cast<AV>(v));
}

template <typename M1, typename M2>
inline h2_pair_matches<M1, M2> Pair(const M1 m1, const M2 m2)
{
   return h2_pair_matches<M1, M2>(m1, m2);
}

#define H2_MATCHER_T2V(t_matchers)                                                                                                                          \
   template <typename T, size_t I>                                                                                                                          \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), h2_vector<h2_matcher<T>> v_matchers = {}) const \
   {                                                                                                                                                        \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                                                                    \
      return t2v<T>(std::integral_constant<size_t, I + 1>(), v_matchers);                                                                                   \
   }                                                                                                                                                        \
   template <typename T>                                                                                                                                    \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, sizeof...(Matchers)>, h2_vector<h2_matcher<T>> v_matchers = {}) const { return v_matchers; }

#define H2_MATCHER_T4V(t_matchers)                                                                                                                                          \
   template <typename K, typename V, size_t I>                                                                                                                              \
   h2_vector<h2_pair_matches<K, V>> t4v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), h2_vector<h2_pair_matches<K, V>> v_matchers = {}) const \
   {                                                                                                                                                                        \
      v_matchers.push_back((h2_pair_matches<K, V>)std::get<I>(t_matchers));                                                                                                 \
      return t4v<K, V>(std::integral_constant<size_t, I + 1>(), v_matchers);                                                                                                \
   }                                                                                                                                                                        \
   template <typename K, typename V>                                                                                                                                        \
   h2_vector<h2_pair_matches<K, V>> t4v(std::integral_constant<size_t, sizeof...(Matchers)>, h2_vector<h2_pair_matches<K, V>> v_matchers = {}) const { return v_matchers; }

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
         h2_fail* fail = v_matchers[i].matches(a, caseless, false);
         if (fail) fail->no = h2_stringify(i);
         h2_fail::append_subling(fails, fail);
      }

      if (!fails == !dont) return nullptr;
      h2_fail* fail = nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont), "Should not match all");
      } else {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

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

      int c = 0;
      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, caseless, false);
         if (!fail) {
            c++;
            break;
         }
         if (fail) fail->no = h2_stringify(i);
         h2_fail::append_subling(fails, fail);
      }

      if ((0 < c) == !dont) return nullptr;
      h2_fail* fail = nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont), "Should not match any one");
      } else {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont), "Not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

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
      int c = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, caseless, false);
         if (!fail) ++c;
      }
      if ((c == 0) == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
   }

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_listof_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::vector<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::deque<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::list<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::forward_list<A> a, bool caseless = false, bool dont = false) const;

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_has_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_has_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const;
   template <typename A, size_t N>
   h2_fail* matches(const std::array<A, N> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::vector<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::deque<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::list<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::forward_list<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::set<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::multiset<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::unordered_set<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::unordered_multiset<A> a, bool caseless = false, bool dont = false) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::map<AK, AV> a, bool caseless, bool dont) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::multimap<AK, AV> a, bool caseless, bool dont) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::unordered_map<AK, AV> a, bool caseless, bool dont) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::unordered_multimap<AK, AV> a, bool caseless, bool dont) const;

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

   H2_MATCHER_T2V(t_matchers)
   H2_MATCHER_T4V(t_matchers)
};

template <typename... Matchers>
struct h2_in_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_in_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, caseless, false);
         if (!fail) ++s;
      }

      if (0 < s == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
   }
   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return "In"; }

   H2_MATCHER_T2V(t_matchers)
};

const h2_polymorphic_matcher<h2_matchee_any> _{h2_matchee_any()};
const h2_polymorphic_matcher<h2_matchee_any> Any{h2_matchee_any()};
const h2_polymorphic_matcher<h2_matchee_null> IsNull{h2_matchee_null(false)};
const h2_polymorphic_matcher<h2_matchee_null> NotNull{h2_matchee_null(true)};
const h2_polymorphic_matcher<h2_matchee_boolean<true>> IsTrue{h2_matchee_boolean<true>()};
const h2_polymorphic_matcher<h2_matchee_boolean<false>> IsFalse{h2_matchee_boolean<false>()};

template <typename E>
inline h2_polymorphic_matcher<h2_equals<E>> Eq(const E expect) { return h2_polymorphic_matcher<h2_equals<E>>(h2_equals<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_not_matches<E>> Nq(const E expect) { return h2_polymorphic_matcher<h2_not_matches<E>>(h2_not_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_ge<E>> Ge(const E expect) { return h2_polymorphic_matcher<h2_matchee_ge<E>>(h2_matchee_ge<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_gt<E>> Gt(const E expect) { return h2_polymorphic_matcher<h2_matchee_gt<E>>(h2_matchee_gt<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_le<E>> Le(const E expect) { return h2_polymorphic_matcher<h2_matchee_le<E>>(h2_matchee_le<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_lt<E>> Lt(const E expect) { return h2_polymorphic_matcher<h2_matchee_lt<E>>(h2_matchee_lt<E>(expect)); }

inline h2_polymorphic_matcher<h2_matchee_memcmp> Me(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(len ? 8 : 0, (const void*)ptr, len * 8)); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M1e(const char* str) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(0, (const void*)str, strlen(str))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M8e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(8, (const void*)ptr, len ? len * 8 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M16e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(16, (const void*)ptr, len ? len * 16 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M32e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(32, (const void*)ptr, len ? len * 32 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M64e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(64, (const void*)ptr, len ? len * 64 : strlen((const char*)ptr))); }

inline h2_polymorphic_matcher<h2_matchee_regex> Re(const h2_string& regex_pattern) { return h2_polymorphic_matcher<h2_matchee_regex>(h2_matchee_regex(regex_pattern)); }
inline h2_polymorphic_matcher<h2_matchee_wildcard> We(const h2_string& wildcard_pattern) { return h2_polymorphic_matcher<h2_matchee_wildcard>(h2_matchee_wildcard(wildcard_pattern)); }
inline h2_polymorphic_matcher<h2_matchee_strcmp> Se(const h2_string& expect) { return h2_polymorphic_matcher<h2_matchee_strcmp>(h2_matchee_strcmp(expect)); }
inline h2_polymorphic_matcher<h2_matchee_substr> Substr(const h2_string& substring) { return h2_polymorphic_matcher<h2_matchee_substr>(h2_matchee_substr(substring)); }
inline h2_polymorphic_matcher<h2_matchee_startswith> StartsWith(const h2_string& prefix_string) { return h2_polymorphic_matcher<h2_matchee_startswith>(h2_matchee_startswith(prefix_string)); }
inline h2_polymorphic_matcher<h2_matchee_endswith> EndsWith(const h2_string& suffix_string) { return h2_polymorphic_matcher<h2_matchee_endswith>(h2_matchee_endswith(suffix_string)); }
inline h2_polymorphic_matcher<h2_matchee_json> Je(const h2_string& expect) { return h2_polymorphic_matcher<h2_matchee_json>(h2_matchee_json(expect)); }

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
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>> Has(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>>(h2_has_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>> In(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>>(h2_in_matches<typename std::decay<const Matchers&>::type...>(matchers...));
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

   bool is_not_enough(/*不够*/) const { return call < least; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_overmuch(/*过多*/) const { return most < call; }

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
            if (c_array[i].is_not_enough()) h2_fail_g(fail, false);
            if (c_array[i].is_satisfied()) delete fail; /* continue; try next h2_callexp */
         } else {
            ++c_array[c_offset = i];
            if (c_array[i].is_saturated()) c_index += 1;
            break;
         }
      }
      if (-1 == c_offset) {
         h2_fail_g(new h2_fail_call(befn, "", "exceed", file, line), false);
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
   static size_t get_length();
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
   h2_string expects(h2_packet* a, bool caseless = false, bool dont = false) const {return "";}
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
   int seq;
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

   void do_fail(h2_fail* fail, bool defer);

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
   int seq;
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
      installer(h2_suite* s, h2_case* c);
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s);
      ~cleaner();
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

#define __Matches_Common(message)                                                                                   \
   template <typename A>                                                                                            \
   bool __matches(const A& a) const;                                                                                \
   template <typename A>                                                                                            \
   h2::h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const                                 \
   {                                                                                                                \
      h2::h2_fail_unexpect* fail = new h2::h2_fail_unexpect("", h2::h2_stringify(a), h2::CD("", caseless, dont)); \
      if (__matches(a) == !dont) return nullptr;                                                                    \
      if (dont) {                                                                                                   \
      } else {                                                                                                      \
         h2::h2_ostringstream osm;                                                                                  \
         osm << std::boolalpha << H2PP_REMOVE_PARENTHESES(message);                                                 \
         fail->user_explain = osm.str().c_str();                                                                    \
      }                                                                                                             \
      return fail;                                                                                                  \
   }                                                                                                                \
   template <typename A>                                                                                            \
   h2::h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return ""; }

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
         h2_fail_g(fail, false);
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
      d->fail = new h2_fail_unexpect();
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
   static bool exempt(h2_backtrace& bt);
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

static inline void h2_fail_g(h2_fail* fail, bool defer)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail(fail, defer);
}
}

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
using h2::Substr;
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
using h2::Has;
using h2::In;
using h2::Pair;
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

#define H2SUITE(name) __H2SUITE(name, H2Q(h2_suite_test))

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
#endif
