
/* v5.15 2021-10-23 20:00:16 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#ifndef __H2UNIT_HPP__
#define __H2UNIT_HPP__
#ifndef __H2_UNIT_HPP__
#define __H2_UNIT_HPP__

#define H2UNIT_VERSION 5.15

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

#if defined _MSC_VER
#include <malloc.h> /* _alloca _msize _expand */
#define alloca _alloca
#elif defined __MINGW32__ || defined __MINGW64__
#else
#include <alloca.h> /* alloca */
#endif

#if defined __GNUC__ || defined __clang__
// #pragma clang diagnostic ignored === #pragma GCC diagnostic ignored
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
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

#ifdef _MSC_VER  // MSVC workaround
#   define H2PP_IS_PROBE(...) _H2PP_TH1((__VA_ARGS__, 0))
#else
#   define H2PP_IS_PROBE(...) __H2PP_TH1(__VA_ARGS__, 0, )
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

#define H2PP_IS_BEGIN_PARENTHESIS(...) H2PP_IS_PROBE(_H2PP_IS_BEGIN_PARENTHESIS_PROBE __VA_ARGS__)
#define _H2PP_IS_BEGIN_PARENTHESIS_PROBE(...) H2PP_PROBE()

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define H2PP_REMOVE_PARENTHESES_IF(...) H2PP_CAT2(_H2PP_REMOVE_PARENTHESES_IF_, H2PP_IS_BEGIN_PARENTHESIS(__VA_ARGS__)) (__VA_ARGS__)
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

#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_RESCAN(H2PP_CAT2(_Macro, H2PP_NARG(__VA_ARGS__)) (__VA_ARGS__))

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

#define H2PP_FULLMESH(MSVC_Workaround) H2PP_RESCAN(H2PP_FULLMESH_R MSVC_Workaround)
#define H2PP_FULLMESH_R(_Split, _Macro, _Args, t, ...) H2PP_CAT2(_H2PP_FULLMESH_, H2PP_IS_BEGIN_PARENTHESIS(t)) ((_Split, _Macro, _Args, t, __VA_ARGS__))
#define _H2PP_FULLMESH_0(MSVC_Workaround) H2PP_RESCAN(_H2PP_FULLMESH1 MSVC_Workaround) // Fullmesh(1,2,3) => (1,2,3) x (1,2,3)
#define _H2PP_FULLMESH_1(MSVC_Workaround) H2PP_RESCAN(_H2PP_FULLMESH2 MSVC_Workaround) // Fullmesh((1,2,3), (4,5,6)) => (1,2,3) x (4,5,6)
#define _H2PP_FULLMESH1(_Split, _Macro, _Args, ...) _H2PP_FULLMESH2(_Split, _Macro, _Args, (__VA_ARGS__), (__VA_ARGS__))
#define _H2PP_FULLMESH2(_Split, _Macro, _Args, _Tuplex, _Tupley) H2PP_EVAL(_H2PP_FOREACHx(_Split, _Macro, _Args, 0, _Tupley, H2PP_REMOVE_PARENTHESES(_Tuplex)))
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
#define H2PP_DEBUG(...) ::printf("%s\n", __VA_ARGS__);

//////// >>>>> generated by build/generate.py
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

#define __H2PP_TH0(_0, ...) _0
#define __H2PP_TH1(_0, _1, ...) _1
#define __H2PP_TH2(_0, _1, _2, ...) _2
#define __H2PP_TH3(_0, _1, _2, _3, ...) _3
#define __H2PP_TH4(_0, _1, _2, _3, _4, ...) _4
#define __H2PP_TH5(_0, _1, _2, _3, _4, _5, ...) _5
#define __H2PP_TH6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define __H2PP_TH7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define __H2PP_TH8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define __H2PP_TH9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define __H2PP_TH10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define __H2PP_TH11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define __H2PP_TH12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define __H2PP_TH13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define __H2PP_TH14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define __H2PP_TH15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define __H2PP_TH16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define __H2PP_TH17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _17
#define __H2PP_TH18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) _18
#define __H2PP_TH19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) _19
#define __H2PP_TH20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, ...) _20
#define __H2PP_TH21(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, ...) _21
#define __H2PP_TH22(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, ...) _22
#define __H2PP_TH23(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, ...) _23
#define __H2PP_TH24(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, ...) _24
#define __H2PP_TH25(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, ...) _25
#define __H2PP_TH26(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, ...) _26
#define __H2PP_TH27(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, ...) _27
#define __H2PP_TH28(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, ...) _28
#define __H2PP_TH29(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, ...) _29
#define __H2PP_TH30(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, ...) _30
#define __H2PP_TH31(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, ...) _31

#define _H2PP_TH0(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH0 MSVC_Workaround)
#define _H2PP_TH1(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH1 MSVC_Workaround)
#define _H2PP_TH2(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH2 MSVC_Workaround)
#define _H2PP_TH3(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH3 MSVC_Workaround)
#define _H2PP_TH4(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH4 MSVC_Workaround)
#define _H2PP_TH5(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH5 MSVC_Workaround)
#define _H2PP_TH6(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH6 MSVC_Workaround)
#define _H2PP_TH7(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH7 MSVC_Workaround)
#define _H2PP_TH8(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH8 MSVC_Workaround)
#define _H2PP_TH9(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH9 MSVC_Workaround)
#define _H2PP_TH10(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH10 MSVC_Workaround)
#define _H2PP_TH11(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH11 MSVC_Workaround)
#define _H2PP_TH12(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH12 MSVC_Workaround)
#define _H2PP_TH13(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH13 MSVC_Workaround)
#define _H2PP_TH14(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH14 MSVC_Workaround)
#define _H2PP_TH15(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH15 MSVC_Workaround)
#define _H2PP_TH16(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH16 MSVC_Workaround)
#define _H2PP_TH17(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH17 MSVC_Workaround)
#define _H2PP_TH18(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH18 MSVC_Workaround)
#define _H2PP_TH19(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH19 MSVC_Workaround)
#define _H2PP_TH20(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH20 MSVC_Workaround)
#define _H2PP_TH21(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH21 MSVC_Workaround)
#define _H2PP_TH22(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH22 MSVC_Workaround)
#define _H2PP_TH23(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH23 MSVC_Workaround)
#define _H2PP_TH24(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH24 MSVC_Workaround)
#define _H2PP_TH25(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH25 MSVC_Workaround)
#define _H2PP_TH26(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH26 MSVC_Workaround)
#define _H2PP_TH27(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH27 MSVC_Workaround)
#define _H2PP_TH28(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH28 MSVC_Workaround)
#define _H2PP_TH29(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH29 MSVC_Workaround)
#define _H2PP_TH30(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH30 MSVC_Workaround)
#define _H2PP_TH31(MSVC_Workaround) H2PP_RESCAN(__H2PP_TH31 MSVC_Workaround)

#define H2PP_TH0(...) _H2PP_TH0((__VA_ARGS__, ))
#define H2PP_TH1(...) _H2PP_TH1((__VA_ARGS__, , ))
#define H2PP_TH2(...) _H2PP_TH2((__VA_ARGS__, , , ))
#define H2PP_TH3(...) _H2PP_TH3((__VA_ARGS__, , , , ))
#define H2PP_TH4(...) _H2PP_TH4((__VA_ARGS__, , , , , ))
#define H2PP_TH5(...) _H2PP_TH5((__VA_ARGS__, , , , , , ))
#define H2PP_TH6(...) _H2PP_TH6((__VA_ARGS__, , , , , , , ))
#define H2PP_TH7(...) _H2PP_TH7((__VA_ARGS__, , , , , , , , ))
#define H2PP_TH8(...) _H2PP_TH8((__VA_ARGS__, , , , , , , , , ))
#define H2PP_TH9(...) _H2PP_TH9((__VA_ARGS__, , , , , , , , , , ))
#define H2PP_TH10(...) _H2PP_TH10((__VA_ARGS__, , , , , , , , , , , ))
#define H2PP_TH11(...) _H2PP_TH11((__VA_ARGS__, , , , , , , , , , , , ))
#define H2PP_TH12(...) _H2PP_TH12((__VA_ARGS__, , , , , , , , , , , , , ))
#define H2PP_TH13(...) _H2PP_TH13((__VA_ARGS__, , , , , , , , , , , , , , ))
#define H2PP_TH14(...) _H2PP_TH14((__VA_ARGS__, , , , , , , , , , , , , , , ))
#define H2PP_TH15(...) _H2PP_TH15((__VA_ARGS__, , , , , , , , , , , , , , , , ))
#define H2PP_TH16(...) _H2PP_TH16((__VA_ARGS__, , , , , , , , , , , , , , , , , ))
#define H2PP_TH17(...) _H2PP_TH17((__VA_ARGS__, , , , , , , , , , , , , , , , , , ))
#define H2PP_TH18(...) _H2PP_TH18((__VA_ARGS__, , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH19(...) _H2PP_TH19((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH20(...) _H2PP_TH20((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH21(...) _H2PP_TH21((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH22(...) _H2PP_TH22((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH23(...) _H2PP_TH23((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH24(...) _H2PP_TH24((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH25(...) _H2PP_TH25((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH26(...) _H2PP_TH26((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH27(...) _H2PP_TH27((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH28(...) _H2PP_TH28((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH29(...) _H2PP_TH29((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH30(...) _H2PP_TH30((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))
#define H2PP_TH31(...) _H2PP_TH31((__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ))

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

#define __H2PP_64TH(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, ...) _64
#ifdef _MSC_VER
#define _H2PP_64TH(MSVC_Workaround)  H2PP_RESCAN(__H2PP_64TH MSVC_Workaround)
#define H2PP_HAS_COMMA(...) _H2PP_64TH((__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))
#define H2PP_ARG_COUNT(...) _H2PP_64TH((__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))
#else
#define H2PP_HAS_COMMA(...) __H2PP_64TH(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define H2PP_ARG_COUNT(...) __H2PP_64TH(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#endif

//////// <<<<< generated by build/generate.py
// source/utils/h2_fs.hpp
struct h2_fs {
   const char* file;
   int line;
   const char* func;
   h2_fs(const char* file_ = nullptr, int line_ = 0, const char* func_ = nullptr) : file(file_), line(line_), func(func_) {}
   bool empty() const;
   const char* basefile() const;
};
// source/utils/h2_template.hpp
template <typename R, typename T>
static R h2_un(T t)
{
   union h2_un {
      T t;
      R r;
   } u;
   u.t = t;
   return u.r;
}

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
inline T* h2_pointer_if(T& a) { return &a; }
template <typename T>
inline T* h2_pointer_if(T* a) { return a; }

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
   static std::true_type has_begin(typename std::enable_if<std::is_same<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::begin)), typename U::const_iterator (U::*)() const>::value>::type*);
   template <typename U>
   static std::false_type has_begin(...);

   template <typename U>
   static auto has_end(U* u) -> typename std::enable_if<std::is_member_function_pointer<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::end))>::value, std::true_type>::type;
   template <typename U>
   static std::false_type has_end(...);

   static constexpr bool value = decltype(has_const_iterator<T>(nullptr))::value && decltype(has_begin<T>(nullptr))::value && decltype(has_end<T>(nullptr))::value;
};
// source/utils/h2_list.hpp
#define h2_list_entry(ptr, type, link) ((type*)((char*)(ptr) - (char*)(&(((type*)(1))->link)) + 1))

#define h2_list_for_each_entry(...) H2PP_VARIADIC_CALL(__h2_list_for_each_entry, H2PP_UNIQUE(t), __VA_ARGS__)
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
struct h2_once {
   unsigned long c = 0;
   operator bool() { return !c++; }
};

struct h2_extract {
   static const char* has(const char* attributes, const char* key);
   static bool numeric(const char* attributes, const char* key, double& value);
   static bool iport(const char* attributes, const char* key, char* str);
   static int fill(const char* attributes, const char* key, unsigned char bytes[]);
};

// #define M(...) func(#__VA_ARGS__, other)
// Unix M() ==> func("", other) stringify empty __VA_ARGS__ to "" string
// Windows M() ==> func(, other) stringify empty __VA_ARGS__ to empty
// #define M(...) func(ss(#__VA_ARGS__), other)
static inline const char* ss(const char* a = "") { return a ? a : ""; }

#define H2Foreach(_Macro_x, ...) H2PP_FOREACH(, _H2ForeachMacro, (_Macro_x), __VA_ARGS__)
#define _H2ForeachMacro(_Args, i, x) H2PP_REMOVE_PARENTHESES(_Args)(x)

#define H2Fullmesh(_Macro_x_y, ...) H2PP_FULLMESH((, _H2FullmeshMacro, (_Macro_x_y), __VA_ARGS__))
#define _H2FullmeshMacro(_Args, i, j, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)

/* clang-format off */
#define h2_singleton(Class_) static Class_& I() { static Class_ i; return i; }
// source/utils/h2_numeric.hpp
struct h2_numeric {
   static bool is_hex_string(const char* s);
   static bool is_bin_string(const char* s);
   static size_t hex_to_bytes(const char* hex, unsigned char* bytes);
   static size_t bin_to_bits(const char* bin, unsigned char* bytes);

   static bool bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits);

   static const char* sequence_number(size_t sequence, size_t shift = 1);
};
// source/utils/h2_libc.hpp
struct h2_libc {
   static void* malloc(size_t size);
   static void free(void* ptr);
   static void* operator new(std::size_t size) { return malloc(size); }
   static void operator delete(void* ptr) { free(ptr); }
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
// source/utils/h2_string.hpp
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

   size_t width(size_t columns = 2) const;
   bool equals(const h2_string& str, bool caseless = false) const;
   bool contains(const h2_string& substr, bool caseless = false) const;
   bool startswith(const h2_string& prefix, bool caseless = false) const;
   bool endswith(const h2_string& suffix, bool caseless = false) const;

   bool isspace() const;
   bool enclosed(const char c = '\"') const;

   h2_string escape(bool utf8 = false) const;
   h2_string unescape() const;
   h2_string unquote(const char c = '\"') const;
   h2_string trim() const;
   h2_string squash(bool quote = false) const;
   h2_string tolower() const;
   h2_string center(size_t width) const;
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

   bool enclosed(const char c) const;
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
static inline const h2_line gray(const h2_line& line) { return delta(line, "dark gray"); }

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
   static h2_line print(T a, bool represent = false) { return "?"; }
};

#define H2_TOSTRING_ABLE(tostring)                                                                            \
   template <typename T>                                                                                      \
   struct h2_##tostring##_able {                                                                              \
      template <typename U>                                                                                   \
      static auto return_type(U* u) -> decltype(u->tostring());                                               \
      template <typename U>                                                                                   \
      static void return_type(...);                                                                           \
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
   static h2_line print(const T& a, bool represent = false) { return print__tostring(a, represent); }
   template <typename U>
   static auto print__tostring(const U& a, bool represent) -> typename std::enable_if<h2::h2_tostring_able<U>::value, h2_line>::type { return const_cast<U&>(a).tostring(); }
   template <typename U>
   static auto print__tostring(const U& a, bool represent) -> typename std::enable_if<!h2::h2_tostring_able<U>::value, h2_line>::type { return print__toString(a, represent); }
   template <typename U>
   static auto print__toString(const U& a, bool represent) -> typename std::enable_if<h2::h2_toString_able<U>::value, h2_line>::type { return const_cast<U&>(a).toString(); }
   template <typename U>
   static auto print__toString(const U& a, bool represent) -> typename std::enable_if<!h2::h2_toString_able<U>::value, h2_line>::type { return print__Tostring(a, represent); }
   template <typename U>
   static auto print__Tostring(const U& a, bool represent) -> typename std::enable_if<h2::h2_Tostring_able<U>::value, h2_line>::type { return const_cast<U&>(a).toString(); }
   template <typename U>
   static auto print__Tostring(const U& a, bool represent) -> typename std::enable_if<!h2::h2_Tostring_able<U>::value, h2_line>::type { return print__ToString(a, represent); }
   template <typename U>
   static auto print__ToString(const U& a, bool represent) -> typename std::enable_if<h2::h2_ToString_able<U>::value, h2_line>::type { return const_cast<U&>(a).ToString(); }
   template <typename U>
   static auto print__ToString(const U& a, bool represent) -> typename std::enable_if<!h2::h2_ToString_able<U>::value, h2_line>::type { return print__to_string(a, represent); }
   template <typename U>
   static auto print__to_string(const U& a, bool represent) -> typename std::enable_if<h2::h2_to_string_able<U>::value, h2_line>::type { return const_cast<U&>(a).to_string(); }
   template <typename U>
   static auto print__to_string(const U& a, bool represent) -> typename std::enable_if<!h2::h2_to_string_able<U>::value, h2_line>::type { return ""; }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false) { return ostream_print(a, represent); }

   template <typename U>
   static h2_line ostream_print(const U& a, bool represent)
   {
      h2_ostringstream oss;
      oss << std::boolalpha << const_cast<U&>(a);
      if (represent) {
         const char* quote = nullptr;
         if (std::is_same<char, U>::value) quote = "'";
         if (std::is_convertible<U, h2_string>::value) quote = "\"";
         if (quote) return gray(quote) + oss.str().c_str() + gray(quote);
      }
      return {oss.str().c_str()};
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
inline h2_line h2_stringify(const T& a, bool represent = false) { return h2_stringify_impl<T>::print(a, represent); }

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
// source/utils/h2_color.hpp
struct h2_color {
   static void prints(const char* style, const char* format, ...);
   static void printl(const h2_line& line, bool cr = true);
   static void printl(const h2_lines& lines, bool cr = true);
   static bool isctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
// source/symbol/h2_nm.hpp
struct h2_symbol {
   h2_list x;
   char name[128];
   unsigned long long addr;
   h2_symbol(const char* name_, unsigned long long addr_) : addr(addr_) { strncpy(name, name_, 127); }
};

struct h2_nm {
   h2_singleton(h2_nm);
   std::map<std::string, unsigned long long>* mangle_symbols;
   h2_list demangle_symbols;
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
      if (std::is_lvalue_reference<T>::value)
         strcat(name, "&");
      else if (std::is_rvalue_reference<T>::value)
         strcat(name, "&&");
      return name;
   }
};
// source/render/h2_option.hpp
struct h2_option {
   h2_singleton(h2_option);

#if defined __linux
   static constexpr char os = 'L';
#elif defined __APPLE__
   static constexpr char os = 'm';
#elif defined _WIN32 || defined __CYGWIN__ // +MinGW
   static constexpr char os = 'W';
#endif

   char args[256];
   const char* path;
   bool colorful = true;
   bool progressing = true;
   bool last_failed = false;
   bool shuffle_cases = false;
   bool memory_check = true;
   bool exception_as_fail = false;
   bool debug = false;
   int list_cases = 0;
   int break_after_fails = 0;
   int run_rounds = 1;
   int fold_json = 9; // 0 unfold, 1 fold simple, 2 fold same, 3 fold peer-miss
   int copy_paste_json = 0; // 0 no quote, 1 quote by ', 2 quote by ", 3 quote by \"
   int verbose = 2;
   char junit_path[256]{'\0'};
   char tap_path[256]{'\0'};
   std::vector<const char*> includes, excludes;

   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int line) const;
};

static const h2_option& O = h2_option::I();  // for pretty
// source/render/h2_layout.hpp
struct h2_layout {
   static h2_lines split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, size_t step, char scale, size_t width);
   static h2_lines unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width);
   static h2_lines seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width);
};
// source/render/h2_failure.hpp
struct h2_fail : h2_libc {
   h2_fail *subling_next = nullptr, *child_next = nullptr;

   const char* assert_type = "Inner";  // Inner(Mock, AllOf, &&, ||)
   const char* assert_op = ",";
   h2_string e_expression, a_expression;
   h2_line explain;
   h2_string user_explain;

   //           expression     expection      represent       value
   // number     Ge(var)        Ge(5)          5               5
   // string     We(var)        We("abc")      "abc"           abc
   // char       We(var)        We('a')        'a'             a

   int seqno = -1;
   h2_fs fs;

   h2_fail(const h2_line& explain_, const h2_fs& fs_) : explain(explain_), fs(fs_) {}
   virtual ~h2_fail();

   h2_line locate();

   virtual void print(size_t si = 0, size_t ci = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, size_t, size_t)> cb, size_t si = 0, size_t ci = 0);
   static void append_subling(h2_fail*& fail, h2_fail* nf);
   static void append_child(h2_fail*& fail, h2_fail* nf);

   static h2_fail* new_normal(const h2_line& explain, const h2_fs& fs = h2_fs());
   static h2_fail* new_unexpect(const h2_line& expection = {}, const h2_line& represent = {}, const h2_line& explain = {});
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection, const h2_line& explain = {});
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, const h2_line& explain = {});
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection, bool caseless, const h2_line& explain = {});
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t length, const size_t width);
   static h2_fail* new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const h2_fs& fs);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample);
   static h2_fail* new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
   static h2_fail* new_exception(const char* explain, const char* type, const h2_backtrace& bt_throw);
   static h2_fail* new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_line& explain = {});
};

static inline void h2_fail_g(h2_fail*);
// source/json/h2_json.hpp
struct h2_json {
   static h2_lines dump(const h2_string& json_string);
   static h2_string select(const h2_string& json_string, const h2_string& selector, bool caseless);
   // < 0 illformed json; = 0 matched; > 0 unmatched
   static int match(const h2_string& expect, const h2_string& actual, bool caseless);
   static bool diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless);
};
// source/matcher/h2_matches.hpp
struct h2_mc {
   bool caseless = false, dont = false, ncop = false, spaceless = false;
   h2_mc(bool caseless_ = false, bool dont_ = false, bool ncop_ = false, bool spaceless_ = false) : caseless(caseless_), dont(dont_), ncop(ncop_), spaceless(spaceless_) {}

   bool fit(bool result) const { return (result == !dont); }
   h2_mc update_caseless(bool caseless_ = false) const { return {caseless_, dont, ncop, spaceless}; }
   h2_mc update_dont(bool dont_ = false) const { return {caseless, dont_, ncop, spaceless}; }
   h2_mc update_spaceless(bool spaceless_ = false) const { return {caseless, dont, ncop, spaceless_}; }
};
struct h2_matches {
   virtual h2_line expection(h2_mc c) const = 0;
};

static inline h2_string DS(bool match) { return match ? "should not match" : ""; }

static inline h2_line CD(const h2_line& s, h2_mc c, const char* dsym = "!")
{
   h2_line t;
   if (!c.ncop && c.dont) t.push_back(dsym);
   if (c.caseless) t.push_back("~");
   if (c.spaceless) t.push_back("*");
   t += s;
   return t;
}

template <typename T>
inline auto h2_matches_expection(const T& e, h2_mc c) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_line>::type { return e.expection(c); }
template <typename T>
inline auto h2_matches_expection(const T& e, h2_mc c) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_line>::type { return CD(h2_representify(e), c); }

#define H2_MATCHES_T2V2E(t_matchers)                                                                                                                    \
   template <typename T>                                                                                                                                \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                                      \
   template <typename T, std::size_t I>                                                                                                                 \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                         \
   {                                                                                                                                                    \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                                    \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                            \
   }                                                                                                                                                    \
   template <typename T>                                                                                                                                \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }         \
   h2_line t2e(h2_mc c, std::integral_constant<std::size_t, 0>) const { return {}; }                                                                    \
   template <std::size_t I>                                                                                                                             \
   h2_line t2e(h2_mc c, std::integral_constant<std::size_t, I>) const                                                                                   \
   {                                                                                                                                                    \
      return t2e(c, std::integral_constant<size_t, I - 1>()) + (1 < I ? gray(", ") : h2_line()) + h2_matches_expection(std::get<I - 1>(t_matchers), c); \
   }                                                                                                                                                    \
   h2_line t2e(h2_mc c) const { return t2e(c, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
// source/matcher/h2_matcher.hpp
template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(T a, int n, h2_mc c) const = 0;
   virtual h2_line expection(h2_mc c) const override { return ""; }
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
   h2_fail* matches(const T& a, int n = 0, h2_mc c = {}) const { return impl->matches(a, n, c); }
   virtual h2_line expection(h2_mc c = {}) const { return impl->expection(c); };
};

template <typename Matches>
struct h2_polymorphic_matcher : h2_matches {
   const Matches m;
   bool caseless = false, dont = false, spaceless = false;
   explicit h2_polymorphic_matcher(const Matches& m_) : m(m_) {}
   h2_polymorphic_matcher& operator~()
   {
      caseless = true;
      return *this;
   }
   h2_polymorphic_matcher& operator!()
   {
      dont = !dont;
      return *this;
   }
   h2_polymorphic_matcher& operator*()
   {
      spaceless = true;
      return *this;
   }
   h2_polymorphic_matcher& operator()() { return *this; }  // IsTrue/IsTrue() both works

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(m, caseless, dont, spaceless), 0); }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      bool caseless, dont, spaceless;
      explicit internal_impl(const Matches& m_, bool caseless_, bool dont_, bool spaceless_) : m(m_), caseless(caseless_), dont(dont_), spaceless(spaceless_) {}
      h2_fail* matches(T a, int n = 0, h2_mc c = {}) const override { return m.matches(a, n, {caseless || c.caseless, dont != c.dont, c.ncop, spaceless || c.spaceless}); }
      h2_line expection(h2_mc c) const override { return m.expection({caseless || c.caseless, dont != c.dont /*XOR ^*/, c.ncop, spaceless || c.spaceless}); }
   };

   virtual h2_line expection(h2_mc c = {}) const override { return h2_matches_expection(m, {caseless || c.caseless, dont != c.dont, c.ncop, spaceless || c.spaceless}); }
};
// source/matcher/h2_equation.hpp
template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc c) const
   {
      if (c.fit(a == e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD(h2_representify(e), c.update_caseless(false), "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& e_, const long double = 0) : e(h2_string(e_)) {}

   h2_fail* matches(const h2_string& a, int, h2_mc c) const
   {
      h2_string _e = e, _a = a;
      if (c.spaceless) _e = e.squash(), _a = a.squash();
      if (c.fit(_a.equals(_e, c.caseless))) return nullptr;
      if (c.fit(h2_pattern::wildcard_match(_e.c_str(), _a.c_str(), c.caseless))) return nullptr;
      if (c.fit(h2_pattern::regex_match(_e.c_str(), _a.c_str(), c.caseless))) return nullptr;
      return h2_fail::new_strcmp(_e, a, c.caseless, expection(c));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD(h2_representify(c.spaceless ? e.squash() : e), c, "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& e_, const long double epsilon_ = 0) : e(e_), epsilon(epsilon_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc c) const
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
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      h2_line t = h2_representify(e);
      if (epsilon != 0) {
         h2_ostringstream oss;
         oss << "±" << std::fixed << epsilon;
         t += oss.str().c_str();
      }
      return CD(t, c.update_caseless(false), "≠");
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_equation<E>> Eq(const T& expect, const long double epsilon = 0)
{
   return h2_polymorphic_matcher<h2_equation<E>>(h2_equation<E>(expect, epsilon));
}
// source/matcher/h2_cast.hpp
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
      h2_fail* matches(T a, int n, h2_mc c) const override { return from.matches(static_cast<U>(a), n, c); }
      void operator=(internal_impl const&) = delete;
   };
};

template <typename T>
struct h2_matcher_cast_impl<T, h2_matcher<T>> {
   static h2_matcher<T> cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> h2_matcher_cast(const M& from) { return h2_matcher_cast_impl<T, M>::cast(from); }
// source/matcher/h2_unary.hpp
struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc opt) const { return nullptr; }
   virtual h2_line expection(h2_mc opt) const override { return "Any"; }
};

struct h2_matches_null : h2_matches {
   const bool reverse;
   explicit h2_matches_null(bool reverse_) : reverse(reverse_) {}
   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc opt) const
   {
      bool result = reverse ? nullptr != (const void*)a : nullptr == (const void*)a;
      if (opt.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(opt), h2_stringify((const void*)a));
   }
   virtual h2_line expection(h2_mc opt) const override
   {
      return reverse != opt.dont /*XOR ^*/ ? "NotNull" : "IsNull";
   }
};

template <bool E>
struct h2_matches_boolean : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc opt) const
   {
      bool result = E ? a : !a;
      if (opt.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(opt), a ? "true" : "false");
   }
   virtual h2_line expection(h2_mc opt) const override
   {
      return (E ? opt.dont : !opt.dont) ? "false" : "true";
   }
};

template <typename Matcher>
struct h2_pointee_matches : h2_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher m_) : m(m_) {}

   template <typename SmartPointer>
   struct PointeeOf {
      typedef typename SmartPointer::element_type type;
   };
   template <typename T>
   struct PointeeOf<T*> {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, int, h2_mc opt) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, 0, opt);
   }
   virtual h2_line expection(h2_mc opt) const override
   {
      return h2_matches_expection(m, opt);
   }
};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_any> Any{h2_matches_any()};

inline h2_polymorphic_matcher<h2_matches_null> _IsNull() { return h2_polymorphic_matcher<h2_matches_null>(h2_matches_null(false)); }
inline h2_polymorphic_matcher<h2_matches_null> _NotNull() { return h2_polymorphic_matcher<h2_matches_null>(h2_matches_null(true)); }
inline h2_polymorphic_matcher<h2_matches_boolean<true>> _IsTrue() { return h2_polymorphic_matcher<h2_matches_boolean<true>>(h2_matches_boolean<true>()); }
inline h2_polymorphic_matcher<h2_matches_boolean<false>> _IsFalse() { return h2_polymorphic_matcher<h2_matches_boolean<false>>(h2_matches_boolean<false>()); }

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
// source/matcher/h2_logic.hpp
template <typename Matcher>
struct h2_not_matches : h2_matches {
   const Matcher m; /* h2_polymorphic_matcher or immediate value or h2_matches */
   explicit h2_not_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      return h2_matcher_cast<A>(m).matches(a, n, c.update_dont(!c.dont));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return h2_matches_expection(m, c.update_dont(!c.dont));
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_and_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_and_matches(const Matcher1& m1_, const Matcher2& m2_) : m1(m1_), m2(m2_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m1).matches(a, n, c.update_dont(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m2).matches(a, n, c.update_dont(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);

      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.dont ? gray("(") : h2_line()) + h2_matches_expection(m1, c.update_dont(false)) + " and " + h2_matches_expection(m2, c.update_dont(false)) + (c.dont ? gray(")") : h2_line()), c.update_caseless(false));
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_or_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_or_matches(const Matcher1& m1_, const Matcher2& m2_) : m1(m1_), m2(m2_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, n, c.update_dont(false));
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, n, c.update_dont(false));
      if (c.fit(!f1 || !f2)) {
         if (f1) delete f1;
         if (f2) delete f2;
         return nullptr;
      }
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, f1);
      h2_fail::append_subling(fails, f2);

      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.dont ? gray("(") : h2_line()) + h2_matches_expection(m1, c.update_dont(false)) + " or " + h2_matches_expection(m2, c.update_dont(false)) + (c.dont ? gray(")") : h2_line()), c.update_caseless(false));
   }
};

template <typename... Matchers>
struct h2_allof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, c.update_dont(false));  // dont not transfer down
         if (fail) fail->seqno = (int)i;
         h2_fail::append_subling(fails, fail);
      }

      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (c.dont) {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a), "should not match all");
      } else {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("AllOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int count = 0;
      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, c.update_dont(false));
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
      h2_fail* fail = nullptr;
      if (c.dont) {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a), "should not match any one");
      } else {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a), "not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("AnyOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, c.update_dont(false));
         if (fail)
            delete fail;
         else {
            fail = h2_fail::new_normal("should not match " + v_matchers[i].expection(c).brush("green"));
            fail->seqno = (int)i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (c.fit(!fails)) {
         delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("NoneOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher m)
{
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(m));
}

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
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> operator&&(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> operator&&(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> operator&&(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> operator||(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> operator||(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> operator||(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}
// source/matcher/h2_inequation.hpp
template <typename E>
struct h2_matches_ge : h2_matches {
   const E e;
   explicit h2_matches_ge(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc c) const
   {
      if (c.fit(a >= e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.ncop ? "" : "≥") + h2_representify(e), c.update_caseless(false));
   }
};

template <typename E>
struct h2_matches_gt : h2_matches {
   const E e;
   explicit h2_matches_gt(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc c) const
   {
      if (c.fit(a > e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.ncop ? "" : ">") + h2_stringify(e), c.update_caseless(false));
   }
};

template <typename E>
struct h2_matches_le : h2_matches {
   const E e;
   explicit h2_matches_le(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc c) const
   {
      if (c.fit(a <= e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.ncop ? "" : "≤") + h2_stringify(e), c.update_caseless(false));
   }
};

template <typename E>
struct h2_matches_lt : h2_matches {
   const E e;
   explicit h2_matches_lt(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, h2_mc c) const
   {
      if (c.fit(a < e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.ncop ? "" : "<") + h2_stringify(e), c.update_caseless(false));
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
// source/matcher/h2_strcmp.hpp
struct h2_matches_regex : h2_matches {
   const h2_string e;
   explicit h2_matches_regex(const h2_string& e_) : e(e_) {}
   h2_fail* matches(const h2_string& a, int n, h2_mc c) const;
   virtual h2_line expection(h2_mc c) const override;
};

struct h2_matches_wildcard : h2_matches {
   const h2_string e;
   explicit h2_matches_wildcard(const h2_string& e_) : e(e_) {}
   h2_fail* matches(const h2_string& a, int n, h2_mc c) const;
   virtual h2_line expection(h2_mc c) const override;
};

struct h2_matches_strcmp : h2_matches {
   const h2_string e;
   explicit h2_matches_strcmp(const h2_string& e_) : e(e_) {}
   h2_fail* matches(const h2_string& a, int n, h2_mc c) const;
   virtual h2_line expection(h2_mc c) const override;
};

struct h2_matches_substr : h2_matches {
   const h2_string substring;
   explicit h2_matches_substr(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, int n, h2_mc c) const;
   virtual h2_line expection(h2_mc c) const override;
};

struct h2_matches_startswith : h2_matches {
   const h2_string prefix_string;
   explicit h2_matches_startswith(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, int n, h2_mc c) const;
   virtual h2_line expection(h2_mc c) const override;
};

struct h2_matches_endswith : h2_matches {
   const h2_string suffix_string;
   explicit h2_matches_endswith(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, int n, h2_mc c) const;
   virtual h2_line expection(h2_mc c) const override;
};

struct h2_matches_json : h2_matches {
   const h2_string e;
   const h2_string selector;
   explicit h2_matches_json(const h2_string& e_, const h2_string& selector_) : e(e_), selector(selector_) {}
   h2_fail* matches(const h2_string& a, int, h2_mc c) const;
   virtual h2_line expection(h2_mc c) const override;
};

struct h2_caseless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(const h2_matcher<h2_string>& matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const { return m.matches(a, n, c.update_caseless(true)); }
   virtual h2_line expection(h2_mc c) const override { return m.expection(c.update_caseless(true)); }
};

struct h2_spaceless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_spaceless_matches(const h2_matcher<h2_string>& matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const { return m.matches(a, n, c.update_spaceless(true)); }
   virtual h2_line expection(h2_mc c) const override { return m.expection(c.update_spaceless(true)); }
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
inline h2_polymorphic_matcher<h2_spaceless_matches> SpaceLess(const M& m) { return h2_polymorphic_matcher<h2_spaceless_matches>(h2_spaceless_matches(h2_matcher<h2_string>(m))); }
// source/matcher/h2_memcmp.hpp
template <typename E>
struct h2_matches_memcmp : h2_matches {
   const E buffer;
   const size_t size;
   const size_t length;
   const size_t width;
   explicit h2_matches_memcmp(const E buffer_, const size_t size_, const size_t length_, const size_t width_) : buffer(buffer_), size(size_), length(length_), width(width_) {}
   h2_fail* matches(const void* a, int n, h2_mc c) const
   {
      unsigned char* e = (unsigned char*)buffer;
      size_t l = length, w = width;
      bool result = true;
      do {
         if (!w && !l && std::is_convertible<E, h2_string>::value) { /* deduce by string format */
            l = strlen((const char*)buffer);
            w = 8;
            if (!strcmp((const char*)buffer, (const char*)a)) break; /*result = true;*/
            if (h2_numeric::is_bin_string((const char*)buffer)) {
               e = (unsigned char*)alloca(l);
               l = h2_numeric::bin_to_bits((const char*)buffer, e);
               w = 1;
            } else if (h2_numeric::is_hex_string((const char*)buffer)) {
               e = (unsigned char*)alloca(l);
               l = h2_numeric::hex_to_bytes((const char*)buffer, e);
               w = 8;
            }
         }
         if (!w) w = h2_sizeof_pointee<E>::value * 8; /* deduce by data type */
         if (!l) l = size;                            /* deduce by array size */
         if (!l || !w) return h2_fail::new_normal(color("length", "red") + " not specified " + gray("in ") + color("Me(buffer, ", "cyan") + color("length", "red") + gray(", width") + color(")", "cyan"));
         result = h2_numeric::bits_equal(e, (const unsigned char*)a, l * w);
      } while (0);

      if (c.fit(result)) return nullptr;
      return h2_fail::new_memcmp((const unsigned char*)e, (const unsigned char*)a, l, w);
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD("Me()", c);
   }
};

template <typename E, typename T = typename std::decay<E>::type>
inline h2_polymorphic_matcher<h2_matches_memcmp<T>> _Me(const E buffer, const size_t size, const size_t length = 0, const size_t width = 0)
{
   return h2_polymorphic_matcher<h2_matches_memcmp<T>>(h2_matches_memcmp<T>((T)buffer, size, length, width));
}

#define H2Me(buffer, ...) H2PP_CAT(__H2Me, H2PP_IS_EMPTY(__VA_ARGS__))(buffer, std::extent<decltype(buffer)>::value, __VA_ARGS__)
#define __H2Me1(buffer, size, ...) h2::_Me(buffer, size)
#define __H2Me0(buffer, size, ...) h2::_Me(buffer, size, __VA_ARGS__)
// source/matcher/h2_container.hpp
template <typename EK, typename EV>
struct h2_pair_matches : h2_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(const EK& k_, const EV& v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, int, h2_mc c) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.first)>::type>(k).matches(a.first, 0, c.update_dont(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.second)>::type>(v).matches(a.second, 0, c.update_dont(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(const A& a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect(expection(c), h2_representify(a));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD(gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")"), c.update_caseless(false));
   }
};

template <typename Matcher>
struct h2_has_matches : h2_matches {
   Matcher m;
   explicit h2_has_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, int n, h2_mc c) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      bool found = false;
      for (auto& i : a) {
         h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(i, n, c.update_dont(false));
         if (!fail) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a), DS(found));
   }

   template <typename A>
   auto matches(A a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      bool found = false;
      for (int i = 0; i < n; ++i) {
         h2_fail* fail = h2_matcher_cast<typename std::decay<decltype(a[i])>::type>(m).matches(a[i], n, c.update_dont(false));
         if (!fail) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a, n), DS(found));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("Has" + gray("(") + h2_matches_expection(m, c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }
};

template <typename Matcher>
struct h2_countof_matches : h2_matches {
   Matcher m;
   explicit h2_countof_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, int n, h2_mc c) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      int count = 0;
      for (auto& c : a) count++; /* container size() is best, but forward_list haven't. iterator works all, regardless speed. https://en.cppreference.com/w/cpp/container */
      return __matches(count, h2_representify(a), c);
   }

   template <typename A>
   auto matches(A a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return __matches(n, h2_representify(a, n), c);
   }

   h2_fail* __matches(int count, h2_line&& represent, h2_mc c) const
   {
      h2_fail* fails = h2_matcher_cast<int>(m).matches(count, 0, c.update_dont(false));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      return h2_fail::new_unexpect(expection(c), represent + "/" + h2_stringify(count));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("CountOf" + gray("(") + h2_matches_expection(m, c.update_caseless(false).update_dont(false)) + gray(")"), c.update_caseless(false));
   }
};

template <typename... Matchers>
struct h2_listof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int, h2_mc c) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;

      h2_vector<h2_matcher<typename A::value_type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, count = 0;
         for (auto& k : a) {
            if (j++ == i) {
               ++count;
               fail = v_matchers[i].matches(k, 0, c.update_dont(false));
               break;
            }
         }
         if (count == 0) {
            fail = h2_fail::new_unexpect(v_matchers[i].expection(c.update_dont(false)), "", "but out of range");
         }
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(A a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ generic array */
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<typename std::decay<decltype(a[0])>::type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a[i], n, c.update_dont(false));
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a, n), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("ListOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_pair_matches<EK, EV>> Pair(const MK& mk, const MV& mv)
{
   return h2_polymorphic_matcher<h2_pair_matches<EK, EV>>(h2_pair_matches<EK, EV>(mk, mv));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matcher&>::type>> Has(const Matcher& m)
{
   return h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matcher&>::type>>(h2_has_matches<typename std::decay<const Matcher&>::type>(m));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matcher&>::type>> CountOf(const Matcher& m)
{
   return h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matcher&>::type>>(h2_countof_matches<typename std::decay<const Matcher&>::type>(m));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
// source/matcher/h2_customize.hpp
#define __Matches_Common(message)                                                           \
   template <typename A>                                                                    \
   bool __matches(const A& a) const;                                                        \
   template <typename A>                                                                    \
   h2::h2_fail* matches(const A& a, int, h2::h2_mc c) const                                 \
   {                                                                                        \
      h2::h2_fail* fail = h2::h2_fail::new_unexpect(h2::CD("", c), h2::h2_representify(a)); \
      if (c.fit(__matches(a))) return nullptr;                                              \
      h2::h2_ostringstream t;                                                               \
      t << H2PP_REMOVE_PARENTHESES(message);                                                \
      fail->user_explain = t.str().c_str();                                                 \
      return fail;                                                                          \
   }                                                                                        \
   virtual h2::h2_line expection(h2::h2_mc c) const override { return ""; }

#define H2MATCHER0(name, message)                                               \
   struct h2_##name##_matches : h2::h2_matches {                                \
      explicit h2_##name##_matches() {}                                         \
      __Matches_Common(message)                                                 \
   };                                                                           \
   h2::h2_polymorphic_matcher<h2_##name##_matches> name{h2_##name##_matches()}; \
   template <typename A>                                                        \
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

#define _H2MATCHER_1(name) H2MATCHER0(name, (""))

#define _H2MATCHER_2_0(name, e1) H2MATCHER1(name, e1, (""))
#define _H2MATCHER_2_1(name, message) H2MATCHER0(name, message)
#define _H2MATCHER_2(name, t) H2PP_CAT(_H2MATCHER_2_, H2PP_IS_BEGIN_PARENTHESIS(t))(name, t)

#define _H2MATCHER_3_0(name, e1, e2) H2MATCHER2(name, e1, e2, (""))
#define _H2MATCHER_3_1(name, e1, message) H2MATCHER1(name, e1, message)
#define _H2MATCHER_3(name, e1, t) H2PP_CAT(_H2MATCHER_3_, H2PP_IS_BEGIN_PARENTHESIS(t))(name, e1, t)

#define _H2MATCHER_4_0(name, e1, e2, e3) H2MATCHER3(name, e1, e2, e3, (""))
#define _H2MATCHER_4_1(name, e1, e2, message) H2MATCHER2(name, e1, e2, message)
#define _H2MATCHER_4(name, e1, e2, t) H2PP_CAT(_H2MATCHER_4_, H2PP_IS_BEGIN_PARENTHESIS(t))(name, e1, e2, t)

#define _H2MATCHER_5_0(name, e1, e2, e3, e4) H2MATCHER4(name, e1, e2, e3, e4, (""))
#define _H2MATCHER_5_1(name, e1, e2, e3, message) H2MATCHER3(name, e1, e2, e3, message)
#define _H2MATCHER_5(name, e1, e2, e3, t) H2PP_CAT(_H2MATCHER_5_, H2PP_IS_BEGIN_PARENTHESIS(t))(name, e1, e2, e3, t)

#define _H2MATCHER_6_0(name, e1, e2, e3, e4, e5) H2MATCHER5(name, e1, e2, e3, e4, e5, (""))
#define _H2MATCHER_6_1(name, e1, e2, e3, e4, message) H2MATCHER4(name, e1, e2, e3, e4, message)
#define _H2MATCHER_6(name, e1, e2, e3, e4, t) H2PP_CAT(_H2MATCHER_6_, H2PP_IS_BEGIN_PARENTHESIS(t))(name, e1, e2, e3, e4, t)

#define _H2MATCHER_7_0(name, e1, e2, e3, e4, e5, e6)
#define _H2MATCHER_7_1(name, e1, e2, e3, e4, e5, message) H2MATCHER5(name, e1, e2, e3, e4, e5, message)
#define _H2MATCHER_7(name, e1, e2, e3, e4, e5, t) H2PP_CAT(_H2MATCHER_7_, H2PP_IS_BEGIN_PARENTHESIS(t))(name, e1, e2, e3, e4, e5, t)

#define H2MATCHER(...) H2PP_VARIADIC_CALL(_H2MATCHER_, __VA_ARGS__)
// source/matcher/h2_matcher.cpp
template <typename T>
inline h2_matcher<T>::h2_matcher() { *this = Any; }

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = Eq(value); }
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
   static void* A(T fp) { return h2_un<void*>(fp); }
   static void* A(const char* fn)
   {
      h2_symbol* res[16];
      int n = h2_nm::get_by_name(fn, res, 16);
      if (n == 1) return h2_load::addr_to_ptr(res[0]->addr);
      h2_vector<h2_string> candidates;
      for (int i = 0; i < n; ++i) candidates.push_back(res[i]->name);
      h2_fail_g(h2_fail::new_symbol(fn, candidates));
      return nullptr;
   }
};

template <typename ReturnType, typename... ArgumentTypes>
struct h2_fp<ReturnType(ArgumentTypes...)> {
   static void* A(ReturnType (*fp)(ArgumentTypes...)) { return h2_un<void*>(fp); }
   static void* A(const char* fn) { return h2_fp<>::A(fn); }
};

template <typename ClassType, typename ReturnType, typename... ArgumentTypes>
struct h2_fp<ClassType, ReturnType(ArgumentTypes...)> {
   // static member function
   static void* A(ReturnType (*f)(ArgumentTypes...)) { return (void*)f; }
   static void* B(ClassType* o, ReturnType (*f)(ArgumentTypes...)) { return (void*)f; }

#if defined _MSC_VER
   // https://github.com/microsoft/Detours
   // &C::f1 ILT+165(??_9C$BAAA) 00007FF7987210AA
   // &C::f2 ILT+410(??_9C$B7AA) 00007FF79872119F
   // follow_jmp &C::f1 C::`vcall'{0}' 00007FF798721F2C
   // follow_jmp &C::f2 C::`vcall'{8}' 00007FF798721F24
   static void* A(ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      long offset;
      if (!is_virtual_mfp(f, offset)) return h2_un<void*>(f);

      ClassType* o = h2_constructible<ClassType>::O(alloca(sizeof(ClassType)));
      if (0 == (unsigned long long)o || 1 == (unsigned long long)o || 2 == (unsigned long long)o) {
         h2_fail_g(h2_fail::new_symbol("vtable of " + h2_string(constructible_errors[(unsigned long long)o]) + h2_cxa::type_name<ClassType>(), {}, "  try: " + color("STUB/MOCK(", "yellow") + color("Object", "bold,red") + color(", Class, Method, ...)", "yellow")));
         return nullptr;
      }
      void* t = get_virtual_mfp(o, offset);
      h2_destructible<ClassType>(o);
      return t;
   }
   static void* B(ClassType* o, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      long offset;
      if (!is_virtual_mfp(f, offset)) return h2_un<void*>(f);
      return get_virtual_mfp(o, offset);
   }

   static void* get_virtual_mfp(ClassType* o, long offset)
   {
      void** vtable = *(void***)o;
      return vtable[offset / sizeof(void*)];
   }
   static bool is_virtual_mfp(ReturnType (ClassType::*f)(ArgumentTypes...), long& offset)
   {
      h2_symbol* symbol = h2_nm::get_by_addr((unsigned long long)h2_cxa::follow_jmp(h2_un<void*>(f)));
      if (!symbol) return false;
      char* p = strstr(symbol->name, "::`vcall'{");
      if (!p) return false;  // not virtual member function
      offset = strtol(p + 10, nullptr, 10);
      return true;
   }
#else
   //  https://itanium-cxx-abi.github.io/cxx-abi/
   //  &ClassType::Method has separate representations for non-virtual and virtual functions.
   //  For non-virtual functions, it is the address of the function.
   //  For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
   //  The least-significant bit therefore discriminates between virtual and non-virtual functions.
   static void* A(ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      if (!is_virtual_mfp(f)) return h2_un<void*>(f);
      ClassType* o = h2_constructible<ClassType>::O(alloca(sizeof(ClassType)));
      if (2 < (unsigned long long)o) {
         void* t = get_virtual_mfp(o, f);
         h2_destructible<ClassType>(o);
         return t;
      }
      char vtable_symbol[1024];
      sprintf(vtable_symbol, "_ZTV%s", typeid(ClassType).name());  // mangle for "vtable for ClassType"
      unsigned long long relative_vtable = h2_nm::get_mangle(vtable_symbol);
      if (!relative_vtable) {
         h2_fail_g(h2_fail::new_symbol("vtable of " + h2_string(constructible_errors[(unsigned long long)o]) + h2_cxa::type_name<ClassType>(), {}, "  try: " + color("STUB/MOCK(", "yellow") + color("Object", "bold,red") + color(", Class, Method, ...)", "yellow")));
         return nullptr;
      }
      return get_virtual_mfp((void**)h2_load::vtable_to_ptr(relative_vtable), f);
   }
   static void* B(ClassType* o, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      if (!is_virtual_mfp(f)) return h2_un<void*>(f);
      return get_virtual_mfp(*(void***)o, f);
   }

   static void* get_virtual_mfp(ClassType* o, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      return get_virtual_mfp(*(void***)o, f);
   }
   static void* get_virtual_mfp(void** vtable, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      return vtable[(h2_un<unsigned long long>(f) & ~1ULL) / sizeof(void*)];
   }
   static bool is_virtual_mfp(ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      if (h2_un<unsigned long long>(&h2_vtable_test::dummy) & 1)
         return (h2_un<unsigned long long>(f) & 1) && (h2_un<unsigned long long>(f) - 1) % sizeof(void*) == 0 && h2_un<unsigned long long>(f) < 1000 * sizeof(void*);
      else
         return h2_un<unsigned long long>(f) % sizeof(void*) == 0 && h2_un<unsigned long long>(f) < 100 * sizeof(void*);
   }
#endif
};
// source/stub/h2_stubs.hpp
struct h2_stubs {
   h2_list stubs;
   bool add(void* srcfp, void* dstfp, const h2_fs& fs);
   void clear(void* srcfp);
   void clear();
};
// source/stub/h2_temporary.hpp
struct h2_stub_temporary_restore : h2_once {
   unsigned char current_opcode[32];
   void* srcfp;
   h2_stub_temporary_restore(void* srcfp);
   ~h2_stub_temporary_restore();
};
// source/stub/h2_stuber.hpp
static inline void h2_stub_g(void*, void*, const h2_fs&);

namespace {
template <int Counter, typename ClassType, typename Signature>
struct h2_stuber;
template <int Counter, typename ClassType, typename ReturnType, typename... ArgumentTypes>
struct h2_stuber<Counter, ClassType, ReturnType(ArgumentTypes...)> {
   h2_singleton(h2_stuber);
   void* srcfp;
   h2_fs fs;

   ReturnType (*dstfp)(ClassType*, ArgumentTypes...);
   struct member_function_stub {  // wrap for calling conversions
      ReturnType fx(ArgumentTypes... arguments) { return I().dstfp((ClassType*)this, std::forward<ArgumentTypes>(arguments)...); }
   };

   static h2_stuber& I(void* srcfp, const h2_fs& fs)
   {
      I().srcfp = srcfp;
      I().fs = fs;
      return I();
   }

   void operator=(ReturnType (*dstfp_)(ClassType*, ArgumentTypes...))
   {
#if defined _WIN32 && (defined __i386__ || defined _M_IX86)  // https://docs.microsoft.com/en-us/cpp/cpp/calling-conventions
      dstfp = dstfp_;
      h2_stub_g(srcfp, h2_fp<member_function_stub, ReturnType(ArgumentTypes...)>::A(&member_function_stub::fx), fs);
#else
      h2_stub_g(srcfp, (void*)dstfp_, fs);
#endif
   }
};
}
// source/stub/h2_stub.hpp
// STUB(                              Src      , Dst )
// STUB(                    Function, Signature, Dst )
// STUB(         ClassType, Method  , Signature, Dst )
// STUB( Object, ClassType, Method  , Signature, Dst )
#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB_, __VA_ARGS__)
#define __H2STUB_2(Src, Dst) h2::h2_stub_g(h2::h2_fp<>::A(Src), (void*)Dst, {__FILE__, __LINE__, #Src})
#define __H2STUB_3(Function, Signature, Dst) h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Dst), {__FILE__, __LINE__, #Function})
#define __H2STUB_4(ClassType, Method, Signature, Dst) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = Dst
#define __H2STUB_5(Object, ClassType, Method, Signature, Dst) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = Dst

#define H2UNSTUB(...) H2PP_VARIADIC_CALL(__H2UNSTUB_, __VA_ARGS__)
#define __H2UNSTUB_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUB_2(Function, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB_3(ClassType, Method, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUB_4(Object, ClassType, Method, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))

// STUBS(                    Function, ReturnType, ArgumentTypes ) { }
// STUBS(         ClassType, Method  , ReturnType, ArgumentTypes ) { }
// STUBS( Object, ClassType, Method  , ReturnType, ArgumentTypes ) { }
#define H2UNSTUBS(...) H2PP_VARIADIC_CALL(__H2UNSTUBS_, __VA_ARGS__)
#define __H2UNSTUBS_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUBS_3(Function, ReturnType, ArgumentTypes) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUBS_4(ClassType, Method, ReturnType, ArgumentTypes) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUBS_5(Object, ClassType, Method, ReturnType, ArgumentTypes) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))

#define H2STUBS(...) H2PP_VARIADIC_CALL(__H2STUBS_, __VA_ARGS__)
#define __H2STUBS_3(Function, ReturnType, ArgumentTypes) ___H2STUBS_3(Function, ReturnType, ArgumentTypes, H2PP_UNIQUE(t_stub3))
#define __H2STUBS_4(ClassType, Method, ReturnType, ArgumentTypes) H2PP_CAT(__H2STUBS_4_, H2PP_IS_EMPTY ArgumentTypes)(ClassType, Method, ReturnType, ArgumentTypes)
#define __H2STUBS_4_0(ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This, H2PP_REMOVE_PARENTHESES(ArgumentTypes)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_4_1(ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_5(Object, ClassType, Method, ReturnType, ArgumentTypes) H2PP_CAT(__H2STUBS_5_, H2PP_IS_EMPTY ArgumentTypes)(Object, ClassType, Method, ReturnType, ArgumentTypes)
#define __H2STUBS_5_0(Object, ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This, H2PP_REMOVE_PARENTHESES(ArgumentTypes)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_5_1(Object, ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)

#define ___H2STUBS_3(Function, ReturnType, ArgumentTypes, Q)                                                                                                                       \
   struct {                                                                                                                                                                        \
      void operator=(ReturnType(*dstfp) ArgumentTypes)                                                                                                                             \
      {                                                                                                                                                                            \
         h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(dstfp), {__FILE__, __LINE__, #Function}); \
      }                                                                                                                                                                            \
   } Q;                                                                                                                                                                            \
   Q = [] ArgumentTypes -> ReturnType /* captureless lambda implicit cast to function pointer */
// source/mock/h2_routine.hpp
template <typename ReturnType>
struct h2_return : h2_libc {
   ReturnType value;
   h2_return() = delete;
   explicit h2_return(ReturnType _value) : value(_value){};
};

template <typename ClassType, typename Signature>
struct h2_routine;

template <typename ClassType, typename ReturnType, typename... ArgumentTypes>
struct h2_routine<ClassType, ReturnType(ArgumentTypes...)> {
   ReturnType (*fp)(ArgumentTypes...) = nullptr;               // normal function pointer
   ReturnType (*mfp)(ClassType*, ArgumentTypes...) = nullptr;  // member function pointer
   h2_shared_ptr<h2_return<ReturnType>> ret;

   h2_routine() {}
   h2_routine(ReturnType r) : ret(new h2_return<ReturnType>(r)) {}
   h2_routine(ReturnType (*f)(ArgumentTypes...)) : fp(f) {}
   h2_routine(ReturnType (*f)(ClassType*, ArgumentTypes...)) : mfp(f) {}

   ReturnType operator()(ClassType* This, ArgumentTypes... arguments)
   {
      if (mfp)
         return mfp(This, arguments...);
      else if (fp)
         return fp(arguments...);
      else if (ret)
         return ret->value;
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

template <typename ClassType, typename... ArgumentTypes>
struct h2_routine<ClassType, void(ArgumentTypes...)> {
   void (*fp)(ArgumentTypes...) = nullptr;
   void (*mfp)(ClassType*, ArgumentTypes...) = nullptr;

   h2_routine() {}
   h2_routine(void (*f)(ArgumentTypes...)) : fp(f) {}
   h2_routine(void (*f)(ClassType*, ArgumentTypes...)) : mfp(f) {}

   void operator()(ClassType* This, ArgumentTypes... arguments)
   {
      if (mfp)
         mfp(This, arguments...);
      else if (fp)
         fp(arguments...);
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

   h2_fail* check(size_t index, size_t total, const h2_fs& fs) const;
   const char* actual() const;
   const char* expect() const;

   static h2_checkin Once() { return h2_checkin(1, 1, "Once"); }
   static h2_checkin Twice() { return h2_checkin(2, 2, "Twice"); }
   static h2_checkin Times(int count) { return h2_checkin(count, count, "Times"); }
   static h2_checkin Any() { return h2_checkin(0, INT_MAX, "Any"); }
   static h2_checkin Atleast(int count) { return h2_checkin(count, INT_MAX, "Atleast"); }
   static h2_checkin Atmost(int count) { return h2_checkin(0, count, "Atmost"); }
   static h2_checkin Between(int left, int right) { return h2_checkin(left, right, "Between"); }
};
// source/mock/h2_tuple.hpp
static inline void h2_assert_g();

template <typename MatcherTuple, typename ArgumentTypeTuple, std::size_t I>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTypeTuple& arguments, std::integral_constant<std::size_t, I>)
{
   h2_fail* fails = matches(matchers, arguments, std::integral_constant<std::size_t, I - 1>());
   h2_fail* fail = std::get<I - 1>(matchers).matches(std::get<I - 1>(arguments));
   if (fail) fail->seqno = I - 1;
   h2_fail::append_subling(fails, fail);
   h2_assert_g();
   return fails;
}
template <typename MatcherTuple, typename ArgumentTypeTuple>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTypeTuple& arguments, std::integral_constant<std::size_t, 0>)
{
   return nullptr;
}
template <typename MatcherTuple, typename ArgumentTypeTuple>
inline h2_fail* h2_tuple_matches(MatcherTuple& matchers, ArgumentTypeTuple& arguments)
{
   return matches(matchers, arguments, std::integral_constant<std::size_t, std::tuple_size<ArgumentTypeTuple>::value>());
}

template <typename ArgumentTypeTuple>
inline void h2_tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, 0>) {}
template <typename ArgumentTypeTuple, std::size_t I>
inline void h2_tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, I>)
{
   h2_tuple_types<ArgumentTypeTuple>(names, std::integral_constant<std::size_t, I - 1>());
   names.push_back(h2_cxa::type_name<typename std::tuple_element<I - 1, ArgumentTypeTuple>::type>());
}
template <typename ArgumentTypeTuple>
inline void h2_tuple_types(h2_vector<h2_string>& names)
{
   return h2_tuple_types<ArgumentTypeTuple>(names, std::integral_constant<std::size_t, std::tuple_size<ArgumentTypeTuple>::value>());
}
// source/mock/h2_mocker.hpp
struct h2_mocker_base : h2_libc {
   h2_list x;
   void *srcfp, *dstfp;
   h2_fs fs;
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
};

namespace {

template <int Counter, typename ClassType, typename Signature>
class h2_mocker;

template <int Counter, typename ClassType, typename ReturnType, typename... ArgumentTypes>
class h2_mocker<Counter, ClassType, ReturnType(ArgumentTypes...)> : h2_mocker_base {
   using ArgumentTypeTuple = std::tuple<ArgumentTypes...>;

#define H2_Typedef_Matcher(__, i) h2_matcher<h2_nth_decay<i, ArgumentTypes...>>
   using MatcherTuple = std::tuple<H2PP_REPEAT((, ), H2_Typedef_Matcher, , 20)>;
#undef H2_Typedef_Matcher

   h2_vector<MatcherTuple> matcher_array;
   h2_vector<h2_routine<ClassType, ReturnType(ArgumentTypes...)>> routine_array;
   h2_routine<ClassType, ReturnType(ArgumentTypes...)> original;

   static ReturnType function_stub(ClassType* This, ArgumentTypes... arguments)
   {
      int index = I().matches(std::forward<ArgumentTypes>(arguments)...);
      h2::h2_stub_temporary_restore t(I().srcfp);
      if (index == -1 || !I().routine_array[index])
         return I().original(This, std::forward<ArgumentTypes>(arguments)...);
      return I().routine_array[index](This, std::forward<ArgumentTypes>(arguments)...);
   }

   struct member_function_stub {  // wrap for calling conversions
      ReturnType fx(ArgumentTypes... arguments) { return function_stub((ClassType*)this, std::forward<ArgumentTypes>(arguments)...); }
   };

   static ReturnType normal_function_stub(ArgumentTypes... arguments) { return function_stub(nullptr, std::forward<ArgumentTypes>(arguments)...); }

   int matches(ArgumentTypes... arguments)
   {
      ArgumentTypeTuple at = std::forward_as_tuple(std::forward<ArgumentTypes>(arguments)...);
      int checkin_offset = -1;
      for (int i = checkin_index; i < (int)checkin_array.size(); ++i) {
         h2_fail* fails = h2_tuple_matches(matcher_array[i], at);
         if (fails) {
            if (checkin_offset != -1) break;
            if (checkin_array[i].is_satisfied()) { /* try next h2_checkin */
               delete fails;
               continue;
            }
            fails->foreach([this, i](h2_fail* f, size_t, size_t) {
               f->explain += gray("on ") + (fs.func + argument(f->seqno));
               if (1 < checkin_array.size()) f->explain += gray(" when ") + h2_numeric::sequence_number((size_t)i) + " " + color(checkin_array[i].expr, "cyan");
            });
            h2_fail* fail = h2_fail::new_normal(signature(), fs);
            h2_fail::append_child(fail, fails);
            h2_fail_g(fail);
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
         h2_fail* fail = h2_fail::new_normal(signature(), fs);
         h2_fail* f = h2_fail::new_normal(fs.func + h2_representify(at) + color(" unexpectedly", "red,bold") + " called");
         h2_fail::append_child(fail, f);
         h2_fail_g(fail);
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
         h2_tuple_types<ArgumentTypeTuple>(i->argument_types);
      }
      return *i;
   }

   static h2_mocker& I(void* srcfp, const h2_fs& fs)
   {
      if (std::is_same<std::false_type, ClassType>::value) {
         I().dstfp = (void*)normal_function_stub;
         I().original.fp = (ReturnType(*)(ArgumentTypes...))srcfp;
      } else {
         I().dstfp = h2::h2_fp<member_function_stub, ReturnType(ArgumentTypes...)>::A(&member_function_stub::fx);
         I().original.mfp = (ReturnType(*)(ClassType*, ArgumentTypes...))srcfp;
      }
      I().srcfp = srcfp;
      I().fs = fs;
      I().reset();
      I().mock();
      return I();
   }

   h2_mocker& greed(bool mode)
   {
      greed_mode = mode;
      return *this;
   }

#define H2_Default_Matcher(__, i) h2_matcher<h2_nth_decay<i, ArgumentTypes...>> _##i = {}
#define H2_Forward_Matcher(__, i) std::forward<h2_matcher<h2_nth_decay<i, ArgumentTypes...>>>(_##i)

   h2_mocker& Once(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Once());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Twice(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Twice());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Times(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Times(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Any(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Any());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Atleast(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atleast(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Atmost(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atmost(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Between(int left, int right, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Between(left, right));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
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

#define H2_Th_Matcher(__, i)                                              \
   h2_mocker& Th##i(h2_matcher<h2_nth_decay<i, ArgumentTypes...>> e = {}) \
   {                                                                      \
      if (checkin_array.empty()) Any();                                   \
      std::get<i>(matcher_array.back()) = e;                              \
      return *this;                                                       \
   }
   H2PP_REPEAT(, H2_Th_Matcher, , 20);
#undef H2_Th_Matcher

   h2_mocker& Return(h2_routine<ClassType, ReturnType(ArgumentTypes...)> r)
   {
      if (checkin_array.empty()) Any();
      if (!routine_array.empty()) routine_array.pop_back();
      routine_array.push_back(r);
      return *this;
   }

   void operator=(ReturnType (*f)(ArgumentTypes...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.fp = f;
   }

   void operator=(ReturnType (*f)(ClassType*, ArgumentTypes...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.mfp = f;
   }
};

}
// source/mock/h2_mocks.hpp
struct h2_mocks {
   h2_list mocks;
   bool add(void* mock);
   h2_fail* clear(bool check);
};
// source/mock/h2_mock.hpp
// MOCK(                    Function, Signature )
// MOCK(         ClassType, Method  , Signature )
// MOCK( Object, ClassType, Method  , Signature )
#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK_, __VA_ARGS__)
#define __H2MOCK_2(Function, Signature) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(Function), {__FILE__, __LINE__, #Function})
#define __H2MOCK_3(ClassType, Method, Signature) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method})
#define __H2MOCK_4(Object, ClassType, Method, Signature) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method})

// MOCKS(                    Function, ReturnType, ArgumentTypes, Inspection ) { }
// MOCKS(         ClassType, Method  , ReturnType, ArgumentTypes, Inspection ) { }
// MOCKS( Object, ClassType, Method  , ReturnType, ArgumentTypes, Inspection ) { }
#define H2MOCKS(...) H2PP_VARIADIC_CALL(__H2MOCKS_, __VA_ARGS__)
#define __H2MOCKS_4(Function, ReturnType, ArgumentTypes, Inspection) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(Function), {__FILE__, __LINE__, #Function}).Inspection = [] ArgumentTypes -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCKS_5(ClassType, Method, ReturnType, ArgumentTypes, Inspection) H2PP_CAT(__H2MOCKS_5_, H2PP_IS_EMPTY ArgumentTypes)(ClassType, Method, ReturnType, ArgumentTypes, Inspection)
#define __H2MOCKS_5_0(ClassType, Method, ReturnType, ArgumentTypes, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This, H2PP_REMOVE_PARENTHESES(ArgumentTypes)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCKS_5_1(ClassType, Method, ReturnType, ArgumentTypes, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCKS_6(Object, ClassType, Method, ReturnType, ArgumentTypes, Inspection) H2PP_CAT(__H2MOCKS_6_, H2PP_IS_EMPTY ArgumentTypes)(Object, ClassType, Method, ReturnType, ArgumentTypes, Inspection)
#define __H2MOCKS_6_0(Object, ClassType, Method, ReturnType, ArgumentTypes, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This, H2PP_REMOVE_PARENTHESES(ArgumentTypes)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCKS_6_1(Object, ClassType, Method, ReturnType, ArgumentTypes, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
// source/memory/h2_exempt.hpp
struct h2_exempt {
   h2_singleton(h2_exempt);
   void* fps[10000];
   int nfp = 0;
   static void setup();
   static void add_by_fp(void* fp);
   static void add_by_name(const char* fn);
};
// source/memory/h2_memory.hpp
struct h2_memory {
   static void initialize();
   static void finalize();
   static void hook(bool overrides = true);

   struct stack {
      static void root();
      static void push(const h2_fs& fs);
      static h2_fail* pop();
      static long long footprint();

      struct block : h2_once {
         bool unmem = false;
         block(const char* attributes, const h2_fs& fs);
         ~block();
      };
   };
};

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(h2::ss(Attributes), {__FILE__, __LINE__}); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2PP_UNIQUE())

template <typename T>
inline void h2_unmem(T f) { h2_exempt::add_by_fp(h2_un<void*>(f)); }
template <>
inline void h2_unmem(const char* f) { h2_exempt::add_by_name(f); }

#define H2UNMEM(f) h2::h2_unmem(f)
// source/except/h2_debug.hpp
struct h2_debugger {
   static void trap();
};

#define h2_debug(shift, ...)                                                        \
   do {                                                                             \
      if (!O.debug) {                                                               \
         h2_color::prints("", __VA_ARGS__);                                         \
         h2_color::prints("", " %s : %d = %s\n", __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace::dump(shift).print(3);                                        \
      }                                                                             \
   } while (0)
// source/stdio/h2_stdio.hpp
struct h2_cout : h2_once {
   h2_fs fs;
   h2_matcher<const char*> m;
   const char *e, *type;
   h2_cout(h2_matcher<const char*> m, const char* e, const char* type, const h2_fs& fs);
   ~h2_cout();
};

#define __H2COUT(Matcher_, expression, type, Q) for (h2::h2_cout Q(Matcher_, expression, h2::ss(type), {__FILE__, __LINE__}); Q;)
#define H2COUT(Matcher_, ...) __H2COUT(Matcher_, #Matcher_, #__VA_ARGS__, H2PP_UNIQUE())
// source/net/h2_dns.hpp
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

   template <typename M1 = h2_polymorphic_matcher<h2_matches_any>, typename M2 = h2_polymorphic_matcher<h2_matches_any>, typename M3 = h2_polymorphic_matcher<h2_matches_any>, typename M4 = h2_polymorphic_matcher<h2_matches_any>>
   static void check(const h2_fs& fs, const char* e, M1 from = Any, M2 to = Any, M3 payload = Any, M4 size = Any)
   {
      h2_assert_g();
      h2_packet* p = h2_sock::fetch();
      if (!p) {
         h2_line t = "Outgoing packet miss Ptx(";
         t.printf("green", "%s", e).printf("", ")");
         h2_fail_g(h2_fail::new_normal(t, fs));
         return;
      }
      h2_fail* fails = nullptr;
      h2_fail* fail_from = h2_matcher_cast<const char*>(from).matches(p->from.c_str(), 0, {});
      if (fail_from) {
         fail_from->explain = "from address";
         h2_fail::append_subling(fails, fail_from);
      }
      h2_fail* fail_to = h2_matcher_cast<const char*>(to).matches(p->to.c_str(), 0, {});
      if (fail_to) {
         fail_to->explain = "to address";
         h2_fail::append_subling(fails, fail_to);
      }
      h2_fail* fail_payload = h2_matcher_cast<const unsigned char*>(payload).matches((unsigned char*)p->data.data(), 0, {});
      if (fail_payload) {
         fail_payload->explain = "payload";
         h2_fail::append_subling(fails, fail_payload);
      }
      h2_fail* fail_size = h2_matcher_cast<const int>(size).matches(p->data.size(), 0, {});
      if (fail_size) {
         fail_size->explain = "payload length";
         h2_fail::append_subling(fails, fail_size);
      }

      if (fails) {
         h2_line t = "Outgoing packet unexpected Ptx(";
         t.printf("green", "%s", e).printf("", ")");
         h2_fail* fail = h2_fail::new_normal(t, fs);
         h2_fail::append_child(fail, fails);
         h2_fail_g(fail);
      }
   }
};

#define __H2SOCK(Q) for (h2::h2_sock Q; Q;)
#define H2SOCK(...) __H2SOCK(H2PP_UNIQUE())

#define Ptx(...) h2::h2_sock::check({__FILE__, __LINE__}, h2::ss(#__VA_ARGS__), __VA_ARGS__)
#define Pij(Packet_, Size_, ...) h2::h2_sock::inject(Packet_, Size_, h2::ss(#__VA_ARGS__))
// source/core/h2_case.hpp
struct h2_case {
   h2_fs fs;
   const char* name;
   bool todo = false, filtered = false, ignored = false;
   bool failed = false, last_failed = false;
   bool scheduled = false;
   int seq = 0;
   int asserts = 0;
   long long footprint = 0;
   h2_list x;
   jmp_buf ctx;
   h2_fail* fails = nullptr;
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;

   h2_case(const char* name_, int todo_, const h2_fs& fs_) : fs(fs_), name(name_), todo(todo_) {}
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() { scheduled = false; }
   void post_cleanup();

   void do_fail(h2_fail* fail, bool defer, bool append);

   struct cleaner : h2_once {
      h2_case* thus;
      cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
      ~cleaner() { thus->prev_cleanup(); }
   };
};
// source/core/h2_suite.hpp
struct h2_stats {
   int passed = 0, failed = 0, todo = 0, filtered = 0, ignored = 0;
   int asserts = 0;
   long long footprint = 0;
   void clear() { passed = 0, failed = 0, todo = 0, filtered = 0, ignored = 0, asserts = 0, footprint = 0; }
};

struct h2_suite {
   h2_fs fs;
   const char* name;
   h2_list x;
   int seq = 0;
   h2_stats stats;
   bool filtered = false;
   jmp_buf ctx;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list cases;
   h2_stubs stubs;
   h2_mocks mocks;

   h2_suite(const char* name, void (*)(h2_suite*, h2_case*), const h2_fs& fs);
   void clear();

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

   h2_stats stats;
   int rounds = 0;
   int last = 0;
   h2_list suites;
   h2_suite* current_suite = nullptr;
   h2_case* current_case = nullptr;
   h2_stubs stubs;
   h2_mocks mocks;
   std::vector<void (*)()> global_setups, global_cleanups;
   std::vector<void (*)()> global_suite_setups, global_suite_cleanups;
   std::vector<void (*)()> global_case_setups, global_case_cleanups;

   void shuffle();
   void shadow();
   void enumerate();
   int main(int argc, const char** argv);
};

static inline void h2_stub_g(void* srcfp, void* dstfp, const h2_fs& fs)
{
   if (!srcfp || !dstfp) return;
   if (h2_runner::I().current_case)
      h2_runner::I().current_case->stubs.add(srcfp, dstfp, fs);
   else if (h2_runner::I().current_suite)
      h2_runner::I().current_suite->stubs.add(srcfp, dstfp, fs);
   else
      h2_runner::I().stubs.add(srcfp, dstfp, fs);
}

static inline void h2_unstub_g(void* srcfp)
{
   if (!srcfp) return;
   if (h2_runner::I().current_case)
      h2_runner::I().current_case->stubs.clear(srcfp);
   else if (h2_runner::I().current_suite)
      h2_runner::I().current_suite->stubs.clear(srcfp);
   else
      h2_runner::I().stubs.clear(srcfp);
}

static inline void h2_mock_g(void* mock)
{
   if (h2_runner::I().current_case)
      h2_runner::I().current_case->mocks.add(mock);
   else if (h2_runner::I().current_suite)
      h2_runner::I().current_suite->mocks.add(mock);
   else
      h2_runner::I().mocks.add(mock);
}

static inline void h2_assert_g()
{
   if (h2_runner::I().current_case) h2_runner::I().current_case->asserts += 1;
   if (h2_runner::I().current_suite) h2_runner::I().current_suite->stats.asserts += 1;
   h2_runner::I().stats.asserts += 1;
}

static inline void h2_fail_g(h2_fail* fail)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_runner::I().current_case) h2_runner::I().current_case->do_fail(fail, O.verbose >= 5, true);
}
// source/core/h2_core.hpp

#define __H2SUITE(suite_name, suite_test)                                                      \
   static void suite_test(h2::h2_suite*, h2::h2_case*);                                        \
   static h2::h2_suite H2PP_UNIQUE(si)(h2::ss(suite_name), &suite_test, {__FILE__, __LINE__}); \
   static void suite_test(h2::h2_suite* suite_1_5_2_8_0_1_1_9_8, h2::h2_case* case_1_1_0_2_6_0_0_2_4)

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2PP_UNIQUE(suite_test_C))

#define H2Setup() if (case_1_1_0_2_6_0_0_2_4)
#define H2Teardown() if (::setjmp(suite_1_5_2_8_0_1_1_9_8->ctx))

#define __H2Case(case_name, case_instance, suite_cleaner, case_cleaner, todo)                                             \
   static h2::h2_case case_instance(h2::ss(case_name), todo, {__FILE__, __LINE__});                                       \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance);                                \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4)                                                                          \
      for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner; case_1_1_0_2_6_0_0_2_4 = nullptr) \
         for (h2::h2_case::cleaner case_cleaner(&case_instance); case_cleaner;)                                           \
            if (!::setjmp(case_instance.ctx))

#define H2Case(...) __H2Case(#__VA_ARGS__, H2PP_UNIQUE(ci), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), 0)
#define H2Todo(...) __H2Case(#__VA_ARGS__, H2PP_UNIQUE(ci), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), 1)

#define __H2CASE(case_name, case_test, suite_test, todo)                                              \
   static void case_test();                                                                           \
   static void suite_test(h2::h2_suite* suite_1_5_2_8_0_1_1_9_8, h2::h2_case* case_1_1_0_2_6_0_0_2_4) \
   {                                                                                                  \
      static h2::h2_case case_instance(h2::ss(case_name), todo, {__FILE__, __LINE__});                \
      static h2::h2_suite::registor suite_registor(suite_1_5_2_8_0_1_1_9_8, &case_instance);          \
      if (&case_instance == case_1_1_0_2_6_0_0_2_4)                                                   \
         for (h2::h2_case::cleaner case_cleaner(&case_instance); case_cleaner;)                       \
            if (!::setjmp(case_instance.ctx))                                                         \
               case_test();                                                                           \
   }                                                                                                  \
   static h2::h2_suite H2PP_UNIQUE(si)(nullptr, &suite_test, {__FILE__, __LINE__});                   \
   static void case_test()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(case_test_C), H2PP_UNIQUE(suite_test_C), 0)
#define H2TODO(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(case_test_C), H2PP_UNIQUE(suite_test_C), 1)

#define _H2_An(...) H2PP_CAT(_H2_An_, H2PP_IS_EMPTY(__VA_ARGS__))((__VA_ARGS__))
#define _H2_An_1(...) 0
#define _H2_An_0(_Args) H2PP_RESCAN(_H2_An_0I _Args)
#define _H2_An_0I(a, ...) a

#define _H2_An_ForEach(...) H2PP_CAT(_H2_An_ForEach_, H2PP_IS_EMPTY(__VA_ARGS__))((__VA_ARGS__))
#define _H2_An_ForEach_1(...) 0
#define _H2_An_ForEach_0(_Args) H2PP_RESCAN(_H2_An_ForEach_0I _Args)
#define _H2_An_ForEach_0I(a, ...) H2PP_CAT(_H2_An_ForEach_0_, H2PP_IS_BEGIN_PARENTHESIS(a))(a)
#define _H2_An_ForEach_0_0(a) _H2_An(a)
#define _H2_An_ForEach_0_1(a) _H2_An(H2PP_REMOVE_PARENTHESES(a))

#define _H2_An_Fullmeshx(...) _H2_An_ForEach(H2PP_HEAD(__VA_ARGS__))
#define _H2_An_Fullmeshy(...) _H2_An_ForEach(H2PP_LAST(__VA_ARGS__))

#define __H2CASES_Macro4(case_test, i, x) \
   H2CASE(i. x) { case_test(x); }
#define __H2CASES_Macro3(_Args) H2PP_RESCAN(__H2CASES_Macro4 _Args)
#define __H2CASES_Macro2(...) __H2CASES_Macro3((__VA_ARGS__))
#define __H2CASES_Macro1(args, i, x) __H2CASES_Macro2(H2PP_REMOVE_PARENTHESES(args), i, x)
#define __H2CASES(case_test, ...)                              \
   template <typename T>                                       \
   static void case_test(T x);                                 \
   H2PP_FOREACH(, __H2CASES_Macro1, (case_test), __VA_ARGS__); \
   template <typename T>                                       \
   static void case_test(T x)
#define H2CASES(...) __H2CASES(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define __H2CASESS_Macro4(case_test, i, j, x, y) \
   H2CASE(i.j. x, y) { case_test(x, y); }
#define __H2CASESS_Macro3(_Args) H2PP_RESCAN(__H2CASESS_Macro4 _Args)
#define __H2CASESS_Macro2(...) __H2CASESS_Macro3((__VA_ARGS__))
#define __H2CASESS_Macro1(args, i, j, x, y) __H2CASESS_Macro2(H2PP_REMOVE_PARENTHESES(args), i, j, x, y)
#define __H2CASESS(case_test, ...)                                 \
   template <typename Tx, typename Ty>                             \
   static void case_test(Tx x, Ty y);                              \
   H2PP_FULLMESH((, __H2CASESS_Macro1, (case_test), __VA_ARGS__)); \
   template <typename Tx, typename Ty>                             \
   static void case_test(Tx x, Ty y)
#define H2CASESS(...) __H2CASESS(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define H2Cases(...) __H2Cases(H2PP_UNIQUE(x), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), __VA_ARGS__)
#define __H2Cases(Qx, suite_cleaner, case_cleaner, ...)                                     \
   auto Qx = _H2_An_ForEach(__VA_ARGS__);                                                   \
   H2PP_FOREACH(, __H2Cases_Macro1, (Qx), __VA_ARGS__);                                     \
   if (case_1_1_0_2_6_0_0_2_4 && case_1_1_0_2_6_0_0_2_4->scheduled)                         \
      for (auto x = Qx; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)           \
         for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner;) \
            for (h2::h2_case::cleaner case_cleaner(case_1_1_0_2_6_0_0_2_4); case_cleaner;)  \
               if (!::setjmp(case_1_1_0_2_6_0_0_2_4->ctx))

#define __H2Cases_Macro4(case_instance, Qx, i, x)                                          \
   static h2::h2_case case_instance(H2PP_STR(i. x), 0, {__FILE__, __LINE__});               \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance); \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4) Qx = x, case_1_1_0_2_6_0_0_2_4->scheduled = true;

#define __H2Cases_Macro3(_Args) H2PP_RESCAN(__H2Cases_Macro4 _Args)
#define __H2Cases_Macro2(...) __H2Cases_Macro3((__VA_ARGS__))
#define __H2Cases_Macro1(args, i, x) __H2Cases_Macro2(H2PP_UNIQUE(ci), H2PP_REMOVE_PARENTHESES(args), i, x)

#define H2Casess(...) __H2Casess(H2PP_UNIQUE(x), H2PP_UNIQUE(y), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), __VA_ARGS__)
#define __H2Casess(Qx, Qy, suite_cleaner, case_cleaner, ...)                                   \
   auto Qx = _H2_An_Fullmeshx(__VA_ARGS__);                                                    \
   auto Qy = _H2_An_Fullmeshy(__VA_ARGS__);                                                    \
   H2PP_FULLMESH((, __H2Casess_Macro1, (Qx, Qy), __VA_ARGS__));                                \
   if (case_1_1_0_2_6_0_0_2_4 && case_1_1_0_2_6_0_0_2_4->scheduled)                            \
      for (auto x = Qx; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)              \
         for (auto y = Qy; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)           \
            for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner;) \
               for (h2::h2_case::cleaner case_cleaner(case_1_1_0_2_6_0_0_2_4); case_cleaner;)  \
                  if (!::setjmp(case_1_1_0_2_6_0_0_2_4->ctx))

#define __H2Casess_Macro4(case_instance, Qx, Qy, i, j, x, y)                               \
   static h2::h2_case case_instance(H2PP_STR(i.j. x, y), 0, {__FILE__, __LINE__});          \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance); \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4) Qx = x, Qy = y, case_1_1_0_2_6_0_0_2_4->scheduled = true;

#define __H2Casess_Macro3(_Args) H2PP_RESCAN(__H2Casess_Macro4 _Args)
#define __H2Casess_Macro2(...) __H2Casess_Macro3((__VA_ARGS__))
#define __H2Casess_Macro1(args, i, j, x, y) __H2Casess_Macro2(H2PP_UNIQUE(ci), H2PP_REMOVE_PARENTHESES(args), i, j, x, y)

#define H2Cases_t(...) static_assert(false, "Cases_t not implemented, due to can't declare/define template class/function in function scope");
#define H2Casess_t(...) static_assert(false, "Casess_t not implemented, due to can't declare/define template class/function in function scope");

#define __H2CASES_T_Macro4(case_test, i, x) \
   H2CASE(i. x) { case_test<x>(); }
#define __H2CASES_T_Macro3(_Args) H2PP_RESCAN(__H2CASES_T_Macro4 _Args)
#define __H2CASES_T_Macro2(...) __H2CASES_T_Macro3((__VA_ARGS__))
#define __H2CASES_T_Macro1(args, i, x) __H2CASES_T_Macro2(H2PP_REMOVE_PARENTHESES(args), i, x)
#define __H2CASES_T(case_test, ...)                              \
   template <typename x>                                         \
   static void case_test();                                      \
   H2PP_FOREACH(, __H2CASES_T_Macro1, (case_test), __VA_ARGS__); \
   template <typename x>                                         \
   static void case_test()
#define H2CASES_T(...) __H2CASES_T(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define __H2CASESS_T_Macro4(case_test, i, j, x, y) \
   H2CASE(i.j. x, y) { case_test<x, y>(); }
#define __H2CASESS_T_Macro3(_Args) H2PP_RESCAN(__H2CASESS_T_Macro4 _Args)
#define __H2CASESS_T_Macro2(...) __H2CASESS_T_Macro3((__VA_ARGS__))
#define __H2CASESS_T_Macro1(args, i, j, x, y) __H2CASESS_T_Macro2(H2PP_REMOVE_PARENTHESES(args), i, j, x, y)
#define __H2CASESS_T(case_test, ...)                                 \
   template <typename x, typename y>                                 \
   static void case_test();                                          \
   H2PP_FULLMESH((, __H2CASESS_T_Macro1, (case_test), __VA_ARGS__)); \
   template <typename x, typename y>                                 \
   static void case_test()
#define H2CASESS_T(...) __H2CASESS_T(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define H2Todos(...) __H2Todos(__VA_ARGS__)
#define __H2Todos(...)    \
   H2Todo(__VA_ARGS__) {} \
   for (auto x = _H2_An_ForEach(__VA_ARGS__); false;)

#define H2Todoss(...) __H2Todoss(__VA_ARGS__)
#define __H2Todoss(...)                                 \
   H2Todo(__VA_ARGS__) {}                               \
   for (auto x = _H2_An_Fullmeshx(__VA_ARGS__); false;) \
      for (auto y = _H2_An_Fullmeshy(__VA_ARGS__); false;)

#define __H2TODOS(case_test, ...) \
   template <typename T>          \
   static void case_test(T x);    \
   H2TODO(__VA_ARGS__) {}         \
   template <typename T>          \
   static void case_test(T x)
#define H2TODOS(...) __H2TODOS(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define __H2TODOSS(case_test, ...)     \
   template <typename Tx, typename Ty> \
   static void case_test(Tx x, Ty y);  \
   H2TODO(__VA_ARGS__) {}              \
   template <typename Tx, typename Ty> \
   static void case_test(Tx x, Ty y)
#define H2TODOSS(...) __H2TODOSS(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define __H2TODOS_T(case_test, ...) \
   template <typename x>            \
   static void case_test();         \
   H2TODO(__VA_ARGS__) {}           \
   template <typename x>            \
   static void case_test()
#define H2TODOS_T(...) __H2TODOS_T(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define __H2TODOSS_T(case_test, ...) \
   template <typename x, typename y> \
   static void case_test();          \
   H2TODO(__VA_ARGS__) {}            \
   template <typename x, typename y> \
   static void case_test()
#define H2TODOSS_T(...) __H2TODOSS_T(H2PP_UNIQUE(case_test_C), __VA_ARGS__)

#define __H2GlobalCallback(Scope, Q)                        \
   namespace {                                              \
   static struct Q {                                        \
      Q() { h2::h2_runner::I().Scope##s.push_back(Scope); } \
      static void Scope();                                  \
   } H2PP_UNIQUE();                                         \
   }                                                        \
   void Q::Scope()

#define H2GlobalSetup() __H2GlobalCallback(global_setup, H2PP_UNIQUE())
#define H2GlobalCleanup() __H2GlobalCallback(global_cleanup, H2PP_UNIQUE())

#define H2GlobalSuiteSetup() __H2GlobalCallback(global_suite_setup, H2PP_UNIQUE())
#define H2GlobalSuiteCleanup() __H2GlobalCallback(global_suite_cleanup, H2PP_UNIQUE())

#define H2GlobalCaseSetup() __H2GlobalCallback(global_case_setup, H2PP_UNIQUE())
#define H2GlobalCaseCleanup() __H2GlobalCallback(global_case_cleanup, H2PP_UNIQUE())
// source/assert/h2_assert.hpp
struct h2_defer_failure : h2_once {
   const char *assert_type, *assert_op = ",";
   const char *e_expression, *a_expression;
   h2_fs fs;
   h2_fail* fails = nullptr;
   h2_ostringstream oss;

   h2_defer_failure(const char* e_expression_, const char* a_expression_, const h2_fs& fs_) : e_expression(e_expression_), a_expression(a_expression_), fs(fs_) {}
   ~h2_defer_failure();
};

template <typename E, typename A>
static inline h2_ostringstream& h2_ok2(h2_defer_failure* d, E e, A a, size_t n = 0, size_t = 0)
{
   d->assert_type = "OK";
   h2_fail* fail = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e).matches((typename h2_decay<A>::type)a, (int)n);
   d->fails = fail;
   if (fail && fail->subling_next) {
      d->fails = h2_fail::new_unexpect();
      h2_fail::append_child(d->fails, fail);
   }
   h2_assert_g();
   return d->oss;
}

static inline h2_ostringstream& h2_ok1(h2_defer_failure* d, bool a)
{
   d->assert_type = "OK1";
   if (!a) d->fails = h2_fail::new_unexpect("true", "false");
   h2_assert_g();
   return d->oss;
}

static inline h2_ostringstream& h2_je(h2_defer_failure* d, h2_string e, h2_string a, h2_string selector)
{
   d->assert_type = "JE";
   h2::h2_matcher<h2_string> m = Je(e, selector);
   d->fails = m.matches(a);
   h2_assert_g();
   return d->oss;
}

template <typename M, typename A>
struct h2_2cp {
   M m;
   A a;
   const char* op;
   h2_2cp(M m_, A a_, const char* op_) : m(m_), a(a_), op(op_) {}
};

template <typename T, typename E = typename h2_decay<T>::type>
struct h2_1cp {
   T t;
   explicit h2_1cp(T t_) : t(t_) {}
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_2cp<decltype(Eq((E)t)), A> operator==(const U& u) const { return {Eq((E)t), (A)u, "=="}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_2cp<decltype(Nq((E)t)), A> operator!=(const U& u) const { return {Nq((E)t), (A)u, "!="}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_2cp<decltype(Lt((E)t)), A> operator>(const U& u) const { return {Lt((E)t), (A)u, ">"}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_2cp<decltype(Gt((E)t)), A> operator<(const U& u) const { return {Gt((E)t), (A)u, "<"}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_2cp<decltype(Le((E)t)), A> operator>=(const U& u) const { return {Le((E)t), (A)u, ">="}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_2cp<decltype(Ge((E)t)), A> operator<=(const U& u) const { return {Ge((E)t), (A)u, "<="}; }
};

struct h2_0cp {
   template <typename T>
   h2_1cp<const T&> operator>(const T& t) const { return h2_1cp<const T&>{t}; }
};

template <typename E, typename A>
static inline h2_ostringstream& h2_cp(h2_defer_failure* d, h2_2cp<E, A> c)
{
   d->assert_type = "CP";
   d->assert_op = c.op;
   h2_fail* fail = h2::h2_matcher_cast<A>(c.m).matches(c.a, 0, {false, false, true});
   d->fails = fail;
   if (fail && fail->subling_next) {
      d->fails = h2_fail::new_unexpect();
      h2_fail::append_child(d->fails, fail);
   }
   h2_assert_g();
   return d->oss;
}

#define H2CP(...) __H2CP(H2PP_UNIQUE(), #__VA_ARGS__, __VA_ARGS__)
#define __H2CP(Q, expression, ...) for (h2::h2_defer_failure Q("", expression, {__FILE__, __LINE__}); Q;) h2::h2_cp(&Q, h2::h2_0cp() > __VA_ARGS__)

#define H2OK(_1, ...) H2PP_CAT(__H2OK, H2PP_IS_EMPTY(__VA_ARGS__))(H2PP_UNIQUE(), #_1, (#__VA_ARGS__), _1, __VA_ARGS__)
#define __H2OK1(Q, a_expression, _, actual, ...) for (h2::h2_defer_failure Q("", a_expression, {__FILE__, __LINE__}); Q;) h2::h2_ok1(&Q, actual)
#define __H2OK0(Q, e_expression, a_expression, expect, ...) for (h2::h2_defer_failure Q(e_expression, a_expression, {__FILE__, __LINE__}); Q;) (std::is_array<decltype(__VA_ARGS__)>::value ? h2::h2_ok2(&Q, expect, __VA_ARGS__, std::extent<decltype(__VA_ARGS__)>::value) : h2::h2_ok2(&Q, expect, __VA_ARGS__))

#define H2JE(...) H2PP_VARIADIC_CALL(__H2JE, H2PP_UNIQUE(), __VA_ARGS__)
#define __H2JE3(Q, expect, actual) for (h2::h2_defer_failure Q(#expect, #actual, {__FILE__, __LINE__}); Q;) h2::h2_je(&Q, expect, actual, "")
#define __H2JE4(Q, expect, actual, selector) for (h2::h2_defer_failure Q(#expect, #actual, {__FILE__, __LINE__}); Q;) h2::h2_je(&Q, expect, actual, selector)
// source/assert/h2_timer.hpp
struct h2_timer : h2_once {
   h2_fs fs;
   int cpu_ms;
   clock_t start;
   h2_timer(int ms, const h2_fs& fs);
   ~h2_timer();
};

#define __H2PF(ms, Q) for (h2::h2_timer Q(ms, {__FILE__, __LINE__}); Q;)
#define H2PF(ms) __H2PF(ms, H2PP_UNIQUE())
// source/render/h2_report.hpp
struct h2_report {
   h2_singleton(h2_report);
   static void initialize();

   bool in = true;
   long long escape_length = 0;
   h2_list reports;
   void on_runner_start(h2_runner* r);
   void on_runner_endup(h2_runner* r);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};
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
#define Teardown H2Teardown
#endif

#ifndef H2_NO_Cleanup
#define Cleanup H2Teardown
#endif

#ifndef H2_NO_CP
#define CP H2CP
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

using h2::_;
using h2::Any;
#define IsNull h2::_IsNull()
#define NotNull h2::_NotNull()
#define IsTrue h2::_IsTrue()
#define IsFalse h2::_IsFalse()
using h2::Eq;
using h2::Nq;
using h2::Ge;
using h2::Gt;
using h2::Le;
using h2::Lt;
#ifndef H2_NO_Me
#define Me H2Me
#endif
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
using h2::Not;
using h2::operator&&;
using h2::operator||;
using h2::AllOf;
using h2::AnyOf;
using h2::NoneOf;
using h2::ListOf;
using h2::CountOf;
using h2::Has;
using h2::Pair;

#ifdef __cplusplus
#define private public
#define protected public
#endif

#endif
#endif
