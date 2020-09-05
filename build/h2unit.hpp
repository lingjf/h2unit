/* v5.6 2020-09-05 18:12:14 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#ifndef __H2UNIT_HPP__
#define __H2UNIT_HPP__

#ifndef __H2_UNIT_HPP__
#define __H2_UNIT_HPP__

#include <cstdio>      /* printf */
#include <cstdlib>     /* malloc */
#include <cstring>     /* strcpy, memcpy */
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
// h2_macro.hpp
/* clang-format off */

#define H2PP__CAT2(_1, _2) _1##_2
#define H2PP_CAT2(_1, _2) H2PP__CAT2(_1, _2)
#define H2PP__CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define H2PP_CAT5(_1, _2, _3, _4, _5) H2PP__CAT5(_1, _2, _3, _4, _5)

#define H2PP_CAT(_1, _2) H2PP__CAT2(_1, _2)

#define H2PP_COMMA(...) ,
#define H2PP_EAT(...)
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
#define _H2PP_EQ__10__10 H2PP_PROBE()
#define _H2PP_EQ__11__11 H2PP_PROBE()
#define _H2PP_EQ__12__12 H2PP_PROBE()
#define _H2PP_EQ__13__13 H2PP_PROBE()
#define _H2PP_EQ__14__14 H2PP_PROBE()
#define _H2PP_EQ__15__15 H2PP_PROBE()

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

#define H2PP_IBP(...) H2PP_IS_PROBE(_H2PP_IS_BEGIN_PARENTHESIS_PROBE __VA_ARGS__)
#define _H2PP_IS_BEGIN_PARENTHESIS_PROBE(...) H2PP_PROBE()

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define H2PP_RPS(...) H2PP_CAT2(_H2PP_REMOVE_PARENTHESES_IF_, H2PP_IBP(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_1(...) H2PP_REMOVE_PARENTHESES(__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_0(...) __VA_ARGS__

#define __H2PP_100TH(_100, _99, _98, _97, _96, _95, _94, _93, _92, _91, _90, _89, _88, _87, _86, _85, _84, _83, _82, _81, _80, _79, _78, _77, _76, _75, _74, _73, _72, _71, _70, _69, _68, _67, _66, _65, _64, _63, _62, _61, _60, _59, _58, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, _41, _40, _39, _38, _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _x, ...) _x
#define __H2PP_RESCAN(_1) _1
#define _H2PP_100TH(_Args) __H2PP_RESCAN(__H2PP_100TH _Args)

#define H2PP_IS_EMPTY(...)                                  \
  _H2PP_IS_EMPTY(_H2PP_HAS_COMMA(__VA_ARGS__),              \
                 _H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__),   \
                 _H2PP_HAS_COMMA(__VA_ARGS__()),            \
                 _H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__()))
#define _H2PP_IS_EMPTY(_1, _2, _3, _4) _H2PP_HAS_COMMA(H2PP__CAT5(_H2PP_IS_EMPTY_CASE_, _1, _2, _3, _4))
#define _H2PP_IS_EMPTY_CASE_0001 ,
#define _H2PP_HAS_COMMA(...) _H2PP_100TH((__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))

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

#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_CAT2(_Macro, H2PP_NARG(__VA_ARGS__))(__VA_ARGS__)

#define H2Q(...) H2PP_CAT5(__VA_ARGS__, _C, __COUNTER__, L, __LINE__)  // generate unique identifier [with prefix]
// h2_template.hpp

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
   using type = typename h2_nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;
};

template <std::size_t Index, typename... Args>
using h2_nth_decay = typename h2_decay<typename h2_nth_type<Index, Args...>::type>::type;

template <typename T, typename = void>
struct h2_sizeof_pointee : std::integral_constant<std::size_t, sizeof(typename std::remove_pointer<T>::type)> {
};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<std::is_void<typename std::remove_pointer<T>::type>::value>::type> : std::integral_constant<std::size_t, 1> {
};

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

   h2_list() : next(this), prev(this) {}

   h2_list& add_head(h2_list& entry);
   h2_list& add_tail(h2_list& entry);
   h2_list& add_before(h2_list& entry);
   h2_list& add_after(h2_list& entry);

   h2_list& push_back(h2_list& entry) { return add_tail(entry); }
   h2_list& push(h2_list& entry) { return add_head(entry); }
   h2_list& pop() { return next->out(); }
   h2_list& pop_back() { return prev->out(); }

   bool is_first(h2_list* entry) const { return next == entry; }
   bool is_last(h2_list* entry) const { return prev == entry; }

   h2_list& out();

   bool empty() const { return next == this; }
   int count() const;
   void sort(int (*cmp)(h2_list*, h2_list*));
};
// h2_misc.hpp

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

static inline const char* comma_if(bool a, const char* t = ", ", const char* f = "") { return a ? t : f; };

#define h2_singleton(_Class) \
   static _Class& I()        \
   {                         \
      static _Class i;       \
      return i;              \
   }
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
// h2_allocate.hpp

template <typename T>
class h2_allocator {
 public:
   typedef size_t size_type;
   typedef long long difference_type;
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

   h2_string& sprintf(const char* format, ...);
   h2_string& replace_all(const char* from, const char* to);

   bool equals(const h2_string& str, bool caseless = false) const;
   bool contains(const h2_string& substr, bool caseless = false) const;
   bool startswith(const h2_string& prefix, bool caseless = false) const;
   bool endswith(const h2_string& suffix, bool caseless = false) const;

   bool isspace() const;
   bool enclosed(const char c = '\"') const;

   h2_string escape() const;
   h2_string unescape() const;
   h2_string unquote(const char c = '\"') const;
   h2_string tolower() const;
   h2_string center(int width) const;
};

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }
// h2_line.hpp

struct h2_line : h2_vector<h2_string> {
   h2_line() {}
   h2_line(const char* a) : h2_vector<h2_string>({a}) {}
   h2_line(const h2_string& a) : h2_vector<h2_string>({a}) {}
   h2_line(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   unsigned width(bool ignore_indent = false) const;
   h2_line& indent(int n, const char c = ' ');
   h2_line& padding(int n, const char c = ' ');

   h2_line& printf(const char* style, const char* format, ...);
   h2_line& operator+=(const h2_line& line);
   h2_line& brush(const char* style);

   bool enclosed(const char c) const;
   h2_line gray_quote() const;
   h2_line acronym(int width = 16, int tail = 0) const;
   h2_string string() const;

   void samesizify(h2_line& b);
};

inline h2_line operator+(const h2_line& a, const h2_line& b)  // implicit conversion const char* / h2_string
{
   h2_line line;
   line.insert(line.end(), a.begin(), a.end());
   line.insert(line.end(), b.begin(), b.end());
   return line;
}

static inline const h2_line color(const h2_line& s, const char* style) { return ("\033{" + h2_string(style) + "}") + s + "\033{reset}"; }
static inline const h2_line delta(const h2_line& s, const char* style) { return ("\033{+" + h2_string(style) + "}") + s + ("\033{-" + h2_string(style) + "}"); }
static inline const h2_line gray(const h2_line& s) { return delta(s, "dark gray"); }

struct h2_lines : h2_vector<h2_line> {
   h2_lines() {}
   h2_lines(std::initializer_list<h2_line> il) : h2_vector<h2_line>(il) {}

   h2_lines& operator+=(const h2_lines& lines);

   unsigned width() const;
   bool foldable(unsigned width = 20);
   h2_line folds();

   h2_string string() const;

   void sequence(unsigned indent = 0, int start = 0);
   void samesizify(h2_lines& b);
};
// h2_stringify.hpp

template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_line print(T a, bool represent = false) { return "?"; }
};

#define H2_STRINGIFY_IMPL_TOSTRING(member)                                                                                    \
   template <typename T>                                                                                                      \
   struct h2_stringify_impl<T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::member)>::value>::type> { \
      static h2_line print(const T& a, bool represent = false) { return const_cast<T&>(a).member(); }                         \
   }

/* tostring() may not be mark const, remove cast const in T a */

H2_STRINGIFY_IMPL_TOSTRING(tostring);
H2_STRINGIFY_IMPL_TOSTRING(toString);
H2_STRINGIFY_IMPL_TOSTRING(Tostring);
H2_STRINGIFY_IMPL_TOSTRING(ToString);
H2_STRINGIFY_IMPL_TOSTRING(to_string);

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false) { return ostream_print(a, represent); }

   template <typename U>
   static h2_line ostream_print(const U& a, bool represent)
   {
      h2_ostringstream os;
      os << std::boolalpha;
      os << const_cast<U&>(a);
      if (represent) {
         const char* quote = nullptr;
         if (std::is_same<char, U>::value) quote = "'";
         if (std::is_convertible<U, h2_string>::value) quote = "\"";
         if (quote) return gray(quote) + os.str().c_str() + gray(quote);
      }
      return {os.str().c_str()};
   }

   static h2_line ostream_print(unsigned char a, bool represent)
   {  // https://en.cppreference.com/w/cpp/string/byte/isprint
      return ostream_print<unsigned int>(static_cast<unsigned int>(a), represent);
   }
};

template <typename K, typename V>
struct h2_stringify_impl<std::pair<K, V>> {
   static h2_line print(const std::pair<K, V>& a, bool represent = false)
   {
      return gray("(") + h2_stringify_impl<K>::print(a.first, represent) + gray(", ") + h2_stringify_impl<V>::print(a.second, represent) + gray(")");
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value && !std::is_convertible<T, h2_string>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      h2_line line;
      line += gray("[");
      for (auto it = a.begin(); it != a.end(); it++) {
         if (it != a.begin()) line += gray(", ");
         line += h2_stringify_impl<typename T::value_type>::print(*it, represent);
      }
      line += gray("]");
      return line;
   }
};

template <typename... Args>
struct h2_stringify_impl<std::tuple<Args...>> {
   static h2_line print(const std::tuple<Args...>& a, bool represent = false)
   {
      return gray("(") + tuple_print(a, represent, std::integral_constant<std::size_t, sizeof...(Args)>()) + gray(")");
   }

   static h2_line tuple_print(const std::tuple<Args...>& a, bool represent, std::integral_constant<std::size_t, 0>) { return {}; }
   template <std::size_t I>
   static h2_line tuple_print(const std::tuple<Args...>& a, bool represent, std::integral_constant<std::size_t, I>)
   {
      return tuple_print(a, represent, std::integral_constant<std::size_t, I - 1>()) + (1 < I ? gray(", ") : h2_line()) + h2_stringify_impl<typename std::decay<decltype(std::get<I - 1>(a))>::type>::print(std::get<I - 1>(a), represent);
   }
};

template <>
struct h2_stringify_impl<std::nullptr_t> {
   static h2_line print(std::nullptr_t a, bool represent = false) { return "nullptr"; }
};

template <typename T>
inline h2_line h2_stringify(const T& a, bool represent = false)
{
   return h2_stringify_impl<T>::print(a, represent);
}

template <typename T>
inline h2_line h2_stringify(T a, size_t n, bool represent)
{
   if (n == 0) return h2_stringify(a, represent);

   h2_line line;
   line += gray("[");
   for (size_t i = 0; i < n; ++i) {
      if (i) line += gray(", ");
      line += h2_stringify(a[i], represent);
   }
   line += gray("]");
   return line;
}

template <typename T>
inline h2_line h2_representify(const T& a) { return h2_stringify(a, true); }

template <typename T>
inline h2_line h2_representify(T a, size_t n) { return h2_stringify(a, n, true); }
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

   unsigned terminal_width;

   char args[256];
   const char* path;
   const char* debug = nullptr;
   bool verbose = false;
   bool colorful = true;
   bool execute_progress = true;
   bool seq = false;
   bool fold_json = true;
   bool copy_paste_json = false;
   bool only_execute_fails = false;
   bool shuffle_order = false;
   bool memory_check = true;
   bool list_cases = false;
   int break_after_fails = 0;
   int rounds = 1;
   char junit[256]{'\0'};
   char tap[256]{'\0'};
   std::vector<const char*> includes, excludes;

   h2_option();
   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int lino) const;
};

static const h2_option& O = h2_option::I();  // for pretty
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

   const char* check_type = "Inner";  // Inner(Mock, AllOf, &&, ||)
   h2_string e_expression, a_expression;
   h2_line explain;
   h2_string user_explain;

   //           expression     expection      represent       value
   // number     Ge(var)        Ge(5)          5               5
   // string     We(var)        We("abc")      "abc"           abc
   // char       We(var)        We('a')        'a'             a

   int seqno = -1;
   const char* file;
   int lino;

   h2_fail(const h2_line& explain_, const char* file_, int lino_) : explain(explain_), file(file_), lino(lino_) {}
   virtual ~h2_fail();

   h2_line locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);

   static h2_fail* new_normal(const h2_line& explain, const char* file = nullptr, int lino = 0);
   static h2_fail* new_unexpect(const h2_line& expection = {}, const h2_line& represent = {}, const h2_line& explain = {}, const char* file = nullptr, int lino = 0);
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection, const h2_line& explain = {}, const char* file = nullptr, int lino = 0);
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, const h2_line& explain = {}, const char* file = nullptr, int lino = 0);
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, bool caseless, const h2_line& explain = {}, const char* file = nullptr, int lino = 0);
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& represent, const h2_line& explain = {}, const char* file = nullptr, int lino = 0);
   static h2_fail* new_memory_leak(const void* ptr, int size, const h2_backtrace& bt_allocate, const char* where, const char* file, int lino);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const int size, const void* addr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file = nullptr, int lino = 0);
   static h2_fail* new_use_after_free(const void* ptr, const void* addr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
};

static inline void h2_fail_g(h2_fail*, bool);
// h2_generator.hpp

/* clang-format off */

#define _H2_Macro_II(_Func, _Args, ...) _Func(_Args, __VA_ARGS__)
#define _H2_Macro_I(...) _H2_Macro_II(__VA_ARGS__)


#define _H2_ForEachv(_Func, _Args, ...) H2PP_CAT(_H2_ForEachv_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, __VA_ARGS__)
#define _H2_ForEachv_I() _H2_ForEachv
#define _H2_ForEachv_0(_Func, _Args, ...) _H2_Macro_I(_Func, _Args, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachv_I)()(_Func, _Args, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachv_1(...)
#define H2ForEach(_Func, _Args, ...) H2PP_EVAL(_H2_ForEachv(_Func, _Args, __VA_ARGS__))

#define _H2_ForEachy(_Func, _Args, x, ...) H2PP_CAT(_H2_ForEachy_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, x, __VA_ARGS__)
#define _H2_ForEachy_I() _H2_ForEachy
#define _H2_ForEachy_0(_Func, _Args, x, ...) _H2_Macro_I(_Func, _Args, x, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachy_I)()(_Func, _Args, x, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachy_1(...)
#define _H2_ForEachx(_Func, _Args, _Tupley, ...) H2PP_CAT(_H2_ForEachx_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, _Tupley, __VA_ARGS__)
#define _H2_ForEachx_I() _H2_ForEachx
#define _H2_ForEachx_0(_Func, _Args, _Tupley, ...) _H2_ForEachy(_Func, _Args, H2PP_HEAD(__VA_ARGS__), H2PP_REMOVE_PARENTHESES(_Tupley)) H2PP_DEFER(_H2_ForEachx_I)()(_Func, _Args, _Tupley, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachx_1(...)
#define _H2_Fullmesh_Impl(_Func, _Args, _Tuplex, _Tupley) H2PP_EVAL(_H2_ForEachx(_Func, _Args, _Tupley, H2PP_REMOVE_PARENTHESES(_Tuplex)))
#define __H2_Fullmesh_Prev0(_Func, _Args, ...) _H2_Fullmesh_Impl(_Func, _Args, (__VA_ARGS__), (__VA_ARGS__))
#define _H2_Fullmesh_Prev0(...) __H2_Fullmesh_Prev0(__VA_ARGS__)
#define _H2_Fullmesh_Prev1(...) _H2_Fullmesh_Impl(__VA_ARGS__)
#define _H2_Fullmesh_Prev(_Func, _Args, t, ...) H2PP_CAT(_H2_Fullmesh_Prev, H2PP_AND(H2PP_IBP(t), H2PP_EQ(1, H2PP_NARG(__VA_ARGS__)))) (_Func, _Args, t, __VA_ARGS__)
#define H2Fullmesh(_Func, _Args, ...) _H2_Fullmesh_Prev(_Func, _Args, __VA_ARGS__)


#define ForForEach_Adapter(_Args, x) H2PP_REMOVE_PARENTHESES(_Args)(x)
#define ForForEach(_Macro_x, ...) H2ForEach(ForForEach_Adapter, (_Macro_x), __VA_ARGS__)

#define ForFullmesh_Adapter(_Args, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)
#define ForFullmesh(_Macro_x_y, ...) H2Fullmesh(ForFullmesh_Adapter, (_Macro_x_y), __VA_ARGS__)


#define H2List_An(...) H2PP_CAT(_H2List_An_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2List_An_0(_1, ...) _1
#define _H2List_An_1(...) 0

#define H2ForEach_An(...) H2PP_CAT(_H2ForEach_An_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2ForEach_An_1(...) 0
#define _H2ForEach_An_0(_1, ...) H2PP_CAT(_H2ForEach_An_0_, H2PP_IBP(_1))(_1)
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
   static h2_string select(const h2_string& json_string, const h2_string& selector, bool caseless);
   // < 0 illformed json; = 0 matched; > 0 unmatched
   static int match(const h2_string& expect, const h2_string& actual, bool caseless);
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
      static void push(const char* file, int lino);
      static h2_fail* pop();
      static long long footprint();

      struct block : h2_once {
         block(const char* attributes, const char* file, int lino);
         ~block();
      };
   };
};

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(Attributes, __FILE__, __LINE__); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2Q(t_block))
// h2_matches.hpp

struct h2_matches {
   virtual h2_line expection(bool caseless, bool dont) const = 0;
};

static inline h2_string DS(bool match)
{
   return match ? "should not match" : "";
}

static inline h2_line CD(const h2_line& s, bool caseless, bool dont, const char* dsym = "!")
{
   h2_line line;
   if (dont) line.push_back(dsym);
   if (caseless) line.push_back("~");
   line += s;
   return line;
}

template <typename T>
inline auto h2_matches_expection(const T& e, bool caseless, bool dont) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_line>::type
{
   return e.expection(caseless, dont);
}
template <typename T>
inline auto h2_matches_expection(const T& e, bool caseless, bool dont) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_line>::type
{
   return CD(h2_representify(e), caseless, dont);
}

#define H2_MATCHES_T2V2E(t_matchers)                                                                                                                                              \
                                                                                                                                                                                  \
   template <typename T>                                                                                                                                                          \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                                                                \
   template <typename T, std::size_t I>                                                                                                                                           \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                                                   \
   {                                                                                                                                                                              \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                                                              \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                                                      \
   }                                                                                                                                                                              \
   template <typename T>                                                                                                                                                          \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }                                   \
                                                                                                                                                                                  \
   h2_line t2e(bool caseless, bool dont, std::integral_constant<std::size_t, 0>) const { return {}; }                                                                             \
   template <std::size_t I>                                                                                                                                                       \
   h2_line t2e(bool caseless, bool dont, std::integral_constant<std::size_t, I>) const                                                                                            \
   {                                                                                                                                                                              \
      return t2e(caseless, dont, std::integral_constant<size_t, I - 1>()) + (1 < I ? gray(", ") : h2_line()) + h2_matches_expection(std::get<I - 1>(t_matchers), caseless, dont); \
   }                                                                                                                                                                              \
   h2_line t2e(bool caseless, bool dont) const { return t2e(caseless, dont, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
// h2_matcher.hpp

template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(T a, int n, bool caseless, bool dont) const = 0;
   virtual h2_line expection(bool caseless, bool dont) const override { return ""; }
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher : h2_matches {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher();         // Any matcher
   h2_matcher(T value);  // Converting constructor
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl_, const int placeholder) : impl(impl_) {}
   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   virtual ~h2_matcher() {}
   h2_fail* matches(const T& a, int n = 0, bool caseless = false, bool dont = false) const { return impl->matches(a, n, caseless, dont); }
   virtual h2_line expection(bool caseless = false, bool dont = false) const { return impl->expection(caseless, dont); };
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
      h2_line expection(bool caseless, bool dont) const override { return m.expection(caseless, dont); }
   };

   virtual h2_line expection(bool caseless = false, bool dont = false) const override
   {
      return h2_matches_expection(m, caseless, dont);
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
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_line expection(bool, bool dont) const override
   {
      return CD(h2_representify(e), false, dont, "≠");
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

      return h2_fail::new_strcmp(e, a, caseless, expection(caseless, dont));
   }
   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD(h2_representify(e), caseless, dont, "≠");
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
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_line expection(bool, bool dont) const override
   {
      return CD(h2_representify(e), false, dont, "≠");
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_equation<E>> Eq(const T& expect, const long double epsilon = 0)
{
   return h2_polymorphic_matcher<h2_equation<E>>(h2_equation<E>(expect, epsilon));
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
      const h2_matcher<U> from;
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}
      h2_fail* matches(T a, int n, bool caseless, bool dont) const override { return from.matches(static_cast<U>(a), n, caseless, dont); }
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
   virtual h2_line expection(bool, bool) const override { return "Any"; }
};

struct h2_matches_null : h2_matches {
   const bool reverse;
   explicit h2_matches_null(bool _reverse) : reverse(_reverse) {}
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool dont) const
   {
      bool _dont = reverse ? !dont : dont;
      if ((nullptr == (const void*)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect(expection(false, dont), h2_stringify((const void*)a));
   }
   virtual h2_line expection(bool, bool dont) const override
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
      return h2_fail::new_unexpect(expection(false, dont), a ? "true" : "false");
   }
   virtual h2_line expection(bool, bool dont) const override
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
   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return h2_matches_expection(m, caseless, dont);
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
   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return h2_matches_expection(m, caseless, !dont);
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
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m1).matches(a, n, caseless, false));
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m2).matches(a, n, caseless, false));
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      h2_fail::append_child(fail, fails);

      return fail;
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD((dont ? gray("(") : h2_line()) + h2_matches_expection(m1, caseless, false) + " and " + h2_matches_expection(m2, caseless, false) + (dont ? gray(")") : h2_line()), false, dont);
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
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, f1);
      h2_fail::append_subling(fails, f2);

      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD((dont ? gray("(") : h2_line()) + h2_matches_expection(m1, caseless, false) + " or " + h2_matches_expection(m2, caseless, false) + (dont ? gray(")") : h2_line()), false, dont);
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
         h2_fail* fail = v_matchers[i].matches(a, n, caseless, false);  // dont not transfer down
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }

      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), "should not match all");
      } else {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD("AllOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
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
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }

      if ((0 < c) == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), "should not match any one");
      } else {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), "not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD("AnyOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
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

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, caseless, false);
         if (fail)
            delete fail;
         else {
            fail = h2_fail::new_normal("should not match " + v_matchers[i].expection(caseless, false).brush("green"));
            fail->seqno = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD("NoneOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
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
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_line expection(bool, bool dont) const override
   {
      return CD("≥" + h2_representify(e), false, dont);
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
      return h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
   }
   virtual h2_line expection(bool, bool dont) const override
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
      return h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
   }
   virtual h2_line expection(bool, bool dont) const override
   {
      return CD("≤" + h2_stringify(e), false, dont);
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
      return h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
   }
   virtual h2_line expection(bool, bool dont) const override
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
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_matches_wildcard : h2_matches {
   const h2_string e;
   explicit h2_matches_wildcard(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_matches_strcmp : h2_matches {
   const h2_string e;
   explicit h2_matches_strcmp(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_matches_substr : h2_matches {
   const h2_string substring;
   explicit h2_matches_substr(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_matches_startswith : h2_matches {
   const h2_string prefix_string;
   explicit h2_matches_startswith(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_matches_endswith : h2_matches {
   const h2_string suffix_string;
   explicit h2_matches_endswith(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_matches_json : h2_matches {
   const h2_string e;
   const h2_string selector;
   explicit h2_matches_json(const h2_string& _e, const h2_string& _selector) : e(_e), selector(_selector) {}
   h2_fail* matches(const h2_string& a, int, bool caseless, bool dont) const;
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_caseless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(const h2_matcher<h2_string>& matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const { return m.matches(a, n, true, dont); }
   virtual h2_line expection(bool caseless, bool dont) const override { return m.expection(true, dont); }
};

inline h2_polymorphic_matcher<h2_matches_regex> Re(const h2_string& regex_pattern) { return h2_polymorphic_matcher<h2_matches_regex>(h2_matches_regex(regex_pattern)); }
inline h2_polymorphic_matcher<h2_matches_wildcard> We(const h2_string& wildcard_pattern) { return h2_polymorphic_matcher<h2_matches_wildcard>(h2_matches_wildcard(wildcard_pattern)); }
inline h2_polymorphic_matcher<h2_matches_strcmp> Se(const h2_string& expect) { return h2_polymorphic_matcher<h2_matches_strcmp>(h2_matches_strcmp(expect)); }
inline h2_polymorphic_matcher<h2_matches_substr> Substr(const h2_string& substring) { return h2_polymorphic_matcher<h2_matches_substr>(h2_matches_substr(substring)); }
inline h2_polymorphic_matcher<h2_matches_startswith> StartsWith(const h2_string& prefix_string) { return h2_polymorphic_matcher<h2_matches_startswith>(h2_matches_startswith(prefix_string)); }
inline h2_polymorphic_matcher<h2_matches_endswith> EndsWith(const h2_string& suffix_string) { return h2_polymorphic_matcher<h2_matches_endswith>(h2_matches_endswith(suffix_string)); }
inline h2_polymorphic_matcher<h2_matches_json> Je(const h2_string& expect, const h2_string& selector = "") { return h2_polymorphic_matcher<h2_matches_json>(h2_matches_json(expect, selector)); }

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
   virtual h2_line expection(bool caseless, bool dont) const override;
};

struct h2_matches_bitcmp : h2_matches {
   const void* e;
   const bool isstring;
   const int nbits;
   explicit h2_matches_bitcmp(const void* _e, const bool _isstring, const int _nbits) : e(_e), isstring(_isstring), nbits(_nbits) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const;
   virtual h2_line expection(bool caseless, bool dont) const override;
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
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
      }
      return fail;
   }
   virtual h2_line expection(bool caseless, bool dont) const override
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
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      }
      return fail;
   }

   template <typename A>
   auto matches(const A& a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD(gray("(") + h2_matches_expection(k, caseless, dont) + gray(", ") + h2_matches_expection(v, caseless, dont) + gray(")"), false, dont);
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
            fail = h2_fail::new_unexpect(v_matchers[i].expection(caseless, false), "", "but out of range");
         }
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(!fails));
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
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD("ListOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
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
            h2_fail* fail = h2_fail::new_unexpect(v_matchers[i].expection(caseless, false), "", "but haven't");
            if (fail) fail->seqno = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(!fails));
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
            h2_fail* fail = h2_fail::new_unexpect(v_matchers[i].expection(caseless, false), "", "but haven't");
            if (fail) fail->seqno = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a, n), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD("Has" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
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
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(result));
   }

   virtual h2_line expection(bool caseless, bool dont) const override
   {
      return CD("In" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
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

      return __matches(count, h2_representify(a), dont);
   }

   template <typename A>
   auto matches(A a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ generic array or not array , no meaningful just for continues */
      int count = 1;
      return __matches(count, h2_representify(a), dont);
   }

   h2_fail* __matches(int count, h2_line&& represent, bool dont) const
   {
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<int>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(count, 0, false, false);
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(false, dont), represent + "/" + h2_stringify(count), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(bool, bool dont) const override
   {
      return CD("CountOf" + gray("(") + t2e(false, dont) + gray(")"), false, false);
   }

   H2_MATCHES_T2V2E(t_matchers)
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

#define __Matches_Common(message)                                                                        \
   template <typename A>                                                                                 \
   bool __matches(const A& a) const;                                                                     \
   template <typename A>                                                                                 \
   h2::h2_fail* matches(const A& a, int, bool caseless, bool dont) const                                 \
   {                                                                                                     \
      h2::h2_fail* fail = h2::h2_fail::new_unexpect(h2::CD("", caseless, dont), h2::h2_representify(a)); \
      if (__matches(a) == !dont) return nullptr;                                                         \
      if (dont) {                                                                                        \
      } else {                                                                                           \
         h2::h2_ostringstream osm;                                                                       \
         osm << std::boolalpha << H2PP_REMOVE_PARENTHESES(message);                                      \
         fail->user_explain = osm.str().c_str();                                                         \
      }                                                                                                  \
      return fail;                                                                                       \
   }                                                                                                     \
   virtual h2::h2_line expection(bool, bool) const override { return ""; }

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
#define _H2MATCHER2(name, t) H2PP_CAT(_H2MATCHER2, H2PP_IBP(t)) (name, t)

#define _H2MATCHER30(name, e1, e2) H2MATCHER2(name, e1, e2, (""))
#define _H2MATCHER31(name, e1, message) H2MATCHER1(name, e1, message)
#define _H2MATCHER3(name, e1, t) H2PP_CAT(_H2MATCHER3, H2PP_IBP(t)) (name, e1, t)

#define _H2MATCHER40(name, e1, e2, e3) H2MATCHER3(name, e1, e2, e3, (""))
#define _H2MATCHER41(name, e1, e2, message) H2MATCHER2(name, e1, e2, message)
#define _H2MATCHER4(name, e1, e2, t) H2PP_CAT(_H2MATCHER4, H2PP_IBP(t)) (name, e1, e2, t)

#define _H2MATCHER50(name, e1, e2, e3, e4) H2MATCHER4(name, e1, e2, e3, e4, (""))
#define _H2MATCHER51(name, e1, e2, e3, message) H2MATCHER3(name, e1, e2, e3, message)
#define _H2MATCHER5(name, e1, e2, e3, t) H2PP_CAT(_H2MATCHER5, H2PP_IBP(t)) (name, e1, e2, e3, t)

#define _H2MATCHER60(name, e1, e2, e3, e4, e5) H2MATCHER5(name, e1, e2, e3, e4, e5, (""))
#define _H2MATCHER61(name, e1, e2, e3, e4, message) H2MATCHER4(name, e1, e2, e3, e4, message)
#define _H2MATCHER6(name, e1, e2, e3, e4, t) H2PP_CAT(_H2MATCHER6, H2PP_IBP(t)) (name, e1, e2, e3, e4, t)

#define _H2MATCHER70(name, e1, e2, e3, e4, e5, e6)
#define _H2MATCHER71(name, e1, e2, e3, e4, e5, message) H2MATCHER5(name, e1, e2, e3, e4, e5, message)
#define _H2MATCHER7(name, e1, e2, e3, e4, e5, t) H2PP_CAT(_H2MATCHER7, H2PP_IBP(t)) (name, e1, e2, e3, e4, e5, t)

/* clang-format on */

#define H2MATCHER(...) H2PP_VARIADIC_CALL(_H2MATCHER, __VA_ARGS__)
// h2_matcher.cpp

template <typename T>
inline h2_matcher<T>::h2_matcher()
{
   *this = Any;
}

template <typename T>
inline h2_matcher<T>::h2_matcher(T value)
{
   *this = Eq(value);
}
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
   bool add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int lino);
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

#define __H2STUB40(Class, Method, Return, Args, Q)                                                                                                                        \
   struct {                                                                                                                                                               \
      void operator=(Return (*substitute_fp)(H2PP_RPS(Class) * that))                                                                                                     \
      {                                                                                                                                                                   \
         h2::h2_stub_g(h2::h2_mfp<H2PP_RPS(Class), Return Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                   \
   } Q;                                                                                                                                                                   \
   Q = [](H2PP_RPS(Class) * that) -> Return

#define __H2STUB41(Class, Method, Return, Args, Q)                                                                                                                        \
   struct {                                                                                                                                                               \
      void operator=(Return (*substitute_fp)(H2PP_RPS(Class) * that, H2PP_REMOVE_PARENTHESES(Args)))                                                                      \
      {                                                                                                                                                                   \
         h2::h2_stub_g(h2::h2_mfp<H2PP_RPS(Class), Return Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                   \
   } Q;                                                                                                                                                                   \
   Q = [](H2PP_RPS(Class) * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

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

   bool insufficient(/*不够*/) const { return call < least; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_excessive(/*过多*/) const { return most < call; }

   h2_fail* check(const char* func, const char* file, int lino);
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
   if (fail) fail->seqno = I - 1;
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
   const char* ret;
   const char* func;
   h2_vector<const char*> argv;
   const char* file;
   int lino;

   h2_line argvs(int seq = -1);
   h2_line signature();

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

/* clang-format off */
#define __H2ARGV0() {}
#define __H2ARGV1(_1) {#_1}
#define __H2ARGV2(_1, _2) {#_1, #_2}
#define __H2ARGV3(_1, _2, _3) {#_1, #_2, #_3}
#define __H2ARGV4(_1, _2, _3, _4) {#_1, #_2, #_3, #_4}
#define __H2ARGV5(_1, _2, _3, _4, _5) {#_1, #_2, #_3, #_4, #_5}
#define __H2ARGV6(_1, _2, _3, _4, _5, _6) {#_1, #_2, #_3, #_4, #_5, #_6}
#define __H2ARGV7(_1, _2, _3, _4, _5, _6, _7) {#_1, #_2, #_3, #_4, #_5, #_6, #_7}
#define __H2ARGV8(_1, _2, _3, _4, _5, _6, _7, _8) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8}
#define __H2ARGV9(_1, _2, _3, _4, _5, _6, _7, _8, _9) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9}
#define __H2ARGV10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10}
#define __H2ARGV11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11}
#define __H2ARGV12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12}
#define __H2ARGV13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12, #_13}
#define __H2ARGV14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12, #_13, #_14}
#define __H2ARGV15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12, #_13, #_14, #_15}

#define __H2ARGV(...) H2PP_CAT(__H2ARGV, H2PP_NARG(__VA_ARGS__))(__VA_ARGS__)  // Duplicated H2PP_VARIADIC_CALL avoid recursion
#define H2ARGV(...) __H2ARGV(H2PP_REMOVE_PARENTHESES(__VA_ARGS__))

#define __H2MOCK4(OriginFunction, Return, Args, Expression) \
   h2::h2_mocker<__COUNTER__, std::false_type, Return Args>::I(h2::h2_fp(OriginFunction), #Return, #OriginFunction, H2ARGV(Args), __FILE__, __LINE__)

#define __H2MOCK5(Class, Method, Return, Args, Expression) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), Return Args>::I(h2::h2_mfp<H2PP_RPS(Class), Return Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #Return, #Class "::" #Method, H2ARGV(Args), __FILE__, __LINE__)

#define __H2MOCK(Expression, ...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__, (Expression))
#define H2MOCK(...) __H2MOCK(#__VA_ARGS__, __VA_ARGS__)
// h2_mocker.hpp

namespace {

template <int Counter, typename Class, typename Signature>
class h2_mocker;

template <int Counter, typename Class, typename Return, typename... Args>
class h2_mocker<Counter, Class, Return(Args...)> : h2_mock {
   /* clang-format off */
#define MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F  \
   h2_matcher<h2_nth_decay<0, Args...>> _0 = {},     \
   h2_matcher<h2_nth_decay<1, Args...>> _1 = {},     \
   h2_matcher<h2_nth_decay<2, Args...>> _2 = {},     \
   h2_matcher<h2_nth_decay<3, Args...>> _3 = {},     \
   h2_matcher<h2_nth_decay<4, Args...>> _4 = {},     \
   h2_matcher<h2_nth_decay<5, Args...>> _5 = {},     \
   h2_matcher<h2_nth_decay<6, Args...>> _6 = {},     \
   h2_matcher<h2_nth_decay<7, Args...>> _7 = {},     \
   h2_matcher<h2_nth_decay<8, Args...>> _8 = {},     \
   h2_matcher<h2_nth_decay<9, Args...>> _9 = {},     \
   h2_matcher<h2_nth_decay<10, Args...>> _10 = {},   \
   h2_matcher<h2_nth_decay<11, Args...>> _11 = {},   \
   h2_matcher<h2_nth_decay<12, Args...>> _12 = {},   \
   h2_matcher<h2_nth_decay<13, Args...>> _13 = {},   \
   h2_matcher<h2_nth_decay<14, Args...>> _14 = {},   \
   h2_matcher<h2_nth_decay<15, Args...>> _15 = {}

#define FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F        \
   std::forward<h2_matcher<h2_nth_decay<0, Args...>>>(_0),     \
   std::forward<h2_matcher<h2_nth_decay<1, Args...>>>(_1),     \
   std::forward<h2_matcher<h2_nth_decay<2, Args...>>>(_2),     \
   std::forward<h2_matcher<h2_nth_decay<3, Args...>>>(_3),     \
   std::forward<h2_matcher<h2_nth_decay<4, Args...>>>(_4),     \
   std::forward<h2_matcher<h2_nth_decay<5, Args...>>>(_5),     \
   std::forward<h2_matcher<h2_nth_decay<6, Args...>>>(_6),     \
   std::forward<h2_matcher<h2_nth_decay<7, Args...>>>(_7),     \
   std::forward<h2_matcher<h2_nth_decay<8, Args...>>>(_8),     \
   std::forward<h2_matcher<h2_nth_decay<9, Args...>>>(_9),     \
   std::forward<h2_matcher<h2_nth_decay<10, Args...>>>(_10),   \
   std::forward<h2_matcher<h2_nth_decay<11, Args...>>>(_11),   \
   std::forward<h2_matcher<h2_nth_decay<12, Args...>>>(_12),   \
   std::forward<h2_matcher<h2_nth_decay<13, Args...>>>(_13),   \
   std::forward<h2_matcher<h2_nth_decay<14, Args...>>>(_14),   \
   std::forward<h2_matcher<h2_nth_decay<15, Args...>>>(_15)
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
                                   h2_matcher<h2_nth_decay<9, Args...>>,
                                   h2_matcher<h2_nth_decay<10, Args...>>,
                                   h2_matcher<h2_nth_decay<11, Args...>>,
                                   h2_matcher<h2_nth_decay<12, Args...>>,
                                   h2_matcher<h2_nth_decay<13, Args...>>,
                                   h2_matcher<h2_nth_decay<14, Args...>>,
                                   h2_matcher<h2_nth_decay<15, Args...>>>;

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
         h2_fail* fails = h2_tuple_matches(matcher_array[i], at);
         if (fails) {
            if (checkin_offset != -1) {
               break;
            }
            if (checkin_array[i].is_satisfied()) { /* try next h2_checkin */
               delete fails;
               continue;
            }
            fails->foreach([this](h2_fail* f, int, int) {
               f->explain += "on " + (func + argvs(f->seqno));
            });
            h2_fail* fail = h2_fail::new_normal(signature(), file, lino);
            h2_fail::append_child(fail, fails);
            h2_fail_g(fail, false);
         } else {
            checkin_index = i;
            checkin_offset = i;
            if (checkin_array[i].is_saturated()) {
               continue;
            }
            if (checkin_array[i].insufficient()) {
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
         h2_fail* fail = h2_fail::new_normal(signature(), file, lino);
         h2_fail* f = h2_fail::new_normal(func + h2_representify(at) + color(" unexpectedly", "red,bold") + " called");
         h2_fail::append_child(fail, f);
         h2_fail_g(fail, false);
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

   static h2_mocker& I(void* origin_fp, const char* ret, const char* func, const h2_vector<const char*>& argv, const char* file, int lino)
   {
      I().origin_fp = origin_fp;
      I().substitute_fp = std::is_same<std::false_type, Class>::value ? (void*)normal_function_stub : (void*)member_function_stub;
      I().ret = ret;
      I().func = func;
      I().argv = argv;
      I().file = file;
      I().lino = lino;
      I().reset();
      I().mock();
      return I();
   }

   h2_mocker& greed(bool mode)
   {
      greed_mode = mode;
      return *this;
   }

   h2_mocker& once(MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      checkin_array.push_back(h2_checkin(1, 1));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& twice(MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      checkin_array.push_back(h2_checkin(2, 2));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& times(int count, MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      checkin_array.push_back(h2_checkin(count, count));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& any(MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      checkin_array.push_back(h2_checkin(0, INT_MAX));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& atleast(int count, MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      checkin_array.push_back(h2_checkin(count, INT_MAX));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& atmost(int count, MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      checkin_array.push_back(h2_checkin(0, count));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& between(int left, int right, MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      checkin_array.push_back(h2_checkin(left, right));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& with(MATCHER_Any_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F)
   {
      if (!matcher_array.empty()) {
         matcher_array.back() = std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9_A_B_C_D_E_F);
      }
      return *this;
   }

   /* clang-format off */
   h2_mocker& th0(h2_matcher<h2_nth_decay<0, Args...>> e = {}) { if (!matcher_array.empty()) std::get<0>(matcher_array.back()) = e; return *this; }
   h2_mocker& th1(h2_matcher<h2_nth_decay<1, Args...>> e = {}) { if (!matcher_array.empty()) std::get<1>(matcher_array.back()) = e; return *this; }
   h2_mocker& th2(h2_matcher<h2_nth_decay<2, Args...>> e = {}) { if (!matcher_array.empty()) std::get<2>(matcher_array.back()) = e; return *this; }
   h2_mocker& th3(h2_matcher<h2_nth_decay<3, Args...>> e = {}) { if (!matcher_array.empty()) std::get<3>(matcher_array.back()) = e; return *this; }
   h2_mocker& th4(h2_matcher<h2_nth_decay<4, Args...>> e = {}) { if (!matcher_array.empty()) std::get<4>(matcher_array.back()) = e; return *this; }
   h2_mocker& th5(h2_matcher<h2_nth_decay<5, Args...>> e = {}) { if (!matcher_array.empty()) std::get<5>(matcher_array.back()) = e; return *this; }
   h2_mocker& th6(h2_matcher<h2_nth_decay<6, Args...>> e = {}) { if (!matcher_array.empty()) std::get<6>(matcher_array.back()) = e; return *this; }
   h2_mocker& th7(h2_matcher<h2_nth_decay<7, Args...>> e = {}) { if (!matcher_array.empty()) std::get<7>(matcher_array.back()) = e; return *this; }
   h2_mocker& th8(h2_matcher<h2_nth_decay<8, Args...>> e = {}) { if (!matcher_array.empty()) std::get<8>(matcher_array.back()) = e; return *this; }
   h2_mocker& th9(h2_matcher<h2_nth_decay<9, Args...>> e = {}) { if (!matcher_array.empty()) std::get<9>(matcher_array.back()) = e; return *this; }
   h2_mocker& th10(h2_matcher<h2_nth_decay<10, Args...>> e = {}) { if (!matcher_array.empty()) std::get<10>(matcher_array.back()) = e; return *this; }
   h2_mocker& th11(h2_matcher<h2_nth_decay<11, Args...>> e = {}) { if (!matcher_array.empty()) std::get<11>(matcher_array.back()) = e; return *this; }
   h2_mocker& th12(h2_matcher<h2_nth_decay<12, Args...>> e = {}) { if (!matcher_array.empty()) std::get<12>(matcher_array.back()) = e; return *this; }
   h2_mocker& th13(h2_matcher<h2_nth_decay<13, Args...>> e = {}) { if (!matcher_array.empty()) std::get<13>(matcher_array.back()) = e; return *this; }
   h2_mocker& th14(h2_matcher<h2_nth_decay<14, Args...>> e = {}) { if (!matcher_array.empty()) std::get<14>(matcher_array.back()) = e; return *this; }
   h2_mocker& th15(h2_matcher<h2_nth_decay<15, Args...>> e = {}) { if (!matcher_array.empty()) std::get<15>(matcher_array.back()) = e; return *this; }
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

}  // namespace
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
      h2_fail::append_subling(fails, h2_matcher_cast<const int>(size).matches(a->data.size(), 0, caseless, dont));
      return fails;
   }

   virtual h2_line expection(bool, bool) const override { return ""; }
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
#define H2SOCK(...) H2PP_CAT(__H2SOCK, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
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
   int lino;
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

   h2_case(const char* name, int status, const char* file, int lino);
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
   int lino;
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

   h2_suite(const char* name, void (*)(h2_suite*, h2_case*), const char* file, int lino);
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
   int lino;
   h2_fail* fail{nullptr};
   h2_ostringstream oss;

   h2_defer_fail(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int lino_);
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

static inline h2_ostringstream& h2_JE(h2_defer_fail* d, h2_string e, h2_string a, h2_string selector)
{
   d->check_type = "JE";
   h2::h2_matcher<h2_string> m = Je(e, selector);
   d->fail = m.matches(a);
   h2_check_g();
   return d->oss;
}

#define __H2OK(Qt, expression, ...) \
   for (h2::h2_defer_fail Qt("", "", expression, __FILE__, __LINE__); Qt;) h2::h2_OK(&Qt, __VA_ARGS__)

#define __H2JE3(Qt, expect, actual) \
   for (h2::h2_defer_fail Qt(#expect, #actual, "", __FILE__, __LINE__); Qt;) h2::h2_JE(&Qt, expect, actual, "")

#define __H2JE4(Qt, expect, actual, selector) \
   for (h2::h2_defer_fail Qt(#expect, #actual, "", __FILE__, __LINE__); Qt;) h2::h2_JE(&Qt, expect, actual, selector)

#define H2OK(...) __H2OK(H2Q(t_defer_fail), (#__VA_ARGS__), __VA_ARGS__)

#define H2JE(...) H2PP_VARIADIC_CALL(__H2JE, H2Q(t_defer_fail), __VA_ARGS__)
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
#endif
