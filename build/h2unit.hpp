
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#ifndef __H2UNIT_HPP__
#define __H2UNIT_HPP__
#define H2UNIT_VERSION 5.17
#define H2UNIT_DATE 2022-01-08
#define H2UNIT_REVISION branches/v5/5a55c718bc22ba52bd4500997b2df18939996efa
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

#if defined _MSC_VER || defined __CYGWIN__
#include <iomanip> /* std::fixed, std::setprecision */
#include <limits>  /* std::numeric_limits */
#endif

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
// source/utils/h2_macro.hpp
/* clang-format off */

#define H2PP_STR(...) _H2PP_STR(__VA_ARGS__)
#define _H2PP_STR(...) #__VA_ARGS__

#define H2PP_CAT(_1, _2) _H2PP_CAT2(_1, _2)
#define H2PP_CAT2(_1, _2) _H2PP_CAT2(_1, _2)
#define _H2PP_CAT2(_1, _2) _1##_2
#define H2PP_CAT5(_1, _2, _3, _4, _5) _H2PP_CAT5(_1, _2, _3, _4, _5)
#define _H2PP_CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5

#define H2PP_COMMA(...) ,
#define H2PP_EMPTY(...)
#define H2PP_DEFER(...) __VA_ARGS__ H2PP_EMPTY()
#define H2PP_RESCAN(_1) _1

// MSVC workaround: Calling arguments not equal with callee parameters
#define H2PP_PROXY(_Macro, _Args) _H2PP_PROXY(_Macro, _Args)
#define H2PP_PROXY2(_Macro, _Spec, _Args) _H2PP_PROXY(_H2PP_CAT2(_Macro, _Spec), _Args)
#define _H2PP_PROXY(_Macro, _Args) H2PP_RESCAN(_Macro _Args)

#ifdef _MSC_VER  // MSVC workaround
#   define H2PP_IS_PROBE(...) H2PP_PROXY(_H2PP_TH1, (__VA_ARGS__, 0))
#else
#   define H2PP_IS_PROBE(...) _H2PP_TH1(__VA_ARGS__, 0, )
#endif

#define H2PP_PROBE() ~, 1,

#define H2PP_EVAL(...) _H2PP_EVAL_128(__VA_ARGS__)

#define H2PP_EQ(_1, _2) H2PP_IS_PROBE(H2PP_CAT5(_H2PP_EQ, __, _1, __, _2))

#define H2PP_NOT(_Cond) H2PP_IS_PROBE(_H2PP_CAT2(_H2PP_NOT_, _Cond))
#define _H2PP_NOT_0 H2PP_PROBE()

#define H2PP_COMPL(_Value) _H2PP_CAT2(_H2PP_COMPL_, _Value)
#define _H2PP_COMPL_0 1
#define _H2PP_COMPL_1 0

#define H2PP_BOOL(_Cond) H2PP_COMPL(H2PP_NOT(_Cond))  // non-zero evaluate to 1

#define H2PP_AND(_1, _2) H2PP_CAT2(_H2PP_AND_, H2PP_BOOL(_1)) (H2PP_BOOL(_2))
#define _H2PP_AND_0(_2) 0
#define _H2PP_AND_1(_2) _2

#define H2PP_OR(_1, _2) H2PP_CAT2(_H2PP_OR_, H2PP_BOOL(_1)) (H2PP_BOOL(_2))
#define _H2PP_OR_0(_2) _2
#define _H2PP_OR_1(_2) 1

#define H2PP_IF_ELSE(_Cond, _Then, _Else) H2PP_CAT2(_H2PP_IF_ELSE_, _Cond) (_Then, _Else)
#define _H2PP_IF_ELSE_1(_Then, _Else) _Then
#define _H2PP_IF_ELSE_0(_Then, _Else) _Else

#define H2PP_IF(_Cond) H2PP_CAT2(_H2PP_IF_, H2PP_BOOL(_Cond))
#define _H2PP_IF_1(...) __VA_ARGS__
#define _H2PP_IF_0(...)

// the first argument starts with something in parentheses
#define H2PP_IS_BEGIN_PARENTHESIS(...) H2PP_IS_PROBE(_H2PP_IS_BEGIN_PARENTHESIS_PROBE __VA_ARGS__)
#define _H2PP_IS_BEGIN_PARENTHESIS_PROBE(...) H2PP_PROBE()

// only one argument and it is enclosed in parentheses
#define H2PP_IS_PARENTHESIS(...) H2PP_IF_ELSE(H2PP_IS_BEGIN_PARENTHESIS(__VA_ARGS__), H2PP_IS_EMPTY(H2PP_EMPTY __VA_ARGS__), 0)

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define H2PP_REMOVE_PARENTHESES_IF(...) H2PP_CAT2(_H2PP_REMOVE_PARENTHESES_IF_, H2PP_IS_PARENTHESIS(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_1(...) H2PP_REMOVE_PARENTHESES(__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_0(...) __VA_ARGS__

// 1. __VA_ARGS__ has no unparen'd commas. ==> 0
// 2. __VA_ARGS__ is not enclosed in parenthesis. ==> 0
// 3. __VA_ARGS__ is not a macro that ()-evaluates to a comma. ==> 0
// 4. __VA_ARGS__ is empty, or has unparen'd commas, or is enclosed in parenthesis, or is a macro that ()-evaluates to comma. ==> 1
#define H2PP_IS_EMPTY(...) _H2PP_IS_EMPTY(H2PP_HAS_COMMA(__VA_ARGS__),            \
                                          H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__), \
                                          H2PP_HAS_COMMA(__VA_ARGS__()),          \
                                          H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__()))
#define _H2PP_IS_EMPTY(_1, _2, _3, _4) H2PP_HAS_COMMA(_H2PP_CAT5(_H2PP_IS_EMPTY_CASE_, _1, _2, _3, _4))
#define _H2PP_IS_EMPTY_CASE_0001 ,

#define H2PP_NARG(...) H2PP_CAT(_H2PP_NARG_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_NARG_0(...) H2PP_ARG_COUNT(__VA_ARGS__)
#define _H2PP_NARG_1(...) 0

#define H2PP_NCALL(_Macro, N, ...) H2PP_RESCAN(H2PP_CAT2(_Macro, H2PP_EQ(N, H2PP_NARG(__VA_ARGS__))) (__VA_ARGS__))
#define H2PP_VCALL(_Macro, ...) H2PP_RESCAN(H2PP_CAT2(_Macro, H2PP_NARG(__VA_ARGS__)) (__VA_ARGS__))

#define H2PP_HEAD(...) H2PP_CAT2(_H2PP_HEAD_, H2PP_IS_EMPTY(__VA_ARGS__)) ((__VA_ARGS__))
#define _H2PP_HEAD_0(MSVC_Workaround) H2PP_RESCAN(_H2PP_HEAD_0_R MSVC_Workaround)
#define _H2PP_HEAD_0_R(_1, ...) _1
#define _H2PP_HEAD_1(...)

#define H2PP_TAIL(...) H2PP_CAT2(_H2PP_TAIL_, H2PP_IS_EMPTY(__VA_ARGS__)) ((__VA_ARGS__))
#define _H2PP_TAIL_0(MSVC_Workaround) H2PP_RESCAN(_H2PP_TAIL_0_R MSVC_Workaround)
#define _H2PP_TAIL_0_R(_1, ...) __VA_ARGS__
#define _H2PP_TAIL_1(...)

#define H2PP_LAST(...) H2PP_EVAL(_H2PP_LAST(__VA_ARGS__))
#define _H2PP_LAST(...) H2PP_CAT2(_H2PP_LAST_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__) //is empty?
#define _H2PP_LAST_1(...) // 0 argument
#define _H2PP_LAST_0(...) H2PP_CAT2(_H2PP_LAST_0_, H2PP_EQ(1, H2PP_NARG(__VA_ARGS__))) (__VA_ARGS__) // at least 1 argument, one and only one?
#define _H2PP_LAST_0_1(...) __VA_ARGS__ // only 1 argument
#define _H2PP_LAST_0_0(...) H2PP_DEFER(__H2PP_LAST)()(H2PP_TAIL(__VA_ARGS__)) // shift first, and again
#define __H2PP_LAST() _H2PP_LAST_0

#define H2PP_REPEAT(_Split, _Macro, _Args, _Count) H2PP_EVAL(_H2PP_REPEAT(_Split, _Macro, _Args, _Count))
#define _H2PP_REPEAT(_Split, _Macro, _Args, _Count) H2PP_CAT2(_H2PP_REPEAT_, H2PP_BOOL(_Count)) (_Split, _Macro, _Args, _Count)
#define _H2PP_REPEAT_0(...)
#define _H2PP_REPEAT_1(_Split, _Macro, _Args, _Count) H2PP_DEFER(__H2PP_REPEAT)()(_Split, _Macro, _Args, H2PP_CAT2(H2PP_DEC_, _Count)) _H2PP_REPEAT_CALL_MACRO(_Split, _Macro, _Args, H2PP_CAT2(H2PP_DEC_, _Count))
#define __H2PP_REPEAT() _H2PP_REPEAT
#define _H2PP_REPEAT_CALL_MACRO(_Split, _Macro, _Args, _i) H2PP_CAT2(_H2PP_REPEAT_CALL_MACRO_, H2PP_EQ(0, _i)) (_Split, _Macro, _Args, _i)
#define _H2PP_REPEAT_CALL_MACRO_1(_Split, _Macro, _Args, _i) _Macro(_Args, _i)
#define _H2PP_REPEAT_CALL_MACRO_0(_Split, _Macro, _Args, _i) H2PP_REMOVE_PARENTHESES_IF(_Split) _Macro(_Args, _i)

#define H2PP_FOREACH(_Split, _Macro, _Args, ...) H2PP_EVAL(_H2PP_FOREACH(_Split, _Macro, _Args, 0, __VA_ARGS__))
#define _H2PP_FOREACH(_Split, _Macro, _Args, _i, ...) H2PP_CAT2(_H2PP_FOREACH_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Split, _Macro, _Args, _i, __VA_ARGS__)
#define _H2PP_FOREACH_1(...)
#define _H2PP_FOREACH_0(_Split, _Macro, _Args, _i, ...) _H2PP_FOREACH_CALL_MACRO(_Split, _Macro, _Args, _i, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(__H2PP_FOREACH)()(_Split, _Macro, _Args, H2PP_CAT2(H2PP_INC_, _i), H2PP_TAIL(__VA_ARGS__))
#define __H2PP_FOREACH() _H2PP_FOREACH
#define _H2PP_FOREACH_CALL_MACRO(_Split, _Macro, _Args, _i, _x) H2PP_CAT2(_H2PP_FOREACH_CALL_MACRO_, H2PP_EQ(0, _i)) (_Split, _Macro, _Args, _i, _x)
#define _H2PP_FOREACH_CALL_MACRO_1(_Split, _Macro, _Args, _i, _x) _Macro(_Args, _i, _x)
#define _H2PP_FOREACH_CALL_MACRO_0(_Split, _Macro, _Args, _i, _x) H2PP_REMOVE_PARENTHESES_IF(_Split) _Macro(_Args, _i, _x)

#define H2PP_FULLMESH(_Split, _Macro, _Args, ...) H2PP_RESCAN(H2PP_CAT2(_H2PP_FULLMESH1_, H2PP_IS_EMPTY(__VA_ARGS__)) ((_Split, _Macro, _Args, __VA_ARGS__)))
#define _H2PP_FULLMESH1_1(...)
#define _H2PP_FULLMESH1_0(MSVC_Workaround) H2PP_RESCAN(H2PP_FULLMESH1 MSVC_Workaround)
#define H2PP_FULLMESH1(_Split, _Macro, _Args, _1, ...) H2PP_CAT2(_H2PP_FULLMESH2_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Split, _Macro, _Args, _1, __VA_ARGS__)
#define _H2PP_FULLMESH2_1(_Split, _Macro, _Args, _1, ...) H2PP_FULLMESH2(_Split, _Macro, _Args, _1, _1)  // Fullmesh((1,2,3)) => (1,2,3) x (1,2,3)
#define _H2PP_FULLMESH2_0(...) _H2PP_FULLMESH2_0_R((__VA_ARGS__))  // Fullmesh((1,2,3), (4,5,6)) => (1,2,3) x (4,5,6)
#define _H2PP_FULLMESH2_0_R(MSVC_Workaround) H2PP_RESCAN(H2PP_FULLMESH2 MSVC_Workaround)
#define H2PP_FULLMESH2(_Split, _Macro, _Args, _Tuplex, _Tupley) H2PP_EVAL(_H2PP_FOREACHx(_Split, _Macro, _Args, 0, _Tupley, H2PP_REMOVE_PARENTHESES(_Tuplex)))
#define _H2PP_FOREACHx(_Split, _Macro, _Args, _i, _Tupley, ...) H2PP_CAT2(_H2PP_FOREACHx_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Split, _Macro, _Args, _i, _Tupley, __VA_ARGS__)
#define _H2PP_FOREACHx_1(...)
#define _H2PP_FOREACHx_0(_Split, _Macro, _Args, _i, _Tupley, ...) _H2PP_FOREACHy(_Split, _Macro, _Args, _i, 0, H2PP_HEAD(__VA_ARGS__), H2PP_REMOVE_PARENTHESES(_Tupley)) H2PP_DEFER(__H2PP_FOREACHx)()(_Split, _Macro, _Args, H2PP_CAT2(H2PP_INC_, _i), _Tupley, H2PP_TAIL(__VA_ARGS__))
#define __H2PP_FOREACHx() _H2PP_FOREACHx
#define _H2PP_FOREACHy(_Split, _Macro, _Args, _i, _j, _x, ...) H2PP_CAT2(_H2PP_FOREACHy_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Split, _Macro, _Args, _i, _j, _x, __VA_ARGS__)
#define _H2PP_FOREACHy_1(...)
#define _H2PP_FOREACHy_0(_Split, _Macro, _Args, _i, _j, _x, ...) _H2PP_FULLMESH_CALL_MACRO(_Split, _Macro, _Args, _i, _j, _x, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(__H2PP_FOREACHy)()(_Split, _Macro, _Args, _i, H2PP_CAT2(H2PP_INC_, _j), _x, H2PP_TAIL(__VA_ARGS__))
#define __H2PP_FOREACHy() _H2PP_FOREACHy
#define _H2PP_FULLMESH_CALL_MACRO(_Split, _Macro, _Args, _i, _j, _x, _y) H2PP_CAT2(_H2PP_FULLMESH_CALL_MACRO_, H2PP_AND(H2PP_EQ(0, _i), H2PP_EQ(0, _j))) (_Split, _Macro, _Args, _i, _j, _x, _y)
#define _H2PP_FULLMESH_CALL_MACRO_1(_Split, _Macro, _Args, _i, _j, _x, _y) _Macro(_Args, _i, _j, _x, _y)
#define _H2PP_FULLMESH_CALL_MACRO_0(_Split, _Macro, _Args, _i, _j, _x, _y) H2PP_REMOVE_PARENTHESES_IF(_Split) _Macro(_Args, _i, _j, _x, _y)

#define H2PP_UNIQUE(...) H2PP_CAT5(h2_, __VA_ARGS__, __COUNTER__, L, __LINE__)  // generate unique identifier [with(out) prefix]
#define H2PP_DEBUG(...) ::printf("%s\n", #__VA_ARGS__);
// source/utils/h2_macro.in.hpp
//////// >>>>> generated by build/generate.py  DON'T EDIT
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
#define _H2PP_EQ__16__16 H2PP_PROBE()
#define _H2PP_EQ__17__17 H2PP_PROBE()
#define _H2PP_EQ__18__18 H2PP_PROBE()
#define _H2PP_EQ__19__19 H2PP_PROBE()
#define _H2PP_EQ__20__20 H2PP_PROBE()
#define _H2PP_EQ__21__21 H2PP_PROBE()
#define _H2PP_EQ__22__22 H2PP_PROBE()
#define _H2PP_EQ__23__23 H2PP_PROBE()
#define _H2PP_EQ__24__24 H2PP_PROBE()
#define _H2PP_EQ__25__25 H2PP_PROBE()
#define _H2PP_EQ__26__26 H2PP_PROBE()
#define _H2PP_EQ__27__27 H2PP_PROBE()
#define _H2PP_EQ__28__28 H2PP_PROBE()
#define _H2PP_EQ__29__29 H2PP_PROBE()
#define _H2PP_EQ__30__30 H2PP_PROBE()
#define _H2PP_EQ__31__31 H2PP_PROBE()

#define _H2PP_TH0(_0, ...) _0
#define _H2PP_TH1(_0, _1, ...) _1
#define _H2PP_TH2(_0, _1, _2, ...) _2
#define _H2PP_TH3(_0, _1, _2, _3, ...) _3
#define _H2PP_TH4(_0, _1, _2, _3, _4, ...) _4
#define _H2PP_TH5(_0, _1, _2, _3, _4, _5, ...) _5
#define _H2PP_TH6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define _H2PP_TH7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define _H2PP_TH8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define _H2PP_TH9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define _H2PP_TH10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define _H2PP_TH11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define _H2PP_TH12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define _H2PP_TH13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define _H2PP_TH14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define _H2PP_TH15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define _H2PP_TH16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define _H2PP_TH17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _17
#define _H2PP_TH18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) _18
#define _H2PP_TH19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) _19
#define _H2PP_TH20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, ...) _20
#define _H2PP_TH21(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, ...) _21
#define _H2PP_TH22(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, ...) _22
#define _H2PP_TH23(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, ...) _23
#define _H2PP_TH24(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, ...) _24
#define _H2PP_TH25(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, ...) _25
#define _H2PP_TH26(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, ...) _26
#define _H2PP_TH27(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, ...) _27
#define _H2PP_TH28(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, ...) _28
#define _H2PP_TH29(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, ...) _29
#define _H2PP_TH30(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, ...) _30
#define _H2PP_TH31(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, ...) _31
#define H2PP_TH0(...) H2PP_PROXY2(_H2PP_TH, 0, (__VA_ARGS__, ))
#define H2PP_TH1(...) H2PP_PROXY2(_H2PP_TH, 1, (__VA_ARGS__, , ))
#define H2PP_TH2(...) H2PP_PROXY2(_H2PP_TH, 2, (__VA_ARGS__, , , ))
#define H2PP_TH3(...) H2PP_PROXY2(_H2PP_TH, 3, (__VA_ARGS__, , , , ))
#define H2PP_TH4(...) H2PP_PROXY2(_H2PP_TH, 4, (__VA_ARGS__, , , , , ))
#define H2PP_TH5(...) H2PP_PROXY2(_H2PP_TH, 5, (__VA_ARGS__, , , , , , ))
#define H2PP_TH6(...) H2PP_PROXY2(_H2PP_TH, 6, (__VA_ARGS__, , , , , , , ))
#define H2PP_TH7(...) H2PP_PROXY2(_H2PP_TH, 7, (__VA_ARGS__, , , , , , , , ))
#define H2PP_TH8(...) H2PP_PROXY2(_H2PP_TH, 8, (__VA_ARGS__, , , , , , , , , ))
#define H2PP_TH9(...) H2PP_PROXY2(_H2PP_TH, 9, (__VA_ARGS__, , , , , , , , , , ))
#define H2PP_TH10(...) H2PP_PROXY2(_H2PP_TH, 10, (__VA_ARGS__, , , , , , , , , , , ))
#define H2PP_TH11(...) H2PP_PROXY2(_H2PP_TH, 11, (__VA_ARGS__, , , , , , , , , , , , ))
#define H2PP_TH12(...) H2PP_PROXY2(_H2PP_TH, 12, (__VA_ARGS__, , , , , , , , , , , , , ))
#define H2PP_TH13(...) H2PP_PROXY2(_H2PP_TH, 13, (__VA_ARGS__, , , , , , , , , , , , , , ))
#define H2PP_TH14(...) H2PP_PROXY2(_H2PP_TH, 14, (__VA_ARGS__, , , , , , , , , , , , , , , ))
#define H2PP_TH15(...) H2PP_PROXY2(_H2PP_TH, 15, (__VA_ARGS__, , , , , , , , , , , , , , , , ))
#define H2PP_TH16(...) H2PP_PROXY2(_H2PP_TH, 16, (__VA_ARGS__, , , , , , , , , , , , , , , , , ))
#define H2PP_TH17(...) H2PP_PROXY2(_H2PP_TH, 17, (__VA_ARGS__, , , , , , , , , , , , , , , , , , ))
#define H2PP_TH18(...) H2PP_PROXY2(_H2PP_TH, 18, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH19(...) H2PP_PROXY2(_H2PP_TH, 19, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH20(...) H2PP_PROXY2(_H2PP_TH, 20, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH21(...) H2PP_PROXY2(_H2PP_TH, 21, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH22(...) H2PP_PROXY2(_H2PP_TH, 22, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH23(...) H2PP_PROXY2(_H2PP_TH, 23, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH24(...) H2PP_PROXY2(_H2PP_TH, 24, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH25(...) H2PP_PROXY2(_H2PP_TH, 25, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH26(...) H2PP_PROXY2(_H2PP_TH, 26, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH27(...) H2PP_PROXY2(_H2PP_TH, 27, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH28(...) H2PP_PROXY2(_H2PP_TH, 28, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH29(...) H2PP_PROXY2(_H2PP_TH, 29, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH30(...) H2PP_PROXY2(_H2PP_TH, 30, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH31(...) H2PP_PROXY2(_H2PP_TH, 31, (__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))

#define H2PP_INC_0 1
#define H2PP_INC_1 2
#define H2PP_INC_2 3
#define H2PP_INC_3 4
#define H2PP_INC_4 5
#define H2PP_INC_5 6
#define H2PP_INC_6 7
#define H2PP_INC_7 8
#define H2PP_INC_8 9
#define H2PP_INC_9 10
#define H2PP_INC_10 11
#define H2PP_INC_11 12
#define H2PP_INC_12 13
#define H2PP_INC_13 14
#define H2PP_INC_14 15
#define H2PP_INC_15 16
#define H2PP_INC_16 17
#define H2PP_INC_17 18
#define H2PP_INC_18 19
#define H2PP_INC_19 20
#define H2PP_INC_20 21
#define H2PP_INC_21 22
#define H2PP_INC_22 23
#define H2PP_INC_23 24
#define H2PP_INC_24 25
#define H2PP_INC_25 26
#define H2PP_INC_26 27
#define H2PP_INC_27 28
#define H2PP_INC_28 29
#define H2PP_INC_29 30
#define H2PP_INC_30 31
#define H2PP_INC_31 32
#define H2PP_DEC_1 0
#define H2PP_DEC_2 1
#define H2PP_DEC_3 2
#define H2PP_DEC_4 3
#define H2PP_DEC_5 4
#define H2PP_DEC_6 5
#define H2PP_DEC_7 6
#define H2PP_DEC_8 7
#define H2PP_DEC_9 8
#define H2PP_DEC_10 9
#define H2PP_DEC_11 10
#define H2PP_DEC_12 11
#define H2PP_DEC_13 12
#define H2PP_DEC_14 13
#define H2PP_DEC_15 14
#define H2PP_DEC_16 15
#define H2PP_DEC_17 16
#define H2PP_DEC_18 17
#define H2PP_DEC_19 18
#define H2PP_DEC_20 19
#define H2PP_DEC_21 20
#define H2PP_DEC_22 21
#define H2PP_DEC_23 22
#define H2PP_DEC_24 23
#define H2PP_DEC_25 24
#define H2PP_DEC_26 25
#define H2PP_DEC_27 26
#define H2PP_DEC_28 27
#define H2PP_DEC_29 28
#define H2PP_DEC_30 29
#define H2PP_DEC_31 30
#define H2PP_DEC_32 31

#define _H2PP_EVAL_128(...) _H2PP_EVAL_64(_H2PP_EVAL_64(__VA_ARGS__))
#define _H2PP_EVAL_64(...) _H2PP_EVAL_32(_H2PP_EVAL_32(__VA_ARGS__))
#define _H2PP_EVAL_32(...) _H2PP_EVAL_16(_H2PP_EVAL_16(__VA_ARGS__))
#define _H2PP_EVAL_16(...) _H2PP_EVAL_8(_H2PP_EVAL_8(__VA_ARGS__))
#define _H2PP_EVAL_8(...) _H2PP_EVAL_4(_H2PP_EVAL_4(__VA_ARGS__))
#define _H2PP_EVAL_4(...) _H2PP_EVAL_2(_H2PP_EVAL_2(__VA_ARGS__))
#define _H2PP_EVAL_2(...) _H2PP_EVAL_1(_H2PP_EVAL_1(__VA_ARGS__))
#define _H2PP_EVAL_1(...) __VA_ARGS__

#define _H2PP_64TH(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, ...) _64
#define H2PP_HAS_COMMA(...) H2PP_PROXY(_H2PP_64TH, (__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))
#define H2PP_ARG_COUNT(...) H2PP_PROXY(_H2PP_64TH, (__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

//////// <<<<< generated by build/generate.py
// source/utils/h2_array.hpp
template <typename T>
struct h2_array {
   typedef typename std::decay<T>::type value_type;
   typedef value_type* pointer;
   typedef const value_type* const_pointer;
   typedef value_type& reference;
   typedef const value_type& const_reference;
   typedef value_type* iterator;
   typedef const value_type* const_iterator;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;

   value_type* elems;
   std::size_t count;
   h2_array(const value_type* elems_, const std::size_t count_) : elems((value_type*)elems_), count(count_) {}

   iterator begin() noexcept { return iterator(data()); }
   const_iterator begin() const noexcept { return const_iterator(data()); }
   iterator end() noexcept { return iterator(data() + count); }
   const_iterator end() const noexcept { return const_iterator(data() + count); }
   const_iterator cbegin() const noexcept { return const_iterator(data()); }
   const_iterator cend() const noexcept { return const_iterator(data() + count); }

   constexpr size_type size() const noexcept { return count; }
   constexpr size_type max_size() const noexcept { return count; }
   constexpr bool empty() const noexcept { return size() == 0; }

   reference operator[](size_type __n) noexcept { return elems[__n]; }
   constexpr const_reference operator[](size_type __n) const noexcept { return elems[__n]; }
   reference at(size_type __n) { return elems[__n]; }
   constexpr const_reference at(size_type __n) const { return elems[__n]; }
   reference front() noexcept { return *begin(); }
   constexpr const_reference front() const noexcept { return elems[0]; }
   reference back() noexcept { return count ? *(end() - 1) : *end(); }
   constexpr const_reference back() const noexcept { return count ? elems[count - 1] : elems[0]; }

   pointer data() noexcept { return elems; }
   const_pointer data() const noexcept { return elems; }
};
// source/utils/h2_template.hpp
template <typename T> struct h2_type_identity { typedef T type; }; // std::type_identity
template <typename...> struct h2_true_type : std::true_type {};

template <typename T, typename = void>
struct h2_is_smart_ptr : std::false_type {};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::shared_ptr<typename T::element_type>>::value>::type> : std::true_type {};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::unique_ptr<typename T::element_type>>::value>::type> : std::true_type {};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::weak_ptr<typename T::element_type>>::value>::type> : std::true_type {};

template <typename U, typename = void>
struct h2_decay_impl : h2_type_identity<U> {};
template <>
struct h2_decay_impl<char*> : h2_type_identity<const char*> {};
template <typename U>
struct h2_decay_impl<U, typename std::enable_if<std::is_enum<U>::value>::type> : h2_type_identity<typename std::underlying_type<U>::type> {};
template <typename T>
struct h2_decay : h2_decay_impl<typename std::decay<T>::type> {};

template <std::size_t I, typename T, typename... S>
struct h2_nth_type_impl : h2_type_identity<typename h2_nth_type_impl<I - 1, S...>::type> {};
template <typename T, typename... S>
struct h2_nth_type_impl<0, T, S...> : h2_type_identity<T> {};
template <std::size_t I, typename... S>
struct h2_nth_type : h2_type_identity<typename h2_nth_type_impl<I, S..., int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type> {};

template <std::size_t I, typename... S>
using h2_nth_decay = typename h2_decay<typename h2_nth_type<I, S...>::type>::type;

template <typename T, typename = void>
struct h2_sizeof_pointee : std::integral_constant<std::size_t, sizeof(typename std::remove_pointer<T>::type)> {};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<std::is_void<typename std::remove_pointer<T>::type>::value>::type> : std::integral_constant<std::size_t, 1> {};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<h2_is_smart_ptr<T>::value>::type> : std::integral_constant<std::size_t, sizeof(typename T::element_type)> {};  // smart ptr not hold void*

template <typename T>
struct h2_pointee_type : h2_type_identity<typename T::element_type> {};
template <typename T>
struct h2_pointee_type<T*> : h2_type_identity<T> {};

template <typename T>
inline T* h2_pointer_if(T* a) { return a; }
template <typename T>
inline auto h2_pointer_if(T& a) -> typename std::enable_if<h2_is_smart_ptr<T>::value, typename T::element_type*>::type { return a.get(); }
template <typename T>
inline auto h2_pointer_if(T& a) -> typename std::enable_if<!h2_is_smart_ptr<T>::value, T*>::type { return &a; }

template <typename T>
struct h2_is_pair : std::false_type {};
template <typename K, typename V>
struct h2_is_pair<std::pair<K, V>> : std::true_type {};

template <typename T, typename = void>
struct h2_is_iterable : std::false_type {};
template <typename T>
struct h2_is_iterable<T,
                      typename std::enable_if<h2_true_type<typename T::value_type,
                                                           typename T::size_type,
                                                           typename T::iterator,
                                                           typename T::const_iterator,
                                                           decltype(std::declval<T>().begin()),
                                                           decltype(std::declval<T>().end()),
                                                           decltype(std::declval<T>().cbegin()),
                                                           decltype(std::declval<T>().cend())>::value>::type> : std::true_type {};

template <typename T, typename = void>
struct h2_has_substr : std::false_type {};
template <typename T>
struct h2_has_substr<T, typename std::enable_if<h2_true_type<decltype(std::declval<T>().substr())>::value>::type> : std::true_type {};

template <typename T> struct h2_is_container : std::conditional<h2_is_iterable<T>::value && !h2_has_substr<T>::value, std::true_type, std::false_type>::type {};
template <typename T> struct h2_is_string : std::conditional<h2_is_iterable<T>::value && h2_has_substr<T>::value, std::true_type, std::false_type>::type {};

template <typename T, typename = void>
struct h2_is_map : std::false_type {};
template <typename T>
struct h2_is_map<T, typename std::enable_if<h2_is_iterable<T>::value && h2_true_type<typename T::mapped_type>::value>::type> : std::true_type {};

template <typename T, typename = void>
struct h2_is_container_adaptor : std::false_type {};
template <typename T>
struct h2_is_container_adaptor<T, typename std::enable_if<h2_true_type<typename T::value_type, typename T::size_type, typename T::container_type>::value>::type> : std::true_type {};

template <typename ContainerAdaptor>
const typename ContainerAdaptor::container_type& underlying_container(const ContainerAdaptor& ca)
{
   struct AntiProtected : ContainerAdaptor {
      static const typename ContainerAdaptor::container_type& get(const ContainerAdaptor& ca) { return ca.*&AntiProtected::c; }
   };
   return AntiProtected::get(ca);
}

#if __cplusplus >= 201402L || (defined _MSVC_LANG && _MSVC_LANG >= 201402L)
#define h2_index_sequence std::index_sequence
#define h2_make_index_sequence std::make_index_sequence
#else
template <std::size_t...> struct h2_index_sequence {};
template <std::size_t N, std::size_t... S> struct h2_make_index_sequence : h2_make_index_sequence<N - 1, N - 1, S...> {};
template <std::size_t... S> struct h2_make_index_sequence<0, S...> : h2_index_sequence<S...> {};
#endif
// source/utils/h2_numberfy.hpp

template <typename T, typename = void>
struct h2_numberfy_impl {
   static std::uintptr_t cast(T a) { return a; }
};
template <typename T>
struct h2_numberfy_impl<T, typename std::enable_if<std::is_pointer<T>::value>::type> {
   static std::uintptr_t cast(T a) { return reinterpret_cast<std::uintptr_t>(a); }  // https://stackoverflow.com/questions/153065/converting-a-pointer-into-an-integer
};
template <typename T>
struct h2_numberfy_impl<T, typename std::enable_if<std::is_member_pointer<T>::value>::type> {  // non-static member object or non-static member function
   static std::uintptr_t cast(T a)
   {
      union {
         T a;
         std::uintptr_t b;
      } u;
      u.a = a;
      return u.b;
   }
};
template <typename T>
struct h2_numberfy_impl<T, typename std::enable_if<h2_is_smart_ptr<T>::value>::type> {
   static std::uintptr_t cast(T a) { return h2_numberfy_impl<typename T::element_type*>::cast(a.get()); }
};
template <>
struct h2_numberfy_impl<std::nullptr_t> {
   static std::uintptr_t cast(std::nullptr_t) { return 0; }
};

template <typename R, typename F>
static R h2_numberfy(F f) { return (R)h2_numberfy_impl<F>::cast(f); }
// template function can't partial specialization, redirect to template class partial specialization
// source/utils/h2_list.hpp
#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))

#define h2_list_for_each_entry(...) H2PP_VCALL(__h2_list_for_each_entry, H2PP_UNIQUE(t), __VA_ARGS__)
#define __h2_list_for_each_entry5(t, p, head, type, link) \
   for (type* p = h2_list_entry((head).next, type, link), *t = h2_list_entry(p->link.next, type, link); &p->link != &(head); p = t, t = h2_list_entry(t->link.next, type, link))
#define __h2_list_for_each_entry6(t, p, i, head, type, link) \
   for (int i = 0; !i; ++i)                                  \
      for (type* p = h2_list_entry((head).next, type, link), *t = h2_list_entry(p->link.next, type, link); &p->link != &(head); p = t, t = h2_list_entry(t->link.next, type, link), ++i)
#define h2_list_for_each_reverse_entry(p, head, type, link) \
   for (type* p = h2_list_entry((head).prev, type, link), *t_ = h2_list_entry(p->link.prev, type, link); &p->link != &(head); p = t_, t_ = h2_list_entry(t_->link.prev, type, link))

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
// source/utils/h2_pattern.hpp

struct h2_pattern {
   static bool regex_match(const char* pattern, const char* subject, bool caseless = false);
   static bool wildcard_match(const char* pattern, const char* subject, bool caseless = false);
   static bool match(const char* pattern, const char* subject, bool caseless = false);
};
// source/utils/h2_misc.hpp
#define H2_FILINE __FILE__ ":" H2PP_STR(__LINE__)

struct h2_once {
   unsigned long long c = 0;
   operator bool() { return !c++; }
};
#define h2_once_if() for (static h2_once ___1_; ___1_;)

// #define M(...) func(#__VA_ARGS__, other)
// Unix M() ==> func("", other) stringify empty __VA_ARGS__ to "" string
// Windows M() ==> func(, other) stringify empty __VA_ARGS__ to empty
// #define M(...) func(ss(#__VA_ARGS__), other)
static inline const char* ss(const char* a = "") { return a ? a : ""; }
static inline const char* comma_if(bool a, const char* t = ", ", const char* f = "") { return a ? t : f; }
static inline const char* quote_if(bool a, const char* t = "\"", const char* f = "") { return a ? t : f; }

#define H2Foreach(Callback_x, ...) H2PP_FOREACH(, _H2ForeachMacro, (Callback_x), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__))
#define _H2ForeachMacro(_Args, i, x) H2PP_REMOVE_PARENTHESES(_Args)(x)

#define H2Fullmesh(Callback_x_y, ...) H2PP_FULLMESH(, _H2FullmeshMacro, (Callback_x_y), __VA_ARGS__)
#define _H2FullmeshMacro(_Args, i, j, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)

/* clang-format off */
#define h2_singleton(Class) static Class& I() { static Class i; return i; }
#define h2_array_append(Array, a) do { for (size_t i = 0; i < sizeof(Array) / sizeof(Array)[0] - 1; ++i) if (!(Array)[i]) { (Array)[i] = (a); break; } } while (0)
// source/utils/h2_libc.hpp
struct h2_libc {
   static void* malloc(size_t size);
   static void free(void* ptr);
   static void* operator new(std::size_t size) { return malloc(size); }
   static void operator delete(void* ptr) { free(ptr); }

   static int write(int fd, const void* buf, size_t count);
};
// source/utils/h2_shared_ptr.hpp
template <typename T>
struct h2_shared_ptr {
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

   void acquire(T* p, long* n)
   {
      pn = n;
      if (p) pn ? ++*pn : *(pn = (long*)h2_libc::malloc(sizeof(long))) = 1;
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
// source/utils/h2_allocate.hpp
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
   template <typename U> struct rebind {
      typedef h2_allocator<U> other;
   };
   template <typename U> h2_allocator(const h2_allocator<U>&) {}
   template <typename U> h2_allocator& operator=(const h2_allocator<U>&) { return *this; }
};

template <typename T> inline bool operator==(const h2_allocator<T>&, const h2_allocator<T>&) { return true; }
template <typename T> inline bool operator!=(const h2_allocator<T>&, const h2_allocator<T>&) { return false; }

template <typename T> using h2_vector = std::vector<T, h2_allocator<T>>;
using h2_ostringstream = std::basic_ostringstream<char, std::char_traits<char>, h2_allocator<char>>;
// source/utils/h2_string.hpp
struct h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
   h2_string() : basic_string() {}
   h2_string(const h2_string& s) : basic_string(s.data(), s.size()) {}
   h2_string(const std::string& s) : basic_string(s.data(), s.size()) {}
   template <typename... T>
   h2_string(const char* fmt, T... t) : basic_string() { sizeof...(T) ? sprintf(fmt, t...) : assign(fmt ? fmt : "(null)"); }
   h2_string(size_t n, const char* s) : basic_string(s, n) {}
   h2_string(size_t n, const char c) : basic_string(n, c) {}

   h2_string& operator=(const h2_string& s) { return assign(s.data(), s.size()), *this; }
   h2_string& operator=(const std::string& s) { return assign(s.data(), s.size()), *this; }
   h2_string& operator=(const char* s) { return assign(s ? s : "(null)"), *this; }
   h2_string& operator=(const char c) { return assign(1, c), *this; }

   h2_string& operator+=(const h2_string& s) { return append(s.data(), s.size()), *this; }
   h2_string& operator+=(const std::string& s) { return append(s.data(), s.size()), *this; }
   h2_string& operator+=(const char* s) { return append(s), *this; }
   h2_string& operator+=(const char c) { return push_back(c), *this; }

   operator unsigned char*() const { return (unsigned char*)data(); }

   h2_string& sprintf(const char* fmt, ...);
   h2_string& replace_all(const char* from, const char* to);

   size_t width(size_t fat = 2) const;
   bool equals(const h2_string& fulstr, bool caseless = false) const;
   bool contains(const h2_string& substr, bool caseless = false) const;
   bool startswith(const h2_string& prefix, bool caseless = false) const;
   bool endswith(const h2_string& suffix, bool caseless = false) const;

   bool enclosed(char left = '\"', char right = '\0') const;
   h2_string unenclose(char left = '\"', char right = '\0') const;

   h2_string escape(bool utf8 = false) const;
   h2_string unescape() const;
   h2_string trim() const;
   h2_string squash(bool quote = false) const;
   h2_string tolower() const;
   h2_string centre(size_t width) const;
   h2_vector<h2_string> disperse() const;
};

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }
// source/utils/h2_line.hpp
struct h2_line : h2_vector<h2_string> {
   h2_line() {}
   h2_line(const char* a) : h2_vector<h2_string>({a}) {}
   h2_line(const h2_string& a) : h2_vector<h2_string>({a}) {}
   h2_line(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   size_t width(bool ignore_indent = false) const;
   h2_line& indent(size_t n, const char c = ' ');
   h2_line& padding(size_t n, const char c = ' ');

   h2_line& printf(const char* style, const char* format, ...);
   h2_line& operator+=(const h2_line& line);
   h2_line& brush(const char* style);

   bool enclosed(char left = '\"', char right = '\0') const;
   bool has(const char* word) const;

   h2_line gray_quote() const;
   h2_line abbreviate(size_t width = 16, size_t tail = 0) const;
   h2_string string() const;

   static void samesizify(h2_line& a, h2_line& b);
};

inline h2_line operator+(const h2_line& a, const h2_line& b)  // implicit conversion const char* / h2_string
{
   h2_line line;
   line.insert(line.end(), a.begin(), a.end());
   line.insert(line.end(), b.begin(), b.end());
   return line;
}

static inline const h2_line color(const h2_line& line, const char* style) { return ("\033{" + h2_string(style) + "}") + line + "\033{reset}"; }
static inline const h2_line delta(const h2_line& line, const char* style) { return ("\033{+" + h2_string(style) + "}") + line + ("\033{-" + h2_string(style) + "}"); }
static inline const h2_line gray(const char* s) { return s && strlen(s) ? delta(s, "dark gray") : h2_line(); }

///// h2_lines
struct h2_lines : h2_vector<h2_line> {
   h2_lines() {}
   h2_lines(std::initializer_list<h2_line> il) : h2_vector<h2_line>(il) {}

   h2_lines& operator+=(const h2_lines& lines);

   size_t width() const;
   bool foldable(size_t width = 20) const;
   h2_line folds() const;

   h2_string string() const;

   void sequence(size_t indent = 0, size_t start = 0);
   static void samesizify(h2_lines& a, h2_lines& b);
};
// source/utils/h2_stringify.hpp
template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_line print(const T&, bool = false) { return "?"; }
};

#define H2_TOSTRING_ABLE(tostring)                                                                            \
   template <typename T> struct h2_##tostring##_able {                                                        \
      template <typename U> static auto return_type(U* u) -> decltype(u->tostring());                         \
      template <typename U> static void return_type(...);                                                     \
      static constexpr bool value = std::is_convertible<decltype(return_type<T>(nullptr)), h2_string>::value; \
   };

H2_TOSTRING_ABLE(tostring);
H2_TOSTRING_ABLE(toString);
H2_TOSTRING_ABLE(Tostring);
H2_TOSTRING_ABLE(ToString);
H2_TOSTRING_ABLE(to_string);

/* tostring() may not be mark const, remove cast const in T a; fix multi-tostring */
template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2::h2_tostring_able<T>::value || h2::h2_toString_able<T>::value || h2::h2_Tostring_able<T>::value || h2::h2_ToString_able<T>::value || h2::h2_to_string_able<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false) { return gray(quote_if(represent)) + print__tostring(a) + gray(quote_if(represent)); }
   template <typename U> static auto print__tostring(const U& a) -> typename std::enable_if<h2::h2_tostring_able<U>::value, h2_string>::type { return const_cast<U&>(a).tostring(); }
   template <typename U> static auto print__tostring(const U& a) -> typename std::enable_if<!h2::h2_tostring_able<U>::value, h2_string>::type { return print__toString(a); }
   template <typename U> static auto print__toString(const U& a) -> typename std::enable_if<h2::h2_toString_able<U>::value, h2_string>::type { return const_cast<U&>(a).toString(); }
   template <typename U> static auto print__toString(const U& a) -> typename std::enable_if<!h2::h2_toString_able<U>::value, h2_string>::type { return print__Tostring(a); }
   template <typename U> static auto print__Tostring(const U& a) -> typename std::enable_if<h2::h2_Tostring_able<U>::value, h2_string>::type { return const_cast<U&>(a).toString(); }
   template <typename U> static auto print__Tostring(const U& a) -> typename std::enable_if<!h2::h2_Tostring_able<U>::value, h2_string>::type { return print__ToString(a); }
   template <typename U> static auto print__ToString(const U& a) -> typename std::enable_if<h2::h2_ToString_able<U>::value, h2_string>::type { return const_cast<U&>(a).ToString(); }
   template <typename U> static auto print__ToString(const U& a) -> typename std::enable_if<!h2::h2_ToString_able<U>::value, h2_string>::type { return print__to_string(a); }
   template <typename U> static auto print__to_string(const U& a) -> typename std::enable_if<h2::h2_to_string_able<U>::value, h2_string>::type { return const_cast<U&>(a).to_string(); }
   template <typename U> static auto print__to_string(const U& a) -> typename std::enable_if<!h2::h2_to_string_able<U>::value, h2_string>::type { return ""; }
};

template <typename T>
struct h2_is_streamable {  // exclude number
   template <typename U> static auto test(U* u) -> decltype(std::declval<std::ostream&>() << *u, std::true_type());
   template <typename U> static auto test(...) -> std::false_type;
   static constexpr bool value = decltype(test<T>(nullptr))::value && !std::is_arithmetic<T>::value;
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_streamable<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      h2_ostringstream oss;
      oss << a;
      represent = represent && std::is_convertible<T, h2_string>::value;
      return gray(quote_if(represent)) + oss.str().c_str() + gray(quote_if(represent));
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
   static h2_line print(const T& a, bool = false)
   {
#if defined _MSC_VER || defined __CYGWIN__  // std::to_string issue
      h2_ostringstream oss;
      oss << std::fixed << std::setprecision(std::numeric_limits<T>::digits10 + 1) << a;
      auto str = oss.str();
#else
      auto str = std::to_string(a);
#endif
      if (str.find_first_of('.') != std::string::npos) {
         str.erase(str.find_last_not_of("0") + 1);
         str.erase(str.find_last_not_of(".") + 1);
      }
      return str.c_str();
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
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      h2_line line;
      int count = 0;  // too large too print
      for (auto it = a.begin(); it != a.end(); ++it)
         if (++count > 100) return gray("[") + "..." + gray("]");
      for (auto it = a.begin(); it != a.end(); ++it)
         line += gray(comma_if(it != a.begin())) + h2_stringify_impl<typename T::value_type>::print(*it, represent);
      return gray("[") + line + gray("]");
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container_adaptor<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      auto _a = underlying_container(a);
      return h2_stringify_impl<decltype(_a)>::print(_a, represent);
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
      return tuple_print(a, represent, std::integral_constant<std::size_t, I - 1>()) + gray(comma_if(1 < I)) + h2_stringify_impl<typename std::decay<decltype(std::get<I - 1>(a))>::type>::print(std::get<I - 1>(a), represent);
   }
};

template <>
struct h2_stringify_impl<std::nullptr_t> {
   static h2_line print(std::nullptr_t a, bool = false) { return "nullptr"; }
};

template <>
struct h2_stringify_impl<bool> {
   static h2_line print(const bool a, bool = false) { return a ? "true" : "false"; }
};

template <>
struct h2_stringify_impl<const char*> {
   static h2_line print(const char* a, bool represent) { return a ? h2_stringify_impl<h2_string>::print(h2_string{a}, represent) : h2_line{"(null)"}; }
};

template <>
struct h2_stringify_impl<char*> {
   static h2_line print(char* a, bool represent) { return h2_stringify_impl<const char*>::print(a, represent); }
};

template <>
struct h2_stringify_impl<unsigned char> {  // https://en.cppreference.com/w/cpp/string/byte/isprint
   static h2_line print(unsigned char a, bool) { return h2_stringify_impl<unsigned int>::print(static_cast<unsigned int>(a), false); }
};

template <>
struct h2_stringify_impl<char> {
   static h2_line print(char a, bool represent) { return gray(quote_if(represent, "'")) + h2_string(1, a) + gray(quote_if(represent, "'")); }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_string<T>::value && std::is_same<wchar_t, typename T::value_type>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      h2_string s;
      for (auto c : a) s += (c <= 0xff) ? static_cast<char>(c) : '?';
      return h2_stringify_impl<h2_string>::print(s, represent);
   }
};

template <>
struct h2_stringify_impl<const wchar_t*> {
   static h2_line print(const wchar_t* a, bool represent) { return a ? h2_stringify_impl<std::wstring>::print(std::wstring{a}, represent) : h2_line{"(null)"}; }
};

template <>
struct h2_stringify_impl<wchar_t*> {
   static h2_line print(wchar_t* a, bool represent) { return h2_stringify_impl<const wchar_t*>::print(a, represent); }
};

template <>
struct h2_stringify_impl<wchar_t> {
   static h2_line print(wchar_t a, bool represent) { return h2_stringify_impl<char>::print((a <= 0xff) ? static_cast<char>(a) : '?', represent); }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<std::is_base_of<std::exception, T>::value>::type> {
   static h2_line print(const T& a, bool represent = false) { return h2_stringify_impl<const char*>::print(a.what(), represent); }
};

template <typename T>
inline h2_line h2_stringify(const T& a, bool represent = false) { return h2_stringify_impl<T>::print(a, represent); }
// source/symbol/h2_nm.hpp
struct h2_symbol {
   h2_list x;
   char symbol[128]{'\0'};
   unsigned long long addr;
   h2_symbol(const char* symbol_, unsigned long long addr_) : addr(addr_) { strncpy(symbol, symbol_, 127); }
   char* name();
};

struct h2_nm {
   h2_singleton(h2_nm);
   h2_list mangle_symbols, demangle_symbols;
   bool leading_underscore = false;
   static int get_by_name(const char* name, h2_symbol* res[], int n);
   static h2_symbol* get_by_addr(unsigned long long addr);
   static unsigned long long get_mangle(const char* name);
};
// source/symbol/h2_load.hpp
struct h2_load {
   h2_singleton(h2_load);
   long long text_offset = -1;
   long long vtable_offset = -1;
   static void* addr_to_ptr(unsigned long long addr);
   static unsigned long long ptr_to_addr(void* ptr);
   static void* vtable_to_ptr(unsigned long long addr);
};

struct h2_vtable_test {
   h2_vtable_test() {}
   virtual ~h2_vtable_test() {}
   virtual void dummy() {}
};
// source/symbol/h2_backtrace.hpp
struct h2_backtrace {
   int count = 0, shift = 0;
   void* frames[100];

   h2_backtrace(int shift_ = 0) : shift(shift_) {}
   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;
   void clear() { count = 0; }

   bool operator==(const h2_backtrace&) const;
   static h2_backtrace& dump(int shift_);

   bool in(void* fps[]) const;
   void print(h2_vector<h2_string>& stacks) const;
   void print(size_t pad) const;
};
// source/symbol/h2_cxa.hpp
struct h2_cxa {
   static char* demangle(const char* mangle_name, char* demangle_name = (char*)alloca(1024), size_t length = 1024);
   static void* follow_jmp(void* fp, int n = 32);

   template <typename T, typename U = typename std::remove_reference<T>::type>
   static const char* type_name(char* name = (char*)alloca(512), size_t size = 512)
   {
      strcpy(name, "");
      if (std::is_const<U>::value) strcat(name, "const ");
      strcat(name, demangle(typeid(U).name()));
      if (std::is_lvalue_reference<T>::value) strcat(name, "&");
      else if (std::is_rvalue_reference<T>::value) strcat(name, "&&");
      return name;
   }
};
// source/report/h2_failure.hpp
struct h2_fail : h2_libc {
   h2_fail *subling_next = nullptr, *child_next = nullptr;

   bool warning = false;
   const char* assert_type = "In";  // In(Mock, AllOf, &&, ||)
   const char* assert_op = ",";
   h2_string e_expression, a_expression;
   h2_line explain;
   h2_string user_explain;

   //           expression     expection      represent       value
   // number     Ge(var)        Ge(5)          5               5
   // string     We(var)        We("abc")      "abc"           abc
   // char       We(var)        We('a')        'a'             a

   int seqno = -1;
   const char* filine;

   h2_fail(const h2_line& explain_, const char* filine_) : explain(explain_), filine(filine_) {}
   virtual ~h2_fail();

   h2_line locate();

   virtual void print(size_t si = 0, size_t ci = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, size_t, size_t)> cb, size_t si = 0, size_t ci = 0);
   static void append_subling(h2_fail*& fails, h2_fail* fail);
   static void append_child(h2_fail*& fails, h2_fail* fail);

   static h2_fail* new_normal(const h2_line& explain, const char* filine = nullptr);
   static h2_fail* new_unexpect(const h2_line& expection = {}, const h2_line& represent = {}, const h2_line& explain = {});
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection, const h2_line& explain = {});
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, const h2_line& explain = {});
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, bool caseless, const h2_line& explain = {});
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t length, const size_t width);
   static h2_fail* new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* filine);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample);
   static h2_fail* new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
   static h2_fail* new_exception(const h2_line& explain, const char* type, const h2_backtrace& bt_throw, bool as_warning, const char* filine = nullptr);
   static h2_fail* new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_line& explain = {});
};
// source/option/h2_option.hpp
static constexpr int OsLinux = 0x11, OsMacOS = 0x12, OsWindows = 0x20;
static constexpr int VerboseQuiet = 0, VerboseCompactFailed = 1, VerboseCompactWarning = 2, VerboseCompactPassed = 3, VerboseNormal = 4, VerboseDetail = 5;
static constexpr int ShuffleCode = 0x0, ShuffleRandom = 0x10, ShuffleName = 0x100, ShuffleFile = 0x1000, ShuffleReverse = 0x10000;
static constexpr int ListNone = 0x0, ListSuite = 0x10, ListCase = 0x100, ListTodo = 0x1000, ListTags = 0x10000;
static constexpr int FoldUnFold = 0, FoldShort = 1, FoldSame = 2, FoldSingle = 3, FoldMax = 5;

struct h2_option {
   h2_singleton(h2_option);

#if defined __linux
   static constexpr int os = OsLinux;
#elif defined __APPLE__
   static constexpr int os = OsMacOS;
#elif defined _WIN32 || defined __CYGWIN__  // +MinGW
   static constexpr int os = OsWindows;
#endif

   char args[256];
   const char* path;
   bool colorful = true;
   bool progressing = true;
   bool only_last_failed = false;
   bool memory_check = true;
   bool continue_assert = false;
   bool debugger_trap = false;
   bool exit_with_fails = false;
   bool as_waring_exception = false;
   bool as_waring_uncaught = false;
   bool as_waring_memory_leak = false;
   bool as_waring_memory_violate = false;
   bool as_waring_memory_double_free = false;
   bool as_waring_memory_asymmetric_free = false;
   int break_after_fails = 0;
   int run_rounds = 1;
   int fold_json = FoldMax;
   int shuffles = ShuffleCode;
   int lists = ListNone;
   int verbose = VerboseNormal;
   const char* json_source_quote = "";
   char junit_path[256]{'\0'};
   const char *includes[128]{nullptr}, *excludes[128]{nullptr};
   const char *file_includes[128]{nullptr}, *file_excludes[128]{nullptr};
   const char *suite_includes[128]{nullptr}, *suite_excludes[128]{nullptr};
   const char *case_includes[128]{nullptr}, *case_excludes[128]{nullptr};
   const char *tags_includes[128]{nullptr}, *tags_excludes[128]{nullptr};

   void parse(int argc, const char** argv);
};

static const h2_option& O = h2_option::I();  // for pretty
// source/core/h2_test.hpp
struct h2_stats {
   int passed = 0, failed = 0, warning = 0, todo = 0, filtered = 0, ignored = 0;
   int asserts = 0;
   long long footprint = 0;
   long long timecost = 0;
   void clear() { passed = failed = warning = todo = filtered = ignored = 0, asserts = 0, footprint = 0, timecost = 0; }
};

struct h2_test {
   h2_list x;

   const char* filine;
   const char* file;  // base file name
   int line;

   const char* describe;
   const char* name = "";
   const char* tags[64]{nullptr};
   char name_buf[512], tags_buf[512];

   int seq = 0;
   bool filtered = false;
   h2_stats stats;
   h2_list stubs;
   h2_list mocks;
   h2_list dnses;

   h2_test(const char* filine, const char* file, int line, const char* describe);
   bool tagged(const char* tag_pattern) const;
};
// source/core/h2_case.hpp
struct h2_case : h2_test {
   bool todo = false;
   bool ignored = false, failed = false, last_failed = false, warning = false;
   jmp_buf fail_hole;
   h2_fail* fails = nullptr;

   h2_case(const char* filine, const char* file, int line, const char* describe, int todo_) : h2_test(filine, file, line, describe), todo(todo_) {}
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() {}
   void post_cleanup();

   void failing(h2_fail* fail, bool defer, bool append);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};
// source/core/h2_suite.hpp
struct h2_suite : h2_test {
   void (*test_fp)(h2_suite*, h2_case*);
   jmp_buf cleanup_hole;
   h2_list cases;

   h2_suite(const char* filine, const char* file, int line, const char* describe, void (*)(h2_suite*, h2_case*));
   void clear();
   bool absent() const { return !describe; }  // nullptr describe means no SUITE wrapper (CASE/TODO ...)

   void enumerate();
   void test(h2_case* c);

   void setup();
   void cleanup();

   struct registor {
      registor(h2_suite* s, h2_case* c);
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s) : thus(s) {}
      ~cleaner();
   };
};
// source/core/h2_runner.hpp
struct h2_runner {
   h2_singleton(h2_runner);

   h2_suite* current_suite = nullptr;
   h2_case* current_case = nullptr;
   int rounds = 0;
   int lasts = 0;
   h2_list suites;
   h2_list stubs;
   h2_list mocks;
   h2_stats stats;

   void (*global_setups[1024])(){nullptr}, (*global_cleanups[1024])(){nullptr};
   void (*global_suite_setups[1024])(){nullptr}, (*global_suite_cleanups[1024])(){nullptr};
   void (*global_case_setups[1024])(){nullptr}, (*global_case_cleanups[1024])(){nullptr};

   void enumerate();
   void filter();
   void shuffle();
   int main(int argc, const char** argv);

   static void stub(void* srcfp, void* dstfp, const char* srcfn, const char* filine);
   static void unstub(void* srcfp);
   static void mock(void* mocker);
   static void failing(h2_fail* fail);
   static void asserts();
};

#define __H2GlobalCallback(Scope, Q)                               \
   namespace {                                                     \
      static struct Q {                                            \
         Q() { h2_array_append(h2::h2_runner::I().Scope, Scope); } \
         static void Scope();                                      \
      } H2PP_UNIQUE();                                             \
   }                                                               \
   void Q::Scope()

#define H2GlobalSetup(...) __H2GlobalCallback(global_setups, H2PP_UNIQUE())
#define H2GlobalCleanup(...) __H2GlobalCallback(global_cleanups, H2PP_UNIQUE())

#define H2GlobalSuiteSetup(...) __H2GlobalCallback(global_suite_setups, H2PP_UNIQUE())
#define H2GlobalSuiteCleanup(...) __H2GlobalCallback(global_suite_cleanups, H2PP_UNIQUE())

#define H2GlobalCaseSetup(...) __H2GlobalCallback(global_case_setups, H2PP_UNIQUE())
#define H2GlobalCaseCleanup(...) __H2GlobalCallback(global_case_cleanups, H2PP_UNIQUE())
// source/core/h2_core.hpp

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2PP_UNIQUE(suite_test_C))
#define __H2SUITE(suite_describe, suite_test)                                                               \
   static void suite_test(h2::h2_suite*, h2::h2_case*);                                                     \
   static h2::h2_suite H2PP_UNIQUE(si)(H2_FILINE, __FILE__, __LINE__, h2::ss(suite_describe), &suite_test); \
   static void suite_test(h2::h2_suite* suite_1_5_2_8_0_1_1_9_8, h2::h2_case* case_1_1_0_2_6_0_0_2_4)

#define H2Setup(...) if (case_1_1_0_2_6_0_0_2_4)
#define H2Cleanup(...) if (::setjmp(suite_1_5_2_8_0_1_1_9_8->cleanup_hole))

#define H2Todo(...) __H2Case(#__VA_ARGS__, H2PP_UNIQUE(ci), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), 1)
#define H2Case(...) __H2Case(#__VA_ARGS__, H2PP_UNIQUE(ci), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), 0)
#define __H2Case(case_describe, case_instance, suite_cleaner, case_cleaner, todo)                                         \
   static h2::h2_case case_instance(H2_FILINE, __FILE__, __LINE__, h2::ss(case_describe), todo);                          \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance);                                \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4)                                                                          \
      for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner; case_1_1_0_2_6_0_0_2_4 = nullptr) \
         for (h2::h2_case::cleaner case_cleaner(&case_instance); case_cleaner;)                                           \
            if (!::setjmp(case_instance.fail_hole))

#define H2TODO(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(case_test_C), H2PP_UNIQUE(suite_test_C), 1)
#define H2CASE(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(case_test_C), H2PP_UNIQUE(suite_test_C), 0)
#define __H2CASE(case_describe, case_test, suite_test, todo)                                          \
   static void case_test();                                                                           \
   static void suite_test(h2::h2_suite* suite_1_5_2_8_0_1_1_9_8, h2::h2_case* case_1_1_0_2_6_0_0_2_4) \
   {                                                                                                  \
      static h2::h2_case case_instance(H2_FILINE, __FILE__, __LINE__, h2::ss(case_describe), todo);   \
      static h2::h2_suite::registor suite_registor(suite_1_5_2_8_0_1_1_9_8, &case_instance);          \
      if (&case_instance == case_1_1_0_2_6_0_0_2_4)                                                   \
         for (h2::h2_case::cleaner case_cleaner(&case_instance); case_cleaner;)                       \
            if (!::setjmp(case_instance.fail_hole))                                                   \
               case_test();                                                                           \
   }                                                                                                  \
   static h2::h2_suite H2PP_UNIQUE(si)(H2_FILINE, __FILE__, __LINE__, nullptr, &suite_test);          \
   static void case_test()

/* clang-format off */
// H2_An((a, b, c)) ==> a
#define H2_An(...) H2PP_EVAL(_H2_An(__VA_ARGS__))
#define _H2_An(...) H2PP_CAT2(_H2_An1_, H2PP_IS_EMPTY(__VA_ARGS__)) ((__VA_ARGS__))
#define _H2_An1_1(...) 0
#define _H2_An1_0(MSVC_Workaround) _H2_An1 MSVC_Workaround
#define _H2_An1(a, ...) H2PP_CAT2(_H2_An2_, H2PP_IS_PARENTHESIS(a)) (a)
#define _H2_An2_0(a) a
#define _H2_An2_1(a) H2PP_DEFER(_H2_An2)() a
#define _H2_An2() _H2_An

#define H2_XAn(...) H2_An(H2PP_HEAD(__VA_ARGS__))
#define H2_YAn(...) H2_An(H2PP_LAST(__VA_ARGS__))
/* clang-format on */

#define H2CASES(case_prefix, ...) __H2CASES1(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASES1(case_test, case_prefix, ...)                                                           \
   template <typename T> static void case_test(T x);                                                      \
   H2PP_FOREACH(, __H2CASES_Callback, (case_test, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   template <typename T> static void case_test(T x)
#define __H2CASES2(case_test, case_prefix, i, x) \
   H2CASE(case_prefix i. x) { case_test(x); }
#define __H2CASES_Callback(Pack, i, x) H2PP_PROXY(__H2CASES2, (H2PP_REMOVE_PARENTHESES(Pack), i, x))

#define H2CASESS(case_prefix, ...) __H2CASESS1(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASESS1(case_test, case_prefix, ...)                                \
   template <typename Tx, typename Ty> static void case_test(Tx x, Ty y);       \
   H2PP_FULLMESH(, __H2CASESS_Callback, (case_test, case_prefix), __VA_ARGS__); \
   template <typename Tx, typename Ty> static void case_test(Tx x, Ty y)
#define __H2CASESS2(case_test, case_prefix, i, j, x, y) \
   H2CASE(case_prefix i.j. x, y) { case_test(x, y); }
#define __H2CASESS_Callback(Pack, i, j, x, y) H2PP_PROXY(__H2CASESS2, (H2PP_REMOVE_PARENTHESES(Pack), i, j, x, y))

#define H2Cases(case_prefix, ...) __H2Cases1(H2PP_UNIQUE(x), H2PP_UNIQUE(do), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), case_prefix, __VA_ARGS__)
#define __H2Cases1(Qx, door_open, suite_cleaner, case_cleaner, case_prefix, ...)                              \
   auto Qx = H2_An(__VA_ARGS__);                                                                              \
   bool door_open = false;                                                                                    \
   H2PP_FOREACH(, __H2Cases_Callback, (Qx, door_open, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   if (case_1_1_0_2_6_0_0_2_4 && door_open)                                                                   \
      for (auto x = Qx; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)                             \
         for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner;)                   \
            for (h2::h2_case::cleaner case_cleaner(case_1_1_0_2_6_0_0_2_4); case_cleaner;)                    \
               if (!::setjmp(case_1_1_0_2_6_0_0_2_4->fail_hole))
#define __H2Cases2(case_instance, Qx, door_open, case_prefix, i, x)                               \
   static h2::h2_case case_instance(H2_FILINE, __FILE__, __LINE__, H2PP_STR(case_prefix i. x), 0); \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance);        \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4) Qx = x, door_open = true;
#define __H2Cases_Callback(Pack, i, x) H2PP_PROXY(__H2Cases2, (H2PP_UNIQUE(ci), H2PP_REMOVE_PARENTHESES(Pack), i, x))

#define H2Casess(case_prefix, ...) __H2Casess1(H2PP_UNIQUE(x), H2PP_UNIQUE(y), H2PP_UNIQUE(do), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), case_prefix, __VA_ARGS__)
#define __H2Casess1(Qx, Qy, door_open, suite_cleaner, case_cleaner, case_prefix, ...)          \
   auto Qx = H2_XAn(__VA_ARGS__);                                                              \
   auto Qy = H2_YAn(__VA_ARGS__);                                                              \
   bool door_open = false;                                                                     \
   H2PP_FULLMESH(, __H2Casess_Callback, (Qx, Qy, door_open, case_prefix), __VA_ARGS__);        \
   if (case_1_1_0_2_6_0_0_2_4 && door_open)                                                    \
      for (auto x = Qx; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)              \
         for (auto y = Qy; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)           \
            for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner;) \
               for (h2::h2_case::cleaner case_cleaner(case_1_1_0_2_6_0_0_2_4); case_cleaner;)  \
                  if (!::setjmp(case_1_1_0_2_6_0_0_2_4->fail_hole))
#define __H2Casess2(case_instance, Qx, Qy, door_open, case_prefix, i, j, x, y)                         \
   static h2::h2_case case_instance(H2_FILINE, __FILE__, __LINE__, H2PP_STR(case_prefix i.j. x, y), 0); \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance);             \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4) Qx = x, Qy = y, door_open = true;
#define __H2Casess_Callback(Pack, i, j, x, y) H2PP_PROXY(__H2Casess2, (H2PP_UNIQUE(ci), H2PP_REMOVE_PARENTHESES(Pack), i, j, x, y))

// can't declare/define template class/function in function scope
#define H2Cases_t(...) static_assert(false, "Cases_t not implemented, use H2CASES_T outside of SUITE{}");
#define H2Casess_t(...) static_assert(false, "Casess_t not implemented, use H2CASESS_T outside of SUITE{}");

#define H2CASES_T(case_prefix, ...) __H2CASES_T1(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASES_T1(case_test, case_prefix, ...)                                                           \
   template <typename x> static void case_test();                                                           \
   H2PP_FOREACH(, __H2CASES_T_Callback, (case_test, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   template <typename x> static void case_test()
#define __H2CASES_T2(case_test, case_prefix, i, x) \
   H2CASE(case_prefix i. x) { case_test<x>(); }
#define __H2CASES_T_Callback(Pack, i, x) H2PP_PROXY(__H2CASES_T2, (H2PP_REMOVE_PARENTHESES(Pack), i, x))

#define H2CASESS_T(case_prefix, ...) __H2CASESS_T1(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASESS_T1(case_test, case_prefix, ...)                                \
   template <typename x, typename y> static void case_test();                     \
   H2PP_FULLMESH(, __H2CASESS_T_Callback, (case_test, case_prefix), __VA_ARGS__); \
   template <typename x, typename y> static void case_test()
#define __H2CASESS_T2(case_test, case_prefix, i, j, x, y) \
   H2CASE(case_prefix i.j. x, y) { case_test<x, y>(); }
#define __H2CASESS_T_Callback(Pack, i, j, x, y) H2PP_PROXY(__H2CASESS_T2, (H2PP_REMOVE_PARENTHESES(Pack), i, j, x, y))

#define H2Todos(case_prefix, ...)     \
   H2Todo(case_prefix __VA_ARGS__) {} \
   if (0)                             \
      for (auto x = H2_An(__VA_ARGS__); false;)

#define H2Todoss(case_prefix, ...)               \
   H2Todo(case_prefix, __VA_ARGS__) {}           \
   if (0)                                        \
      for (auto x = H2_XAn(__VA_ARGS__); false;) \
         for (auto y = H2_YAn(__VA_ARGS__); false;)

#define H2TODOS(case_prefix, ...) __H2TODOS(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2TODOS(case_test, case_prefix, ...)       \
   template <typename T> static void case_test(T x); \
   H2TODO(case_prefix, __VA_ARGS__) {}               \
   template <typename T> static void case_test(T x)

#define H2TODOSS(case_prefix, ...) __H2TODOSS(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2TODOSS(case_test, case_prefix, ...)                           \
   template <typename Tx, typename Ty> static void case_test(Tx x, Ty y); \
   H2TODO(case_prefix, __VA_ARGS__) {}                                    \
   template <typename Tx, typename Ty> static void case_test(Tx x, Ty y)

#define H2TODOS_T(case_prefix, ...) __H2TODOS_T(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2TODOS_T(case_test, case_prefix, ...)  \
   template <typename x> static void case_test(); \
   H2TODO(case_prefix, __VA_ARGS__) {}            \
   template <typename x> static void case_test()

#define H2TODOSS_T(case_prefix, ...) __H2TODOSS_T(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2TODOSS_T(case_test, case_prefix, ...)             \
   template <typename x, typename y> static void case_test(); \
   H2TODO(case_prefix, __VA_ARGS__) {}                        \
   template <typename x, typename y> static void case_test()
// source/report/h2_report.hpp
struct h2_runner;
struct h2_suite;
struct h2_case;

struct h2_report_interface {
   virtual void on_runner_start(h2_runner*) = 0;
   virtual void on_runner_endup(h2_runner*) = 0;
   virtual void on_suite_start(h2_suite*) = 0;
   virtual void on_suite_endup(h2_suite*) = 0;
   virtual void on_case_start(h2_suite*, h2_case*) = 0;
   virtual void on_case_endup(h2_suite*, h2_case*) = 0;
};

struct h2_report : h2_report_interface {
   h2_singleton(h2_report);
   static void initialize();

   struct registor {
      registor(h2_report_interface* report);
   };

   bool backable = false;
   h2_report_interface* reports[8]{nullptr};

   virtual void on_runner_start(h2_runner* r) override;
   virtual void on_runner_endup(h2_runner* r) override;
   virtual void on_suite_start(h2_suite* s) override;
   virtual void on_suite_endup(h2_suite* s) override;
   virtual void on_case_start(h2_suite* s, h2_case* c) override;
   virtual void on_case_endup(h2_suite* s, h2_case* c) override;
};

#define H2Report(Class) static h2::h2_report::registor H2PP_UNIQUE(report)(new Class)
// source/matcher/h2_matches.hpp
struct C {
   int dimension, range_start, range_end, times;
   bool no_compare_operator, negative, case_insensitive, squash_whitespace;
   C(int array_size_ = -1, bool no_compare_operator_ = false, bool negative_ = false, bool case_insensitive_ = false, bool squash_whitespace_ = false, int range_start_ = 0, int range_end_ = 5413722, int times_ = 1) : dimension(array_size_), range_start(range_start_), range_end(range_end_), times(times_), no_compare_operator(no_compare_operator_), negative(negative_), case_insensitive(case_insensitive_), squash_whitespace(squash_whitespace_) {}

   bool in(const int i) const { return range_start <= i && i < range_end; }
   bool fit(bool result) const { return result == !negative; }
   C clear_size() const { return {-1, no_compare_operator, negative, case_insensitive, squash_whitespace, 0, 5413722, times}; }
   C update_negative(bool target = false) const { return {dimension, no_compare_operator, target, case_insensitive, squash_whitespace, range_start, range_end, times}; }
   C update_caseless(bool target = false) const { return {dimension, no_compare_operator, negative, target, squash_whitespace, range_start, range_end, times}; }
   C update_spaceless(bool target = false) const { return {dimension, no_compare_operator, negative, case_insensitive, target, range_start, range_end, times}; }

   h2_line pre(const char* ns = "!") const
   {
      h2_line t;
      if (!no_compare_operator && negative) t.push_back(ns);
      if (case_insensitive) t.push_back("~");
      if (squash_whitespace) t.push_back("*");
      return t;
   }
   h2_line post() const { return times == 1 ? h2_line() : " * " + h2_stringify(times); }
};

struct h2_matches {
   virtual h2_line expection(const C& c) const = 0;
};

struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      if (c.fit(true)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.negative ? "!Any" : "Any";
   }
};

struct h2_matches_null : h2_matches {
   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<std::is_pointer<A>::value || h2_is_smart_ptr<A>::value || std::is_integral<A>::value, h2_fail*>::type
   {
      bool result = !a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<!std::is_pointer<A>::value && !h2_is_smart_ptr<A>::value && !std::is_integral<A>::value, h2_fail*>::type
   {
      bool result = std::is_same<std::nullptr_t, typename std::decay<decltype(a)>::type>::value;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.negative ? "!NULL" : "NULL";
   }
};

struct h2_matches_bool : h2_matches {
   const bool e;
   explicit h2_matches_bool(bool e_) : e(e_) {}
   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      bool result = e ? !!a : !a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      return (e ? c.negative : !c.negative) ? "false" : "true";
   }
};

template <typename T> inline auto h2_matches_expection(const T& e, const C& c) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_line>::type { return e.expection(c); }
template <typename T> inline auto h2_matches_expection(const T& e, const C& c) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_line>::type { return h2_stringify(e, true); }
// source/matcher/h2_matcher.hpp
template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(const T& a, const C& c) const = 0;
   virtual h2_line expection(const C& c) const override { return ""; }
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher : h2_matches {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher();         // Any matcher
   h2_matcher(T value);  // Converting constructor
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl_, const int /*placeholder*/) : impl(impl_) {}
   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   virtual ~h2_matcher() {}
   h2_fail* matches(const T& a, const C& c = {}) const { return impl->matches(a, c); }
   virtual h2_line expection(const C& c = {}) const { return impl->expection(c); };
};

template <typename Matches>
struct h2_polymorphic_matcher : h2_matches {
   using matches_type = Matches;
   const Matches m;
   int range_start = -1, range_end = -1, times = 1;
   bool negative = false, case_insensitive = false, squash_whitespace = false;
   explicit h2_polymorphic_matcher(const Matches& m_) : m(m_) {}
   h2_polymorphic_matcher& operator!()
   {
      negative = !negative;
      return *this;
   }
   h2_polymorphic_matcher& operator~()
   {
      case_insensitive = true;
      return *this;
   }
   h2_polymorphic_matcher& operator*()
   {
      squash_whitespace = true;
      return *this;
   }
   h2_polymorphic_matcher& operator*(int times_)
   {
      times = times_;
      return *this;
   }
   h2_polymorphic_matcher& operator()(int start, int end = -1)  // [Start, End)
   {
      range_start = start, range_end = end;
      if (end == -1) range_start = 0, range_end = start;
      return *this;
   }
   h2_polymorphic_matcher& operator/(int end)
   {
      range_start = 0, range_end = end;
      return *this;
   }
#define H2_MATCHES_CONFIGURE c.dimension, c.no_compare_operator, negative != c.negative, /*XOR ^*/ case_insensitive || c.case_insensitive, squash_whitespace || c.squash_whitespace, range_start != -1 && range_end != -1 ? range_start : c.range_start, range_start != -1 && range_end != -1 ? range_end : c.range_end, c.times* times
   template <typename T> struct matches_matcher : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      int range_start, range_end, times;
      bool negative, case_insensitive, squash_whitespace;
      explicit matches_matcher(const Matches& m_, int range_start_, int range_end_, int times_, bool negative_, bool case_insensitive_, bool squash_whitespace_) : m(m_), range_start(range_start_), range_end(range_end_), times(times_), negative(negative_), case_insensitive(case_insensitive_), squash_whitespace(squash_whitespace_) {}
      h2_fail* matches(const T& a, const C& c = {}) const override { return m.matches(a, {H2_MATCHES_CONFIGURE}); }
      h2_line expection(const C& c) const override { return m.expection({H2_MATCHES_CONFIGURE}); }
   };
   template <typename T> operator h2_matcher<T>() const { return h2_matcher<T>(new matches_matcher<const T&>(m, range_start, range_end, times, negative, case_insensitive, squash_whitespace), 0); }
   virtual h2_line expection(const C& c = {}) const override { return h2_matches_expection(m, {H2_MATCHES_CONFIGURE}); }
};

template <typename T, typename = void>
struct h2_is_polymorphic_matcher : std::false_type {};
template <typename T>
struct h2_is_polymorphic_matcher<T, typename std::enable_if<h2_true_type<typename T::matches_type>::value>::type> : std::true_type {};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
static inline h2_polymorphic_matcher<h2_matches_any> Any() { return h2_polymorphic_matcher<h2_matches_any>(h2_matches_any()); }
// source/matcher/h2_matches_equation.hpp
template <typename E>
struct h2_type_equation : h2_matches {
   explicit h2_type_equation() {}

   template <typename A>
   h2_fail* matches(const A&, const C& c) const
   {
      if (c.fit(std::is_same<E, A>::value)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_cxa::type_name<A>());
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre("≠") + h2_cxa::type_name<E>();
   }
};

template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<!std::is_pointer<E>::value && !std::is_pointer<A>::value, h2_fail*>::type
   {
      if (c.fit(a == e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<std::is_pointer<E>::value || std::is_pointer<A>::value, h2_fail*>::type
   {
      if (!e) return h2_matches_null().matches(a, c);
      if (c.fit((void*)a == (void*)e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre("≠") + h2_stringify(e, true);
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_string _e = e, _a(a);
      if (c.squash_whitespace) _e = e.squash(), _a = _a.squash();
      if (c.fit(_a.equals(_e, c.case_insensitive))) return nullptr;
      // if (c.fit(h2_pattern::wildcard_match(_e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
      return h2_fail::new_strcmp(_e, a, c.case_insensitive, expection(c));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.pre("≠") + h2_stringify(c.squash_whitespace ? e.squash() : e, true);
   }
};

template <>
struct h2_equation<const char*> : h2_matches {
   const char* e;
   explicit h2_equation(const char* e_, const long double = 0) : e(e_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<std::is_convertible<A, h2_string>::value, h2_fail*>::type
   {
      if (!e) return h2_matches_null().matches(a, c);
      h2_equation<h2_string> string_m(e);
      return string_m.matches(a, c);
   }
   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<!std::is_convertible<A, h2_string>::value, h2_fail*>::type
   {
      if (!e) return h2_matches_null().matches(a, c);
      if (c.fit((void*)a == (void*)e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      if (!e) return h2_matches_null().expection(c);
      return c.pre("≠") + h2_stringify(c.squash_whitespace ? h2_string(e).squash() : h2_string(e), true);
   }
};

template <>
struct h2_equation<char*> : h2_equation<const char*> {
   explicit h2_equation(const char* e_, const long double = 0) : h2_equation<const char*>((const char*)e_) {}
};

struct h2_approximate {
   static constexpr unsigned long long A = 100000000000000ull;
   static constexpr unsigned long long B = 1000000000000000000ull;

   unsigned long long operator*(const double& epsilon) const
   {
      return (unsigned long long)(fabs(epsilon) * A);
   }

   static bool is_scale(const long double epsilon)
   {
      if (epsilon == 0) return false;
      return epsilon < B;
   }
   static const long double absolute_margin(const long double epsilon)
   {
      if (epsilon == 0) return 0.00001;
      return (epsilon - B) / A;
   }
   static const long double percentage_scale(const long double epsilon)
   {
      return (epsilon / A) / 100;
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& e_, const long double epsilon_ = 0) : e(e_), epsilon(epsilon_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<std::is_floating_point<E>::value || std::is_floating_point<A>::value, h2_fail*>::type
   {
      // the machine epsilon has to be scaled to the magnitude of the values used
      // and multiplied by the desired precision in ULPs (units in the last place)
      // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
      //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
      bool result;
      if (h2::h2_approximate::is_scale(epsilon))  // percentage/scale
         result = std::fabs((const long double)a - (const long double)e) <= std::fabs(e * h2::h2_approximate::percentage_scale(epsilon));
      else  // absolute/margin
         result = std::fabs((const long double)a - (const long double)e) <= std::fabs(h2::h2_approximate::absolute_margin(epsilon));
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<!std::is_floating_point<E>::value && !std::is_floating_point<A>::value, h2_fail*>::type
   {
      bool result = h2_numberfy<std::uintptr_t>(e) == h2_numberfy<std::uintptr_t>(a);
      if (c.fit(result)) return nullptr;
      if (!e && (std::is_pointer<A>::value || h2_is_smart_ptr<A>::value))
         return h2_fail::new_unexpect(c.negative ? "!NULL" : "NULL", h2_stringify(a, true));
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      h2_line t = h2_stringify(e);
      if (epsilon != 0) {
         if (h2::h2_approximate::is_scale(epsilon))  // percentage/scale
            t += "±" + h2_stringify(h2::h2_approximate::percentage_scale(epsilon) * 100.0) + "%";
         else  // absolute/margin
            t += "±" + h2_stringify(std::fabs(h2::h2_approximate::absolute_margin(epsilon)));
      }
      return c.update_caseless(false).pre("≠") + t;
   }
};

// template <typename T> const h2_polymorphic_matcher<h2_type_equation<T>> TypeEq(h2_type_equation<T>()); // variable template
template <typename T, typename P = h2_polymorphic_matcher<h2_type_equation<T>>>
inline P TypeEq() { return P(h2_type_equation<T>()); }

template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_equation<E>>>
auto Equals(const T& expect, const long double epsilon = 0) -> typename std::enable_if<!std::is_same<std::nullptr_t, E>::value && !std::is_same<bool, E>::value, P>::type { return P(h2_equation<E>(expect, epsilon)); }

template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_null>>
auto Equals(const T&, const long double = 0) -> typename std::enable_if<std::is_same<std::nullptr_t, E>::value, P>::type { return P(h2_matches_null()); }

template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_bool>>
auto Equals(const T& expect, const long double = 0) -> typename std::enable_if<std::is_same<bool, E>::value, P>::type { return P(h2_matches_bool(expect)); }

#define H2Eq(expect, ...) H2PP_CAT(H2Eq_, H2PP_IS_EMPTY(__VA_ARGS__))(expect, __VA_ARGS__)
#define H2Eq_1(expect, ...) h2::Equals(expect, 0)
#define H2Eq_0(expect, ...) h2::Equals(expect, h2::h2_approximate() * __VA_ARGS__ + h2::h2_approximate::B)
// source/matcher/h2_matcher_cast.hpp
template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from)
   {
      return do_cast(from,
                     std::integral_constant < bool, std::is_same<bool, M>::value || std::is_same<std::nullptr_t, M>::value > {},
                     std::is_convertible<M, h2_matcher<T>>{} /* h2_matcher::h2_matcher(T value) Converting constructor | h2_polymorphic_matcher::operator h2_matcher<T>() */,
                     std::is_convertible<M, T>{});
   }

   template <bool Ignore1, bool Ignore2>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore1>, std::integral_constant<bool, Ignore2>) { return Equals(from); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type, std::false_type) { return Equals(from); }
};

template <typename T, typename U>
struct h2_matcher_cast_impl<T, h2_matcher<U>> {
   static h2_matcher<T> cast(const h2_matcher<U>& from) { return h2_matcher<T>(new internal_impl(from), 0); }

   struct internal_impl : h2_matcher_impl<const T&>, h2_libc {
      const h2_matcher<U> from;
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}
      h2_fail* matches(const T& a, const C& c) const override { return from.matches(static_cast<U>(a), c); }
      void operator=(internal_impl const&) = delete;
   };
};

template <typename T>
struct h2_matcher_cast_impl<T, h2_matcher<T>> {
   static h2_matcher<T> cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> h2_matcher_cast(const M& from) { return h2_matcher_cast_impl<T, M>::cast(from); }
// source/matcher/h2_matches_cast.hpp
template <typename Cast, typename Matcher>
struct h2_castof_matches : h2_matches {
   const Matcher m;
   explicit h2_castof_matches(Matcher m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<Cast>(m).matches((Cast)a, c);
   }
   virtual h2_line expection(const C& c) const override
   {
      return h2_matches_expection(m, c);
   }
};

template <typename D, typename T, typename P = h2_polymorphic_matcher<h2_castof_matches<D, T>>>
inline P CastOf(T expect) { return P(h2_castof_matches<D, T>(expect)); }
// source/matcher/h2_matches_range.hpp
template <int Start, int End>
struct h2_range {
   static constexpr int start = End == -1 ? 0 : Start;
   static constexpr int end = End == -1 ? Start : End;
   static bool in(const int i) { return start <= i && i < end; }
};

struct h2_range_matches : h2_matches {
   const double start, end, step;
   explicit h2_range_matches(const double& start_, const double& end_ = -0.15048889911, const double& step_ = -0.15048889911) : start(start_), end(end_), step(step_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {  // same as python range arguments
      double _start = start, _end = end, _step = step;
      if (end == -0.15048889911) _start = 0, _end = start;
      if (step == -0.15048889911) _step = 1;
      bool found = false;
      for (double i = _start; i < _end; i += _step)
         if (std::fabs((const double)a - i) <= std::fabs(h2::h2_approximate::absolute_margin(0))) {
            found = true;
            break;
         }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      h2_line t = h2_stringify(start);
      if (end != -0.15048889911) t += gray(", ") + h2_stringify(end);
      if (step != -0.15048889911) t += gray(", ") + h2_stringify(step);
      return c.update_caseless(false).pre() + "Range" + gray("(") + t + gray(")");
   }
};

inline auto Range(const double& start, const double& end = -0.15048889911, const double& step = -0.15048889911) -> h2_polymorphic_matcher<h2_range_matches> { return h2_polymorphic_matcher<h2_range_matches>(h2_range_matches(start, end, step)); }
// source/matcher/h2_matches_container.hpp
template <typename... Matchers>
struct h2_matcher_tuple {
   std::tuple<Matchers...> t_matchers;
   explicit h2_matcher_tuple(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename T>
   void __matchers(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}
   template <typename T, std::size_t I>
   void __matchers(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const
   {
      __matchers(v_matchers, std::integral_constant<std::size_t, I - 1>());
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));
   }
   template <typename T>
   void matchers(h2_vector<h2_matcher<T>>& v_matchers) const { return __matchers(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }

   h2_line __expection(const C& c, std::integral_constant<std::size_t, 0>) const { return {}; }
   template <std::size_t I>
   h2_line __expection(const C& c, std::integral_constant<std::size_t, I>) const
   {
      return __expection(c, std::integral_constant<size_t, I - 1>()) + gray(comma_if(1 < I)) + h2_matches_expection(std::get<I - 1>(t_matchers), c);
   }
   h2_line expection(const C& c) const { return __expection(c, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
};

template <typename EK, typename EV>
struct h2_pair_matches : h2_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(const EK& k_, const EV& v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.first)>::type>(k).matches(a.first, c.clear_size().update_negative(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.second)>::type>(v).matches(a.second, c.clear_size().update_negative(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }
   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")");
   }
};

template <typename T>
struct h2_is_pair_matches : std::false_type {};
template <typename MK, typename MV>
struct h2_is_pair_matches<h2_pair_matches<MK, MV>> : std::true_type {};

template <typename T, typename = void>
struct h2_is_polymorphic_matcher_pair_matches : std::false_type {};
template <typename T>
struct h2_is_polymorphic_matcher_pair_matches<T, typename std::enable_if<h2_is_polymorphic_matcher<T>::value && h2_is_pair_matches<typename T::matches_type>::value>::type> : std::true_type {};

#define H2_MATCHES_CONTAINER1()                                                                                                                                                                              \
   template <typename A>                                                                                                                                                                                     \
   auto matches(const A& a, const C& c) const->typename std::enable_if<h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type                                                         \
   {                                                                                                                                                                                                         \
      return matches(underlying_container(a), c);                                                                                                                                                            \
   }                                                                                                                                                                                                         \
   template <typename A>                                                                                                                                                                                     \
   auto matches(A a, const C& c) const->typename std::enable_if<!h2_is_iterable<typename std::decay<A>::type>::value && !h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type       \
   { /* native c/c++ array or pointer */                                                                                                                                                                     \
      return matches(h2_array<decltype(a[0])>(a, c.dimension == -1 ? (c.range_end - c.range_start < Range::end - Range::start ? c.range_end - c.range_start : Range::end - Range::start) : c.dimension), c); \
   }

#define H2_MATCHES_CONTAINER2(name)                                                                                                                                 \
   H2_MATCHES_CONTAINER1()                                                                                                                                          \
   virtual h2_line expection(const C& c) const override                                                                                                             \
   {                                                                                                                                                                \
      return c.update_caseless(false).pre() + (name) + gray("(") + h2_matches_expection(m, c.update_caseless(false).update_negative(false)) + gray(")") + c.post(); \
   }                                                                                                                                                                \
   bool range_in(const C& c, const int i) const { return c.in(i) && Range::in(i); }

#define H2_MATCHES_STATS(value, prev_result)                                                                       \
   bool result = (prev_result);                                                                                    \
   if (result) {                                                                                                   \
      h2_fail* fail = h2_matcher_cast<decltype(value)>(m).matches((value), c.clear_size().update_negative(false)); \
      if (fail) delete fail, result = false;                                                                       \
   }                                                                                                               \
   if (c.fit(result)) return nullptr;                                                                              \
   return h2_fail::new_unexpect(expection(c), ((prev_result) ? h2_stringify(value) : h2_line()) + h2_stringify(a, true));

template <typename Range, typename Matcher>  // [Start, End)
struct h2_every1_matches : h2_matches {
   Matcher m;
   explicit h2_every1_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      int i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) {
            h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(ia, c.clear_size().update_negative(false));
            h2_fail::append_subling(fails, fail);
         }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }
   H2_MATCHES_CONTAINER2("Every")
};

template <typename Range, typename EK, typename EV, typename M = h2_pair_matches<EK, EV>, typename P = h2_polymorphic_matcher<M>>
struct h2_every2_matches : h2_every1_matches<Range, P> {
   EK k;
   EV v;
   explicit h2_every2_matches(const EK& k_, const EV& v_) : h2_every1_matches<Range, P>(P(M(k_, v_))), k(k_), v(v_) {}
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + "Every" + gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")");
   }
};

template <typename Range, typename Matcher>
struct h2_has1_matches : h2_matches {
   Matcher m;
   explicit h2_has1_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto __matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_map<typename std::decay<A>::type>::value && !h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // HasKey scenario
      int found = 0, i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) {
            h2_fail* fail = h2_matcher_cast<typename std::decay<decltype(ia.first)>::type>(m).matches(ia.first, c.clear_size().update_negative(false));
            if (!fail) {
               if (++found >= c.times) break;
            } else delete fail;
         }
      if (c.fit(found >= c.times)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto __matches(const A& a, const C& c) const -> typename std::enable_if<!h2_is_map<typename std::decay<A>::type>::value || h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // Normal scenario
      int found = 0, i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) {
            h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(ia, c.clear_size().update_negative(false));
            if (!fail) {
               if (++found >= c.times) break;
            } else delete fail;
         }
      if (c.fit(found >= c.times)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type { return __matches(a, c); }

   H2_MATCHES_CONTAINER2("Has")
};

template <typename EK, typename EV, typename M = h2_pair_matches<EK, EV>, typename P = h2_polymorphic_matcher<M>>
struct h2_has2_matches : h2_has1_matches<h2_range<0, 5413722>, P> {
   const EK k;
   const EV v;
   const char* type;
   explicit h2_has2_matches(const EK& k_, const EV& v_, const char* type_ = "Has") : h2_has1_matches<h2_range<0, 5413722>, P>(P(M(k_, v_))), k(k_), v(v_), type(type_) {}

   virtual h2_line expection(const C& c) const override
   {
      h2_line t;
      if (strcmp("HasValue", type)) t += h2_matches_expection(k, c.update_negative(false));
      if (!strcmp("Has", type)) t += ", ";
      if (strcmp("HasKey", type)) t += h2_matches_expection(v, c.update_negative(false));
      return c.update_caseless(false).pre() + type + gray("(") + t + gray(")") + c.post();
   }
};

template <typename Range, typename... Matchers>
struct h2_listof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_listof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_vector<h2_matcher<typename A::value_type>> matchers;
      e.matchers(matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, count = 0;
         for (auto const& ia : a) {
            if (j++ == i + (c.range_start > Range::start ? c.range_start : Range::start)) {
               ++count;
               fail = matchers[i].matches(ia, c.clear_size().update_negative(false));
               break;
            }
         }
         if (count == 0) fail = h2_fail::new_unexpect(matchers[i].expection(c.update_negative(false)), "", "but out of range");
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + gray("[") + e.expection(c.update_negative(false)) + gray("]");
   }
   H2_MATCHES_CONTAINER1()
};

template <typename Matcher, typename Range = h2_range<0, 5413722>>
struct h2_countof_matches : h2_matches {
   Matcher m;
   explicit h2_countof_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      size_t count = 0;  // std::forward_list no size()
      for (auto it = a.cbegin(); it != a.cend(); ++it) count++;
      H2_MATCHES_STATS(count, true);
   }
   H2_MATCHES_CONTAINER2("CountOf")
};

template <typename Range, typename Matcher>
struct h2_maxmin_matches : h2_matches {
   const bool is_max;
   Matcher m;
   explicit h2_maxmin_matches(const bool is_max_, const Matcher& m_) : is_max(is_max_), m(m_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      int i = 0, count = 0;
      typename A::value_type value;
      for (auto const& ia : a)  // std::max_element / std::max_element
         if (range_in(c, i++)) {
            if (++count == 1) value = ia;
            if (is_max ? (ia > value) : (ia < value)) value = ia;
         }
      H2_MATCHES_STATS(value, count != 0);
   }
   H2_MATCHES_CONTAINER2(is_max ? "MaxOf" : "MinOf")
};

template <typename Range, typename Matcher>
struct h2_avg_matches : h2_matches {
   Matcher m;
   explicit h2_avg_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      double sum = 0, count = 0;
      int i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) sum += ia, ++count;
      H2_MATCHES_STATS(count ? sum / count : 0, count != 0);
   }
   H2_MATCHES_CONTAINER2("AvgOf")
};

template <typename Range, typename Matcher>
struct h2_median_matches : h2_matches {
   Matcher m;
   explicit h2_median_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, const C& c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      std::vector<typename A::value_type> b;
      int i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++))
            for (auto it = b.begin();; ++it)  // std::sort(b.begin(), b.end());  // std::nth_element /// avoid include <algorithm> for compile time
               if (it == b.end() || ia < *it) {
                  b.insert(it, ia);
                  break;
               }
      double value = 0;
      if (b.size()) value = b.size() % 2 ? b[b.size() / 2] : (b[b.size() / 2] + b[b.size() / 2 + 1]) / 2;
      H2_MATCHES_STATS(value, b.size() != 0);
   }
   H2_MATCHES_CONTAINER2("MedianOf")
};

template <typename TK, typename TV, typename EK = typename h2_decay<TK>::type, typename EV = typename h2_decay<TV>::type, typename P = h2_polymorphic_matcher<h2_pair_matches<EK, EV>>>
inline P Pair(const TK& expect_key, const TV& expect_value) { return P(h2_pair_matches<EK, EV>(expect_key, expect_value)); }

template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_has1_matches<h2_range<Rs, Re>, E>>>
inline P Has(const T& expect) { return P(h2_has1_matches<h2_range<Rs, Re>, E>(expect)); }
template <typename TK, typename TV, typename EK = typename h2_decay<TK>::type, typename EV = typename h2_decay<TV>::type, typename P = h2_polymorphic_matcher<h2_has2_matches<EK, EV>>>
inline P Has(const TK& expect_key, const TV& expect_value) { return P(h2_has2_matches<EK, EV>(expect_key, expect_value)); }
template <typename TK, typename EK = typename h2_decay<TK>::type, typename EV = h2_polymorphic_matcher<h2_matches_any>, typename P = h2_polymorphic_matcher<h2_has2_matches<EK, EV>>>
inline P HasKey(const TK& expect_key) { return P(h2_has2_matches<EK, EV>(expect_key, _, "HasKey")); }
template <typename TV, typename EV = typename h2_decay<TV>::type, typename EK = h2_polymorphic_matcher<h2_matches_any>, typename P = h2_polymorphic_matcher<h2_has2_matches<EK, EV>>>
inline P HasValue(const TV& expect_value) { return P(h2_has2_matches<EK, EV>(_, expect_value, "HasValue")); }

template <int Rs = 5413722, int Re = -1, typename... T, typename P = h2_polymorphic_matcher<h2_listof_matches<h2_range<Rs, Re>, typename std::decay<const T&>::type...>>>
inline P ListOf(const T&... expects) { return P(h2_listof_matches<h2_range<Rs, Re>, typename std::decay<const T&>::type...>(expects...)); }

template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_every1_matches<h2_range<Rs, Re>, E>>>
inline P Every(const T& expect) { return P(h2_every1_matches<h2_range<Rs, Re>, E>(expect)); }
template <int Rs = 5413722, int Re = -1, typename T1, typename T2, typename E1 = typename std::decay<const T1&>::type, typename E2 = typename std::decay<const T2&>::type, typename P = h2_polymorphic_matcher<h2_every2_matches<h2_range<Rs, Re>, E1, E2>>>
inline P Every(const T1& expect1, const T2& expect2) { return P(h2_every2_matches<h2_range<Rs, Re>, E1, E2>(expect1, expect2)); }

template <typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_countof_matches<E>>>
inline P CountOf(const T& expect) { return P(h2_countof_matches<E>(expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_maxmin_matches<h2_range<Rs, Re>, E>>>
inline P MaxOf(const T& expect) { return P(h2_maxmin_matches<h2_range<Rs, Re>, E>(true, expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_maxmin_matches<h2_range<Rs, Re>, E>>>
inline P MinOf(const T& expect) { return P(h2_maxmin_matches<h2_range<Rs, Re>, E>(false, expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_avg_matches<h2_range<Rs, Re>, E>>>
inline P AvgOf(const T& expect) { return P(h2_avg_matches<h2_range<Rs, Re>, E>(expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_median_matches<h2_range<Rs, Re>, E>>>
inline P MedianOf(const T& expect) { return P(h2_median_matches<h2_range<Rs, Re>, E>(expect)); }
// source/matcher/h2_matches_logic.hpp
template <typename Matcher>
struct h2_not_matches : h2_matches {
   const Matcher m; /* h2_polymorphic_matcher or immediate value or h2_matches */
   explicit h2_not_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<A>(m).matches(a, c.update_negative(!c.negative));
   }
   virtual h2_line expection(const C& c) const override
   {
      return h2_matches_expection(m, c.update_negative(!c.negative));
   }
};

template <typename MatcherT, typename MatcherF>
struct h2_conditional_matches : h2_matches {
   const bool cond;
   const MatcherT mt;
   const MatcherF mf;
   explicit h2_conditional_matches(const bool cond_, const MatcherT& mt_, const MatcherF& mf_) : cond(cond_), mt(mt_), mf(mf_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return cond ? h2_matcher_cast<A>(mt).matches(a, c) : h2_matcher_cast<A>(mf).matches(a, c);
   }
   virtual h2_line expection(const C& c) const override
   {
      return cond ? h2_matches_expection(mt, c) : h2_matches_expection(mf, c);
   }
};

template <typename MatcherL, typename MatcherR>
struct h2_and_matches : h2_matches {
   const MatcherL ml;
   const MatcherR mr;
   explicit h2_and_matches(const MatcherL& ml_, const MatcherR& mr_) : ml(ml_), mr(mr_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<A>(ml).matches(a, c.update_negative(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<A>(mr).matches(a, c.update_negative(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      auto sl = h2_matches_expection(ml, c.update_negative(false));
      if (sl.has(" or ")) sl = gray("(") + sl + gray(")");
      auto sr = h2_matches_expection(mr, c.update_negative(false));
      if (sr.has(" and ") || sr.has(" or ")) sr = gray("(") + sr + gray(")");
      h2_line s = sl + " and " + sr;
      if (c.negative) s = gray("(") + s + gray(")");
      return c.update_caseless(false).pre() + s;
   }
};

template <typename MatcherL, typename MatcherR>
struct h2_or_matches : h2_matches {
   const MatcherL ml;
   const MatcherR mr;
   explicit h2_or_matches(const MatcherL& ml_, const MatcherR& mr_) : ml(ml_), mr(mr_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(ml).matches(a, c.update_negative(false));
      h2_fail* f2 = h2_matcher_cast<A>(mr).matches(a, c.update_negative(false));
      if (c.fit(!f1 || !f2)) {
         if (f1) delete f1;
         if (f2) delete f2;
         return nullptr;
      }
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, f1);
      h2_fail::append_subling(fails, f2);

      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      auto sl = h2_matches_expection(ml, c.update_negative(false));
      auto sr = h2_matches_expection(mr, c.update_negative(false));
      if (sr.has(" or ")) sr = gray("(") + sr + gray(")");
      h2_line s = sl + " or " + sr;
      if (c.negative) s = gray("(") + s + gray(")");
      return c.update_caseless(false).pre() + s;
   }
};

template <typename... Matchers>
struct h2_allof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_allof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_vector<h2_matcher<A>> matchers;
      e.matchers(matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = matchers[i].matches(a, c.update_negative(false));  // dont not transfer down
         if (fail) fail->seqno = (int)i;
         h2_fail::append_subling(fails, fail);
      }

      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + "AllOf" + gray("(") + e.expection(c.update_negative(false)) + gray(")");
   }
};

template <typename... Matchers>
struct h2_anyof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_anyof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_vector<h2_matcher<A>> matchers;
      e.matchers(matchers);

      int count = 0;
      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = matchers[i].matches(a, c.update_negative(false));
         if (!fail) {
            count++;
            break;
         }
         if (fail) fail->seqno = (int)i;
         h2_fail::append_subling(fails, fail);
      }

      if (c.fit(0 < count)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + "AnyOf" + gray("(") + e.expection(c.update_negative(false)) + gray(")");
   }
};

template <typename... Matchers>
struct h2_noneof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_noneof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_vector<h2_matcher<A>> matchers;
      e.matchers(matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = matchers[i].matches(a, c.update_negative(false));
         if (fail) delete fail;
         else {
            fail = h2_fail::new_normal("should not match " + matchers[i].expection(c).brush("green"));
            fail->seqno = (int)i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + "NoneOf" + gray("(") + e.expection(c.update_negative(false)) + gray(")");
   }
};

template <typename T, typename P = h2_polymorphic_matcher<h2_not_matches<T>>>
inline P Not(T expect) { return P(h2_not_matches<T>(expect)); }

template <typename T, typename F, typename P = h2_polymorphic_matcher<h2_conditional_matches<T, F>>>
inline P Conditional(bool cond, T true_expect, F false_expect) { return P(h2_conditional_matches<T, F>(cond, true_expect, false_expect)); }

template <typename... T, typename M = h2_allof_matches<typename std::decay<const T&>::type...>, typename P = h2_polymorphic_matcher<M>>
inline P AllOf(const T&... expects) { return P(M(expects...)); }
template <typename... T, typename M = h2_anyof_matches<typename std::decay<const T&>::type...>, typename P = h2_polymorphic_matcher<M>>
inline P AnyOf(const T&... expects) { return P(M(expects...)); }
template <typename... T, typename M = h2_noneof_matches<typename std::decay<const T&>::type...>, typename P = h2_polymorphic_matcher<M>>
inline P NoneOf(const T&... expects) { return P(M(expects...)); }

template <typename T1, typename T2, typename M = h2_and_matches<h2_polymorphic_matcher<T1>, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator&&(const h2_polymorphic_matcher<T1>& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(expect1, expect2)); }
template <typename T1, typename T2, typename E2 = h2_matcher<typename h2_decay<T2>::type>, typename M = h2_and_matches<h2_polymorphic_matcher<T1>, E2>, typename P = h2_polymorphic_matcher<M>>
inline P operator&&(const h2_polymorphic_matcher<T1>& expect1, const T2& expect2) { return P(M(expect1, E2(expect2))); }
template <typename T1, typename T2, typename E1 = h2_matcher<typename h2_decay<T1>::type>, typename M = h2_and_matches<E1, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator&&(const T1& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(E1(expect1), expect2)); }

template <typename T1, typename T2, typename M = h2_or_matches<h2_polymorphic_matcher<T1>, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator||(const h2_polymorphic_matcher<T1>& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(expect1, expect2)); }
template <typename T1, typename T2, typename E2 = h2_matcher<typename h2_decay<T2>::type>, typename M = h2_or_matches<h2_polymorphic_matcher<T1>, E2>, typename P = h2_polymorphic_matcher<M>>
inline P operator||(const h2_polymorphic_matcher<T1>& expect1, const T2& expect2) { return P(M(expect1, E2(expect2))); }
template <typename T1, typename T2, typename E1 = h2_matcher<typename h2_decay<T1>::type>, typename M = h2_or_matches<E1, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator||(const T1& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(E1(expect1), expect2)); }
// source/matcher/h2_matches_inequation.hpp
template <typename E>
struct h2_matches_ge : h2_matches {
   const E e;
   explicit h2_matches_ge(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      if (c.fit(a >= e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : "≥") + h2_stringify(e, true);
   }
};

template <typename E>
struct h2_matches_gt : h2_matches {
   const E e;
   explicit h2_matches_gt(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      if (c.fit(a > e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : ">") + h2_stringify(e);
   }
};

template <typename E>
struct h2_matches_le : h2_matches {
   const E e;
   explicit h2_matches_le(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      if (c.fit(a <= e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : "≤") + h2_stringify(e);
   }
};

template <typename E>
struct h2_matches_lt : h2_matches {
   const E e;
   explicit h2_matches_lt(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      if (c.fit(a < e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : "<") + h2_stringify(e);
   }
};

template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_not_matches<E>>>
inline P Nq(const T& expect) { return P(h2_not_matches<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_ge<E>>>
inline P Ge(const T& expect) { return P(h2_matches_ge<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_gt<E>>>
inline P Gt(const T& expect) { return P(h2_matches_gt<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_le<E>>>
inline P Le(const T& expect) { return P(h2_matches_le<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_lt<E>>>
inline P Lt(const T& expect) { return P(h2_matches_lt<E>(expect)); }
// source/matcher/h2_matches_memcmp.hpp
struct h2_memcmp_util {
   static bool bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits);
   static bool is_hex_string(const char* s);
   static bool is_bin_string(const char* s);
   static size_t bin_to_bits(const char* bin, unsigned char* bytes);
   static size_t hex_to_bytes(const char* hex, unsigned char* bytes);
};

template <typename E>
struct h2_matches_memcmp : h2_matches {
   const E buffer;
   const size_t dimension, size, width;
   explicit h2_matches_memcmp(const E buffer_, const size_t dimension_, const size_t size_, const size_t width_) : buffer(buffer_), dimension(dimension_), size(size_), width(width_) {}
   h2_fail* matches(const void* a, const C& c) const
   {
      unsigned char* e = (unsigned char*)buffer;
      const char* b = (const char*)e;
      size_t z = size, w = width;
      bool result = true;
      do {
         if (!w && !z && std::is_convertible<E, h2_string>::value) { /* deduce by string format */
            z = strlen(b);
            w = 8;
            if (!strcmp(b, (const char*)a)) break;
            if (h2_memcmp_util::is_bin_string(b)) {
               e = (unsigned char*)alloca(z);
               z = h2_memcmp_util::bin_to_bits(b, e);
               w = 1;
            } else if (h2_memcmp_util::is_hex_string(b)) {
               e = (unsigned char*)alloca(z);
               z = h2_memcmp_util::hex_to_bytes(b, e);
               w = 8;
            }
         }
         if (!w) w = h2_sizeof_pointee<E>::value * 8; /* deduce by data type */
         if (!z) z = dimension;                       /* deduce by array dimension */
         if (!z || !w) return h2_fail::new_normal(color("size", "red") + " not specified " + gray("in ") + color("Me(buffer, ", "cyan") + color("size", "red") + gray(", width") + color(")", "cyan"));
         result = h2_memcmp_util::bits_equal(e, (const unsigned char*)a, z * w);
      } while (0);
      if (c.fit(result)) return nullptr;
      return h2_fail::new_memcmp(e, (const unsigned char*)a, z, w);
   }
   virtual h2_line expection(const C& c) const override { return c.pre() + "Me()"; }
};

template <typename T, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_memcmp<E>>>
inline P Memcmp(const T buffer, const size_t dimension, const size_t size = 0, const size_t width = 0) { return P(h2_matches_memcmp<E>((E)buffer, dimension, size, width)); }
inline auto Memcmp(const h2_string& buffer, const size_t dimension) -> h2_polymorphic_matcher<h2_matches_memcmp<h2_string>> { return h2_polymorphic_matcher<h2_matches_memcmp<h2_string>>(h2_matches_memcmp<h2_string>(buffer, dimension, buffer.size(), 8)); }

#define H2Me(buffer, ...) H2PP_CAT(H2Me_, H2PP_IS_EMPTY(__VA_ARGS__))(buffer, std::extent<decltype(buffer)>::value, __VA_ARGS__)
#define H2Me_1(buffer, dimension, ...) h2::Memcmp(buffer, dimension)
#define H2Me_0(buffer, dimension, ...) h2::Memcmp(buffer, dimension, __VA_ARGS__)
// source/matcher/h2_matches_strcmp.hpp
struct h2_matches_strcmp : h2_matches {
   const h2_string e;
   explicit h2_matches_strcmp(const h2_string& e_) : e(e_) {}
   h2_fail* matches(const h2_string& a, const C& c) const;
   virtual h2_line expection(const C& c) const override;
};

struct h2_matches_string : h2_matches {
   const char* name;
   const h2_string e;
   explicit h2_matches_string(const char* name_, const h2_string& e_) : name(name_), e(e_) {}
   virtual h2_line expection(const C& c) const override;
};

struct h2_matches_regex : h2_matches_string {
   explicit h2_matches_regex(const h2_string& e_) : h2_matches_string("Re", e_) {}
   h2_fail* matches(const h2_string& a, const C& c) const;
};

struct h2_matches_wildcard : h2_matches_string {
   explicit h2_matches_wildcard(const h2_string& e_) : h2_matches_string("We", e_) {}
   h2_fail* matches(const h2_string& a, const C& c) const;
};

struct h2_matches_substr : h2_matches_string {
   explicit h2_matches_substr(const h2_string& substring) : h2_matches_string("Substr", substring) {}
   h2_fail* matches(const h2_string& a, const C& c) const;
};

struct h2_matches_startswith : h2_matches_string {
   explicit h2_matches_startswith(const h2_string& prefix_string) : h2_matches_string("StartsWith", prefix_string) {}
   h2_fail* matches(const h2_string& a, const C& c) const;
};

struct h2_matches_endswith : h2_matches_string {
   explicit h2_matches_endswith(const h2_string& suffix_string) : h2_matches_string("EndsWith", suffix_string) {}
   h2_fail* matches(const h2_string& a, const C& c) const;
};

struct h2_caseless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(const h2_matcher<h2_string>& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const { return m.matches(a, c.update_caseless(true)); }
   virtual h2_line expection(const C& c) const override { return m.expection(c.update_caseless(true)); }
};

struct h2_spaceless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_spaceless_matches(const h2_matcher<h2_string>& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const { return m.matches(a, c.update_spaceless(true)); }
   virtual h2_line expection(const C& c) const override { return m.expection(c.update_spaceless(true)); }
};

inline auto Se(const h2_string& expect) -> h2_polymorphic_matcher<h2_matches_strcmp> { return h2_polymorphic_matcher<h2_matches_strcmp>(h2_matches_strcmp(expect)); }
inline auto Re(const h2_string& regex_pattern) -> h2_polymorphic_matcher<h2_matches_regex> { return h2_polymorphic_matcher<h2_matches_regex>(h2_matches_regex(regex_pattern)); }
inline auto We(const h2_string& wildcard_pattern) -> h2_polymorphic_matcher<h2_matches_wildcard> { return h2_polymorphic_matcher<h2_matches_wildcard>(h2_matches_wildcard(wildcard_pattern)); }
inline auto Substr(const h2_string& substring) -> h2_polymorphic_matcher<h2_matches_substr> { return h2_polymorphic_matcher<h2_matches_substr>(h2_matches_substr(substring)); }
inline auto StartsWith(const h2_string& prefix_string) -> h2_polymorphic_matcher<h2_matches_startswith> { return h2_polymorphic_matcher<h2_matches_startswith>(h2_matches_startswith(prefix_string)); }
inline auto EndsWith(const h2_string& suffix_string) -> h2_polymorphic_matcher<h2_matches_endswith> { return h2_polymorphic_matcher<h2_matches_endswith>(h2_matches_endswith(suffix_string)); }

template <typename T, typename P = h2_polymorphic_matcher<h2_caseless_matches>>
inline P CaseLess(const T& expect) { return P(h2_caseless_matches(h2_matcher<h2_string>(expect))); }
template <typename T, typename P = h2_polymorphic_matcher<h2_spaceless_matches>>
inline P SpaceLess(const T& expect) { return P(h2_spaceless_matches(h2_matcher<h2_string>(expect))); }
// source/matcher/h2_matches_json.hpp
struct h2_matches_json : h2_matches {
   const h2_string e, selector;
   explicit h2_matches_json(const h2_string& e_, const h2_string& selector_) : e(e_), selector(selector_) {}
   h2_fail* matches(const h2_string& a, const C& c) const;
   virtual h2_line expection(const C& c) const override;
};

inline auto Je(const h2_string& expect, const h2_string& selector = "") -> h2_polymorphic_matcher<h2_matches_json> { return h2_polymorphic_matcher<h2_matches_json>(h2_matches_json(expect, selector)); }
// source/matcher/h2_matches_pointee.hpp
template <typename Matcher>
struct h2_pointee_matches : h2_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<typename h2_pointee_type<A>::type>(m).matches(*a, c.clear_size());
   }
   virtual h2_line expection(const C& c) const override
   {
      return h2_matches_expection(m, c);
   }
};

template <typename T, typename P = h2_polymorphic_matcher<h2_pointee_matches<T>>>
inline P Pointee(T expect) { return P(h2_pointee_matches<T>(expect)); }
// source/matcher/h2_matches_member.hpp
template <typename Matcher, typename Class, typename Data>
struct h2_member_data_matches : h2_matches {
   const Matcher m;
   const Data Class::*data;
   explicit h2_member_data_matches(Matcher m_, Data Class::*data_) : m(m_), data(data_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<Data>(m).matches(h2_pointer_if(a)->*data, c);
   }
   virtual h2_line expection(const C& c) const override { return h2_matches_expection(m, c); }
};

template <typename T, typename Class, typename Data, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_data_matches<E, Class, Data>>>
inline P Member_member(T expect, Data Class::*data) { return P(h2_member_data_matches<E, Class, Data>(expect, data)); }

template <typename T, typename U = typename std::remove_reference<T>::type>
struct mfp_decay {  // std::decay drop reference, std::make_tuple (template function argument deduce) also drop reference, use foreach macro
   template <typename R> struct __reference_wrapper : std::false_type {
      typedef R underlying_type;
   };
   template <typename R> struct __reference_wrapper<std::reference_wrapper<R>> : std::true_type {
      typedef R& underlying_type;
   };
   typedef typename std::conditional<__reference_wrapper<T>::value,
                                     typename __reference_wrapper<T>::underlying_type,
                                     typename std::conditional<std::is_array<U>::value,
                                                               typename std::remove_extent<U>::type*,
                                                               typename std::conditional<std::is_function<U>::value,
                                                                                         typename std::add_pointer<U>::type,
                                                                                         T>::type>::type>::type type;
};

#if __cplusplus >= 201402L || (defined _MSVC_LANG && _MSVC_LANG >= 201402L)  // Return type deduction / auto
// h2_apply => std::apply since C++17
template <typename A, typename M, typename T, std::size_t... S>
static inline auto h2_apply(A& a, M method, T args, h2_index_sequence<S...>) { return (a.*method)(std::get<S>(args)...); }
template <typename A, typename M, typename... Args>
static inline auto h2_apply(A& a, M method, std::tuple<Args...> args) { return h2_apply(a, method, args, h2_make_index_sequence<sizeof...(Args)>()); }

template <typename Matcher, typename Method, typename Args = std::tuple<>>
struct h2_member_method_matches : h2_matches {
   const Matcher m;
   Method method;
   Args args;
   explicit h2_member_method_matches(Matcher m_, Method method_, Args args_) : m(m_), method(method_), args(args_) {}

   template <typename A, typename P = typename std::add_pointer<typename std::remove_pointer<typename std::decay<A>::type>::type>::type>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<decltype(h2_apply(*(P) nullptr, method, args))>(m).matches(h2_apply(*(P)h2_pointer_if(a), method, args), c);
   }
   virtual h2_line expection(const C& c) const override { return h2_matches_expection(m, c); }
};

template <typename... Args> struct mfp {  // mfp improve std::mem_fn for overloading
   template <typename Class, typename ReturnType> static auto get(ReturnType (Class::*f)(Args...)) { return f; }
   template <typename Class, typename ReturnType> static auto get(ReturnType (Class::*f)(Args...) const) { return f; }
};

template <typename T, typename Method, typename Args = std::tuple<>, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, Method, Args>>>
inline P Member_method(T expect, Method method, Args args) { return P(h2_member_method_matches<E, Method, Args>(expect, method, args)); }

template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)()>>>
inline P Member_member(T expect, ReturnType (Class::*method)()) { return P(h2_member_method_matches<E, ReturnType (Class::*)()>(expect, method, std::tuple<>())); }
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() const>>>
inline P Member_member(T expect, ReturnType (Class::*method)() const) { return P(h2_member_method_matches<E, ReturnType (Class::*)() const>(expect, method, std::tuple<>())); }
#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)
// noexcept-specification is a part of the function type since C++17
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() noexcept>>>
inline P Member_member(T expect, ReturnType (Class::*method)() noexcept) { return P(h2_member_method_matches<E, ReturnType (Class::*)() noexcept>(expect, method, std::tuple<>())); }
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() const noexcept>>>
inline P Member_member(T expect, ReturnType (Class::*method)() const noexcept) { return P(h2_member_method_matches<E, ReturnType (Class::*)() const noexcept>(expect, method, std::tuple<>())); }
#endif
#endif

#define H2Member(expect, member, ...) H2PP_RESCAN(H2PP_CAT2(H2Member_, H2PP_IS_EMPTY(__VA_ARGS__))(expect, member, __VA_ARGS__))
#define H2Member_1(expect, member, ...) h2::Member_member(expect, member)
// #define H2Member_0(expect, Class, member, ...) h2::Member_method(expect, h2::mfp<H2PP_FOREACH((, ), __H2MfpDecay, , __VA_ARGS__)>::get(member), std::tuple<H2PP_FOREACH((, ), __H2MfpDecay, , __VA_ARGS__)>(__VA_ARGS__))  // without arguments braced
#define H2Member_0(...) H2PP_NCALL(H2Member_0_, 3, __VA_ARGS__)
#define H2Member_0_0(expect, member, ...) h2::Member_method(expect, h2::mfp<H2PP_FOREACH((, ), __H2MfpDecay, , __VA_ARGS__)>::get(member), std::tuple<H2PP_FOREACH((, ), __H2MfpDecay, , __VA_ARGS__)>(__VA_ARGS__))
#define H2Member_0_1(expect, member, ...) H2PP_RESCAN(H2PP_CAT2(H2Member_0_1_, H2PP_IS_PARENTHESIS(__VA_ARGS__))(expect, member, __VA_ARGS__))
#define H2Member_0_1_0(expect, member, ...) h2::Member_method(expect, h2::mfp<H2PP_FOREACH((, ), __H2MfpDecay, , __VA_ARGS__)>::get(member), std::tuple<H2PP_FOREACH((, ), __H2MfpDecay, , __VA_ARGS__)>(__VA_ARGS__))
#define H2Member_0_1_1(expect, member, ...) h2::Member_method(expect, h2::mfp<H2PP_FOREACH((, ), __H2MfpDecay, , H2PP_REMOVE_PARENTHESES(__VA_ARGS__))>::get(member), std::tuple<H2PP_FOREACH((, ), __H2MfpDecay, , H2PP_REMOVE_PARENTHESES(__VA_ARGS__))> __VA_ARGS__)
#define __H2MfpDecay(Dummy, i, x) h2::mfp_decay<decltype(x)>::type
// source/matcher/h2_matcher.cpp
template <typename T>
inline h2_matcher<T>::h2_matcher() { *this = h2_polymorphic_matcher<h2_matches_any>(h2_matches_any()); }

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = Equals(value); }
// source/matcher/h2_matcher_customize.hpp
#define __H2MATCHES(name, args, message)                                                      \
   const char* matcher_name = name;                                                           \
   template <typename A> bool __matches(const A& a) const;                                    \
   template <typename A> h2::h2_fail* matches(const A& a, const h2::C& c) const               \
   {                                                                                          \
      if (c.fit(__matches(a))) return nullptr;                                                \
      h2::h2_fail* fail = h2::h2_fail::new_unexpect(expection(c), h2::h2_stringify(a, true)); \
      h2::h2_ostringstream t;                                                                 \
      t << H2PP_REMOVE_PARENTHESES(message);                                                  \
      fail->user_explain = t.str().c_str();                                                   \
      return fail;                                                                            \
   }                                                                                          \
   virtual h2::h2_line expection(const h2::C& c) const override { return c.pre() + matcher_name + h2::gray("(") + args + h2::gray(")"); }

#define H2MATCHER0(name, message)                                                                                                                   \
   struct h2_##name##_matches : h2::h2_matches {                                                                                                    \
      explicit h2_##name##_matches() {}                                                                                                             \
      __H2MATCHES(#name, "", message)                                                                                                               \
   };                                                                                                                                               \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches> name() { return h2::h2_polymorphic_matcher<h2_##name##_matches>(h2_##name##_matches()); } \
   template <typename A> bool h2_##name##_matches::__matches(const A& a) const

#define H2MATCHER1(name, e1, message)                                                                                                                                                                 \
   template <typename E1> struct h2_##name##_matches : h2::h2_matches {                                                                                                                               \
      const E1 e1;                                                                                                                                                                                    \
      explicit h2_##name##_matches(const E1& _e1) : e1(_e1) {}                                                                                                                                        \
      __H2MATCHES(#name, h2::h2_stringify(e1, true), message)                                                                                                                                         \
   };                                                                                                                                                                                                 \
   template <typename E1> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1>> name(const E1 _e1) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1>>(h2_##name##_matches<E1>(_e1)); } \
   template <typename E1> template <typename A> bool h2_##name##_matches<E1>::__matches(const A& a) const

#define H2MATCHER2(name, e1, e2, message)                                                                                                                                                                                                         \
   template <typename E1, typename E2> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                              \
      const E1 e1;                                                                                                                                                                                                                                \
      const E2 e2;                                                                                                                                                                                                                                \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2) : e1(_e1), e2(_e2) {}                                                                                                                                                            \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true), message)                                                                                                                                       \
   };                                                                                                                                                                                                                                             \
   template <typename E1, typename E2> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>> name(const E1 _e1, const E2 _e2) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>>(h2_##name##_matches<E1, E2>(_e1, _e2)); } \
   template <typename E1, typename E2> template <typename A> bool h2_##name##_matches<E1, E2>::__matches(const A& a) const

#define H2MATCHER3(name, e1, e2, e3, message)                                                                                                                                                                                                                                                 \
   template <typename E1, typename E2, typename E3> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                                                             \
      const E1 e1;                                                                                                                                                                                                                                                                            \
      const E2 e2;                                                                                                                                                                                                                                                                            \
      const E3 e3;                                                                                                                                                                                                                                                                            \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3) : e1(_e1), e2(_e2), e3(_e3) {}                                                                                                                                                                                \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true) + h2::gray(", ") + h2::h2_stringify(e3, true), message)                                                                                                                                     \
   };                                                                                                                                                                                                                                                                                         \
   template <typename E1, typename E2, typename E3> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>> name(const E1 _e1, const E2 _e2, const E3 _e3) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>>(h2_##name##_matches<E1, E2, E3>(_e1, _e2, _e3)); } \
   template <typename E1, typename E2, typename E3> template <typename A> bool h2_##name##_matches<E1, E2, E3>::__matches(const A& a) const

#define H2MATCHER4(name, e1, e2, e3, e4, message)                                                                                                                                                                                                                                                                                         \
   template <typename E1, typename E2, typename E3, typename E4> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                                                                                            \
      const E1 e1;                                                                                                                                                                                                                                                                                                                        \
      const E2 e2;                                                                                                                                                                                                                                                                                                                        \
      const E3 e3;                                                                                                                                                                                                                                                                                                                        \
      const E4 e4;                                                                                                                                                                                                                                                                                                                        \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4) : e1(_e1), e2(_e2), e3(_e3), e4(_e4) {}                                                                                                                                                                                                    \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true) + h2::gray(", ") + h2::h2_stringify(e3, true) + h2::gray(", ") + h2::h2_stringify(e4, true), message)                                                                                                                                   \
   };                                                                                                                                                                                                                                                                                                                                     \
   template <typename E1, typename E2, typename E3, typename E4> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>>(h2_##name##_matches<E1, E2, E3, E4>(_e1, _e2, _e3, _e4)); } \
   template <typename E1, typename E2, typename E3, typename E4> template <typename A> bool h2_##name##_matches<E1, E2, E3, E4>::__matches(const A& a) const

#define H2MATCHER5(name, e1, e2, e3, e4, e5, message)                                                                                                                                                                                                                                                                                                                                 \
   template <typename E1, typename E2, typename E3, typename E4, typename E5> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                                                                                                                           \
      const E1 e1;                                                                                                                                                                                                                                                                                                                                                                    \
      const E2 e2;                                                                                                                                                                                                                                                                                                                                                                    \
      const E3 e3;                                                                                                                                                                                                                                                                                                                                                                    \
      const E4 e4;                                                                                                                                                                                                                                                                                                                                                                    \
      const E5 e5;                                                                                                                                                                                                                                                                                                                                                                    \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4, const E5& _e5) : e1(_e1), e2(_e2), e3(_e3), e4(_e4), e5(_e5) {}                                                                                                                                                                                                                        \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true) + h2::gray(", ") + h2::h2_stringify(e3, true) + h2::gray(", ") + h2::h2_stringify(e4, true) + h2::gray(", ") + h2::h2_stringify(e5, true), message)                                                                                                                                 \
   };                                                                                                                                                                                                                                                                                                                                                                                 \
   template <typename E1, typename E2, typename E3, typename E4, typename E5> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4, const E5 _e5) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>>(h2_##name##_matches<E1, E2, E3, E4, E5>(_e1, _e2, _e3, _e4, _e5)); } \
   template <typename E1, typename E2, typename E3, typename E4, typename E5> template <typename A> bool h2_##name##_matches<E1, E2, E3, E4, E5>::__matches(const A& a) const

#define H2MATCHER_1(name) H2MATCHER0(name, (""))

#define H2MATCHER_2(name, t) H2PP_CAT(H2MATCHER_2_, H2PP_IS_PARENTHESIS(t))(name, t)
#define H2MATCHER_2_0(name, e1) H2MATCHER1(name, e1, (""))
#define H2MATCHER_2_1(name, message) H2MATCHER0(name, message)

#define H2MATCHER_3(name, e1, t) H2PP_CAT(H2MATCHER_3_, H2PP_IS_PARENTHESIS(t))(name, e1, t)
#define H2MATCHER_3_0(name, e1, e2) H2MATCHER2(name, e1, e2, (""))
#define H2MATCHER_3_1(name, e1, message) H2MATCHER1(name, e1, message)

#define H2MATCHER_4(name, e1, e2, t) H2PP_CAT(H2MATCHER_4_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, t)
#define H2MATCHER_4_0(name, e1, e2, e3) H2MATCHER3(name, e1, e2, e3, (""))
#define H2MATCHER_4_1(name, e1, e2, message) H2MATCHER2(name, e1, e2, message)

#define H2MATCHER_5(name, e1, e2, e3, t) H2PP_CAT(H2MATCHER_5_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, e3, t)
#define H2MATCHER_5_0(name, e1, e2, e3, e4) H2MATCHER4(name, e1, e2, e3, e4, (""))
#define H2MATCHER_5_1(name, e1, e2, e3, message) H2MATCHER3(name, e1, e2, e3, message)

#define H2MATCHER_6(name, e1, e2, e3, e4, t) H2PP_CAT(H2MATCHER_6_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, e3, e4, t)
#define H2MATCHER_6_0(name, e1, e2, e3, e4, e5) H2MATCHER5(name, e1, e2, e3, e4, e5, (""))
#define H2MATCHER_6_1(name, e1, e2, e3, e4, message) H2MATCHER4(name, e1, e2, e3, e4, message)

#define H2MATCHER_7(name, e1, e2, e3, e4, e5, t) H2PP_CAT(H2MATCHER_7_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, e3, e4, e5, t)
#define H2MATCHER_7_0(name, e1, e2, e3, e4, e5, e6)
#define H2MATCHER_7_1(name, e1, e2, e3, e4, e5, message) H2MATCHER5(name, e1, e2, e3, e4, e5, message)

#define H2MATCHER(...) H2PP_VCALL(H2MATCHER_, __VA_ARGS__)
// source/stub/h2_fp.hpp
/* clang-format off */
template <typename T> static void h2_destructible(...) {}
template <typename T> static auto h2_destructible(T* o) -> typename std::enable_if<std::is_destructible<T>::value && !std::is_abstract<T>::value, void>::type { o->~T(); }

template <typename T, int I> struct h2_constructible_error { static T* O(void* m) { return static_cast<T*>(m = (void*)I); } };
template <typename T> struct h2_constructible0 : std::true_type { static T* O(void* m) { return new (m) T(); } };

template <typename, typename> struct h2_constructible1_impl : std::false_type {};
template <typename, typename> struct h2_constructible2_impl : std::false_type {};
template <typename, typename> struct h2_constructible3_impl : std::false_type {};
template <typename, typename> struct h2_constructible4_impl : std::false_type {};
template <typename, typename> struct h2_constructible5_impl : std::false_type {};
template <typename, typename> struct h2_constructible6_impl : std::false_type {};
template <typename, typename> struct h2_constructible7_impl : std::false_type {};
template <typename, typename> struct h2_constructible8_impl : std::false_type {};
template <typename, typename> struct h2_constructible9_impl : std::false_type {};

#if !(defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)) // not gcc4
template <typename T> struct h2_constructible1_impl<T, decltype(T({}))> : std::true_type { static T* O(void* m) { return new (m) T({}); } };
template <typename T> struct h2_constructible2_impl<T, decltype(T({}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}); } };
template <typename T> struct h2_constructible3_impl<T, decltype(T({}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}); } };
template <typename T> struct h2_constructible4_impl<T, decltype(T({}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}); } };
template <typename T> struct h2_constructible5_impl<T, decltype(T({}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible6_impl<T, decltype(T({}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible7_impl<T, decltype(T({}, {}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible8_impl<T, decltype(T({}, {}, {}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible9_impl<T, decltype(T({}, {}, {}, {}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}, {}); } };
#endif

template <typename T> using h2_constructible1 = h2_constructible1_impl<T, T>;
template <typename T> using h2_constructible2 = h2_constructible2_impl<T, T>;
template <typename T> using h2_constructible3 = h2_constructible3_impl<T, T>;
template <typename T> using h2_constructible4 = h2_constructible4_impl<T, T>;
template <typename T> using h2_constructible5 = h2_constructible5_impl<T, T>;
template <typename T> using h2_constructible6 = h2_constructible6_impl<T, T>;
template <typename T> using h2_constructible7 = h2_constructible7_impl<T, T>;
template <typename T> using h2_constructible8 = h2_constructible8_impl<T, T>;
template <typename T> using h2_constructible9 = h2_constructible9_impl<T, T>;

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

static constexpr const char* const constructible_errors[] = {"", "abstract ", "non default constructor "};

template <typename = void, typename = void>
struct h2_fp {
   template <typename T>
   static void* get(T fp) { return h2_numberfy<void*>(fp); }
   static void* get(const char* fn)
   {
      h2_symbol* res[16];
      int n = h2_nm::get_by_name(fn, res, 16);
      if (n == 1) return h2_load::addr_to_ptr(res[0]->addr);
      h2_vector<h2_string> candidates;
      for (int i = 0; i < n; ++i) candidates.push_back(res[i]->name());
      h2_runner::failing(h2_fail::new_symbol(fn, candidates));
      return nullptr;
   }
};

template <typename ReturnType, typename... Args>
struct h2_fp<ReturnType(Args...)> {
   static void* get(ReturnType (*fp)(Args...)) { return h2_numberfy<void*>(fp); }
   static void* get(const char* fn) { return h2_fp<>::get(fn); }
};

template <typename Class, typename ReturnType, typename... Args>
struct h2_fp<Class, ReturnType(Args...)> {
   // static member function
   static void* get(ReturnType (*f)(Args...)) { return (void*)f; }
   static void* get(Class* o, ReturnType (*f)(Args...)) { return (void*)f; }

   // normal member function, const-qualified member functions
   static void* get(ReturnType (Class::*f)(Args...)) { return get1(h2_numberfy<void*>(f)); }
   static void* get(ReturnType (Class::*f)(Args...) const) { return get1(h2_numberfy<void*>(f)); }
   static void* get(Class* o, ReturnType (Class::*f)(Args...)) { return get2(o, h2_numberfy<void*>(f)); }
   static void* get(Class* o, ReturnType (Class::*f)(Args...) const) { return get2(o, h2_numberfy<void*>(f)); }

#if defined _MSC_VER
   // https://github.com/microsoft/Detours
   // &C::f1 ILT+165(??_9C$BAAA) 00007FF7987210AA
   // &C::f2 ILT+410(??_9C$B7AA) 00007FF79872119F
   // follow_jmp &C::f1 C::`vcall'{0}' 00007FF798721F2C
   // follow_jmp &C::f2 C::`vcall'{8}' 00007FF798721F24
   static void* get1(void* f)
   {
      long offset;
      if (!is_virtual_mfp(f, offset)) return f;

      Class* o = h2_constructible<Class>::O(alloca(sizeof(Class)));
      if (0 == (unsigned long long)o || 1 == (unsigned long long)o || 2 == (unsigned long long)o) {
         h2_runner::failing(h2_fail::new_symbol("vtable of " + h2_string(constructible_errors[(unsigned long long)o]) + h2_cxa::type_name<Class>(), {}, "  try: " + color("STUB/MOCK(", "yellow") + color("Object", "bold,red") + color(", Class, Method, ...)", "yellow")));
         return nullptr;
      }
      void* t = get_virtual_mfp(o, offset);
      h2_destructible<Class>(o);
      return t;
   }
   static void* get2(Class* o, void* f)
   {
      long offset;
      if (!is_virtual_mfp(f, offset)) return f;
      return get_virtual_mfp(o, offset);
   }

   static void* get_virtual_mfp(Class* o, long offset)
   {
      void** vtable = *(void***)o;
      return vtable[offset / sizeof(void*)];
   }
   static bool is_virtual_mfp(void* f, long& offset)
   {
      h2_symbol* symbol = h2_nm::get_by_addr((unsigned long long)h2_cxa::follow_jmp(f));
      if (!symbol) return false;
      char* p = strstr(symbol->name(), "::`vcall'{");
      if (!p) return false;  // not virtual member function
      offset = strtol(p + 10, nullptr, 10);
      return true;
   }
#else
   //  https://itanium-cxx-abi.github.io/cxx-abi/
   //  &Class::Method has separate representations for non-virtual and virtual functions.
   //  For non-virtual functions, it is the address of the function.
   //  For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
   //  The least-significant bit therefore discriminates between virtual and non-virtual functions.
   static void* get1(void* f)
   {
      if (!is_virtual_mfp(f)) return f;
      Class* o = h2_constructible<Class>::O(alloca(sizeof(Class)));
      if (2 < (unsigned long long)o) {
         void* t = get_virtual_mfp(o, f);
         h2_destructible<Class>(o);
         return t;
      }
      char symbol[1024];
      // https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling-special-vtables
      sprintf(symbol, "_ZTV%s", typeid(Class).name());  // mangle for "vtable for Class"
      unsigned long long relative_vtable = h2_nm::get_mangle(symbol);
      if (!relative_vtable) {
         sprintf(symbol, "_ZTI%s", typeid(Class).name());  // mangle for "typeinfo for Class" for abstract class
         relative_vtable = h2_nm::get_mangle(symbol);
         if (!relative_vtable) {
            h2_runner::failing(h2_fail::new_symbol("vtable of " + h2_string(constructible_errors[(unsigned long long)o]) + h2_cxa::type_name<Class>(), {}, "  try: " + color("STUB/MOCK(", "yellow") + color("Object", "bold,red") + color(", Class, Method, ...)", "yellow")));
            return nullptr;
         }
      }
      return get_virtual_mfp((void**)h2_load::vtable_to_ptr(relative_vtable), f);
   }
   static void* get2(Class* o, void* f)
   {
      if (!is_virtual_mfp(f)) return f;
      return get_virtual_mfp(*(void***)o, f);
   }

   static void* get_virtual_mfp(Class* o, void* f)
   {
      return get_virtual_mfp(*(void***)o, f);
   }
   static void* get_virtual_mfp(void** vtable, void* f)
   {
      return vtable[(h2_numberfy<unsigned long long>(f) & ~1ULL) / sizeof(void*)];
   }
   static bool is_virtual_mfp(void* f)
   {
      if (h2_numberfy<unsigned long long>(&h2_vtable_test::dummy) & 1)
         return (h2_numberfy<unsigned long long>(f) & 1) && (h2_numberfy<unsigned long long>(f) - 1) % sizeof(void*) == 0 && h2_numberfy<unsigned long long>(f) < 1000 * sizeof(void*);
      else
         return h2_numberfy<unsigned long long>(f) % sizeof(void*) == 0 && h2_numberfy<unsigned long long>(f) < 100 * sizeof(void*);
   }
#endif
};
// source/stub/h2_stubs.hpp
struct h2_stubs {
   static bool add(h2_list& stubs, void* srcfp, void* dstfp, const char* srcfn, const char* filine);
   static void clear(h2_list& stubs, void* srcfp);
   static void clear(h2_list& stubs);
};
// source/stub/h2_temporary.hpp
struct h2_stub_temporary_restore : h2_once {
   unsigned char current_opcode[32];
   void* srcfp;
   h2_stub_temporary_restore(void* srcfp);
   ~h2_stub_temporary_restore();
};
// source/stub/h2_stuber.hpp
namespace {
template <int Counter, typename Class, typename Signature> struct h2_stuber;
template <int Counter, typename Class, typename ReturnType, typename... Args>
struct h2_stuber<Counter, Class, ReturnType(Args...)> {
   h2_singleton(h2_stuber);
   void* srcfp;
   const char* srcfn;
   const char* filine;

   static h2_stuber& I(void* srcfp, const char* srcfn, const char* filine)
   {
      I().srcfp = srcfp;
      I().srcfn = srcfn;
      I().filine = filine;
      return I();
   }

#if defined _WIN32 && (defined __i386__ || defined _M_IX86)
   ReturnType (*dstfp_)(Class*, Args...);
   struct member_calling_conversions_wrapper {
      ReturnType fx(Args... args) { return I().dstfp_((Class*)this, std::forward<Args>(args)...); }
   };
   void operator=(ReturnType (*dstfp)(Class*, Args...))
   {
      dstfp_ = dstfp;
      h2_runner::stub(srcfp, h2_fp<member_calling_conversions_wrapper, ReturnType(Args...)>::get(&member_calling_conversions_wrapper::fx), srcfn, filine);
   }
#else
   void operator=(ReturnType (*dstfp)(Class*, Args...))  // captureless lambda implicit cast to function pointer
   {
      h2_runner::stub(srcfp, (void*)dstfp, srcfn, filine);
   }
#endif
   void operator=(ReturnType (*dstfp)(Args...))  // stub normal function
   {
      h2_runner::stub(srcfp, (void*)dstfp, srcfn, filine);
   }
};
}
// source/stub/h2_stub.hpp
// STUB(                          Source             , Destination )
// STUB(                Function, ReturnType(Args...), Destination )
// STUB(         Class, Method  , ReturnType(Args...), Destination )
// STUB( Object, Class, Method  , ReturnType(Args...), Destination )
#define H2STUB(...) H2PP_VCALL(H2STUB_, __VA_ARGS__)
#define H2STUB_2(Source, Destination) h2::h2_runner::stub(h2::h2_fp<>::get(Source), (void*)Destination, #Source, H2_FILINE)
#define H2STUB_3(Function, Signature, Destination) h2::h2_runner::stub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Destination), #Function, H2_FILINE)
#define H2STUB_4(Class, Method, Signature, Destination) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = Destination
#define H2STUB_5(Object, Class, Method, Signature, Destination) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = Destination

#define H2UNSTUB(...) H2PP_VCALL(H2UNSTUB_, __VA_ARGS__)
#define H2UNSTUB_1(Source) h2::h2_runner::unstub(h2::h2_fp<>::get(Source))
#define H2UNSTUB_2(Function, Signature) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define H2UNSTUB_3(Class, Method, Signature) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define H2UNSTUB_4(Object, Class, Method, Signature) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

// STUBS(                Function, ReturnType, Args ) { }
// STUBS(         Class, Method  , ReturnType, Args ) { }
// STUBS( Object, Class, Method  , ReturnType, Args ) { }
#define H2UNSTUBS(...) H2PP_VCALL(H2UNSTUBS_, __VA_ARGS__)
#define H2UNSTUBS_1(Source) h2::h2_runner::unstub(h2::h2_fp<>::get(Source))
#define H2UNSTUBS_3(Function, ReturnType, Args) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define H2UNSTUBS_4(Class, Method, ReturnType, Args) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define H2UNSTUBS_5(Object, Class, Method, ReturnType, Args) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

#define H2STUBS(...) H2PP_VCALL(H2STUBS_, __VA_ARGS__)
#define H2STUBS_3(Function, ReturnType, Args) h2::h2_stuber<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(H2PP_REMOVE_PARENTHESES_IF(Function)), #Function, H2_FILINE) = [] Args -> ReturnType
#define H2STUBS_4(Class, Method, ReturnType, Args) H2PP_CAT(H2STUBS_4_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args)
#define H2STUBS_4_0(Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2STUBS_4_1(Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2STUBS_5(Object, Class, Method, ReturnType, Args) H2PP_CAT(H2STUBS_5_, H2PP_IS_EMPTY Args)(Object, Class, Method, ReturnType, Args)
#define H2STUBS_5_0(Object, Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2STUBS_5_1(Object, Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
// source/mock/h2_routine.hpp
template <typename ReturnType>
struct h2_return : h2_libc {
   ReturnType value;
   h2_return() = delete;
   explicit h2_return(ReturnType _value) : value(_value){};
};

template <typename Class, typename Signature> struct h2_routine;

template <typename Class, typename ReturnType, typename... Args>
struct h2_routine<Class, ReturnType(Args...)> {
   ReturnType (*fp)(Args...) = nullptr;           // normal function pointer
   ReturnType (*mfp)(Class*, Args...) = nullptr;  // member function pointer
   h2_shared_ptr<h2_return<ReturnType>> ret;

   h2_routine() {}
   h2_routine(ReturnType r) : ret(new h2_return<ReturnType>(r)) {}
   h2_routine(ReturnType (*f)(Args...)) : fp(f) {}
   h2_routine(ReturnType (*f)(Class*, Args...)) : mfp(f) {}

   ReturnType operator()(Class* This, Args... args)
   {
      if (mfp) return mfp(This, args...);
      else if (fp) return fp(args...);
      else if (ret) return ret->value;
      /* never reach! make compiler happy. return uninitialized value is undefined behaviour, clang illegal instruction. */
      return ret->value;
   }
   operator bool() const
   {
      return fp || mfp || ret;
   }
   void clear()
   {
      fp = nullptr;
      mfp = nullptr;
      ret.reset();
   }
};

template <typename Class, typename... Args>
struct h2_routine<Class, void(Args...)> {
   void (*fp)(Args...) = nullptr;
   void (*mfp)(Class*, Args...) = nullptr;

   h2_routine() {}
   h2_routine(void (*f)(Args...)) : fp(f) {}
   h2_routine(void (*f)(Class*, Args...)) : mfp(f) {}

   void operator()(Class* This, Args... args)
   {
      if (mfp) mfp(This, args...);
      else if (fp) fp(args...);
   }
   operator bool() const
   {
      return fp || mfp;
   }
   void clear()
   {
      fp = nullptr;
      mfp = nullptr;
   }
};
// source/mock/h2_checkin.hpp
struct h2_checkin { /* 考勤 ; 函数被调次数期望 */
   int call = 0;
   const int least, most;
   const h2_string expr;

   h2_checkin(int least_, int most_, const h2_string& expr_) : least(least_), most(most_), expr(expr_) {}

   bool insufficient(/*不够*/) const { return call < least; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_excessive(/*过多*/) const { return most < call; }

   h2_fail* check(size_t index, size_t total, const char* srcfn) const;
   const char* actual() const;
   const char* expect() const;

   static h2_checkin Once() { return h2_checkin(1, 1, "Once"); }
   static h2_checkin Twice() { return h2_checkin(2, 2, "Twice"); }
   static h2_checkin Times(int count) { return h2_checkin(count, count, "Times"); }
   static h2_checkin Any() { return h2_checkin(0, 0x7fffffff, "Any"); }
   static h2_checkin Atleast(int count) { return h2_checkin(count, 0x7fffffff, "Atleast"); }
   static h2_checkin Atmost(int count) { return h2_checkin(0, count, "Atmost"); }
   static h2_checkin Between(int left, int right) { return h2_checkin(left, right, "Between"); }
};
// source/mock/h2_tuple.hpp
template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* __tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, 0>) { return nullptr; }
template <typename MatcherTuple, typename ArgumentTuple, std::size_t I>
inline h2_fail* __tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, I>)
{
   h2_fail* fails = __tuple_matches(matchers, arguments, std::integral_constant<std::size_t, I - 1>());
   h2_fail* fail = std::get<I - 1>(matchers).matches(std::get<I - 1>(arguments));
   if (fail) fail->seqno = I - 1;
   h2_fail::append_subling(fails, fail);
   h2_runner::asserts();
   return fails;
}
template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments)
{
   return __tuple_matches(matchers, arguments, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}

template <typename ArgumentTuple>
inline void __tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, 0>) {}
template <typename ArgumentTuple, std::size_t I>
inline void __tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, I>)
{
   __tuple_types<ArgumentTuple>(names, std::integral_constant<std::size_t, I - 1>());
   names.push_back(h2_cxa::type_name<typename std::tuple_element<I - 1, ArgumentTuple>::type>());
}
template <typename ArgumentTuple>
inline void tuple_types(h2_vector<h2_string>& names)
{
   return __tuple_types<ArgumentTuple>(names, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}
// source/mock/h2_mocker.hpp
struct h2_mocker_base : h2_libc {
   h2_list x;
   void *srcfp, *dstfp;
   const char *srcfn, *filine;
   char return_type[512];
   h2_vector<h2_string> argument_types;
   bool greed_mode = true;

   h2_line argument(int seq, const char* def = "") const;
   h2_line signature() const;

   h2_vector<h2_checkin> checkin_array;
   int checkin_index = 0;
   h2_fail* check() const;

   virtual void reset() = 0;
   void mock();

   void failing(h2_fail* fail, int checkin_offset) const;
};

namespace {

template <int Counter, typename Class, typename Signature> class h2_mocker;
template <int Counter, typename Class, typename ReturnType, typename... Args>
class h2_mocker<Counter, Class, ReturnType(Args...)> : h2_mocker_base {
   using ArgumentTuple = std::tuple<Args...>;

#define H2_Typedef_Matcher(__, i) h2_matcher<h2_nth_decay<i, Args...>>
   using MatcherTuple = std::tuple<H2PP_REPEAT((, ), H2_Typedef_Matcher, , 20)>;
#undef H2_Typedef_Matcher

   h2_vector<MatcherTuple> matcher_array;
   h2_vector<h2_routine<Class, ReturnType(Args...)>> routine_array;
   h2_routine<Class, ReturnType(Args...)> original;

   static ReturnType function_stub(Class* This, Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().srcfp);
      if (index == -1 || !I().routine_array[index])
         return I().original(This, std::forward<Args>(args)...);
      return I().routine_array[index](This, std::forward<Args>(args)...);
   }

   struct member_function_stub {  // wrap for calling conversions
      ReturnType fx(Args... args) { return function_stub((Class*)this, std::forward<Args>(args)...); }
   };

   static ReturnType normal_function_stub(Args... args) { return function_stub(nullptr, std::forward<Args>(args)...); }

   int matches(Args... args)
   {
      ArgumentTuple at = std::forward_as_tuple(std::forward<Args>(args)...);
      int checkin_offset = -1;
      for (int i = checkin_index; i < (int)checkin_array.size(); ++i) {
         h2_fail* fails = tuple_matches(matcher_array[i], at);
         if (fails) {
            if (checkin_offset != -1) break;
            if (checkin_array[i].is_satisfied()) { /* try next h2_checkin */
               delete fails;
               continue;
            }
            failing(fails, i);
         } else {
            checkin_index = i;
            checkin_offset = i;
            if (checkin_array[i].is_saturated()) continue;
            if (checkin_array[i].insufficient()) break;
            /* satisfied */
            if (greed_mode) break;
            /* continue */
         }
      }
      if (checkin_offset != -1) checkin_array[checkin_offset].call += 1;
      if (checkin_offset == -1) {
         h2_fail* fail = h2_fail::new_normal(signature(), filine);
         h2_fail* f = h2_fail::new_normal(srcfn + h2_stringify(at, true) + color(" unexpectedly", "red,bold") + " called");
         h2_fail::append_child(fail, f);
         h2_runner::failing(fail);
      }
      return checkin_offset;
   }

   void reset() override
   {
      checkin_array.clear();
      matcher_array.clear();
      routine_array.clear();
      checkin_index = 0;
      greed_mode = true;
   }

 public:
   static h2_mocker& I()
   {
      static h2_mocker* i = nullptr;
      if (!i) {
         i = new h2_mocker();
         h2_cxa::type_name<ReturnType>(i->return_type, sizeof(i->return_type));
         tuple_types<ArgumentTuple>(i->argument_types);
      }
      return *i;
   }

   static h2_mocker& I(void* srcfp, const char* srcfn, const char* filine)
   {
      if (std::is_same<std::false_type, Class>::value) {
         I().dstfp = (void*)normal_function_stub;
         I().original.fp = (ReturnType(*)(Args...))srcfp;
      } else {
         I().dstfp = h2::h2_fp<member_function_stub, ReturnType(Args...)>::get(&member_function_stub::fx);
         I().original.mfp = (ReturnType(*)(Class*, Args...))srcfp;
      }
      I().srcfp = srcfp;
      I().srcfn = srcfn;
      I().filine = filine;
      I().reset();
      I().mock();
      return I();
   }

   h2_mocker& greed(bool mode)
   {
      greed_mode = mode;
      return *this;
   }

#define H2_Default_Matcher(__, i) h2_matcher<h2_nth_decay<i, Args...>> _##i = {}
#define H2_Forward_Matcher(__, i) std::forward<h2_matcher<h2_nth_decay<i, Args...>>>(_##i)

   h2_mocker& Once(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Once());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Twice(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Twice());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Times(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Times(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Any(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Any());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Atleast(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atleast(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Atmost(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atmost(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Between(int left, int right, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Between(left, right));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& With(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      if (checkin_array.empty()) Any();
      matcher_array.back() = std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20));
      return *this;
   }

#undef H2_Default_Matcher
#undef H2_Forward_Matcher

#define H2_Th_Matcher(__, i)                                     \
   h2_mocker& Th##i(h2_matcher<h2_nth_decay<i, Args...>> e = {}) \
   {                                                             \
      if (checkin_array.empty()) Any();                          \
      std::get<i>(matcher_array.back()) = e;                     \
      return *this;                                              \
   }
   H2PP_REPEAT(, H2_Th_Matcher, , 20);
#undef H2_Th_Matcher

   h2_mocker& Return(h2_routine<Class, ReturnType(Args...)> r)
   {
      if (checkin_array.empty()) Any();
      if (!routine_array.empty()) routine_array.pop_back();
      routine_array.push_back(r);
      return *this;
   }

   void operator=(ReturnType (*f)(Args...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.fp = f;
   }

   void operator=(ReturnType (*f)(Class*, Args...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.mfp = f;
   }
};
}
// source/mock/h2_mocks.hpp
struct h2_mocks {
   static bool add(h2_list& mocks, void* mocker);
   static h2_fail* clear(h2_list& mocks, bool check);
};
// source/mock/h2_mock.hpp
// MOCK(                Function, ReturnType(Args...) )
// MOCK(         Class, Method  , ReturnType(Args...) )
// MOCK( Object, Class, Method  , ReturnType(Args...) )
#define H2MOCK(...) H2PP_VCALL(H2MOCK_, __VA_ARGS__)
#define H2MOCK_2(Function, Signature) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(Function), #Function, H2_FILINE)
#define H2MOCK_3(Class, Method, Signature) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE)
#define H2MOCK_4(Object, Class, Method, Signature) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE)

// MOCKS(                Function, ReturnType, (Args...), Inspection ) { }
// MOCKS(         Class, Method  , ReturnType, (Args...), Inspection ) { }
// MOCKS( Object, Class, Method  , ReturnType, (Args...), Inspection ) { }
#define H2MOCKS(...) H2PP_VCALL(H2MOCKS_, __VA_ARGS__)
#define H2MOCKS_4(Function, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(Function), #Function, H2_FILINE).Inspection = [] Args -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_5(Class, Method, ReturnType, Args, Inspection) H2PP_CAT(H2MOCKS_5_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspection)
#define H2MOCKS_5_0(Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_5_1(Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_6(Object, Class, Method, ReturnType, Args, Inspection) H2PP_CAT(H2MOCKS_6_, H2PP_IS_EMPTY Args)(Object, Class, Method, ReturnType, Args, Inspection)
#define H2MOCKS_6_0(Object, Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_6_1(Object, Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
// source/memory/h2_exempt.hpp
struct h2_exempt {
   h2_singleton(h2_exempt);
   void* fps[4096]{nullptr};
   static void setup();
   static void add_by_fp(void* fp);
   static void add_by_name(const char* fn);
};
// source/memory/h2_memory.hpp
struct h2_memory {
   static void initialize();
   static void finalize();
   static void hook(bool overrides = true);
   static void try_free(void* ptr);

   struct stack {
      static void root();
      static void push(const char* filine);
      static h2_fail* pop();
      static long long footprint();

      struct block : h2_once {
         bool unmem = false;
         block(const char* attributes, const char* filine);
         ~block();
      };
   };
};

template <typename T> inline void h2_unmem(T f) { h2_exempt::add_by_fp(h2_numberfy<void*>(f)); }
template <> inline void h2_unmem(const char* f) { h2_exempt::add_by_name(f); }
template <> inline void h2_unmem(char* f) { h2_exempt::add_by_name((const char*)f); }

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(h2::ss(Attributes), H2_FILINE); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2PP_UNIQUE())

#define H2UNMEM(...) H2PP_CAT(H2UNMEM_, H2PP_IS_EMPTY(__VA_ARGS__))(__VA_ARGS__)
#define H2UNMEM_1(...) __H2BLOCK("unmem", H2PP_UNIQUE())
#define H2UNMEM_0(...) h2::h2_unmem(__VA_ARGS__)
// source/except/h2_debug.hpp
#define h2_debug(shift, ...)                                            \
   do {                                                                 \
      if (!O.debugger_trap) {                                           \
         ::printf(" " __VA_ARGS__);                                     \
         ::printf(" %s : %d = %s\n", __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace::dump(shift).print(3);                            \
      }                                                                 \
   } while (0)
// source/except/h2_exception.hpp
using nothrow = std::integral_constant<int, 32717>;

struct h2_exception {
   h2_singleton(h2_exception);
   jmp_buf catch_hole;
   bool catching = false;
   h2_backtrace last_bt;
   void* thrown_exception;
   std::type_info* type_info;
   char last_type[1024];

   static void initialize();

   static h2_once try_catch();
   static void try_catch_finally(h2_fail* fail, const char* what);

   template <typename T, typename M>
   static auto matches(T* a, M m) -> typename std::enable_if<std::is_base_of<std::exception, T>::value, h2_fail*>::type
   {
      return h2_matcher_cast<const char*>(m).matches(a->what(), {0});
   }

   template <typename T, typename M>
   static auto matches(T* a, M m) -> typename std::enable_if<!std::is_base_of<std::exception, T>::value, h2_fail*>::type
   {
      return h2_matcher_cast<T>(m).matches(*a, {0});
   }

   template <typename T>
   static auto what(T* a) -> typename std::enable_if<std::is_base_of<std::exception, T>::value, const char*>::type { return a->what(); }
   template <typename T>
   static auto what(T* a) -> typename std::enable_if<!std::is_base_of<std::exception, T>::value, const char*>::type { return nullptr; }

   template <typename T, typename M>
   static void check_catch(M m, const char* filine)
   {
      h2_fail* fail = nullptr;
      if (std::is_same<nothrow, T>::value) {  // no throw check
         if (h2_exception::I().thrown_exception)
            fail = h2_fail::new_exception("was thrown but expect no throw", h2_exception::I().last_type, h2_exception::I().last_bt, false, filine);
      } else {
         if (!h2_exception::I().thrown_exception) {
            fail = h2_fail::new_normal("expect exception " + color(h2_cxa::demangle(typeid(T).name()), "green") + " thrown but not", filine);
         } else {
            if (!(typeid(T) == *h2_exception::I().type_info)) {  // check type
               fail = h2_fail::new_exception("was thrown but expect type is " + color(h2_cxa::demangle(typeid(T).name()), "green"), h2_exception::I().last_type, h2_exception::I().last_bt, false, filine);
            } else {  // check value
               fail = matches((T*)h2_exception::I().thrown_exception, m);
               if (fail) {
                  fail->filine = filine;
                  h2_fail::append_child(fail, h2_fail::new_exception("was thrown", h2_exception::I().last_type, h2_exception::I().last_bt, false, filine));
               }
            }
         }
      }
      try_catch_finally(fail, what((T*)h2_exception::I().thrown_exception));
   }
};

#define H2Catch_0() for (auto _Q__ = h2::h2_exception::try_catch(); _Q__; h2::h2_exception::check_catch<nothrow>(_, H2_FILINE))
#define H2Catch_1(type) for (auto _Q__ = h2::h2_exception::try_catch(); _Q__; h2::h2_exception::check_catch<type>(_, H2_FILINE))
#define H2Catch_2(type, matcher) for (auto _Q__ = h2::h2_exception::try_catch(); _Q__; h2::h2_exception::check_catch<type>(matcher, H2_FILINE))
/* clang-format off */
#define H2Catch(...) H2PP_VCALL(H2Catch_, __VA_ARGS__) if (!::setjmp(h2::h2_exception::I().catch_hole))
// source/stdio/h2_stdio.hpp
struct h2_cout : h2_once {
   const char* filine;
   h2_matcher<const char*> m;
   const char *e, *type;
   h2_cout(h2_matcher<const char*> m, const char* e, const char* type, const char* filine);
   ~h2_cout();
};

#define __H2COUT(Matcher_, expression, type, Q) for (h2::h2_cout Q(Matcher_, expression, h2::ss(type), H2_FILINE); Q;)
#define H2COUT(Matcher_, ...) __H2COUT(Matcher_, #Matcher_, #__VA_ARGS__, H2PP_UNIQUE())
// source/net/h2_dns.hpp
struct h2_dns {
   static void initialize();
   static void setaddrinfo(int count, ...);
};

struct h2_dnses {
   static void add(h2_list& dnses, h2_list& name);
   static void clear(h2_list& dnses);
};

#define H2DNS(...) h2::h2_dns::setaddrinfo(H2PP_NARG(__VA_ARGS__), __VA_ARGS__)
// source/net/h2_socket.hpp
struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(size_, data_){};
};

struct h2_sock : h2_once {
   h2_sock();
   ~h2_sock();
   static void clear();
   static void inject(const void* packet, size_t size, const char* attributes = "");  // from=1.2.3.4:5678, to=4.3.2.1:8765
   static h2_packet* fetch();
   static void failing(h2_fail* fail);
   template <typename M1 = h2_polymorphic_matcher<h2_matches_any>, typename M2 = h2_polymorphic_matcher<h2_matches_any>, typename M3 = h2_polymorphic_matcher<h2_matches_any>, typename M4 = h2_polymorphic_matcher<h2_matches_any>>
   static void check(const char* filine, const char* e, M1 from = _, M2 to = _, M3 payload = _, M4 size = _)
   {
      h2_fail* fail = nullptr;
      h2_packet* p = h2_sock::fetch();
      if (!p) {
         h2_line t = "Outgoing packet miss Ptx(";
         t.printf("green", "%s", e).printf("", ")");
         fail = h2_fail::new_normal(t, filine);
      } else {
         h2_fail* fails = nullptr;
         h2_fail* fail_from = h2_matcher_cast<const char*>(We(from)).matches(p->from.c_str());
         if (fail_from) {
            fail_from->explain = "from address";
            h2_fail::append_subling(fails, fail_from);
         }
         h2_fail* fail_to = h2_matcher_cast<const char*>(We(to)).matches(p->to.c_str());
         if (fail_to) {
            fail_to->explain = "to address";
            h2_fail::append_subling(fails, fail_to);
         }
         h2_fail* fail_payload = h2_matcher_cast<const unsigned char*>(payload).matches((unsigned char*)p->data.data());
         if (fail_payload) {
            fail_payload->explain = "payload";
            h2_fail::append_subling(fails, fail_payload);
         }
         h2_fail* fail_size = h2_matcher_cast<const int>(size).matches(p->data.size());
         if (fail_size) {
            fail_size->explain = "payload length";
            h2_fail::append_subling(fails, fail_size);
         }
         if (fails) {
            h2_line t = "Outgoing packet unexpected Ptx(";
            t.printf("green", "%s", e).printf("", ")");
            fail = h2_fail::new_normal(t, filine);
            h2_fail::append_child(fail, fails);
         }
      }
      failing(fail);
   }
};

#define __H2SOCK(Q) for (h2::h2_sock Q; Q;)
#define H2SOCK(...) __H2SOCK(H2PP_UNIQUE())

#define Ptx(...) h2::h2_sock::check(H2_FILINE, h2::ss(#__VA_ARGS__), __VA_ARGS__)
#define Pij(Packet_, Size_, ...) h2::h2_sock::inject(Packet_, Size_, h2::ss(#__VA_ARGS__))
// source/assert/h2_assert.hpp
struct h2_warning {
   h2_singleton(h2_warning);
   bool warning = false;
   bool swap(bool new_warning)
   {
      bool old_warning = I().warning;
      I().warning = new_warning;
      return old_warning;
   }
};

struct h2_assert : h2_once {
   bool oppose;
   h2_fail* fails = nullptr;
   h2_ostringstream oss;
   h2_assert(bool oppose_) : oppose(oppose_) {}
   h2_ostringstream& stash(h2_fail* fail, const char* assert_type, const h2_line& expection, const h2_line& represent, const char* assert_op = ",");
   void failing(const char* e_expression, const char* a_expression, const char* filine);
};

static inline h2_ostringstream& h2_je(h2_assert* d, h2_string e, h2_string a, h2_string selector)
{
   h2::h2_matcher<h2_string> m = Je(e, selector);
   return d->stash(m.matches(a), "JE", {}, {});
}

template <typename E, typename A>
static inline h2_ostringstream& h2_ok2(h2_assert* d, E e, const A& a, std::false_type, int)
{
   auto m = h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   return d->stash(m.matches(a, {}), d->oppose ? "KO2" : "OK2", m.expection({}), h2_stringify(a, true));
}

template <typename E, typename A>
static inline h2_ostringstream& h2_ok2(h2_assert* d, E e, const A a, std::true_type, int dimension)
{
   auto m = h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   return d->stash(m.matches((typename h2_decay<A>::type)a, {dimension}), d->oppose ? "KO2" : "OK2", m.expection({dimension}), h2_stringify(a, true));
}

template <typename M, typename A>
struct h2_2cp {
   M m;
   A a;
   const char* op;
   h2_2cp(M m_, A a_, const char* op_) : m(m_), a(a_), op(op_) {}
};

template <typename U, typename E = typename h2_decay<U>::type>
struct h2_1cp {
   U a;
   explicit h2_1cp(U u) : a(u) {}
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Equals((E)a)), A> operator==(const V& v) const { return {Equals((E)a), (A)v, "=="}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Nq((E)a)), A> operator!=(const V& v) const { return {Nq((E)a), (A)v, "!="}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Lt((E)a)), A> operator>(const V& v) const { return {Lt((E)a), (A)v, ">"}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Gt((E)a)), A> operator<(const V& v) const { return {Gt((E)a), (A)v, "<"}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Le((E)a)), A> operator>=(const V& v) const { return {Le((E)a), (A)v, ">="}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Ge((E)a)), A> operator<=(const V& v) const { return {Ge((E)a), (A)v, "<="}; }
};

struct h2_0cp {
   template <typename T>
   h2_1cp<const T&> operator>(const T& t) const { return h2_1cp<const T&>{t}; }
};

template <typename E, typename A>
static inline h2_ostringstream& h2_ok1(h2_assert* d, h2_2cp<E, A> c2)
{
   auto m = h2_matcher_cast<A>(c2.m);
   return d->stash(m.matches(c2.a, {-1, true}), d->oppose ? "KO2" : "OK2", m.expection({-1, true}), h2_stringify(c2.a, true), c2.op);
}

template <typename A>
static inline h2_ostringstream& h2_ok1(h2_assert* d, h2_1cp<A> c1)
{
   h2_fail* fail = nullptr;
   if (!c1.a) fail = h2_fail::new_unexpect("true", h2_stringify(c1.a, true));
   return d->stash(fail, d->oppose ? "KO1" : "OK1", "", h2_stringify(c1.a, true));
}

#define H2OK(_1, ...) H2PP_CAT(H2OK_, H2PP_IS_EMPTY(__VA_ARGS__))(H2PP_UNIQUE(), #_1, (#__VA_ARGS__), _1, __VA_ARGS__)
#define H2OK_1(Q, expression, _, actual, ...) \
   for (h2::h2_assert Q(false); Q; Q.failing("", expression, H2_FILINE)) h2::h2_ok1(&Q, h2::h2_0cp() > actual)
#define H2OK_0(Q, e_expression, a_expression, expect, actual) \
   for (h2::h2_assert Q(false); Q; Q.failing(e_expression, a_expression, H2_FILINE)) h2::h2_ok2(&Q, expect, actual, std::is_array<decltype(actual)>{}, std::extent<decltype(actual)>::value)

#define H2KO(_1, ...) H2PP_CAT(H2KO_, H2PP_IS_EMPTY(__VA_ARGS__))(H2PP_UNIQUE(), #_1, (#__VA_ARGS__), _1, __VA_ARGS__)
#define H2KO_1(Q, expression, _, actual, ...) \
   for (h2::h2_assert Q(true); Q; Q.failing("", expression, H2_FILINE)) h2::h2_ok1(&Q, h2::h2_0cp() > actual)
#define H2KO_0(Q, e_expression, a_expression, expect, actual) \
   for (h2::h2_assert Q(true); Q; Q.failing(e_expression, a_expression, H2_FILINE)) h2::h2_ok2(&Q, expect, actual, std::is_array<decltype(actual)>{}, std::extent<decltype(actual)>::value)

#define H2JE(...) H2PP_VCALL(H2JE_, H2PP_UNIQUE(), __VA_ARGS__)
#define H2JE_3(Q, expect, actual) \
   for (h2::h2_assert Q(false); Q; Q.failing(#expect, #actual, H2_FILINE)) h2::h2_je(&Q, expect, actual, "")
#define H2JE_4(Q, expect, actual, selector) \
   for (h2::h2_assert Q(false); Q; Q.failing(#expect, #actual, H2_FILINE)) h2::h2_je(&Q, expect, actual, selector)

#define H2Warning if (!h2::h2_warning::I().swap(true))
// source/assert/h2_timer.hpp
struct h2_timer : h2_once {
   const char* filine;
   int cpu_ms;
   clock_t start;
   h2_timer(int ms, const char* filine);
   ~h2_timer();
};

#define __H2PF(ms, Q) for (h2::h2_timer Q(ms, H2_FILINE); Q;)
#define H2PF(ms) __H2PF(ms, H2PP_UNIQUE())
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

#ifndef H2_NO_KO
#define KO H2KO
#endif

#ifndef H2_NO_JE
#define JE H2JE
#endif

#ifndef H2_NO_Warning
#define Warning H2Warning
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
#define Eq(expect, ...) H2Eq(expect, __VA_ARGS__)
#endif
using h2::TypeEq;
using h2::Nq;
using h2::Ge;
using h2::Gt;
using h2::Le;
using h2::Lt;
#ifndef H2_NO_Me
#define Me(expect, ...) H2Me(expect, __VA_ARGS__)
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
#ifndef H2_NO_Member
#define Member(expect, member, ...) H2Member(expect, member, __VA_ARGS__)
#endif
using h2::CastOf;
using h2::Not;
using h2::Conditional;
using h2::operator&&;
using h2::operator||;
using h2::AllOf;
using h2::AnyOf;
using h2::NoneOf;
using h2::ListOf;
using h2::Every;
using h2::Has;
using h2::HasKey;
using h2::HasValue;
using h2::CountOf;
using h2::MaxOf;
using h2::MinOf;
using h2::AvgOf;
#define MeanOf AvgOf
using h2::MedianOf;
using h2::Pair;
using h2::nothrow;

// source/matcher/h2_matcher_aux.hpp
static inline h2::h2_string File(const char* filename)  // slurp
{
   FILE* f = fopen(filename, "rb");
   if (!f) return "";
   fseek(f, 0, SEEK_END);
   auto l = ftell(f);
   fseek(f, 0, SEEK_SET);
   h2::h2_string s(l, ' ');
   fread((char*)s.data(), 1, l, f);
   ((char*)s.data())[l] = '\0';
   fclose(f);
   return s;
}

H2MATCHER(IsEven) { return a % 2 == 0; }
H2MATCHER(IsOdd) { return a % 2 != 0; }
H2MATCHER(IsDivable, expect) { return a % expect == 0; }
H2MATCHER(IsInteger) { return ::floor(a) == a; }
H2MATCHER(IsNaN) { return std::isnan(a); }

#if defined __cplusplus && !defined H2_NO_Redefine_private
// clang11 bug, not works option: -fno-access-control -fno-no-access-control
#define private public
#define protected public
#endif

#endif
#endif
