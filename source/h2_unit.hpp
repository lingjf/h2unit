
#ifndef __H2_UNIT_HPP__
#define __H2_UNIT_HPP__

#define H2UNIT_VERSION 5.9

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

#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
#   define WIN32_LEAN_AND_MEAN /* fix winsock.h winsock2.h conflict */
#   define NOMINMAX            /* fix std::min/max conflict with windows::min/max */
#   include <windows.h>
#   include <malloc.h> /* alloca */
#   define alloca _alloca
#   define ssize_t int
#else
#   include <alloca.h> /* alloca */
#endif

#if defined __GNUC__ || defined __clang__
//clang  #pragma clang diagnostic ignored <==> pragma GCC diagnostic ignored
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#   pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#   pragma GCC diagnostic ignored "-Wparentheses"
#   pragma GCC diagnostic ignored "-Wsign-compare"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wwrite-strings"
#   pragma GCC diagnostic ignored "-Wreturn-type"
#elif defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
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
#include "utils/h2_macro.hpp"          //
#include "utils/h2_template.hpp"       //
#include "utils/h2_list.hpp"           //
#include "utils/h2_misc.hpp"           //
#include "utils/h2_numeric.hpp"        //
#include "utils/h2_libc.hpp"           // list
#include "utils/h2_shared_ptr.hpp"     // libc
#include "utils/h2_allocate.hpp"       // libc
#include "utils/h2_string.hpp"         // allocate
#include "utils/h2_row.hpp"            // string
#include "utils/h2_stringify.hpp"      // string, row
#include "utils/h2_color.hpp"          // row
#include "utils/h2_nm.hpp"             //
#include "utils/h2_backtrace.hpp"      // row
#include "other/h2_option.hpp"         //
#include "other/h2_layout.hpp"         //
#include "other/h2_debug.hpp"          // option
#include "other/h2_failure.hpp"        //
#include "json/h2_json.hpp"            //
#include "memory/h2_exempt.hpp"        //
#include "memory/h2_memory.hpp"        // failure
#include "exception/h2_exception.hpp"  //
#include "matcher/h2_matches.hpp"      // failure
#include "matcher/h2_matcher.hpp"      // matches, failure
#include "matcher/h2_equation.hpp"     // matches
#include "matcher/h2_cast.hpp"         // matcher
#include "matcher/h2_unary.hpp"        // matches, matcher, failure
#include "matcher/h2_logic.hpp"        // matches, matcher, failure
#include "matcher/h2_inequation.hpp"   // matches, matcher, failure
#include "matcher/h2_strcmp.hpp"       // matches, matcher, failure
#include "matcher/h2_memcmp.hpp"       // matches, matcher, failure
#include "matcher/h2_container.hpp"    // matches, matcher, failure
#include "matcher/h2_customize.hpp"    // matches, matcher, failure
#include "matcher/h2_matcher.cpp"      // matches, matcher, equation
#include "stub/h2_fp.hpp"              //
#include "stub/h2_mfp.hpp"             //
#include "stub/h2_stub.hpp"            //
#include "mock/h2_routine.hpp"         //
#include "mock/h2_checkin.hpp"         // failure
#include "mock/h2_match.hpp"           // failure
#include "mock/h2_mock.hpp"            // failure, checkin, routine, matcher, stub
#include "mock/h2_mocks.hpp"           // failure, checkin
#include "extension/h2_dns.hpp"        //
#include "extension/h2_socket.hpp"     // stub, failure, matcher
#include "extension/h2_stdio.hpp"      //
#include "extension/h2_perf.hpp"       // failure
#include "core/h2_case.hpp"            // failure, stub, mock, dns, socket
#include "core/h2_suite.hpp"           // case, stub, mock
#include "core/h2_task.hpp"            // suite, case, failure, stub, mock, option, debug
#include "assert/h2_assert.hpp"        // failure, matcher
#include "other/h2_report.hpp"         // task, suite, case
}  // namespace h2

#include "stub/h2_use.hpp"
#include "mock/h2_use.hpp"
#include "core/h2_use.hpp"
#include "assert/h2_use.hpp"

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

#ifndef Setup
#   define Setup H2Setup
#else
#   pragma message("Setup conflict, using H2Setup instead.")
#endif

#ifndef Teardown
#   define Teardown H2Teardown
#else
#   pragma message("Teardown conflict, using H2Teardown instead.")
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

#ifndef UNSTUB
#   define UNSTUB H2UNSTUB
#else
#   pragma message("UNSTUB conflict, using H2UNSTUB instead.")
#endif

#ifndef UNMOCK
#   define UNMOCK H2UNMOCK
#else
#   pragma message("UNMOCK conflict, using H2UNMOCK instead.")
#endif

#ifndef BLOCK
#   define BLOCK H2BLOCK
#else
#   pragma message("BLOCK conflict, using H2BLOCK instead.")
#endif

#ifndef UNMEM
#   define UNMEM H2UNMEM
#else
#   pragma message("UNMEM conflict, using H2UNMEM instead.")
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

#ifndef PF
#   define PF H2PF
#else
#   pragma message("PF conflict, using H2PF instead.")
#endif

#ifndef GlobalSetup
#   define GlobalSetup H2GlobalSetup
#else
#   pragma message("GlobalSetup conflict, using H2GlobalSetup instead.")
#endif

#ifndef GlobalCleanup
#   define GlobalCleanup H2GlobalCleanup
#else
#   pragma message("GlobalCleanup conflict, using H2GlobalCleanup instead.")
#endif

#ifndef GlobalSuiteSetup
#   define GlobalSuiteSetup H2GlobalSuiteSetup
#else
#   pragma message("GlobalSuiteSetup conflict, using H2GlobalSuiteSetup instead.")
#endif

#ifndef GlobalSuiteCleanup
#   define GlobalSuiteCleanup H2GlobalSuiteCleanup
#else
#   pragma message("GlobalSuiteCleanup conflict, using H2GlobalSuiteCleanup instead.")
#endif

#ifndef GlobalCaseSetup
#   define GlobalCaseSetup H2GlobalCaseSetup
#else
#   pragma message("GlobalCaseSetup conflict, using H2GlobalCaseSetup instead.")
#endif

#ifndef GlobalCaseCleanup
#   define GlobalCaseCleanup H2GlobalCaseCleanup
#else
#   pragma message("GlobalCaseCleanup conflict, using H2GlobalCaseCleanup instead.")
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

#ifndef TEST_C
#   define private public
#endif

#endif
