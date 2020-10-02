
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
#include <map>         /* std::map */
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
#include "utils/h2_macro.hpp"
#include "utils/h2_template.hpp"
#include "utils/h2_list.hpp"
#include "utils/h2_misc.hpp"
#include "utils/h2_numeric.hpp"
#include "utils/h2_libc.hpp"
#include "utils/h2_shared_ptr.hpp"
#include "utils/h2_allocate.hpp"
#include "utils/h2_string.hpp"
#include "h2_line.hpp"                // utils
#include "h2_stringify.hpp"           // utils
#include "utils/h2_nm.hpp"            //
#include "h2_option.hpp"              // utils
#include "h2_layout.hpp"              // line
#include "h2_color.hpp"               // line
#include "h2_backtrace.hpp"           // line
#include "h2_debug.hpp"               // option, backtrace
#include "h2_patch.hpp"               // backtrace
#include "h2_failure.hpp"             // backtrace
#include "h2_generator.hpp"           // utils
#include "json/h2_json.hpp"           // line
#include "memory/h2_memory.hpp"       // failure
#include "matcher/h2_matches.hpp"     // failure
#include "matcher/h2_matcher.hpp"     // matches, failure
#include "matcher/h2_equation.hpp"    // matches
#include "matcher/h2_cast.hpp"        // matcher
#include "matcher/h2_unary.hpp"       // matches, matcher, failure
#include "matcher/h2_logic.hpp"       // matches, matcher, failure
#include "matcher/h2_inequation.hpp"  // matches, matcher, failure
#include "matcher/h2_strcmp.hpp"      // matches, matcher, failure
#include "matcher/h2_memcmp.hpp"      // matches, matcher, failure
#include "matcher/h2_container.hpp"   // matches, matcher, failure
#include "matcher/h2_customize.hpp"   // matches, matcher, failure
#include "matcher/h2_matcher.cpp"     // matches, matcher, equation
#include "stub/h2_fp.hpp"             // nm
#include "stub/h2_mfp.hpp"            // nm
#include "stub/h2_stub.hpp"           //
#include "mock/h2_routine.hpp"        //
#include "mock/h2_checkin.hpp"        // failure
#include "mock/h2_match.hpp"          // failure
#include "mock/h2_mocking.hpp"        // failure, checkin, routine, matcher, stub
#include "mock/h2_mocks.hpp"          // failure, checkin
#include "mock/h2_mock.hpp"           // failure, checkin
#include "extension/h2_dns.hpp"       //
#include "extension/h2_socket.hpp"    // stub, failure, matcher
#include "extension/h2_stdio.hpp"     //
#include "core/h2_case.hpp"           // failure, stub, mock, dns, socket
#include "core/h2_suite.hpp"          // case, stub, mock
#include "core/h2_task.hpp"           // suite, case, failure, stub, mock, option, debug
#include "assert/h2_assert.hpp"       // failure, matcher
#include "h2_report.hpp"              // task, suite, case
}  // namespace h2
#include "assert/h2_use.hpp"

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

#ifndef CASES_T
#   define CASES_T H2CASES_T
#else
#   pragma message("CASES_T conflict, using H2CASES_T instead.")
#endif

#ifndef CASESS_T
#   define CASESS_T H2CASESS_T
#else
#   pragma message("CASESS_T conflict, using H2CASESS_T instead.")
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

#define __H2SUITE(name, Q)                                       \
   static void Q(h2::h2_suite*, h2::h2_case*);                   \
   static h2::h2_suite H2Q(suite)(name, &Q, __FILE__, __LINE__); \
   static void Q(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5)

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2Q(h2_suite_test))

#define H2Cleanup()                                \
   if (::setjmp(suite_2_0_1_3_0_1_0_2->jump) == 0) \
      suite_2_0_1_3_0_1_0_2->jumpable = true;      \
   if (!case_2_0_1_7_0_3_2_5)

#define __H2Case(name, status, Q)                                                                             \
   static h2::h2_case Q(name, status, __FILE__, __LINE__);                                                    \
   static h2::h2_suite::installer H2Q(i)(suite_2_0_1_3_0_1_0_2, &Q);                                          \
   if (&Q == case_2_0_1_7_0_3_2_5)                                                                            \
      for (h2::h2_suite::cleaner _1_9_8_0_(suite_2_0_1_3_0_1_0_2); _1_9_8_0_; case_2_0_1_7_0_3_2_5 = nullptr) \
         for (h2::h2_case::cleaner _1_9_8_1_(&Q); _1_9_8_1_;)                                                 \
            if (::setjmp(Q.jump) == 0)

#define H2Case(...) __H2Case(#__VA_ARGS__, h2::h2_case::initial, H2Q(t_case))
#define H2Todo(...) __H2Case(#__VA_ARGS__, h2::h2_case::todo, H2Q(t_case))

#define __H2CASE(name, status, QC, QS)                                                    \
   static void QC();                                                                      \
   static void QS(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5) \
   {                                                                                      \
      static h2::h2_case c(name, status, __FILE__, __LINE__);                             \
      static h2::h2_suite::installer i(suite_2_0_1_3_0_1_0_2, &c);                        \
      if (&c == case_2_0_1_7_0_3_2_5)                                                     \
         for (h2::h2_case::cleaner a(&c); a;)                                             \
            if (::setjmp(c.jump) == 0)                                                    \
               QC();                                                                      \
   }                                                                                      \
   static h2::h2_suite H2Q(suite)("Anonymous", &QS, __FILE__, __LINE__);                  \
   static void QC()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, h2::h2_case::initial, H2Q(h2_case_test), H2Q(h2_suite_test))
#define H2TODO(...) __H2CASE(#__VA_ARGS__, h2::h2_case::todo, H2Q(h2_case_test), H2Q(h2_suite_test))

#endif
