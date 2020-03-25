/* v5.0  2020-03-25 23:18:11 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */
#ifndef H2_2FILES
#define H2_2FILES
#ifndef ___H2UNIT_H___
#define ___H2UNIT_H___

#include <cstdio>      /* printf */
#include <cstdlib>     /* malloc */
#include <cstring>     /* strcpy, memcpy */
#include <cstddef>     /* ptrdiff_t */
#include <climits>     /* INT_MAX */
#include <csetjmp>     /* setjmp, longjmp */
#include <alloca.h>    /* alloca */
#include <sstream>     /* basic_ostringstream */
#include <string>      /* std::string */
#include <vector>      /* std::vector */
#include <tuple>       /* std::tuple */
#include <functional>  /* function */
#include <utility>     /* forward_as_tuple */
#include <type_traits> /* std::true_type */

#if defined _WIN32
#   include <windows.h>
#endif

#if defined __GNUC__
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#   pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#   pragma GCC diagnostic ignored "-Wparentheses"
#   pragma GCC diagnostic ignored "-Wsign-compare"
#   pragma GCC diagnostic ignored "-Wwrite-strings"
#elif defined __clang__
#   pragma clang diagnostic ignored "-Wint-to-pointer-cast"
#   pragma clang diagnostic ignored "-Wparentheses"
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wwritable-strings"
#endif

#if defined H2_1FILE
#   define h2_inline inline
#elif defined H2_2FILES
#   define h2_inline
#else
#   define h2_inline inline
#endif

namespace h2 {
/* https://www.boost.org/doc/libs/1_65_0/libs/preprocessor/doc/index.html */

#define H2PP__CAT2(_1, _2) _1##_2
#define H2PP_CAT2(_1, _2) H2PP__CAT2(_1, _2)
#define H2PP__CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define H2PP_CAT5(_1, _2, _3, _4, _5) H2PP__CAT5(_1, _2, _3, _4, _5)

#define H2PP_EAT(...)
#define H2PP_DEFER(...) __VA_ARGS__ H2PP_EAT()

#define H2PP_EVAL(...) H2PP_EVAL64(__VA_ARGS__)
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

#define H2PP_IS_PROBE(...) H2PP_2nd(__VA_ARGS__, 0, )
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

/* clang-format off */
#define H2PP_AND(x, y) H2PP_CAT2(_H2PP_AND_, H2PP_BOOL(x))(H2PP_BOOL(y))
#define _H2PP_AND_0(y) 0
#define _H2PP_AND_1(y) y

#define H2PP_OR(x, y) H2PP_CAT2(_H2PP_OR_, H2PP_BOOL(x))(H2PP_BOOL(y))
#define _H2PP_OR_0(y) y
#define _H2PP_OR_1(y) 1

#define H2PP_IF_ELSE(_Cond, _Then, _Else) H2PP_CAT2(_H2PP_IF_ELSE_, _Cond) (_Then, _Else)
#define _H2PP_IF_ELSE_1(_Then, _Else) _Then
#define _H2PP_IF_ELSE_0(_Then, _Else) _Else
/* clang-format on */

#define H2PP_IF(_Cond) H2PP_CAT2(_H2PP_IF_, H2PP_BOOL(_Cond))
#define _H2PP_IF_1(...) __VA_ARGS__
#define _H2PP_IF_0(...)

#define H2PP_IS_BEGIN_PARENTHESES(x) H2PP_IS_PROBE(_H2PP_IS_BEGIN_PARENTHESES_PROBE x)
#define _H2PP_IS_BEGIN_PARENTHESES_PROBE(...) H2PP_PROBE()

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define H2PP_0ARG(...) _H2PP_0ARG(_H2PP_HAS_COMMA(__VA_ARGS__),             \
                                  _H2PP_HAS_COMMA(_H2PP_COMMA __VA_ARGS__), \
                                  _H2PP_HAS_COMMA(__VA_ARGS__()),           \
                                  _H2PP_HAS_COMMA(_H2PP_COMMA __VA_ARGS__()))
#define _H2PP_0ARG(_1, _2, _3, _4) _H2PP_HAS_COMMA(H2PP__CAT5(_H2PP_0ARG_CASE_, _1, _2, _3, _4))
#define _H2PP_0ARG_CASE_0001 ,
#define _H2PP_HAS_COMMA(...) _H2PP_TH(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _H2PP_COMMA(...) ,

#define H2PP_NARG(...) H2PP_IF_ELSE(H2PP_0ARG(__VA_ARGS__), 0, _H2PP_NARG(__VA_ARGS__))
#define _H2PP_NARG(...) _H2PP_TH(__VA_ARGS__, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define _H2PP_TH(_250, _249, _248, _247, _246, _245, _244, _243, _242, _241, _240, _239, _238, _237, _236, _235, _234, _233, _232, _231, _230, _229, _228, _227, _226, _225, _224, _223, _222, _221, _220, _219, _218, _217, _216, _215, _214, _213, _212, _211, _210, _209, _208, _207, _206, _205, _204, _203, _202, _201, _200, _199, _198, _197, _196, _195, _194, _193, _192, _191, _190, _189, _188, _187, _186, _185, _184, _183, _182, _181, _180, _179, _178, _177, _176, _175, _174, _173, _172, _171, _170, _169, _168, _167, _166, _165, _164, _163, _162, _161, _160, _159, _158, _157, _156, _155, _154, _153, _152, _151, _150, _149, _148, _147, _146, _145, _144, _143, _142, _141, _140, _139, _138, _137, _136, _135, _134, _133, _132, _131, _130, _129, _128, _127, _126, _125, _124, _123, _122, _121, _120, _119, _118, _117, _116, _115, _114, _113, _112, _111, _110, _109, _108, _107, _106, _105, _104, _103, _102, _101, _100, _99, _98, _97, _96, _95, _94, _93, _92, _91, _90, _89, _88, _87, _86, _85, _84, _83, _82, _81, _80, _79, _78, _77, _76, _75, _74, _73, _72, _71, _70, _69, _68, _67, _66, _65, _64, _63, _62, _61, _60, _59, _58, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, _41, _40, _39, _38, _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _x, ...) _x

/* clang-format off */
#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_CAT2(_Macro, H2PP_NARG(__VA_ARGS__)) (__VA_ARGS__)

#define H2PP_HEAD(...) H2PP_CAT2(_H2PP_HEAD_, H2PP_0ARG(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_HEAD_0(_1, ...) _1
#define _H2PP_HEAD_1(...)

#define H2PP_TAIL(...) H2PP_CAT2(_H2PP_TAIL_, H2PP_0ARG(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_TAIL_0(_1, ...) __VA_ARGS__
#define _H2PP_TAIL_1(...)

#define H2PP_LAST(...) H2PP_CAT2(_H2PP_LAST_, H2PP_0ARG(__VA_ARGS__)) (__VA_ARGS__) //is empty?
#define _H2PP_LAST_1(...) // empty
#define _H2PP_LAST_0(...) H2PP_EVAL(_H2PP_LAST_(__VA_ARGS__)) //at least 1 argument
#define _H2PP_LAST_(...) H2PP_CAT2(_H2PP_LAST__, H2PP_EQ(1, H2PP_NARG(__VA_ARGS__))) (__VA_ARGS__) // one and only one?
#define _H2PP_LAST__0(...) H2PP_DEFER(_H2PP_LAST_I)()(H2PP_TAIL(__VA_ARGS__)) // shift first, and again
#define _H2PP_LAST__1(...) __VA_ARGS__ // only 1 argument
#define _H2PP_LAST_I() _H2PP_LAST_

/* clang-format on */

#define H2Q(_Prefix) H2PP_CAT5(_Prefix, _L, __LINE__, C, __COUNTER__)

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

#define h2_singleton(_Class) \
   static _Class& I() {      \
      static _Class i;       \
      return i;              \
   }

#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))
#define h2_list_for_each_entry(p, head, type, link) for (type* p = h2_list_entry((head)->next, type, link), *t = h2_list_entry(p->link.next, type, link); &p->link != (head); p = t, t = h2_list_entry(t->link.next, type, link))

#define h2_list_pop_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->pop(), type, link))
#define h2_list_top_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_first(), type, link))
#define h2_list_bottom_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_last(), type, link))

struct h2_list {
   struct h2_list *next, *prev;

   static void __add_between(h2_list* _new, h2_list* prev, h2_list* next) {
      next->prev = _new;
      _new->next = next;
      _new->prev = prev;
      prev->next = _new;
   }

   h2_list() : next(this), prev(this) {}

   void add_head(h2_list* entry) { __add_between(entry, this, this->next); }
   void add_tail(h2_list* entry) { __add_between(entry, this->prev, this); }

   void push_back(h2_list* entry) { add_tail(entry); }
   void push(h2_list* entry) { add_head(entry); }
   h2_list* pop() { return empty() ? nullptr : next->out(); }

   bool is_first(h2_list* entry) const { return next == entry; }
   bool is_last(h2_list* entry) const { return prev == entry; }

   h2_list* get_first() const { return empty() ? nullptr : next; }
   h2_list* get_last() const { return empty() ? nullptr : prev; }

   h2_list* out() {
      prev->next = next;
      next->prev = prev;
      next = prev = this;
      return this;
   }

   bool empty() const { return next == this; }
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

   const char* path;
   char args[256];
   int verbose, listing, breakable, randomize;
   bool colorable, memory_check;
   char *debug, junit[256];
   std::vector<const char*> include_patterns, exclude_patterns;

   h2_option() : verbose(0), listing(0), breakable(0), randomize(0), colorable(true), memory_check(true), debug(nullptr), junit{0} {}

   void parse(int argc_, const char** argv_);

   int isLinux() const { return 1 == os; }
   int isMAC() const { return 2 == os; }
   int isWindows() const { return 3 == os; }

   void usage();
   bool filter(const char* suitename, const char* casename, const char* filename) const;
   const char* style(const char* s) const;
};

static const h2_option& O = h2_option::I();

static inline const char* S(const char* style) { return h2_option::I().style(style); }

struct h2_libc {
   static void* malloc(size_t sz);
   static void free(void* ptr);
   static int write(FILE* stream, const void* buf, size_t nbyte);
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
   template <typename U> struct rebind { typedef h2_allocator<U> other; };
   template <typename U> h2_allocator(const h2_allocator<U>&) {}
   template <typename U> h2_allocator& operator=(const h2_allocator<U>&) { return *this; }
};

template <typename T> inline bool operator==(const h2_allocator<T>&, const h2_allocator<T>&) { return true; }
template <typename T> inline bool operator!=(const h2_allocator<T>&, const h2_allocator<T>&) { return false; }

template <typename T> using h2_vector = std::vector<T, h2_allocator<T>>;
typedef std::basic_ostringstream<char, std::char_traits<char>, h2_allocator<char>> h2_ostringstream;


struct h2_nohook { /* new derived class/struct with libc malloc/free */
   static void* operator new(std::size_t sz) { return h2_libc::malloc(sz); }
   static void operator delete(void* ptr) { h2_libc::free(ptr); }
};
struct h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
   h2_string() : basic_string() {}
   h2_string(const h2_string& __str) : basic_string(__str.c_str()) {}
   h2_string(const char* __s) : basic_string(__s) {}
   h2_string(const std::string& __s) : basic_string(__s.c_str()) {}
   h2_string(size_t __n, char __c) : basic_string(__n, __c) {}
   h2_string(const char* __s, size_t __n) : basic_string(__s, __n) {}

   h2_string& operator=(const h2_string& __str) { return assign(__str.c_str()), *this; }
   h2_string& operator=(const char* __s) { return assign(__s), *this; }
   h2_string& operator=(const std::string& __str) { return assign(__str.c_str()), *this; }
   h2_string& operator=(char __c) { return assign(1, __c), *this; }

   h2_string& operator+=(const h2_string& __str) { return append(__str.c_str()), *this; }
   h2_string& operator+=(const char* __s) { return append(__s), *this; }
   h2_string& operator+=(const std::string& __str) { return append(__str.c_str()), *this; }
   h2_string& operator+=(char __c) { return push_back(__c), *this; }

   bool equals(h2_string __str, bool caseless = false) const;
   bool contains(h2_string __substr, bool caseless = false) const;
   bool startswith(h2_string __prefix, bool caseless = false) const;
   bool endswith(h2_string __suffix, bool caseless = false) const;
   bool wildcard_match(h2_string __pattern, bool caseless = false) const;
   bool regex_match(h2_string __pattern, bool caseless = false) const;

   h2_string& tolower();
   static h2_string tolower(h2_string from) { return from.tolower(); }
   h2_string acronym(int atmost = 16) const;
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
class h2_shared_ptr : h2_nohook {
 public:
   h2_shared_ptr() : px(nullptr), pn(nullptr) {}
   explicit h2_shared_ptr(T* p) { acquire(p, nullptr); }
   h2_shared_ptr(const h2_shared_ptr& that) { acquire(that.px, that.pn); }
   ~h2_shared_ptr() { release(); }
   h2_shared_ptr& operator=(h2_shared_ptr that) {
      std::swap(px, that.px);
      std::swap(pn, that.pn);
      return *this;
   }
   operator bool() const { return pn && 0 < *pn; }
   T& operator*() const { return *px; }
   T* operator->() const { return px; }

 private:
   void acquire(T* p, long* n) {
      pn = n;
      if (p) {
         if (!pn)
            pn = new (h2_libc::malloc(sizeof(long))) long(1);
         else
            ++(*pn);
      }
      px = p;
   }
   void release() {
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

#define h2_debug(...)                                                                 \
   do {                                                                               \
      if (!O.debug) {                                                                 \
         printf("%s %s : %d = %s\n", #__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace bt(0);                                                          \
         bt.print();                                                                  \
      }                                                                               \
   } while (0)

struct tinyexpr {
   static double eval(const char* expression, int* error);
};

struct h2_json_exporter {
   static bool match(const h2_string expect, const h2_string actual);
   static int diff(const h2_string expect, const h2_string actual, int terminal_width, h2_string& str);
};

struct h2_backtrace {
   int count, shift;
   void* array[100];

   h2_backtrace() : count(0), shift(0) {}
   h2_backtrace(int shift_);

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(h2_backtrace& bt);

   bool has(void* func, int size) const;
   void print() const;
};

struct h2_fail : h2_nohook {
   h2_fail *x_next, *y_next;

   const char* file;
   int line;

   const char* func;
   int argi;

   h2_string _k, _h, _m, _u;
   h2_string e_expr, _e, a_expr, _a;
   int w_type;  // 0 is MOCK; 1 is OK(condition); 2 is OK(expect, actual); 3 is JE

   h2_fail(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1);
   virtual ~h2_fail();

   void locate(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1);
   void set_w_type(int w_type_);
   void set_e_expr(const char* e_expr_);
   void set_a_expr(const char* a_expr_);

   void kprintf(const char* format, ...);
   void hprintf(const char* format, ...);
   void eprintf(const char* format, ...);
   void aprintf(const char* format, ...);
   void mprintf(const char* format, ...);
   void uprintf(const char* format, ...);
   void mprintf() {}

   void print_locate();
   virtual void print();
   virtual void print(FILE* fp);
};

static inline void h2_append_x_fail(h2_fail*& fail, h2_fail* n) {
   if (!fail) {
      fail = n;
   } else {
      h2_fail** pp = &fail->x_next;
      while (*pp) pp = &(*pp)->x_next;
      *pp = n;
   }
}

static inline void h2_append_y_fail(h2_fail*& fail, h2_fail* n) {
   if (!fail) {
      fail = n;
   } else {
      h2_fail** pp = &fail->y_next;
      while (*pp) pp = &(*pp)->y_next;
      *pp = n;
   }
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const char* file_ = nullptr, int line_ = 0, const char* func_ = nullptr, const char* format = "", ...);
   void print();
};

struct h2_fail_unexpect : h2_fail {
   h2_fail_unexpect(const char* file_ = nullptr, int line_ = 0);
   void print_OK1();
   void print_OK2();
   void print_OK3();
   void print_MOCK();
   void print();
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const h2_string expect, actual;
   const bool caseless;
   h2_fail_strcmp(const h2_string& expect_, const h2_string& actual_, bool caseless_, const char* file_ = nullptr, int line_ = 0);
   void print();
   char* fmt_char(char c, bool eq);
};

struct h2_fail_json : h2_fail_unexpect {
   const h2_string expect, actual;
   h2_fail_json(const h2_string& expect_, const h2_string& actual_, const char* file_ = nullptr, int line_ = 0);
   void print();
};

struct h2_fail_memcmp : h2_fail_unexpect {
   h2_vector<unsigned char> expect, actual;
   h2_fail_memcmp(const unsigned char* expect_, const unsigned char* actual_, int len, const char* file_ = nullptr, int line_ = 0);
   void print();
   char* fmt_byte(unsigned char c, unsigned char t, int j, const char* style, char* p);
};

struct h2_fail_memoverflow : h2_fail {
   const unsigned char* ptr;
   const int offset;
   const unsigned char* magic;
   const h2_vector<unsigned char> spot;
   const h2_backtrace bt0, bt1;
   h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0);
   void print();
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

   h2_fail_memleak(const char* file_ = nullptr, int line_ = 0, const char* where_ = "");

   void add(void* ptr, int size, h2_backtrace& bt);
   void print();
};

struct h2_fail_doublefree : h2_fail {
   const h2_backtrace bt0, bt1;
   h2_fail_doublefree(void* ptr_, h2_backtrace& bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0);
   void print();
};

struct h2_fail_instantiate : h2_fail {
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   const bool why_abstract;
   h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_abstract_, const char* file_, int line_);
   void print();
};

struct h2_stub : h2_nohook {
   h2_list x;
   unsigned char thunk[64];
   void *befp, *tofp;
   const char* file;
   int line;

   h2_stub(void* befp_, const char* file_ = nullptr, int line_ = 0);

   void replace(void* tofp_);
   void restore();

   struct temporary_restore : h2_once {
      h2_stub* thus;
      temporary_restore(h2_stub* stub) : thus(stub) { thus->restore(); }
      ~temporary_restore() { thus->replace(thus->tofp); }
   };
};

struct h2_stubs {
   h2_list stubs;
   bool add(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line);
   void clear();
};
static inline void h2_fail_g(void* fail);

struct h2_heap {
   static void dosegv();
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

   static inline bool is_virtual(U& u) {
      return (u.v & 1) && (u.v - 1) % sizeof(void*) == 0 && (u.v - 1) / sizeof(void*) < 1000;
   }

   static inline void* get_vmfp(U& u, Class* obj) {
      void** vtable = *(void***)obj;
      return vtable[(u.v - 1) / sizeof(void*)];
   }

   static void* A(F f, const char* action_type, const char* return_type, const char* class_type, const char* method_name, const char* return_args, const char* file, int line) {
      U u{f};
      if (!is_virtual(u)) return u.p;
      Class* o = h2_constructible<Class>::O(alloca(sizeof(Class)));
      if (1 == (intptr_t)o || 2 == (intptr_t)o)
         h2_fail_g(new h2_fail_instantiate(action_type, return_type, class_type, method_name, return_args, 1 == (intptr_t)o, file, line));
      return get_vmfp(u, o);
   }

   template <typename Derived>
   static void* A(F f, Derived obj) {
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
   h2_matcher(T value);
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
   struct internal_impl : h2_matcher_impl<T>, h2_nohook {
      const Matches m;
      explicit internal_impl(const Matches& matches_) : m(matches_) {}
      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override { return m.matches(a, caseless, dont); }
   };
};

template <typename A, typename E>
inline h2_fail* h2_common_unexpect(const A& a, const E& e, bool dont, const char* op) {
   h2_ostringstream ose, osa;
   ose << std::boolalpha << e;
   osa << std::boolalpha << a;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("%s", ose.str().c_str());
   fail->aprintf("%s", osa.str().c_str());
   if (dont)
      fail->mprintf("shoud not %s", op);
   else
      fail->mprintf("not %s", op);

   return fail;
}

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
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if (r) dont = !dont;
      if ((a == e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, "equals");
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

   struct internal_impl : h2_matcher_impl<T>, h2_nohook {
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}

      // Delegate the matching logic to the source h2_matcher.
      h2_fail* matches(T a, bool caseless, bool dont) const override {
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
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      return h2_matcher_cast<A>(m).matches(a, caseless, !dont);
   }
};

template <typename M1, typename M2>
struct h2_and_matches {
   const M1 m1;
   const M2 m2;
   explicit h2_and_matches(M1 _m1, M2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      h2_fail* fail = nullptr;
      h2_append_y_fail(fail, h2_matcher_cast<A>(m1).matches(a, caseless, false));
      h2_append_y_fail(fail, h2_matcher_cast<A>(m2).matches(a, caseless, false));
      if (!fail == !dont) return nullptr;
      if (dont) {
         fail = new h2_fail_unexpect();
         fail->mprintf("should not both match");
      } else {
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
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, caseless, false);
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, caseless, false);
      bool result = !f1 || !f2;
      if (result == !dont) return nullptr;
      h2_fail* fail = new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not match any");
      else
         fail->mprintf("not match any");

      return fail;
   }
};

struct h2_any_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
};

struct h2_null_matches {
   const bool r;
   explicit h2_null_matches(bool _r = false) : r(_r) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if (r) dont = !dont;
      if ((nullptr == (const void*)a) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("%p", (const void*)a);
      if (dont)
         fail->mprintf("should not null");
      else
         fail->mprintf("is not null");
      return fail;
   }
};

struct h2_boolean_matches {
   const bool e;
   explicit h2_boolean_matches(bool _e) : e(_e) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((e == (bool)a) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf(a ? "true" : "false");
      if (dont)
         fail->mprintf("should not %s", e ? "true" : "false");
      else
         fail->mprintf("is not %s", e ? "true" : "false");
      return fail;
   }
};

template <typename E>
struct h2_ge_matches {
   const E e;
   explicit h2_ge_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a >= e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, ">=");
   }
};

template <typename E>
struct h2_gt_matches {
   const E e;
   explicit h2_gt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a > e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, ">");
   }
};

template <typename E>
struct h2_le_matches {
   const E e;
   explicit h2_le_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a <= e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, "<=");
   }
};

template <typename E>
struct h2_lt_matches {
   const E e;
   explicit h2_lt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a < e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, "<");
   }
};

struct h2_memcmp_matches {
   const void* e;
   const int size;
   explicit h2_memcmp_matches(const void* _e, const int _size) : e(_e), size(_size) {}
   h2_fail* matches(const void* a, bool caseless = false, bool dont = false) const;
};

template <typename Matcher>
struct h2_pointee_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher matcher_) : m(matcher_) {}

   template <typename SmartPointer>
   struct PointeeOf { typedef typename SmartPointer::element_type type; };
   template <typename T>
   struct PointeeOf<T*> { typedef T type; };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
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

#define H2_MATCHER_T2V(t_matchers)                                                                                                                            \
   template <typename T, size_t I>                                                                                                                            \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), h2_vector<h2_matcher<T>> v_matchers = {}) const { \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                                                                      \
      return t2v<T>(std::integral_constant<size_t, I + 1>(), v_matchers);                                                                                     \
   }                                                                                                                                                          \
   template <typename T>                                                                                                                                      \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, sizeof...(Matchers)>, h2_vector<h2_matcher<T>> v_matchers = {}) const { return v_matchers; }

template <typename... Matchers>
struct h2_allof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<A, 0>();

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* f = v_matchers[i].matches(a, caseless, false);
         if (f) f->kprintf(" %d. ", i);
         h2_append_y_fail(fails, f);
      }

      if (!fails == !dont) return nullptr;

      h2_fail* fail = new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not matches all of matchers");
      else {
         fail->mprintf("should matches all of matchers");
         h2_append_x_fail(fail, fails);
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
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* f = v_matchers[i].matches(a, caseless, false);
         if (!f) s++;
         if (f) f->kprintf(" %d. ", i);
         h2_append_y_fail(fails, f);
      }

      if ((0 < s) == !dont) return nullptr;

      h2_fail* fail = (h2_fail*)new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not matches any of matchers");
      else {
         fail->mprintf("not matches any of matchers");
         h2_append_x_fail(fail, fails);
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
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* f = m.matches(a, caseless, false);
         if (!f) ++s;
      }

      if ((s == 0) == !dont) return nullptr;

      h2_fail_unexpect* fail = new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not matches none of matcher");
      else
         fail->mprintf("not matches none of matcher");
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_listof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<decltype(a[0]), 0>();

      h2_fail* fail = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i)
         h2_append_y_fail(fail, v_matchers[i].matches(a[i], caseless, dont));

      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

const h2_polymorphic_matcher<h2_any_matches> _{h2_any_matches()};
const h2_polymorphic_matcher<h2_any_matches> Any{h2_any_matches()};
const h2_polymorphic_matcher<h2_null_matches> IsNull{h2_null_matches()};
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

inline h2_polymorphic_matcher<h2_memcmp_matches> Me(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(ptr, len)); }
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
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> operator~(M m) { return CaseLess(m); }

template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher m) { return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(m)); }
template <typename Matches>
inline h2_polymorphic_matcher<h2_not_matches<h2_polymorphic_matcher<Matches>>> operator!(const h2_polymorphic_matcher<Matches>& m) { return Not(m); }

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> AllOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>>(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> AnyOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>>(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> NoneOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>>(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2) {
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const M2& m2) {
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator&&(const M1& m1, const h2_polymorphic_matcher<M2>& m2) {
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2) {
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const M2& m2) {
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator||(const M1& m1, const h2_polymorphic_matcher<M2>& m2) {
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

   h2_fail* check();
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

   Return operator()(Class* that, Args... args) {
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

   void operator()(Class* that, Args... args) {
      if (_f2)
         _f2(that, args...);
      else if (_f1)
         _f1(args...);
   }
};

struct h2_mock : h2_nohook {
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

   h2_fail* times_check() {
      h2_fail* fail = nullptr;
      for (auto& c : c_array) h2_append_y_fail(fail, c.check());
      if (fail) fail->locate(file, line, befn);
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
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args, const char* file, int line, const char* func) {
      h2_fail* fail = h2_tuple_match<N - 1>::matches(matchers, args, file, line, func);
      h2_fail* f = std::get<N - 1>(matchers).matches(std::get<N - 1>(args));
      if (f) f->locate(file, line, func, N - 1);
      h2_append_x_fail(fail, f);
      return fail;
   }
};

template <>
struct h2_tuple_match<0> {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args, const char* file, int line, const char* func) {
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

   static Return normal_function_stub(Args... args) {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](nullptr, std::move(args)...);
   }

   static Return member_function_stub(Class* that, Args... args) {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](that, std::move(args)...);
   }

   h2_fail* matches(matcher_tuple& matchers, argument_tuple& args) {
      if (1 == std::tuple_size<argument_tuple>::value) return nullptr;
      return h2_tuple_match<std::tuple_size<argument_tuple>::value>::matches(matchers, args, file, line, befn);
   }

   int matches(Args... args) {
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
      if (-1 == c_offset)
         h2_fail_g(new h2_fail_normal(file, line, befn, "unexpect call"));

      return c_offset;
   }

   void reset() override { c_array.clear(), m_array.clear(), r_array.clear(), c_index = 0; }

 public:
   static h2_mocker& I(void* befp = nullptr, const char* befn = nullptr, const char* file = nullptr, int line = 0) {
      static h2_mocker* i = nullptr;
      if (!i) i = new h2_mocker(befp, befn, file, line);
      if (befp && file) {
         i->reset();
         h2_mock_g(i);
      }
      return *i;
   }

   h2_mocker& once(MATCHER_Any_0_1_2_3_4_5_6_7_8_9) {
      c_array.push_back(h2_callexp(1, 1));
      m_array.push_back(std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& twice(MATCHER_Any_0_1_2_3_4_5_6_7_8_9) {
      c_array.push_back(h2_callexp(2, 2));
      m_array.push_back(std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& times(int count) {
      c_array.push_back(h2_callexp(count, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& any(MATCHER_Any_0_1_2_3_4_5_6_7_8_9) {
      c_array.push_back(h2_callexp(0, INT_MAX));
      m_array.push_back(std::forward_as_tuple(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atleast(int count) {
      c_array.push_back(h2_callexp(count, INT_MAX));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atmost(int count) {
      c_array.push_back(h2_callexp(0, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& between(int left, int right) {
      c_array.push_back(h2_callexp(left, right));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& with(MATCHER_Any_0_1_2_3_4_5_6_7_8_9) {
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

   h2_mocker& returns(h2_routine<Class, Return(Args...)> r) {
      if (!r_array.empty()) r_array.back() = r;
      return *this;
   }

   h2_mocker& does(std::function<Return(Args...)> f) {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return *this;
   }

   h2_mocker& does(std::function<Return(Class*, Args...)> f) {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return *this;
   }

   h2_mocker& operator=(std::function<Return(Args...)> f) { return does(f); }
   h2_mocker& operator=(std::function<Return(Class*, Args...)> f) { return does(f); }
};

struct h2_mocks {
   h2_list s;
   bool add(h2_mock* mock);
   h2_fail* clear();
};

struct h2_stdio_exporter {
   static void capture_cout(char* buffer, int size = 1024 * 1024);
   static const char* capture_cout();
};

struct h2_dns : h2_nohook {
   h2_list x, y;
   const char* hostname;
   int count;
   char array[32][128];

   h2_dns(const char* hostname_) : hostname(hostname_) {}
};

struct h2_dnses {
   h2_list s;
   void add(h2_dns* dns);
   void clear();
};

struct h2_packet : h2_nohook {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(data_, size_){};
   bool match(const char* from_pattern, const char* to_pattern);
   bool can_recv(const char* local_iport);
};

struct h2_sock : h2_nohook {
   h2_list x, y;

   h2_stub sendto_stub;
   h2_stub recvfrom_stub;
   h2_stub sendmsg_stub;
   h2_stub recvmsg_stub;
   h2_stub send_stub;
   h2_stub recv_stub;
   h2_stub accept_stub;
   h2_stub connect_stub;

   struct socket {
      int fd;
      h2_string from, to;
      socket(int fd_, const char* from_, const char* to_) : fd(fd_), from(from_), to(to_) {}
   };

   h2_vector<socket> sockets;

   h2_sock();
   ~h2_sock();

   void put_outgoing_udp(const char* from, const char* to, const char* data, size_t size);
   void put_incoming_udp(const char* from, const char* to, const char* data, size_t size);
   void put_outgoing_tcp(int fd, const char* data, size_t size);
   void put_incoming_tcp(const char* from, const char* to, const char* data, size_t size);

   char last_to[128];
   h2_list incoming_udps;
   h2_list outgoing_udps;

   h2_list incoming_tcps;
   h2_list outgoing_tcps;
};

template <typename M1, typename M2, typename M3, typename M4>
struct h2_packet_matches {
   const M1 from;
   const M2 to;
   const M3 data;
   const M4 size;
   explicit h2_packet_matches(M1 from_, M2 to_, M3 data_, M4 size_) : from(from_), to(to_), data(data_), size(size_) {}

   h2_fail* matches(h2_packet* a, bool caseless = false, bool dont = false) const {
      h2_fail* fails = nullptr;
      h2_append_y_fail(fails, h2_matcher_cast<const char*>(from).matches(a->from.c_str(), caseless, dont));
      h2_append_y_fail(fails, h2_matcher_cast<const char*>(to).matches(a->to.c_str(), caseless, dont));
      h2_append_y_fail(fails, h2_matcher_cast<const unsigned char*>(data).matches((unsigned char*)a->data.data(), caseless, dont));
      h2_append_y_fail(fails, h2_matcher_cast<const int>(size).matches(a->data.length(), caseless, dont));
      return fails;
   }
};

template <typename M1, typename M2, typename M3, typename M4>
inline h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>> PktEq(M1 from, M2 to, M3 data, M4 size) {
   return h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>>(h2_packet_matches<M1, M2, M3, M4>(from, to, data, size));
}

struct h2_network_exporter {
   static void setaddrinfo(int count, ...);
   static h2_packet* sock_start_and_fetch();
   static void udp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to);
   static void tcp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to);
};

static constexpr const char* CSS[] = {"init", "Passed", "Failed", "TODO", "Filtered"};

struct h2_case {
   static constexpr const int INITED = 0, PASSED = 1, FAILED = 2, TODOED = 3, FILTED = 4;

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
   const char* name;
   const char* file;
   int line;
   long long seq;
   h2_stubs stubs;
   h2_mocks mocks;
   int status_stats[8];
   jmp_buf jump;
   bool jumpable;
   void (*test_code_plus)(h2_suite*, h2_case*);
   std::vector<h2_case*> case_list;
   h2_once enumerate;

   h2_suite(const char* name_, void (*p)(h2_suite*, h2_case*), const char* file_, int line_);

   std::vector<h2_case*>& cases();
   void execute(h2_case* c);

   void setup() {}
   void cleanup();

   struct installer {
      installer(h2_suite* s, h2_case* c) {
         static long long seq = INT_MAX;
         s->case_list.push_back(c);
         s->seq = c->seq = ++seq;
      }
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s) : thus(s) {}
      ~cleaner() {
         if (thus->jumpable) ::longjmp(thus->jump, 1);
      }
   };
};

struct h2_log {
   int total_cases, done_cases, percentage;
   long long tt, ts, tc;
   h2_log();
   virtual void on_task_start(int cases);
   virtual void on_task_endup(int status_stats[8]);
   virtual void on_suite_start(h2_suite* s);
   virtual void on_suite_endup(h2_suite* s);
   virtual void on_case_start(h2_suite* s, h2_case* c);
   virtual void on_case_endup(h2_suite* s, h2_case* c);
};

struct h2_logs {
   std::vector<h2_log*> logs;
   void init();
   void on_task_start(int cases) {
      for (auto log : logs) log->on_task_start(cases);
   }
   void on_task_endup(int status_stats[8]) {
      for (auto log : logs) log->on_task_endup(status_stats);
   }
   void on_suite_start(h2_suite* s) {
      for (auto log : logs) log->on_suite_start(s);
   }
   void on_suite_endup(h2_suite* s) {
      for (auto log : logs) log->on_suite_endup(s);
   }
   void on_case_start(h2_suite* s, h2_case* c) {
      for (auto log : logs) log->on_case_start(s, c);
   }
   void on_case_endup(h2_suite* s, h2_case* c) {
      for (auto log : logs) log->on_case_endup(s, c);
   }
};

#define __Matches_Common(message)                                                     \
   template <typename A>                                                              \
   bool __matches(const A& a) const;                                                  \
   template <typename A>                                                              \
   h2::h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { \
      h2::h2_fail_unexpect* fail = new h2::h2_fail_unexpect();                        \
      if (__matches(a) == !dont) return nullptr;                                      \
      if (dont) {                                                                     \
         fail->mprintf("should not matches");                                         \
      } else {                                                                        \
         h2::h2_ostringstream osm;                                                    \
         osm << std::boolalpha << H2PP_REMOVE_PARENTHESES(message);                   \
         fail->mprintf("%s", osm.str().c_str());                                      \
      }                                                                               \
      return fail;                                                                    \
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
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1>> name(const E1 _e1) {              \
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
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>> name(const E1 _e1, const E2 _e2) {         \
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
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>> name(const E1 _e1, const E2 _e2, const E3 _e3) {    \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>>(h2_##name##_matches<E1, E2, E3>(_e1, _e2, _e3)); \
   }                                                                                                                      \
   template <typename E1, typename E2, typename E3>                                                                       \
   template <typename A>                                                                                                  \
   bool h2_##name##_matches<E1, E2, E3>::__matches(const A& a) const

#define H2MATCHER4(name, e1, e2, e3, e4, message)                                                                                        \
   template <typename E1, typename E2, typename E3, typename E4>                                                                         \
   struct h2_##name##_matches {                                                                                                          \
      const E1 e1;                                                                                                                       \
      const E2 e2;                                                                                                                       \
      const E3 e3;                                                                                                                       \
      const E4 e4;                                                                                                                       \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4) : e1(_e1), e2(_e2), e3(_e3), e4(_e4) {}   \
      __Matches_Common(message)                                                                                                          \
   };                                                                                                                                    \
   template <typename E1, typename E2, typename E3, typename E4>                                                                         \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4) { \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>>(h2_##name##_matches<E1, E2, E3, E4>(_e1, _e2, _e3, _e4));   \
   }                                                                                                                                     \
   template <typename E1, typename E2, typename E3, typename E4>                                                                         \
   template <typename A>                                                                                                                 \
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
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4, const E5 _e5) {     \
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

   std::vector<h2_suite*> suites;

   static void drop_last_order();
   static int count();
   static int sort();
   static void print_list();
   static void list_then_exit();
};

struct h2_defer_fail : h2_once {
   int w_type;
   const char *e_expr, *a_expr;
   const char* file;
   int line;
   h2_fail* fail;
   h2_ostringstream oss;

   h2_defer_fail(int w_type_, const char* e_expr_, const char* a_expr_, const char* file_, int line_)
     : w_type(w_type_), e_expr(e_expr_), a_expr(a_expr_), file(file_), line(line_), fail(nullptr) {}

   ~h2_defer_fail() {
      if (fail) {
         fail->set_w_type(w_type);
         fail->set_e_expr(e_expr);
         fail->set_a_expr(a_expr);
         fail->uprintf("%s", oss.str().c_str());
         fail->locate(file, line);
         h2_fail_g(fail);
      }
   }
};

static inline h2_ostringstream& h2_OK1(bool a, h2_defer_fail* d) {
   if (!a) {
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("false");
      d->fail = fail;
   }
   return d->oss;
}

template <typename E, typename A>
static inline h2_ostringstream& h2_OK2(E e, A a, h2_defer_fail* d) {
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   d->fail = m.matches((typename h2_decay<A>::type)a);
   return d->oss;
}

static inline h2_ostringstream& h2_JE(h2_string e, h2_string a, h2_defer_fail* d) {
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


#define _H2_ForEachv(_Func, _Args, ...) H2PP_CAT2(_H2_ForEachv_, H2PP_0ARG(__VA_ARGS__)) (_Func, _Args, __VA_ARGS__)
#define _H2_ForEachv_I() _H2_ForEachv
#define _H2_ForEachv_0(_Func, _Args, ...) _H2_Macro_I(_Func, _Args, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachv_I)()(_Func, _Args, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachv_1(...)
#define H2ForEach(_Func, _Args, ...) H2PP_EVAL(_H2_ForEachv(_Func, _Args, __VA_ARGS__))

#define _H2_ForEachy(_Func, _Args, x, ...) H2PP_CAT2(_H2_ForEachy_, H2PP_0ARG(__VA_ARGS__)) (_Func, _Args, x, __VA_ARGS__)
#define _H2_ForEachy_I() _H2_ForEachy
#define _H2_ForEachy_0(_Func, _Args, x, ...) _H2_Macro_I(_Func, _Args, x, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachy_I)()(_Func, _Args, x, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachy_1(...)
#define _H2_ForEachx(_Func, _Args, _Tupley, ...) H2PP_CAT2(_H2_ForEachx_, H2PP_0ARG(__VA_ARGS__)) (_Func, _Args, _Tupley, __VA_ARGS__)
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


#define H2List_An(...) H2PP_CAT2(_H2List_An_, H2PP_0ARG(__VA_ARGS__)) (__VA_ARGS__)
#define _H2List_An_0(_1, ...) _1
#define _H2List_An_1(...) 0

#define H2ForEach_An(...) H2PP_CAT2(_H2ForEach_An_, H2PP_0ARG(__VA_ARGS__)) (__VA_ARGS__)
#define _H2ForEach_An_1(...) 0
#define _H2ForEach_An_0(_1, ...) H2PP_CAT2(_H2ForEach_An_0_, H2PP_IS_BEGIN_PARENTHESES(_1))(_1)
#define _H2ForEach_An_0_0(_1) H2List_An(_1)
#define _H2ForEach_An_0_1(_1) H2List_An(H2PP_REMOVE_PARENTHESES(_1))

#define H2Fullmesh_Ax(...) H2ForEach_An(H2PP_HEAD(__VA_ARGS__))
#define H2Fullmesh_Ay(...) H2ForEach_An(H2PP_LAST(__VA_ARGS__))

/* clang-format on */

#define ___ForEach_CASE_Macro(name, Qc, x) CASE(name x) { Qc(x); }
#define __ForEach_CASE_Macro(...) ___ForEach_CASE_Macro(__VA_ARGS__)
#define _ForEach_CASE_Macro(Args, x) __ForEach_CASE_Macro(H2PP_REMOVE_PARENTHESES(Args), x)
#define _ForEach_CASE_Impl(name, Qc, ...)                   \
   template <typename T>                                    \
   void Qc(T x);                                            \
   H2ForEach(_ForEach_CASE_Macro, (name, Qc), __VA_ARGS__); \
   template <typename T>                                    \
   void Qc(T x)
#define H2CASES(name, ...) _ForEach_CASE_Impl(name, H2Q(f), __VA_ARGS__)

#define ___Fullmesh_CASE_Macro(name, Qc, x, y) CASE(name x, y) { Qc(x, y); }
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
   Case(name x) {                                      \
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
   Case(name x, y) {                                           \
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

struct h2_task {
   h2_singleton(h2_task);

   h2_logs logs;
   h2_stubs stubs;
   h2_mocks mocks;
   int state, status_stats[8];
   h2_suite* current_suite;
   h2_case* current_case;
   std::vector<void (*)()> global_setups, global_teardowns;
   std::vector<void (*)()> global_suite_setups, global_suite_teardowns;
   std::vector<void (*)()> global_case_setups, global_case_teardowns;

   h2_task();

   void prepare();
   void postpare();
   void execute();
};

static inline void h2_stub_g(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   if (200 <= h2_task::I().state) {
      if (h2_task::I().current_case)
         h2_task::I().current_case->stubs.add(befp, tofp, befn, tofn, file, line);
      else if (h2_task::I().current_suite)
         h2_task::I().current_suite->stubs.add(befp, tofp, befn, tofn, file, line);
      else
         h2_task::I().stubs.add(befp, tofp, befn, tofn, file, line);
   }
}

static inline void h2_mock_g(h2_mock* mock) {
   if (200 <= h2_task::I().state) {
      if (h2_task::I().current_case)
         h2_task::I().current_case->mocks.add(mock) && h2_task::I().current_case->stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      else if (h2_task::I().current_suite)
         h2_task::I().current_suite->mocks.add(mock) && h2_task::I().current_suite->stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      else
         h2_task::I().mocks.add(mock) && h2_task::I().stubs.add(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
   }
}

static inline void h2_fail_g(void* fail) {
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail((h2_fail*)fail);
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
#define JE(...) H2JE(__VA_ARGS__)
#define MOCK(...) H2MOCK(__VA_ARGS__)
#define STUB(...) H2STUB(__VA_ARGS__)
#define BLOCK(...) H2BLOCK(__VA_ARGS__)
#define DNS(...) H2DNS(__VA_ARGS__)
#define SOCK(...) H2SOCK(__VA_ARGS__)
#define UDP(...) H2UDP(__VA_ARGS__)
#define TCP(...) H2TCP(__VA_ARGS__)
#define COUT(...) H2COUT(__VA_ARGS__)

#define MATCHER(...) H2MATCHER(__VA_ARGS__)

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
using h2::Re;
using h2::We;
using h2::Je;
using h2::Se;
using h2::Contains;
using h2::StartsWith;
using h2::EndsWith;
using h2::CaseLess;
using h2::operator~;
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
   static void QP(h2::h2_suite* ___suite, h2::h2_case* ___case)

#define H2SUITE(name) __H2SUITE(name, H2Q(h2_suite_test_code_plus))

#define __H2Cleanup()                 \
   if (::setjmp(___suite->jump) == 0) \
      ___suite->jumpable = true;      \
   if (!___case)

#define H2Cleanup() __H2Cleanup()

#define __H2Case(name, todo, Qc, Q1, Q2)                              \
   static h2::h2_case Qc(name, todo, __FILE__, __LINE__);             \
   static h2::h2_suite::installer H2Q(installer)(___suite, &Qc);      \
   if (&Qc == ___case)                                                \
      for (h2::h2_suite::cleaner Q1(___suite); Q1; ___case = nullptr) \
         for (h2::h2_case::cleaner Q2(&Qc); Q2;)                      \
            if (::setjmp(Qc.jump) == 0)

#define H2Case(name) __H2Case(name, 0, H2Q(t_case), H2Q(_1), H2Q(_2))
#define H2Todo(name) __H2Case(name, 1, H2Q(t_case), H2Q(_1), H2Q(_2))

#define __H2CASE(name, todo, QR, QP)                                     \
   static void QR();                                                     \
   static void QP(h2::h2_suite* ___suite, h2::h2_case* ___case) {        \
      static h2::h2_case c(name, todo, __FILE__, __LINE__);              \
      static h2::h2_suite::installer i(___suite, &c);                    \
      if (&c == ___case)                                                 \
         for (h2::h2_case::cleaner a(&c); a;)                            \
            if (::setjmp(c.jump) == 0) QR();                             \
   }                                                                     \
   static h2::h2_suite H2Q(suite)("Anonymous", &QP, __FILE__, __LINE__); \
   static void QR()

#define H2CASE(name) __H2CASE(name, 0, H2Q(h2_case_test_code), H2Q(h2_suite_test_code_plus))
#define H2TODO(name) __H2CASE(name, 1, H2Q(h2_case_test_code), H2Q(h2_suite_test_code_plus))

#define CASES(name, ...) H2CASES(#name, __VA_ARGS__)
#define CASESS(name, ...) H2CASESS(#name, __VA_ARGS__)
#define Cases(name, ...) H2Cases(#name, __VA_ARGS__)
#define Casess(name, ...) H2Casess(#name, __VA_ARGS__)

#define __H2MOCK2(BeFunc, Signature) \
   h2::h2_mocker<__COUNTER__, __LINE__, std::false_type, Signature>::I((void*)BeFunc, #BeFunc, __FILE__, __LINE__)

#define __H2MOCK3(Class, Method, Signature) \
   h2::h2_mocker<__COUNTER__, __LINE__, Class, Signature>::I(h2::h2_mfp<Class, Signature>::A(&Class::Method, "MOCK", "", #Class, #Method, #Signature, __FILE__, __LINE__), #Class "::" #Method, __FILE__, __LINE__)

#define __H2MOCK4(Class, Method, Signature, Instance) \
   h2::h2_mocker<__COUNTER__, __LINE__, Class, Signature>::I(h2::h2_mfp<Class, Signature>::A(&Class::Method, Instance), #Class "::" #Method, __FILE__, __LINE__)

#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)

#define __H2STUB3(BeFunc, ToFunc, _1)                                                    \
   do {                                                                                  \
      h2::h2_stub_g((void*)BeFunc, (void*)ToFunc, #BeFunc, #ToFunc, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB4(Return, BeFunc, Args, Qt)                                         \
   struct {                                                                         \
      void operator=(Return(*toF) Args) {                                           \
         Return(*beF) Args = BeFunc;                                                \
         h2::h2_stub_g((void*)beF, (void*)(toF), #BeFunc, "~", __FILE__, __LINE__); \
      }                                                                             \
   } Qt;                                                                            \
   Qt = [] Args -> Return

#define __H2STUB50(Return, Class, Method, Args, Qt)                                                                                                                                                 \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that)) {                                                                                                                                                 \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Qt;                                                                                                                                                                                            \
   Qt = [](Class * that) -> Return

#define __H2STUB51(Return, Class, Method, Args, Qt)                                                                                                                                                 \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                                                                                  \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Qt;                                                                                                                                                                                            \
   Qt = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB5(Return, Class, Method, Args, Qt) \
   H2PP_IF_ELSE(H2PP_0ARG Args, __H2STUB50(Return, Class, Method, Args, Qt), __H2STUB51(Return, Class, Method, Args, Qt))

#define __H2STUB60(Return, Class, Method, Args, Instance, Qt)                                                                                    \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that)) {                                                                                              \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Qt;                                                                                                                                         \
   Qt = [](Class * that) -> Return

#define __H2STUB61(Return, Class, Method, Args, Instance, Qt)                                                                                    \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                               \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Qt;                                                                                                                                         \
   Qt = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB6(Return, Class, Method, Args, Instance, Qt) \
   H2PP_IF_ELSE(H2PP_0ARG Args, __H2STUB60(Return, Class, Method, Args, Instance, Qt), __H2STUB61(Return, Class, Method, Args, Instance, Qt))

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__, H2Q(t_stub))

#define __H2BLOCK0(Qb) for (h2::h2_heap::stack::block Qb(__FILE__, __LINE__); Qb;)
#define __H2BLOCK1(Qb, ...) for (h2::h2_heap::stack::block Qb(__FILE__, __LINE__, __VA_ARGS__); Qb;)
#define H2BLOCK(...) H2PP_IF_ELSE(H2PP_0ARG(__VA_ARGS__), __H2BLOCK0(H2Q(t_block)), __H2BLOCK1(H2Q(t_block), __VA_ARGS__))
// #define H2BLOCK(...) for (h2::h2_heap::stack::block Qb(__FILE__, __LINE__, ##__VA_ARGS__); Qb;)
// #define H2BLOCK(...) for (h2::h2_heap::stack::block Qb(__FILE__, __LINE__, __VA_OPT__(,) __VA_ARGS__); Qb;)

#define H2DNS(...) h2::h2_network_exporter::setaddrinfo(H2PP_NARG(__VA_ARGS__), __VA_ARGS__)

#define H2SOCK() h2::h2_network_exporter::sock_start_and_fetch()

#define __H2UDP2(packet, size) h2::h2_network_exporter::udp_inject_received(packet, size, nullptr, "*");
#define __H2UDP3(packet, size, from) h2::h2_network_exporter::udp_inject_received(packet, size, from, "*");
#define __H2UDP4(packet, size, from, to) h2::h2_network_exporter::udp_inject_received(packet, size, from, to);
#define H2UDP(...) H2PP_VARIADIC_CALL(__H2UDP, __VA_ARGS__)

#define __H2TCP2(packet, size) h2::h2_network_exporter::tcp_inject_received(packet, size, nullptr, "*");
#define __H2TCP3(packet, size, from) h2::h2_network_exporter::tcp_inject_received(packet, size, from, "*");
#define __H2TCP4(packet, size, from, to) h2::h2_network_exporter::tcp_inject_received(packet, size, from, to);
#define H2TCP(...) H2PP_VARIADIC_CALL(__H2TCP, __VA_ARGS__)

#define H2COUT(...) h2::h2_stdio_exporter::capture_cout(__VA_ARGS__)

#define h2_main(argc, argv)                 \
   do {                                     \
      h2::h2_option::I().parse(argc, argv); \
      h2::h2_task::I().prepare();           \
      DNS("127.0.0.1");                     \
      h2::h2_task::I().execute();           \
      h2::h2_task::I().postpare();          \
   } while (0)

#endif
#endif
