
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

/* =====> Interface <====== */

#define SUITE(...) H2SUITE(__VA_ARGS__)
#define CASE(...) H2CASE(__VA_ARGS__)
#define TODO(...) H2TODO(__VA_ARGS__)

#define Setup(...) H2Setup(__VA_ARGS__)
#define Teardown(...) H2Teardown(__VA_ARGS__)

#define Case(...) H2Case(__VA_ARGS__)
#define Todo(...) H2Todo(__VA_ARGS__)

#define OK(...) H2OK(__VA_ARGS__)
#define JE(...) H2JE(__VA_ARGS__)
#define MOCK(...) H2MOCK(__VA_ARGS__)
#define STUB(...) H2STUB(__VA_ARGS__)
#define BLOCK(...) H2BLOCK(__VA_ARGS__)
#define DNS(...) H2DNS(__VA_ARGS__)

#define _ h2::Any__
#define Any() h2::Any__
#define Null() h2::Null__()
#define Eq(e) h2::Eq__(e)
#define Ge(e) h2::Ge__(e)
#define Gt(e) h2::Gt__(e)
#define Le(e) h2::Le__(e)
#define Lt(e) h2::Lt__(e)
#define Me(...) h2::Me__(__VA_ARGS__)
#define Pe(e) h2::Pe__(e)
#define Re(e) h2::Re__(e)
#define We(e) h2::We__(e)
#define Contains(e) h2::Contains__(e)
#define StartsWith(e) h2::StartsWith__(e)
#define EndsWith(e) h2::EndsWith__(e)
#define CaseLess(e) h2::CaseLess__(e)
#define Je(e) h2::Je__(e)
#define Not(e) h2::Not__(e)
#define AllOf(...) h2::AllOf__(__VA_ARGS__)
#define AnyOf(...) h2::AnyOf__(__VA_ARGS__)
#define NoneOf(...) h2::NoneOf__(__VA_ARGS__)
#define ListOf(...) h2::ListOf__(__VA_ARGS__)

/* ==================> implementation ============================== */

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

#define __H2SUITE(Suitename, s, a)                              \
   static void s(h2::h2_suite*, h2::h2_case*);                  \
   static h2::h2_suite a(Suitename, &s, __FILE__, __LINE__, 0); \
   static void s(h2::h2_suite* ___suite, h2::h2_case* case___)

#define H2SUITE(...) __H2SUITE(H2PP_STRINGIZE(__VA_ARGS__), H2Q(suite), H2Q(a))

#define __H2Setup(t) \
   for (int t = 1; t--; case___ ? void() : h2::h2_suite_setup_g(___suite)) ___suite->setup = [&]()
#define __H2Teardown(t) \
   for (int t = 1; t--; case___ ? void() : h2::h2_suite_teardown_g(___suite)) ___suite->teardown = [&]()

#define H2Setup() __H2Setup(H2Q(t))
#define H2Teardown() __H2Teardown(H2Q(t))

#define __H2CaseLambda(Casename, Status, c, t)                           \
   static h2::h2_case c(Casename, ___suite, Status, __FILE__, __LINE__); \
   if (&c == case___)                                                    \
      for (bool t = true; t; c.execute(), t = false) c.lambda_code = [&]()

#define __H2CaseSnippet(Casename, Status, c, h)                          \
   static h2::h2_case c(Casename, ___suite, Status, __FILE__, __LINE__); \
   if (&c == case___)                                                    \
      for (h2::h2_case::H h(&c); h && ::setjmp(c.jb) == 0;)

#ifdef H2CaseLambda
#   define H2Case(...) __H2CaseLambda(H2PP_STRINGIZE(__VA_ARGS__), h2::h2_case::INITED, H2Q(c), H2Q(t))
#   define H2Todo(...) __H2CaseLambda(H2PP_STRINGIZE(__VA_ARGS__), h2::h2_case::TODOED, H2Q(c), H2Q(t))
#else
#   define H2Case(...) __H2CaseSnippet(H2PP_STRINGIZE(__VA_ARGS__), h2::h2_case::INITED, H2Q(c), H2Q(h))
#   define H2Todo(...) __H2CaseSnippet(H2PP_STRINGIZE(__VA_ARGS__), h2::h2_case::TODOED, H2Q(c), H2Q(h))
#endif

#define __H2CASE(Casename, Status, a, C, t)                                                \
   static h2::h2_suite a("", h2::h2_suite::execute, __FILE__, __LINE__, 1);                \
   namespace {                                                                             \
   struct C : private h2::h2_case {                                                        \
      C(h2::h2_suite* suite) : h2::h2_case(Casename, suite, Status, __FILE__, __LINE__) {} \
      void derive_code() override;                                                         \
   };                                                                                      \
   static C t(&a);                                                                         \
   }                                                                                       \
   void C::derive_code()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, h2::h2_case::INITED, H2Q(a), H2Q(h2_case), H2Q(t))
#define H2TODO(...) __H2CASE(#__VA_ARGS__, h2::h2_case::TODOED, H2Q(a), H2Q(h2_case), H2Q(t))

#define __H2OK4(condition, _1, _2, f)                                 \
   do {                                                               \
      if (!(condition)) {                                             \
         h2::h2_fail* f = new h2::h2_fail_normal(__FILE__, __LINE__); \
         f->kprintf("%s is false", #condition);                       \
         h2::h2_fail_g(f);                                            \
      }                                                               \
   } while (0)

#define __H2OK5(expect, actual, a, e, f)                             \
   do {                                                              \
      auto a = actual;                                               \
      h2::h2_matcher<typename std::conditional<                      \
        std::is_enum<typename std::decay<decltype(a)>::type>::value, \
        int, /* translate enum type to int type */                   \
        typename std::decay<decltype(a)>::type>::type>               \
        e(expect);                                                   \
      h2::h2_fail* f = e.matches(a);                                 \
      if (f) {                                                       \
         f->locate(__FILE__, __LINE__);                              \
         h2::h2_fail_g(f);                                           \
      }                                                              \
   } while (0)

#define H2OK(...) H2PP_VARIADIC_CALL(__H2OK, __VA_ARGS__, H2Q(a), H2Q(e), H2Q(f))

#define H2JE(expect, actual) __H2OK5(Je(expect), actual, H2Q(a), H2Q(e), H2Q(f))

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

#define __H2STUB4(Return, BeFunc, Args, Q)                                          \
   struct {                                                                         \
      void operator=(Return(*toF) Args) {                                           \
         Return(*beF) Args = BeFunc;                                                \
         h2::h2_stub_g((void*)beF, (void*)(toF), #BeFunc, "~", __FILE__, __LINE__); \
      }                                                                             \
   } Q;                                                                             \
   Q = [] Args -> Return

#define __H2STUB50(Return, Class, Method, Args, Q)                                                                                                                                                  \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that)) {                                                                                                                                                 \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Q;                                                                                                                                                                                             \
   Q = [](Class * that) -> Return

#define __H2STUB51(Return, Class, Method, Args, Q)                                                                                                                                                  \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                                                                                  \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Q;                                                                                                                                                                                             \
   Q = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB5(Return, Class, Method, Args, Q) \
   H2PP_IF(H2PP_0ARGS Args, __H2STUB50(Return, Class, Method, Args, Q), __H2STUB51(Return, Class, Method, Args, Q))

#define __H2STUB60(Return, Class, Method, Args, Instance, Q)                                                                                     \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that)) {                                                                                              \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Q;                                                                                                                                          \
   Q = [](Class * that) -> Return

#define __H2STUB61(Return, Class, Method, Args, Instance, Q)                                                                                     \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                               \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Q;                                                                                                                                          \
   Q = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB6(Return, Class, Method, Args, Instance, Q) \
   H2PP_IF(H2PP_0ARGS Args, __H2STUB60(Return, Class, Method, Args, Instance, Q), __H2STUB61(Return, Class, Method, Args, Instance, Q))

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__, H2Q(t))

#define __H2BLOCK0(t) for (h2::h2_stack::T t(__FILE__, __LINE__); t;)

#define __H2BLOCK1(t, ...) for (h2::h2_stack::T t(__FILE__, __LINE__, __VA_ARGS__); t;)

#define H2BLOCK(...) H2PP_IF(H2PP_0ARGS(__VA_ARGS__), __H2BLOCK0(H2Q(t)), __H2BLOCK1(H2Q(t), __VA_ARGS__))
// #define H2BLOCK(...) for (h2::h2_stack::T t(__FILE__, __LINE__, ##__VA_ARGS__); t;)
// #define H2BLOCK(...) for (h2::h2_stack::T t(__FILE__, __LINE__, __VA_OPT__(,) __VA_ARGS__); t;)

#define H2DNS(hostname, ...) h2::h2_ns::setaddrinfo(hostname, H2PP_NARGS(__VA_ARGS__), __VA_ARGS__)

namespace h2 {

#include "h2_tool.cpp"
#include "h2_list.cpp"
#include "h2_option.cpp"
#include "h2_allocate.cpp"
#include "h2_expr.cpp"
#include "h2_json.cpp"
#include "h2_backtrace.cpp"
#include "h2_failure.cpp"
#include "h2_stub.cpp"
#include "h2_heap.cpp"
#include "h2_mfp.cpp"
#include "h2_matcher.cpp"
#include "h2_callexp.cpp"
#include "h2_routine.cpp"
#include "h2_mock.cpp"
#include "h2_ns.cpp"
#include "h2_case.cpp"
#include "h2_suite.cpp"
#include "h2_log.cpp"
#include "h2_extra.cpp"
#include "h2_directory.cpp"
#include "h2_task.cpp"
}  // namespace h2

h2_selectany int main(int argc, const char** argv) {
   h2::h2_option::I(argc, argv);
   h2::h2_task::I().prepare();
   h2::h2_task::I().execute();
   h2::h2_task::I().cleanup();
   return 0;
}

#endif
