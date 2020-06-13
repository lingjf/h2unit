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
#include "h2_routine.hpp"
#include "h2_mock.hpp"
#include "h2_stdio.hpp"
#include "h2_dns.hpp"
#include "h2_socket.hpp"
#include "h2_case.hpp"
#include "h2_suite.hpp"
#include "h2_report.hpp"
#include "h2_directory.hpp"
#include "h2_check.hpp"
#include "h2_generator.hpp"
#include "h2_patch.hpp"
#include "h2_task.hpp"
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

#ifndef OK
#   define OK H2OK
#else
#   pragma message("OK is already defined using H2OK instead")
#endif

#define JE(e, a) H2JE(e, a)
#define MOCK(...) H2MOCK(__VA_ARGS__)
#define STUB(...) H2STUB(__VA_ARGS__)

#ifndef BLOCK
#   define BLOCK H2BLOCK
#endif

#define DNS(...) H2DNS(__VA_ARGS__)

#ifndef SOCK
#   define SOCK H2SOCK
#endif

#ifndef COUT
#   define COUT H2COUT
#endif

#define MATCHER(...) H2MATCHER(__VA_ARGS__)

#define CASES(name, ...) H2CASES(#name, __VA_ARGS__)
#define CASESS(name, ...) H2CASESS(#name, __VA_ARGS__)
#define Cases(name, ...) H2Cases(#name, __VA_ARGS__)
#define Casess(name, ...) H2Casess(#name, __VA_ARGS__)

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
   static void QP(h2::h2_suite* ________suite, h2::h2_case* _________case)

#define H2SUITE(name) __H2SUITE(name, H2Q(h2_suite_test))

#define __H2Cleanup()                      \
   if (::setjmp(________suite->jump) == 0) \
      ________suite->jumpable = true;      \
   if (!_________case)

#define H2Cleanup() __H2Cleanup()

#define __H2Case(name, status, Qc, Q1, Q2)                                       \
   static h2::h2_case Qc(name, __FILE__, __LINE__, status);                      \
   static h2::h2_suite::installer H2Q(installer)(________suite, &Qc);            \
   if (&Qc == _________case)                                                     \
      for (h2::h2_suite::cleaner Q1(________suite); Q1; _________case = nullptr) \
         for (h2::h2_case::cleaner Q2(&Qc); Q2;)                                 \
            if (::setjmp(Qc.jump) == 0)

#define H2Case(name) __H2Case(name, h2::h2_case::initial, H2Q(t_case), H2Q(_1), H2Q(_2))
#define H2Todo(name) __H2Case(name, h2::h2_case::todo, H2Q(t_case), H2Q(_1), H2Q(_2))

#define __H2CASE(name, status, QR, QP)                                     \
   static void QR();                                                       \
   static void QP(h2::h2_suite* ________suite, h2::h2_case* _________case) \
   {                                                                       \
      static h2::h2_case c(name, __FILE__, __LINE__, status);              \
      static h2::h2_suite::installer i(________suite, &c);                 \
      if (&c == _________case)                                             \
         for (h2::h2_case::cleaner a(&c); a;)                              \
            if (::setjmp(c.jump) == 0)                                     \
               QR();                                                       \
   }                                                                       \
   static h2::h2_suite H2Q(suite)("Anonymous", &QP, __FILE__, __LINE__);   \
   static void QR()

#define H2CASE(name) __H2CASE(name, h2::h2_case::initial, H2Q(h2_case_test_code), H2Q(h2_suite_test_code))
#define H2TODO(name) __H2CASE(name, h2::h2_case::todo, H2Q(h2_case_test_code), H2Q(h2_suite_test_code))

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
