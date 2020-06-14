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
#include <utility>       /* std::forward, std::pair */
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
#include "h2_pp.hpp"
#include "h2_kit.hpp"
#include "h2_numeric.hpp"
#include "h2_list.hpp"
#include "h2_option.hpp"
#include "h2_libc.hpp"
#include "h2_allocate.hpp"
#include "h2_string.hpp"
#include "h2_line.hpp"
#include "h2_layout.hpp"
#include "h2_color.hpp"
#include "h2_shared_ptr.hpp"
#include "h2_debug.hpp"
#include "h2_expr.hpp"
#include "h2_json.hpp"
#include "h2_backtrace.hpp"
#include "h2_failure.hpp"
#include "h2_stub.hpp"
#include "h2_heap.hpp"
#include "h2_mfp.hpp"
#include "h2_matchee.hpp"
#include "h2_matcher.hpp"
#include "h2_matches.hpp"
#include "h2_matcheu.hpp"
#include "h2_attendance.hpp"
#include "h2_function.hpp"
#include "h2_mock.hpp"
#include "h2_stdio.hpp"
#include "h2_dns.hpp"
#include "h2_socket.hpp"
#include "h2_case.hpp"
#include "h2_suite.hpp"
#include "h2_directory.hpp"
#include "h2_check.hpp"
#include "h2_generator.hpp"
#include "h2_patch.hpp"
#include "h2_task.hpp"
#include "h2_report.hpp"
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
   static void QP(h2::h2_suite* _2_0_1_3_suite, h2::h2_case* _2_0_1_7_case)

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2Q(h2_suite_test))

#define H2Cleanup()                         \
   if (::setjmp(_2_0_1_3_suite->jump) == 0) \
      _2_0_1_3_suite->jumpable = true;      \
   if (!_2_0_1_7_case)

#define __H2Case(name, status, Qc, Q1, Q2)                                        \
   static h2::h2_case Qc(name, status, __FILE__, __LINE__);                       \
   static h2::h2_suite::installer H2Q(installer)(_2_0_1_3_suite, &Qc);            \
   if (&Qc == _2_0_1_7_case)                                                      \
      for (h2::h2_suite::cleaner Q1(_2_0_1_3_suite); Q1; _2_0_1_7_case = nullptr) \
         for (h2::h2_case::cleaner Q2(&Qc); Q2;)                                  \
            if (::setjmp(Qc.jump) == 0)

#define H2Case(...) __H2Case(#__VA_ARGS__, h2::h2_case::initial, H2Q(t_case), H2Q(_1), H2Q(_2))
#define H2Todo(...) __H2Case(#__VA_ARGS__, h2::h2_case::todo, H2Q(t_case), H2Q(_1), H2Q(_2))

#define __H2CASE(name, status, QR, QP)                                      \
   static void QR();                                                        \
   static void QP(h2::h2_suite* _2_0_1_3_suite, h2::h2_case* _2_0_1_7_case) \
   {                                                                        \
      static h2::h2_case c(name, status, __FILE__, __LINE__);               \
      static h2::h2_suite::installer i(_2_0_1_3_suite, &c);                 \
      if (&c == _2_0_1_7_case)                                              \
         for (h2::h2_case::cleaner a(&c); a;)                               \
            if (::setjmp(c.jump) == 0)                                      \
               QR();                                                        \
   }                                                                        \
   static h2::h2_suite H2Q(suite)("Anonymous", &QP, __FILE__, __LINE__);    \
   static void QR()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, h2::h2_case::initial, H2Q(h2_case_test), H2Q(h2_suite_test))
#define H2TODO(...) __H2CASE(#__VA_ARGS__, h2::h2_case::todo, H2Q(h2_case_test), H2Q(h2_suite_test))

#define __H2BLOCK(Attributes, Qb) for (h2::h2_heap::stack::block Qb(Attributes, __FILE__, __LINE__); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2Q(t_block))

#define H2DNS(...) h2::h2_dns::setaddrinfo(H2PP_NARG(__VA_ARGS__), __VA_ARGS__)

/* clang-format off */
#define __H2SOCK0(_Packet, _Size, ...) h2::h2_socket::inject_received(_Packet, _Size, #__VA_ARGS__)
#define __H2SOCK1(...) h2::h2_socket::start_and_fetch()
#define H2SOCK(...) H2PP_CAT2(__H2SOCK, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
/* clang-format on */

#define H2COUT(...) h2::h2_stdio::capture_cout(#__VA_ARGS__)

#endif
