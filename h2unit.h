/* v5.6 2020-07-18 16:32:18 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */
#ifndef __H2UNIT_H__
#define __H2UNIT_H__
// h2_unit.hpp

#ifndef __H2_UNIT_HPP__
#define __H2_UNIT_HPP__

#define H2UNIT_VERSION 5.6

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
#include <utility>     /* std::forward, std::pair */
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
#   pragma GCC diagnostic ignored "-Wreturn-type"
#elif defined __clang__
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#   pragma clang diagnostic ignored "-Wint-to-pointer-cast"
#   pragma clang diagnostic ignored "-Wparentheses"
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wwritable-strings"
#   pragma clang diagnostic ignored "-Wreturn-type"
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
// h2_pp.hpp
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

#define H2PP_REMOVE_PARENTHESES_IF(...) H2PP_CAT2(_H2PP_REMOVE_PARENTHESES_IF_, H2PP_IS_BEGIN_PARENTHESES(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_1(...) H2PP_REMOVE_PARENTHESES(__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_0(...) __VA_ARGS__

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
// h2_list.hpp

#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))
#define h2_list_for_each_entry(p, head, type, link) \
   for (int li = 0; li == 0; ++li)                  \
      for (type* p = h2_list_entry((head).next, type, link), *_t = h2_list_entry(p->link.next, type, link); &p->link != &(head); p = _t, _t = h2_list_entry(_t->link.next, type, link), ++li)

#define h2_list_pop_entry(head, type, link) ((head).empty() ? (type*)0 : h2_list_entry(&(head).pop(), type, link))
#define h2_list_top_entry(head, type, link) ((head).empty() ? (type*)0 : h2_list_entry((head).next, type, link))
#define h2_list_bottom_entry(head, type, link) ((head).empty() ? (type*)0 : h2_list_entry((head).prev, type, link))

struct h2_list {
   struct h2_list *next, *prev;

   static void __add_between(h2_list* thus, h2_list* prev, h2_list* next);

   h2_list() : next(this), prev(this) {}

   void add_head(h2_list& entry) { __add_between(&entry, this, this->next); }
   void add_tail(h2_list& entry) { __add_between(&entry, this->prev, this); }
   void add_before(h2_list& entry) { __add_between(&entry, this->prev, this); }
   void add_after(h2_list& entry) { __add_between(&entry, this, this->next); }

   void push_back(h2_list& entry) { add_tail(entry); }
   void push(h2_list& entry) { add_head(entry); }
   h2_list& pop() { return next->out(); }
   h2_list& pop_back() { return prev->out(); }

   bool is_first(h2_list* entry) const { return next == entry; }
   bool is_last(h2_list* entry) const { return prev == entry; }

   h2_list& out();

   bool empty() const { return next == this; }
   int count() const;
   void sort(int (*cmp)(h2_list*, h2_list*));
};
// h2_numeric.hpp

struct h2_numeric {
   static bool not2n(unsigned x);
   static unsigned mask2n(unsigned x);

   static int hex_to_byte(char c);
   static bool is_bin_string(const char* s);
   static bool is_hex_string(const char* s);
   static int extract_hex_string(const char* s, bool ignore_space = false);

   static int bin_to_bits(const char* bin, unsigned char* bytes);

   static int hex_to_bits(const char* hex, unsigned char* bytes);
   static int hex_to_bytes(const char* hex, unsigned char* bytes);

   static bool bits_equal(const unsigned char* b1, const unsigned char* b2, int nbits);

   static const char* sequence_number(int sequence, int shift = 1);

   static long long parse_int_after_equal(const char* s);
};
// h2_libc.hpp

struct h2_libc {
   static void* malloc(size_t size);
   static void free(void* ptr);
   static ssize_t write(int fd, const void* buf, size_t count);
   static void* operator new(std::size_t size) { return malloc(size); }
   static void operator delete(void* ptr) { free(ptr); }
};
// h2_allocate.hpp

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
// h2_string.hpp

struct h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
   h2_string() : basic_string() {}
   h2_string(const h2_string& str) : basic_string(str.c_str()) {}
   h2_string(const std::string& str) : basic_string(str.c_str()) {}
   template <typename... T>
   h2_string(const char* s_fmt, T... t) : basic_string() { sizeof...(T) ? sprintf(s_fmt, t...) : assign(s_fmt); }
   h2_string(size_t n, const char* s) : basic_string(s, n) {}
   h2_string(size_t n, char c) : basic_string(n, c) {}

   h2_string& operator=(const h2_string& str) { return assign(str.c_str()), *this; }
   h2_string& operator=(const std::string& str) { return assign(str.c_str()), *this; }
   h2_string& operator=(const char* s) { return assign(s), *this; }
   h2_string& operator=(char c) { return assign(1, c), *this; }

   h2_string& operator+=(const h2_string& str) { return append(str.c_str()), *this; }
   h2_string& operator+=(const std::string& str) { return append(str.c_str()), *this; }
   h2_string& operator+=(const char* s) { return append(s), *this; }
   h2_string& operator+=(char c) { return push_back(c), *this; }

   bool equals(const h2_string& str, bool caseless = false) const;
   bool contains(const h2_string& substr, bool caseless = false) const;
   bool startswith(const h2_string& prefix, bool caseless = false) const;
   bool endswith(const h2_string& suffix, bool caseless = false) const;

   bool isspace() const;
   bool enclosed(const char c = '\"') const;

   h2_string unquote(const char c = '\"') const;
   h2_string& replace_all(const char* from, const char* to);
   h2_string& tolower();
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
// h2_kit.hpp

#define H2Q(...) H2PP_CAT5(__VA_ARGS__, _C, __COUNTER__, L, __LINE__)  // generate unique identifier [with prefix]

template <typename...>
using h2_void_t = void;

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

template <std::size_t I, typename T, typename... Args>
struct h2_nth_type_impl {
   using type = typename h2_nth_type_impl<I - 1, Args...>::type;
};
template <typename T, typename... Args>
struct h2_nth_type_impl<0, T, Args...> {
   using type = T;
};
template <std::size_t Index, typename... Args>
struct h2_nth_type {
   using type = typename h2_nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;
};

template <std::size_t Index, typename... Args>
using h2_nth_decay = typename h2_decay<typename h2_nth_type<Index, Args...>::type>::type;

template <typename T, typename = void>
struct h2_sizeof_pointee : std::integral_constant<std::size_t, sizeof(typename std::remove_pointer<T>::type)> {
};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<std::is_void<typename std::remove_pointer<T>::type>::value>::type> : std::integral_constant<std::size_t, 1> {
};

struct h2_once {
   operator bool()
   {
      bool ret = c == 0;
      c = 1;
      return ret;
   }

 private:
   int c = 0;
};

struct h2_with {
   FILE* f;
   int (*c)(FILE*);
   h2_with(FILE* file, int (*close)(FILE*) = ::fclose) : f(file), c(close) {}
   ~h2_with() { (f && c) && c(f); }
};

struct h2_pattern {
   static bool regex_match(const char* pattern, const char* subject, bool caseless = false);
   static bool wildcard_match(const char* pattern, const char* subject, bool caseless = false);
   static bool match(const char* pattern, const char* subject, bool caseless = false);
};

static inline const char* comma_if(bool a) { return a ? ", " : ""; };

#define h2_singleton(_Class) \
   static _Class& I()        \
   {                         \
      static _Class i;       \
      return i;              \
   }
// h2_stringify.hpp

template <typename T>
struct h2_is_ostreamable {
   template <typename U>
   static auto test(U* u) -> decltype(std::declval<std::ostream&>() << *u, std::true_type());
   template <typename U>
   static auto test(...) -> std::false_type;
   static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template <typename T>
struct h2_is_pair : std::false_type {
};
template <typename K, typename V>
struct h2_is_pair<std::pair<K, V>> : std::true_type {
};

template <typename T>
struct h2_is_container {
   template <typename U>
   static std::true_type has_const_iterator(typename U::const_iterator*);
   template <typename U>
   static std::false_type has_const_iterator(...);

   template <typename U>
   static std::true_type has_begin(typename std::enable_if<
                                   std::is_same<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::begin)),
                                                typename U::const_iterator (U::*)() const>::value>::type*);
   template <typename U>
   static std::false_type has_begin(...);

   template <typename U>
   static auto has_end(U* u) -> typename std::enable_if<std::is_member_function_pointer<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::end))>::value, std::true_type>::type;
   template <typename U>
   static std::false_type has_end(...);

   static constexpr bool value = decltype(has_const_iterator<T>(nullptr))::value &&
                                 decltype(has_begin<T>(nullptr))::value &&
                                 decltype(has_end<T>(nullptr))::value;
};

template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_string print(T a)
   {
      return "?";
   }
};

#define H2_STRINGIFY_IMPL_TOSTRING(member)                                                                                    \
   template <typename T>                                                                                                      \
   struct h2_stringify_impl<T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::member)>::value>::type> { \
      static h2_string print(const T& a)                                                                                      \
      {                                                                                                                       \
         return const_cast<T&>(a).member();                                                                                   \
      }                                                                                                                       \
   }

/* tostring() may not be mark const, remove cast const in T a */

H2_STRINGIFY_IMPL_TOSTRING(tostring);
H2_STRINGIFY_IMPL_TOSTRING(toString);
H2_STRINGIFY_IMPL_TOSTRING(Tostring);
H2_STRINGIFY_IMPL_TOSTRING(ToString);
H2_STRINGIFY_IMPL_TOSTRING(to_string);

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_string print(const T& a)
   {
      return ostream_print(a);
   }

   template <typename U>
   static h2_string ostream_print(const U& a)
   {
      h2_ostringstream os;
      os << std::boolalpha;
      os << const_cast<U&>(a);
      return h2_string(os.str().c_str());
   }

   static h2_string ostream_print(unsigned char a)
   {  // https://en.cppreference.com/w/cpp/string/byte/isprint
      return ostream_print<unsigned int>(static_cast<unsigned int>(a));
   }
};

template <typename K, typename V>
struct h2_stringify_impl<std::pair<K, V>> {
   static h2_string print(const std::pair<K, V>& a)
   {
      return "(" + h2_stringify_impl<K>::print(a.first) + ", " + h2_stringify_impl<V>::print(a.second) + ")";
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value && !std::is_convertible<T, h2_string>::value>::type> {
   static h2_string print(const T& a)
   {
      h2_string ret = "[";
      for (auto it = a.begin(); it != a.end(); it++) {
         ret += comma_if(it != a.begin()) + h2_stringify_impl<typename T::value_type>::print(*it);
      }
      return ret + "]";
   }
};

template <typename... Args>
struct h2_stringify_impl<std::tuple<Args...>> {
   static h2_string print(const std::tuple<Args...>& a)
   {
      return "(" + tuple_print(a, std::integral_constant<std::size_t, sizeof...(Args)>()) + ")";
   }

   static h2_string tuple_print(const std::tuple<Args...>& a, std::integral_constant<std::size_t, 0>) { return ""; }
   template <std::size_t I>
   static h2_string tuple_print(const std::tuple<Args...>& a, std::integral_constant<std::size_t, I>)
   {
      return tuple_print(a, std::integral_constant<std::size_t, I - 1>()) + comma_if(1 < I) + h2_stringify_impl<typename std::decay<decltype(std::get<I - 1>(a))>::type>::print(std::get<I - 1>(a));
   }
};

template <>
struct h2_stringify_impl<std::nullptr_t> {
   static h2_string print(std::nullptr_t a)
   {
      return "nullptr";
   }
};

template <typename T>
inline h2_string h2_stringify(const T& a)
{
   return h2_stringify_impl<T>::print(a);
}

template <typename T>
inline h2_string h2_stringify(T a, size_t n)
{
   if (n == 0) return h2_stringify(a);
   h2_string ret = "[";
   for (size_t i = 0; i < n; ++i) {
      ret += comma_if(i) + h2_stringify(a[i]);
   }
   return ret + "]";
}

template <typename T>
inline h2_string h2_quote_stringfiy(const T& a)
{
   return (std::is_convertible<T, h2_string>::value ? "\"" : "") + h2_stringify(a) + (std::is_convertible<T, h2_string>::value ? "\"" : "");
}
// h2_option.hpp

struct h2_option {
   h2_singleton(h2_option);

#if defined __linux__
   static constexpr const char* os = "linux";
#elif defined __APPLE__
   static constexpr const char* os = "macos";
#elif defined _WIN32
   static constexpr const char* os = "windows";
#endif

   unsigned term_size;

   char args[256];
   const char* path;
   const char* debug = nullptr;
   bool verbose = false;
   bool colorfull = true;
   bool seq = false;
   bool fold = true;
   bool paste = false;
   bool only = false;
   bool shuffle = false;
   bool memory_check = true;
   bool listing = false;
   int breakable = 0;
   int rounds = 1;
   char junit[256]{'\0'};
   char tap[256]{'\0'};
   std::vector<const char*> includes, excludes;

   h2_option();
   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int line) const;
};

static const h2_option& O = h2_option::I();  // for pretty
// h2_shared_ptr.hpp

template <typename T>
class h2_shared_ptr : h2_libc {
 public:
   h2_shared_ptr() = default;
   explicit h2_shared_ptr(T* p) { acquire(p, nullptr); }
   h2_shared_ptr(const h2_shared_ptr& that) { acquire(that.px, that.pn); }
   ~h2_shared_ptr() { release(); }
   h2_shared_ptr& operator=(h2_shared_ptr that)
   {
      std::swap(px, that.px);
      std::swap(pn, that.pn);
      return *this;
   }
   void reset() { release(); }
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
            ++*pn;
      }
      px = p;
   }
   void release()
   {
      if (pn && !--*pn) {
         delete px;
         h2_libc::free(pn);
      }
      px = nullptr;
      pn = nullptr;
   }

   T* px = nullptr;
   long* pn = nullptr;
};
// h2_line.hpp

struct h2_line : h2_vector<h2_string> {
   h2_line() {}
   h2_line(const char* a) : h2_vector<h2_string>({a}) {}
   h2_line(h2_string a) : h2_vector<h2_string>({a}) {}
   h2_line(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   unsigned width(bool ignore_indent = false) const;
   h2_line& indent(int n, const char c = ' ');
   h2_line& padding(int n, const char c = ' ');

   h2_line& printf(const char* style, const char* format, ...);
   h2_line& concat_back(const h2_line& line, const char* style = nullptr);
   h2_line& concat_front(const h2_line& line_or_word, const char* style = nullptr);

   void brush(const char* style);
   void samesizify(h2_line& b);
};

struct h2_lines : h2_vector<h2_line> {
   h2_lines() {}
   h2_lines(std::initializer_list<h2_line> il) : h2_vector<h2_line>(il) {}

   h2_lines& operator+=(const h2_lines& lines);
   h2_lines& concat_front(const h2_lines& lines);
   h2_lines& concat_front(const h2_line& line);

   unsigned width() const;
   bool foldable(unsigned width = 20);
   h2_line folds();

   void sequence(unsigned indent = 0, int start = 0);
   void samesizify(h2_lines& b);
};
// h2_layout.hpp

struct h2_layout {
   static h2_lines split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, int step, char scale, unsigned width);
   static h2_lines unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, unsigned width);
   static h2_lines seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, unsigned width);
};
// h2_color.hpp

struct h2_color {
   static void printf(const char* style, const char* format, ...);
   static void printf(const h2_line& line);
   static void printf(const h2_lines& lines);

   static bool is_ctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
// h2_nm.hpp

struct h2_nm {
   h2_singleton(h2_nm);
   h2_list symbols;
   h2_nm();
   static unsigned long long get(const char* name);
   static long long text_offset();
   static long long vtable_offset();
   static bool in_main(unsigned long long addr);
};
// h2_backtrace.hpp

struct h2_backtrace {
   int count = 0, shift = 0;
   void* array[100];

   h2_backtrace() = default;
   h2_backtrace(int shift_);

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(const h2_backtrace&);

   bool has(void* func, int size) const;
   void print(h2_vector<h2_string>& stacks) const;
   void print(int pad) const;
};
// h2_debug.hpp

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
// h2_patch.hpp

struct h2_patch {
   static void initialize();
   static bool exempt(const h2_backtrace& bt);
};
// h2_failure.hpp

struct h2_fail : h2_libc {
   h2_fail *subling_next{nullptr}, *child_next{nullptr};

   const char* file;
   int line;

   const char* func;
   int seq = -1;

   const char* check_type = "Inner";  // Inner(Mock, AllOf, &&, ||)
   h2_string e_expression, a_expression;
   h2_line explain;
   h2_string user_explain;

   //    expression     expection      represent       value
   //     Ge(var)        Ge(5)          5               5
   //     We(var)        We("abc")      "abc"           abc

   h2_fail(const h2_line& explain_, const char* file_, int line_, const char* func_ = nullptr, int seq_ = -1)
     : file(file_), line(line_), func(func_), seq(seq_), explain(explain_) {}
   virtual ~h2_fail();

   const char* locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);

   static h2_fail* new_normal(const h2_line& explain, const char* func = nullptr, const char* file = nullptr, int line = 0);
   static h2_fail* new_unexpect(const h2_string& e_represent = "", const h2_string& a_represent = "", const h2_string& expection = "", const h2_line& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_string& expection, const char* file = nullptr, int line = 0);
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, const char* file = nullptr, int line = 0);
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, bool caseless, const char* file = nullptr, int line = 0);
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& expection, const h2_string& a_represent, const h2_line& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_memory_leak(const void* ptr, int size, const h2_backtrace& bt_allocate, const char* where, const char* file, int line);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const int size, const void* addr, const char* action, h2_vector<unsigned char> spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file = nullptr, int line = 0);
   static h2_fail* new_use_after_free(const void* ptr, const void* addr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
   static h2_fail* new_call(const char* func, const char* expect, const char* actual, const h2_line& explain = {}, const char* file = nullptr, int line = 0);
};

static inline void h2_fail_g(h2_fail*, bool);
// h2_generator.hpp

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
#define H2CASES(name, ...) _ForEach_CASE_Impl(#name, H2Q(f), __VA_ARGS__)

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
#define H2CASESS(name, ...) _Fullmesh_CASE_Impl(#name, H2Q(f), __VA_ARGS__)

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
#define H2Cases(name, ...) _ForEach_Case_Impl(#name, H2Q(j), H2Q(b), H2Q(v), H2Q(l), __VA_ARGS__)

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
#define H2Casess(name, ...) _Fullmesh_Case_Impl(#name, H2Q(j), H2Q(b), H2Q(l), H2Q(x), H2Q(y), __VA_ARGS__)
// h2_json.hpp

struct h2_json {
   static h2_lines format(const h2_string& json_string);
   static bool match(const h2_string& expect, const h2_string& actual, bool caseless);
   static bool diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless);
};
// h2_memory.hpp

struct h2_memory {
   static void initialize();
   static void finalize();
   static void overrides();
   static void restores();

   struct stack {
      static void root();
      static void push(const char* file, int line);
      static h2_fail* pop();
      static long long footprint();

      struct block : h2_once {
         block(const char* attributes, const char* file, int line);
         ~block();
      };
   };
};

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(Attributes, __FILE__, __LINE__); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2Q(t_block))
// h2_matches.hpp

struct h2_matches {
   virtual h2_string expects(bool caseless, bool dont) const = 0;
};

static inline h2_string DS(bool match)
{
   return match ? "should not match" : "";
}

static inline h2_string CD(const h2_string& s, bool caseless = false, bool dont = false)
{
   h2_string z = s;
   if (dont) z = "!" + z;
   if (caseless) z = "~" + z;
   return z;
}

template <typename T>
inline auto h2_matches_expects(const T& a, bool caseless, bool dont) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_string>::type
{
   return a.expects(caseless, dont);
}
template <typename T>
inline auto h2_matches_expects(const T& a, bool caseless, bool dont) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_string>::type
{
   return CD(h2_stringify(a), caseless, dont);
}

#define H2_MATCHER_T2V2E(t_matchers)                                                                                                                           \
                                                                                                                                                               \
   template <typename T>                                                                                                                                       \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                                             \
   template <typename T, std::size_t I>                                                                                                                        \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                                \
   {                                                                                                                                                           \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                                           \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                                   \
   }                                                                                                                                                           \
   template <typename T>                                                                                                                                       \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }                \
                                                                                                                                                               \
   h2_string t2e(bool caseless, bool dont, std::integral_constant<std::size_t, 0>) const { return ""; }                                                        \
   template <std::size_t I>                                                                                                                                    \
   h2_string t2e(bool caseless, bool dont, std::integral_constant<std::size_t, I>) const                                                                       \
   {                                                                                                                                                           \
      return t2e(caseless, dont, std::integral_constant<size_t, I - 1>()) + comma_if(1 < I) + h2_matches_expects(std::get<I - 1>(t_matchers), caseless, dont); \
   }                                                                                                                                                           \
   h2_string t2e(bool caseless, bool dont) const { return t2e(caseless, dont, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
// h2_matcher.hpp

template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(T a, int n, bool caseless, bool dont) const = 0;
   virtual h2_string expects(bool caseless, bool dont) const override { return ""; }
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher : h2_matches {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher() {}
   h2_matcher(T value);  // Converting constructor
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl_, const int placeholder) : impl(impl_) {}
   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   virtual ~h2_matcher() {}
   h2_fail* matches(const T& a, int n = 0, bool caseless = false, bool dont = false) const { return impl->matches(a, n, caseless, dont); }
   virtual h2_string expects(bool caseless = false, bool dont = false) const { return impl->expects(caseless, dont); };
};

template <typename Matches>
struct h2_polymorphic_matcher : h2_matches {
   const Matches m;
   explicit h2_polymorphic_matcher(const Matches& _matches) : m(_matches) {}

   template <typename T>
   operator h2_matcher<T>() const
   {
      return h2_matcher<T>(new internal_impl<const T&>(m), 0);
   }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      explicit internal_impl(const Matches& _matches) : m(_matches) {}
      h2_fail* matches(T a, int n = 0, bool caseless = false, bool dont = false) const override { return m.matches(a, n, caseless, dont); }
      h2_string expects(bool caseless, bool dont) const override { return m.expects(caseless, dont); }
   };

   virtual h2_string expects(bool caseless = false, bool dont = false) const override
   {
      return h2_matches_expects(m, caseless, dont);
      // return m.expects(caseless, dont);
   }
};
// h2_equation.hpp

template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& _e, const long double = 0) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a == e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD(h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& _e, const long double = 0) : e(h2_string(_e)) {}

   h2_fail* matches(const h2_string& a, int, bool caseless, bool dont) const
   {
      if (a.equals(e, caseless) == !dont) return nullptr;
      if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
      if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;

      return h2_fail::new_strcmp(e, a, caseless, expects(caseless, dont));
   }
   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("\"" + e + "\"", caseless, dont);
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& _e, const long double _epsilon = 0) : e(_e), epsilon(_epsilon) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      bool result;
      if (std::is_floating_point<E>::value || std::is_floating_point<A>::value) {
         //
         // the machine epsilon has to be scaled to the magnitude of the values used
         // and multiplied by the desired precision in ULPs (units in the last place)
         // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
         //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
         long double _epsilon = epsilon;
         if (_epsilon == 0) _epsilon = 0.00001;
         result = std::fabs((const long double)a - (const long double)e) < _epsilon;
      } else {
         result = a == e;
      }
      if (result == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD(h2_stringify(e), false, dont);
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_equation<E>> Eq(const T& expect, const long double epsilon = 0)
{
   return h2_polymorphic_matcher<h2_equation<E>>(h2_equation<E>(expect, epsilon));
}

template <typename T>
inline h2_matcher<T>::h2_matcher(T value)
{
   *this = Eq(value);
}
// h2_cast.hpp

template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from) { return do_cast(from, std::is_convertible<M, h2_matcher<T>>{}, std::is_convertible<M, T>{}); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type) { return h2_polymorphic_matcher<h2_equation<M>>(h2_equation<M>(from)); }
};

template <typename T, typename U>
struct h2_matcher_cast_impl<T, h2_matcher<U>> {
   static h2_matcher<T> cast(const h2_matcher<U>& from) { return h2_matcher<T>(new internal_impl(from)); }

   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}

      // Delegate the matching logic to the source h2_matcher.
      h2_fail* matches(T a, int n, bool caseless, bool dont) const override
      {
         using FromType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::type;
         using ToType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<U>::type>::type>::type;
         // Do not allow implicitly converting base*/& to derived*/&.
         static_assert((std::is_pointer<typename std::remove_reference<T>::type>::value != std::is_pointer<typename std::remove_reference<U>::type>::value) || std::is_same<FromType, ToType>::value || !std::is_base_of<FromType, ToType>::value, "Can't implicitly convert from <base> to <derived>");

         return from.matches(static_cast<U>(a), n, caseless, dont);
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
inline h2_matcher<T> h2_matcher_cast(const M& from)
{
   return h2_matcher_cast_impl<T, M>::cast(from);
}
// h2_unary.hpp

struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool) const { return nullptr; }
   virtual h2_string expects(bool, bool) const override { return "Any"; }
};

struct h2_matches_null : h2_matches {
   const bool reverse;
   explicit h2_matches_null(bool _reverse) : reverse(_reverse) {}
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool dont) const
   {
      bool _dont = reverse ? !dont : dont;
      if ((nullptr == (const void*)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect("", h2_stringify((const void*)a), expects(false, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return (reverse ? !dont : dont) ? "NotNull" : "IsNull";
   }
};

template <bool E>
struct h2_matches_boolean : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool dont) const
   {
      bool _dont = E ? dont : !dont;
      if (((bool)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect("", a ? "true" : "false", expects(false, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return (E ? dont : !dont) ? "false" : "true";
   }
};

template <typename Matcher>
struct h2_pointee_matches : h2_matches {
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
   h2_fail* matches(A a, int, bool caseless, bool dont) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, 0, caseless, dont);
   }
   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return h2_matches_expects(m, caseless, dont);
   }
};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_any> Any{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_null> IsNull{h2_matches_null(false)};
const h2_polymorphic_matcher<h2_matches_null> NotNull{h2_matches_null(true)};
const h2_polymorphic_matcher<h2_matches_boolean<true>> IsTrue{h2_matches_boolean<true>()};
const h2_polymorphic_matcher<h2_matches_boolean<false>> IsFalse{h2_matches_boolean<false>()};

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
// h2_logic.hpp

template <typename Matcher>
struct h2_not_matches : h2_matches {
   const Matcher m; /* h2_polymorphic_matcher or immediate value or h2_matches */
   explicit h2_not_matches(const Matcher& _m) : m(_m) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      return h2_matcher_cast<A>(m).matches(a, n, caseless, !dont);
   }
   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return h2_matches_expects(m, caseless, !dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_and_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_and_matches(const Matcher1& _m1, const Matcher2& _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m1).matches(a, n, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m2).matches(a, n, caseless, false));
      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(caseless, dont), DS(true));
      }
      return fail;
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD(h2_matches_expects(m1, caseless, false) + " && " + h2_matches_expects(m2, caseless, false), false, dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_or_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_or_matches(const Matcher1& _m1, const Matcher2& _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, n, caseless, false);
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, n, caseless, false);
      bool result = !f1 || !f2;
      if (result == !dont) {
         if (f1) delete f1;
         if (f2) delete f2;
         return nullptr;
      }
      return h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(caseless, dont), DS(result));
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD(h2_matches_expects(m1, caseless, false) + " || " + h2_matches_expects(m2, caseless, false), false, dont);
   }
};

template <typename... Matchers>
struct h2_allof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, caseless, false);
         if (fail) fail->seq = i;
         h2_fail::append_subling(fails, fail);
      }

      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(caseless, dont), "Should not match all");
      } else {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(caseless, dont));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("AllOf(" + t2e(caseless, false) + ")", false, dont);
   }

   H2_MATCHER_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int c = 0;
      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, caseless, false);
         if (!fail) {
            c++;
            break;
         }
         if (fail) fail->seq = i;
         h2_fail::append_subling(fails, fail);
      }

      if ((0 < c) == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(caseless, dont), "Should not match any one");
      } else {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(caseless, dont), "Not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("AnyOf(" + t2e(caseless, false) + ")", false, dont);
   }

   H2_MATCHER_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int c = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, n, caseless, false);
         if (!fail) ++c;
         if (fail) delete fail;
      }
      bool result = c == 0;
      if (result == !dont) return nullptr;
      return h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(caseless, dont), DS(result));
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("NoneOf(" + t2e(caseless, false) + ")", false, dont);
   }

   H2_MATCHER_T2V2E(t_matchers)
};

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
// h2_inequation.hpp

template <typename E>
struct h2_matches_ge : h2_matches {
   const E e;
   explicit h2_matches_ge(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a >= e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD(">=" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_gt : h2_matches {
   const E e;
   explicit h2_matches_gt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a > e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD(">" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_le : h2_matches {
   const E e;
   explicit h2_matches_le(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a <= e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD("<=" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_lt : h2_matches {
   const E e;
   explicit h2_matches_lt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a < e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD("<" + h2_stringify(e), false, dont);
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_not_matches<E>> Nq(const T& expect) { return h2_polymorphic_matcher<h2_not_matches<E>>(h2_not_matches<E>(expect)); }

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_ge<E>> Ge(const T& expect) { return h2_polymorphic_matcher<h2_matches_ge<E>>(h2_matches_ge<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_gt<E>> Gt(const T& expect) { return h2_polymorphic_matcher<h2_matches_gt<E>>(h2_matches_gt<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_le<E>> Le(const T& expect) { return h2_polymorphic_matcher<h2_matches_le<E>>(h2_matches_le<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_lt<E>> Lt(const T& expect) { return h2_polymorphic_matcher<h2_matches_lt<E>>(h2_matches_lt<E>(expect)); }
// h2_strcmp.hpp

struct h2_matches_regex : h2_matches {
   const h2_string e;
   explicit h2_matches_regex(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_matches_wildcard : h2_matches {
   const h2_string e;
   explicit h2_matches_wildcard(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_matches_strcmp : h2_matches {
   const h2_string e;
   explicit h2_matches_strcmp(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_matches_substr : h2_matches {
   const h2_string substring;
   explicit h2_matches_substr(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_matches_startswith : h2_matches {
   const h2_string prefix_string;
   explicit h2_matches_startswith(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_matches_endswith : h2_matches {
   const h2_string suffix_string;
   explicit h2_matches_endswith(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_matches_json : h2_matches {
   const h2_string e;
   explicit h2_matches_json(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_caseless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(const h2_matcher<h2_string>& matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const { return m.matches(a, n, true, dont); }
   virtual h2_string expects(bool caseless, bool dont) const override { return m.expects(true, dont); }
};

inline h2_polymorphic_matcher<h2_matches_regex> Re(const h2_string& regex_pattern) { return h2_polymorphic_matcher<h2_matches_regex>(h2_matches_regex(regex_pattern)); }
inline h2_polymorphic_matcher<h2_matches_wildcard> We(const h2_string& wildcard_pattern) { return h2_polymorphic_matcher<h2_matches_wildcard>(h2_matches_wildcard(wildcard_pattern)); }
inline h2_polymorphic_matcher<h2_matches_strcmp> Se(const h2_string& expect) { return h2_polymorphic_matcher<h2_matches_strcmp>(h2_matches_strcmp(expect)); }
inline h2_polymorphic_matcher<h2_matches_substr> Substr(const h2_string& substring) { return h2_polymorphic_matcher<h2_matches_substr>(h2_matches_substr(substring)); }
inline h2_polymorphic_matcher<h2_matches_startswith> StartsWith(const h2_string& prefix_string) { return h2_polymorphic_matcher<h2_matches_startswith>(h2_matches_startswith(prefix_string)); }
inline h2_polymorphic_matcher<h2_matches_endswith> EndsWith(const h2_string& suffix_string) { return h2_polymorphic_matcher<h2_matches_endswith>(h2_matches_endswith(suffix_string)); }
inline h2_polymorphic_matcher<h2_matches_json> Je(const h2_string& expect) { return h2_polymorphic_matcher<h2_matches_json>(h2_matches_json(expect)); }

template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(const M& m) { return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(h2_matcher<h2_string>(m))); }
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> operator~(const M& m) { return CaseLess(m); }
// h2_memcmp.hpp

struct h2_matches_bytecmp : h2_matches {
   const int width;
   const void* e;
   const bool isstring;
   const int nbytes;
   explicit h2_matches_bytecmp(const int _width, const void* _e, const bool _isstring, const int _nbytes) : width(_width), e(_e), isstring(_isstring), nbytes(_nbytes) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

struct h2_matches_bitcmp : h2_matches {
   const void* e;
   const bool isstring;
   const int nbits;
   explicit h2_matches_bitcmp(const void* _e, const bool _isstring, const int _nbits) : e(_e), isstring(_isstring), nbits(_nbits) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const;
   virtual h2_string expects(bool caseless, bool dont) const override;
};

template <typename E>
struct h2_matches_memcmp : h2_matches {
   const E e;
   const int length;
   explicit h2_matches_memcmp(const E _e, const int _length) : e(_e), length(_length) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const
   {
      h2_fail* fail = (h2_fail*)1;

      if (std::is_convertible<E, h2_string>::value) { /* deduce */
         if (h2_numeric::is_bin_string((const char*)e)) {
            h2_matches_bitcmp t((const void*)e, true, length);
            fail = t.matches(a, n, false, false);
         }
      }

      if (fail) {
         h2_matches_bytecmp t(h2_sizeof_pointee<E>::value * 8, e, std::is_convertible<E, h2_string>::value, length * h2_sizeof_pointee<E>::value);
         fail = t.matches(a, n, false, false);
      }

      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_stringify(a), expects(caseless, dont));
      }
      return fail;
   }
   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("Me()", caseless, dont);
   }
};

template <typename E, typename T = typename std::decay<E>::type>
inline h2_polymorphic_matcher<h2_matches_memcmp<T>> Me(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_memcmp<T>>(h2_matches_memcmp<T>((T)expect, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bitcmp> M1e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bitcmp>(h2_matches_bitcmp((const void*)expect, std::is_convertible<E, h2_string>::value, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M8e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(8, (const void*)expect, std::is_convertible<E, h2_string>::value, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M16e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(16, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 2));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M32e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(32, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 4));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M64e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(64, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 8));
}
// h2_container.hpp

template <typename EK, typename EV>
struct h2_pair_matches : h2_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(const EK& k_, const EV& v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, int, bool caseless, bool dont) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      using AK = typename std::decay<decltype(a.first)>::type;
      using AV = typename std::decay<decltype(a.second)>::type;
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<AK>(k).matches(a.first, 0, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<AV>(v).matches(a.second, 0, caseless, false));
      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_stringify(a), expects(caseless, dont));
      }
      return fail;
   }

   template <typename A>
   auto matches(const A& a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect("", h2_stringify(a), expects(caseless, dont));
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("(" + h2_matches_expects(k, caseless, dont) + ", " + h2_matches_expects(v, caseless, dont) + ")", false, dont);
   }
};

template <typename... Matchers>
struct h2_listof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int, bool caseless, bool dont) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;

      h2_vector<h2_matcher<typename A::value_type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, c = 0;
         for (auto& k : a) {
            if (j++ == i) {
               ++c;
               fail = v_matchers[i].matches(k, 0, caseless, false);
               break;
            }
         }
         if (c == 0) {
            fail = h2_fail::new_unexpect("", "", v_matchers[i].expects(caseless, false), "out of range");
         }
         if (fail) fail->seq = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", h2_stringify(a), expects(caseless, dont), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(A a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ generic array */
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<typename std::decay<decltype(a[0])>::type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a[i], n, caseless, false);
         if (fail) fail->seq = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", h2_stringify(a), expects(caseless, dont), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("ListOf(" + t2e(caseless, false) + ")", false, dont);
   }

   H2_MATCHER_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_has_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_has_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int n, bool caseless, bool dont) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_vector<h2_matcher<typename A::value_type>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         bool found = false;
         for (auto& j : a) {
            h2_fail* fail = v_matchers[i].matches(j, n, caseless, false);
            if (!fail) {
               found = true;
               break;
            }
         }
         if (!found) {
            h2_fail* fail = h2_fail::new_normal({"haven't ", "\033{red}", v_matchers[i].expects(caseless, false), "\033{reset}"});
            if (fail) fail->seq = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", h2_stringify(a), expects(caseless, dont), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(A a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_vector<h2_matcher<typename std::decay<decltype(a[0])>::type>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         bool found = false;
         for (int j = 0; j < n; ++j) {
            h2_fail* fail = v_matchers[i].matches(a[j], n, caseless, false);
            if (!fail) {
               found = true;
               break;
            }
         }
         if (!found) {
            h2_fail* fail = h2_fail::new_normal({"haven't ", "\033{red}", v_matchers[i].expects(caseless, false), "\033{reset}"});
            if (fail) fail->seq = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", h2_stringify(a, n), expects(caseless, dont), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("Has(" + t2e(caseless, false) + ")", false, dont);
   }

   H2_MATCHER_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_in_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_in_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, n, caseless, false);
         if (!fail) ++s;
         if (fail) delete fail;
      }
      bool result = 0 < s;
      if (result == !dont) return nullptr;
      return h2_fail::new_unexpect("", h2_stringify(a), expects(caseless, dont), DS(result));
   }

   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return CD("In(" + t2e(caseless, false) + ")", false, dont);
   }

   H2_MATCHER_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_countof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_countof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int n, bool, bool dont) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      int count = 0;
      /* https://en.cppreference.com/w/cpp/container */
      /* container size() is best, but forward_list haven't. iterator works all, regardless speed */
      for (auto& c : a) count++;

      return __matches(count, h2_stringify(a), dont);
   }

   template <typename A>
   auto matches(A a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ generic array or not array , no meaningful just for continues */
      int count = 1;
      return __matches(count, h2_stringify(a), dont);
   }

   h2_fail* __matches(int count, h2_string&& a_represent, bool dont) const
   {
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<int>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(count, 0, false, false);
         if (fail) fail->seq = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", a_represent + "/" + h2_stringify(count), expects(false, dont), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_string expects(bool, bool dont) const override
   {
      return CD("CountOf(" + t2e(false, dont) + ")", false, false);
   }

   H2_MATCHER_T2V2E(t_matchers)
};

template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_pair_matches<EK, EV>> Pair(const MK& mk, const MV& mv)
{
   return h2_polymorphic_matcher<h2_pair_matches<EK, EV>>(h2_pair_matches<EK, EV>(mk, mv));
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
inline h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>> Have(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>>(h2_has_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>> In(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>>(h2_in_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matchers&>::type...>> CountOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matchers&>::type...>>(h2_countof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
// h2_customize.hpp

#define __Matches_Common(message)                                                                         \
   template <typename A>                                                                                  \
   bool __matches(const A& a) const;                                                                      \
   template <typename A>                                                                                  \
   h2::h2_fail* matches(const A& a, int, bool caseless, bool dont) const                                  \
   {                                                                                                      \
      h2::h2_fail* fail = h2::h2_fail::new_unexpect("", h2::h2_stringify(a), h2::CD("", caseless, dont)); \
      if (__matches(a) == !dont) return nullptr;                                                          \
      if (dont) {                                                                                         \
      } else {                                                                                            \
         h2::h2_ostringstream osm;                                                                        \
         osm << std::boolalpha << H2PP_REMOVE_PARENTHESES(message);                                       \
         fail->user_explain = osm.str().c_str();                                                          \
      }                                                                                                   \
      return fail;                                                                                        \
   }                                                                                                      \
   virtual h2::h2_string expects(bool caseless, bool dont) const override { return ""; }

#define H2MATCHER0(name, message)                                                     \
   struct h2_##name##_matches : h2::h2_matches {                                      \
      explicit h2_##name##_matches() {}                                               \
      __Matches_Common(message)                                                       \
   };                                                                                 \
   const h2::h2_polymorphic_matcher<h2_##name##_matches> name{h2_##name##_matches()}; \
   template <typename A>                                                              \
   bool h2_##name##_matches::__matches(const A& a) const

#define H2MATCHER1(name, e1, message)                                                           \
   template <typename E1>                                                                       \
   struct h2_##name##_matches : h2::h2_matches {                                                \
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
   struct h2_##name##_matches : h2::h2_matches {                                                             \
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
   struct h2_##name##_matches : h2::h2_matches {                                                                          \
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
   struct h2_##name##_matches : h2::h2_matches {                                                                                       \
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
   struct h2_##name##_matches : h2::h2_matches {                                                                                                               \
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
// h2_fp.hpp

template <typename T>
void* h2_fp(T p)
{
   void* fp = (void*)p;
   if (std::is_convertible<T, h2::h2_string>::value) {
      fp = (void*)(h2_nm::get((const char*)p) + h2_nm::text_offset());
   }
   return fp;
}
// h2_stub.hpp

struct h2_stubs {
   h2_list stubs;
   bool add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   unsigned char saved[32];
   void* origin_fp;
   h2_stub_temporary_restore(void* origin_fp);
   ~h2_stub_temporary_restore();
};

#define __H2STUB2(OriginFunction, SubstituteFunction)                                                           \
   do {                                                                                                         \
      h2::h2_stub_g(h2::h2_fp(OriginFunction), (void*)SubstituteFunction, #OriginFunction, __FILE__, __LINE__); \
   } while (0)

#define ____H2STUB3(OriginFunction, Return, Args, Q)                                                            \
   struct {                                                                                                     \
      void operator=(Return(*substitute_fp) Args)                                                               \
      {                                                                                                         \
         h2::h2_stub_g(h2::h2_fp(OriginFunction), (void*)(substitute_fp), #OriginFunction, __FILE__, __LINE__); \
      }                                                                                                         \
   } Q;                                                                                                         \
   Q = [] Args -> Return /* captureless lambda implicit cast to function pointer */

#define __H2STUB3(OriginFunction, Return, Args) ____H2STUB3(OriginFunction, Return, Args, H2Q(t_stub3))

#define __H2STUB40(Class, Method, Return, Args, Q)                                                                                                                                                                              \
   struct {                                                                                                                                                                                                                     \
      void operator=(Return (*substitute_fp)(H2PP_REMOVE_PARENTHESES_IF(Class) * that))                                                                                                                                         \
      {                                                                                                                                                                                                                         \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                         \
   } Q;                                                                                                                                                                                                                         \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * that) -> Return

#define __H2STUB41(Class, Method, Return, Args, Q)                                                                                                                                                                              \
   struct {                                                                                                                                                                                                                     \
      void operator=(Return (*substitute_fp)(H2PP_REMOVE_PARENTHESES_IF(Class) * that, H2PP_REMOVE_PARENTHESES(Args)))                                                                                                          \
      {                                                                                                                                                                                                                         \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                         \
   } Q;                                                                                                                                                                                                                         \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB4(Class, Method, Return, Args) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Args, __H2STUB40(Class, Method, Return, Args, H2Q(t_stub40)), __H2STUB41(Class, Method, Return, Args, H2Q(t_stub41)))

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__)
// h2_mfp.hpp

/* clang-format off */

template <typename T, int I> struct h2_constructible_error {
   static T* O(void* m) { return static_cast<T*>(m = (void*)I); }
};
template <typename T> struct h2_constructible0 : std::true_type {
   static T* O(void* m) { return new (m) T(); }
};

template <typename, typename> struct h2_constructible1_impl : std::false_type {};
template <typename, typename> struct h2_constructible2_impl : std::false_type {};
template <typename, typename> struct h2_constructible3_impl : std::false_type {};
template <typename, typename> struct h2_constructible4_impl : std::false_type {};
template <typename, typename> struct h2_constructible5_impl : std::false_type {};
template <typename, typename> struct h2_constructible6_impl : std::false_type {};
template <typename, typename> struct h2_constructible7_impl : std::false_type {};
template <typename, typename> struct h2_constructible8_impl : std::false_type {};
template <typename, typename> struct h2_constructible9_impl : std::false_type {};

#if (defined(__GNUC__) && __GNUC__ >= 5) || defined __clang__ || defined _WIN32

template <typename T>
struct h2_constructible1_impl<h2_void_t<decltype(T({}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}); }
};

template <typename T>
struct h2_constructible2_impl<h2_void_t<decltype(T({}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}); }
};

template <typename T>
struct h2_constructible3_impl<h2_void_t<decltype(T({}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}); }
};

template <typename T>
struct h2_constructible4_impl<h2_void_t<decltype(T({}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible5_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible6_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible7_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible8_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible9_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}, {}); }
};

#endif

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

//  g++ -std=c++11 -fdump-class-hierarchy

//  &Class::Method has separate representations for non-virtual and virtual functions.
//  For non-virtual functions, it is the address of the function.
//  For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
//  The least-significant bit therefore discriminates between virtual and non-virtual functions.

template <typename Class, typename Signature>
struct h2_mfp;

template <typename Class, typename Return, typename... Args>
struct h2_mfp<Class, Return(Args...)> {
   typedef Return (Class::*F)(Args...);
   typedef union {
      F f;
      void* p;
      long long v;
   } U;

   static inline bool is_virtual_member(U& u)
   {
      return (u.v & 1) && (u.v - 1) % sizeof(void*) == 0
             /* assumption: virtual member count less than 3000 */
             && (u.v - 1) / sizeof(void*) < 3000;
   }

   static void* A(F f)
   {
      U u{f};
      if (!is_virtual_member(u)) return u.p;
      void** vtable = nullptr;
      Class* object = h2_constructible<Class>::O(alloca(sizeof(Class)));
      if (0 == (long long)object || 1 == (long long)object || 2 == (long long)object) {
         char vtable_symbol[1024];
         sprintf(vtable_symbol, "_ZTV%s", typeid(Class).name());  // mangled for "vtable for Class"
         unsigned long long relative_vtable = h2_nm::get(vtable_symbol);
         if (relative_vtable) {
            vtable = (void**)(relative_vtable + h2_nm::vtable_offset());
         }
      } else {
         vtable = *(void***)object;
      }
      if (!vtable) return nullptr;
      return vtable[(u.v - 1) / sizeof(void*)];
   }

   static long long B(F f)
   {
      U u{f};
      return u.v;
   }
};
// h2_checkin.hpp

/* 考勤 ; 函数被调次数期望 */
struct h2_checkin {
   int call = 0;
   int least, most;

   h2_checkin(int _least, int _most) : least(_least), most(_most) {}

   void operator++() { call += 1; }

   bool is_not_enough(/*不够*/) const { return call < least; }
   bool is_minimal_satisfied(/*最小满足*/) const { return least == call; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_overmuch(/*过多*/) const { return most < call; }

   h2_fail* check(const char* func, const char* file, int line);
   const char* actual();
   const char* expect();
};
// h2_function.hpp

template <typename Return>
struct h2_return : h2_libc {
   Return value;
   h2_return() = delete;
   explicit h2_return(Return _value) : value(_value){};
};

template <typename Class, typename Signature>
struct h2_function;

template <typename Class, typename Return, typename... Args>
struct h2_function<Class, Return(Args...)> {
   std::function<Return(Args...)> normal_function = {};  // functional alignment issue
   std::function<Return(Class*, Args...)> member_function = {};
   h2_shared_ptr<h2_return<Return>> return_value;

   h2_function(Return r) : return_value(new h2_return<Return>(r)) {}
   h2_function(std::function<Return(Args...)> f) : normal_function(f) {}
   h2_function(std::function<Return(Class*, Args...)> f) : member_function(f) {}

   Return operator()(Class* that, Args... args)
   {
      if (member_function) {
         return member_function(that, args...);
      } else if (normal_function) {
         return normal_function(args...);
      } else if (return_value) {
         return return_value->value;
      }
      /* return uninitialized value */
   }
   void clear()
   {
      normal_function = nullptr;
      member_function = nullptr;
      return_value.reset();
   }
};

template <typename Class, typename... Args>
struct h2_function<Class, void(Args...)> {
   std::function<void(Args...)> normal_function = {};
   std::function<void(Class*, Args...)> member_function = {};

   h2_function() {}
   h2_function(std::function<void(Args...)> f) : normal_function(f) {}
   h2_function(std::function<void(Class*, Args...)> f) : member_function(f) {}

   void operator()(Class* that, Args... args)
   {
      if (member_function) {
         member_function(that, args...);
      } else if (normal_function) {
         normal_function(args...);
      }
   }
   void clear()
   {
      normal_function = nullptr;
      member_function = nullptr;
   }
};
// h2_tuple.hpp

static inline void h2_check_g();

template <typename MatcherTuple, typename ArgumentTuple, std::size_t I>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, I>)
{
   h2_fail* fails = matches(matchers, arguments, std::integral_constant<std::size_t, I - 1>());
   h2_fail* fail = std::get<I - 1>(matchers).matches(std::get<I - 1>(arguments));
   if (fail) fail->seq = I - 1;
   h2_fail::append_subling(fails, fail);
   h2_check_g();
   return fails;
}

template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, 0>)
{
   return nullptr;
}

template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* h2_tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments)
{
   return matches(matchers, arguments, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}
// h2_mock.hpp

struct h2_mock : h2_libc {
   h2_list x;
   void *origin_fp, *substitute_fp;
   const char* origin_fn;
   const char* file;
   int line;

   h2_vector<h2_checkin> checkin_array;
   int checkin_index = 0;

   virtual void reset() = 0;
   void mock();
   h2_fail* times_check();
};

struct h2_mocks {
   h2_list mocks;
   bool add(h2_mock* mock);
   h2_fail* clear(bool check);
};

#ifdef _WIN32
#   define __H2_LINENO__ 0
#else
#   define __H2_LINENO__ __LINE__
#endif

#define __H2MOCK3(OriginFunction, Return, Args) \
   h2::h2_mocker<__COUNTER__, __H2_LINENO__, std::false_type, Return Args>::I(h2::h2_fp(OriginFunction), #OriginFunction, __FILE__, __LINE__)

#define __H2MOCK4(Class, Method, Return, Args) \
   h2::h2_mocker<__COUNTER__, __H2_LINENO__, H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, __FILE__, __LINE__)

#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)
// h2_mocker.hpp

template <int Counter, int Lineno, typename Class, typename Signature>
class h2_mocker;

template <int Counter, int Lineno, typename Class, typename Return, typename... Args>
class h2_mocker<Counter, Lineno, Class, Return(Args...)> : h2_mock {
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

// #define FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9

#define FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9                \
   std::forward<h2_matcher<h2_nth_decay<0, Args...>>>(_0), \
   std::forward<h2_matcher<h2_nth_decay<1, Args...>>>(_1), \
   std::forward<h2_matcher<h2_nth_decay<2, Args...>>>(_2), \
   std::forward<h2_matcher<h2_nth_decay<3, Args...>>>(_3), \
   std::forward<h2_matcher<h2_nth_decay<4, Args...>>>(_4), \
   std::forward<h2_matcher<h2_nth_decay<5, Args...>>>(_5), \
   std::forward<h2_matcher<h2_nth_decay<6, Args...>>>(_6), \
   std::forward<h2_matcher<h2_nth_decay<7, Args...>>>(_7), \
   std::forward<h2_matcher<h2_nth_decay<8, Args...>>>(_8), \
   std::forward<h2_matcher<h2_nth_decay<9, Args...>>>(_9)
   /* clang-format on */

   using ArgumentTuple = std::tuple<Args...>;
   using MatcherTuple = std::tuple<h2_matcher<h2_nth_decay<0, Args...>>,
                                   h2_matcher<h2_nth_decay<1, Args...>>,
                                   h2_matcher<h2_nth_decay<2, Args...>>,
                                   h2_matcher<h2_nth_decay<3, Args...>>,
                                   h2_matcher<h2_nth_decay<4, Args...>>,
                                   h2_matcher<h2_nth_decay<5, Args...>>,
                                   h2_matcher<h2_nth_decay<6, Args...>>,
                                   h2_matcher<h2_nth_decay<7, Args...>>,
                                   h2_matcher<h2_nth_decay<8, Args...>>,
                                   h2_matcher<h2_nth_decay<9, Args...>>>;

   h2_vector<MatcherTuple> matcher_array;
   h2_vector<h2_function<Class, Return(Args...)>> function_array;
   bool greed_mode = true;

   static Return normal_function_stub(Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().origin_fp);
      return I().function_array[index](nullptr, std::forward<Args>(args)...);
   }

   static Return member_function_stub(Class* that, Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().origin_fp);
      return I().function_array[index](that, std::forward<Args>(args)...);
   }

   int matches(Args... args)
   {
      ArgumentTuple at = std::forward_as_tuple(std::forward<Args>(args)...);
      int checkin_offset = -1;
      for (int i = checkin_index; i < checkin_array.size(); ++i) {
         h2_fail* fail = h2_tuple_matches(matcher_array[i], at);
         if (fail) {
            if (checkin_offset != -1) {
               break;
            }
            if (checkin_array[i].is_satisfied()) {
               if (i < checkin_array.size() - 1) { /* try next h2_checkin */
                  delete fail;
                  continue;
               }
            }
            fail->foreach([this](h2_fail* f, int, int) { f->file = file, f->line = line, f->func = origin_fn; });
            h2_fail_g(fail, false);
         } else {
            checkin_index = i;
            checkin_offset = i;
            if (checkin_array[i].is_saturated()) {
               continue;
            }
            if (checkin_array[i].is_not_enough()) {
               break;
            }
            /* satisfied */
            if (greed_mode) {
               break;
            }
            /* continue */
         }
      }
      if (checkin_offset != -1) {
         ++checkin_array[checkin_offset];
      }
      if (checkin_offset == -1) {
         h2_fail_g(h2_fail::new_call(origin_fn, "", "unexpect", {}, file, line), false);
      }
      return checkin_offset;
   }

   void reset() override
   {
      checkin_array.clear();
      matcher_array.clear();
      function_array.clear();
      checkin_index = 0;
      greed_mode = true;
   }

   h2_function<Class, Return(Args...)> origin()
   {
      if (std::is_same<std::false_type, Class>::value)
         return h2_function<Class, Return(Args...)>(std::function<Return(Args...)>((Return(*)(Args...))origin_fp));
      else
         return h2_function<Class, Return(Args...)>(std::function<Return(Class*, Args...)>((Return(*)(Class*, Args...))origin_fp));
   }

 public:
   static h2_mocker& I()
   {
      static h2_mocker* i = nullptr;
      if (!i) i = new h2_mocker();
      return *i;
   }

   static h2_mocker& I(void* origin_fp, const char* origin_fn, const char* file, int line)
   {
      I().origin_fp = origin_fp;
      I().substitute_fp = std::is_same<std::false_type, Class>::value ? (void*)normal_function_stub : (void*)member_function_stub;
      I().origin_fn = origin_fn;
      I().file = file;
      I().line = line;
      I().reset();
      I().mock();
      return I();
   }

   h2_mocker& greed(bool mode)
   {
      greed_mode = mode;
      return *this;
   }

   h2_mocker& once(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      checkin_array.push_back(h2_checkin(1, 1));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& twice(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      checkin_array.push_back(h2_checkin(2, 2));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& times(int count)
   {
      checkin_array.push_back(h2_checkin(count, count));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& any(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      checkin_array.push_back(h2_checkin(0, INT_MAX));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& atleast(int count)
   {
      checkin_array.push_back(h2_checkin(count, INT_MAX));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& atmost(int count)
   {
      checkin_array.push_back(h2_checkin(0, count));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& between(int left, int right)
   {
      checkin_array.push_back(h2_checkin(left, right));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& with(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      if (!matcher_array.empty()) {
         matcher_array.back() = std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9);
      }
      return *this;
   }

   /* clang-format off */
   h2_mocker& th1(h2_matcher<h2_nth_decay<0, Args...>> e = Any) { if (!matcher_array.empty()) std::get<0>(matcher_array.back()) = e; return *this; }
   h2_mocker& th2(h2_matcher<h2_nth_decay<1, Args...>> e = Any) { if (!matcher_array.empty()) std::get<1>(matcher_array.back()) = e; return *this; }
   h2_mocker& th3(h2_matcher<h2_nth_decay<2, Args...>> e = Any) { if (!matcher_array.empty()) std::get<2>(matcher_array.back()) = e; return *this; }
   h2_mocker& th4(h2_matcher<h2_nth_decay<3, Args...>> e = Any) { if (!matcher_array.empty()) std::get<3>(matcher_array.back()) = e; return *this; }
   h2_mocker& th5(h2_matcher<h2_nth_decay<4, Args...>> e = Any) { if (!matcher_array.empty()) std::get<4>(matcher_array.back()) = e; return *this; }
   h2_mocker& th6(h2_matcher<h2_nth_decay<5, Args...>> e = Any) { if (!matcher_array.empty()) std::get<5>(matcher_array.back()) = e; return *this; }
   h2_mocker& th7(h2_matcher<h2_nth_decay<6, Args...>> e = Any) { if (!matcher_array.empty()) std::get<6>(matcher_array.back()) = e; return *this; }
   h2_mocker& th8(h2_matcher<h2_nth_decay<7, Args...>> e = Any) { if (!matcher_array.empty()) std::get<7>(matcher_array.back()) = e; return *this; }
   h2_mocker& th9(h2_matcher<h2_nth_decay<8, Args...>> e = Any) { if (!matcher_array.empty()) std::get<8>(matcher_array.back()) = e; return *this; }
   /* clang-format on */

   h2_mocker& returns()
   {
      if (!function_array.empty()) {
         function_array.back().clear();
      }
      return *this;
   }

   h2_mocker& returns(h2_function<Class, Return(Args...)> r)
   {
      if (!function_array.empty()) {
         function_array.pop_back();
         function_array.push_back(r);
      }
      return *this;
   }

   h2_mocker& does(std::function<Return(Args...)> f)
   {
      if (!function_array.empty()) {
         function_array.pop_back();
         function_array.push_back(h2_function<Class, Return(Args...)>(f));
      }
      return *this;
   }

   h2_mocker& does(std::function<Return(Class*, Args...)> f)
   {
      if (!function_array.empty()) {
         function_array.pop_back();
         function_array.push_back(h2_function<Class, Return(Args...)>(f));
      }
      return *this;
   }

   h2_mocker& operator=(std::function<Return(Args...)> f) { return does(f); }
   h2_mocker& operator=(std::function<Return(Class*, Args...)> f) { return does(f); }
};
// h2_dns.hpp

struct h2_dns {
   static void initialize();
   static void setaddrinfo(int count, ...);
};

struct h2_dnses {
   h2_list dnses;
   void add(h2_list& name);
   void clear();
};

#define H2DNS(...) h2::h2_dns::setaddrinfo(H2PP_NARG(__VA_ARGS__), __VA_ARGS__)
// h2_socket.hpp

struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(size_, data_){};
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
struct h2_packet_matches : h2_matches {
   const M1 from;
   const M2 to;
   const M3 data;
   const M4 size;
   explicit h2_packet_matches(M1 from_, M2 to_, M3 data_, M4 size_) : from(from_), to(to_), data(data_), size(size_) {}

   h2_fail* matches(h2_packet* a, int, bool caseless = false, bool dont = false) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(from).matches(a->from.c_str(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(to).matches(a->to.c_str(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const unsigned char*>(data).matches((unsigned char*)a->data.data(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const int>(size).matches(a->data.length(), 0, caseless, dont));
      return fails;
   }

   virtual h2_string expects(bool caseless, bool dont) const override { return ""; }
};

template <typename M1, typename M2, typename M3, typename M4>
inline h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>> PktEq(M1 from = Any, M2 to = Any, M3 data = Any, M4 size = Any)
{
   return h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>>(h2_packet_matches<M1, M2, M3, M4>(from, to, data, size));
}

struct h2_socket {
   static h2_packet* start_and_fetch();
   // from=1.2.3.4:5678, to=4.3.2.1:8765
   static void inject_received(const void* packet, size_t size, const char* attributes);
};

/* clang-format off */
#define __H2SOCK0(_Packet, _Size, ...) h2::h2_socket::inject_received(_Packet, _Size, #__VA_ARGS__)
#define __H2SOCK1(...) h2::h2_socket::start_and_fetch()
#define H2SOCK(...) H2PP_CAT2(__H2SOCK, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
/* clang-format on */
// h2_stdio.hpp

struct h2_stdio {
   static void initialize();
   static size_t capture_length();
   static const char* capture_cout(const char* type);
   static void capture_cancel();
};

#define H2COUT(...) h2::h2_stdio::capture_cout(#__VA_ARGS__)
// h2_case.hpp

struct h2_case {
   enum { initial = 0,
          passed,
          failed,
          todo,
          filtered,
          ignored,
          statuss };

   const char* name;
   const char* file;
   int line;
   h2_list x;
   int seq = 0;
   int last_status = initial;
   int status = initial;
   int checks = 0;
   long long footprint = 0;
   jmp_buf jump;
   h2_fail* fails{nullptr};
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;
   h2_sock* sock{nullptr};

   h2_case(const char* name, int status, const char* file, int line);
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() {}
   void post_cleanup();

   void do_fail(h2_fail* fail, bool defer);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c);
      ~cleaner();
   };
};
// h2_suite.hpp

struct h2_suite {
   const char* name;
   const char* file;
   int line;
   h2_list x;
   int seq = 0;
   int stats[h2_case::statuss]{0};
   int checks = 0;
   long long footprint = 0;
   jmp_buf jump;
   bool jumpable = false;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list cases;
   h2_stubs stubs;
   h2_mocks mocks;

   h2_suite(const char* name, void (*)(h2_suite*, h2_case*), const char* file, int line);
   void clear();

   void enumerate();
   void execute(h2_case* c);

   void setup();
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
// h2_task.hpp

struct h2_task {
   h2_singleton(h2_task);

   int stats[h2_case::statuss]{0};
   int checks = 0;
   int rounds = 0;
   int last = 0;
   h2_list suites;
   h2_suite* current_suite = nullptr;
   h2_case* current_case = nullptr;
   h2_stubs stubs;
   h2_mocks mocks;
   std::vector<void (*)()> global_setups, global_teardowns;
   std::vector<void (*)()> global_suite_setups, global_suite_teardowns;
   std::vector<void (*)()> global_case_setups, global_case_teardowns;

   void shuffle();
   void shadow();
   void enumerate();
   int execute();
};

static inline void h2_stub_g(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line)
{
   if (!origin_fp || !substitute_fp) return;
   if (h2_task::I().current_case)
      h2_task::I().current_case->stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
   else
      h2_task::I().stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
}

static inline void h2_mock_g(h2_mock* mock)
{
   if (h2_task::I().current_case)
      h2_task::I().current_case->mocks.add(mock);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->mocks.add(mock);
   else
      h2_task::I().mocks.add(mock);
}

static inline void h2_check_g()
{
   if (h2_task::I().current_case) h2_task::I().current_case->checks += 1;
   if (h2_task::I().current_suite) h2_task::I().current_suite->checks += 1;
   h2_task::I().checks += 1;
}

static inline void h2_fail_g(h2_fail* fail, bool defer)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail(fail, defer);
}
// h2_check.hpp

struct h2_defer_fail : h2_once {
   const char* check_type;
   const char *e_expression, *a_expression, *expression;
   const char* file;
   int line;
   h2_fail* fail{nullptr};
   h2_ostringstream oss;

   h2_defer_fail(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_);
   ~h2_defer_fail();
};

static inline h2_ostringstream& h2_OK(h2_defer_fail* d, bool a)
{
   d->check_type = "OK1";
   if (!a) d->fail = h2_fail::new_unexpect("true", "false");
   h2_check_g();
   return d->oss;
}

template <typename E, typename A>
static inline h2_ostringstream& h2_OK(h2_defer_fail* d, E e, A a, int n = 0)
{
   d->check_type = "OK2";
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   h2_fail* fail = m.matches((typename h2_decay<A>::type)a, n);
   d->fail = fail;
   if (fail && fail->subling_next) {
      d->fail = h2_fail::new_unexpect();
      h2_fail::append_child(d->fail, fail);
   }
   h2_check_g();
   return d->oss;
}

static inline h2_ostringstream& h2_JE(h2_defer_fail* d, h2_string e, h2_string a)
{
   d->check_type = "JE";
   h2::h2_matcher<h2_string> m = Je(e);
   d->fail = m.matches(a);
   h2_check_g();
   return d->oss;
}

#define __H2OK(Qt, expression, ...) \
   for (h2::h2_defer_fail Qt("", "", expression, __FILE__, __LINE__); Qt;) h2::h2_OK(&Qt, __VA_ARGS__)

#define __H2JE(Qt, expect, actual) \
   for (h2::h2_defer_fail Qt(#expect, #actual, "", __FILE__, __LINE__); Qt;) h2::h2_JE(&Qt, expect, actual)

#define H2OK(...) __H2OK(H2Q(t_defer_fail), (#__VA_ARGS__), __VA_ARGS__)

#define H2JE(expect, actual) __H2JE(H2Q(t_defer_fail), expect, actual)
// h2_report.hpp

struct h2_report {
   h2_singleton(h2_report);
   static void initialize();

   h2_list reports;
   void on_task_start(h2_task* t);
   void on_task_endup(h2_task* t);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};
}  // namespace h2

/* ======> Interface <====== */

#ifndef SUITE
#   define SUITE H2SUITE
#else
#   pragma message("SUITE conflict, using H2SUITE instead.")
#endif

#ifndef CASE
#   define CASE H2CASE
#else
#   pragma message("CASE conflict, using H2CASE instead.")
#endif

#ifndef TODO
#   define TODO H2TODO
#else
#   pragma message("TODO conflict, using H2TODO instead.")
#endif

#ifndef Case
#   define Case H2Case
#else
#   pragma message("Case conflict, using H2Case instead.")
#endif

#ifndef Todo
#   define Todo H2Todo
#else
#   pragma message("Todo conflict, using H2Todo instead.")
#endif

#ifndef Cleanup
#   define Cleanup H2Cleanup
#else
#   pragma message("Cleanup conflict, using H2Cleanup instead.")
#endif

#ifndef OK
#   define OK H2OK
#else
#   pragma message("OK conflict, using H2OK instead.")
#endif

#ifndef JE
#   define JE H2JE
#else
#   pragma message("JE conflict, using H2JE instead.")
#endif

#ifndef MOCK
#   define MOCK H2MOCK
#else
#   pragma message("MOCK conflict, using H2MOCK instead.")
#endif

#ifndef STUB
#   define STUB H2STUB
#else
#   pragma message("STUB conflict, using H2STUB instead.")
#endif

#ifndef BLOCK
#   define BLOCK H2BLOCK
#else
#   pragma message("BLOCK conflict, using H2BLOCK instead.")
#endif

#ifndef DNS
#   define DNS H2DNS
#else
#   pragma message("DNS conflict, using H2DNS instead.")
#endif

#ifndef SOCK
#   define SOCK H2SOCK
#else
#   pragma message("SOCK conflict, using H2SOCK instead.")
#endif

#ifndef COUT
#   define COUT H2COUT
#else
#   pragma message("COUT conflict, using H2COUT instead.")
#endif

#ifndef GlobalSetup
#   define GlobalSetup H2GlobalSetup
#else
#   pragma message("GlobalSetup conflict, using H2GlobalSetup instead.")
#endif

#ifndef GlobalTeardown
#   define GlobalTeardown H2GlobalTeardown
#else
#   pragma message("GlobalTeardown conflict, using H2GlobalTeardown instead.")
#endif

#ifndef GlobalSuiteSetup
#   define GlobalSuiteSetup H2GlobalSuiteSetup
#else
#   pragma message("GlobalSuiteSetup conflict, using H2GlobalSuiteSetup instead.")
#endif

#ifndef GlobalSuiteTeardown
#   define GlobalSuiteTeardown H2GlobalSuiteTeardown
#else
#   pragma message("GlobalSuiteTeardown conflict, using H2GlobalSuiteTeardown instead.")
#endif

#ifndef GlobalCaseSetup
#   define GlobalCaseSetup H2GlobalCaseSetup
#else
#   pragma message("GlobalCaseSetup conflict, using H2GlobalCaseSetup instead.")
#endif

#ifndef GlobalCaseTeardown
#   define GlobalCaseTeardown H2GlobalCaseTeardown
#else
#   pragma message("GlobalCaseTeardown conflict, using H2GlobalCaseTeardown instead.")
#endif

#ifndef MATCHER
#   define MATCHER H2MATCHER
#else
#   pragma message("MATCHER conflict, using H2MATCHER instead.")
#endif

#ifndef CASES
#   define CASES H2CASES
#else
#   pragma message("CASES conflict, using H2CASES instead.")
#endif

#ifndef CASESS
#   define CASESS H2CASESS
#else
#   pragma message("CASESS conflict, using H2CASESS instead.")
#endif

#ifndef Cases
#   define Cases H2Cases
#else
#   pragma message("Cases conflict, using H2Cases instead.")
#endif

#ifndef Casess
#   define Casess H2Casess
#else
#   pragma message("Casess conflict, using H2Casess instead.")
#endif

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
using h2::CountOf;
using h2::Have;
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
      } H2Q();                                             \
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
   static void QP(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5)

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2Q(h2_suite_test))

#define H2Cleanup()                                \
   if (::setjmp(suite_2_0_1_3_0_1_0_2->jump) == 0) \
      suite_2_0_1_3_0_1_0_2->jumpable = true;      \
   if (!case_2_0_1_7_0_3_2_5)

#define __H2Case(name, status, Qc, Q1, Q2)                                                      \
   static h2::h2_case Qc(name, status, __FILE__, __LINE__);                                     \
   static h2::h2_suite::installer H2Q(i)(suite_2_0_1_3_0_1_0_2, &Qc);                           \
   if (&Qc == case_2_0_1_7_0_3_2_5)                                                             \
      for (h2::h2_suite::cleaner Q1(suite_2_0_1_3_0_1_0_2); Q1; case_2_0_1_7_0_3_2_5 = nullptr) \
         for (h2::h2_case::cleaner Q2(&Qc); Q2;)                                                \
            if (::setjmp(Qc.jump) == 0)

#define H2Case(...) __H2Case(#__VA_ARGS__, h2::h2_case::initial, H2Q(t_case), H2Q(_1), H2Q(_2))
#define H2Todo(...) __H2Case(#__VA_ARGS__, h2::h2_case::todo, H2Q(t_case), H2Q(_1), H2Q(_2))

#define __H2CASE(name, status, QR, QP)                                                    \
   static void QR();                                                                      \
   static void QP(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5) \
   {                                                                                      \
      static h2::h2_case c(name, status, __FILE__, __LINE__);                             \
      static h2::h2_suite::installer i(suite_2_0_1_3_0_1_0_2, &c);                        \
      if (&c == case_2_0_1_7_0_3_2_5)                                                     \
         for (h2::h2_case::cleaner a(&c); a;)                                             \
            if (::setjmp(c.jump) == 0)                                                    \
               QR();                                                                      \
   }                                                                                      \
   static h2::h2_suite H2Q(suite)("Anonymous", &QP, __FILE__, __LINE__);                  \
   static void QR()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, h2::h2_case::initial, H2Q(h2_case_test), H2Q(h2_suite_test))
#define H2TODO(...) __H2CASE(#__VA_ARGS__, h2::h2_case::todo, H2Q(h2_case_test), H2Q(h2_suite_test))

#endif

#include <cassert>  /* assert */
#include <cctype>   /* tolower, isspace */
#include <cstdarg>  /* va_list */
#include <cstdint>  /* int32_t */
#include <errno.h>  /* strerror */
#include <iostream> /* cout */
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
#   define h2_weak_attribute
#else
#   define h2_weak_attribute __attribute__((weak))
#endif

h2_weak_attribute int main(int argc, const char** argv)
{
   h2::h2_option::I().parse(argc, argv);
   return h2::h2_task::I().execute();
}

namespace h2 {

// h2_kit.cpp

#ifdef _WIN32

static inline char* basename(char* path)
{
   static char t[MAX_PATH + 1];
   strcpy(t, path);
   PathRemoveFileSpecA(t);
   return t;
}

#endif

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

h2_inline bool h2_pattern::match(const char* pattern, const char* subject, bool caseless)
{
   return wildcard_match(pattern, subject, caseless) || regex_match(pattern, subject, caseless);
}

static inline long long h2_now()
{
#ifdef _WIN32
   return GetTickCount();
#else
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
#endif
}

static inline void h2_sleep(long long milliseconds)
{
#ifdef _WIN32
   Sleep(milliseconds);
#else
   ::usleep(milliseconds * 1000);
#endif
}

static inline unsigned h2_page_size()
{
   static unsigned s_page_size = 0;
   if (s_page_size == 0) {
#ifdef _WIN32
      SYSTEM_INFO si;
      GetSystemInfo(&si);
      s_page_size = (unsigned)si.dwPageSize;
#else
      s_page_size = sysconf(_SC_PAGESIZE);
#endif
   }
   return s_page_size;
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
// h2_numeric.cpp

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

h2_inline int h2_numeric::extract_hex_string(const char* s, bool ignore_space)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') s += 2;
   for (const char* p = s; *p; p++) {
      if (::isxdigit(*p)) continue;
      if (::isspace(*p) && ignore_space) continue;
      break;
   }
   return 0;
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

h2_inline long long h2_numeric::parse_int_after_equal(const char* s)
{
   long long n = 0;
   const char* p = strchr(s, '=');
   if (p) {
      for (p += 1; *p && ::isspace(*p);) p++;  // strip left space
      if (p[0] == '0' && ::tolower(p[1]) == 'x')
         n = strtoll(p + 2, (char**)0, 16);
      else
         n = strtoll(p, (char**)0, 10);
   }
   return n;
}
// h2_list.cpp

h2_inline void h2_list::__add_between(h2_list* thus, h2_list* prev, h2_list* next)
{
   next->prev = thus;
   thus->next = next;
   thus->prev = prev;
   prev->next = thus;
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

// h2_tinyexpr.cpp
// TINYEXPR - Tiny recursive descent parser and evaluation engine in C
//
// Copyright (c) 2015-2018 Lewis Van Winkle
//
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
            ret = (0.0/0.0);
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
        if (a < 0.0) return (0.0/0.0);
        if (a > UINT_MAX) return (1.0/0.0);
        unsigned int ua = (unsigned int)(a);
        unsigned long int result = 1, i;
        for (i = 1; i <= ua; i++) {
            if (i > ULONG_MAX / result) return (1.0/0.0);
            result *= i;
        }
        return (double)result;
    }
    static double ncr(double n, double r) {
        if (n < 0.0 || r < 0.0 || n < r) return (0.0/0.0);
        if (n > UINT_MAX || r > UINT_MAX) return (1.0/0.0);
        unsigned long int un = (unsigned int)(n), ur = (unsigned int)(r), i;
        unsigned long int result = 1;
        if (ur > un / 2) ur = un - ur;
        for (i = 1; i <= ur; i++) {
            if (result > ULONG_MAX / (un - ur + i)) return (1.0/0.0);
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
                ret->value = (0.0/0.0);
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
        if (!n) return (0.0/0.0);

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
                    default: return (0.0/0.0);
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
                    default: return (0.0/0.0);
                }

            default: return (0.0/0.0);
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
// h2_node.cpp

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
      h2_list_for_each_entry (p, children, h2_json_node, x)
         if (li == index)
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
            _value = h2_stringify((long long)value_double);
         else
            _value = h2_stringify(value_double);
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

   void print(h2_lines& lines, bool fold = false, bool slash = false, int depth = 0, int next = 0)
   {
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back(slash_if(slash) + "\"" + key_string + slash_if(slash) + "\": ");
      if (is_null())
         line.push_back("null");
      else if (is_bool())
         line.push_back(value_boolean ? "true" : "false");
      else if (is_number()) {
         if (value_double - ::floor(value_double) == 0)
            line.push_back(h2_stringify((long long)value_double));
         else
            line.push_back(h2_stringify(value_double));
      } else if (is_string())
         line.push_back(slash_if(slash) + "\"" + value_string + slash_if(slash) + "\"");
      else if (is_pattern())
         line.push_back(slash_if(slash) + "\"/" + value_string + "/" + slash_if(slash) + "\"");
      else if (is_array() || is_object()) {
         h2_lines children_lines;
         h2_list_for_each_entry (p, children, h2_json_node, x)
            p->print(children_lines, fold, slash, depth + 1, children.count() - li - 1);

         line.push_back(is_array() ? "[" : "{");
         if (fold && children_lines.foldable()) {
            line.concat_back(children_lines.folds());
         } else {
            lines.push_back(line), line.clear();
            lines += children_lines;
            line.indent(depth * 2);
         }
         line.push_back(is_array() ? "]" : "}");
      }
      if (line.size()) {
         if (next) line.push_back(", ");
         lines.push_back(line), line.clear();
      }
   }
};
// h2_lexical.cpp

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
// h2_syntax.cpp

struct h2_json_syntax {
   int i = 0;
   const h2_vector<h2_string>& lexical;
   h2_json_syntax(const h2_vector<h2_string>& _lexical) : lexical(_lexical) {}

   bool parse(h2_json_node& root_node)
   {
      return parse_value(root_node);
   }

   h2_string& filter_string(h2_string& s)
   {
      if (s.enclosed('\"'))
         s = s.unquote('\"');
      else if (s.enclosed('\''))
         s = s.unquote('\'');

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
// h2_tree.cpp

struct h2_json_tree : h2_json_node {
   h2_vector<h2_string> lexical;
   h2_json_syntax syntax{lexical};
   bool illformed;
   h2_json_tree(const char* json_string, int json_length = -1)
   {
      h2_json_lexical::parse(lexical, json_string, json_length);
      illformed = !syntax.parse(*this);
   }
   h2_line serialize()
   {
      h2_line line;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            line.printf("yellow,bold,underline", " %s ", lexical[j].c_str());
         else
            line.push_back(" " + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) {
         line.printf("yellow,bold", "...");
      }
      return line;
   }
   h2_lines format()
   {
      h2_lines lines;
      print(lines, O.fold, O.paste);
      return lines;
   }
};
// h2_match.cpp

struct h2_json_match {
   static bool match_array(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      if (e->size() != a->size()) return false;
      h2_list_for_each_entry (p, e->children, h2_json_node, x)
         if (!match(p, a->get(li), caseless))
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
// h2_dual.cpp

struct h2_json_dual : h2_libc {  // combine two node into a dual
   bool key_equal = false, match = false;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   const char *e_class = "blob", *a_class = "blob";
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_lines e_blob, a_blob;
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
         if (e) e->print(e_blob, O.fold, false, depth);
         if (a) a->print(a_blob, O.fold, false, depth);
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

   void align(h2_lines& e_lines, h2_lines& a_lines, h2_list* subling = nullptr)
   {
      if (!strcmp(e_class, "blob")) {
         e_blob.samesizify(a_blob);
         for (auto& line : e_blob) line.brush("cyan");
         for (auto& line : a_blob) line.brush("yellow");

         e_lines += e_blob;
         a_lines += a_blob;
         return;
      }

      h2_line e_line, a_line;
      e_line.indent(depth * 2);
      a_line.indent(depth * 2);

      if (e_key.size()) {
         if (!key_equal) e_line.push_back("\033{green}");
         e_line.push_back(e_key);
         if (!key_equal) e_line.push_back("\033{reset}");
         e_line.push_back(": ");
      }

      if (a_key.size()) {
         if (!key_equal) a_line.push_back("\033{red,bold}");
         a_line.push_back(a_key);
         if (!key_equal) a_line.push_back("\033{reset}");
         a_line.push_back(": ");
      }

      if (!strcmp(e_class, "atomic")) {
         if (e_value.size()) {
            if (!match) e_line.push_back("\033{green}");
            e_line.push_back(e_value);
            if (!match) e_line.push_back("\033{reset}");
         }
         if (a_value.size()) {
            if (!match) a_line.push_back("\033{red,bold}");
            a_line.push_back(a_value);
            if (!match) a_line.push_back("\033{reset}");
         }
      } else if (!strcmp(e_class, "object") || !strcmp(e_class, "array")) {
         h2_lines e_children_lines, a_children_lines;
         h2_list_for_each_entry (p, children, h2_json_dual, x)
            p->align(e_children_lines, a_children_lines, &children);

         e_line.push_back(strcmp(e_class, "object") ? "[" : "{");
         a_line.push_back(strcmp(a_class, "object") ? "[" : "{");
         if (O.fold && e_children_lines.foldable() && a_children_lines.foldable()) {
            e_line.concat_back(e_children_lines.folds());
            a_line.concat_back(a_children_lines.folds());
         } else {
            e_lines.push_back(e_line), e_line.clear();
            e_lines += e_children_lines;
            e_line.indent(depth * 2);
            a_lines.push_back(a_line), a_line.clear();
            a_lines += a_children_lines;
            a_line.indent(depth * 2);
         }
         e_line.push_back(strcmp(e_class, "object") ? "]" : "}");
         a_line.push_back(strcmp(a_class, "object") ? "]" : "}");
      }
      if (e_line.size()) {
         if (has_next(subling, true)) e_line.push_back(", ");
         e_lines.push_back(e_line), e_line.clear();
      }
      if (a_line.size()) {
         if (has_next(subling, false)) a_line.push_back(", ");
         a_lines.push_back(a_line), a_line.clear();
      }
   }
};
// h2_json.cpp

h2_inline h2_lines h2_json::format(const h2_string& json_string)
{
   h2_lines lines;
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) {
      lines.push_back(tree.serialize());
      return lines;
   }
   lines = tree.format();
   if (O.paste) {
      if (0 < lines.size()) lines.front().concat_front("\""), lines.back().push_back("\"");
      unsigned max_width = lines.width();
      for (auto& line : lines) {
         line.padding(max_width - line.width() + 3);
         line.indent(1);
         line.push_back("\\");
      }
   }
   return lines;
}

h2_inline bool h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   return h2_json_match::match(&e_tree, &a_tree, caseless);
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) {
      return false;
   }
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_lines, a_lines);
   return true;
}

// h2_matcher.cpp
// h2_strcmp.cpp

h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expects(caseless, dont));
}
h2_inline h2_string h2_matches_regex::expects(bool caseless, bool dont) const
{
   return CD("Re(" + h2_stringify(e) + ")", caseless, dont);
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expects(caseless, dont));
}
h2_inline h2_string h2_matches_wildcard::expects(bool caseless, bool dont) const
{
   return CD("We(" + h2_stringify(e) + ")", caseless, dont);
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.equals(e, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expects(caseless, dont));
}
h2_inline h2_string h2_matches_strcmp::expects(bool caseless, bool dont) const
{
   return CD("\"" + h2_stringify(e) + "\"", caseless, dont);
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.contains(substring, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(substring, a, expects(caseless, dont));
}
h2_inline h2_string h2_matches_substr::expects(bool caseless, bool dont) const
{
   return CD("Substr(\"" + h2_stringify(substring) + "\")", caseless, dont);
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expects(caseless, dont));
}
h2_inline h2_string h2_matches_startswith::expects(bool caseless, bool dont) const
{
   return CD("StartsWith(\"" + h2_stringify(prefix_string) + "\")", caseless, dont);
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expects(caseless, dont));
}
h2_inline h2_string h2_matches_endswith::expects(bool caseless, bool dont) const
{
   return CD("EndsWith(\"" + h2_stringify(suffix_string) + "\")", caseless, dont);
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if ((h2_json::match(e, a, caseless)) == !dont) return nullptr;
   return h2_fail::new_json(e, a, expects(caseless, dont), caseless);
}
h2_inline h2_string h2_matches_json::expects(bool caseless, bool dont) const
{
   return CD("Je(\"" + h2_stringify(e) + "\")", caseless, dont);
}
// h2_memcmp.cpp

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
   return h2_fail::new_memcmp((const unsigned char*)e, (const unsigned char*)a, width, _nbytes * 8, "", h2_stringify(a), "memcmp " + readable_size(width, _nbytes * 8));
}

h2_inline h2_string h2_matches_bytecmp::expects(bool caseless, bool dont) const
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
   return h2_fail::new_memcmp(_e, (const unsigned char*)a, 1, _nbits, "", h2_stringify(a), "memcmp " + readable_size(1, _nbits));
}

h2_inline h2_string h2_matches_bitcmp::expects(bool caseless, bool dont) const
{
   return CD("Me()", caseless, dont);
}

// h2_piece.cpp

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
      unsigned alignment_2n = alignment;
      if (h2_numeric::not2n(alignment)) alignment_2n = h2_numeric::mask2n(alignment) + 1;
      if (alignment_2n < sizeof(void*)) alignment_2n = sizeof(void*);

      unsigned user_size_plus = (user_size + alignment_2n - 1 + alignment_2n) & ~(alignment_2n - 1);
      page_count = ::ceil(user_size_plus / (double)page_size);

#ifdef _WIN32
      page_ptr = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if (page_ptr == NULL) ::printf("VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      page_ptr = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (page_ptr == MAP_FAILED) ::printf("mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      user_ptr = page_ptr + page_size * page_count - user_size_plus + alignment;

      mark_snowfield();
   }

   ~h2_piece()
   {
#ifdef _WIN32
      VirtualFree(page_ptr, 0, MEM_DECOMMIT | MEM_RELEASE);
#else
      ::munmap(page_ptr, page_size * (page_count + 1));
#endif
   }

   void set_forbidden(unsigned permission, void* page = 0, size_t size = 0)
   {
      if (page) forbidden_page = page;
      if (size) forbidden_size = size;

#ifdef _WIN32
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
         ::printf("mprotect failed %s\n", strerror(errno));
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
      if (!fail)
         fail = check_snowfield(page_ptr, user_ptr);
      return fail;
   }

   h2_fail* leak_check(const char* where, const char* file, int line)
   {
      if (free_times) return nullptr;
      return h2_fail::new_memory_leak(user_ptr, user_size, bt_allocate, where, file, line);
   }

   h2_fail* violate_check()
   {
      if (!violate_times) return nullptr;
      if (violate_after_free)
         return h2_fail::new_use_after_free(user_ptr, violate_address, violate_action, bt_allocate, bt_release, violate_backtrace);
      else
         return h2_fail::new_overflow(user_ptr, user_size, violate_address, violate_action, h2_vector<unsigned char>(), bt_allocate, violate_backtrace);
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

      h2_backtrace bt_release(strcmp("macos", O.os) ? 5 : 6);
      return h2_fail::new_asymmetric_free(user_ptr, who_allocate, who_release, bt_allocate, bt_release);
   }

   h2_fail* check_double_free()
   {
      h2_fail* fail = nullptr;
      h2_backtrace bt(strcmp("macos", O.os) ? 5 : 6);
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
// h2_block.cpp

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
      h2_list_for_each_entry (p, pieces, h2_piece, x) {
         h2_fail* fail1 = p->violate_check();
         if (fail1) return fail1;
         if (!noleak) {
            h2_fail* fail2 = p->leak_check(where, file, line);
            if (fail2) return fail2;
         }
      }
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

      pieces.push(p->x);
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
// h2_stack.cpp

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
      h2_backtrace bt(strcmp("macos", O.os) ? 2 : 3);
      h2_block* b = h2_patch::exempt(bt) ? h2_list_bottom_entry(blocks, h2_block, x) : h2_list_top_entry(blocks, h2_block, x);
      return b ? b->new_piece(who, size, alignment, fill ? *fill : 0, fill, bt) : nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return p->rel_piece(who, piece);
      }
      h2_debug("Warning: free %p not found!", ptr);
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
// h2_override.cpp

// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h
// https://github.com/google/tcmalloc/blob/master/tcmalloc/libc_override.h

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" {
extern malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
}
#endif

struct h2_override {
   h2_singleton(h2_override);

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
      if (size == 0) {
         free(ptr);
         return nullptr;
      }

      h2_piece* old_p = h2_stack::I().get_piece(ptr);
      if (!old_p) return nullptr;

      h2_piece* new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (!new_p) return nullptr;

      memcpy(new_p->user_ptr, old_p->user_ptr, old_p->user_size);
      h2_fail_g(h2_stack::I().rel_piece("free", ptr), false);

      return new_p->user_ptr;
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
      return p ? p->user_size : 0;
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

   h2_override()
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

   void overrides()
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
      stubs.add((void*)::free, (void*)free, "free", __FILE__, __LINE__);
      stubs.add((void*)::malloc, (void*)malloc, "malloc", __FILE__, __LINE__);
      stubs.add((void*)::realloc, (void*)realloc, "realloc", __FILE__, __LINE__);
      stubs.add((void*)::calloc, (void*)calloc, "calloc", __FILE__, __LINE__);
      stubs.add((void*)::_aligned_malloc, (void*)_aligned_malloc, "_aligned_malloc", __FILE__, __LINE__);
#else
#endif
#if defined _ISOC11_SOURCE
      stubs.add((void*)::aligned_alloc, (void*)aligned_alloc, "aligned_alloc", __FILE__, __LINE__);
#endif
      stubs.add((void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t)) operator new), "new", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&)) operator new), "new", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t)) operator new[]), "new[]", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&)) operator new[]), "new[]", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*)) operator delete), "delete", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&)) operator delete), "delete", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*)) operator delete[]), "delete[]", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&)) operator delete[]), "delete[]", __FILE__, __LINE__);
   }

   void restores()
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
};
// h2_crash.cpp

struct h2_crash {
#ifndef _WIN32
   // https://www.gnu.org/software/libsigsegv/
   static void segment_fault_handler(int sig, siginfo_t* si, void* unused)
   {
      h2_piece* piece = h2_stack::I().host_piece(si->si_addr);
      if (piece) {
         piece->violate_forbidden(si->si_addr);
      } else {
         h2_debug();
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
// h2_memory.cpp

h2_inline void h2_memory::initialize()
{
   if (O.memory_check && !O.debug) h2_crash::install_segment_fault_handler();
   stack::root();
   if (O.memory_check) h2_override::I().overrides();
}
h2_inline void h2_memory::finalize()
{
   if (O.memory_check) h2_override::I().restores();
}
h2_inline void h2_memory::overrides()
{
   if (O.memory_check) h2_override::I().overrides();
}
h2_inline void h2_memory::restores()
{
   if (O.memory_check) h2_override::I().restores();
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
      n_limit = h2_numeric::parse_int_after_equal(p_limit);
   }

   const char* p_align = strcasestr(attributes, "align");
   if (p_align) {
      n_align = (int)h2_numeric::parse_int_after_equal(p_align);
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

// h2_e9.cpp
// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_e9 {
   static constexpr int size = sizeof(void*) == 8 ? 2 + 8 + 2 : 5;

   static bool save(void* origin_fp, unsigned char* saved)
   {
#ifdef _WIN32
      DWORD t;
      if (!VirtualProtect(origin_fp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t)) {  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
         ::printf("STUB failed: VirtualProtect PAGE_EXECUTE_READWRITE %d\n", GetLastError());
         return false;
      }
#else
      /* uintptr_t is favourite, but is optional type in c++ std, using unsigned long long for portable */
      unsigned long long page_size = (unsigned long long)h2_page_size();
      unsigned long long origin_start = reinterpret_cast<unsigned long long>(origin_fp);
      unsigned long long page_start = origin_start & ~(page_size - 1);
      int page_count = ::ceil((origin_start + size - page_start) / (double)page_size);

      if (mprotect(reinterpret_cast<void*>(page_start), page_count * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         ::printf("STUB failed: mprotect PROT_READ | PROT_WRITE | PROT_EXEC %s\n", strerror(errno));
         return false;
      }
#endif
      if (saved) memcpy(saved, origin_fp, size);
      return true;
   }

   static void set(void* origin_fp, void* substitute_fp)
   {
      unsigned char* I = reinterpret_cast<unsigned char*>(origin_fp);
      ptrdiff_t delta = (unsigned char*)substitute_fp - (unsigned char*)origin_fp - 5;

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
      if (delta < INT_MIN || INT_MAX < delta) {  //x86_64 asm("movq $substitute_fp, %rax; jmpq %rax")
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &substitute_fp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {  //i386 asm("jmp offset")
         unsigned char C[] = {0xE9, 0, 0, 0, 0};
         *(int32_t*)(&C[1]) = delta;
         memcpy(I, C, sizeof(C));
      }
#endif
   }

   static void reset(void* origin_fp, unsigned char* saved)
   {
      memcpy(origin_fp, saved, size);
   }
};
// h2_native.cpp

struct h2_native : h2_libc {
   h2_list x;
   unsigned char saved[32];
   void* origin_fp;
   int reference_count = 0;
   h2_native(void* _origin_fp) : origin_fp(_origin_fp)
   {
      h2_e9::save(origin_fp, saved);
   }
   ~h2_native()
   {
      restore();
   }
   void restore()
   {
      h2_e9::reset(origin_fp, saved);
   }
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
// h2_stub.cpp

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
         ::printf("STUB failed: %s %s:%d\n", origin_fn, file, line);
         return false;
      }
      stub = new h2_stub(origin_fp);
      stubs.push(stub->x);
   }
   stub->stub(substitute_fp);
   return true;
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

// h2_checkin.cpp

h2_inline h2_fail* h2_checkin::check(const char* func, const char* file, int line)
{
   if (is_satisfied() || is_saturated()) return nullptr;
   return h2_fail::new_call(func, expect(), actual(), {}, file, line);
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
   } else if (least == most)
      sprintf(st, "exactly %d times", least);
   else if (most == INT_MAX)
      sprintf(st, "at least %d times", least);
   else  // 0 < least < most < INT_MAX
      sprintf(st, "between %d and %d times", least, most);

   return st;
}
// h2_mock.cpp

h2_inline void h2_mock::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(origin_fp, substitute_fp, origin_fn, file, line);
}

h2_inline h2_fail* h2_mock::times_check()
{
   h2_fail* fail = nullptr;
   for (auto& c : checkin_array) {
      h2_fail::append_subling(fail, c.check(origin_fn, file, line));
      h2_check_g();
   }
   return fail;
}

h2_inline bool h2_mocks::add(h2_mock* mock)
{
   h2_list_for_each_entry (p, mocks, h2_mock, x)
      if (p == mock) return false;
   mocks.push(mock->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(bool check)
{
   h2_fail* fail = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mock, x) {
      if (check) h2_fail::append_subling(fail, p->times_check());
      p->reset();
      p->x.out();
   }
   return fail;
}

// h2_dns.cpp

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
// h2_socket.cpp

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
      h2_sock* sock = h2_list_top_entry(I().socks, h2_sock, y);

      do {
         h2_list_for_each_entry (p, sock->incoming, h2_packet, x) {
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
      h2_sock* sock = h2_list_top_entry(I().socks, h2_sock, y);
      sock->sockets.push_back({fd, c, tcp->from.c_str()});
      if (tcp->data.length())
         sock->incoming.push(tcp->x);
      else
         delete tcp;

      return fd;
   }

   static int connect(int socket, const struct sockaddr* address, socklen_t address_len)
   {
      h2_sock* sock = h2_list_top_entry(I().socks, h2_sock, y);
      sock->sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      if (tcp->data.length())
         sock->incoming.push(tcp->x);
      else
         delete tcp;
      return 0;
   }

   static ssize_t send(int socket, const void* buffer, size_t length, int flags)
   {
      h2_sock* sock = h2_list_top_entry(I().socks, h2_sock, y);
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
      h2_sock* sock = h2_list_top_entry(I().socks, h2_sock, y);
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
   stubs.add((void*)::sendto, (void*)h2__socket::sendto, "sendto", __FILE__, __LINE__);
   stubs.add((void*)::recvfrom, (void*)h2__socket::recvfrom, "recvfrom", __FILE__, __LINE__);
#ifndef _WIN32
   stubs.add((void*)::sendmsg, (void*)h2__socket::sendmsg, "sendmsg", __FILE__, __LINE__);
   stubs.add((void*)::recvmsg, (void*)h2__socket::recvmsg, "recvmsg", __FILE__, __LINE__);
#endif
   stubs.add((void*)::send, (void*)h2__socket::send, "send", __FILE__, __LINE__);
   stubs.add((void*)::recv, (void*)h2__socket::recv, "recv", __FILE__, __LINE__);
   stubs.add((void*)::accept, (void*)h2__socket::accept, "accept", __FILE__, __LINE__);
   stubs.add((void*)::connect, (void*)h2__socket::connect, "connect", __FILE__, __LINE__);
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
   outgoing.push_back((new h2_packet(from, to, data, size))->x);
}

h2_inline void h2_sock::put_outgoing(int fd, const char* data, size_t size)
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

h2_inline void h2_sock::put_incoming(const char* from, const char* to, const char* data, size_t size)
{
   incoming.push_back((new h2_packet(strlen(from) ? from : last_to, to, data, size))->x);
}

h2_inline h2_packet* h2_socket::start_and_fetch()
{
   if (!h2_task::I().current_case) return nullptr;

   h2_sock* sock = h2_task::I().current_case->sock;
   if (!sock) {
      sock = h2_task::I().current_case->sock = new h2_sock();
      h2__socket::I().socks.push(sock->y);
   }
   return h2_list_pop_entry(sock->outgoing, h2_packet, x);
}

static inline void extract_iport_after_equal(const char* s, char* o)
{
   const char* p = strchr(s, '=');
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

h2_inline void h2_socket::inject_received(const void* packet, size_t size, const char* attributes)
{
   h2_sock* sock = h2_list_top_entry(h2__socket::I().socks, h2_sock, y);
   char from[256], to[256];
   parse_sock_attributes(attributes, from, to);
   if (sock) sock->put_incoming(from, to, (const char*)packet, size);
}
// h2_stdio.cpp

struct h2__stdio {
   h2_singleton(h2__stdio);
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

   h2__stdio()
   {
#ifndef _WIN32
      stubs.add((void*)::write, (void*)write, "write", __FILE__, __LINE__);
#endif
#if defined __APPLE__ || defined _WIN32
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
#endif
#ifndef _WIN32
      stubs.add((void*)::syslog, (void*)syslog, "syslog", __FILE__, __LINE__);
      stubs.add((void*)::vsyslog, (void*)vsyslog, "vsyslog", __FILE__, __LINE__);
#endif
   }

   const char* start_capture(bool _stdout, bool _stderr, bool _syslog)
   {
      capturing = true;
      stdout_capturable = _stdout;
      stderr_capturable = _stderr;
      syslog_capturable = _syslog;
      buffer->clear();
      return buffer->c_str();
   }

   const char* stop_capture()
   {
      capturing = false;
      stdout_capturable = stderr_capturable = syslog_capturable = false;
      buffer->push_back('\0');
      return buffer->c_str();
   }

   bool capturing = false;
   const char* toggle_capture()
   {
      if (capturing)
         return stop_capture();
      else
         return start_capture(true, true, true);
   }
};

h2_inline void h2_stdio::initialize()
{
   ::setbuf(stdout, 0);  // unbuffered
   h2__stdio::I().buffer = new h2_string();
}

h2_inline size_t h2_stdio::capture_length()
{
   return h2__stdio::I().capture_length;
}

h2_inline const char* h2_stdio::capture_cout(const char* type)
{
   if (!strlen(type)) return h2__stdio::I().toggle_capture();
   if (strcasestr(type, "stop")) return h2__stdio::I().stop_capture();
   return h2__stdio::I().start_capture(strcasestr(type, "out"), strcasestr(type, "err"), strcasestr(type, "syslog"));
}

h2_inline void h2_stdio::capture_cancel()
{
   h2__stdio::I().stop_capture();
}

// h2_color.cpp

struct h2__color {
   h2_singleton(h2__color);

   char current[8][32];
   int default_attribute;
#ifdef _WIN32
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

   h2__color()
   {
      memset(current, 0, sizeof(current));
      default_attribute = 0;
#ifdef _WIN32
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
            if (h2_option::I().colorfull) I().change();
         } else {
            h2_libc::write(fileno(stdout), p, 1);
         }
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
#ifdef _WIN32
            return K[i].attribute;
#else
            return K[i].value;
#endif

      return default_attribute;
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

h2_inline void h2_color::printf(const h2_line& line)
{
   for (auto& word : line)
      h2__color::I().print(word.c_str());
   h2__color::I().print("\n");
}

h2_inline void h2_color::printf(const h2_lines& lines)
{
   for (auto& line : lines) printf(line);
}
// h2_debug.cpp

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
// h2_failure.cpp

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

static inline h2_line h2_acronym(const h2_string& s, int width = 16, int tail = 0)
{
   if (s.size() <= width) {
      h2_string t;
      for (int i = 0; i < s.size(); ++i) escape_char(s.c_str()[i], t);
      return {t};
   } else {
      h2_string t1, t2;
      for (int i = 0; i < width - 3 - tail; ++i) escape_char(s.c_str()[i], t1);
      for (int i = 0; i < tail; ++i) escape_char(s.c_str()[s.size() - tail + i], t2);
      return {t1, "\033{+dark gray}", "...", "\033{-dark gray}", t2};
   }
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

h2_inline const char* h2_fail::locate()
{
   static char st[1024];
   strcpy(st, "");

   if (func && strlen(func)) sprintf(st + strlen(st), " in %s(%s)", func, h2::h2_numeric::sequence_number(seq));
   if (file && strlen(file) && 0 < line) sprintf(st + strlen(st), " at %s:%d", file, line);
   return st;
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index, int child_index)
{
   cb(this, subling_index, child_index);
   if (child_next) child_next->foreach(cb, 0, child_index + 1);
   if (subling_next) subling_next->foreach(cb, subling_index + 1, child_index);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_line& explain_ = {}, const char* func_ = nullptr, const char* file_ = nullptr, int line_ = 0) : h2_fail(explain_, file_, line_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_line line;
      line.indent(child_index * 2 + 1);
      if (0 <= seq) line.printf("dark gray", "%d. ", seq);
      line.concat_back(explain);
      if (strlen(locate())) line.printf("", ",%s", locate());
      h2_color::printf(line);
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   static const char* s_null[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0", nullptr};
   static const char* s_true[] = {"IsTrue", "true", "1", nullptr};
   static const char* s_false[] = {"IsFalse", "false", "0", nullptr};
   static const char** S[] = {s_null, s_true, s_false};

   for (int i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
      if (h2_in(a.c_str(), S[i]) && h2_in(b.c_str(), S[i]))
         return true;

   return false;
}

static inline void quote_acronym_print(h2_line& line, const h2_string& str, const char* style)
{
   if (str.enclosed('\"')) line.printf("dark gray", "\"");
   line.concat_back(h2_acronym(str.unquote('\"'), O.verbose ? 10000 : 30, str.enclosed('\"') ? 2 : 3), style);
   if (str.enclosed('\"')) line.printf("dark gray", "\"");
}

struct h2_fail_unexpect : h2_fail {
   h2_string e_represent, a_represent;
   h2_string expection;
   h2_fail_unexpect(const h2_string& e_represent_ = "", const h2_string& a_represent_ = "", const h2_string& expection_ = "", const h2_line& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(explain_, file_, line_), e_represent(e_represent_), a_represent(a_represent_), expection(expection_) {}

   void print_OK1(h2_line& line)
   {
      line.push_back("OK( ");
      line.concat_back(h2_acronym(a_expression, O.verbose ? 10000 : 30), "cyan");
      line.push_back(" ) is ");
      line.printf("bold,red", "false");
   }
   void print_OK2(h2_line& line)
   {
      line.push_back("OK( ");

      if (!expection.size() || e_expression == expection || is_synonym(e_expression, expection)) {
         quote_acronym_print(line, e_expression, "green");
      } else {
         quote_acronym_print(line, e_expression, "cyan");
         line.printf("dark gray", "==>");
         quote_acronym_print(line, expection, "green");
      }

      line.push_back(", ");

      if (!a_represent.size() || a_expression == a_represent || is_synonym(a_expression, a_represent)) {
         quote_acronym_print(line, a_expression, "bold,red");
      } else {
         quote_acronym_print(line, a_represent, "bold,red");
         line.printf("dark gray", "<==");
         quote_acronym_print(line, a_expression, "cyan");
      }

      line.push_back(" )");
   }
   void print_JE(h2_line& line)
   {
      line.push_back("JE( ");
      line.concat_back(h2_acronym(e_expression, O.verbose ? 10000 : 30, 2), "cyan");
      line.push_back(", ");
      line.concat_back(h2_acronym(a_expression, O.verbose ? 10000 : 30, 2), "bold,red");
      line.push_back(" )");
   }
   void print_Inner(h2_line& line)
   {
      if (0 <= seq) line.printf("dark gray", "%d. ", seq);
      line.push_back("expect is ");
      quote_acronym_print(line, expection, "green");
      line.push_back(", actual is ");
      quote_acronym_print(line, a_represent, "bold,red");
   }

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_line line;
      line.indent(child_index * 2 + 1);
      if (!strcmp("Inner", check_type)) print_Inner(line);
      if (!strcmp("OK1", check_type)) print_OK1(line);
      if (!strcmp("OK2", check_type)) print_OK2(line);
      if (!strcmp("JE", check_type)) print_JE(line);
      if (explain.width()) {
         line.push_back(" ");
         line.concat_back(explain);
         line.push_back(",");
      }
      if (user_explain.size()) line.printf("", " %s,", user_explain.c_str());
      line.push_back(locate());
      h2_color::printf(line);
   }
};

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

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_string& expection_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(h2_quote_stringfiy(e_value_), h2_quote_stringfiy(a_value_), expection_, {}, file_, line_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
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

         h2_color::printf(h2_layout::unified(e_line, a_line, "expect", "actual", O.term_size));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(h2_quote_stringfiy(e_value_), h2_quote_stringfiy(a_value_), expection_, {}, file_, line_), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
         h2_line e_line, a_line;
         for (size_t i = 0; i < e_value.size(); ++i)
            fmt_char(e_value[i], true, "", e_line);

         for (size_t i = 0; i < a_value.size(); ++i)
            fmt_char(a_value[i], true, "", a_line);

         h2_color::printf(h2_layout::seperate(e_line, a_line, "expect", "actual", O.term_size));
      }
   }
};

struct h2_fail_json : h2_fail_unexpect {
   h2_string e_value, a_value;
   const bool caseless;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection_, bool caseless_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(e_value_, a_value_, expection_, {}, file_, line_), e_value(e_value_), a_value(a_value_), caseless(caseless_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_lines e_lines, a_lines;
      bool illformed = !h2_json::diff(e_value, a_value, e_lines, a_lines, caseless);
      if (illformed) explain.push_back("illformed json");
      h2_fail_unexpect::print(subling_index, child_index);
      if (O.paste || illformed) {
         e_lines = h2_json::format(e_value);
         a_lines = h2_json::format(a_value);
         h2_color::printf("dark gray", "expect");
         h2_color::printf("bold,green", ">\n");
         h2_color::printf(e_lines);
         h2_color::printf("dark gray", "actual");
         h2_color::printf("bold,red", ">\n");
         h2_color::printf(a_lines);
      } else {
         h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual", (O.seq ? 1 : 0), 'd', O.term_size - 1);
         for (auto& line : lines) line.indent(1);
         h2_color::printf(lines);
      }
   }
};

struct h2_fail_memcmp : h2_fail_unexpect {
   h2_vector<unsigned char> e_value, a_value;
   const int width, nbits;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, const h2_string& expection_, const h2_string& a_represent_, const h2_line& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("", a_represent_, expection_, explain_, file_, line_), e_value(e_value_, e_value_ + (nbits_ + 7) / 8), a_value(a_value_, a_value_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}

   void print(int subling_index, int child_index) override
   {
      h2_fail_unexpect::print(subling_index, child_index);
      h2_lines e_lines, a_lines;
      int bytes_per_row = 0;
      switch (width) {
      case 1: print_bits(e_lines, a_lines, bytes_per_row = 4); break;
      case 8: print_ints<unsigned char>(e_lines, a_lines, bytes_per_row = (O.term_size < 80 ? 8 : 16)); break;
      case 16: print_ints<unsigned short>(e_lines, a_lines, bytes_per_row = 16); break;
      case 32: print_ints<unsigned int>(e_lines, a_lines, bytes_per_row = 16); break;
      case 64: print_ints<unsigned long long>(e_lines, a_lines, bytes_per_row = 16); break;
      default: break;
      }
      h2_color::printf(h2_layout::split(e_lines, a_lines, "expect", "actual", bytes_per_row * 8 / width, 'x', O.term_size));
   }

   void print_bits(h2_lines& e_lines, h2_lines& a_lines, int bytes_per_row)
   {
      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (int j = 0; j < bytes_per_row; ++j) {
            if (j) e_line.push_back(" ");
            if (j) a_line.push_back(" ");
            for (int k = 0; k < 8; ++k) {
               if ((i * bytes_per_row + j) * 8 + k < nbits) {
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
   void print_ints(h2_lines& e_lines, h2_lines& a_lines, int bytes_per_row)
   {
      char fmt[32];
      sprintf(fmt, "%%s%%0%dX", (int)sizeof(T) * 2);

      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (int j = 0; j < bytes_per_row; j += sizeof(T)) {
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
   const int size;
   const h2_backtrace bt_allocate, bt_release;

   h2_fail_memory(const void* ptr_, const int size_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail({}, file_, line_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, int size_, const h2_backtrace& bt_allocate_, const char* where_, const char* file_, int line_)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), where(where_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::printf("", " %p", ptr);
      h2_color::printf("bold,red", " memory leak");
      h2_color::printf("red", " %d", size);
      h2_color::printf("", " bytes in %s totally%s\n", where, locate());
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::printf("", " %p", ptr);
      h2_color::printf("bold,red", " double free");
      h2_color::printf("", " at backtrace:\n", ptr), bt_double_free.print(2);
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::printf("", "  already free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::printf("", " %p allocate with ", ptr);
      h2_color::printf("bold,red", "%s", who_allocate);
      h2_color::printf("", ", release by ");
      h2_color::printf("bold,red", "%s", who_release);
      h2_color::printf("", " asymmetrically at backtrace:\n"), bt_release.print(2);
      if (0 < bt_allocate.count) h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_overflow : h2_fail_memory {
   const void* addr;                    /* 犯罪地点 */
   const char* action;                  /* 犯罪行为 */
   const h2_vector<unsigned char> spot; /* 犯罪现场 */
   const h2_backtrace bt_trample;       /* 犯罪过程 */
   h2_fail_overflow(const void* ptr_, const int size_, const void* addr_, const char* action_, h2_vector<unsigned char> spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), addr(addr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      int offset = ptr < addr ? (long long)addr - ((long long)ptr + size) : (long long)addr - (long long)ptr;
      h2_color::printf("", " %p %+d (%p)", ptr, offset, addr);
      h2_color::printf("bold,red", " %s", action);
      h2_color::printf("", " %s ", offset >= 0 ? "overflow" : "underflow");

      for (int i = 0; i < spot.size(); ++i)
         h2_color::printf("bold,red", "%02X ", spot[i]);

      h2_color::printf("", ",%s\n", locate());
      if (bt_trample.count) h2_color::printf("", "  trampled at backtrace:\n"), bt_trample.print(3);
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
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
      h2_color::printf("", " %p %+d (%p)", ptr, (long long)addr - (long long)ptr, addr);
      h2_color::printf("bold,red", " %s after free", action);
      h2_color::printf("", " at backtrace:\n"), bt_use.print(2);
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::printf("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_call : h2_fail {
   h2_string e_who, e_call, a_call;
   h2_fail_call(const char* func_, const char* expect, const char* actual, const h2_line& explain_ = {}, const char* file_ = nullptr, int line_ = 0) : h2_fail(explain_, file_, line_), e_who(func_), e_call(expect), a_call(actual) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_line line = {" ", e_who, "() "};
      if (e_call.size()) {
         line.push_back("expected ");
         line.printf("green", e_call.c_str());
         line.push_back(" calls but ");
      }
      line.printf("red,bold", a_call.c_str());
      line.push_back(" called");
      if (e_call.size()) line.push_back(" actually");
      line.push_back(locate());
      h2_color::printf(line);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_line& explain, const char* func, const char* file, int line)
{
   return new h2_fail_normal(explain, func, file, line);
}

h2_inline h2_fail* h2_fail::new_unexpect(const h2_string& e_represent, const h2_string& a_represent, const h2_string& expection, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_unexpect(e_represent, a_represent, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_string& expection, const char* file, int line)
{
   return new h2_fail_strcmp(e_value, a_value, caseless, expection, file, line);
}
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, const char* file, int line)
{
   return new h2_fail_strfind(e_value, a_value, expection, file, line);
}
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, bool caseless, const char* file, int line)
{
   return new h2_fail_json(e_value, a_value, expection, caseless, file, line);
}
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& expection, const h2_string& a_represent, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_memcmp(e_value, a_value, width, nbits, expection, a_represent, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, int size, const h2_backtrace& bt_allocate, const char* where, const char* file, int line)
{
   return new h2_fail_memory_leak(ptr, size, bt_allocate, where, file, line);
}
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free)
{
   return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free);
}
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release)
{
   return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release);
}
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const int size, const void* addr, const char* action, h2_vector<unsigned char> spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file, int line)
{
   return new h2_fail_overflow(ptr, size, addr, action, spot, bt_allocate, bt_trample, file, line);
}
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* addr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use)
{
   return new h2_fail_use_after_free(ptr, addr, action, bt_allocate, bt_release, bt_use);
}
h2_inline h2_fail* h2_fail::new_call(const char* func, const char* expect, const char* actual, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_call(func, expect, actual, explain, file, line);
}
// h2_libc.cpp

struct h2_libc_malloc {
   h2_singleton(h2_libc_malloc);

   struct buddy {
      size_t size;
      h2_list x;
   };

   int pages = 0;
   h2_list buddies;

   void merge()
   {
      buddy* b = nullptr;
      h2_list_for_each_entry (p, buddies, buddy, x) {
         if (b) {
            if (((char*)b) + b->size == (char*)p) {
               b->size += p->size;
               p->x.out();
               continue;
            }
         }
         b = p;
      }
   }

   void insert(buddy* b)
   {
      buddy* n = nullptr;
      h2_list_for_each_entry (p, buddies, buddy, x) {
         if (((char*)b) + b->size <= (char*)p) {
            n = p;
            break;
         }
      }
      if (n)
         n->x.add_before(b->x);
      else
         buddies.add_tail(b->x);

      merge();
   }

   void* malloc(size_t size)
   {
      size = (size + 7) / 8 * 8;
      buddy* b = nullptr;
      h2_list_for_each_entry (p, buddies, buddy, x) {
         if (size <= p->size - sizeof(p->size)) {
            b = p;
            p->x.out();
            break;
         }
      }
      if (!b) {
         int page_size = h2_page_size();
         int page_count = ::ceil((size + sizeof(b->size)) / (double)page_size);
         if (pages == 0) page_count = 1024 * 25;
         pages += page_count;
#ifdef _WIN32
         PVOID ptr = VirtualAlloc(NULL, page_count * page_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
         if (ptr == NULL) ::printf("VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
         void* ptr = ::mmap(nullptr, page_count * page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
         if (ptr == MAP_FAILED) ::printf("mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif
         b = (buddy*)ptr;
         b->size = page_count * page_size;
      }
      size_t bz = b->size;
      b->size = size + sizeof(b->size);
      size_t rz = bz - b->size;
      if (sizeof(buddy) <= rz) {
         buddy* r = (buddy*)(((char*)b) + b->size);
         r->size = rz;
         insert(r);
      } else {
         b->size += rz;
      }

      return (void*)&b->x;
   }

   void free(void* ptr)
   {
      buddy* b = (buddy*)(((char*)ptr) - sizeof(b->size));
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
// h2_line.cpp

h2_inline unsigned h2_line::width(bool ignore_indent) const
{
   unsigned w = 0;
   for (auto& word : *this)
      if (!h2_color::is_ctrl(word.c_str()))
         if (!ignore_indent || !word.isspace())
            w += word.size();
   return w;
}

h2_inline h2_line& h2_line::indent(int n, const char c)
{
   insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::padding(int n, const char c)
{
   push_back(h2_string(n, c));
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

h2_inline h2_line& h2_line::concat_back(const h2_line& line, const char* style)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   insert(end(), line.begin(), line.end());
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_line& h2_line::concat_front(const h2_line& line_or_word, const char* style)
{
   if (style && strlen(style)) insert(begin(), "\033{reset}");
   insert(begin(), line_or_word.begin(), line_or_word.end());
   if (style && strlen(style)) insert(begin(), "\033{" + h2_string(style) + "}");
   return *this;
}

h2_inline void h2_line::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
}

h2_inline void h2_line::samesizify(h2_line& b)
{
   int w = width(), b_w = b.width();
   padding(std::max(w, b_w) - w);
   b.padding(std::max(w, b_w) - b_w);
}

h2_inline h2_lines& h2_lines::operator+=(const h2_lines& lines)
{
   insert(end(), lines.begin(), lines.end());
   return *this;
}

h2_inline h2_lines& h2_lines::concat_front(const h2_lines& lines)
{
   insert(begin(), lines.begin(), lines.end());
   return *this;
}

h2_inline h2_lines& h2_lines::concat_front(const h2_line& line)
{
   insert(begin(), line);
   return *this;
}

h2_inline unsigned h2_lines::width() const
{
   unsigned m = 0;
   for (auto& line : *this)
      m = std::max(m, line.width());
   return m;
}

h2_inline bool h2_lines::foldable(unsigned width)
{
   int sum = 0;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace() && !h2_color::is_ctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.length();

   return sum < width;
}

h2_inline h2_line h2_lines::folds()
{
   h2_line folded_line;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace())  // drop indent
            folded_line.push_back(word);
   return folded_line;
}

h2_inline void h2_lines::sequence(unsigned indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      h2_line t;
      t.printf("dark gray", "%lu. ", i + start);
      at(i).concat_front(t);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_lines::samesizify(h2_lines& b)
{
   int max_y = std::max(size(), b.size());
   for (size_t i = size(); i < max_y; ++i) push_back(h2_line());
   for (size_t i = b.size(); i < max_y; ++i) b.push_back(h2_line());
}
// h2_report.cpp

struct h2_report_impl {
   h2_list x;
   int suites = 0, cases = 0;
   int suite_index = 0, suite_case_index = 0, task_case_index = 0;
   long long task_cost = 0, suite_cost = 0, case_cost = 0;

   virtual void on_task_start(h2_task* t)
   {
      suites = t->suites.count();
      h2_list_for_each_entry (s, t->suites, h2_suite, x) {
         cases += s->cases.count();
      }
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

struct h2_report_console : h2_report_impl {
   void print_percentage(bool percent = true, bool number = false)
   {
      static size_t last = 0;
      h2_color::printf("", h2_stdio::capture_length() == last ? "\r" : "\n");
      if (percent) {
         h2_color::printf("dark gray", "[");
         h2_color::printf("", "%3d%%", cases ? (int)(task_case_index * 100 / cases) : 100);
         h2_color::printf("dark gray", "] ");
      }
      if (number) {
         h2_color::printf("dark gray", "(");
         h2_color::printf("", "%d", task_case_index);
         h2_color::printf("dark gray", "/");
         h2_color::printf("", "%d", cases);
         h2_color::printf("dark gray", ") ");
      }
      last = h2_stdio::capture_length();
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
      if (c++) h2_color::printf("dark gray", ",");
      h2_color::printf(style, "%d", n);
      h2_color::printf("", " %s", name);
   }
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   void on_task_endup(h2_task* t) override
   {
      h2_report_impl::on_task_endup(t);
      if (O.listing) {
         h2_color::printf("bold,green", "Listing <%d suites, %d cases, %d todo>\n", suites, cases - t->stats[h2_case::todo], t->stats[h2_case::todo]);
      } else {
         print_percentage(false, false);
         if (O.verbose)
            h2_color::printf("", "\n");
         if (0 < t->stats[h2_case::failed])
            h2_color::printf("bold,red", "Failure ");
         else
            h2_color::printf("bold,green", "Success ");

         if (0 < nonzero_count(t->stats[h2_case::failed], t->stats[h2_case::todo], t->stats[h2_case::filtered], t->stats[h2_case::ignored]))
            h2_color::printf("dark gray", "(");

         int c = 0;
         comma_status(t->stats[h2_case::passed], "green", "passed", c);
         if (t->stats[h2_case::failed]) comma_status(t->stats[h2_case::failed], "red", "failed", c);
         if (t->stats[h2_case::todo]) comma_status(t->stats[h2_case::todo], "yellow", "todo", c);
         if (t->stats[h2_case::filtered]) comma_status(t->stats[h2_case::filtered], "blue", "filtered", c);
         if (t->stats[h2_case::ignored]) comma_status(t->stats[h2_case::ignored], "blue", "ignored", c);
         if (0 < nonzero_count(t->stats[h2_case::failed], t->stats[h2_case::todo], t->stats[h2_case::filtered], t->stats[h2_case::ignored])) {
            h2_color::printf("dark gray", ")");
            h2_color::printf("", " %d", cases);
         }
         h2_color::printf("", " case%s", 1 < cases ? "s" : "");
         h2_color::printf("dark gray", ",");
         h2_color::printf("", "%d check%s", t->checks, 1 < t->checks ? "s" : "");
         if (1 < t->rounds) {
            h2_color::printf("dark gray", ",");
            h2_color::printf("", "%d rounds", t->rounds);
         }
         h2_color::printf("dark gray", ",");
         h2_color::printf("", "%s \n", format_duration(task_cost));
      }
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      if (O.listing) {
         h2_color::printf("", "SUITE%d. ", ++suite_index);
         h2_color::printf("bold,blue", "%s", s->name);
         h2_color::printf("", " %s:%d\n", s->file, s->line);
      }
   }

   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.verbose && O.includes.size() + O.excludes.size() == 0) {
         print_percentage();
         h2_color::printf("", "%s", s->name);
         if (1 < nonzero_count(s->stats[h2_case::passed], s->stats[h2_case::failed], s->stats[h2_case::todo], s->stats[h2_case::filtered], s->stats[h2_case::ignored]))
            h2_color::printf("dark gray", " (");
         else
            h2_color::printf("dark gray", " - ");

         int c = 0;
         if (s->stats[h2_case::passed]) comma_status(s->stats[h2_case::passed], "", "passed", c);
         if (s->stats[h2_case::failed]) comma_status(s->stats[h2_case::failed], "", "failed", c);
         if (s->stats[h2_case::todo]) comma_status(s->stats[h2_case::todo], "", "todo", c);
         if (s->stats[h2_case::filtered]) comma_status(s->stats[h2_case::filtered], "", "filtered", c);
         if (s->stats[h2_case::ignored]) comma_status(s->stats[h2_case::ignored], "", "ignored", c);

         if (1 < nonzero_count(s->stats[h2_case::passed], s->stats[h2_case::failed], s->stats[h2_case::todo], s->stats[h2_case::filtered], s->stats[h2_case::ignored]))
            h2_color::printf("dark gray", ")");
         if (0 < s->cases.count())
            h2_color::printf("", " case%s", 1 < s->cases.count() ? "s" : "");

         h2_color::printf("dark gray", " in suite");
         if (0 < s->checks) {
            h2_color::printf("dark gray", ",");
            h2_color::printf("", "%d check%s", s->checks, 1 < s->checks ? "s" : "");
         }
         if (0 < s->footprint) {
            h2_color::printf("dark gray", ",");
            h2_color::printf("", "%s", format_volume(s->footprint));
         }
         if (1 < suite_cost) {
            h2_color::printf("dark gray", ",");
            h2_color::printf("", "%s", format_duration(suite_cost));
         }
         h2_color::printf("", "\n");
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (O.listing) {
         h2_color::printf("", "   %s%d. ", c->status == h2_case::todo ? "TODO" : "CASE", suite_case_index);
         h2_color::printf("cyan", "%s", c->name);
         h2_color::printf("", " %s:%d\n", basename((char*)c->file), c->line);
      }
   }
   void print_suite_case(const char* s, const char* c)
   {
      h2_color::printf("", "%s", s);
      h2_color::printf("dark gray", " | ");
      h2_color::printf("", "%s", c);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (O.listing) return;
      switch (c->status) {
      case h2_case::initial: break;
      case h2_case::todo:
         print_percentage();
         print_suite_case(s->name, c->name);
         h2_color::printf("yellow", " Todo");
         h2_color::printf("", " at %s:%d\n", basename((char*)c->file), c->line);
         break;
      case h2_case::filtered: break;
      case h2_case::passed:
         if (O.verbose) {
            print_percentage();
            print_suite_case(s->name, c->name);
            h2_color::printf("green", " Passed ");
            h2_color::printf("", "%d checks", c->checks);
            if (0 < c->footprint) {
               h2_color::printf("dark gray", ",");
               h2_color::printf("", " %s", format_volume(c->footprint));
            }
            if (1 < case_cost) {
               h2_color::printf("dark gray", ",");
               h2_color::printf("", " %s", format_duration(case_cost));
            }
            h2_color::printf("", "\n");
         } else if (!O.debug)
            print_percentage(true, true);
         break;
      case h2_case::failed:
         print_percentage();
         print_suite_case(s->name, c->name);
         h2_color::printf("bold,red", " Failed ");
         h2_color::printf("", "at %s:%d\n", basename((char*)c->file), c->line);
         if (c->fails) c->fails->foreach([](h2_fail* fail, int subling_index, int child_index) { fail->print(subling_index, child_index); });
         h2_color::printf("", "\n");
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
   };
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats[h2_case::failed], s->name, s->stats[h2_case::todo] + s->stats[h2_case::filtered], s->cases.count(), 0, "");
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (!f) return;

      static constexpr const char* CSS[] = {"Initial", "Passed", "Failed", "TODO", "Filtered", "Ignored"};
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, CSS[c->status], case_cost / 1000.0);

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
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   I().reports.push_back(console_report.x);
   if (strlen(O.junit) && !O.listing) I().reports.push_back(junit_report.x);
   if (strlen(O.tap) && !O.listing) I().reports.push_back(tap_report.x);
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
// h2_string.cpp

static inline h2_string getlower(h2_string from) { return from.tolower(); }

h2_inline bool h2_string::equals(const h2_string& str, bool caseless) const
{
   if (!caseless) return *this == str;
   return getlower(c_str()) == getlower(str);
}

h2_inline bool h2_string::contains(const h2_string& substr, bool caseless) const
{
   if (!caseless) return find(substr) != h2_string::npos;
   return getlower(c_str()).find(getlower(substr)) != h2_string::npos;
}

h2_inline bool h2_string::startswith(const h2_string& prefix, bool caseless) const
{
   if (size() < prefix.size()) return false;
   if (!caseless) return find(prefix) == 0;
   return getlower(c_str()).find(getlower(prefix)) == 0;
}

h2_inline bool h2_string::endswith(const h2_string& suffix, bool caseless) const
{
   if (size() < suffix.size()) return false;
   if (!caseless) return rfind(suffix) == length() - suffix.length();
   return getlower(c_str()).rfind(getlower(suffix)) == length() - suffix.length();
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

h2_inline h2_string h2_string::unquote(const char c) const
{
   if (!enclosed(c)) return *this;
   return h2_string(size() - 2, c_str() + 1);
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

h2_inline h2_string& h2_string::tolower()
{
   for (auto& c : *this) c = ::tolower(c);
   return *this;
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

// h2_nm.cpp

struct h2_symbol : h2_libc {
   h2_list x;
   char name[128];
   unsigned long long addr;
   h2_symbol(char* _name, unsigned long long _addr) : addr(_addr) { strncpy(name, _name, sizeof(name)); }
};

h2_inline h2_nm::h2_nm()
{
   char nm[256], line[1024], addr[128], type[32], name[1024];
   sprintf(nm, "nm %s", O.path);
   h2_with f(::popen(nm, "r"), ::pclose);
   if (f.f)
      while (::fgets(line, sizeof(line) - 1, f.f))
         if (3 == sscanf(line, "%s%s%s", addr, type, name))  // if (strchr("tTwWsSvV", type[0]))
            symbols.push_back((new h2_symbol(name + !strcmp("macos", O.os), (unsigned long long)strtoull(addr, nullptr, 16)))->x);
}

h2_inline unsigned long long h2_nm::get(const char* name)
{
   if (!name || strlen(name) == 0) return 0;
   h2_list_for_each_entry (p, I().symbols, h2_symbol, x)
      if (!strcmp(p->name, name))
         return p->addr;
   return 0;
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
// h2_backtrace.cpp

static inline bool demangle(const char* mangled, char* demangled, size_t len)
{
   int status = 0;
#ifndef _WIN32
   abi::__cxa_demangle(mangled, demangled, &len, &status);
#endif
   return status == 0;
}

static inline bool addr2line(unsigned long long addr, char* output, size_t len)
{
   char t[256];
#if defined __APPLE__
   sprintf(t, "atos -o %s 0x%llx", O.path, addr);
#else
   sprintf(t, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#endif
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

h2_inline h2_backtrace::h2_backtrace(int shift_) : shift(shift_)
{
#ifndef _WIN32
   h2_memory::restores();
   count = ::backtrace(array, sizeof(array) / sizeof(array[0]));
   h2_memory::overrides();
#endif
}

h2_inline bool h2_backtrace::operator==(const h2_backtrace& bt)
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
   h2_memory::restores();
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
         address = h2_nm::get(mangled);
         if (address != ULLONG_MAX)
            if (addr2line(address + offset, addr2lined, sizeof(addr2lined)))
               if (strlen(addr2lined))
                  p = addr2lined;
      }
      stacks.push_back(p);

      if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_nm::in_main(address + offset))
         break;
   }
   free(backtraces);
   h2_memory::overrides();
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
// h2_check.cpp

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

h2_inline h2_defer_fail::h2_defer_fail(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_)
  : e_expression(e_expression_), a_expression(a_expression_), expression(expression_), file(file_), line(line_) {}

h2_inline h2_defer_fail::~h2_defer_fail()
{
   if (fail) {
      fail->file = file;
      fail->line = line;
      fail->check_type = check_type;
      if (!strcmp("OK1", check_type)) {
         fail->e_expression = e_expression;
         fail->a_expression = expression;
      } else if (!strcmp("OK2", check_type)) {
         const char* comma = find_outer_comma(expression);
         if (comma) {
            const char *p, *q;
            for (p = comma - 1; expression <= p && ::isspace(*p);) p--;
            fail->e_expression.assign(expression, (p + 1) - expression);

            for (q = comma + 1; ::isspace(*q);) q++;
            fail->a_expression.assign(q, (expression + strlen(expression)) - q);
         } else {
            fail->e_expression = e_expression;
            fail->a_expression = a_expression;
         }
      } else {
         fail->e_expression = e_expression;
         fail->a_expression = a_expression;
      }
      fail->user_explain = oss.str().c_str();
      h2_fail_g(fail, false);
   }
}
// h2_layout.cpp

static inline h2_lines line_break(const h2_line& line, unsigned width)
{
   h2_lines lines;
   h2_string current_style;
   h2_line wrap;
   unsigned length = 0;

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

static inline unsigned int_width(int maximum, char scale)
{
   char t1[64], t2[64];
   sprintf(t1, "%%%c", scale);
   sprintf(t2, t1, maximum);
   return strlen(t2);
}

static inline void lines_merge(h2_lines& lines, const h2_lines& left_lines, const h2_lines& right_lines, unsigned left_width, unsigned right_width, int step, char scale, int seq_width)
{
   char seq_fmt[32];
   sprintf(seq_fmt, "%%0%d%c│ ", seq_width, scale);
   const h2_line left_empty = {h2_string(left_width, ' ')}, right_empty = {h2_string(right_width, ' ')};
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
         line.concat_back(j < left_wrap_lines.size() ? left_wrap_lines[j] : left_empty, "reset");
         line.printf("dark gray", j < left_wrap_lines.size() - 1 ? "\\│ " : " │ ");
         line.concat_back(j < right_wrap_lines.size() ? right_wrap_lines[j] : right_empty, "reset");
         line.printf("dark gray", j < right_wrap_lines.size() - 1 ? "\\" : " ");
         lines.push_back(line);
      }
   }
}

h2_inline h2_lines h2_layout::split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, int step, char scale, unsigned width)
{
   unsigned seq_width = int_width(step * std::max(left_lines.size(), right_lines.size()), scale);
   unsigned valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

   unsigned left_width = std::max(left_lines.width(), 8u); /* at least title width */
   unsigned right_width = std::max(right_lines.width(), 8u);

   if (left_width < valid_width / 2)
      right_width = std::min(valid_width - left_width, right_width);
   else if (right_width < valid_width / 2)
      left_width = std::min(valid_width - right_width, left_width);
   else
      left_width = right_width = valid_width / 2;

   h2_lines lines;
   h2_line title_line = {"\033{reset}", "\033{dark gray}", (step ? h2_string(seq_width + 2, ' ') : ""), h2_string(left_title).center(left_width), "   ", h2_string(right_title).center(right_width), "\033{reset}"};
   lines.push_back(title_line);

   lines_merge(lines, left_lines, right_lines, left_width, right_width, step, scale, seq_width);
   return lines;
}

h2_inline h2_lines h2_layout::unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, unsigned width)
{
   h2_lines lines;

   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines up_lines = line_break(up_line, width - up_title_line.width());
   h2_lines down_lines = line_break(down_line, width - down_title_line.width());

   for (size_t i = 0; i < std::max(up_lines.size(), down_lines.size()); ++i) {
      if (i < up_lines.size()) {
         up_lines[i].concat_front(up_title_line);
         lines.push_back(up_lines[i]);
      }
      if (i < down_lines.size()) {
         down_lines[i].concat_front(down_title_line);
         lines.push_back(down_lines[i]);
      }
   }

   return lines;
}

static inline h2_lines prefix_break(const h2_line& line, const h2_line& title, unsigned width)
{
   h2_lines lines = line_break(line, width - title.width());

   for (size_t i = 0; i < lines.size(); ++i) {
      if (i == 0)
         lines[i].concat_front(title);
      else
         lines[i].indent(title.width());
   }
   return lines;
}

h2_inline h2_lines h2_layout::seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, unsigned width)
{
   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines lines = prefix_break(up_line, up_title_line, width);
   return lines += prefix_break(down_line, down_title_line, width);
}
// h2_option.cpp

static inline void usage()
{
   ::printf("  \033[33m╭─────────────────────────────────────────────────────────────────────────╮\033[0m\n");
   ::printf("  \033[33m│\033[0m                                                                         \033[33m│\033[0m\n");
   ::printf("  \033[33m│\033[0m                       Current version \033[32mh2unit \033[31m%-9g                  \033[33m│\033[0m\n", H2UNIT_VERSION);
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
  │ -s     │           │ Shuffle cases and execute in random order          │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -b[n]  │    [n]    │ Breaking test once n (default 1) failures occurred │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -o     │           │ Only execute last failed cases                     │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -r[n]  │    [n]    │ Repeat run n rounds (default 1) when no failure    │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -c     │           │ Output in black-white color style                  │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -f     │           │ Toggle fold simple json object or array            │\n\
  ├────────┼───────────┼────────────────────────────────────────────────────┤\n\
  │ -p     │           │ Print C/C++ source code json for copy/paste        │\n\
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
   const char** args;  // argv + 1
   int i = -1;
   const char* j = nullptr;

   getopt(int argc_, const char** args_) : argc(argc_), args(args_) {}

   const char* extract_string()
   {
      ++i;
      return i < argc ? args[i] : nullptr;
   }

   const char next_option()
   {
      do {
         for (; j && *++j;) return *j;
         for (; (j = extract_string()) && j[0] != '-';) {}
      } while (j);
      return '\0';
   }

   void extract_number(int& value)
   {
      if (j) { // j always not null
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

static inline unsigned h2_term_size()
{
#ifdef _WIN32
   return 80;
#else
   struct winsize w;
   if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) return 80;
   return w.ws_col < 16 || 256 < w.ws_col ? 80 : w.ws_col;
#endif
}

h2_inline h2_option::h2_option()
{
   term_size = h2_term_size();
#ifdef _WIN32
   memory_check = false;
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
      case 'c': colorfull = !colorfull; break;
      case 's': shuffle = !shuffle; break;
      case 'o': only = true; break;
      case 'S': seq = true; break;
      case 'f': fold = !fold; break;
      case 'p': paste = true; break;
      case 'm': memory_check = !memory_check; break;
      case 'l': listing = true; break;
      case 'b':
         breakable = 1;
         get.extract_number(breakable);
         break;
      case 'r':
         rounds = 1;
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
      case 'x':
         while ((t = get.extract_string())) excludes.push_back(t);
         break;
      case 'h':
      case '?':
         usage();
         exit(0);
      }
   }
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
   if (includes.size())
      if (!match3(includes, suitename) && !match3(includes, casename) && !match3(includes, filename))
         return true;
   if (excludes.size())
      if (match3(excludes, suitename) || match3(excludes, casename) || match3(excludes, filename))
         return true;
   return false;
}
// h2_patch.cpp

struct h2__patch {
   h2_singleton(h2__patch);
   h2_stubs stubs;

   static char* asctime(const struct tm* timeptr)
   {
      static char st[256];
      asctime_r(timeptr, st);
      return st;
   }
   static char* asctime_r(const struct tm* timeptr, char* buf)
   {
      char* ret;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::asctime_r); t;) ret = ::asctime_r(timeptr, buf);
      h2_memory::overrides();
      return ret;
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
      struct tm* ret;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::gmtime_r); t;) ret = ::gmtime_r(clock, result);
      h2_memory::overrides();
      return ret;
   }
   static struct tm* gmtime(const time_t* clock)
   {
      static struct tm st;
      gmtime_r(clock, &st);
      return &st;
   }
   static time_t mktime(struct tm* timeptr)
   {
      time_t ret;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::mktime); t;) ret = ::mktime(timeptr);
      h2_memory::overrides();
      return ret;
   }

   h2__patch()
   {
      stubs.add((void*)::gmtime, (void*)gmtime, "gmtime", __FILE__, __LINE__);
      stubs.add((void*)::gmtime_r, (void*)gmtime_r, "gmtime_r", __FILE__, __LINE__);
      stubs.add((void*)::ctime, (void*)ctime, "ctime", __FILE__, __LINE__);
      stubs.add((void*)::ctime_r, (void*)ctime_r, "ctime_r", __FILE__, __LINE__);
      stubs.add((void*)::asctime, (void*)asctime, "asctime", __FILE__, __LINE__);
      stubs.add((void*)::asctime_r, (void*)asctime_r, "asctime_r", __FILE__, __LINE__);
      stubs.add((void*)::localtime, (void*)localtime, "localtime", __FILE__, __LINE__);
      stubs.add((void*)::localtime_r, (void*)localtime_r, "localtime_r", __FILE__, __LINE__);
      stubs.add((void*)::mktime, (void*)mktime, "mktime", __FILE__, __LINE__);
   }
};

h2_inline void h2_patch::initialize()
{
   h2__patch::I();
}

h2_inline bool h2_patch::exempt(const h2_backtrace& bt)
{
   static struct {
      void* base;
      int size;
   } exempt_functions[] = {
     {(void*)sscanf, 300},
     {(void*)sprintf, 300},
     {(void*)vsnprintf, 300},
#ifdef __APPLE__
     {(void*)vsnprintf_l, 300},
#endif
     {(void*)h2_pattern::regex_match, 0x100}, // linux is 0xcb size, MAC is 0x100 (gap to next symbol)
   };

   for (auto& x : exempt_functions)
      if (bt.has(x.base, x.size))
         return true;

   return false;
}

// h2_case.cpp

h2_inline h2_case::h2_case(const char* name_, int status_, const char* file_, int line_)
  : name(name_), file(file_), line(line_), status(status_) {}

h2_inline void h2_case::clear()
{
   if (fails) delete fails;
   fails = nullptr;
   sock = nullptr;
   checks = 0;
}

h2_inline void h2_case::prev_setup()
{
   status = passed;
   h2_memory::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup()
{
   h2_stdio::capture_cancel();
   if (sock) delete sock;
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear(true);
   footprint = h2_memory::stack::footprint();
   // should memory check stats into check_count ?
   h2_fail::append_subling(fail, h2_memory::stack::pop());

   if (!fail) return;
   if (status != failed)
      h2_fail::append_subling(fails, fail);
   else
      delete fail;
   status = failed;
}

h2_inline void h2_case::do_fail(h2_fail* fail, bool defer)
{
   status = failed;
   h2_fail::append_subling(fails, fail);
   if (!defer) ::longjmp(jump, 1);
}

h2_inline h2_case::cleaner::cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
h2_inline h2_case::cleaner::~cleaner() { thus->prev_cleanup(); }
// h2_suite.cpp

h2_inline h2_suite::h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const char* file_, int line_)
  : name(name_), file(file_), line(line_), test_code(test_code_)
{
   h2_task::I().suites.push_back(x);
}

h2_inline void h2_suite::clear()
{
   checks = 0;
   memset(stats, 0, sizeof(stats));
}

h2_inline void h2_suite::setup()
{
   h2_memory::stack::push(file, line);
}

h2_inline void h2_suite::cleanup()
{
   stubs.clear();
   mocks.clear(false);
   footprint = h2_memory::stack::footprint();
   h2_memory::stack::pop();
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

h2_inline h2_suite::installer::installer(h2_suite* s, h2_case* c)
{
   static int seq = INT_MAX / 4;
   s->cases.push_back(c->x);
   s->seq = c->seq = ++seq;
}

h2_inline h2_suite::cleaner::cleaner(h2_suite* s) : thus(s) {}
h2_inline h2_suite::cleaner::~cleaner()
{
   if (thus->jumpable) ::longjmp(thus->jump, 1);
}
// h2_task.cpp

static inline void save_last_order(h2_list& suites)
{
   h2_with f(fopen(".last_order", "w"));
   if (!f.f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         fprintf(f.f, "%s\n%s\n%d\n", s->name, c->name, c->status);
}

static inline void __mark(h2_list& suites, char* suitename, char* casename, int status)
{
   static int seq = INT_MIN / 4;

   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, s->name))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name)) {
               s->seq = c->seq = ++seq;
               c->last_status = status;
            }
}

static inline int mark_last_order(h2_list& suites)
{
   int count = 0;
   char suitename[1024], casename[1024], status[32];
   h2_with f(fopen(".last_order", "r"));
   if (!f.f) return 0;
   while (fgets(suitename, sizeof(suitename), f.f) &&
          fgets(casename, sizeof(casename), f.f) &&
          fgets(status, sizeof(status), f.f)) {
      suitename[strlen(suitename) - 1] = '\0';  // remove \n in save_last_order
      casename[strlen(casename) - 1] = '\0';
      status[strlen(status) - 1] = '\0';
      __mark(suites, suitename, casename, atoi(status));
      count++;
   }
   return count;
}

static inline void drop_last_order()
{
   ::remove(".last_order");
}

static int h2_suite_cmp(h2_list* a, h2_list* b)
{
   return h2_list_entry(a, h2_suite, x)->seq - h2_list_entry(b, h2_suite, x)->seq;
}

static int h2_case_cmp(h2_list* a, h2_list* b)
{
   return h2_list_entry(a, h2_case, x)->seq - h2_list_entry(b, h2_case, x)->seq;
}

h2_inline void h2_task::shuffle()
{
   last = mark_last_order(suites);
   srand(h2_now());
   if (O.shuffle && last == 0)
      h2_list_for_each_entry (s, suites, h2_suite, x)
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            s->seq = c->seq = rand();

   suites.sort(h2_suite_cmp);
   h2_list_for_each_entry (s, suites, h2_suite, x)
      s->cases.sort(h2_case_cmp);
}

h2_inline void h2_task::shadow()
{
   if (stats[h2_case::failed] == 0)
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
      for (auto& teardown : global_suite_teardowns) teardown();
   }
}

h2_inline int h2_task::execute()
{
   h2_report::initialize();
   h2_memory::initialize();
   h2_patch::initialize();
   h2_stdio::initialize();
   h2_dns::initialize();

   for (auto& setup : global_setups) setup();
   enumerate();
   h2_report::I().on_task_start(this);
   for (rounds = 0; rounds < O.rounds && !stats[h2::h2_case::failed]; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if (0 < O.breakable && O.breakable <= stats[h2_case::failed]) break;
            current_case = c;
            if (O.filter(s->name, c->name, c->file, c->line)) c->status = h2_case::filtered;
            h2_report::I().on_case_start(s, c);
            if (O.only && h2_case::failed != c->last_status) {
               if (h2_case::initial == c->status) c->status = h2_case::ignored;
            }
            if (h2_case::initial == c->status && !O.listing) {
               for (auto& setup : global_case_setups) setup();
               s->execute(c);
               for (auto& teardown : global_case_teardowns) teardown();
            }
            h2_report::I().on_case_endup(s, c);
            stats[c->status] += 1;
            s->stats[c->status] += 1;
            c->clear();
         }
         s->cleanup();
         for (auto& teardown : global_suite_teardowns) teardown();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      shadow();
   }
   h2_report::I().on_task_endup(this);
   for (auto& teardown : global_teardowns) teardown();

   stubs.clear();
   mocks.clear(false);
   h2_memory::finalize();

   return stats[h2_case::failed];
}

}  // namespace h2
#endif
