#ifndef __H2_UNIT_HPP__
#define __H2_UNIT_HPP__

#include <cstdio>      /* printf */
#include <cstdlib>     /* malloc */
#include <cstdint>     /* std::uintptr_t */
#include <cstring>     /* strcpy, memcpy */
#include <cmath>       /* fabs, NAN */
#include <csetjmp>     /* setjmp, longjmp */
#include <sstream>     /* std::basic_ostringstream */
#include <string>      /* std::string */
#include <vector>      /* std::vector */
#include <memory>      /* std::allocator, std::shared_ptr */
#include <tuple>       /* std::tuple */
#include <functional>  /* std::function */
#include <utility>     /* std::forward, std::pair */
#include <type_traits> /* std::true_type */

#if defined _MSC_VER
#include <malloc.h> /* _alloca _msize _expand */
#define alloca _alloca
#elif defined __MINGW32__ || defined __MINGW64__
#else
#include <alloca.h> /* alloca */
#endif

#if defined __GNUC__ || defined __clang__
// #pragma clang diagnostic ignored === #pragma GCC diagnostic ignored
#pragma GCC diagnostic ignored "-Wparentheses"   // OK
#pragma GCC diagnostic ignored "-Wsign-compare"  // (in)equation
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined _MSC_VER
#pragma warning(disable : 4244)  // conversion possible loss of data
#pragma warning(disable : 4267)  // conversion possible loss of data
#pragma warning(disable : 4305)  // type cast
#pragma warning(disable : 4312)  // type cast
#pragma warning(disable : 4611)  // setjmp non-portable
#pragma warning(disable : 4996)  // unsafe
#endif

#if defined __H2UNIT_HPP__
#define h2_inline
#else
#define h2_inline inline
#endif

namespace h2 {
#include "utils/h2_macro.hpp"                 //
#include "utils/h2_macro.in.hpp"              //
#include "utils/h2_array.hpp"                 //
#include "utils/h2_template.hpp"              //
#include "utils/h2_numberfy.hpp"              //
#include "utils/h2_list.hpp"                  //
#include "utils/h2_pattern.hpp"               //
#include "utils/h2_misc.hpp"                  //
#include "utils/h2_libc.hpp"                  // list
#include "utils/h2_shared_ptr.hpp"            // libc
#include "utils/h2_allocate.hpp"              // libc
#include "utils/h2_string.hpp"                // allocate
#include "utils/h2_line.hpp"                  // string
#include "utils/h2_stringify.hpp"             // string, line
#include "symbol/h2_nm.hpp"                   //
#include "symbol/h2_load.hpp"                 //
#include "symbol/h2_backtrace.hpp"            // line
#include "symbol/h2_cxa.hpp"                  //
#include "render/h2_failure.hpp"              //
#include "render/h2_option.hpp"               //
#include "core/h2_test.hpp"                   //
#include "core/h2_case.hpp"                   // failure
#include "core/h2_suite.hpp"                  // case
#include "core/h2_runner.hpp"                 // failure
#include "core/h2_core.hpp"                   // [macro]
#include "render/h2_report.hpp"               //!runner, suite, case
#include "matcher/h2_matches.hpp"             // failure
#include "matcher/h2_matcher.hpp"             // matches, failure
#include "matcher/h2_matches_equation.hpp"    //!matches
#include "matcher/h2_matcher_cast.hpp"        // matcher, equation
#include "matcher/h2_matches_cast.hpp"        // matches, matcher, failure
#include "matcher/h2_matches_container.hpp"   // matches, matcher, failure
#include "matcher/h2_matches_logic.hpp"       // matches, matcher, failure
#include "matcher/h2_matches_inequation.hpp"  // matches, matcher, failure
#include "matcher/h2_matches_memcmp.hpp"      // matches, matcher, failure
#include "matcher/h2_matches_strcmp.hpp"      // matches, matcher, failure
#include "matcher/h2_matches_pointee.hpp"     // matches, matcher, failure
#include "matcher/h2_matches_range.hpp"       // matches, matcher, failure
#include "matcher/h2_matches_member.hpp"      // matches, matcher, failure
#include "matcher/h2_matcher.cpp"             //!matches, matcher, equation
#include "matcher/h2_matcher_customize.hpp"   // matches, matcher, failure
#include "stub/h2_fp.hpp"                     // nm, load
#include "stub/h2_stubs.hpp"                  // -
#include "stub/h2_temporary.hpp"              // -
#include "stub/h2_stuber.hpp"                 // -
#include "stub/h2_stub.hpp"                   // -
#include "mock/h2_routine.hpp"                //
#include "mock/h2_checkin.hpp"                // failure
#include "mock/h2_tuple.hpp"                  // failure, runner
#include "mock/h2_mocker.hpp"                 // failure, checkin, routine, matcher, stub
#include "mock/h2_mocks.hpp"                  // failure, checkin
#include "mock/h2_mock.hpp"                   //
#include "memory/h2_exempt.hpp"               // -
#include "memory/h2_memory.hpp"               // failure, fp
#include "except/h2_debug.hpp"                // option
#include "except/h2_exception.hpp"            //
#include "stdio/h2_stdio.hpp"                 //
#include "net/h2_dns.hpp"                     //
#include "net/h2_socket.hpp"                  // stub, failure, matcher
#include "assert/h2_assert.hpp"               // failure, matcher
#include "assert/h2_timer.hpp"                // failure
}

#ifndef H2_NO_SUITE
#define SUITE H2SUITE
#endif

#ifndef H2_NO_CASE
#define CASE H2CASE
#endif

#ifndef H2_NO_TODO
#define TODO H2TODO
#endif

#ifndef H2_NO_Case
#define Case H2Case
#endif

#ifndef H2_NO_Todo
#define Todo H2Todo
#endif

#ifndef H2_NO_Setup
#define Setup H2Setup
#endif

#ifndef H2_NO_Teardown
#define Teardown H2Cleanup
#endif

#ifndef H2_NO_Cleanup
#define Cleanup H2Cleanup
#endif

#ifndef H2_NO_OK
#define OK H2OK
#endif

#ifndef H2_NO_JE
#define JE H2JE
#endif

#ifndef H2_NO_MOCK
#define MOCK H2MOCK
#endif

#ifndef H2_NO_UNMOCK
#define UNMOCK H2UNSTUB
#endif

#ifndef H2_NO_MOCKS
#define MOCKS H2MOCKS
#endif

#ifndef H2_NO_UNMOCKS
#define UNMOCKS H2UNSTUBS
#endif

#ifndef H2_NO_STUB
#define STUB H2STUB
#endif

#ifndef H2_NO_UNSTUB
#define UNSTUB H2UNSTUB
#endif

#ifndef H2_NO_STUBS
#define STUBS H2STUBS
#endif

#ifndef H2_NO_UNSTUBS
#define UNSTUBS H2UNSTUBS
#endif

#ifndef H2_NO_BLOCK
#define BLOCK H2BLOCK
#endif

#ifndef H2_NO_UNMEM
#define UNMEM H2UNMEM
#endif

#ifndef H2_NO_DNS
#define DNS H2DNS
#endif

#ifndef H2_NO_SOCK
#define SOCK H2SOCK
#endif

#ifndef H2_NO_COUT
#define COUT H2COUT
#endif

#ifndef H2_NO_PF
#define PF H2PF
#endif

#ifndef H2_NO_GlobalSetup
#define GlobalSetup H2GlobalSetup
#endif

#ifndef H2_NO_GlobalCleanup
#define GlobalCleanup H2GlobalCleanup
#endif

#ifndef H2_NO_GlobalSuiteSetup
#define GlobalSuiteSetup H2GlobalSuiteSetup
#endif

#ifndef H2_NO_GlobalSuiteCleanup
#define GlobalSuiteCleanup H2GlobalSuiteCleanup
#endif

#ifndef H2_NO_GlobalCaseSetup
#define GlobalCaseSetup H2GlobalCaseSetup
#endif

#ifndef H2_NO_GlobalCaseCleanup
#define GlobalCaseCleanup H2GlobalCaseCleanup
#endif

#ifndef H2_NO_MATCHER
#define MATCHER H2MATCHER
#endif

#ifndef H2_NO_CASES
#define CASES H2CASES
#endif

#ifndef H2_NO_CASESS
#define CASESS H2CASESS
#endif

#ifndef H2_NO_Cases
#define Cases H2Cases
#endif

#ifndef H2_NO_Casess
#define Casess H2Casess
#endif

#define Cases_t H2Cases_t
#define Casess_t H2Casess_t

#ifndef H2_NO_CASES_T
#define CASES_T H2CASES_T
#endif

#ifndef H2_NO_CASESS_T
#define CASESS_T H2CASESS_T
#endif

#ifndef H2_NO_Todos
#define Todos H2Todos
#endif

#ifndef H2_NO_Todoss
#define Todoss H2Todoss
#endif

#ifndef H2_NO_TODOS
#define TODOS H2TODOS
#endif

#ifndef H2_NO_TODOSS
#define TODOSS H2TODOSS
#endif

#ifndef H2_NO_TODOS_T
#define TODOS_T H2TODOS_T
#endif

#ifndef H2_NO_TODOSS_T
#define TODOSS_T H2TODOSS_T
#endif

#ifndef H2_NO_Catch
#define Catch H2Catch
#endif

using h2::_;
using h2::Any;
#ifndef H2_NO_Eq
#define Eq H2Eq
#endif
using h2::TypeEq;
using h2::Nq;
using h2::Ge;
using h2::Gt;
using h2::Le;
using h2::Lt;
#ifndef H2_NO_Me
#define Me H2Me
#endif
using h2::Range;
using h2::Re;
using h2::We;
using h2::Je;
using h2::Se;
using h2::Substr;
using h2::StartsWith;
using h2::EndsWith;
using h2::CaseLess;
using h2::SpaceLess;
using h2::Pointee;
using h2::Member;
using h2::CastOf;
using h2::Not;
using h2::Conditional;
using h2::operator&&;
using h2::operator||;
using h2::AllOf;
using h2::AnyOf;
using h2::NoneOf;
using h2::ListOf;
using h2::CountOf;
using h2::MaxOf;
using h2::MinOf;
using h2::AvgOf;
#define MeanOf AvgOf
using h2::MedianOf;
using h2::Has;
using h2::HasKey;
using h2::HasValue;
using h2::Pair;
using h2::nothrow;

#include "matcher/h2_matcher_aux.hpp"

#if defined __cplusplus && !defined H2_NO_Redefine_private
// clang11 bug, not works option: -fno-access-control -fno-no-access-control
#define private public
#define protected public
#endif

#endif
