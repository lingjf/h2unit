/* v4.1  2020-02-16 23:37:36 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#ifndef ___H2UNIT_H___
#define ___H2UNIT_H___

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <cstring>
#include <cassert>
#include <cctype>  /* tolower */
#include <climits> /* INT_MAX */
#include <csetjmp> /* setjmp, longjmp */
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <tuple>
#include <random>    /* shuffle */
#include <algorithm> /* shuffle */
#include <functional>
#include <utility> /* forward_as_tuple */
#include <memory>  /* allocator */
#include <type_traits>
#include <typeinfo> /* typeid */
#include <errno.h>
#include <unistd.h>    /* sysconf */
#include <signal.h>    /* sigaction */
#include <alloca.h>    /* alloca */
#include <execinfo.h>  /* backtrace */
#include <cxxabi.h>    /* demangle */
#include <sys/mman.h>  /* mprotect, mmap */
#include <sys/ioctl.h> /* ioctl */
#include <sys/time.h>  /* gettimeofday */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>     /* getaddrinfo, gethostbyname */
#include <arpa/inet.h> /* inet_addr */

#if defined __GLIBC__
#   include <malloc.h> /* __malloc_hook */
#elif defined __APPLE__
#   include <AvailabilityMacros.h>
#   include <malloc/malloc.h> /* malloc_zone_t */
#elif defined _WIN32
#   include <windows.h>
#endif

#if defined __GNUC__
#   pragma GCC diagnostic ignored "-Wsign-compare"
#   pragma GCC diagnostic ignored "-Wwrite-strings"
#   pragma GCC diagnostic ignored "-Wdangling-else"
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined __clang__
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wwritable-strings"
#   pragma clang diagnostic ignored "-Wdangling-else"
#endif

//
//
//
//
//
//
// Pin me at LINE 70, otherwise relocate README.md #L
#define __H2OK4(condition, _1, _2, f)                         \
   do {                                                       \
      if (!(condition)) {                                     \
         h2_fail* f = new h2_fail_normal(__FILE__, __LINE__); \
         f->kprintf("%s is false", #condition);               \
         h2_fail_g(f);                                        \
      }                                                       \
   } while (0)

#define __H2OK5(expect, actual, a, e, f)                             \
   do {                                                              \
      auto a = actual;                                               \
      h2_matcher<typename std::conditional<                          \
        std::is_enum<typename std::decay<decltype(a)>::type>::value, \
        int, /* translate enum type to int type */                   \
        typename std::decay<decltype(a)>::type>::type>               \
        e(expect);                                                   \
      h2_fail* f = e.matches(a);                                     \
      if (f) {                                                       \
         f->locate(__FILE__, __LINE__);                              \
         h2_fail_g(f);                                               \
      }                                                              \
   } while (0)

#define H2OK(...) H2PP_VARIADIC_CALL(__H2OK, __VA_ARGS__, H2Q(a), H2Q(e), H2Q(f))

#define H2JE(expect, actual) __H2OK5(Je(expect), actual, H2Q(a), H2Q(e), H2Q(f))

#ifndef OK
#   define OK H2OK
#endif
#ifndef JE
#   define JE H2JE
#endif

#define __H2SUITE(Suitename, s, a)                          \
   static void s(h2_suite*, h2_case*);                      \
   static h2_suite a(Suitename, &s, __FILE__, __LINE__, 0); \
   static void s(h2_suite* ___suite, h2_case* case___)

#define H2SUITE(...) __H2SUITE(H2PP_STRINGIZE(__VA_ARGS__), H2Q(suite), H2Q(a))
#ifndef SUITE
#   define SUITE H2SUITE
#endif

#define __H2Setup(t) \
   for (int t = 1; t--; case___ ? void() : h2_suite_setup_g(___suite)) ___suite->setup = [&]()
#define __H2Teardown(t) \
   for (int t = 1; t--; case___ ? void() : h2_suite_teardown_g(___suite)) ___suite->teardown = [&]()

#define H2Setup() __H2Setup(H2Q(t))
#define H2Teardown() __H2Teardown(H2Q(t))
#ifndef Setup
#   define Setup H2Setup
#endif
#ifndef Teardown
#   define Teardown H2Teardown
#endif

#define __H2Case1(Casename, Status, c, t)                            \
   static h2_case c(Casename, ___suite, Status, __FILE__, __LINE__); \
   if (&c == case___)                                                \
      for (bool t = true; t; c.execute(), t = false) c.ul_code = [&]()

#define __H2Case2(Casename, Status, c, t)                            \
   static h2_case c(Casename, ___suite, Status, __FILE__, __LINE__); \
   if (&c == case___)                                                \
      for (h2_case::T t(&c); t && ::setjmp(c.jb) == 0;)

#ifdef H2C
#   define H2Case(...) __H2Case1(H2PP_STRINGIZE(__VA_ARGS__), h2_case::INITED, H2Q(c), H2Q(t))
#   define H2Todo(...) __H2Case1(H2PP_STRINGIZE(__VA_ARGS__), h2_case::TODOED, H2Q(c), H2Q(t))
#else
#   define H2Case(...) __H2Case2(H2PP_STRINGIZE(__VA_ARGS__), h2_case::INITED, H2Q(c), H2Q(t))
#   define H2Todo(...) __H2Case2(H2PP_STRINGIZE(__VA_ARGS__), h2_case::TODOED, H2Q(c), H2Q(t))
#endif

#ifndef Case
#   define Case H2Case
#endif
#ifndef Todo
#   define Todo H2Todo
#endif

#define __H2CASE(Casename, Status, a, C, t)                                        \
   static h2_suite a("", h2_suite::execute, __FILE__, __LINE__, 1);                \
   namespace {                                                                     \
   struct C : private h2_case {                                                    \
      C(h2_suite* suite) : h2_case(Casename, suite, Status, __FILE__, __LINE__) {} \
      void uf_code() override;                                                     \
   };                                                                              \
   static C t(&a);                                                                 \
   }                                                                               \
   void C::uf_code()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, h2_case::INITED, H2Q(a), H2Q(h2_case), H2Q(t))
#define H2TODO(...) __H2CASE(#__VA_ARGS__, h2_case::TODOED, H2Q(a), H2Q(h2_case), H2Q(t))
#ifndef CASE
#   define CASE H2CASE
#endif
#ifndef TODO
#   define TODO H2TODO
#endif

#define __H2MOCK2(BeFunc, Signature) \
   h2_mocker<__COUNTER__, __LINE__, std::false_type, Signature>::I((void*)BeFunc, #BeFunc, __FILE__, __LINE__)

#define __H2MOCK3(Class, Method, Signature) \
   h2_mocker<__COUNTER__, __LINE__, Class, Signature>::I(h2_mfp<Class, Signature>::A(&Class::Method, "MOCK", "", #Class, #Method, #Signature, __FILE__, __LINE__), #Class "::" #Method, __FILE__, __LINE__)

#define __H2MOCK4(Class, Method, Signature, Instance) \
   h2_mocker<__COUNTER__, __LINE__, Class, Signature>::I(h2_mfp<Class, Signature>::A(&Class::Method, Instance), #Class "::" #Method, __FILE__, __LINE__)

#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)

#ifndef MOCK
#   define MOCK H2MOCK
#endif

#define __H2STUB3(BeFunc, ToFunc, _1)                                                \
   do {                                                                              \
      h2_stub_g((void*)BeFunc, (void*)ToFunc, #BeFunc, #ToFunc, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB4(Return, BeFunc, Args, Q)                                      \
   struct {                                                                     \
      void operator=(Return(*toF) Args) {                                       \
         Return(*beF) Args = BeFunc;                                            \
         h2_stub_g((void*)beF, (void*)(toF), #BeFunc, "~", __FILE__, __LINE__); \
      }                                                                         \
   } Q;                                                                         \
   Q = [] Args -> Return

#define __H2STUB50(Return, Class, Method, Args, Q)                                                                                                                                          \
   struct {                                                                                                                                                                                 \
      void operator=(Return (*toF)(Class * that)) {                                                                                                                                         \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                     \
   } Q;                                                                                                                                                                                     \
   Q = [](Class * that) -> Return

#define __H2STUB51(Return, Class, Method, Args, Q)                                                                                                                                          \
   struct {                                                                                                                                                                                 \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                                                                          \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                     \
   } Q;                                                                                                                                                                                     \
   Q = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB5(Return, Class, Method, Args, Q) \
   H2PP_IF(H2PP_0ARGS Args, __H2STUB50(Return, Class, Method, Args, Q), __H2STUB51(Return, Class, Method, Args, Q))

#define __H2STUB60(Return, Class, Method, Args, Instance, Q)                                                                             \
   struct {                                                                                                                              \
      void operator=(Return (*toF)(Class * that)) {                                                                                      \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                  \
   } Q;                                                                                                                                  \
   Q = [](Class * that) -> Return

#define __H2STUB61(Return, Class, Method, Args, Instance, Q)                                                                             \
   struct {                                                                                                                              \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                       \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                  \
   } Q;                                                                                                                                  \
   Q = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB6(Return, Class, Method, Args, Instance, Q) \
   H2PP_IF(H2PP_0ARGS Args, __H2STUB60(Return, Class, Method, Args, Instance, Q), __H2STUB61(Return, Class, Method, Args, Instance, Q))

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__, H2Q(t))

#ifndef STUB
#   define STUB H2STUB
#endif

#define __H2BLOCK0(t) for (h2_stack::T t(__FILE__, __LINE__); t;)

#define __H2BLOCK1(t, ...) for (h2_stack::T t(__FILE__, __LINE__, __VA_ARGS__); t;)

#define H2BLOCK(...) H2PP_IF(H2PP_0ARGS(__VA_ARGS__), __H2BLOCK0(H2Q(t)), __H2BLOCK1(H2Q(t), __VA_ARGS__))
// #define H2BLOCK(...) for (h2_stack::T t(__FILE__, __LINE__, ##__VA_ARGS__); t;)
// #define H2BLOCK(...) for (h2_stack::T t(__FILE__, __LINE__, __VA_OPT__(,) __VA_ARGS__); t;)

#ifndef BLOCK
#   define BLOCK H2BLOCK
#endif

static inline void h2_fail_g(void* fail);

/* https://www.boost.org/doc/libs/1_65_0/libs/preprocessor/doc/index.html */

#define H2PP_CAT2(_1, _2) _H2PP_CAT2(_1, _2)
#define _H2PP_CAT2(_1, _2) _1##_2
#define H2PP_CAT5(_1, _2, _3, _4, _5) _H2PP_CAT5(_1, _2, _3, _4, _5)
#define _H2PP_CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5

#define H2PP_STRINGIZE(...) _H2PP_STRINGIZE(__VA_ARGS__)
#define _H2PP_STRINGIZE(...) #__VA_ARGS__

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

/* clang-format off */
#define H2PP_IF(_Cond, _Then, _Else) H2PP_CAT2(_H2PP_IF_, _Cond) (_Then, _Else)
#define _H2PP_IF_1(_Then, _Else) _Then
#define _H2PP_IF_0(_Then, _Else) _Else
/* clang-format on */

#define _H2PP_NARGS(...) _H2PP_16TH(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define _H2PP_HAS_COMMA(...) _H2PP_16TH(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _H2PP_16TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define _H2PP_COMMA(...) ,

#define H2PP_0ARGS(...) _H2PP_0ARGS(_H2PP_HAS_COMMA(__VA_ARGS__),             \
                                    _H2PP_HAS_COMMA(_H2PP_COMMA __VA_ARGS__), \
                                    _H2PP_HAS_COMMA(__VA_ARGS__()),           \
                                    _H2PP_HAS_COMMA(_H2PP_COMMA __VA_ARGS__()))

#define _H2PP_0ARGS(_1, _2, _3, _4) _H2PP_HAS_COMMA(_H2PP_CAT5(_H2PP_0ARGS_CASE_, _1, _2, _3, _4))

#define _H2PP_0ARGS_CASE_0001 ,

#define H2PP_NARGS(...) H2PP_IF(H2PP_0ARGS(__VA_ARGS__), 0, _H2PP_NARGS(__VA_ARGS__))

/* clang-format off */
#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_CAT2(_Macro, H2PP_NARGS(__VA_ARGS__)) (__VA_ARGS__)
/* clang-format on */

#define H2Q(Prefix) H2PP_CAT5(Prefix, _, __COUNTER__, _, __LINE__)

// H2_ALIGN_UP(15, 8) == 16
#define H2_ALIGN_UP(n, s) (((n) + (s)-1) / (s) * (s))
// H2_ALIGN_DOWN(15, 8) == 8
#define H2_ALIGN_DOWN(n, s) ((n) / (s) * (s))
// H2_DIV_ROUND_UP(15, 8) == 2
#define H2_DIV_ROUND_UP(n, s) (((n) + (s)-1) / (s))

struct h2_with {
   FILE* f;
   int (*c)(FILE*);
   h2_with(FILE* file, int (*close)(FILE*) = ::fclose) : f(file), c(close) {}
   ~h2_with() { f&& c&& c(f); }
};

static inline bool h2_wildcard_match(const char* pattern, const char* subject) {
   const char *scur = subject, *pcur = pattern;
   const char *sstar = nullptr, *pstar = nullptr;
   while (*scur) {
      if (*scur == *pcur || *pcur == '?') {
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
}

static inline long long h2_milliseconds() {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

static inline const char* h2_style(const char* style_str, char* style_abi) {
   static struct {
      const char *name, *value;
   } K[] = {
     {"reset", "0;"},
     {"bold", "1;"},
     {"italics", "3;"},
     {"underline", "4;"},
     {"inverse", "7;"},
     {"strikethrough", "9;"},
     {"black", "30;"},
     {"red", "31;"},
     {"green", "32;"},
     {"yellow", "33;"},
     {"blue", "34;"},
     {"purple", "35;"},
     {"cyan", "36;"},
     {"gray", "37;"},
     {"default", "39;"},
     {"dark gray", "90;"},
     {"light red", "91;"},
     {"light green", "92;"},
     {"light yellow", "93;"},
     {"light blue", "94;"},
     {"light purple", "95;"},
     {"light cyan", "96;"},
     {"white", "97;"},
     {"bg_black", "40;"},
     {"bg_red", "41;"},
     {"bg_green", "42;"},
     {"bg_yellow", "43;"},
     {"bg_blue", "44;"},
     {"bg_purple", "45;"},
     {"bg_cyan", "46;"},
     {"bg_white", "47;"},
     {"bg_default", "49;"}};

   char t[1024];
   strcpy(t, style_str);

   strcpy(style_abi, "\033[");

   for (char* p = strtok(t, ","); p; p = strtok(NULL, ","))
      for (size_t i = 0; i < sizeof(K) / sizeof(K[0]); i++)
         if (!strcmp(K[i].name, p)) {
            strcat(style_abi, K[i].value);
            break;
         }

   style_abi[strlen(style_abi) - 1] = 'm';

   return style_abi;
}

static inline const char* h2_acronym_string(const char* full, int atmost = 10) {
   static char st[32];
   strncpy(st, full, atmost);
   strcpy(st + atmost, "...");
   return st;
}

static inline const char* h2_center_string(const char* str, int width, char* out) {
   int z = strlen(str), l = (width - z) / 2, r = width - l - z;
   char t[32];
   sprintf(t, "%%%ds%%s%%%ds", l, r);
   sprintf(out, t, "", str, "");
   return out;
}

static inline bool h2_endswith_string(const char* haystack, const char* needle) {
   int haystack_length = strlen(haystack), needle_length = strlen(needle);
   return haystack_length < needle_length ? false : strncmp(haystack + haystack_length - needle_length, needle, needle_length) == 0;
}

#if defined _WIN32
#   define h2_selectany __declspec(selectany)
#else
#   define h2_selectany __attribute__((weak))
#endif

#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))

#define _h2_h2_list_for_each_safe(p, t, head) for (h2_list* p = (head)->next, *t = p->next; p != (head); p = t, t = t->next)
#define h2_list_for_each(p, head) _h2_h2_list_for_each_safe(p, H2Q(t), head)

#define _h2_list_for_each_entry_safe(p, t, head, type, link) for (type* p = h2_list_entry((head)->next, type, link), *t = h2_list_entry(p->link.next, type, link); &p->link != (head); p = t, t = h2_list_entry(t->link.next, type, link))
#define h2_list_for_each_entry(p, head, type, link) _h2_list_for_each_entry_safe(p, H2Q(t), head, type, link)

#define h2_list_pop_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->pop(), type, link))
#define h2_list_top_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_first(), type, link))
#define h2_list_bottom_entry(head, type, link) ((head)->empty() ? (type*)0 : h2_list_entry((head)->get_last(), type, link))

struct h2_list {
   struct h2_list *next, *prev;

   static void __add_between(h2_list* new_, h2_list* prev, h2_list* next) {
      next->prev = new_;
      new_->next = next;
      new_->prev = prev;
      prev->next = new_;
   }

   h2_list() : next(this), prev(this) {}

   void add_head(h2_list* entry) { __add_between(entry, this, this->next); }

   void add_tail(h2_list* entry) { __add_between(entry, this->prev, this); }

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

struct h2_configure {
   const char* path;
   long platform;

   char listing;
   long breakable;
   bool verbose;
   bool colorable;
   bool randomize;
   bool memory_check;
   char junit[256];

   const char *include_patterns[9], *exclude_patterns[9];

   h2_configure()
     : path(nullptr),
#if defined __linux__
       platform(1),
#elif defined __APPLE__
       platform(2),
#elif defined _WIN32
       platform(3),
#endif
       listing('\0'),
       breakable(0),
       verbose(false),
       colorable(true),
       randomize(false),
       memory_check(true),
       junit{0},
       include_patterns{0},
       exclude_patterns{0} {
   }

   int isLinux() const { return 1 == platform; }
   int isMAC() const { return 2 == platform; }
   int isWindows() const { return 3 == platform; }

   /* clang-format off */
   static h2_configure& I() { static h2_configure __; return __; }
   /* clang-format on */

   void usage() {
      printf("Usage:\n"
             "-v                  Make the operation more talkative\n"
             "-l [sc]             List out all suites and cases\n"
             "-b [n]              Breaking test once n (default is 1) failures occurred\n"
             "-c                  Output in black-white color mode\n"
             "-r                  Run cases in random order\n"
             "-m                  Run cases without memory check\n"
             "-j [path]           Generate junit report, default is .xml\n"
             "-i {patterns}       Run cases which case name, suite name or file name matches\n"
             "-x {patterns}       Run cases which case name, suite name and file name not matches\n");
   }

   void insert(const char* patterns[9], const char* pattern) {
      for (int i = 0; i < 9; ++i)
         if (!patterns[i]) {
            patterns[i] = pattern;
            break;
         }
   }

   void opt(int argc, const char** argv) {
      path = argv[0];

      for (int i = 1; i < argc; ++i) {
         if (argv[i][0] != '-') continue;
         for (const char* p = argv[i] + 1; *p; p++) {
            switch (*p) {
            case 'v': verbose = true; break;
            case 'l':
               listing = 'A';
               if (i + 1 < argc && argv[i + 1][0] != '-') listing = argv[++i][0];
               break;
            case 'b':
               breakable = 1;
               if (i + 1 < argc && argv[i + 1][0] != '-') breakable = atoi(argv[++i]);
               break;
            case 'c': colorable = !colorable; break;
            case 'r': randomize = true; break;
            case 'm': memory_check = !memory_check; break;
            case 'j':
               sprintf(junit, "%s.xml", path);
               if (i + 1 < argc && argv[i + 1][0] != '-') strcpy(junit, argv[++i]);
               break;
            case 'i':
               for (int j = i + 1; j < argc && argv[j][0] != '-'; ++j, ++i) insert(include_patterns, argv[j]);
               break;
            case 'x':
               for (int j = i + 1; j < argc && argv[j][0] != '-'; ++j, ++i) insert(exclude_patterns, argv[j]);
               break;
            case '-': break;
            case 'h':
            case '?':
               usage();
               exit(0);
            default:
               printf("unknown option: -%c, -h for help\n", *p);
               exit(0);
            }
         }
      }
   }

   int filter(const char* patterns[9], const char* subject) {
      for (int i = 0; i < 9 && patterns[i]; ++i)
         if (strchr(patterns[i], '?') || strchr(patterns[i], '*')) {
            if (h2_wildcard_match(patterns[i], subject)) return 1;
         } else {
            if (strstr(subject, patterns[i])) return 1;
         }
      return patterns[0] ? 0 : -1;
   }

   bool filter(const char* suitename, const char* casename, const char* filename) {
      if (0 == filter(include_patterns, suitename) && 0 == filter(include_patterns, casename) && 0 == filter(include_patterns, filename))
         return true;
      if (1 == filter(exclude_patterns, suitename) || 1 == filter(exclude_patterns, casename) || 1 == filter(exclude_patterns, filename))
         return true;
      return false;
   }

   const char* style(const char* style_str) {
      if (!I().colorable) return "";

      static char shift_buffer[8][128];
      static long shift_index = 0;

      return h2_style(style_str, shift_buffer[++shift_index % 8]);
   }

   int get_term_columns() {
      struct winsize w;
      if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) return 80;
      return w.ws_col;
   }
};

static inline h2_configure& h2cfg() { return h2_configure::I(); }

struct h2_raw {
   static void* malloc(size_t sz) {
      if (!h2cfg().memory_check) {
         return ::malloc(sz);
      }
      void* ptr = mmap(nullptr, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (ptr == MAP_FAILED) return nullptr;

      uintptr_t* p = (uintptr_t*)ptr;
      *p = (uintptr_t)sz;
      return (void*)(p + 1);
   }

   static void free(void* ptr) {
      if (!h2cfg().memory_check) {
         ::free(ptr);
         return;
      }
      if (!ptr) return;
      uintptr_t* p = ((uintptr_t*)ptr) - 1;
      munmap((void*)p, (size_t)*p);
   }
};

/* clang-format off */
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

   pointer allocate(size_type n, const void* = 0) { return (T*)h2_raw::malloc(n * sizeof(T)); }
   void deallocate(void* p, size_type) { h2_raw::free(p); }

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
/* clang-format on */

template <typename T>
class h2_shared_ptr {
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
            pn = new (h2_raw::malloc(sizeof(long))) long(1);
         else
            ++(*pn);
      }
      px = p;
   }
   void release() {
      if (pn && !--(*pn)) {
         delete px;
         h2_raw::free(pn);
      }
   }

   T* px;
   long* pn;

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
};

class h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
 public:
   h2_string() : basic_string() {}
   h2_string(const h2_string& __str) : basic_string(__str.c_str()) {}
   h2_string(const char* __s) : basic_string(__s) {}
   h2_string(const std::string& __s) : basic_string(__s.c_str()) {}
   h2_string(size_t __n, char __c) : basic_string(__n, __c) {}

   h2_string& operator=(const h2_string& __str) { return this->assign(__str.c_str()), *this; }
   h2_string& operator=(const char* __s) { return this->assign(__s), *this; }
   h2_string& operator=(const std::string& __str) { return this->assign(__str.c_str()), *this; }
   h2_string& operator=(char __c) { return this->assign(1, __c), *this; }

   h2_string& operator+=(const h2_string& __str) { return this->append(__str.c_str()), *this; }
   h2_string& operator+=(const char* __s) { return this->append(__s), *this; }
   h2_string& operator+=(const std::string& __str) { return this->append(__str.c_str()), *this; }
   h2_string& operator+=(char __c) { return this->push_back(__c), *this; }

   h2_string& printf(const char* format, ...) {
      va_list a;

      va_start(a, format);
#if defined _WIN32
      int length = _vscprintf(format, a);
#else
      int length = vsnprintf(nullptr, 0, format, a);
#endif
      va_end(a);

      char* t = (char*)alloca(length + 1);

      va_start(a, format);
      vsprintf(t, format, a);
      va_end(a);

      append(t);

      return *this;
   }

   bool equals(h2_string __str, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__str);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2 == e2;
   }

   bool contains(h2_string __substr, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__substr);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2.find(e2) != h2_string::npos;
   }

   bool startswith(h2_string __prefix, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__prefix);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2.length() >= e2.length() && a2.substr(0, e2.length()) == e2;
   }

   bool endswith(h2_string __suffix, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__suffix);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2.length() >= e2.length() && a2.substr(a2.length() - e2.length()) == e2;
   }
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
// TINYEXPR - Tiny recursive descent parser and evaluation engine in C
//
// Copyright (c) 2015-2018 Lewis Van Winkle
//
// http://CodePlea.com

#ifndef __TINYEXPR_H__
#define __TINYEXPR_H__

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


class tinyexpr
{
public:    
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

private:

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
        te_expr *ret = (te_expr *)h2_raw::malloc(size);
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
        h2_raw::free((void *)n);
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


#endif /*__TINYEXPR_H__*/

struct h2_json {
   static const int t_absent = 0;

   static const int t_null = 1;
   static const int t_boolean = 2;
   static const int t_number = 3;
   static const int t_string = 4;
   static const int t_regexp = 5;
   static const int t_array = 6;
   static const int t_object = 7;

   static constexpr char indent_char = ' ';
   static constexpr char samelength_char = ' ';
   static constexpr char occupy_char = ' ';
   static constexpr char columns_char = ' ';

   struct P {
      const char* text;
      int length;
      int offset;

      struct P& strip() {
         while (offset < length && ::isspace(text[offset])) offset++;
         return *this;
      }
      bool startswith(const char* s, int n) {
         if (length - offset < n) return false;
         return ::strncmp(text + offset, s, n) == 0;
      }
      bool startswith(char from, char to = '\0') {
         if (length - offset < 1) return false;
         if (to == '\0') to = from;
         return from <= text[offset] && text[offset] <= to;
      }
   };

   struct Node {
      int type;

      h2_string key_string;
      h2_string value_string;
      double value_double;
      bool value_boolean;
      h2_vector<Node*> children; /* array or object */

      Node() : type(t_null), value_double(0), value_boolean(false) {}

      int size() { return children.size(); }

      Node* get(int index) { return 0 <= index && index < children.size() ? children[index] : nullptr; }

      Node* get(const char* name) {
         if (!name) return nullptr;

         for (auto node : children)
            if (!node->key_string.compare(name))
               return node;

         return nullptr;
      }

      void del(Node* child) {
         for (auto it = children.begin(); it != children.end(); it++)
            if (child == *it) {
               children.erase(it);
               delete child;
               return;
            }
      }

      bool is_null() { return t_null == type; }
      bool is_bool() { return t_boolean == type; }
      bool is_number() { return t_number == type; }
      bool is_string() { return t_string == type; }
      bool is_regexp() { return t_regexp == type; }
      bool is_array() { return t_array == type; }
      bool is_object() { return t_object == type; }

      bool parse_number(P& x) {
         int i;
         for (i = 0; x.offset + i < x.length; ++i) {
            const char c = x.text[x.offset + i];
            if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0')
               break;
         }

         value_string.assign(x.text + x.offset, i);

         int err = 0;
         value_double = tinyexpr::te_interp(value_string.c_str(), &err);
         type = t_number;
         x.offset += i;

         return 0 == err;
      }

      bool parse_string(P& x) {
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

      bool parse_regexp(P& x) {
         bool ret = parse_string(x);
         type = t_regexp;
         return ret;
      }

      bool parse_value(P& x) {
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
         /* t_string */
         if (x.startswith('\"') || x.startswith('\'')) return parse_string(x);
         /* t_regexp */
         if (x.startswith('/')) return parse_regexp(x);

         /* t_array */
         if (x.startswith('[')) return parse_array(x);
         /* t_object */
         if (x.startswith('{')) return parse_object(x);

         /* t_number */
         if (1 /* x.startswith('-') || x.startswith('0', '9') */) return parse_number(x);

         return false;
      }

      bool parse_array(P& x) {
         x.offset++;  //pass [

         while (!x.strip().startswith(']')) {
            Node* new_node = new Node();
            if (!new_node) return false;

            children.push_back(new_node);

            if (!new_node->parse_value(x)) return false;

            if (x.strip().startswith(',')) x.offset++;
         }

         type = t_array;
         x.offset++;

         return true;
      }

      bool parse_object(P& x) {
         x.offset++;  //pass {

         while (!x.strip().startswith('}')) {
            Node* new_node = new Node();
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

      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };

   static Node* parse(const char* json_string, int length = 0) {
      if (length == 0) length = strlen(json_string);

      if (!json_string || length == 0) return nullptr;

      P x;

      x.text = json_string;
      x.length = length;
      x.offset = 0;

      Node* root = new Node();
      if (!root->parse_value(x.strip())) return nullptr;

      return root;
   }

   static void frees(Node* root) {
      for (auto node : root->children) frees(node);
      delete root;
   }

   static bool match_array(Node* e, Node* a) {
      if (!e || !a) return false;
      if (e->children.size() != a->children.size()) return false;
      for (size_t i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->children[i]))
            return false;
      return true;
   }

   static bool match_object(Node* e, Node* a) {
      if (!e || !a) return false;
      if (e->children.size() > a->children.size()) return false;
      for (size_t i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->get(e->children[i]->key_string.c_str())))
            return false;
      return true;
   }

   static bool match(Node* e, Node* a) {
      if (!e || !a) return false;

      switch (e->type) {
      case t_null:
         if (a->is_null()) return true;
         break;
      case t_boolean:
         if (a->is_bool() && e->value_boolean == a->value_boolean) return true;
         break;
      case t_number:
         if (a->is_number() && fabs(e->value_double - a->value_double) < 0.00001) return true;
         break;
      case t_string:
         if (a->is_string() && e->value_string == a->value_string) return true;
         break;
      case t_regexp:
         if (a->is_string()) {
            bool result;
            try {
               std::regex re(e->value_string.c_str());
               result = std::regex_match(a->value_string.c_str(), re);
            }
            catch (const std::regex_error& e) {
               result = false;
            }
            return result;
         }
         break;
      case t_array:
         if (a->is_array() && match_array(e, a)) return true;
         break;
      case t_object:
         if (a->is_object() && match_object(e, a)) return true;
         break;
      };

      return false;
   }

   static bool match(const char* expect, const char* actual) {
      Node *e = parse(expect), *a = parse(actual);

      bool result = match(e, a);

      frees(e);
      frees(a);

      return result;
   }

   struct Dual {
      int depth;
      int e_type, a_type;
      h2_string e_key, a_key;
      h2_string e_value, a_value;
      h2_vector<Dual*> child;
      Dual* perent;

      Dual(int depth_, Dual* perent_) : depth(depth_), e_type(t_absent), a_type(t_absent), perent(perent_) {}

      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };

   static void frees(Dual* root) {
      for (auto dual : root->child) frees(dual);
      delete root;
   }

   static void node2dual(Node* node, int& type, h2_string& key, h2_string& value) {
      if (!node) return;

      char t[128];
      type = t_string;

      if (node->key_string.size()) key = "\"" + node->key_string + "\"";

      switch (node->type) {
      case t_null:
         type = t_string;
         value = "null";
         break;
      case t_boolean:
         type = t_string;
         value = node->value_boolean ? "true" : "false";
         break;
      case t_number:
         type = t_string;
         sprintf(t, "%1.15g", node->value_double);
         value = t;
         break;
      case t_string:
         type = t_string;
         value = "\"" + node->value_string + "\"";
         break;
      case t_regexp:
         type = t_string;
         value = node->value_string;
         break;
      case t_array:
         type = t_array;
         break;
      case t_object:
         type = t_object;
         break;
      }
   }

   static void samelengthify(h2_string& e, h2_string& a) {
      int e_l = e.length(), a_l = a.length();

      e.append(std::max(e_l, a_l) - e_l, samelength_char);
      a.append(std::max(e_l, a_l) - a_l, samelength_char);
   }

   static void dual(Node* e, Node* a, Dual* d) {
      node2dual(e, d->e_type, d->e_key, d->e_value);
      node2dual(a, d->a_type, d->a_key, d->a_value);
      samelengthify(d->e_key, d->a_key);
      samelengthify(d->e_value, d->a_value);

      if (d->e_type != d->a_type) {
         if (d->e_type == t_object) d->e_type = t_string, d->e_value = "{ ... }";
         if (d->e_type == t_array) d->e_type = t_string, d->e_value = "[ ... ]";
         if (d->a_type == t_object) d->a_type = t_string, d->a_value = "{ ... }";
         if (d->a_type == t_array) d->a_type = t_string, d->a_value = "[ ... ]";
         samelengthify(d->e_value, d->a_value);
         return;
      }

      if (d->e_type == t_object) {
         for (auto i = e->children.begin(); i != e->children.end();) {
            Node *e1 = *i, *a1 = a->get(e1->key_string.c_str());
            if (!a1)
               for (auto& j : a->children)
                  if (match(e1, j)) {
                     a1 = j;
                     break;
                  }
            if (a1) {
               Dual* d1 = new Dual(d->depth + 1, d);
               d->child.push_back(d1);
               dual(e1, a1, d1);
               a->del(a1);
               i = e->children.erase(i);
               delete e1;
            } else
               i++;
         }

         for (size_t i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            Dual* d1 = new Dual(d->depth + 1, d);
            d->child.push_back(d1);
            Node *e1 = e->get(i), *a1 = a->get(i);
            dual(e1, a1, d1);
         }
      }

      if (d->e_type == t_array) {
         for (size_t i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            Dual* d1 = new Dual(d->depth + 1, d);
            d->child.push_back(d1);
            Node *e1 = e->get(i), *a1 = a->get(i);
            dual(e1, a1, d1);
         }
      }
   }

   static h2_string indent(int depth) { return h2_string(depth * 2, indent_char); }
   static h2_string occupy(h2_string p) { return h2_string(p.length(), occupy_char); }

   static void diff(Dual* d, h2_vector<h2_string>& e, h2_vector<h2_string>& a) {
      if (!d) return;
      e.push_back("\n");
      e.push_back(indent(d->depth));
      a.push_back("\n");
      a.push_back(indent(d->depth));

      if (d->e_type != t_absent) {
         if (d->a_type == t_absent) {  // only e-side exist
            if (d->e_key.size()) {
               e.push_back("#cyan");
               e.push_back(d->e_key + ": ");
               e.push_back("#reset");
            }
            if (d->e_value.size()) {
               e.push_back("#cyan");
               e.push_back(d->e_value);
               e.push_back("#reset");
            }
         } else {
            if (d->e_key.size()) {
               if (d->e_key != d->a_key) e.push_back("#green");
               e.push_back(d->e_key);
               if (d->e_key != d->a_key) e.push_back("#reset");
               e.push_back(": ");
            }
            if (d->e_value.size()) {
               if (d->e_value != d->a_value) e.push_back("#green");
               e.push_back(d->e_value);
               if (d->e_value != d->a_value) e.push_back("#reset");
            }
         }
      } else {
         if (d->a_key.size()) e.push_back(occupy(d->a_key + ": "));
         if (d->a_value.size()) e.push_back(occupy(d->a_value));
      }

      if (d->a_type != t_absent) {
         if (d->e_type == t_absent) {  // only a-side exist
            const char* style = "#red,bold";
            if (d->perent && d->perent->a_type == t_object) {
               style = "#yellow";
            }
            if (d->a_key.size()) {
               a.push_back(style);
               a.push_back(d->a_key + ": ");
               a.push_back("#reset");
            }
            if (d->a_value.size()) {
               a.push_back(style);
               a.push_back(d->a_value);
               a.push_back("#reset");
            }
         } else {
            if (d->a_key.size()) {
               if (d->a_key != d->e_key) a.push_back("#red,bold");
               a.push_back(d->a_key);
               if (d->a_key != d->e_key) a.push_back("#reset");
               a.push_back(": ");
            }
            if (d->a_value.size()) {
               if (d->a_value != d->e_value) a.push_back("#red,bold");
               a.push_back(d->a_value);
               if (d->a_value != d->e_value) a.push_back("#reset");
            }
         }
      } else {
         if (d->e_key.size()) a.push_back(occupy(d->e_key + ": "));
         if (d->e_value.size()) a.push_back(occupy(d->e_value));
      }

      /* e/a type shoud be same */

      if (d->e_type == t_object && d->a_type == t_object) {
         e.push_back("{");
         a.push_back("{");
      }
      if (d->e_type == t_array && d->a_type == t_array) {
         e.push_back("[");
         a.push_back("[");
      }

      if ((d->e_type == t_object && d->a_type == t_object) || (d->e_type == t_array && d->a_type == t_array)) {
         for (size_t i = 0; i < d->child.size(); i++) {
            diff(d->child[i], e, a);

            bool e_not_last = false, a_not_last = false;
            for (size_t j = i + 1; j < d->child.size(); j++) {
               e_not_last = e_not_last || (d->child[j]->e_type != t_absent);
               a_not_last = a_not_last || (d->child[j]->a_type != t_absent);
            }
            if (e_not_last) e.push_back(",");
            if (a_not_last) a.push_back(",");
         }
         if (d->child.size()) {
            e.push_back("\n");
            e.push_back(indent(d->depth));
            a.push_back("\n");
            a.push_back(indent(d->depth));
         }
      }

      if (d->e_type == t_object && d->a_type == t_object) {
         e.push_back("}");
         a.push_back("}");
      }
      if (d->e_type == t_array && d->a_type == t_array) {
         e.push_back("]");
         a.push_back("]");
      }
   }

   typedef h2_vector<h2_string> Line;
   typedef h2_vector<Line> Lines;

   static void merge_line(h2_vector<h2_string>& list, Lines& lines) {
      Line line;
      for (auto& s : list) {
         if (s == "\n") {
            lines.push_back(line);
            line.clear();
            continue;
         }
         line.push_back(s);
      }
      lines.push_back(line);
      line.clear();
   }

   static int lines_most(Lines& lines) {
      int most = 0;
      for (auto& line : lines) {
         int curr = 0;
         for (auto& word : line)
            if (word[0] != '#') curr += word.length();
         most = std::max(most, curr);
      }
      return most;
   }

   static int line_wrap(Line& line, int columns) {
      int char_count = 0;
      for (auto& word : line)
         if (word[0] != '#') char_count += word.length();

      return H2_DIV_ROUND_UP(char_count, columns);  // num_of_line
   }

   static h2_string line_wrap(Line& line, int index, int columns, h2_string& current_style) {
      int s = 0, u = 0;
      h2_string wrap;
      for (auto& word : line)
         if (word[0] == '#') {
            if (index * columns <= s && s < (index + 1) * columns) {
               const char* style = h2cfg().style(word.c_str() + 1);
               wrap.append(style);
               current_style = style;
            }
         } else {
            for (auto& c : word) {
               if (index * columns <= s && s < (index + 1) * columns) {
                  wrap.append(1, c);
                  ++u;
               }
               ++s;
            }
         }

      wrap.append(columns - u, columns_char);
      return wrap;
   }

   static void print(Lines& e_lines, Lines& a_lines, int side_columns) {
      h2_string e_last_style, a_last_style;
      // assert(e_lines.size() == a_lines.size());
      for (size_t i = 0; i < std::max(e_lines.size(), a_lines.size()); ++i) {
         auto e_line = e_lines[i];
         auto a_line = a_lines[i];
         int e_wraps = line_wrap(e_line, side_columns - 2);
         int a_wraps = line_wrap(a_line, side_columns - 2);
         // assert(e_wraps == a_wraps);
         int K = std::max(e_wraps, a_wraps);
         for (int j = 0; j < K; ++j) {
            h2_string e_current_style, a_current_style;
            auto e_wrap = line_wrap(e_line, j, side_columns - 2, e_current_style);
            auto a_wrap = line_wrap(a_line, j, side_columns - 2, a_current_style);
            ::printf("%s%s %s%s%s│%s %s%s %s%s%s%s\n",
                     e_last_style.c_str(), e_wrap.c_str(), h2cfg().style("reset"),
                     h2cfg().style("dark gray"), j == K - 1 ? " " : "\\", h2cfg().style("reset"),
                     a_last_style.c_str(), a_wrap.c_str(), h2cfg().style("reset"),
                     h2cfg().style("dark gray"), j == K - 1 ? " " : "\\", h2cfg().style("reset"));

            e_last_style = e_current_style;
            a_last_style = a_current_style;
         }
      }
   }

   static void diff_print(const char* expect, const char* actual, int terminal_columns) {
      Node *e_node = parse(expect), *a_node = parse(actual);

      Dual* d = new Dual(0, nullptr);
      dual(e_node, a_node, d);

      frees(e_node);
      frees(a_node);

      h2_vector<h2_string> e_list, a_list;
      diff(d, e_list, a_list);
      frees(d);

      Lines e_lines, a_lines;
      merge_line(e_list, e_lines);
      merge_line(a_list, a_lines);

      int e_most = lines_most(e_lines), a_most = lines_most(a_lines);

      int fav_columns = std::max(std::max(e_most, a_most) + 3, 30);
      int side_columns = std::min(terminal_columns / 2 - 4, fav_columns);

      char t1[256], t2[256];
      ::printf("%s%s%s%s│%s%s%s%s\n",
               h2cfg().style("dark gray"),
               h2_center_string("expect", side_columns, t1),
               h2cfg().style("reset"),
               h2cfg().style("dark gray"), h2cfg().style("reset"),
               h2cfg().style("dark gray"),
               h2_center_string("actual", side_columns, t2),
               h2cfg().style("reset"));

      print(e_lines, a_lines, side_columns);
   }
};

static inline void h2_dohook_g();
static inline void h2_unhook_g();

struct h2_nm {
   /* clang-format off */
   static h2_nm& I() { static h2_nm __; return __; }
   /* clang-format on */

   unsigned long get(const char* name) const {
      auto it = symbols.find(name);
      return it != symbols.end() ? it->second : ULONG_MAX;
   }

   bool in_main(unsigned long addr) const { return main_addr == ULONG_MAX ? false : main_addr < addr && addr < main_addr + 64; }

   h2_nm() {
      char nm[256], line[1024], addr[32], type[32], name[1024];
      sprintf(nm, "nm %s", h2cfg().path);
      h2_with f(::popen(nm, "r"), ::pclose);
      if (f.f)
         while (::fgets(line, sizeof(line) - 1, f.f))
            if (3 == sscanf(line, "%s%s%s", addr, type, name))
               if (::tolower((int)type[0]) == 't' || ::tolower((int)type[0]) == 'w')
                  symbols.insert(std::make_pair(name + h2cfg().isMAC(), strtol(addr, nullptr, 16)));

      main_addr = get("main");
   }

   std::map<std::string, unsigned long> symbols;
   unsigned long main_addr;
};

struct h2_backtrace {
   int count, shift;
   void* array[100];

   h2_backtrace() : count(0), shift(0) {}

   h2_backtrace(int shift_) : shift(shift_) {
      h2_unhook_g();
      count = ::backtrace(array, sizeof(array) / sizeof(array[0]));
      h2_dohook_g();
   }

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(h2_backtrace& bt) {
      if (count != bt.count) return false;
      for (int i = 0; i < count; ++i)
         if (array[i] != bt.array[i])
            return false;
      return true;
   }

   bool has(void* func, int size) {
      for (int i = 0; i < count; ++i)
         if (func <= array[i] && (unsigned char*)array[i] < ((unsigned char*)func) + size)
            return true;
      return false;
   }

   void print() const {
      h2_unhook_g();
      char** backtraces = backtrace_symbols(array, count);
      for (int i = shift; i < count; ++i) {
         char *p = backtraces[i], module[256], mangled[256], demangled[256], addr2lined[512];
         unsigned long address = 0, offset = 0;
         if (extract(backtraces[i], module, mangled, &offset)) {
            if (strlen(mangled)) p = mangled;
            if (demangle(mangled, demangled, sizeof(demangled)) && strlen(demangled)) p = demangled;
            address = strlen(mangled) ? h2_nm::I().get(mangled) : 0;
            if (address != ULONG_MAX && h2_endswith_string(h2cfg().path, module))
               if (addr2line(address + offset, addr2lined, sizeof(addr2lined)))
                  p = addr2lined;
         }
         ::printf("   %d. %s\n", i - shift, p);

         if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_nm::I().in_main(address + offset))
            break;
      }
      free(backtraces);
      h2_dohook_g();
   }

   bool addr2line(unsigned long addr, char* output, size_t len) const {
      char t[256];
#if defined __APPLE__
      sprintf(t, "atos -o %s 0x%lx", h2cfg().path, addr);
#else
      sprintf(t, "addr2line -C -a -s -p -f -e %s -i %lx", h2cfg().path, addr);
#endif
      h2_with f(::popen(t, "r"), ::pclose);
      if (!f.f || !::fgets(output, len, f.f)) return false;
      for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
      return 0 < strlen(output);
   }

   bool extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long* offset) const {
      //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
      if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %lu", module, mangled, offset))
         return true;

      //Linux: with '-rdynamic' linker option
      //Linux: module_name(mangled_function_name+relative_offset_to_function)[absolute_address]
      //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
      if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%lx", module, mangled, offset))
         return true;

      mangled[0] = '\0';

      //Linux: Ubuntu without '-rdynamic' linker option
      //Linux: module_name(+relative_offset_to_function)[absolute_address]
      //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%lx", module, offset))
         return true;

      //Linux: Redhat/CentOS without '-rdynamic' linker option
      //Linux: module_name()[relative_offset_to_module]
      //Linux: `./a.out() [0x40b960]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^[][0x%lx", module, offset))
         return true;

      //Where?
      //Linux: module_name[relative_offset_to_module]
      //Linux: `./a.out[0x4060e7]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%lx", module, offset))
         return true;

      return false;
   }

   bool demangle(const char* mangled, char* demangled, size_t len) const {
      int status = 0;
      strcpy(demangled, mangled);
      abi::__cxa_demangle(mangled, demangled, &len, &status);
      return status == 0;
   }
};

#define h2_debug(...)                                                              \
   do {                                                                            \
      printf("%s %s : %d = %s\n", #__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__); \
      h2_backtrace bt(0);                                                          \
      bt.print();                                                                  \
   } while (0)

#define H2_FAIL_FOREACH(f, First)                                 \
   for (h2_fail* x_fail = First; x_fail; x_fail = x_fail->x_next) \
      for (h2_fail* f = x_fail; f; f = f->y_next)

#define H2_XPRINTF(format, value)            \
   do {                                      \
      char t[1024 * 8];                      \
      va_list args;                          \
      va_start(args, format);                \
      vsnprintf(t, sizeof(t), format, args); \
      va_end(args);                          \
      value = value + t;                     \
   } while (0)

struct h2_fail {
   h2_fail *x_next, *y_next;

   const char* file;
   int line;

   const char* func;
   int argi;

   h2_string _k;

   static constexpr const char* A9 = "1st\0002nd\0003rd\0004th\0005th\0006th\0007th\0008th\0009th";

   h2_fail(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1)
     : x_next(nullptr), y_next(nullptr), file(file_), line(line_), func(func_), argi(argi_) {}

   virtual ~h2_fail() {
      if (y_next) delete y_next;
      if (x_next) delete x_next;
   };

   void locate(const char* file_, int line_, const char* func_ = nullptr, int argi_ = -1) {
      file = file_, line = line_, func = func_, argi = argi_;
      if (y_next) y_next->locate(file_, line_, func_, argi_);
   }

   void kprintf(const char* format, ...) { H2_XPRINTF(format, _k); }

   virtual void print() { _k.size() && printf(" %s", _k.c_str()); }

   void print_locate() {
      if (func && strlen(func)) printf(", in %s(%s)", func, 0 <= argi && argi < 9 ? A9 + argi * 4 : "?");
      if (file && strlen(file) && 0 < line) printf(", at %s:%d", file, line);
      printf("\n");
   }

   virtual void print(FILE* fp) { fprintf(fp, "%s", _k.c_str()); }

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
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

struct h2_fail_normal : public h2_fail {
   h2_fail_normal(const char* file_ = nullptr, int line_ = 0, const char* func_ = nullptr, const char* format = "", ...)
     : h2_fail(file_, line_, func_) { H2_XPRINTF(format, _k); }

   void print() { h2_fail::print(), print_locate(); }
};

struct h2_fail_unexpect : public h2_fail {
   h2_string _e, _a, _h, _m, _t;

   h2_fail_unexpect(const char* file_ = nullptr, int line_ = 0) : h2_fail(file_, line_) {}

   void hprintf(const char* format, ...) { H2_XPRINTF(format, _h); }
   void eprintf(const char* format, ...) { H2_XPRINTF(format, _e); }
   void mprintf(const char* format, ...) { H2_XPRINTF(format, _m); }
   void aprintf(const char* format, ...) { H2_XPRINTF(format, _a); }
   void tprintf(const char* format, ...) { H2_XPRINTF(format, _t); }

   void print() {
      h2_fail::print(); /* nothing */
      printf(" %s%s%s%s %s %s%s%s%s",
             _h.c_str(),
             h2cfg().style("bold,red"), _a.c_str(), h2cfg().style("reset"),
             _m.c_str(),
             h2cfg().style("green"), _e.c_str(), h2cfg().style("reset"),
             _t.c_str());
      print_locate();
   }

   void print(FILE* fp) {
      h2_fail::print(fp);
      fprintf(fp, " %s %s %s %s %s ", _h.c_str(), _a.c_str(), _m.c_str(), _e.c_str(), _t.c_str());
   }
};

struct h2_fail_strcmp : public h2_fail {
   const h2_string e, a;
   const bool caseless;

   h2_fail_strcmp(const char* expect, const char* actual, bool caseless_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e(expect), a(actual), caseless(caseless_) { kprintf("String not %sequals", caseless_ ? "case-insensitive " : ""); }

   void print() {
      h2_fail::print(), print_locate();

      int columns = h2cfg().get_term_columns() - 12;
      int rows = H2_DIV_ROUND_UP(std::max(e.length(), a.length()), columns);

      for (int i = 0; i < rows; ++i) {
         char eline[1024], aline[1024], *ep = eline, *ap = aline;
         ep += sprintf(ep, "%sexpect%s>%s ", h2cfg().style("dark gray"), h2cfg().style("green"), h2cfg().style("reset"));
         ap += sprintf(ap, "%sactual%s> ", h2cfg().style("dark gray"), h2cfg().style("reset"));
         for (int j = 0; j < columns; ++j) {
            char _e = i * columns + j < (int)e.length() ? e[i * columns + j] : ' ';
            char _a = i * columns + j < (int)a.length() ? a[i * columns + j] : ' ';

            bool eq = caseless ? ::tolower(_e) == ::tolower(_a) : _e == _a;
            ep += sprintf(ep, "%s%c%s", eq ? "" : h2cfg().style("green"), _e, eq ? "" : h2cfg().style("reset"));
            ap += sprintf(ap, "%s%c%s", eq ? "" : h2cfg().style("red,bold"), _a, eq ? "" : h2cfg().style("reset"));
         }
         printf("%s\n%s\n", eline, aline);
      }
   }
};

struct h2_fail_memcmp : public h2_fail {
   h2_vector<unsigned char> e, a;
   const void* p;

   h2_fail_memcmp(const void* expect, const void* actual, int len, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e((unsigned char*)expect, ((unsigned char*)expect) + len), a((unsigned char*)actual, ((unsigned char*)actual) + len), p(actual) {
      kprintf("Memory %p binary %d bytes not equal", p, len);
   }

   void print() {
      h2_fail::print(), print_locate();

      printf("                     %sexpect%s                       %s│%s                       %sactual%s \n",
             h2cfg().style("dark gray"), h2cfg().style("reset"), h2cfg().style("dark gray"), h2cfg().style("reset"), h2cfg().style("dark gray"), h2cfg().style("reset"));

      int size = e.size();
      int rows = H2_DIV_ROUND_UP(size, 16);

      for (int i = 0; i < rows; ++i) {
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) printf("%s", h2cfg().style("green"));
            printf(j < 8 ? "%02X " : " %02X", e[i * 16 + j]);
            printf("%s", h2cfg().style("reset"));
         }
         printf("  %s│%s  ", h2cfg().style("dark gray"), h2cfg().style("reset"));
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) printf("%s", h2cfg().style("bold,red"));
            printf(j < 8 ? "%02X " : " %02X", a[i * 16 + j]);
            printf("%s", h2cfg().style("reset"));
         }
         printf("\n");
      }
   }
};

struct h2_fail_memoverflow : public h2_fail {
   const unsigned char* ptr;
   const int offset;
   const unsigned char* magic;
   const h2_vector<unsigned char> spot;
   const h2_backtrace bt0, bt1;

   h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), ptr((unsigned char*)ptr_), offset(offset_), magic(magic_), spot(((unsigned char*)ptr_) + offset_, ((unsigned char*)ptr_) + offset_ + size), bt0(bt0_), bt1(bt1_) {
      kprintf(" Memory overflow malloc %p %+d (%p) ", ptr, offset, ptr + offset);
   }

   void print() {
      h2_fail::print();

      for (size_t i = 0; i < spot.size(); ++i)
         printf("%s%02X %s", magic[i] == spot[i] ? h2cfg().style("green") : h2cfg().style("bold,red"), spot[i], h2cfg().style("reset"));

      print_locate();
      if (0 < bt1.count) printf("  %p trampled at backtrace:\n", ptr + offset), bt1.print();
      if (0 < bt0.count) printf("  which allocated at backtrace:\n"), bt0.print();
   }
};

struct h2_fail_memleak : public h2_fail {
   const char* where;
   struct A {
      void *ptr, *ptr2;
      int size, size2, bytes, times;
      h2_backtrace bt;
      A(void* ptr_, int size_, h2_backtrace& bt_) : ptr(ptr_), ptr2(nullptr), size(size_), size2(0), bytes(size_), times(1), bt(bt_) {}
   };
   h2_vector<A> leaks;
   long long bytes, times, places;

   h2_fail_memleak(const char* file_ = nullptr, int line_ = 0, const char* where_ = "")
     : h2_fail(file_, line_), where(where_), bytes(0), times(0), places(0) {}

   void add(void* ptr, int size, h2_backtrace& bt) {
      bytes += size;
      times += 1;
      for (auto c : leaks)
         if (c.bt == bt) {
            c.ptr2 = ptr;
            c.size2 = size;
            c.bytes += size;
            c.times += 1;
            return;
         }
      places += 1;
      leaks.push_back(A(ptr, size, bt));
   }

   void print() {
      char t1[64] = "", t2[64] = "";
      if (1 < places) sprintf(t1, "%lld places ", places);
      if (1 < times) sprintf(t2, "%lld times ", times);

      kprintf("Memory Leaked %s%s%lld bytes in %s totally", t1, t2, bytes, where);
      h2_fail::print(), print_locate();
      for (auto c : leaks) {
         char t3[64] = " ", t4[64] = "", t5[64] = "";
         if (1 < c.times) sprintf(t3, ", %p ... ", c.ptr2);
         if (1 < c.times) sprintf(t4, "%d times ", c.times);
         if (1 < c.times) sprintf(t5, " (%d, %d ...)", c.size, c.size2);
         printf("  %p%sLeaked %s%d bytes%s, at backtrace\n", c.ptr, t3, t4, c.bytes, t5);
         c.bt.print();
      }
   }
};

struct h2_fail_doublefree : public h2_fail {
   const h2_backtrace bt0, bt1;

   h2_fail_doublefree(void* ptr_, h2_backtrace& bt0_, h2_backtrace bt1_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), bt0(bt0_), bt1(bt1_) { kprintf("%p double freed", ptr_); }

   void print() {
      h2_fail::print(), printf(" at backtrace:\n");
      bt1.print();
      if (0 < bt0.count) printf("  which allocated at backtrace:\n"), bt0.print();
   }
};

struct h2_fail_json : public h2_fail {
   const h2_string e, a;

   h2_fail_json(const char* k, const char* expect, const char* actual, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(file_, line_), e(expect), a(actual) { _k = k; }

   void print() {
      h2_fail::print(), print_locate();

      int terminal_columns = h2cfg().get_term_columns();
      if (terminal_columns < 10) terminal_columns = 80;
      h2_json::diff_print(e.c_str(), a.c_str(), terminal_columns);
   }
};

struct h2_fail_instantiate : public h2_fail {
   const char *action_type, *return_type, *class_type, *method_name, *return_args;
   const int why;

   h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_, const char* file_, int line_)
     : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why(why_) {
      if (why == 1)
         kprintf("Instantiate 'class %s' is a abstract class", class_type);
      else if (why == 2)
         kprintf("Instantiate 'class %s' don't know initialize arguments", class_type);
   }

   void print() {
      h2_fail::print(), print_locate();

      printf("You may take following solutions to fix it: \n");
      if (why == 1) {
         printf("1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s%s, Derived_%s(...)%s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                h2cfg().style("bold,yellow"),
                class_type,
                h2cfg().style("reset"));
      } else if (why == 2) {
         printf("1. Define default constructor in class %s, or \n", class_type);
         printf("2. Add parameterized construction in %s(%s%s%s, %s, %s%s, %s(...)%s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                h2cfg().style("bold,yellow"),
                class_type,
                h2cfg().style("reset"));
      }
   }
};
// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_thunk {
#if defined __x86_64__
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined __i386__
   unsigned char saved_code[sizeof(void*) + 1];
#endif

   bool save(void* befp) {
      static uintptr_t pagesize = (uintptr_t)sysconf(_SC_PAGE_SIZE);
      uintptr_t start = reinterpret_cast<uintptr_t>(befp);
      uintptr_t pagestart = start & (~(pagesize - 1));

      if (mprotect(reinterpret_cast<void*>(pagestart), H2_ALIGN_UP(start + sizeof(saved_code) - pagestart, pagesize), PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         printf("STUB failed %s\n", strerror(errno));
         return false;
      }

      memcpy(saved_code, befp, sizeof(saved_code));
      return true;
   }

   void set(void* befp, void* tofp) {
      unsigned char* I = reinterpret_cast<unsigned char*>(befp);
      ptrdiff_t delta = (unsigned char*)tofp - (unsigned char*)befp;

#if defined(__i386__) || defined(__x86_64__)
      //x86 __asm("jmp $tofp") : 0xE9 {offset=tofp-befp-5}
      //x86 __asm("movl $tofp, %eax; jmpl %eax") : 0xB8 {tofp} 0xFF 0xE0
      //x86_64 __asm("movq $tofp, %rax; jmpq %rax") : 0x48 0xB8 {tofp} 0xFF 0xE0
      if (delta < INT_MIN || INT_MAX < delta) {
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &tofp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {
         *I++ = 0xE9;
         *(int32_t*)I = delta - 5;
      }
#endif
   }

   void* reset(void* befp) { return memcpy(befp, saved_code, sizeof(saved_code)); }
};

struct h2_stub : protected h2_thunk {
   h2_list x;
   void* befp;
   const char* file;
   int line;

   h2_stub(void* befp_, const char* file_ = nullptr, int line_ = 0) : befp(befp_), file(file_), line(line_) {
      if (!save(befp_)) befp = nullptr;
   }

   void replace(void* tofp) { set(befp, tofp); }

   void restore() { befp&& reset(befp); }

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
};
#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" {
extern malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
}
#endif

static unsigned char forbidden_zone[] = {0xbe, 0xaf, 0xca, 0xfe, 0xc0, 0xde, 0xfa, 0xce};

struct h2_piece {
   void *ptr, *page;
   h2_list x;
   int size, pagesize, pagecount;
   unsigned escape : 1, freed : 1;
   h2_backtrace bt;

   h2_piece(void* ptr_, void* page_, int size_, int pagesize_, int pagecount_, h2_backtrace& bt_)
     : ptr(ptr_), page(page_), size(size_), pagesize(pagesize_), pagecount(pagecount_), escape(0), freed(0), bt(bt_) {}

   bool in_range(const void* p) {
      const unsigned char* p0 = (const unsigned char*)page;
      const unsigned char* p2 = p0 + pagesize * (pagecount + 1);
      return p0 <= (const unsigned char*)p && (const unsigned char*)p < p2;
   }

   static h2_piece* allocate(int size, int alignment, h2_backtrace& bt) {
      auto pagesize = sysconf(_SC_PAGE_SIZE);
      int pagecount = H2_DIV_ROUND_UP(size + (alignment ? alignment : 8) + sizeof(forbidden_zone), pagesize);
      unsigned char* p = (unsigned char*)mmap(nullptr, pagesize * (pagecount + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (p == MAP_FAILED) return nullptr;
      unsigned char* ptr = p + pagesize * pagecount - size;
      ptr = (unsigned char*)H2_ALIGN_DOWN((intptr_t)ptr, alignment ? alignment : 8);

      h2_piece* m = new (h2_raw::malloc(sizeof(h2_piece))) h2_piece((void*)ptr, p, size, pagesize, pagecount, bt);

      unsigned char* p1 = ptr - sizeof(forbidden_zone);
      memcpy(p1, forbidden_zone, sizeof(forbidden_zone));

      unsigned char* p2 = p1 - sizeof(void*);
      *(void**)p2 = (void*)m;

      memcpy(ptr + size, forbidden_zone, sizeof(forbidden_zone));

      if (mprotect((void*)(p + pagesize * pagecount), pagesize, PROT_READ) != 0)
         printf("mprotect failed %s\n", strerror(errno));

      return m;
   }

   static h2_fail* prefree(h2_piece* m) {
      if (m->freed++) return new h2_fail_doublefree(m->ptr, m->bt, h2_backtrace(h2cfg().isMAC() ? 5 : 4));

      h2_fail* fail = nullptr;

      if (memcmp((unsigned char*)m->ptr + m->size, forbidden_zone, sizeof(forbidden_zone)))
         h2_append_x_fail(fail, new h2_fail_memoverflow(m->ptr, m->size, forbidden_zone, sizeof(forbidden_zone), m->bt, h2_backtrace()));

      if (memcmp((unsigned char*)m->ptr - sizeof(forbidden_zone), forbidden_zone, sizeof(forbidden_zone)))
         h2_append_x_fail(fail, new h2_fail_memoverflow(m->ptr, -(int)sizeof(forbidden_zone), forbidden_zone, sizeof(forbidden_zone), m->bt, h2_backtrace()));

      if (mprotect(m->page, m->pagesize * (m->pagecount + 1), PROT_READ) != 0)
         printf("mprotect failed %s\n", strerror(errno));

      return fail;
   }

   static void release(h2_piece* m) {
      munmap(m->page, m->pagesize * (m->pagecount + 1));
      h2_raw::free(m);
   }
};

struct h2_block {
   h2_list x;

   h2_list using_list, freed_list;

   const char* file;
   int line;
   const char* where;
   long long limited;
   const char* fill;

   h2_block(const char* file_, int line_, const char* where_, long long limited_, const char* fill_)
     : file(file_), line(line_), where(where_), limited(limited_), fill(fill_) {}

   h2_fail* leak_check() {
      h2_fail_memleak* fail = nullptr;

      if (!using_list.empty()) {
         fail = new h2_fail_memleak(file, line, where);
         h2_list_for_each_entry(m, &using_list, h2_piece, x) fail->add(m->ptr, m->size, m->bt);
      }

      h2_list_for_each_entry(m, &freed_list, h2_piece, x) m->x.out(), h2_piece::release(m);

      return fail;
   }

   h2_piece* newm(int size, int alignment, const char* fill_, h2_backtrace& bt) {
      if (limited < size) return nullptr;

      h2_piece* m = h2_piece::allocate(size, alignment, bt);

      if (fill_ ? fill_ : (fill_ = fill))
         for (int i = 0, j = 0, l = strlen(fill_); i < size; ++i, ++j)
            ((char*)m->ptr)[i] = fill_[j % (l ? l : 1)];

      using_list.push(&m->x);
      return m;
   }

   h2_piece* getm(const void* ptr) {
      h2_list_for_each_entry(m, &using_list, h2_piece, x) if (m->ptr == ptr) return m;
      h2_list_for_each_entry(m, &freed_list, h2_piece, x) if (m->ptr == ptr) return m;
      return nullptr;
   }

   h2_fail* relm(h2_piece* m) {
      limited += m->size;

      m->x.out();
      freed_list.push(&m->x);
      return h2_piece::prefree(m);
   }

   h2_piece* whom(const void* addr) {
      h2_list_for_each_entry(m, &using_list, h2_piece, x) if (m->in_range(addr)) return m;
      h2_list_for_each_entry(m, &freed_list, h2_piece, x) if (m->in_range(addr)) return m;
      return nullptr;
   }
};

class h2_stack {
 private:
   h2_list blocks;

   bool escape(h2_backtrace& bt) {
      static struct {
         unsigned char* base;
         int size;
      } exclude_functions[] = {
        {(unsigned char*)printf, 300},
        {(unsigned char*)sprintf, 300},
        {(unsigned char*)vsnprintf, 300},
        {(unsigned char*)sscanf, 300},
        {(unsigned char*)localtime, 300}};

      for (size_t i = 0; i < sizeof(exclude_functions) / sizeof(exclude_functions[0]); ++i)
         if (bt.has(exclude_functions[i].base, exclude_functions[i].size))
            return true;

      return false;
   }

 public:
   bool push(const char* file, int line, const char* where, long long limited = 0x7fffffffffffLL, const char* fill = nullptr) {
      h2_block* b = new (h2_raw::malloc(sizeof(h2_block))) h2_block(file, line, where, limited, fill);
      blocks.push(&b->x);
      return true;
   }

   h2_fail* pop() {
      h2_block* b = h2_list_pop_entry(&blocks, h2_block, x);
      h2_fail* fail = b->leak_check();
      h2_raw::free(b);
      return fail;
   }

   h2_piece* newm(size_t size, size_t alignment, const char* fill) {
      h2_backtrace bt(h2cfg().isMAC() ? 6 : 2);
      h2_block* b = escape(bt) ? h2_list_bottom_entry(&blocks, h2_block, x) : h2_list_top_entry(&blocks, h2_block, x);
      return b ? b->newm(size, alignment, fill, bt) : nullptr;
   }

   h2_piece* getm(const void* ptr) {
      h2_list_for_each_entry(b, &blocks, h2_block, x) {
         h2_piece* m = b->getm(ptr);
         if (m) return m;
      }
      return nullptr;
   }

   h2_fail* relm(void* ptr) {
      h2_list_for_each_entry(b, &blocks, h2_block, x) {
         h2_piece* m = b->getm(ptr);
         if (m) return b->relm(m);
      }

      h2_debug("Warning: free not found!");
      return nullptr;
   }

   h2_piece* whom(const void* addr) {
      h2_list_for_each_entry(b, &blocks, h2_block, x) {
         h2_piece* m = b->whom(addr);
         if (m) return m;
      }
      return nullptr;
   }

   /* clang-format off */
   static h2_stack& G() { static h2_stack __; return __; }
   /* clang-format on */

   struct T {
      int count;

      T(const char* file, int line, long long limited = 0x7fffffffffffLL, const char* fill = nullptr)
        : count(0) { h2_stack::G().push(file, line, "block", limited, fill); }
      ~T() { h2_fail_g(h2_stack::G().pop()); }

      operator bool() { return 0 == count++; }
   };
};

struct h2_hook {
   static void free(void* ptr) {
      if (ptr) h2_fail_g(h2_stack::G().relm(ptr)); /* overflow or double free */
   }

   static void* malloc(size_t size) {
      h2_piece* m = h2_stack::G().newm(size, 0, nullptr);
      return m ? m->ptr : nullptr;
   }

   static void* calloc(size_t count, size_t size) {
      h2_piece* m = h2_stack::G().newm(size * count, 0, "\0");
      return m ? m->ptr : nullptr;
   }

   static void* realloc(void* ptr, size_t size) {
      if (size == 0) {
         if (ptr) h2_fail_g(h2_stack::G().relm(ptr));
         return nullptr;
      }

      h2_piece* old_m = h2_stack::G().getm(ptr);
      if (!old_m) return nullptr;

      h2_piece* new_m = h2_stack::G().newm(size, 0, nullptr);
      if (!new_m) return nullptr;

      memcpy(new_m->ptr, old_m->ptr, old_m->size);
      h2_fail_g(h2_stack::G().relm(ptr));

      return new_m->ptr;
   }

   static int posix_memalign(void** memptr, size_t alignment, size_t size) {
      h2_piece* m = h2_stack::G().newm(size, alignment, nullptr);
      return m ? (*memptr = m->ptr, 0) : ENOMEM;
   }

   static void* aligned_alloc(size_t alignment, size_t size) {
      h2_piece* m = h2_stack::G().newm(size, alignment, nullptr);
      return m ? m->ptr : nullptr;
   }

#if defined __GLIBC__

   static void free_hook(void* __ptr, const void* caller) { free(__ptr); }
   static void* malloc_hook(size_t __size, const void* caller) { return malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return aligned_alloc(__alignment, __size); }

   void (*sys__free_hook)(void*, const void*);
   void* (*sys__malloc_hook)(size_t, const void*);
   void* (*sys__realloc_hook)(void*, size_t, const void*);
   void* (*sys__memalign_hook)(size_t, size_t, const void*);

#elif defined __APPLE__
   static size_t mz_size(malloc_zone_t* zone, const void* ptr) {
      h2_piece* m = h2_stack::G().getm(ptr);
      return m ? m->size : 0;
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

   static malloc_zone_t* get_default_zone() {
      malloc_zone_t** zones = nullptr;
      unsigned int num_zones = 0;

      if (KERN_SUCCESS != malloc_get_all_zones(0, nullptr, (vm_address_t**)&zones, &num_zones)) num_zones = 0;
      if (num_zones) return zones[0];
      return malloc_default_zone();
   }

   malloc_introspection_t mi;
   malloc_zone_t mz;

#else

#endif

   h2_stub free_stub;
   h2_stub malloc_stub;
   h2_stub calloc_stub;
   h2_stub realloc_stub;
   h2_stub posix_memalign_stub;

   h2_hook()
     : free_stub((void*)::free), malloc_stub((void*)::malloc), calloc_stub((void*)::calloc), realloc_stub((void*)::realloc), posix_memalign_stub((void*)::posix_memalign) {
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

#else
#endif
   }

   /* clang-format off */
   static h2_hook& I() { static h2_hook __; return __; }
   /* clang-format on */

   void dohook() {
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

#else
      free_stub.replace((void*)hook::free);
      malloc_stub.replace((void*)hook::malloc);
      calloc_stub.replace((void*)hook::calloc);
      realloc_stub.replace((void*)hook::realloc);
      posix_memalign_stub.replace((void*)hook::posix_memalign);
#endif
   }

   void unhook() {
#if defined __GLIBC__
      __free_hook = sys__free_hook;
      __malloc_hook = sys__malloc_hook;
      __realloc_hook = sys__realloc_hook;
      __memalign_hook = sys__memalign_hook;

#elif defined __APPLE__

      malloc_zone_unregister(&mz);

#else
      free_stub.restore();
      malloc_stub.restore();
      calloc_stub.restore();
      realloc_stub.restore();
      posix_memalign_stub.restore();
#endif
   }

   static void overflow_handler(int sig, siginfo_t* si, void* unused) {
      h2_piece* m = h2_stack::G().whom(si->si_addr);
      if (m) h2_fail_g(new h2_fail_memoverflow(m->ptr, (intptr_t)si->si_addr - (intptr_t)m->ptr, nullptr, 0, m->bt, h2_backtrace(h2cfg().isMAC() ? 5 : 4)));
      h2_debug();
      exit(1);
   }
};

static inline void h2_sihook_g() {
   if (!h2cfg().memory_check) return;
   struct sigaction act;
   act.sa_sigaction = h2_hook::overflow_handler;
   sigemptyset(&act.sa_mask);
   act.sa_flags = SA_SIGINFO;
   if (sigaction(SIGSEGV, &act, nullptr) == -1)
      perror("Register SIGSEGV handler failed");
   if (sigaction(SIGBUS, &act, nullptr) == -1)
      perror("Register SIGBUS handler failed");
}

static inline void h2_dohook_g() {
   if (h2cfg().memory_check) h2_hook::I().dohook();
}
static inline void h2_unhook_g() {
   if (h2cfg().memory_check) h2_hook::I().unhook();
}

// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h

/* clang-format off */

template <typename...> using h2_void_t = void;

template <typename T, int I> struct h2_constructible_error {
   static T* O(void* m) { return static_cast<T*>(m = (void*)I); }
};

template <typename, typename> struct h2_constructible0_impl : std::false_type {};
template <typename T>
struct h2_constructible0_impl<h2_void_t<decltype(T())>, T> : std::true_type {
   static T* O(void* m) { return new (m) T(); }
}; /* placement new */

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
   typedef Return (Class::*mfp_type)(Args...);
   typedef union {
      mfp_type f;
      void* p;
      intptr_t v;
   } cast_type;

   static inline bool is_virtual(cast_type& u) {
      return (u.v & 1) && (u.v - 1) % sizeof(void*) == 0 && (u.v - 1) / sizeof(void*) < 1000;
   }

   static inline void* get_vmfp(cast_type& u, Class* obj) {
      void** vtable = *(void***)obj;
      return vtable[(u.v - 1) / sizeof(void*)];
   }

   static void* A(mfp_type f, const char* action_type, const char* return_type, const char* class_type, const char* method_name, const char* return_args, const char* file, int line) {
      cast_type u{f};

      if (!is_virtual(u)) return u.p;

      Class* o = h2_constructible<Class>::O(alloca(sizeof(Class)));
      if (0 == (intptr_t)o || 1 == (intptr_t)o || 2 == (intptr_t)o)
         h2_fail_g(new h2_fail_instantiate(action_type, return_type, class_type, method_name, return_args, (int)(intptr_t)o, file, line));
      return get_vmfp(u, o);
   }

   template <typename Derived>
   static void* A(mfp_type f, Derived obj) {
      cast_type u{f};

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
struct h2_matcher : public h2_matcher_base<T> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl, const int placeholder) : h2_matcher_base<T>(impl, placeholder) {}
   h2_matcher(T value);
};

template <>
struct h2_matcher<const char*> : public h2_matcher_base<const char*> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const char* const&>* impl, const int placeholder) : h2_matcher_base<const char*>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<const std::string&> : public h2_matcher_base<const std::string&> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* impl, const int placeholder) : h2_matcher_base<const std::string&>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<std::string> : public h2_matcher_base<std::string> {
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
   struct internal_impl : public h2_matcher_impl<T> {
      const Matches m;
      explicit internal_impl(const Matches& matches_) : m(matches_) {}
      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override { return m.matches(a, caseless, dont); }
      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };
};

template <typename A, typename E>
inline h2_fail* h2_desc(const A& a, const E& e, bool dont, const char* op) {
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

struct h2_string_eq_matches {
   const h2_string e;
   h2_string_eq_matches(const h2_string& _e) : e(_e) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.equals(e, caseless) == !dont) return nullptr;
      if (dont) {
         h2_fail_unexpect* fail = new h2_fail_unexpect();
         fail->eprintf("\"%s\"", h2_acronym_string(e.c_str()));
         fail->aprintf("\"%s\"", h2_acronym_string(a.c_str()));
         fail->mprintf("should not %sequal to", caseless ? "caseless " : "");
         return fail;
      } else {
         if (35 < e.length() && 35 < a.length()) return new h2_fail_strcmp(e.c_str(), a.c_str(), caseless);
         h2_fail_unexpect* fail = new h2_fail_unexpect();
         fail->eprintf("\"%s\"", e.c_str());
         fail->aprintf("\"%s\"", a.c_str());
         fail->mprintf("not %sequal to", caseless ? "caseless " : "");
         return fail;
      }
   }
};

struct h2_float_eq_matches {
   const long double e;
   explicit h2_float_eq_matches(const long double _e) : e(_e) {}

   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const {
      // the machine epsilon has to be scaled to the magnitude of the values used
      // and multiplied by the desired precision in ULPs (units in the last place)
      // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
      //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
      bool result = std::fabs(a - e) < 0.00001;
      if (result == !dont) return nullptr;
      return h2_desc(a, e, dont, "equal to");
   }
};

template <typename E>
struct h2_eq_matches {
   const E e;
   explicit h2_eq_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a == e) == !dont) return nullptr;
      return h2_desc(a, e, dont, "equal to");
   }
};

template <>
struct h2_eq_matches<char*> : public h2_string_eq_matches {
   explicit h2_eq_matches(const char* e) : h2_string_eq_matches(h2_string(e)) {}
};
template <>
struct h2_eq_matches<const char*> : public h2_string_eq_matches {
   explicit h2_eq_matches(const char* e) : h2_string_eq_matches(h2_string(e)) {}
};
template <>
struct h2_eq_matches<h2_string> : public h2_string_eq_matches {
   explicit h2_eq_matches(const h2_string e) : h2_string_eq_matches(e) {}
};
template <>
struct h2_eq_matches<std::string> : public h2_string_eq_matches {
   explicit h2_eq_matches(const std::string e) : h2_string_eq_matches(h2_string(e)) {}
};

template <>
struct h2_eq_matches<float> : public h2_float_eq_matches {
   explicit h2_eq_matches(const float e) : h2_float_eq_matches(e) {}
};
template <>
struct h2_eq_matches<double> : public h2_float_eq_matches {
   explicit h2_eq_matches(const double e) : h2_float_eq_matches(e) {}
};
template <>
struct h2_eq_matches<long double> : public h2_float_eq_matches {
   explicit h2_eq_matches(const long double e) : h2_float_eq_matches(e) {}
};

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = h2_polymorphic_matcher<h2_eq_matches<T>>(h2_eq_matches<T>(value)); }

inline h2_matcher<const char*>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<const char*>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }

template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from) { return do_cast(from, std::is_convertible<M, h2_matcher<T>>{}, std::is_convertible<M, T>{}); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type) { return h2_polymorphic_matcher<h2_eq_matches<M>>(h2_eq_matches<M>(from)); }
};

template <typename T, typename U>
struct h2_matcher_cast_impl<T, h2_matcher<U>> {
   static h2_matcher<T> cast(const h2_matcher<U>& from) { return h2_matcher<T>(new internal_impl(from)); }

   struct internal_impl : public h2_matcher_impl<T> {
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

      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };
};

template <typename T>
struct h2_matcher_cast_impl<T, h2_matcher<T>> {
   static h2_matcher<T> cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> h2_matcher_cast(const M& from) { return h2_matcher_cast_impl<T, M>::cast(from); }

struct h2_any_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
};

struct h2_null_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((nullptr == reinterpret_cast<const void*>(a)) == !dont) return nullptr;
      if (dont)
         return new h2_fail_normal(nullptr, 0, nullptr, "shoud not be null");
      else
         return new h2_fail_normal(nullptr, 0, nullptr, "is not null %p", reinterpret_cast<const void*>(a));
   }
};

template <typename E>
struct h2_ge_matches {
   const E e;
   explicit h2_ge_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a >= e) == !dont) return nullptr;
      return h2_desc(a, e, dont, ">=");
   }
};

template <typename E>
struct h2_gt_matches {
   const E e;
   explicit h2_gt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a > e) == !dont) return nullptr;
      return h2_desc(a, e, dont, ">");
   }
};

template <typename E>
struct h2_le_matches {
   const E e;
   explicit h2_le_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a <= e) == !dont) return nullptr;
      return h2_desc(a, e, dont, "<=");
   }
};

template <typename E>
struct h2_lt_matches {
   const E e;
   explicit h2_lt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a < e) == !dont) return nullptr;
      return h2_desc(a, e, dont, "<");
   }
};

struct h2_me_matches {
   const void* e;
   const int size;
   explicit h2_me_matches(const void* _e, const int _size) : e(_e), size(_size) {}

   h2_fail* matches(const void* a, bool caseless = false, bool dont = false) const {
      int sz = size ? size : strlen((const char*)e);
      if ((memcmp(e, a, sz) == 0) == !dont) return nullptr;
      return new h2_fail_memcmp(e, a, sz);
   }
};

template <typename Matcher>
struct h2_pe_matches {
   const Matcher m;
   explicit h2_pe_matches(Matcher matcher_) : m(matcher_) {}

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

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      bool result;
      try {
         std::regex re(e);
         result = std::regex_match(a, re);
      }
      catch (const std::regex_error&) {
         result = false;
      }
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud matches Regex");
      else
         fail->mprintf("not matches Regex");

      return fail;
   }
};

struct h2_wildcard_matches {
   const h2_string e;
   explicit h2_wildcard_matches(const h2_string& _e) : e(_e) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (h2_wildcard_match(e.c_str(), a.c_str()) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud matches Wildcard");
      else
         fail->mprintf("not matches Wildcard");

      return fail;
   }
};

struct h2_contains_matches {
   const h2_string substring;
   explicit h2_contains_matches(const h2_string& substring_) : substring(substring_) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.contains(substring, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", substring.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %shas substr", caseless ? "caseless " : "");
      else
         fail->mprintf("not %shas substr", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_startswith_matches {
   const h2_string prefix_string;
   explicit h2_startswith_matches(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", prefix_string.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %sstarts with", caseless ? "caseless " : "");
      else
         fail->mprintf("not %sstarts with", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_endswith_matches {
   const h2_string suffix_string;
   explicit h2_endswith_matches(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", suffix_string.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %sends with", caseless ? "caseless " : "");
      else
         fail->mprintf("not %sends with", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_je_matches {
   const h2_string e;
   explicit h2_je_matches(const h2_string& _e) : e(_e) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if ((h2_json::match(e.c_str(), a.c_str())) == !dont) return nullptr;
      if (dont)
         return new h2_fail_json("JSON should not equals", e.c_str(), a.c_str());
      else
         return new h2_fail_json("JSON not equals", e.c_str(), a.c_str());
   }
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      return m.matches(a, true, dont);
   }
};

template <typename Matcher>
struct h2_not_matches {
   const Matcher m;
   explicit h2_not_matches(Matcher matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      return h2_matcher_cast<A>(m).matches(a, caseless, !dont);
   }
};

#define H2_MATCHER_T2V(t_matchers)                                                                          \
   template <typename T, size_t I>                                                                          \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), \
                                h2_vector<h2_matcher<T>> v_matchers = {}) const {                           \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                    \
      return t2v<T>(std::integral_constant<size_t, I + 1>(), v_matchers);                                   \
   }                                                                                                        \
   template <typename T>                                                                                    \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, sizeof...(Matchers)>,                        \
                                h2_vector<h2_matcher<T>> v_matchers = {}) const { return v_matchers; }

template <typename... Matchers>
struct h2_allof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<A, 0>();

      h2_fail* fail = nullptr;
      for (auto& m : v_matchers)
         h2_append_y_fail(fail, m.matches(a, caseless, false));

      if (!fail == !dont) return nullptr;

      if (dont)
         return new h2_fail_normal(nullptr, 0, nullptr, "should not matches any of matcher");
      else
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
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, caseless, false);
         if (!fail) s++;
         if (fail) delete fail;
      }

      if ((0 < s) == !dont) return nullptr;

      h2_ostringstream osa;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("\"%s\"", osa.str().c_str());
      if (dont)
         fail->mprintf("should matches none of matcher");
      else
         fail->mprintf("not matches any of matcher");
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

      h2_fail* fail = nullptr;
      for (auto& m : v_matchers)
         h2_append_y_fail(fail, m.matches(a, caseless, true));

      if (!fail == !dont) return nullptr;

      if (dont)
         return new h2_fail_normal(nullptr, 0, nullptr, "should matches any of matcher");
      else
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
      for (size_t i = 0; i < v_matchers.size(); ++i)
         h2_append_y_fail(fail, v_matchers[i].matches(a[i], caseless, dont));

      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

const h2_polymorphic_matcher<h2_any_matches> _{h2_any_matches()};

inline h2_polymorphic_matcher<h2_any_matches> Any() {
   return h2_polymorphic_matcher<h2_any_matches>(h2_any_matches());
}
inline h2_polymorphic_matcher<h2_null_matches> Null() {
   return h2_polymorphic_matcher<h2_null_matches>(h2_null_matches());
}
template <typename E>
inline h2_polymorphic_matcher<h2_eq_matches<E>> Eq(const E expect) {
   return h2_polymorphic_matcher<h2_eq_matches<E>>(h2_eq_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_ge_matches<E>> Ge(const E expect) {
   return h2_polymorphic_matcher<h2_ge_matches<E>>(h2_ge_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_gt_matches<E>> Gt(const E expect) {
   return h2_polymorphic_matcher<h2_gt_matches<E>>(h2_gt_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_le_matches<E>> Le(const E expect) {
   return h2_polymorphic_matcher<h2_le_matches<E>>(h2_le_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_lt_matches<E>> Lt(const E expect) {
   return h2_polymorphic_matcher<h2_lt_matches<E>>(h2_lt_matches<E>(expect));
}
inline h2_polymorphic_matcher<h2_me_matches> Me(const void* buf, const int size = 0) {
   return h2_polymorphic_matcher<h2_me_matches>(h2_me_matches(buf, size));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_pe_matches<Matcher>> Pe(Matcher expect) {
   return h2_polymorphic_matcher<h2_pe_matches<Matcher>>(h2_pe_matches<Matcher>(expect));
}
inline h2_polymorphic_matcher<h2_regex_matches> Re(const h2_string& regex_pattern) {
   return h2_polymorphic_matcher<h2_regex_matches>(h2_regex_matches(regex_pattern));
}
inline h2_polymorphic_matcher<h2_wildcard_matches> We(const h2_string& wildcard_pattern) {
   return h2_polymorphic_matcher<h2_wildcard_matches>(h2_wildcard_matches(wildcard_pattern));
}
inline h2_polymorphic_matcher<h2_contains_matches> Contains(const h2_string& substring) {
   return h2_polymorphic_matcher<h2_contains_matches>(h2_contains_matches(substring));
}
inline h2_polymorphic_matcher<h2_startswith_matches> StartsWith(const h2_string& prefix_string) {
   return h2_polymorphic_matcher<h2_startswith_matches>(h2_startswith_matches(prefix_string));
}
inline h2_polymorphic_matcher<h2_endswith_matches> EndsWith(const h2_string& suffix_string) {
   return h2_polymorphic_matcher<h2_endswith_matches>(h2_endswith_matches(suffix_string));
}
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(h2_matcher<h2_string> expect) {
   return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(expect));
}
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(h2_string expect) {
   return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(expect));
}
inline h2_polymorphic_matcher<h2_je_matches> Je(const h2_string& expect) {
   return h2_polymorphic_matcher<h2_je_matches>(h2_je_matches(expect));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher expect) {
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(expect));
}
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

struct h2_callexp {
   int call;
   int least, most;

   h2_callexp(int _least, int _most) : call(0), least(_least), most(_most) {}

   void operator++() { call += 1; }

   bool is_shortage(/*不够*/) const { return call < least; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_overfill(/*过多*/) const { return most < call; }

   h2_fail* check() {
      if (is_satisfied() || is_saturated()) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("%s", actual());
      fail->mprintf("called but expect");
      fail->eprintf("%s", expect());
      return fail;
   }

   const char* actual() {
      static char st[64];
      if (call > 0)
         sprintf(st, "%d times", call);
      else
         sprintf(st, "never");
      return st;
   }

   const char* expect() {
      static char st[128];
      if (least == 0)
         if (most == 0)
            sprintf(st, "never called");
         else if (most == INT_MAX)
            sprintf(st, "any number of times");
         else
            sprintf(st, "at most %d times", most);
      else if (least == most)
         sprintf(st, "exactly %d times", least);
      else if (most == INT_MAX)
         sprintf(st, "at least %d times", least);
      else  // 0 < least < most < INT_MAX
         sprintf(st, "between %d and %d times", least, most);

      return st;
   }
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

struct h2_mock {
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

static inline bool h2_mock_g(h2_mock*);

template <size_t I, typename T, typename... Args>
struct h2_nth_type_impl {
   using type = typename h2_nth_type_impl<I - 1, Args...>::type;
};

template <typename T, typename... Args>
struct h2_nth_type_impl<0, T, Args...> {
   using type = T;
};

template <size_t Index, typename... Args>
using h2_nth_type = typename h2_nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;

template <size_t Index, typename... Args>
using h2_nth_type_decay = typename std::decay<h2_nth_type<Index, Args...>>::type;

template <size_t N>
struct h2_tuple_match {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args, const char* file, int line, const char* func) {
      h2_fail* f0 = h2_tuple_match<N - 1>::matches(matchers, args, file, line, func);
      h2_fail* f1 = std::get<N - 1>(matchers).matches(std::get<N - 1>(args));
      if (f1) f1->locate(file, line, func, N - 1);
      h2_append_x_fail(f0, f1);
      return f0;
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
#define __H2_MATCHER_TYPE_LIST                \
   h2_matcher<h2_nth_type_decay<0, Args...>>, \
   h2_matcher<h2_nth_type_decay<1, Args...>>, \
   h2_matcher<h2_nth_type_decay<2, Args...>>, \
   h2_matcher<h2_nth_type_decay<3, Args...>>, \
   h2_matcher<h2_nth_type_decay<4, Args...>>, \
   h2_matcher<h2_nth_type_decay<5, Args...>>, \
   h2_matcher<h2_nth_type_decay<6, Args...>>, \
   h2_matcher<h2_nth_type_decay<7, Args...>>, \
   h2_matcher<h2_nth_type_decay<8, Args...>>, \
   h2_matcher<h2_nth_type_decay<9, Args...>>

#define __H2_MATCHER_PARAMETER_DEFAULT_LIST           \
   h2_matcher<h2_nth_type_decay<0, Args...>> a_0 = _, \
   h2_matcher<h2_nth_type_decay<1, Args...>> a_1 = _, \
   h2_matcher<h2_nth_type_decay<2, Args...>> a_2 = _, \
   h2_matcher<h2_nth_type_decay<3, Args...>> a_3 = _, \
   h2_matcher<h2_nth_type_decay<4, Args...>> a_4 = _, \
   h2_matcher<h2_nth_type_decay<5, Args...>> a_5 = _, \
   h2_matcher<h2_nth_type_decay<6, Args...>> a_6 = _, \
   h2_matcher<h2_nth_type_decay<7, Args...>> a_7 = _, \
   h2_matcher<h2_nth_type_decay<8, Args...>> a_8 = _, \
   h2_matcher<h2_nth_type_decay<9, Args...>> a_9 = _

#define __H2_MATCHER_ARGUMENT_LIST \
   a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7, a_8, a_9

   /* clang-format on */

   typedef std::tuple<Args..., int> argument_tuple;
   typedef std::tuple<__H2_MATCHER_TYPE_LIST> matcher_tuple;

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
            if (c_array[i].is_shortage()) h2_fail_g(fail);
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

   h2_mocker& register_and_return_reference() {
      if (!h2_mock_g(this)) reset();
      return *this;
   }

 public:
   static h2_mocker& I(void* befp = nullptr, const char* befn = nullptr, const char* file = nullptr, int line = 0) {
      static h2_mocker* I = nullptr;
      if (!I) I = new (h2_raw::malloc(sizeof(h2_mocker))) h2_mocker(befp, befn, file, line);
      return I->register_and_return_reference();
   }

   h2_mocker& once(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      c_array.push_back(h2_callexp(1, 1));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& twice(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      c_array.push_back(h2_callexp(2, 2));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& times(int count) {
      c_array.push_back(h2_callexp(count, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& any(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      c_array.push_back(h2_callexp(0, INT_MAX));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& atleast(int count) {
      c_array.push_back(h2_callexp(count, INT_MAX));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& atmost(int count) {
      c_array.push_back(h2_callexp(0, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& between(int left, int right) {
      c_array.push_back(h2_callexp(left, right));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& with(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      if (!m_array.empty()) m_array.back() = std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST);
      return register_and_return_reference();
   }

   /* clang-format off */
   h2_mocker& th1(h2_matcher<h2_nth_type_decay<0, Args...>> a_=_) { if (!m_array.empty()) std::get<0>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th2(h2_matcher<h2_nth_type_decay<1, Args...>> a_=_) { if (!m_array.empty()) std::get<1>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th3(h2_matcher<h2_nth_type_decay<2, Args...>> a_=_) { if (!m_array.empty()) std::get<2>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th4(h2_matcher<h2_nth_type_decay<3, Args...>> a_=_) { if (!m_array.empty()) std::get<3>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th5(h2_matcher<h2_nth_type_decay<4, Args...>> a_=_) { if (!m_array.empty()) std::get<4>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th6(h2_matcher<h2_nth_type_decay<5, Args...>> a_=_) { if (!m_array.empty()) std::get<5>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th7(h2_matcher<h2_nth_type_decay<6, Args...>> a_=_) { if (!m_array.empty()) std::get<6>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th8(h2_matcher<h2_nth_type_decay<7, Args...>> a_=_) { if (!m_array.empty()) std::get<7>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th9(h2_matcher<h2_nth_type_decay<8, Args...>> a_=_) { if (!m_array.empty()) std::get<8>(m_array.back()) = a_; return register_and_return_reference(); }
   /* clang-format on */

   h2_mocker& returns(h2_routine<Class, Return(Args...)> r) {
      if (!r_array.empty()) r_array.back() = r;
      return register_and_return_reference();
   }

   h2_mocker& does(std::function<Return(Args...)> f) {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return register_and_return_reference();
   }

   h2_mocker& does(std::function<Return(Class*, Args...)> f) {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return register_and_return_reference();
   }

   h2_mocker& operator=(std::function<Return(Args...)> f) { return does(f); }
   h2_mocker& operator=(std::function<Return(Class*, Args...)> f) { return does(f); }
};

struct h2_suite;
static inline void h2_suite_case_g(h2_suite*, void*);
static inline void h2_suite_setup_g(h2_suite*);
static inline void h2_suite_teardown_g(h2_suite*);

static constexpr const char* h2_cs[] = {"init", "TODO", "Filtered", "Passed", "Failed"};

struct h2_case {
   /* clang-format off */
   enum S { INITED = 0, PASSED, FAILED, TODOED, FILTED };
   /* clang-format on */

   const char* name;
   h2_suite* suite;
   S status;
   const char* file;
   int line;
   long long t_start, t_end;

   void prev_setup() {
      t_start = h2_milliseconds();

      status = PASSED;
      h2_stack::G().push(file, line, "case");
   }

   void post_setup() { jc = 1; }

   void prev_teardown() { jc = 0; }

   void post_teardown() {
      ul_code = nullptr;
      h2_fail* fail = mock_double_check();
      stub_restore();
      h2_append_x_fail(fail, h2_stack::G().pop());

      if (fail)
         if (status == FAILED)
            delete fail;
         else {
            status = FAILED;
            h2_append_x_fail(fails, fail);
         }

      t_end = h2_milliseconds();
   }

   void execute() {
      prev_setup();
      h2_suite_setup_g(suite);
      post_setup();

      if (::setjmp(jb) == 0) {
         uf_code();
         if (ul_code) ul_code();
      } else
         status = FAILED;

      prev_teardown();
      h2_suite_teardown_g(suite);
      post_teardown();
   }

   h2_list stubs, mocks;

   void do_stub(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
      h2_stub* stub = nullptr;
      h2_list_for_each_entry(p, &stubs, h2_stub, x) if (p->befp == befp && (stub = p)) break;
      if (!stub) {
         stub = new h2_stub(befp, file, line);
         stubs.push(&stub->x);
      }
      stub->replace(tofp);
   }

   void stub_restore() {
      h2_list_for_each_entry(p, &stubs, h2_stub, x) {
         p->restore();
         p->x.out();
         delete p;
      }
   }

   bool do_mock(h2_mock* mock) {
      h2_list_for_each_entry(p, &mocks, h2_mock, x) if (p == mock) return true;
      do_stub(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
      mocks.push(&mock->x);
      return true;
   }

   h2_fail* mock_double_check() {
      h2_fail* fail = nullptr;
      h2_list_for_each_entry(p, &mocks, h2_mock, x) {
         h2_append_x_fail(fail, p->times_check());
         p->reset();
         p->x.out();
      }
      return fail;
   }

   int jc;
   jmp_buf jb;
   h2_fail* fails;

   void do_fail(h2_fail* fail) {
      status = FAILED;
      h2_append_x_fail(fails, fail);
      if (0 < jc--) ::longjmp(jb, 1);
   }

   struct T {
      h2_case* _case;
      int count;

      T(h2_case* case_) : _case(case_), count(0) {
         _case->prev_setup();
         h2_suite_setup_g(_case->suite);
         _case->post_setup();
      }
      ~T() {
         _case->prev_teardown();
         h2_suite_teardown_g(_case->suite);
         _case->post_teardown();
      }

      operator bool() { return 0 == count++; }
   };

   h2_case(const char* name_, h2_suite* suite_, S status_, const char* file_, int line_)
     : name(name_), suite(suite_), status(status_), file(file_), line(line_), jc(0), fails(nullptr), ul_code() {
      h2_suite_case_g(suite, this);
   }

   virtual void uf_code() {}
   std::function<void()> ul_code;
};

struct h2_suite {
   typedef void (*P)(h2_suite*, h2_case*);

   const char* name;
   P p;
   std::function<void()> setup, teardown;
   int status[8];
   const char* file;
   int line;

   int enumerates;
   std::vector<h2_case*> case_list;

   h2_suite(const char* name_, P p_, const char* file_, const int line_, int enumerates_)
     : name(name_), p(p_), setup(), teardown(), status{0}, file(file_), line(line_), enumerates(enumerates_) {
      suites().push_back(this);
   }

   std::vector<h2_case*>& cases() {
      if (!enumerates++) p(this, nullptr); /* enumerate cases */
      return case_list;
   }

   static void execute(h2_suite*, h2_case* _case) {
      if (_case) _case->execute();
   }

   /* clang-format off */
   static std::vector<h2_suite*>& suites() { static std::vector<h2_suite*> __; return __; }
   /* clang-format off */
};

static inline void h2_suite_case_g(h2_suite* suite, void* case_) { suite->case_list.push_back((h2_case*)case_); }

static inline void h2_suite_setup_g(h2_suite* suite) {
   if (suite && suite->setup)
      suite->setup(), suite->setup = nullptr;
}

static inline void h2_suite_teardown_g(h2_suite* suite) {
   if (suite && suite->teardown)
      suite->teardown(), suite->teardown = nullptr;
}

struct h2_log {
   virtual void on_task_start(){};
   virtual void on_task_endup(int status[8], int cases, long long duration){};
   virtual void on_case_start(h2_case* c){};
   virtual void on_case_endup(h2_case* c){};
};

/* clang-format off */
struct h2_logs : public h2_log {
   std::vector<h2_log*> logs;
   void add(h2_log* log) { logs.push_back(log); }
   void on_task_start() { for (auto& log : logs) log->on_task_start(); }
   void on_task_endup(int status[8], int cases, long long duration) { for (auto& log : logs) log->on_task_endup(status, cases, duration); }
   void on_case_start(h2_case* c) { for (auto& log : logs) log->on_case_start(c); }
   void on_case_endup(h2_case* c) { for (auto& log : logs) log->on_case_endup(c); }
};
/* clang-format on */

struct h2_log_console : public h2_log {
   void on_task_endup(int status[8], int cases, long long duration) {
      if (0 < status[h2_case::FAILED]) {
         printf("%s", h2cfg().style("bold,red"));
         printf("\nFailed <%d failed, %d passed, %d todo, %d filtered, %lld ms>\n", status[h2_case::FAILED], status[h2_case::PASSED], status[h2_case::TODOED], status[h2_case::FILTED], duration);
      } else {
         printf("%s", h2cfg().style("bold,green"));
         printf("\nPassed <%d passed, %d todo, %d filtered, %d cases, %lld ms>\n", status[h2_case::PASSED], status[h2_case::TODOED], status[h2_case::FILTED], cases, duration);
      }
      printf("%s", h2cfg().style("reset"));
   }
   void on_case_endup(h2_case* c) {
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         printf("CASE(%s, %s): TODO at %s:%d\n", c->suite->name, c->name, c->file, c->line);
         break;
      case h2_case::FILTED:
         break;
      case h2_case::PASSED:
         if (h2cfg().verbose) {
            printf("%s", h2cfg().style("light blue"));
            printf("CASE(%s, %s): Passed - %lld ms\n", c->suite->name, c->name, c->t_end - c->t_start);
            printf("%s", h2cfg().style("reset"));
         }
         break;
      case h2_case::FAILED:
         printf("%s", h2cfg().style("bold,purple"));
         printf("CASE(%s, %s): Failed at %s:%d\n", c->suite->name, c->name, c->file, c->line);
         printf("%s", h2cfg().style("reset"));
         H2_FAIL_FOREACH(fail, c->fails) { fail->print(); }
         ::printf("\n");
         break;
      }

      printf("%s", h2cfg().style("reset"));
   }
};

struct h2_log_xml : public h2_log {
   void on_task_endup(int status[8], int cases, long long duration) {
      h2_with f(fopen(h2cfg().junit, "w"));
      if (!f.f) return;

      fprintf(f.f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f.f, "<testsuites>\n");

      for (auto& s : h2_suite::suites()) {
         fprintf(f.f, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
                 s->status[h2_case::FAILED], s->name, s->status[h2_case::TODOED] + s->status[h2_case::FILTED], (int)s->cases().size(), 0, "");

         for (auto& c : s->cases()) {
            fprintf(f.f, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
                    c->suite->name, c->name, h2_cs[c->status], (c->t_end - c->t_start) / 1000.0);

            if (c->status == h2_case::FAILED) {
               fprintf(f.f, "      <failure message=\"%s:%d:", c->file, c->line);
               H2_FAIL_FOREACH(fail, c->fails) {
                  fprintf(f.f, "{newline}");
                  fail->print(f.f);
               }
               fprintf(f.f, "\" type=\"AssertionFailedError\"></failure>\n");
            }
            fprintf(f.f, "      <system-out></system-out><system-err></system-err>\n");
            fprintf(f.f, "    </testcase>\n");
         }
         fprintf(f.f, "  </testsuite>\n");
      }
      fprintf(f.f, "</testsuites>\n");
   }
};

struct h2_extra {
   static struct sockaddr_storage* get_sockaddrs() {
      static struct sockaddr_storage sockaddrs[100] = {{0}};
      return sockaddrs;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res) {
      static struct addrinfo addrinfos[100];
      struct sockaddr_storage* sockaddrs = get_sockaddrs();

      struct addrinfo** pp = res;

      for (int i = 0; i < 100; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
         if (0 == b->sin_addr.s_addr) 
            break;

         struct addrinfo* a = &addrinfos[i];
         a->ai_addr = (struct sockaddr*)&sockaddrs[i];
         a->ai_addrlen = sizeof(struct sockaddr_in);
         a->ai_family = AF_INET;
         a->ai_socktype = SOCK_STREAM;
         a->ai_protocol = IPPROTO_TCP;
         a->ai_canonname = nullptr;
         a->ai_next = nullptr;
         if (hints) {
            a->ai_family = hints->ai_family;
            a->ai_socktype = hints->ai_socktype;
            a->ai_protocol = hints->ai_protocol;
         }
         *pp = a;
         pp = &a->ai_next;
      }
      *pp = nullptr;
      return 0;
   }

   static void freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* gethostbyname(const char* name) {
      static char* h_aliases[1] = {0};
      static char* h_addr_list[100];
      static struct hostent h;
      h.h_name = (char*)name;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      struct sockaddr_storage* sockaddrs = get_sockaddrs();
      for (int i = 0; i < 100; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
         if (0 == b->sin_addr.s_addr) break;
         h_addr_list[i] = (char*)&b->sin_addr.s_addr;
      }

      return &h;
   }

   /* clang-format off */
   static h2_extra& I() { static h2_extra __; return __; }
   /* clang-format on */

   h2_extra() : getaddrinfo_stub((void*)::getaddrinfo), freeaddrinfo_stub((void*)::freeaddrinfo) {}

   h2_stub getaddrinfo_stub;
   h2_stub freeaddrinfo_stub;

   void dohook() {
      getaddrinfo_stub.replace((void*)h2_extra::getaddrinfo);
      freeaddrinfo_stub.replace((void*)h2_extra::freeaddrinfo);
   }

   void unhook() {
      getaddrinfo_stub.restore();
      freeaddrinfo_stub.restore();
   }
};

static inline void h2_setaddrinfo(int count, ...) {
   struct sockaddr_storage* sockaddrs = h2_extra::get_sockaddrs();

   int i = 0;
   va_list a;
   va_start(a, count);
   for (; i < count; ++i) {
      const char* p = va_arg(a, const char*);
      struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
      memset(b, 0, sizeof(struct sockaddr_in));
      b->sin_family = AF_INET;
      b->sin_addr.s_addr = inet_addr(p);
   }
   va_end(a);

   memset(&sockaddrs[i], 0, sizeof(struct sockaddr));
}

#define H2DNS(...) h2_setaddrinfo(H2PP_NARGS(__VA_ARGS__), __VA_ARGS__)

struct h2_order {
   static constexpr const char* last_order_file_path = ".last_order";

   static void drop_last_order() { ::remove(last_order_file_path); }

   static void read_last_order(std::vector<std::string>& list) {
      char suite_case_name[1024];
      h2_with f(fopen(last_order_file_path, "r"));
      if (f.f)
         while (1 == fscanf(f.f, "%[^\n]\n", suite_case_name))
            list.push_back(suite_case_name);
   }

   static void save_last_order(std::vector<h2_case*>& list) {
      h2_with f(fopen(last_order_file_path, "w"));
      if (f.f)
         for (auto c : list)
            fprintf(f.f, "%s[:]%s\n", c->suite->name, c->name);
   }

   static std::vector<h2_case*> case_list() {
      std::vector<h2_case*> case1_list, case2_list;
      std::vector<std::string> last_list;

      for (auto s : h2_suite::suites())
         for (auto c : s->cases())
            case1_list.push_back(c);

      read_last_order(last_list);

      if (0 < last_list.size()) {
         for (auto& k : last_list)
            for (auto it = case1_list.begin(); it != case1_list.end(); it++)
               if (k == (*it)->suite->name + std::string("[:]") + (*it)->name) {
                  case2_list.push_back(*it);
                  case1_list.erase(it);
                  break;
               }

         for (auto it = case1_list.begin(); it != case1_list.end(); it = case1_list.erase(it))
            case2_list.push_back(*it);

         return case2_list;
      }

      if (h2cfg().randomize) {
         shuffle(case1_list.begin(), case1_list.end(), std::default_random_engine{std::random_device()()});
         save_last_order(case1_list);
      }

      return case1_list;
   }

   static void print_list() {
      int ss = 0, cs = 0, t = h2cfg().listing;
      bool sb = t == 'a' || t == 'A' || t == 's' || t == 'S', cb = t == 'a' || t == 'A' || t == 'c' || t == 'C';

      for (auto s : h2_suite::suites()) {
         const char* sn = strlen(s->name) ? s->name : "(Anonymous)";
         if (t = 0, sb) {
            if (!h2cfg().filter(sn, "", "")) t++;
            for (auto c : s->cases())
               if (!h2cfg().filter(sn, cb ? c->name : "", "")) t++;
            if (t) printf("S%d. %s \n", ++ss, sn);
         }
         if (t = 0, cb)
            for (auto c : s->cases())
               if (!h2cfg().filter(sn, c->name, ""))
                  sb ? printf("C%d/S%d/%d. %s // %s \n", ++cs, ss, ++t, sn, c->name) : printf("C%d. %s // %s \n", ++cs, sn, c->name);
      }
   }

   static void list_then_exit() { print_list(), exit(0); }
};

struct h2_task {
   std::vector<h2_case*> case_list;

   h2_logs logs;
   h2_log_console console_log;
   h2_log_xml xml_log;

   int status[8];

   h2_task() : status{0}, current_case(nullptr) {}

   h2_case* current_case;

   /* clang-format off */
   static h2_task& I() { static h2_task __; return __; }
   /* clang-format on */

   void prepare(int argc, const char** argv) {
      h2cfg().opt(argc, argv);

      if (h2cfg().listing) h2_order::list_then_exit();

      logs.add(&console_log);
      if (strlen(h2cfg().junit)) logs.add(&xml_log);

      h2_nm::I(); /* invoke system nm */

      h2_sihook_g();

      case_list = h2_order::case_list();

      h2_extra::I().dohook();
      h2_setaddrinfo(1, "127.0.0.1");

      h2_stack::G().push(__FILE__, __LINE__, "root");
      h2_dohook_g();
   }

   void cleanup() {
      h2_unhook_g();
      h2_extra::I().unhook();
      if (status[h2_case::FAILED] == 0) h2_order::drop_last_order();
   }

   void execute() {
      long long t_start = h2_milliseconds();
      logs.on_task_start();
      for (auto c : case_list) {
         current_case = c;
         logs.on_case_start(c);
         if (h2cfg().filter(c->suite->name, c->name, c->file)) c->status = h2_case::FILTED;
         if (h2_case::INITED == c->status) c->suite->p(c->suite, c);
         logs.on_case_endup(c);
         status[c->status] += 1;
         c->suite->status[c->status] += 1;
         if (0 < h2cfg().breakable && h2cfg().breakable <= status[h2_case::FAILED]) break;
      }
      logs.on_task_endup(status, case_list.size(), h2_milliseconds() - t_start);
   }
};

static inline void h2_stub_g(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   if (h2_task::I().current_case) h2_task::I().current_case->do_stub(befp, tofp, befn, tofn, file, line);
}

static inline bool h2_mock_g(h2_mock* mock) {
   return h2_task::I().current_case ? h2_task::I().current_case->do_mock(mock) : false;
}

static inline void h2_fail_g(void* fail) {
   if (h2_task::I().current_case && fail) h2_task::I().current_case->do_fail((h2_fail*)fail);
}

h2_selectany int main(int argc, const char** argv) {
   h2_task::I().prepare(argc, argv);
   h2_task::I().execute();
   h2_task::I().cleanup();
   return 0;
}

#endif
