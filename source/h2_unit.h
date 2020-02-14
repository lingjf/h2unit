
#ifndef ___H2UNIT_H___
#define ___H2UNIT_H___

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <cstring>
#include <cassert>
#include <cctype>
#include <climits>
#include <csetjmp>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include <tuple>
#include <algorithm>
#include <functional>
#include <utility>
#include <random>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>    /* sigaction */
#include <alloca.h>    /* alloca */
#include <execinfo.h>  /* backtrace */
#include <cxxabi.h>    /* demangle */
#include <sys/mman.h>  /* mprotect mmap */
#include <sys/ioctl.h> /* ioctl */
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#if defined __GLIBC__
#   include <malloc.h>
#elif defined __APPLE__
#   include <AvailabilityMacros.h>
#   include <malloc/malloc.h>
#endif

#if defined __clang__
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wwritable-strings"
#elif defined __GNUC__
#   pragma GCC diagnostic ignored "-Wsign-compare"
#   pragma GCC diagnostic ignored "-Wwrite-strings"
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

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

#define __H2SUITE(Suitename, s, a)                              \
   static void s(h2_suite*, h2_case*);                          \
   static h2_suite a(Suitename, &s, __FILE__, __LINE__, false); \
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
   static h2_suite a("", h2_suite::execute, __FILE__, __LINE__, true);             \
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

#define __H2STUB3(BeFunc, ToFunc, Q)                                                 \
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

#include "h2_tool.cpp"
#include "h2_list.cpp"
#include "h2_configure.cpp"
#include "h2_allocate.cpp"
#include "h2_expr.cpp"
#include "h2_json.cpp"
#include "h2_backtrace.cpp"
#include "h2_failure.cpp"
#include "h2_stub.cpp"
#include "h2_heap.cpp"
#include "h2_mfp.cpp"
#include "h2_matcher.cpp"
#include "h2_callx.cpp"
#include "h2_routine.cpp"
#include "h2_mock.cpp"
#include "h2_case.cpp"
#include "h2_suite.cpp"
#include "h2_log.cpp"
#include "h2_extra.cpp"
#include "h2_order.cpp"
#include "h2_task.cpp"

h2_selectany int main(int argc, const char** argv) {
   h2_task::I().prepare(argc, argv);
   h2_task::I().execute();
   h2_task::I().cleanup();
   return 0;
}

#endif
