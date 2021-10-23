
/* v5.15 2021-10-23 20:00:16 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#ifndef __H2UNIT_H__
#define __H2UNIT_H__
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

#include <cassert>   /* assert */
#include <cctype>    /* tolower, isspace */
#include <cstdarg>   /* va_list */
#include <errno.h>   /* strerror */
#include <exception> /* std::exception */
#include <iostream>  /* std::cout, std::streambuf */
#include <memory>    /* std::allocator */
#include <regex>     /* std::regex */
#include <signal.h>  /* sigaction */
#include <time.h>    /* clock */
#include <typeinfo>  /* std::typeid, std::type_info */

#if !defined _MSC_VER
#include <cxxabi.h> /* abi::__cxa_demangle, abi::__cxa_throw */
#endif

#if defined __linux || defined __APPLE__  // -MSCV -Cygwin -MinGW
#include <execinfo.h>                     /* backtrace, backtrace_symbols */
#endif

#if defined __GLIBC__
#include <malloc.h> /* __malloc_hook */
#endif

#if defined _WIN32  // +MinGW
#ifndef NOMINMAX    // MinGW already defined
#define NOMINMAX    // fix std::min/max conflict with windows::min/max
#endif
#include <winsock2.h> /* socket */
#include <windows.h>
#include <ws2tcpip.h> /* getaddrinfo */
#include <io.h>       /* _write */
#define strcasecmp _stricmp
#define fileno _fileno
#define socklen_t int
#define ssize_t int
#endif

#if defined __CYGWIN__
#include <windows.h>
#endif

#if defined _WIN32 || defined __CYGWIN__  // +MinGW
#include <dbghelp.h>                      /* CaptureStackBackTrace, SymFromAddr */
#include <shlwapi.h>                      /* StrStrIA */
#define strcasestr StrStrIA
#endif

#if defined _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Dbghelp.lib")
#endif

#if defined __linux || defined __APPLE__ || defined __CYGWIN__
#include <arpa/inet.h>  /* inet_addr, inet_pton */
#include <fcntl.h>      /* fcntl */
#include <fnmatch.h>    /* fnmatch */
#include <netdb.h>      /* getaddrinfo, gethostbyname */
#include <sys/time.h>   /* gettimeofday */
#include <sys/ioctl.h>  /* ioctl */
#include <sys/mman.h>   /* mprotect, mmap */
#include <sys/socket.h> /* sockaddr */
#include <sys/types.h>  /* size_t */
#include <syslog.h>     /* syslog, vsyslog */
#include <unistd.h>     /* sysconf */
#endif

#if defined __APPLE__
#include <AvailabilityMacros.h>
#include <malloc/malloc.h> /* malloc_zone_t */
#endif

#if defined _WIN32  // +MinGW
#define LIBC__write ::_write
#else
#define LIBC__write ::write
#endif

#if defined _MSC_VER
#define h2__stdcall __stdcall
#else
#define h2__stdcall
#endif

#if defined _WIN32 || defined __CYGWIN__  // +MinGW
int main(int argc, const char** argv);
#if defined __H2UNIT_HPP__ || defined IMPORT_MAIN
int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#endif
#else
__attribute__((weak)) int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#endif

namespace h2 {
// source/utils/h2_fs.cpp
h2_inline bool h2_fs::empty() const
{
   return !file || file[0] == '\0';
}
h2_inline const char* h2_fs::basefile() const
{
   if (file)
      for (const char* p = file + strlen(file); file <= p; --p)
         if (*p == '/' || *p == '\\')
            return p + 1;
   return file;
}
// source/utils/h2_list.cpp
inline void add_between(h2_list* thus, h2_list* prev, h2_list* next)
{
   next->prev = thus;
   thus->next = next;
   thus->prev = prev;
   prev->next = thus;
}

h2_inline h2_list& h2_list::add_head(h2_list& entry)
{
   add_between(&entry, this, this->next);
   return *this;
}

h2_inline h2_list& h2_list::add_tail(h2_list& entry)
{
   add_between(&entry, this->prev, this);
   return *this;
}

h2_inline h2_list& h2_list::add_before(h2_list& entry)
{
   add_between(&entry, this->prev, this);
   return *this;
}

h2_inline h2_list& h2_list::add_after(h2_list& entry)
{
   add_between(&entry, this, this->next);
   return *this;
}

h2_inline h2_list& h2_list::out()
{
   prev->next = next;
   next->prev = prev;
   next = prev = this;
   return *this;
}

h2_inline int h2_list::count() const
{
   int c = 0;
   for (auto p = next; p != this; p = p->next) ++c;
   return c;
}

h2_inline void h2_list::sort(int (*cmp)(h2_list*, h2_list*))
{
   h2_list sorted, *q;

   while (!empty()) {
      h2_list& p = pop();
      for (q = sorted.next; q != &sorted; q = q->next)
         if (0 < cmp(q, &p))
            break;
      q->add_before(p);
   }
   while (!sorted.empty()) {
      push_back(sorted.pop());
   }
}
// source/utils/h2_numeric.cpp
static inline bool not2n(unsigned x)
{
   return x & (x - 1);
}

static inline unsigned mask2n(unsigned x)
{
   x |= (x >> 1);
   x |= (x >> 2);
   x |= (x >> 4);
   x |= (x >> 8);
   x |= (x >> 16);
   return x;
}

static inline int hex_to_byte(char c)
{
   return '0' <= c && c <= '9' ? c - '0' : ('A' <= c && c <= 'F' ? c - 'A' + 10 : ('a' <= c && c <= 'f' ? c - 'a' + 10 : -1));
}

h2_inline size_t h2_numeric::hex_to_bytes(const char* hex, unsigned char* bytes)
{
   char b;
   size_t i = 0, c = 0;

   for (; ::isxdigit(hex[c]);) ++c;
   if (c % 2 == 1) {
      b = '0';
      i = 1;
      c += 1;
      hex = hex - 1;
   }
   for (; i < c; ++i) {
      if (i % 2 == 1)
         bytes[i / 2] = (unsigned char)((hex_to_byte(b) << 4) + hex_to_byte(hex[i]));
      b = hex[i];
   }
   return c / 2;
}

h2_inline size_t h2_numeric::bin_to_bits(const char* bin, unsigned char* bytes)
{
   memset(bytes, 0, strlen(bin));
   size_t c = 0;
   for (const char* p = bin; *p; p++) {
      if (*p == ' ') continue;
      size_t i = c / 8, j = 7 - c % 8;
      ++c;
      unsigned char ebit = *p == '1' ? 1 : 0;
      bytes[i] = bytes[i] | (ebit << j);
   }
   return c;
}

h2_inline bool h2_numeric::bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits)
{
   for (size_t k = 0; k < nbits; ++k) {
      size_t i = k / 8, j = 7 - k % 8;
      if (((b1[i] >> j) & 1) != ((b2[i] >> j) & 1)) return false;
   }
   return true;
}

h2_inline bool h2_numeric::is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; p++)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

h2_inline bool h2_numeric::is_bin_string(const char* s)
{
   for (const char* p = s; *p; p++)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

h2_inline const char* h2_numeric::sequence_number(size_t sequence, size_t shift)
{
   static const char* st[] = {"0th", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th"};
   static char ss[64];

   sequence += shift;
   if (sequence < sizeof(st) / sizeof(st[0])) return st[sequence];
   sprintf(ss, "%dth", (int)sequence);
   return ss;
}

static inline size_t number_strlen(unsigned long long number, int base)
{
   unsigned long long _10000000 = 1;
   for (size_t i = 1;; ++i) {
      _10000000 *= base;
      if (number < _10000000) return i;
   }
}

static inline const char* format_duration(long long ms)
{
   static char st[128];
   if (ms < 100)
      sprintf(st, "%lld milliseconds", ms);
   else if (ms < 1000 * 60)
      sprintf(st, "%.2g second%s", ms / 1000.0, ms == 1000 ? "" : "s");
   else if (ms < 1000 * 60 * 60)
      sprintf(st, "%.2g minute%s", ms / 60000.0, ms == 60000 ? "" : "s");
   else
      sprintf(st, "%.2g hour%s", ms / 3600000.0, ms == 3600000 ? "" : "s");

   return st;
}

static inline const char* format_volume(long long footprint)
{
   static char st[128];
   if (footprint < 1024LL)
      sprintf(st, "%lld", footprint);
   else if (footprint < 1024LL * 1024LL)
      sprintf(st, "%.2gKB", footprint / 1024.0);
   else if (footprint < 1024LL * 1024LL * 1024LL)
      sprintf(st, "%.2gMB", footprint / (1024.0 * 1024.0));
   else
      sprintf(st, "%.2gGB", footprint / (1024.0 * 1024.0 * 1024.0));
   return st;
}
// source/utils/h2_pattern.cpp
h2_inline bool h2_pattern::regex_match(const char* pattern, const char* subject, bool caseless)
{
   bool result = false;
#if !defined _WIN32 || !defined NDEBUG  // Windows regex suck under release version and memory check
   h2_memory::hook(false);
   try {  // c++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error&) {
      result = false;
   }
   h2_memory::hook();
#endif
   return result;
}

h2_inline bool h2_pattern::wildcard_match(const char* pattern, const char* subject, bool caseless)
{
#if defined _WIN32 || defined __CYGWIN__
   const char *scur = subject, *pcur = pattern;
   const char *sstar = nullptr, *pstar = nullptr;
   while (*scur) {
      if (*pcur == '?') {
         ++scur;
         ++pcur;
      } else if (*pcur == '*') {
         pstar = pcur++;
         sstar = scur;
      } else if (caseless ? ::tolower(*scur) == ::tolower(*pcur) : *scur == *pcur) {
         ++scur;
         ++pcur;
      } else if (pstar) {
         pcur = pstar + 1;
         scur = ++sstar;
      } else
         return false;
   }
   while (*pcur == '*') ++pcur;
   return !*pcur;
#else
   return !fnmatch(pattern, subject, caseless ? FNM_CASEFOLD : 0);
#endif
}

h2_inline bool h2_pattern::match(const char* pattern, const char* subject, bool caseless)
{
   return wildcard_match(pattern, subject, caseless) || regex_match(pattern, subject, caseless);
}
// source/utils/h2_diff.cpp

struct h2_fuzzy {
   static size_t levenshtein(const char* s1, const char* s2, size_t n1, size_t n2, bool caseless)
   {
      h2_vector<size_t> col(n2 + 1), prevCol(n2 + 1);
      for (h2_vector<size_t>::size_type i = 0; i < prevCol.size(); i++)
         prevCol[i] = (int)i;
      for (size_t i = 0; i < n1; i++) {
         col[0] = i + 1;
         for (size_t j = 0; j < n2; j++) {
            bool eq = caseless ? ::tolower(s1[i]) == ::tolower(s2[j]) : s1[i] == s2[j];
            col[j + 1] = std::min(std::min(1 + col[j], 1 + prevCol[1 + j]), prevCol[j] + (eq ? 0 : 1));
         }
         col.swap(prevCol);
      }
      return prevCol[n2];
   }

   // 1 absolute match, 0 absolute not match
   static double similarity(const char* s1, const char* s2, bool caseless)
   {
      size_t n1 = strlen(s1), n2 = strlen(s2);
      if (!n1 && !n2) return 1;
      return 1.0 - levenshtein(s1, s2, n1, n2, caseless) / ((n1 + n2) * 0.5);
   }
};

// https://en.wikipedia.org/wiki/Longest_common_subsequence_problem
struct h2_LCS {
   h2_vector<h2_string> s1, s2;
   bool caseless;
   h2_vector<h2_vector<int>> c, d, p;

   h2_LCS(h2_vector<h2_string> s1_, h2_vector<h2_string> s2_, bool caseless_ = false) : s1(s1_), s2(s2_), caseless(caseless_) {}

   void LCS_table()
   {
      for (size_t i = 0; i < s1.size() + 1; i++) {
         c.push_back(h2_vector<int>(s2.size() + 1));
         d.push_back(h2_vector<int>(s2.size() + 1));
         p.push_back(h2_vector<int>(s2.size() + 1));
      }

      for (size_t i = 0; i < s1.size() + 1; i++) c[i][0] = 0;
      for (size_t i = 0; i < s2.size() + 1; i++) c[0][i] = 0;

      for (size_t i = 1; i < s1.size() + 1; i++) {
         for (size_t j = 1; j < s2.size() + 1; j++) {
            if (s1[i - 1].equals(s2[j - 1], caseless)) {
               c[i][j] = c[i - 1][j - 1] + 1;
               d[i][j] = 1030; // 10:30 upper left
            } else if (c[i - 1][j] > c[i][j - 1]) {
               c[i][j] = c[i - 1][j];
               d[i][j] = 900; // 9:30 left
            } else {
               c[i][j] = c[i][j - 1];
               d[i][j] = 1200; // 12:00 upper
            }
         }
      }
   }

   void LCS_traceback(size_t i, size_t j)
   {
      if (i == 0 || j == 0) return;
      if (d[i][j] == 1030) { // 10:30
         p[i][j] = 1;
         LCS_traceback(i - 1, j - 1);
      } else if (d[i][j] == 900) { // 9:00
         LCS_traceback(i - 1, j);
      } else { // 1200 12:00
         LCS_traceback(i, j - 1);
      }
   }

   std::pair<h2_vector<int>, h2_vector<int>> lcs()
   {
      LCS_table();
      LCS_traceback(s1.size(), s2.size());

      h2_vector<int> l1(s1.size()), l2(s2.size());
      for (size_t i = 1; i < s1.size() + 1; i++) {
         l1[i - 1] = 0;
         for (size_t j = 1; j < s2.size() + 1; j++)
            if (p[i][j]) l1[i - 1] = 1;
      }
      for (size_t j = 1; j < s2.size() + 1; j++) {
         l2[j - 1] = 0;
         for (size_t i = 1; i < s1.size() + 1; i++)
            if (p[i][j]) l2[j - 1] = 1;
      }
      return {l1, l2};
   }
};
// source/utils/h2_misc.cpp
static inline const char* get_key(const char* subject, const char* key)
{
   return strcasestr(subject, key);
}
static inline const char* get_eq(const char* start)
{
   for (; *start && ::isspace(*start);) start++;  //strip left space
   return *start == '=' ? start : nullptr;
}

h2_inline const char* h2_extract::has(const char* attributes, const char* key)
{
   return strcasestr(attributes, key);
}

h2_inline bool h2_extract::numeric(const char* attributes, const char* key, double& value)
{
   const char* p_key = get_key(attributes, key);
   if (!p_key) return false;
   const char* p_eq = get_eq(p_key + strlen(key));
   if (!p_eq) return false;
   const char* p_numeric = p_eq + 1;
   for (; *p_numeric && ::isspace(*p_numeric);) p_numeric++;  //strip left space
   value = strtod(p_numeric, nullptr);
   return true;
}

h2_inline bool h2_extract::iport(const char* attributes, const char* key, char* str)
{
   const char* p_key = get_key(attributes, key);
   if (!p_key) return false;
   const char* p_eq = get_eq(p_key + strlen(key));
   if (!p_eq) return false;

   for (const char* p = p_eq + 1; *p; p++) {
      if (::isdigit(*p) || *p == '.' || *p == ':' || *p == '*' || *p == '?') {
         *str++ = *p;
         *str = '\0';
      } else {
         if (!(::isspace(*p) || *p == '\"')) break;
      }
   }
   return true;
}

h2_inline int h2_extract::fill(const char* attributes, const char* key, unsigned char bytes[])
{
   const char* p_key = get_key(attributes, key);
   if (!p_key) return 0;
   const char* p_eq = get_eq(p_key + strlen(key));
   if (!p_eq) return 0;
   const char* p = p_eq + 1;
   for (; *p && ::isspace(*p);) p++;  // strip left space
   if (p[0] == '0' && ::tolower(p[1]) == 'x') {
      return (int)h2_numeric::hex_to_bytes(p + 2, bytes);
   } else {
      unsigned long long v = strtoull(p, nullptr, 10);
      if (v <= 0xFFULL)
         return *((unsigned char*)bytes) = (unsigned char)v, 1;
      else if (v <= 0xFFFFULL)
         return *((unsigned short*)bytes) = (unsigned short)v, 2;
      else if (v <= 0xFFFFFFFFULL)
         return *((unsigned int*)bytes) = (unsigned int)v, 4;
      else
         return *((unsigned long long*)bytes) = (unsigned long long)v, 8;
   }
}

static inline long long h2_now()
{
#if defined _WIN32
   return GetTickCount();
#else
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
#endif
}

static inline void h2_sleep(long long milliseconds)
{
#if defined _WIN32
   Sleep((DWORD)milliseconds);
#else
   ::usleep(milliseconds * 1000);
#endif
}

static inline unsigned h2_page_size()
{
#if defined _WIN32
   SYSTEM_INFO si;
   GetSystemInfo(&si);
   return (unsigned)si.dwPageSize;
#else
   return sysconf(_SC_PAGESIZE);
#endif
}

static inline bool h2_in(const char* x, const char* s[], int n = 0)
{
   for (int i = 0; s[i] && i < (n ? n : 1000); ++i)
      if (!strcmp(s[i], x)) return true;
   return false;
}

static inline const char* comma_if(bool a, const char* t = ", ", const char* f = "") { return a ? t : f; };

#define h2_sprintvf(str, fmt, ap)               \
   do {                                         \
      va_list bp;                               \
      va_copy(bp, ap);                          \
      int len = vsnprintf(nullptr, 0, fmt, bp); \
      str = (char*)alloca(len + 1);             \
      va_end(bp);                               \
      len = vsnprintf(str, len + 1, fmt, ap);   \
   } while (0)

#define h2_sprintf(str, fmt)     \
   do {                          \
      va_list ap;                \
      va_start(ap, fmt);         \
      h2_sprintvf(str, fmt, ap); \
      va_end(ap);                \
   } while (0)
// source/utils/h2_libc.cpp
struct h2_libc_malloc {
   h2_singleton(h2_libc_malloc);

   struct buddy {
      unsigned long long size;
      h2_list x;
      buddy(const unsigned long long size_) : size(size_) {}
      bool join_right(buddy* b) { return ((char*)this) + size == (char*)b; }
      bool join_left(buddy* b) { return ((char*)b) + b->size == (char*)this; }
   };

   struct block {
      unsigned long long bytes;
      block* next = nullptr;
      h2_list buddies;

      block(const unsigned long long bytes_) : bytes(bytes_)
      {
         buddy* b = new ((char*)this + sizeof(block)) buddy(bytes - sizeof(block));
         buddies.add_tail(b->x);
      }

      buddy* malloc(const unsigned long long size)
      {
         h2_list_for_each_entry (p, buddies, buddy, x) {
            if (size + sizeof(p->size) <= p->size) {
               unsigned long long left = p->size - (size + sizeof(p->size));
               if (sizeof(buddy) + 64 <= left) {  // avoid smash buddy for performance
                  buddy* b = new ((char*)p + left) buddy(size + sizeof(b->size));
                  p->size = left;
                  return b;
               } else {
                  p->x.out();
                  return p;
               }
            }
         }
         return nullptr;
      }

      bool free(buddy* b)
      {
         if ((char*)b < (char*)this && (char*)this + bytes <= (char*)b) return false;
         h2_list_for_each_entry (p, buddies, buddy, x) {
            if (p->join_right(b)) {
               p->size += b->size;
               return true;
            }
            if (p->join_left(b)) {
               p->x.add_before(b->x);
               b->size += p->size;
               p->x.out();
               return true;
            }
            if ((char*)b + b->size < (char*)p) {
               p->x.add_before(b->x);
               return true;
            }
         }
         buddies.add_tail(b->x);
         return true;
      }
   };

   block* next = nullptr;

   void batch(const unsigned long long size)
   {
      size_t brk_size = 4 * 1024 * 1024;
      size_t brk_count = (size_t)::ceil(size / (double)brk_size);

#if defined _WIN32
      PVOID ptr = VirtualAlloc(NULL, brk_count * brk_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if (ptr == NULL) ::printf("VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      void* ptr = ::mmap(nullptr, brk_count * brk_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (ptr == MAP_FAILED) ::printf("mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      block* p = new (ptr) block(brk_count * brk_size);
      p->next = next;
      next = p;
   }

   buddy* alloc(const unsigned long long size)
   {
      for (block* p = next; p; p = p->next) {
         buddy* b = p->malloc(size);
         if (b) return b;
      }
      return nullptr;
   }

   void* malloc(const size_t size)
   {
      unsigned long long _size = (size + 7) / 8 * 8;
      buddy* b = alloc(_size);
      if (!b) {
         batch(_size);
         b = alloc(_size);
      }

      return b ? (void*)&b->x : nullptr;
   }

   void free(void* ptr)
   {
      if (ptr) {
         buddy* b = (buddy*)((char*)ptr - sizeof(b->size));
         for (block* p = next; p; p = p->next)
            if (p->free(b)) return;
      }
   }
};

h2_inline void* h2_libc::malloc(size_t size)
{
   if (!O.memory_check) return ::malloc(size);
   return h2_libc_malloc::I().malloc(size + 10);
}

h2_inline void h2_libc::free(void* ptr)
{
   if (!O.memory_check) return ::free(ptr);
   if (ptr) h2_libc_malloc::I().free(ptr);
}
// source/utils/h2_string.cpp
static inline size_t utf8len(const char* s)
{
   if (0xf0 == (0xf8 & *(const unsigned char*)s)) return 4;  // 4-byte utf8 code point (began with 0b11110xxx)
   if (0xe0 == (0xf0 & *(const unsigned char*)s)) return 3;  // 3-byte utf8 code point (began with 0b1110xxxx)
   if (0xc0 == (0xe0 & *(const unsigned char*)s)) return 2;  // 2-byte utf8 code point (began with 0b110xxxxx)
   return 1;                                                 // 1-byte ascii (began with 0b0xxxxxxx)
}

h2_inline h2_string& h2_string::sprintf(const char* format, ...)
{
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   append(alloca_str);
   return *this;
}

h2_inline h2_string& h2_string::replace_all(const char* from, const char* to)
{
   size_t start_pos = 0, from_length = strlen(from), to_length = strlen(to);
   while ((start_pos = find(from, start_pos)) != h2_string::npos) {
      replace(start_pos, from_length, to);
      start_pos += to_length;  // where 'to' is a substring of 'from'
   }
   return *this;
}

h2_inline size_t h2_string::width(size_t columns) const  // wcwidth()/wcswidth()
{
   size_t w = 0, n = 0;
   for (const char* p = c_str(); *p != '\0'; p += n) {
      n = utf8len(p);
      w += (n == 1 ? 1 : columns);
   }
   return w;
}

h2_inline bool h2_string::equals(const h2_string& str, bool caseless) const
{
   if (!caseless) return *this == str;
   return tolower() == str.tolower();
}

h2_inline bool h2_string::contains(const h2_string& substr, bool caseless) const
{
   if (!caseless) return find(substr) != h2_string::npos;
   return tolower().find(substr.tolower()) != h2_string::npos;
}

h2_inline bool h2_string::startswith(const h2_string& prefix, bool caseless) const
{
   if (size() < prefix.size()) return false;
   if (!caseless) return find(prefix) == 0;
   return tolower().find(prefix.tolower()) == 0;
}

h2_inline bool h2_string::endswith(const h2_string& suffix, bool caseless) const
{
   if (size() < suffix.size()) return false;
   if (!caseless) return rfind(suffix) == size() - suffix.size();
   return tolower().rfind(suffix.tolower()) == size() - suffix.size();
}

h2_inline bool h2_string::isspace() const
{
   for (auto& c : *this)
      if (!::isspace(c)) return false;
   return true;
}

h2_inline bool h2_string::enclosed(const char c) const
{
   return front() == c && back() == c;
}

h2_inline h2_string h2_string::escape(bool utf8) const
{
   h2_string s;
   for (auto& c : *this) {
      switch (c) {
         case '\n': s.append(utf8 ? "␍" : "\\n"); break;
         case '\r': s.append(utf8 ? "␊" : "\\r"); break;
         case '\t': s.append(utf8 ? "␉" : "\\t"); break;
         case '\0': s.append(utf8 ? "␀" : "\0"); break;
         default: s.push_back(c); break;
      }
   }
   return s;
}

h2_inline h2_string h2_string::unescape() const
{
   h2_string s = *this;
   s.replace_all("\\b", "\b");
   s.replace_all("\\f", "\f");
   s.replace_all("\\n", "\n");
   s.replace_all("\\r", "\r");
   s.replace_all("\\t", "\t");
   s.replace_all("\\\"", "\"");
   s.replace_all("\\\\", "\\");
   // todo: escape \u12ab
   return s;
}

h2_inline h2_string h2_string::unquote(const char c) const
{
   if (!enclosed(c)) return *this;
   return h2_string(size() - 2, c_str() + 1);
}

h2_inline h2_string h2_string::trim() const
{
   const auto a = find_first_not_of("\t\n ");
   if (a == h2_string::npos) return "";
   const auto b = find_last_not_of("\t\n ");
   return substr(a, b - a + 1).c_str();
}

h2_inline h2_string h2_string::squash(bool quote) const
{
   h2_string s;
   bool quote1 = false, quote2 = false;
   int spaces = 0;
   for (char c : trim()) {
      if (c == '\t' || c == '\n' || c == ' ') c = ' ';
      if (c != ' ') spaces = 0;
      if (!quote && c == '\'') quote1 = !quote1;
      if (!quote && c == '\"') quote2 = !quote2;
      if (!quote1 && !quote2 && c == ' ' && spaces++) continue;
      s.push_back(c);
   }
   return s;
}

h2_inline h2_string h2_string::tolower() const
{
   h2_string s;
   for (auto& c : *this) s.push_back(::tolower(c));
   return s;
}

h2_inline h2_string h2_string::center(size_t width) const
{
   if (width <= size()) return *this;
   size_t left = (width - size()) / 2, right = width - left - size();
   h2_string s;
   s.append(left, ' ');
   s.append(*this);
   s.append(right, ' ');
   return s;
}

h2_inline h2_vector<h2_string> h2_string::disperse() const
{
   h2_vector<h2_string> chars;
   for (const char* p = c_str(); *p != '\0';) {
      auto n = utf8len(p);
      chars.push_back(h2_string(n, p));
      p += n;
   }
   return chars;
}
// source/utils/h2_line.cpp
h2_inline size_t h2_line::width(bool ignore_indent) const
{
   size_t w = 0;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         if (!ignore_indent || !word.isspace())
            w += word.width();
   return w;
}

h2_inline h2_line& h2_line::indent(size_t n, const char c)
{
   if (n) insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::padding(size_t n, const char c)
{
   if (n) push_back(h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_line& h2_line::operator+=(const h2_line& line)
{
   insert(end(), line.begin(), line.end());
   return *this;
}

h2_inline h2_line& h2_line::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
   return *this;
}

h2_inline bool h2_line::enclosed(const char c) const
{
   bool f = false, ff = false, b = false;
   for (auto& word : *this) {
      if (!h2_color::isctrl(word.c_str())) {
         if (!ff) f = word.front() == c;
         ff = true;
         b = word.back() == c;
      }
   }

   return f && b;
}

h2_inline h2_line h2_line::gray_quote() const
{
   if (!enclosed('\"') && !enclosed('\'')) return *this;

   h2_line line;
   size_t i = 0, w = width();

   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         line.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i == 0) {
            h.append(c.c_str());
         } else if (i == w - 1) {
            t.append(c.c_str());
         } else {
            m.append(c.c_str());
         }
         i += c.width();
      }
      if (h.size()) line += gray(h);
      if (m.size()) line.push_back(m);
      if (t.size()) line += gray(t);
   }

   return line;
}

h2_inline h2_line h2_line::abbreviate(size_t width, size_t tail) const
{
   h2_line line1, line2;
   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str()))
         line1.push_back(word);
      else
         line1.push_back(word.escape());
   }

   size_t i = 0, line1_width = line1.width();
   if (line1_width <= width) return line1;

   for (auto& word : line1) {
      if (h2_color::isctrl(word.c_str())) {
         line2.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i < width - 3 - tail) {
            h.append(c.c_str());
         } else if (i == width - 3 - tail) {
            m = "...";
         } else if (line1_width - tail <= i) {
            t.append(c.c_str());
         }
         i += c.width();
      }
      if (h.size()) line2.push_back(h);
      if (m.size()) line2 += gray(m);
      if (t.size()) line2.push_back(t);
   }

   return line2;
}

h2_inline h2_string h2_line::string() const
{
   h2_string s;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         s += word;
   return s;
}

h2_inline void h2_line::samesizify(h2_line& a, h2_line& b)
{
   size_t a_w = a.width(), b_w = b.width();
   a.padding(std::max(a_w, b_w) - a_w);
   b.padding(std::max(a_w, b_w) - b_w);
}

///// h2_lines
h2_inline h2_lines& h2_lines::operator+=(const h2_lines& lines)
{
   insert(end(), lines.begin(), lines.end());
   return *this;
}

h2_inline size_t h2_lines::width() const
{
   size_t m = 0;
   for (auto& line : *this)
      m = std::max(m, line.width());
   return m;
}

h2_inline bool h2_lines::foldable(size_t width) const
{
   size_t sum = 0;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.size();

   return sum < width;
}

h2_inline h2_line h2_lines::folds() const
{
   h2_line folded_line;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace())  // drop indent
            folded_line.push_back(word);
   return folded_line;
}

h2_inline h2_string h2_lines::string() const
{
   h2_string s;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_lines::sequence(size_t indent, size_t start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = gray(h2_string("%d. ", (int)(i + start))) + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_lines::samesizify(h2_lines& a, h2_lines& b)
{
   auto size = std::max(a.size(), b.size());
   for (auto i = a.size(); i < size; ++i) a.push_back(h2_line());
   for (auto i = b.size(); i < size; ++i) b.push_back(h2_line());
}
// source/utils/h2_color.cpp
struct h2_shell {
   h2_singleton(h2_shell);
   char current[8][32];
   size_t cww;

   h2_shell()
   {
      memset(current, 0, sizeof(current));
      cww = 120;
#if defined _WIN32
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      int columns, rows;

      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
      columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
      rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

      cww = 16 < columns ? columns : 120;
#else
      struct winsize w;
      if (-1 != ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) cww = 16 < w.ws_col ? w.ws_col : 120;
#endif
   }

   void clear_style()
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         current[i][0] = '\0';
   }
   void push_style(const char* style, size_t length)
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] == '\0') {
            strncpy(current[i], style, length);
            current[i][length] = '\0';
            break;
         }
   }
   void pop_style(const char* style, size_t length)
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (!strncmp(current[i], style, length) && strlen(current[i]) == length)
            current[i][0] = '\0';
   }
   void change()
   {
      char a[256];
      sprintf(a, "\033[%d;", style2value("reset"));
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            sprintf(a + strlen(a), "%d;", style2value(current[i]));
      a[strlen(a) - 1] = 'm';
      LIBC__write(1, a, strlen(a));
   }
   void parse(const char* style)
   {
      const char* p = style + 2;
      char s = '+';
      if (*p == '+' || *p == '-') s = *p++;

      for (;;) {
         size_t l = strcspn(p, ",}");
         s == '-' ? pop_style(p, l) : push_style(p, l);
         if (!strncmp("reset", p, l)) clear_style();
         if (*(p + l) == '}' || *(p + l) == '\0') break;
         p += l + 1;
      }
   }
   void print(const char* str)
   {
      /* Windows PowerShell works, but CMD not, refer to v5.11 SetConsoleTextAttribute */
      if (h2_color::isctrl(str)) {
         if (h2_option::I().colorful) I().parse(str), I().change();
      } else {
         LIBC__write(fileno(stdout), str, strlen(str));
      }
   }
   int style2value(const char* style)  // https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_sequences
   {
      if (!strcmp(style, "reset")) return 0;
      if (!strcmp(style, "bold")) return 1;
      if (!strcmp(style, "italics")) return 3;
      if (!strcmp(style, "underline")) return 4;
      if (!strcmp(style, "inverse")) return 7;
      if (!strcmp(style, "strikethrough")) return 9;
      if (!strcmp(style, "black")) return 30;
      if (!strcmp(style, "red")) return 31;
      if (!strcmp(style, "green")) return 32;
      if (!strcmp(style, "yellow")) return 33;
      if (!strcmp(style, "blue")) return 34;
      if (!strcmp(style, "purple")) return 35;
      if (!strcmp(style, "cyan")) return 36;
      if (!strcmp(style, "gray")) return 37;
      if (!strcmp(style, "default")) return 39;
      if (!strcmp(style, "dark gray")) return 90;
      if (!strcmp(style, "light red")) return 91;
      if (!strcmp(style, "light green")) return 92;
      if (!strcmp(style, "light yellow")) return 93;
      if (!strcmp(style, "light blue")) return 94;
      if (!strcmp(style, "light purple")) return 95;
      if (!strcmp(style, "light cyan")) return 96;
      if (!strcmp(style, "white")) return 97;
      if (!strcmp(style, "bg_black")) return 40;
      if (!strcmp(style, "bg_red")) return 41;
      if (!strcmp(style, "bg_green")) return 42;
      if (!strcmp(style, "bg_yellow")) return 43;
      if (!strcmp(style, "bg_blue")) return 44;
      if (!strcmp(style, "bg_purple")) return 45;
      if (!strcmp(style, "bg_cyan")) return 46;
      if (!strcmp(style, "bg_white")) return 47;
      return 0;
   }
};

h2_inline void h2_color::prints(const char* style, const char* format, ...)
{
   if (style && strlen(style)) {
      char t[128];
      sprintf(t, "\033{%s}", style);
      h2_shell::I().print(t);
   }

   char* alloca_str;
   h2_sprintf(alloca_str, format);
   h2_shell::I().print(alloca_str);

   if (style && strlen(style)) h2_shell::I().print("\033{reset}");
}

h2_inline void h2_color::printl(const h2_line& line, bool cr)
{
   for (auto& word : line) h2_shell::I().print(word.c_str());
   if (cr) h2_shell::I().print("\n");
}

h2_inline void h2_color::printl(const h2_lines& lines, bool cr)
{
   for (auto& line : lines) printl(line, cr);
}
// source/symbol/h2_nm.cpp
#if !defined _MSC_VER
static inline void nm_mangle(std::map<std::string, unsigned long long>*& symbols)
{
   h2_memory::hook(false);
   char nm[256], line[2048], addr[128], type, name[2048];
   symbols = new std::map<std::string, unsigned long long>();
#   if defined __APPLE__
   sprintf(nm, "nm -U %s", O.path);
#   else
   sprintf(nm, "nm --defined-only %s", O.path);
#   endif
   FILE* f = ::popen(nm, "r");
   if (f) {
      while (::fgets(line, sizeof(line) - 1, f)) {
         if (3 != sscanf(line, "%s %c %s", addr, &type, name)) continue;
         if (strchr("bBcCdDiIuU", type)) continue;  // reject bBcCdDiIuU, accept tTwWsSvV, sS for vtable
         int underscore = 0;
         if (O.os == 'm') underscore = 1;  // remove prefix '_' in MacOS
         (*symbols)[name + underscore] = (unsigned long long)strtoull(addr, nullptr, 16);
      }
      ::pclose(f);
   }
   h2_memory::hook();
}

static inline void nm_demangle(h2_list& symbols)
{
   h2_memory::hook(false);
   char nm[256], line[2048], addr[128], type, name[2048];
#   if defined __APPLE__
   sprintf(nm, "nm -f bsd --demangle -U -n %s", O.path);
#   else
   sprintf(nm, "nm -f bsd --demangle --defined-only -n %s", O.path);
#   endif
   FILE* f = ::popen(nm, "r");
   if (f) {
      while (::fgets(line, sizeof(line) - 1, f)) {
         if (3 != sscanf(line, "%s %c %[^\n]", addr, &type, name)) continue;
         if (strchr("bBcCdDiIuU", type)) continue;
         int underscore = 0;
         if (O.os == 'm' && !strchr(name, '(')) underscore = 1;
         h2_symbol* symbol = new h2_symbol(name + underscore, (unsigned long long)strtoull(addr, nullptr, 16));
         if (symbol) symbols.push_back(symbol->x);
      }
      ::pclose(f);
   }
   h2_memory::hook();
}
#endif

static inline bool strncmp_reverse(const char* a, const char* ae, const char* b, const char* be, size_t n)  // [a, ae) [b, be)
{
   if (ae < a + n || be < b + n) return false;
   return !strncmp(ae - n, be - n, n);
}

h2_inline int h2_nm::get_by_name(const char* name, h2_symbol* res[], int n)
{
   if (!name) return 0;
   size_t len = strlen(name);
   if (len == 0) return 0;
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(GetCurrentProcess(), name, symbol)) return 0;
   static h2_symbol s_symbol("", 0);
   s_symbol.addr = (unsigned long long)symbol->Address;
   res[0] = &s_symbol;
   return 1;
#else
   if (I().demangle_symbols.empty()) nm_demangle(I().demangle_symbols);

   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      if (!strcmp(p->name, name)) {
         res[0] = p;
         return 1;
      }
   }
   int count = 0;
   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      char* parentheses = strchr(p->name, '(');
      if (!parentheses) continue;
      if (!strncmp_reverse(p->name, parentheses, name, name + len, len)) continue;  // compare function name
      char* func = parentheses - len;
      if (p->name < func && (func[-1] != ':' || func[-2] != ':')) continue;  // strip namespace
      if (count < n) res[count++] = p;
   }
   return count;
#endif
}

h2_inline h2_symbol* h2_nm::get_by_addr(unsigned long long addr)
{
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromAddr(GetCurrentProcess(), (DWORD64)(addr), 0, symbol)) return nullptr;
   static h2_symbol s_symbol("", 0);
   strcpy(s_symbol.name, symbol->Name);
   s_symbol.addr = (unsigned long long)symbol->Address;
   return &s_symbol;
#else
   return nullptr;
#endif
}

h2_inline unsigned long long h2_nm::get_mangle(const char* name)
{
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(GetCurrentProcess(), name, symbol)) return 0;
   return (unsigned long long)symbol->Address;
#else
   if (!name || strlen(name) == 0) return 0;
   if (!I().mangle_symbols) nm_mangle(I().mangle_symbols);

   auto it = I().mangle_symbols->find(name);
   return it != I().mangle_symbols->end() ? it->second : 0ULL;
#endif
}
// source/symbol/h2_load.cpp
static inline long long get_load_text_offset()
{
   h2_symbol* s[16];
   if (h2_nm::get_by_name("main", s, 16) == 0) return 0;
   return (long long)&main - (long long)s[0]->addr;
}

static inline long long get_load_vtable_offset()
{
   char vtable_symbol[256];
   h2_vtable_test t;
   long long absolute_vtable = (long long)*(void***)&t;
   sprintf(vtable_symbol, "_ZTV%s", typeid(h2_vtable_test).name());  // mangled for "vtable for h2::h2_vtable_test"
   long long relative_vtable = (long long)h2_nm::get_mangle(vtable_symbol);
   if (relative_vtable == 0)
      h2_color::prints("yellow", "\nDon't find vtable for h2::h2_vtable_test %s\n", vtable_symbol);
   return absolute_vtable - relative_vtable;
}

h2_inline void* h2_load::vtable_to_ptr(unsigned long long addr)
{
   if (I().vtable_offset == -1) I().vtable_offset = get_load_vtable_offset();
   return (void*)(addr + I().vtable_offset);
}

h2_inline void* h2_load::addr_to_ptr(unsigned long long addr)
{
#if defined _MSC_VER || defined __CYGWIN__
   return (void*)addr;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (void*)(addr + I().text_offset);
#endif
}

h2_inline unsigned long long h2_load::ptr_to_addr(void* ptr)
{
#if defined _MSC_VER || defined __CYGWIN__
   return (unsigned long long)ptr;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (unsigned long long)ptr - I().text_offset;
#endif
}
// source/symbol/h2_backtrace.cpp
static inline char* addr2line(unsigned long long addr)
{
   static char buf[1024];
   char cmd[256], *ret = nullptr;
#if defined __APPLE__
   sprintf(cmd, "atos -o %s 0x%llx", O.path, addr);
#else
   sprintf(cmd, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#endif
#if !defined _MSC_VER
   FILE* f = ::popen(cmd, "r");
   if (f) {
      ret = ::fgets(buf, sizeof(buf), f);
      ::pclose(f);
   }
   if (!ret) return nullptr;
   if (O.os == 'm' ? !memcmp(buf, "0x", 2) : !!strstr(buf, "??:")) return nullptr;
   for (int i = strlen(buf) - 1; 0 <= i && ::isspace(buf[i]); --i) buf[i] = '\0';  //strip tail
#endif
   return buf;
}

static inline bool backtrace_extract(const char* line, char* mangle_name, unsigned long long* displacement = nullptr)
{
   unsigned long long _t;
#if defined __APPLE__
   //MAC: `3   a.out  0x000000010e777f3d _ZN2h24unit6mallocEm + 45
   if (2 == ::sscanf(line, "%*s%*s%*s%s + %llu", mangle_name, displacement ? displacement : &_t)) return true;
#else
   static unsigned long long v1 = 0, v2 = 0, once = 0;
   //Linux: `./a.out(_ZN2h24unit7executeEv+0x131)[0x55aa6bb840ef]
   if (2 == ::sscanf(line, "%*[^(]%*[^_a-zA-Z]%1023[^)+]+0x%llx", mangle_name, displacement ? displacement : &_t)) return (bool)++v2;
   //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   mangle_name[0] = '\0';
   if (1 == ::sscanf(line, "%*[^(]%*[^+]+0x%llx", displacement ? displacement : &_t)) return (bool)++v1;

   if (!v2 && !once++) h2_color::prints("yellow", "\nAdd -rdynamic to linker options\n");
#endif
   return false;
}

h2_inline bool h2_backtrace::operator==(const h2_backtrace& bt) const
{
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (frames[i] != bt.frames[i])
         return false;
   return true;
}

h2_inline h2_backtrace& h2_backtrace::dump(int shift_)
{
   static h2_backtrace s;
   s.shift = shift_;
#if defined _MSC_VER || defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__
   s.count = CaptureStackBackTrace(0, sizeof(s.frames) / sizeof(s.frames[0]), s.frames, NULL);
#else
   h2_memory::hook(false);
   s.count = ::backtrace(s.frames, sizeof(s.frames) / sizeof(s.frames[0]));
   h2_memory::hook();
#endif
   return s;
}

h2_inline bool h2_backtrace::in(void* fps[]) const
{
   bool ret = false;
#if defined _MSC_VER
   for (int i = shift; !ret && i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      if (SymFromAddr(GetCurrentProcess(), (DWORD64)(frames[i]), 0, symbol))
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)symbol->Address == (unsigned long long)fps[j])
               ret = true;
   }
#elif defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__
   for (int i = shift; !ret && i < count; ++i)
      for (int j = 0; !ret && fps[j]; ++j)
         if ((unsigned long long)fps[j] <= (unsigned long long)frames[i] && (unsigned long long)frames[i] < 100 + (unsigned long long)fps[j])
            ret = true;
#else
   h2_memory::hook(false);
   char** symbols = backtrace_symbols(frames, count);
   for (int i = shift; !ret && i < count; ++i) {
      char _t[1024];
      unsigned long long displacement = 0;
      if (backtrace_extract(symbols[i], _t, &displacement))
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)frames[i] - displacement == (unsigned long long)fps[j])
               ret = true;
   }
   free(symbols);
   h2_memory::hook();
#endif
   return ret;
}

h2_inline void h2_backtrace::print(h2_vector<h2_string>& stacks) const
{
#if defined _WIN32  // +MinGW
   for (int i = shift; i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      h2_string frame;
      if (SymFromAddr(GetCurrentProcess(), (DWORD64)(frames[i]), 0, symbol))
         frame.sprintf("%s %p+%lld ", symbol->Name, symbol->Address, (DWORD64)(frames[i]) - (DWORD64)symbol->Address);
      DWORD dwDisplacement;
      IMAGEHLP_LINE64 fileline;
      SymSetOptions(SYMOPT_LOAD_LINES);
      fileline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
      if (SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)(frames[i]), &dwDisplacement, &fileline))
         frame.sprintf("%s:%d", fileline.FileName, fileline.LineNumber);
      stacks.push_back(frame);
      if (!strcmp("main", symbol->Name)) break;
   }
#elif defined __CYGWIN__
   for (int i = shift; i < count; ++i) {
      char* p = nullptr;
      p = addr2line(h2_load::ptr_to_addr(frames[i]));
      if (p) stacks.push_back(p);
   }
#else
   h2_memory::hook(false);
   char** symbols = backtrace_symbols(frames, count);
   h2_memory::hook();
   for (int i = shift; i < count; ++i) {
      char *p = nullptr, mangle_name[1024] = "", demangle_name[1024] = "";
      backtrace_extract(symbols[i], mangle_name);
      if (O.verbose >= 4 || O.os != 'm') p = addr2line(h2_load::ptr_to_addr(frames[i])); /* atos is slow */
      if (!p) p = h2_cxa::demangle(mangle_name, demangle_name);
      if (!p || !strlen(p)) p = symbols[i];
      stacks.push_back(p);
      if (!strcmp("main", mangle_name) || !strcmp("__libc_start_main", mangle_name)) break;
   }
   h2_memory::hook(false);
   free(symbols);
   h2_memory::hook();
#endif
}

h2_inline void h2_backtrace::print(size_t pad) const
{
   h2_vector<h2_string> stacks;
   print(stacks);
   h2_lines lines;
   for (auto& c : stacks) lines.push_back(c.startswith("h2::") || c.contains(": h2::") ? gray(c) : h2_line(c));
   lines.sequence(pad);
   h2_color::printl(lines);
}
// source/symbol/h2_cxa.cpp
h2_inline char* h2_cxa::demangle(const char* mangle_name, char* demangle_name, size_t length)
{
#if defined _MSC_VER
   return (char*)mangle_name;
#else
   int status = -1;
   if (strlen(mangle_name)) abi::__cxa_demangle(mangle_name, demangle_name, &length, &status);
   return status == 0 ? demangle_name : (char*)mangle_name;
#endif
}

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64

static inline unsigned char* follow_JMP32REL(unsigned char* target)
{
   int relative_offset;
   memcpy(reinterpret_cast<void*>(&relative_offset), reinterpret_cast<void*>(target + 1), 4);
   return target + 5 + relative_offset;
}
static inline unsigned char* follow_JMP8REL(unsigned char* target)
{
   signed char relative_offset;
   memcpy(reinterpret_cast<void*>(&relative_offset), reinterpret_cast<void*>(target + 1), 1);
   return target + 2 + relative_offset;
}
static inline unsigned char* follow_JMP32ABS(unsigned char* target)
{
   void** new_target_p;
   if (sizeof(void*) == 8) {  // In 64-bit mode JMPs are RIP-relative, not absolute
      int target_offset;
      memcpy(reinterpret_cast<void*>(&target_offset), reinterpret_cast<void*>(target + 2), 4);
      new_target_p = reinterpret_cast<void**>(target + target_offset + 6);
   } else {
      memcpy(&new_target_p, reinterpret_cast<void*>(target + 2), 4);
   }
   return reinterpret_cast<unsigned char*>(*new_target_p);
}

h2_inline void* h2_cxa::follow_jmp(void* fp, int n)
{
   unsigned char* p = (unsigned char*)fp;
   while (n--) {
      if (p[0] == 0xE9) {  // ASM_JMP32REL
         p = follow_JMP32REL(p);
      } else if (p[0] == 0xEB) {  //ASM_JMP8REL, Visual Studio 7.1 implements new[] as an 8 bit jump to new
         p = follow_JMP8REL(p);
      } else if (p[0] == 0xFF && p[1] == 0x25) {  // ASM_JMP32ABS_0 ASM_JMP32ABS_1
         p = follow_JMP32ABS(p);
      } else if (sizeof(void*) == 8 && p[0] == 0x48 && p[1] == 0xFF && p[2] == 0x25) {  // in Visual Studio 2012 we're seeing jump like that: rex.W jmpq *0x11d019(%rip)
         p = follow_JMP32ABS(p + 1);
      } else {
         break;
      }
   }
   return (void*)p;
}

#elif defined __arm64__ || defined __aarch64__

static inline long long sign_extend(unsigned long long value, unsigned int bits)
{
   const unsigned int left = 64 - bits;
   const long long m1 = -1;
   const long long wide = (long long)(value << left);
   const long long sign = (wide < 0) ? (m1 << left) : 0;
   return value | sign;
}

static inline unsigned long fetch_opcode(void* fp, int i = 0)
{
   return *(unsigned long*)(((unsigned char*)fp) + i * 4);
}

h2_inline void* h2_cxa::follow_jmp(void* fp, int n)
{
   while (n--) {
      // (gdb) disassemble /r printf
      // Dump of assembler code for function printf@plt:
      //    0x0000000000c1f8a0 <+0>:     90 66 00 b0     adrp    x16, 0x18f0000 <div@got.plt>
      //    0x0000000000c1f8a4 <+4>:     11 ee 41 f9     ldr     x17, [x16, #984]
      //    0x0000000000c1f8a8 <+8>:     10 62 0f 91     add     x16, x16, #0x3d8
      //    0x0000000000c1f8ac <+12>:    20 02 1f d6     br      x17
      if ((fetch_opcode(fp, 0) & 0x9f00001f) == (0x90000000 | 16)) {                                                     // C6.2.10 ADRP
         if ((fetch_opcode(fp, 1) & 0xffe003ff) == (0xf9400000 | 16 << 5 | 17)) {                                        // C6.2.101 LDR (immediate)
            if ((fetch_opcode(fp, 2) & 0xfffffc1f) == 0xd61f0000 || (fetch_opcode(fp, 3) & 0xfffffc1f) == 0xd61f0000) {  // BR X17 0xd61f0000 | 17 << 5
               unsigned long long low2 = (fetch_opcode(fp, 0) >> 29) & 3;
               unsigned long long high19 = (fetch_opcode(fp, 0) >> 5) & ~(~0ULL << 19);
               long long page = sign_extend((high19 << 2) | low2, 21) << 12;
               unsigned long long offset = ((fetch_opcode(fp, 1) >> 10) & ~(~0ULL << 12)) << 3;
               unsigned char* target_p = (unsigned char*)((unsigned long long)fp & 0xfffffffffffff000ULL) + page + offset;
               fp = (void*)*(unsigned char**)target_p;
            }
         }
      } else if ((fetch_opcode(fp, 0) & 0xf8000000) == 0x58000000) {  // LDR X17,[PC+n]
         if ((fetch_opcode(fp, 1) & 0xfffffc1f) == 0xd61f0000) {      // BR X17
            unsigned long n = (fetch_opcode(fp, 0) & 0x07ffffe0) >> 3;
            fp = (void*)*(unsigned long long*)(((unsigned long long)fp) + n);
         }
      }
   }
   return fp;
}

#endif
// source/json/h2_tinyexpr.cpp
// TINYEXPR - Tiny recursive descent parser and evaluation engine in C; Copyright (c) 2015-2018 Lewis Van Winkle
/* clang-format off */
struct tinyexpr
{
    static int TYPE_MASK(int type) { return type & 0x0000001F; }
    static bool IS_PURE(int type) { return (type & TE_FLAG_PURE) != 0; }
    static bool IS_FUNCTION(int type) { return (type & TE_FUNCTION0) != 0; }
    static bool IS_CLOSURE(int type) { return (type & TE_CLOSURE0) != 0; }
    static int ARITY(int type) { return type & (TE_FUNCTION0 | TE_CLOSURE0) ? (type & 0x00000007) : 0; }

    /* Parses the input expression, evaluates it, and frees it. */
    /* Returns NaN on error. */
    static double te_interp(const char *expression, int *error) {
        te_expr *n = te_compile(expression, 0, 0, error);
        double ret;
        if (n) {
            ret = te_eval(n);
            te_free(n);
        } else {
            ret = (NAN);
        }
        return ret;
    }

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

    enum { TE_CONSTANT = 1 };

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
        te_expr *ret = (te_expr *)h2_libc::malloc(size);
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
        h2_libc::free((void *)n);
    }

    static double _fabs(double x) {return ::fabs(x);}
    static double _cos(double x) {return ::cos(x);}
    static double _acos(double x) {return ::acos(x);}
    static double _cosh(double x) {return ::cosh(x);}
    static double _sin(double x) {return ::sin(x);}
    static double _asin(double x) {return ::asin(x);}
    static double _sinh(double x) {return ::sinh(x);}
    static double _tan(double x) {return ::tan(x);}
    static double _atan(double x) {return ::atan(x);}
    static double _atan2(double x, double y) {return ::atan2(x, y);}
    static double _tanh(double x) {return ::tanh(x);}
    static double _sqrt(double x) {return ::sqrt(x);}
    static double _ln(double x) {return ::log(x);}
    static double _log(double x) {return ::log(x);}
    static double _log10(double x) {return ::log10(x);}
    static double _log2(double x) {return ::log2(x);}
    static double _floor(double x) {return ::floor(x);}
    static double _ceil(double x) {return ::ceil(x);}
    static double _pow(double x, double y) {return ::pow(x, y);}
    static double _exp(double x) {return ::exp(x);}
    static double _fmod(double x, double y) {return ::fmod(x, y);}

    static double pi(void) {return 3.14159265358979323846;}
    static double e(void) {return 2.71828182845904523536;}
    static double fac(double a) {/* simplest version of fac */
        if (a < 0.0) return (NAN);
        if (a > UINT_MAX) return (NAN);
        unsigned int ua = (unsigned int)(a);
        unsigned long int result = 1, i;
        for (i = 1; i <= ua; i++) {
            if (i > ULONG_MAX / result) return (NAN);
            result *= i;
        }
        return (double)result;
    }
    static double ncr(double n, double r) {
        if (n < 0.0 || r < 0.0 || n < r) return (NAN);
        if (n > UINT_MAX || r > UINT_MAX) return (NAN);
        unsigned long int un = (unsigned int)(n), ur = (unsigned int)(r), i;
        unsigned long int result = 1;
        if (ur > un / 2) ur = un - ur;
        for (i = 1; i <= ur; i++) {
            if (result > ULONG_MAX / (un - ur + i)) return (NAN);
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
            {"atan2", (const void *)_atan2,  TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"ceil", (const void *)_ceil,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"cos", (const void *)_cos,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"cosh", (const void *)_cosh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"e", (const void *)e,          TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"exp", (const void *)_exp,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"fac", (const void *)fac,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"floor", (const void *)_floor,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ln", (const void *)_ln,       TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log", (const void *)_log,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log10", (const void *)_log10,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log2", (const void *)_log2,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ncr", (const void *)ncr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"npr", (const void *)npr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"pi", (const void *)pi,        TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"pow", (const void *)_pow,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"sin", (const void *)_sin,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"sinh", (const void *)_sinh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"sqrt", (const void *)_sqrt,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"tan", (const void *)_tan,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"tanh", (const void *)_tanh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
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

                    const te_variable *var = find_lookup(s, start, (int)(s->next - start));
                    if (!var) var = find_builtin(start, (int)(s->next - start));

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
                ret->value = (NAN);
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

#define TE_F(...) ((double(*)(__VA_ARGS__))n->function)
#define TE_M(e) te_eval((const te_expr *)n->parameters[e])

    static double te_eval(const te_expr *n) {
        if (!n) return (NAN);

        switch(TYPE_MASK(n->type)) {
            case TE_CONSTANT: return n->value;
            case TE_VARIABLE: return *n->bound;

            case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
            case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
                switch(ARITY(n->type)) {
                    case 0: return TE_F(void)();
                    case 1: return TE_F(double)(TE_M(0));
                    case 2: return TE_F(double, double)(TE_M(0), TE_M(1));
                    case 3: return TE_F(double, double, double)(TE_M(0), TE_M(1), TE_M(2));
                    case 4: return TE_F(double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3));
                    case 5: return TE_F(double, double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4));
                    case 6: return TE_F(double, double, double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5));
                    case 7: return TE_F(double, double, double, double, double, double, double)(TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5), TE_M(6));
                    default: return (NAN);
                }

            case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
            case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
                switch(ARITY(n->type)) {
                    case 0: return TE_F(void*)(n->parameters[0]);
                    case 1: return TE_F(void*, double)(n->parameters[1], TE_M(0));
                    case 2: return TE_F(void*, double, double)(n->parameters[2], TE_M(0), TE_M(1));
                    case 3: return TE_F(void*, double, double, double)(n->parameters[3], TE_M(0), TE_M(1), TE_M(2));
                    case 4: return TE_F(void*, double, double, double, double)(n->parameters[4], TE_M(0), TE_M(1), TE_M(2), TE_M(3));
                    case 5: return TE_F(void*, double, double, double, double, double)(n->parameters[5], TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4));
                    case 6: return TE_F(void*, double, double, double, double, double, double)(n->parameters[6], TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5));
                    case 7: return TE_F(void*, double, double, double, double, double, double, double)(n->parameters[7], TE_M(0), TE_M(1), TE_M(2), TE_M(3), TE_M(4), TE_M(5), TE_M(6));
                    default: return (NAN);
                }

            default: return (NAN);
        }

    }

#undef TE_F
#undef TE_M

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
                *error = (int)(s.next - s.start);
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
// source/json/h2_node.cpp
struct h2_json_node : h2_libc {
   static constexpr int t_absent = 0;
   static constexpr int t_null = 1;
   static constexpr int t_boolean = 2;
   static constexpr int t_number = 3;
   static constexpr int t_string = 4;
   static constexpr int t_pattern = 5;  // regex or wildcard pattern
   static constexpr int t_array = 6;
   static constexpr int t_object = 7;

   int type = t_absent;
   int index = 0;
   h2_string key_string;
   h2_string value_string;
   double value_double = 0;
   bool value_boolean = false;
   h2_list children, x; /* array or object */

   h2_json_node(int index_ = 0) : index(index_) {}
   ~h2_json_node()
   {
      h2_list_for_each_entry (p, children, h2_json_node, x) {
         p->x.out();
         delete p;
      }
   }

   int size() const
   {
      return children.count();
   }

   h2_json_node* get(int index) const
   {
      if (index < 0) index = children.count() + index;
      h2_list_for_each_entry (p, i, children, h2_json_node, x)
         if (i == index)
            return p;
      return nullptr;
   }

   h2_json_node* get(const h2_string& name, bool caseless) const
   {
      h2_list_for_each_entry (p, children, h2_json_node, x)
         if (p->key_string.equals(name, caseless))
            return p;
      return nullptr;
   }

   bool is_null() const { return t_null == type; }
   bool is_bool() const { return t_boolean == type; }
   bool is_number() const { return t_number == type; }
   bool is_string() const { return t_string == type; }
   bool is_pattern() const { return t_pattern == type; }
   bool is_array() const { return t_array == type; }
   bool is_object() const { return t_object == type; }

   h2_string quote_if(int quote) const
   {
      switch (quote) {
         case 1: return "'";
         case 2: return "\"";
         case 3: return "\\\"";
         default: return "";
      }
   }

   h2_string format_value(int quote) const
   {
      switch (type) {
         case t_null: return "null";
         case t_boolean: return value_boolean ? "true" : "false";
         case t_number: return (value_double - ::floor(value_double) == 0) ? std::to_string((long long)value_double).c_str() : std::to_string(value_double).c_str();
         case t_string: return quote_if(quote) + value_string + quote_if(quote);
         case t_pattern: return "/" + value_string + "/";
         case t_array:
         case t_object:
         default: return "";
      }
   }

   void format(int& _type, h2_string& _key, h2_string& _value, int quote = 0) const
   {
      _type = type;
      if (key_string.size()) _key = quote_if(quote) + key_string + quote_if(quote);
      _value = format_value(quote);
   }

   h2_lines format(bool fold, int quote = 0, size_t depth = 0, int next = 0) const
   {
      h2_lines lines;
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back(quote_if(quote) + key_string + quote_if(quote) + ": ");
      if (is_array() || is_object()) {
         h2_lines children_lines;
         h2_list_for_each_entry (p, i, children, h2_json_node, x)
            children_lines += p->format(fold, quote, depth + 1, children.count() - i - 1);
         line.push_back(is_array() ? "[" : "{");
         if (fold && children_lines.foldable()) {
            line += children_lines.folds();
         } else {
            lines.push_back(line), line.clear();
            lines += children_lines;
            line.indent(depth * 2);
         }
         line.push_back(is_array() ? "]" : "}");
      } else {
         line.push_back(format_value(quote));
      }
      if (line.size()) {
         if (next) line.push_back(", ");
         lines.push_back(line), line.clear();
      }
      return lines;
   }
};
// source/json/h2_lexical.cpp
struct h2_json_lexical {
   static void new_lexis(h2_vector<h2_string>& lexical, const char* start, const int size)
   {
      const char *left = start, *right = start + size;
      for (; left < right && *left && ::isspace(*left);) left++;
      for (; left < right - 1 && ::isspace(*(right - 1));) right--;
      lexical.push_back(h2_string(right - left, left));
   }

   static void parse(h2_vector<h2_string>& lexical, const char* json_string, int json_length = -1)
   {
      enum {
         st_idle,
         st_escape,
         st_single_quote,
         st_double_quote,
         st_pattern,
         st_normal
      };

      const char* pending = nullptr;
      const char* p;
      int state = st_idle, stash_state = st_idle;
      for (p = json_string; *p && json_length--; p++) {
         switch (state) {
            case st_idle:
               if (::isspace(*p)) {
                  continue;
               } else if (strchr("{:}[,]", *p)) {
                  new_lexis(lexical, p, 1);
               } else {
                  pending = p;
                  state = st_normal;
                  if ('\"' == *p) {
                     state = st_double_quote;
                  } else if ('\'' == *p) {
                     state = st_single_quote;
                  } else if ('/' == *p) {
                     state = st_pattern;
                  } else if ('\\' == *p) {
                     stash_state = state, state = st_escape;
                  }
               }
               break;
            case st_escape:
               state = stash_state;
               break;
            case st_single_quote:
               if ('\'' == *p) {
                  new_lexis(lexical, pending, (int)((p + 1) - pending));
                  pending = nullptr;
                  state = st_idle;
               } else if ('\\' == *p) {
                  stash_state = state, state = st_escape;
               }
               break;
            case st_double_quote:
               if ('\"' == *p) {
                  new_lexis(lexical, pending, (int)((p + 1) - pending));
                  pending = nullptr;
                  state = st_idle;
               } else if ('\\' == *p) {
                  stash_state = state, state = st_escape;
               }
               break;
            case st_pattern:
               if ('/' == *p) {
                  new_lexis(lexical, pending, (int)((p + 1) - pending));
                  pending = nullptr;
                  state = st_idle;
               }
               /* no escape char */
               break;
            case st_normal:
               if (strchr("{:}[,]", *p)) {
                  new_lexis(lexical, pending, (int)(p - pending));
                  pending = nullptr;
                  new_lexis(lexical, p, 1);
                  state = st_idle;
               } else if ('\\' == *p) {
                  stash_state = state, state = st_escape;
               }
               break;
         }
      }
      if (pending) {
         new_lexis(lexical, pending, (int)(p - pending));
      }
   }
};
// source/json/h2_syntax.cpp
struct h2_json_syntax {
   size_t i = 0;
   const h2_vector<h2_string>& lexical;
   h2_json_syntax(const h2_vector<h2_string>& _lexical) : lexical(_lexical) {}

   bool parse(h2_json_node& root_node)
   {
      if (!parse_value(root_node)) return false;
      return lexical.size() <= i;  // nothing more, "{},{}"
   }

   h2_string& filter_string(h2_string& s) const
   {
      if (s.enclosed('\"'))
         s = s.unquote('\"');
      else if (s.enclosed('\''))
         s = s.unquote('\'');
      s = s.unescape();
      return s;
   }

   bool interpret_number(const h2_string& s, double& value) const
   {
      int err = 0;
      value = tinyexpr::te_interp(s.c_str(), &err);
      return 0 == err;
   }

   bool desire(const char* s)
   {
      if (lexical.size() <= i || !lexical[i].equals(s)) return false;
      ++i;
      return true;
   }

   bool parse_value(h2_json_node& node)
   {
      if (lexical.size() <= i) return true;

      if (lexical[i].equals("null")) {
         ++i;
         node.type = h2_json_node::t_null;
         return true;
      }
      if (lexical[i].equals("false")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = false;
         return true;
      }
      if (lexical[i].equals("true")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = true;
         return true;
      }
      if (lexical[i].equals("[")) return parse_array(node);
      if (lexical[i].equals("{")) return parse_object(node);
      if (lexical[i].startswith("/")) return parse_pattern(node);

      if (lexical[i].equals(":")) return false;
      if (lexical[i].equals(",")) return false;

      return parse_string_or_number(node);
   }

   bool parse_key(h2_json_node& node)
   {
      node.key_string = lexical[i++];
      filter_string(node.key_string);
      return true;
   }

   bool parse_pattern(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (node.value_string.enclosed('/'))
         node.value_string = node.value_string.unquote('/');
      node.type = h2_json_node::t_pattern;
      return true;
   }

   bool parse_string_or_number(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (interpret_number(node.value_string, node.value_double)) {
         node.type = h2_json_node::t_number;
         node.value_string = "";
         return true;
      }
      node.value_double = 0;
      filter_string(node.value_string);
      node.type = h2_json_node::t_string;
      return true;
   }

   bool parse_array(h2_json_node& node)
   {
      if (!desire("[")) return false;
      int n = 0;
      while (i < lexical.size() && !lexical[i].equals("]")) {
         h2_json_node* new_node = new h2_json_node(n++);
         node.children.push_back(new_node->x);
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            i++;
         else
            break;
      }

      if (!desire("]")) return false;
      node.type = h2_json_node::t_array;
      return true;
   }

   bool parse_object(h2_json_node& node)
   {
      if (!desire("{")) return false;
      int n = 0;
      while (i < lexical.size() && !lexical[i].equals("}")) {
         h2_json_node* new_node = new h2_json_node(n++);
         node.children.push_back(new_node->x);
         if (!parse_key(*new_node)) return false;
         if (!desire(":")) return false;
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            ++i;
         else
            break;
      }

      if (!desire("}")) return false;
      node.type = h2_json_node::t_object;
      return true;
   }
};
// source/json/h2_select.cpp
struct h2_json_select {
   struct value {
      int index;
      h2_string key;
   };

   h2_vector<value> values;

   h2_json_select(const char* selector)
   {
      const int st_idle = 0;
      const int st_in_dot = 1;
      const int st_in_bracket = 2;
      int state = 0;
      const char *s = nullptr, *p = selector;
      do {
         switch (state) {
            case st_idle:
               if (*p == '.') {
                  state = st_in_dot;
                  s = p + 1;
               } else if (*p == '[') {
                  state = st_in_bracket;
                  s = p + 1;
               }
               break;
            case st_in_dot:
               if (*p == '.') {  // end a part
                  if (s < p) add(s, p - 1, true);
                  // restart a new part
                  state = st_in_dot;
                  s = p + 1;
               } else if (*p == '[') {  // end a part
                  if (s < p) add(s, p - 1, true);
                  // restart a new part
                  state = st_in_bracket;
                  s = p + 1;
               } else if (*p == '\0') {
                  if (s < p) add(s, p - 1, true);
                  state = st_idle;
               }
               break;
            case st_in_bracket:
               if (*p == ']') {
                  if (s < p) add(s, p - 1, false);
                  state = st_idle;
               }
               break;
         }
      } while (*p++);
   }

   void add(const char* start, const char* end, bool only_key)
   {
      for (; start <= end && ::isspace(*start);) start++;  //strip left space
      for (; start <= end && ::isspace(*end);) end--;      //strip right space
      if (start <= end) {
         if (!only_key) {
            if (strspn(start, "-0123456789") == (size_t)(end - start + 1)) {
               values.push_back({atoi(start), ""});
               return;
            } else if ((*start == '\"' && *end == '\"') || (*start == '\'' && *end == '\'')) {
               start++, end--;
            }
         }
         if (start <= end) values.push_back({0, h2_string(end - start + 1, start)});
      }
   }
};
// source/json/h2_tree.cpp
struct h2_json_tree : h2_json_node {
   h2_vector<h2_string> lexical;
   h2_json_syntax syntax{lexical};
   bool illformed;

   h2_json_tree(const char* json_string, int json_length = -1)
   {
      h2_json_lexical::parse(lexical, json_string, json_length);
      illformed = !syntax.parse(*this);
   }

   h2_json_node* select(const char* selector, bool caseless)
   {
      h2_json_select select(selector);
      h2_json_node* node = this;
      for (auto& c : select.values)
         node = c.key.size() ? node->get(c.key, caseless) : node->get(c.index);
      if (node) node->key_string = "";
      return node;
   }

   h2_line serialize() const
   {
      h2_line line;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            line.printf("yellow,bold,underline", "%s%s ", comma_if(j, " "), lexical[j].c_str());
         else
            line.push_back(comma_if(j, " ") + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) {
         line.printf("yellow,bold,underline", " ... ");
      }
      return line;
   }
};
// source/json/h2_match.cpp
struct h2_json_match {
   static bool match_array(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      if (e->size() != a->size()) return false;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         if (!match(p, a->get(i), caseless))
            return false;
      return true;
   }

   static bool match_object(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      if (e->size() > a->size()) return false;
      h2_list_for_each_entry (p, e->children, h2_json_node, x)
         if (!match(p, a->get(p->key_string, caseless), caseless))
            return false;
      return true;
   }

   static bool match(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      switch (e->type) {
         case h2_json_node::t_null:
            return a->is_null();
         case h2_json_node::t_boolean:
            return a->is_bool() && e->value_boolean == a->value_boolean;
         case h2_json_node::t_number:
            return a->is_number() && ::fabs(e->value_double - a->value_double) < 0.00001;
         case h2_json_node::t_string:
            return a->is_string() && e->value_string.equals(a->value_string, caseless);
         case h2_json_node::t_pattern:
            return a->is_string() && h2_pattern::match(e->value_string.c_str(), a->value_string.c_str(), caseless);
         case h2_json_node::t_array:
            return a->is_array() && match_array(e, a, caseless);
         case h2_json_node::t_object:
            return a->is_object() && match_object(e, a, caseless);
         default: return false;
      }
   }

   static double array_similarity(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      double score = 0.0, sub_score = 0.0;
      if (e->size() == a->size()) score += 0.3;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         sub_score += (sub_score * i + similarity(p, a->get(i), caseless)) / (i + 1);
      return score + 0.7 * sub_score;
   }

   static double object_similarity(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      double score = 0.0, sub_score = 0.0;
      if (e->size() == a->size()) score += 0.3;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         sub_score += (sub_score * i + similarity(p, a->get(p->key_string, false), caseless)) / (i + 1);
      return score + 0.7 * sub_score;
   }

   static double similarity(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return 0;
      double score = 0.0;
      if (e->type == h2_json_node::t_array && a->type == h2_json_node::t_array) {
         score += array_similarity(e, a, caseless);
      } else if (e->type == h2_json_node::t_object && a->type == h2_json_node::t_object) {
         score += object_similarity(e, a, caseless);
      } else if (e->type == a->type) {
         score += h2_fuzzy::similarity(e->value_string.c_str(), a->value_string.c_str(), caseless);
      } else {
      }
      if (e->key_string.size() || a->key_string.size()) {
         score = score * 0.5 + 0.5 * h2_fuzzy::similarity(e->key_string.c_str(), a->key_string.c_str(), caseless);
      }
      return score;
   }

   static h2_json_node* search(const h2_list& haystack, h2_json_node* needle, bool caseless)
   {
      double max_score = 0.2;
      h2_json_node* straw = nullptr;
      h2_list_for_each_entry (p, haystack, h2_json_node, x) {
         double score = similarity(needle, p, caseless);
         if (score > max_score) {
            max_score = score;
            straw = p;
         }
      }
      return straw;
   }
};
// source/json/h2_dual.cpp
struct h2_json_dual : h2_libc {  // Combine two node into a dual
   size_t depth;
   int relationship, index = INT_MAX;
   bool key_equal = false, value_match = false;
   bool e_last = true, a_last = true;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_list children, x;

   static int index_cmp(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_json_dual, x)->index - h2_list_entry(b, h2_json_dual, x)->index;
   }

   ~h2_json_dual()
   {
      h2_list_for_each_entry (p, children, h2_json_dual, x) {
         p->x.out();
         delete p;
      }
   }

   void __move_dual(h2_json_node* child_e, h2_json_node* child_a, bool caseless)
   {
      children.push_back((new h2_json_dual(child_e, child_a, caseless, depth + 1))->x);
      child_e->x.out();
      delete child_e;
      child_a->x.out();
      delete child_a;
   }

   void __mark_last()
   {
      int e_count = 0, a_count = 0;
      h2_list_for_each_reverse_entry (p, children, h2_json_dual, x) {
         if (p->e_type != h2_json_node::t_absent) p->e_last = e_count++ == 0;
         if (p->a_type != h2_json_node::t_absent) p->a_last = a_count++ == 0;
      }
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, bool caseless, size_t depth_ = 0, int relationship_ = 0) : depth(depth_), relationship(relationship_)
   {
      if (e) index = e->index;
      if (e) e->format(e_type, e_key, e_value, 2);
      if (a) a->format(a_type, a_key, a_value, 2);
      key_equal = e_key.equals(a_key, caseless);
      value_match = h2_json_match::match(e, a, caseless);

      if (e_type == h2_json_node::t_object || a_type == h2_json_node::t_object) {
         if (e && a) {
            h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
               h2_json_node* child_a = a->get(child_e->key_string, false);
               if (!child_a && caseless) child_a = a->get(child_e->key_string, true);
               if (child_a) __move_dual(child_e, child_a, caseless);
            }
            h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
               h2_json_node* child_a = h2_json_match::search(a->children, child_e, caseless);
               if (child_a) __move_dual(child_e, child_a, caseless);
            }
         }
         for (int i = 0; i < std::max(e ? e->size() : 0, a ? a->size() : 0); ++i) {
            children.push_back((new h2_json_dual(e ? e->get(i) : nullptr, a ? a->get(i) : nullptr, caseless, depth + 1, -1))->x);
         }
         children.sort(index_cmp);
      } else if (e_type == h2_json_node::t_array || a_type == h2_json_node::t_array) {
         for (int i = 0; i < std::max(e ? e->size() : 0, a ? a->size() : 0); ++i)
            children.push_back((new h2_json_dual(e ? e->get(i) : nullptr, a ? a->get(i) : nullptr, caseless, depth + 1))->x);
      }
      __mark_last();
   }

   const char* __e_style() const { return a_type == h2_json_node::t_absent || relationship < 0 ? "light blue" : "green"; }
   const char* __a_style() const { return e_type == h2_json_node::t_absent || relationship < 0 ? "light purple" : "red,bold"; }

   void align(h2_lines& e_lines, h2_lines& a_lines) const
   {
      h2_lines e_ls, a_ls;
      h2_line e_line, a_line;
      e_line.indent(depth * 2);
      a_line.indent(depth * 2);

      if (e_type != h2_json_node::t_absent && e_key.size())
         e_line += color(e_key, key_equal ? "" : __e_style()) + ": ";
      if (a_type != h2_json_node::t_absent && a_key.size())
         a_line += color(a_key, key_equal ? "" : __a_style()) + ": ";

      if (e_type != h2_json_node::t_absent && e_value.size())
         e_line += color(e_value, value_match ? "" : __e_style());
      if (a_type != h2_json_node::t_absent && a_value.size())
         a_line += color(a_value, value_match ? "" : __a_style());

      if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) e_line.push_back(e_type == h2_json_node::t_object ? "{" : "[");
      if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) a_line.push_back(a_type == h2_json_node::t_object ? "{" : "[");

      h2_lines e_children_lines, a_children_lines;
      h2_list_for_each_entry (p, children, h2_json_dual, x)
         p->align(e_children_lines, a_children_lines);

      if ((O.fold_json >= 2 && key_equal && value_match) || (O.fold_json >= 3 && relationship < 0)) {
         e_line += e_children_lines.foldable() ? e_children_lines.folds() : gray(" ... ");
         a_line += a_children_lines.foldable() ? a_children_lines.folds() : gray(" ... ");
      } else if (O.fold_json && e_children_lines.foldable() && a_children_lines.foldable()) {
         e_line += e_children_lines.folds();
         a_line += a_children_lines.folds();
      } else {
         if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) {
            e_ls.push_back(e_line), e_line.clear();
            e_ls += e_children_lines;
            e_line.indent(depth * 2);
         }
         if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) {
            a_ls.push_back(a_line), a_line.clear();
            a_ls += a_children_lines;
            a_line.indent(depth * 2);
         }
      }
      if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) e_line.push_back(e_type == h2_json_node::t_object ? "}" : "]");
      if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) a_line.push_back(a_type == h2_json_node::t_object ? "}" : "]");

      if (e_type != h2_json_node::t_absent && !e_last) e_line.push_back(", ");
      if (a_type != h2_json_node::t_absent && !a_last) a_line.push_back(", ");
      e_ls.push_back(e_line);
      a_ls.push_back(a_line);
      h2_lines::samesizify(e_ls, a_ls);
      e_lines += e_ls;
      a_lines += a_ls;
   }
};
// source/json/h2_json.cpp
h2_inline h2_lines h2_json::dump(const h2_string& json_string)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return {tree.serialize()};
   h2_lines lines = tree.format(O.fold_json, O.copy_paste_json);
   if (O.copy_paste_json) {
      if (!lines.empty()) {
         lines.front() = "\"" + lines.front();
         lines.back() = lines.back() + "\"";
      }
      size_t max_width = lines.width();
      for (size_t i = 0; i < lines.size(); ++i) {
         lines[i].padding(max_width - lines[i].width() + 3);
         if (i < lines.size() - 1) lines[i].push_back("\\");
      }
   }
   return lines;
}

h2_inline h2_string h2_json::select(const h2_string& json_string, const h2_string& selector, bool caseless)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return json_string;
   h2_json_node* node = tree.select(selector.c_str(), caseless);
   if (!node) return "";
   return node->format(O.fold_json, 2).string();
}

h2_inline int h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return -1;
   return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return false;
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_lines, a_lines);
   return true;
}
// source/matcher/h2_strcmp.cpp
h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, int n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(h2_pattern::regex_match(e.c_str(), _a.c_str(), c.caseless))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_regex::expection(h2_mc c) const
{
   return CD("Re" + gray("(") + h2_stringify(e) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, int n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(h2_pattern::wildcard_match(e.c_str(), _a.c_str(), c.caseless))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_wildcard::expection(h2_mc c) const
{
   return CD("We" + gray("(") + h2_stringify(e) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, int n, h2_mc c) const
{
   h2_string _e = e, _a = a;
   if (c.spaceless) _e = e.squash(), _a = a.squash();
   if (c.fit(_a.equals(_e, c.caseless))) return nullptr;
   return h2_fail::new_strfind(_e, a, expection(c));
}
h2_inline h2_line h2_matches_strcmp::expection(h2_mc c) const
{
   return CD(h2_representify(c.spaceless ? e.squash() : e), c, "≠");
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, int n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(_a.contains(substring, c.caseless))) return nullptr;
   return h2_fail::new_strfind(substring, a, expection(c));
}
h2_inline h2_line h2_matches_substr::expection(h2_mc c) const
{
   return CD("Substr" + gray("(") + h2_representify(substring) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, int n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(_a.startswith(prefix_string, c.caseless))) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expection(c));
}
h2_inline h2_line h2_matches_startswith::expection(h2_mc c) const
{
   return CD("StartsWith" + gray("(") + h2_representify(prefix_string) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, int n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(_a.endswith(suffix_string, c.caseless))) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expection(c));
}
h2_inline h2_line h2_matches_endswith::expection(h2_mc c) const
{
   return CD("EndsWith" + gray("(") + h2_representify(suffix_string) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, int, h2_mc c) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, c.caseless);
   int ret = h2_json::match(e, _a, c.caseless);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(c), c.caseless, "illformed json");
   if (c.fit(ret == 0)) return nullptr;
   return h2_fail::new_json(e, _a, expection(c), c.caseless, DS(c.dont));
}
h2_inline h2_line h2_matches_json::expection(h2_mc c) const
{
   return CD(h2_stringify(e), c, "≠");
}
// source/memory/h2_piece.cpp
struct h2_piece : h2_libc {
   h2_list x;
   unsigned char *user_ptr, *page_ptr;
   size_t user_size, page_size, page_count;
   // free
   const char* who_allocate;
   h2_backtrace bt_allocate, bt_release;
   int free_times = 0;
   // snowfield
   unsigned char snow_flower;
   // forbidden
   static constexpr const unsigned readable = 1, writable = 1 << 1;
   void* forbidden_page{nullptr};
   size_t forbidden_size = 0;
   int violate_times = 0;
   void* violate_ptr{nullptr};
   const char* violate_action = "";
   bool violate_after_free = false;
   h2_backtrace violate_backtrace;

   h2_piece(size_t size, size_t alignment, const char* who, h2_backtrace& bt) : user_size(size), page_size(h2_page_size()), who_allocate(who), bt_allocate(bt)
   {
      size_t alignment_2n = alignment;
      if (not2n((unsigned)alignment)) alignment_2n = (size_t)mask2n((unsigned)alignment) + 1;
      if (alignment_2n < sizeof(void*)) alignment_2n = sizeof(void*);

      size_t user_size_plus = (user_size + alignment_2n - 1 + alignment_2n) & ~(alignment_2n - 1);
      page_count = (size_t)::ceil(user_size_plus / (double)page_size);

#if defined _WIN32
      page_ptr = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      if (page_ptr == NULL) ::printf("VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      page_ptr = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (page_ptr == MAP_FAILED) ::printf("mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      user_ptr = page_ptr + page_size * page_count - user_size_plus + alignment;

      mark_snowfield();
   }

   ~h2_piece()
   {
#if defined _WIN32
      VirtualFree(page_ptr, 0, MEM_DECOMMIT | MEM_RELEASE);
#else
      ::munmap(page_ptr, page_size * (page_count + 1));
#endif
   }

   void set_forbidden(unsigned permission, void* page = 0, size_t size = 0)
   {
      if (page) forbidden_page = page;
      if (size) forbidden_size = size;

#if defined _WIN32
      DWORD old_permission, new_permission;
      new_permission = PAGE_NOACCESS;
      if (permission & readable)
         new_permission = PAGE_READONLY;
      if (permission & writable)
         new_permission = PAGE_READWRITE;
      if (!VirtualProtect(forbidden_page, forbidden_size, new_permission, &old_permission))
         h2_color::prints("yellow", "VirtualProtect failed %lu\n", GetLastError());
#else
      int new_permission = PROT_NONE;
      if (permission & readable)
         new_permission = PROT_READ;
      if (permission & writable)
         new_permission = PROT_READ | PROT_WRITE;
      if (::mprotect(forbidden_page, forbidden_size, new_permission) != 0)
         h2_color::prints("yellow", "mprotect failed %s\n", strerror(errno));
#endif
   }

#if defined _WIN32
   void violate_forbidden(void* ptr, const char* type)
   {
      if (!violate_times++) { /* 只记录第一犯罪现场 */
         violate_backtrace = h2_backtrace::dump(3);
         violate_ptr = ptr;
         violate_action = type;
         violate_after_free = 0 < free_times;
      }
      set_forbidden(readable | writable);
   }
#else
   void violate_forbidden(void* ptr)
   {
      /* 区分读写犯罪方法(一次或二次进入 segment fault):
         1) 设区域为不可读不可写
         2) 读或写行为触发 segment fault, 并进入handler
         3) 设区域为只可读不可写, 先认为犯罪为读
         4) 重新执行代码, 如果是读行为, 则结束(犯罪已经记录为读)
         5) 写行为再次触发 segment fault, 并再次进入handler
         6) 设区域为可读可写, 修正犯罪为写
         7) 恢复执行代码
       */
      auto bt = h2_backtrace::dump(4);
      if (!violate_times++) { /* 只记录第一犯罪现场 */
         set_forbidden(readable);
         violate_backtrace = bt;
         violate_ptr = ptr;
         violate_action = "read";
         violate_after_free = 0 < free_times;
      } else {
         set_forbidden(readable | writable);
         if (bt == violate_backtrace) /* 是第一犯罪现场 */
            violate_action = "write";
      }
   }
#endif

   void mark_snowfield()
   {
      static unsigned char s_snow_flower = (unsigned char)::rand();
      snow_flower = ++s_snow_flower;
      memset(page_ptr, snow_flower, user_ptr - page_ptr);
      memset(user_ptr + user_size, snow_flower, (page_ptr + page_size * page_count) - (user_ptr + user_size));
      set_forbidden(readable, page_ptr + page_size * page_count, page_size);
   }

   h2_fail* check_snowfield(const unsigned char* start, const unsigned char* end)
   {
      for (const unsigned char* p = start; p < end; ++p) {
         if (*p == snow_flower) continue;
         int n = std::min((int)(end - p), 8);
         for (; 0 < n; --n)
            if (p[n - 1] != snow_flower) break;
         h2_vector<unsigned char> spot(p, p + n);
         return h2_fail::new_overflow(user_ptr, user_size, p, "write", spot, bt_allocate, h2_backtrace());
      }
      return nullptr;
   }

   h2_fail* check_snowfield()
   {
      h2_fail* fail = nullptr;
      fail = check_snowfield(user_ptr + user_size, page_ptr + page_size * page_count);
      if (!fail) fail = check_snowfield(page_ptr, user_ptr);
      return fail;
   }

   h2_fail* violate_fail()
   {
      if (violate_after_free)
         return h2_fail::new_use_after_free(user_ptr, violate_ptr, violate_action, bt_allocate, bt_release, violate_backtrace);
      else
         return h2_fail::new_overflow(user_ptr, user_size, violate_ptr, violate_action, h2_vector<unsigned char>(), bt_allocate, violate_backtrace);
   }

   h2_fail* check_asymmetric_free(const char* who_release)
   {
      static const char* a1[] = {"malloc", "calloc", "realloc", "posix_memalign", "aligned_alloc", nullptr};
      static const char* a2[] = {"free", nullptr};
      static const char* b1[] = {"new", "new nothrow", nullptr};
      static const char* b2[] = {"delete", "delete nothrow", nullptr};
      static const char* c1[] = {"new[]", "new[] nothrow", nullptr};
      static const char* c2[] = {"delete[]", "delete[] nothrow", nullptr};
      static const char** S[] = {a1, a2, b1, b2, c1, c2};
      for (size_t i = 0; i < sizeof(S) / sizeof(S[0]); i += 2)
         if (h2_in(who_allocate, S[i]) && h2_in(who_release, S[i + 1]))
            return nullptr;
      if (bt_allocate.in(h2_exempt::I().fps)) return nullptr;
      return h2_fail::new_asymmetric_free(user_ptr, who_allocate, who_release, bt_allocate, bt_release);
   }

   h2_fail* check_double_free(h2_backtrace& bt)
   {
      if (!free_times++) {
         bt_release = bt;
         return nullptr;
      }
      return h2_fail::new_double_free(user_ptr, bt_allocate, bt_release, bt);
   }

   h2_fail* free(const char* who_release)
   {
      h2_fail* fail = check_double_free(h2_backtrace::dump(4));
      if (!fail) fail = check_asymmetric_free(who_release);
      if (!fail) fail = check_snowfield();
      if (!fail) set_forbidden(0, page_ptr, page_size * (page_count + 1));
      return fail;
   }

   bool in_page_range(const unsigned char* p) const
   {
      return page_ptr <= p && p < page_ptr + page_size * (page_count + 1);
   }
};
// source/memory/h2_leaky.cpp
struct h2_leaky {
   struct leak {
      void* ptr;
      h2_backtrace bt;
      h2_vector<std::pair<size_t, size_t>> sizes;

      leak(void* ptr_, const h2_backtrace& bt_) : ptr(ptr_), bt(bt_) {}

      h2_vector<std::pair<size_t, size_t>>::iterator find(size_t size)
      {
         for (auto it = sizes.begin(); it != sizes.end(); it++)
            if (it->first == size)
               return it;
         return sizes.end();
      }

      void add(size_t size)
      {
         if (sizes.end() == find(size)) sizes.push_back({size, 0});
         find(size)->second++;
      }

      h2_fail* check(const char* where, const h2_fs& fs)
      {
         size_t s = 0;
         for (auto& p : sizes)
            s += p.first * p.second;
         return h2_fail::new_memory_leak(ptr, s, sizes, bt, where, fs);
      }
   };

   h2_vector<leak> leaks;

   h2_vector<leak>::iterator find(const h2_backtrace& bt)
   {
      for (auto it = leaks.begin(); it != leaks.end(); it++)
         if (it->bt == bt)
            return it;
      return leaks.end();
   }

   void add(void* ptr, size_t size, const h2_backtrace& bt)
   {
      if (leaks.end() == find(bt)) leaks.push_back({ptr, bt});
      find(bt)->add(size);
   }

   h2_fail* check(const char* where, const h2_fs& fs)
   {
      h2_fail* fails = nullptr;
      for (auto& p : leaks) h2_fail::append_subling(fails, p.check(where, fs));
      return fails;
   }
};
// source/memory/h2_block.cpp
struct h2_block_attributes {
   unsigned long long limit = LLONG_MAX / 2;
   int alignment = sizeof(void*);
   unsigned char s_fill[32];
   int n_fill = 0;
   bool noleak = false;

   h2_block_attributes(const char* attributes)
   {
      double d;
      if (h2_extract::has(attributes, "noleak")) noleak = true;
      if (h2_extract::numeric(attributes, "limit", d)) limit = (unsigned long long)d;
      if (h2_extract::numeric(attributes, "align", d)) alignment = (int)d;
      n_fill = h2_extract::fill(attributes, "fill", s_fill);
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list pieces;

   h2_block_attributes attributes;
   unsigned long long footprint = 0, allocated = 0;
   const char* where;
   h2_fs fs;

   h2_block(const char* attributes_, const char* where_, const h2_fs& fs_) : attributes(attributes_), where(where_), fs(fs_) {}

   h2_fail* check()
   {
      h2_fail* fails = nullptr;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->violate_times)
            h2_fail::append_subling(fails, p->violate_fail());

      if (fails) return fails;

      h2_leaky leaky;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (!attributes.noleak && !p->free_times)
            leaky.add(p->user_ptr, p->user_size, p->bt_allocate);

      fails = leaky.check(where, fs);
      if (fails) return fails;

      /* why not chain fails in subling? report one fail ignore more for clean.
         when fail, memory may be in used, don't free and keep it for robust */
      h2_list_for_each_entry (p, pieces, h2_piece, x) {
         p->x.out();
         delete p;
      }
      return nullptr;
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, unsigned char c_fill, bool fill, h2_backtrace& bt)
   {
      if (attributes.limit < allocated + size) return nullptr;
      allocated += size;
      if (footprint < allocated) footprint = allocated;

      // allocate action alignment is prior to block level alignment
      if (alignment == 0) alignment = attributes.alignment;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      // allocate action fill is prior to block level fill
      unsigned char* s_fill = attributes.s_fill;
      int n_fill = attributes.n_fill;
      if (fill) {
         s_fill = &c_fill;
         n_fill = 1;
      }
      if (0 < n_fill)
         for (size_t i = 0, j = 0; i < size; ++i, ++j)
            ((unsigned char*)p->user_ptr)[i] = s_fill[j % n_fill];

      pieces.push_back(p->x);
      return p;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      allocated -= p->user_size;
      return p->free(who);
   }

   h2_piece* get_piece(const void* ptr) const
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->user_ptr == ptr) return p;
      return nullptr;
   }

   h2_piece* host_piece(const void* ptr) const
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->in_page_range((const unsigned char*)ptr)) return p;
      return nullptr;
   }
};
// source/memory/h2_stack.cpp
struct h2_stack {
   h2_singleton(h2_stack);
   h2_list blocks;
   bool at_exit = false;

   void push(const char* block_attributes, const char* where, const h2_fs &fs)
   {
      h2_block* b = new h2_block(block_attributes, where, fs);
      blocks.push(b->x);
   }

   h2_fail* pop()
   {
      h2_block* b = h2_list_pop_entry(blocks, h2_block, x);
      h2_fail* fail = b->check();
      delete b;
      return fail;
   }

   h2_block* top()
   {
      return h2_list_top_entry(blocks, h2_block, x);
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, const char* fill)
   {
      auto bt = h2_backtrace::dump(2);
      h2_block* b = bt.in(h2_exempt::I().fps) ? h2_list_bottom_entry(blocks, h2_block, x) : h2_list_top_entry(blocks, h2_block, x);
      return b ? b->new_piece(who, size, alignment, fill ? *fill : 0, fill, bt) : nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return p->rel_piece(who, piece);
      }
      if (!at_exit && O.os != 'W')
         if (!h2_backtrace::dump(1).in(h2_exempt::I().fps))
            h2_debug(2, "Warning: %s %p not found!", who, ptr);
      return nullptr;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return piece;
      }
      return nullptr;
   }

   h2_piece* host_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->host_piece(ptr);
         if (piece) return piece;
      }
      return nullptr;
   }
};
// source/memory/h2_override.cpp
struct h2_override {
   static size_t size(const void* ptr)
   {
      h2_piece* p = h2_stack::I().get_piece(ptr);
      return p ? p->user_size : 0;
   }
   static void free(void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("free", ptr));
   }
   static void* malloc(size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("malloc", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* calloc(size_t count, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("calloc", size * count, 0, "\0");
      return p ? p->user_ptr : nullptr;
   }
   static void* realloc(void* ptr, size_t size)
   {
      h2_piece *old_p = nullptr, *new_p = nullptr;
      if (size == 0 && O.os == 'm') size = 1;
      if (ptr) old_p = h2_stack::I().get_piece(ptr);
      if (size) new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (old_p && new_p) memcpy(new_p->user_ptr, old_p->user_ptr, std::min(old_p->user_size, size));
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("free", ptr));
      return new_p ? new_p->user_ptr : nullptr;
   }
   static int posix_memalign(void** memptr, size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("posix_memalign", size, alignment, nullptr);
      return p ? (*memptr = p->user_ptr, 0) : ENOMEM;
   }
   static void* aligned_alloc(size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("aligned_alloc", size, alignment, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new(std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new(std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new nothrow", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new[](std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new[]", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new[](std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new[] nothrow", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void operator delete(void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete(void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete nothrow", ptr));
   }
   static void operator delete[](void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[]", ptr));
   }
   static void operator delete[](void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[] nothrow", ptr));
   }
};
// source/memory/h2_override_stdlib.cpp
struct h2_override_stdlib {
   h2_stubs stubs;

   void set()
   {
      stubs.add((void*)::free, (void*)h2_override::free, {__FILE__, __LINE__, "free"});
      stubs.add((void*)::malloc, (void*)h2_override::malloc, {__FILE__, __LINE__, "malloc"});
      stubs.add((void*)::realloc, (void*)h2_override::realloc, {__FILE__, __LINE__, "realloc"});
      stubs.add((void*)::calloc, (void*)h2_override::calloc, {__FILE__, __LINE__, "calloc"});
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
      stubs.add((void*)::posix_memalign, (void*)h2_override::posix_memalign, {__FILE__, __LINE__, "posix_memalign"});
#endif
#if defined _ISOC11_SOURCE
      stubs.add((void*)::aligned_alloc, (void*)h2_override::aligned_alloc, {__FILE__, __LINE__, "aligned_alloc"});
#endif
      // valloc pvalloc memalign deprecated
      stubs.add((void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), {__FILE__, __LINE__, "new"});
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), {__FILE__, __LINE__, "new nothrow"});
      stubs.add((void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), {__FILE__, __LINE__, "new[]"});
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), {__FILE__, __LINE__, "new[] nothrow"});
      stubs.add((void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), {__FILE__, __LINE__, "delete"});
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), {__FILE__, __LINE__, "delete nothrow"});
      stubs.add((void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), {__FILE__, __LINE__, "delete[]"});
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), {__FILE__, __LINE__, "delete[] nothrow"});
   }

   void reset() { stubs.clear(); }
};
#if defined __linux
// source/memory/h2_override_linux.cpp
// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html

struct h2_override_platform {
   static void free_hook(void* __ptr, const void* caller) { h2_override::free(__ptr); }
   static void* malloc_hook(size_t __size, const void* caller) { return h2_override::malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return h2_override::realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return h2_override::aligned_alloc(__alignment, __size); }

   void (*saved__free_hook)(void*, const void*);
   void* (*saved__malloc_hook)(size_t, const void*);
   void* (*saved__realloc_hook)(void*, size_t, const void*);
   void* (*saved__memalign_hook)(size_t, size_t, const void*);

   h2_override_platform()
   {
      saved__free_hook = __free_hook;
      saved__malloc_hook = __malloc_hook;
      saved__realloc_hook = __realloc_hook;
      saved__memalign_hook = __memalign_hook;
   }

   void set()
   {
      __free_hook = free_hook;
      __malloc_hook = malloc_hook;
      __realloc_hook = realloc_hook;
      __memalign_hook = memalign_hook;
   }

   void reset()
   {
      __free_hook = saved__free_hook;
      __malloc_hook = saved__malloc_hook;
      __realloc_hook = saved__realloc_hook;
      __memalign_hook = saved__memalign_hook;
   }
};
#elif defined __APPLE__
// source/memory/h2_override_macos.cpp
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
#endif

struct h2_override_platform {
   static size_t mz_size(malloc_zone_t* zone, const void* ptr) { return h2_override::size(ptr); }
   static void* mz_malloc(malloc_zone_t* zone, size_t size) { return h2_override::malloc(size); }
   static void* mz_calloc(malloc_zone_t* zone, size_t num_items, size_t size) { return h2_override::calloc(num_items, size); }
   static void* mz_valloc(malloc_zone_t* zone, size_t size) { return h2_override::malloc(size); }
   static void mz_free(malloc_zone_t* zone, void* ptr) { h2_override::free(ptr); }
   static void mz_free_sized(malloc_zone_t* zone, void* ptr, size_t size) { h2_override::free(ptr); }
   static void* mz_realloc(malloc_zone_t* zone, void* ptr, size_t size) { return h2_override::realloc(ptr, size); }
   static void* mz_memalign(malloc_zone_t* zone, size_t align, size_t size) { return h2_override::aligned_alloc(align, size); }
   static void mz_destroy(malloc_zone_t* zone) {}

   static kern_return_t mi_enumerator(task_t task, void*, unsigned type_mask, vm_address_t zone_address, memory_reader_t reader, vm_range_recorder_t recorder) { return KERN_FAILURE; }
   static size_t mi_good_size(malloc_zone_t* zone, size_t size) { return size; }
   static boolean_t mi_check(malloc_zone_t* zone) { return true; }
   static void mi_print(malloc_zone_t* zone, boolean_t verbose) {}
   static void mi_log(malloc_zone_t* zone, void* address) {}
   static void mi_force_lock(malloc_zone_t* zone) {}
   static void mi_force_unlock(malloc_zone_t* zone) {}

   static boolean_t mi_zone_locked(malloc_zone_t* zone) { return false; }  // Hopefully unneeded by us!

   static malloc_zone_t* get_default_zone()
   {
      malloc_zone_t** zones = nullptr;
      unsigned int num_zones = 0;

      if (KERN_SUCCESS != malloc_get_all_zones(0, nullptr, (vm_address_t**)&zones, &num_zones)) num_zones = 0;
      if (num_zones) return zones[0];
      return malloc_default_zone();
   }

   malloc_introspection_t mi;
   malloc_zone_t mz;

   h2_override_platform()
   {
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

#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      mz.version = 6;
      mz.free_definite_size = &mz_free_sized;
      mz.memalign = &mz_memalign;
      mi.zone_locked = &mi_zone_locked;
      if (malloc_default_purgeable_zone) malloc_default_purgeable_zone();
#endif
   }

   void set()
   {
      malloc_zone_register(&mz);
      malloc_zone_t* default_zone = get_default_zone();
      malloc_zone_unregister(default_zone);
      malloc_zone_register(default_zone);
   }

   void reset()
   {
      malloc_zone_unregister(&mz);
   }
};
#elif defined _MSC_VER
// source/memory/h2_override_windows.cpp
// https://github.com/microsoft/mimalloc
// https://github.com/gperftools/gperftools

struct h2_override_platform {
   h2_stubs stubs;

   // windows specific free_base, free_dbg called by CRT internal functions or operator delete
   // windows specific _msize, _expand
   // A MS CRT "internal" function, implemented using _calloc_impl
   // obscured: _aligned_offset_malloc, _aligned_realloc, _aligned_recalloc, _aligned_offset_realloc, _aligned_offset_recalloc, _malloca, _freea ,_recalloc

   static void _free_base(void* ptr) { h2_override::free(ptr); }
   static void* _expand(void* memblock, size_t size) { return NULL; }
   // When _DEBUG _CRTDBG_MAP_ALLOC (default undefined) is defined CRT maps all to _*_dbg, bug CRT Debug version enabled.
#ifndef NDEBUG
   static void _free_dbg(void* userData, int blockType) { h2_override::free(userData); }
   static void* _malloc_dbg(size_t size, int blockType, const char* filename, int linenumber) { return h2_override::malloc(size); }
   static void* _realloc_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return h2_override::realloc(userData, newSize); }
   static void* _calloc_dbg(size_t num, size_t size, int blockType, const char* filename, int linenumber) { return h2_override::calloc(num, size); }
   static size_t _msize_dbg(void* userData, int blockType) { return h2_override::size(userData); }
   static void* _expand_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return NULL; }
#endif
   static void* _aligned_malloc(size_t size, size_t alignment) { return h2_override::aligned_alloc(size, alignment); }
   static void _aligned_free(void* memblock) { h2_override::free(memblock); }

   static char* _strdup(char* s)
   {
      char* ret = (char*)h2_override::malloc(strlen(s) + 1);
      if (ret) strcpy(ret, s);
      return ret;
   }

   void set()
   {
      stubs.add((void*)::_free_base, (void*)_free_base, {__FILE__, __LINE__, "_free_base"});
      stubs.add((void*)::_msize, (void*)h2_override::size, {__FILE__, __LINE__, "_msize"});
      stubs.add((void*)::_expand, (void*)_expand, {__FILE__, __LINE__, "_expand"});
#ifndef NDEBUG
      stubs.add((void*)::_free_dbg, (void*)_free_dbg, {__FILE__, __LINE__, "_free_dbg"});
      // stubs.add((void*)::_malloc_dbg, (void*)_malloc_dbg, {__FILE__, __LINE__, "_malloc_dbg"});
      // stubs.add((void*)::_realloc_dbg, (void*)_realloc_dbg, {__FILE__, __LINE__, "_realloc_dbg"});
      // stubs.add((void*)::_calloc_dbg, (void*)_calloc_dbg, {__FILE__, __LINE__, "_calloc_dbg"});
      // stubs.add((void*)::_expand_dbg, (void*)_expand_dbg, {__FILE__, __LINE__, "_expand_dbg"});
#endif
      //// stubs.add((void*)::_calloc_crt, (void*)h2_override::calloc, {__FILE__, __LINE__, "_calloc_crt"});
      stubs.add((void*)::_aligned_malloc, (void*)_aligned_malloc, {__FILE__, __LINE__, "_aligned_malloc"});
      stubs.add((void*)::_aligned_free, (void*)_aligned_free, {__FILE__, __LINE__, "_aligned_free"});
      stubs.add((void*)::_strdup, (void*)_strdup, {__FILE__, __LINE__, "_strdup"});  // strdup call to _strdup
   }

   void reset() { stubs.clear(); }
};
#else  // +MinGW
// source/memory/h2_override_cygwin.cpp
struct h2_override_platform {
   h2_stubs stubs;

   static char* strdup(char* s)
   {
      char* ret = (char*)h2_override::malloc(strlen(s) + 1);
      return ret && strcpy(ret, s), ret;
   }
   static char* strndup(char* s, size_t n)
   {
      char* ret = (char*)h2_override::malloc(n + 1);
      return ret && strncpy(ret, s, n), ret;
   }

   void set()
   {
      stubs.add((void*)::strdup, (void*)strdup, {__FILE__, __LINE__, "strdup"});
#if defined __CYGWIN__
      stubs.add((void*)::strndup, (void*)strndup, {__FILE__, __LINE__, "strndup"});
#endif
   }
   void reset() { stubs.clear(); }
};
#endif
// source/memory/h2_memory.cpp
struct h2_overrides {
   h2_singleton(h2_overrides);
   h2_override_stdlib stdlib;
   h2_override_platform platform;

   void set()
   {
      stdlib.set();
      platform.set();
   }
   void reset()
   {
      platform.reset();
      stdlib.reset();
   }
};

h2_inline void h2_memory::initialize()
{
   if (O.memory_check) h2_exempt::setup();
   hook();
   stack::root();
}
h2_inline void h2_memory::finalize()
{
   if (O.memory_check) h2_stack::I().at_exit = true;
}
h2_inline void h2_memory::hook(bool overrides)
{
   static bool s_overrides = false;
   if (O.memory_check) {
      if (overrides) {
         if (!s_overrides) h2_overrides::I().set();
      } else {
         if (s_overrides) h2_overrides::I().reset();
      }
      s_overrides = overrides;
   }
}

h2_inline void h2_memory::stack::root()
{
   h2_stack::I().push("", "root", {__FILE__, __LINE__});
}
h2_inline void h2_memory::stack::push(const h2_fs& fs)
{
   h2_stack::I().push("", "case", fs);
}
h2_inline h2_fail* h2_memory::stack::pop()
{
   return h2_stack::I().pop();
}
h2_inline long long h2_memory::stack::footprint()
{
   return h2_stack::I().top()->footprint;
}

h2_inline h2_memory::stack::block::block(const char* attributes, const h2_fs& fs)
{
   unmem = h2_extract::has(attributes, "unmem");
   if (unmem) h2_memory::hook(false);
   h2_stack::I().push(attributes, "block", fs);
}
h2_inline h2_memory::stack::block::~block()
{
   h2_fail_g(h2_stack::I().pop());
   if (unmem) h2_memory::hook();
}
// source/memory/h2_exempt.cpp
struct h2_exempt_stub {
   static time_t mktime(struct tm* timeptr) { return 1325347200; }
   static char* asctime(const struct tm* timeptr) { return (char*)"SUN JAN 1 00:00:00 2012\n"; }
   static char* asctime_r(const struct tm* timeptr, char* buf) { return strcpy(buf, (char*)"SUN JAN 1 00:00:00 2012\n"); }
   static char* ctime(const time_t* clock) { return (char*)"SUN JAN 1 00:00:00 2012\n"; }
   static char* ctime_r(const time_t* clock, char* buf) { return strcpy(buf, (char*)"SUN JAN 1 00:00:00 2012\n"); }
   static struct tm* localtime(const time_t* clock) { return gmtime(clock); }
   static struct tm* localtime_r(const time_t* timep, struct tm* result) { return gmtime_r(timep, result); }
   static struct tm* gmtime(const time_t* clock) { return gmtime_r(clock, nullptr); }

   static struct tm* gmtime_r(const time_t* clock, struct tm* result)
   {
      static struct tm st;
      if (!result) result = &st;
      memset(result, 0, sizeof(struct tm));
      //// result->tm_sec = 0; result->tm_min = 0; result->tm_hour = 0; result->tm_mon = 0; result->tm_wday = 0; result->tm_yday = 0;
      result->tm_mday = 1;
      result->tm_year = 2012 - 1900;
      return result;
   }
};

h2_inline void h2_exempt::setup()
{
   static h2_stubs stubs;

   stubs.add((void*)::gmtime, (void*)h2_exempt_stub::gmtime, {__FILE__, __LINE__, "gmtime"});
   stubs.add((void*)::ctime, (void*)h2_exempt_stub::ctime, {__FILE__, __LINE__, "ctime"});
   stubs.add((void*)::asctime, (void*)h2_exempt_stub::asctime, {__FILE__, __LINE__, "asctime"});
   stubs.add((void*)::localtime, (void*)h2_exempt_stub::localtime, {__FILE__, __LINE__, "localtime"});
   stubs.add((void*)::mktime, (void*)h2_exempt_stub::mktime, {__FILE__, __LINE__, "mktime"});

#if defined _WIN32
   add_by_fp((void*)::_wchdir);
   add_by_fp((void*)::fopen);
   add_by_fp((void*)::fclose);
   add_by_fp((void*)::strftime);
   add_by_fp((void*)::gmtime_s);
   add_by_fp((void*)::_gmtime32);
   add_by_fp((void*)::_gmtime64);
   add_by_fp((void*)::_gmtime32_s);
   add_by_fp((void*)::_gmtime64_s);
   add_by_fp(h2_un<void*>(&std::type_info::name));
#else
   stubs.add((void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, {__FILE__, __LINE__, "gmtime_r"});
   stubs.add((void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, {__FILE__, __LINE__, "ctime_r"});
   stubs.add((void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, {__FILE__, __LINE__, "asctime_r"});
   stubs.add((void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, {__FILE__, __LINE__, "localtime_r"});
   add_by_fp((void*)::sscanf);
   add_by_fp((void*)::sprintf);
   add_by_fp((void*)::vsnprintf);

#   if defined __APPLE__ && defined __clang__
   add_by_fp((void*)::snprintf);
   add_by_fp((void*)::snprintf_l);
   add_by_fp((void*)::strftime_l);
   add_by_fp((void*)::strtod_l);
   add_by_fp((void*)::strtold);
   add_by_fp((void*)::strtof_l);
#   endif
#endif

#if defined __GNUC__
   add_by_fp((void*)abi::__cxa_demangle);
   add_by_fp((void*)abi::__cxa_throw);
#   if !defined __clang__
   add_by_fp((void*)::__cxa_allocate_exception);
#   endif
#endif

   add_by_fp((void*)h2_pattern::regex_match);
}

h2_inline void h2_exempt::add_by_name(const char* fn)
{
   h2_symbol* res[16];
   int n = h2_nm::get_by_name(fn, res, 16);
   for (int i = 0; i < n; ++i)
      add_by_fp(h2_load::addr_to_ptr(res[i]->addr));
}

h2_inline void h2_exempt::add_by_fp(void* fp)
{
   I().fps[I().nfp++] = h2_cxa::follow_jmp(fp);
   I().fps[I().nfp] = nullptr;
}
// source/except/h2_debug.cpp
#if defined __linux
static inline bool under_debug(int, const char*)
{
   char t[1024];
   FILE* f = ::fopen("/proc/self/status", "r");
   if (!f) return false;
   bool ret = false;
   while (::fgets(t, sizeof(t) - 1, f)) {
      if (strncmp(t, "TracerPid:\t", 11) == 0) {
         ret = t[11] != '\0' && t[11] != '0';
         break;
      }
   }
   ::fclose(f);
   return ret;
}
#elif defined __APPLE__
static inline bool under_debug(int pid, const char* path)
{
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", pid);
   FILE* f = ::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r");
   if (!f) return false;
   bool ret = false;
   while (::fgets(t, sizeof(t) - 1, f)) {
      if (strstr(t, h2_fs(path).basefile()) || strstr(t, attach_pid)) {
         ret = true;
         break;
      }
   }
   ::pclose(f);
   return false;
}
#endif

h2_inline void h2_debugger::trap()
{
#if defined __linux || defined __APPLE__
   int pid = (int)getpid();
   if (!under_debug(pid, O.path)) {
      static h2_once only_one_time;
      if (only_one_time) {
         char cmd[512];
#if defined __linux
         sprintf(cmd, "sudo gdb --pid=%d", pid);
#elif defined __APPLE__
         sprintf(cmd, "sudo lldb --attach-pid %d", pid);
#endif
         if (fork() == 0)
            exit(system(cmd));
         else
            while (!under_debug(pid, O.path)) h2_sleep(100);
      }
   }
#endif
}
// source/except/h2_crash.cpp
struct h2_crash {
#if defined _WIN32
   static LONG segment_fault_handler(_EXCEPTION_POINTERS* ExceptionInfo)
   {
      if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
         h2_piece* piece = h2_stack::I().host_piece((const void*)ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
         if (piece) {
            auto operation = ExceptionInfo->ExceptionRecord->ExceptionInformation[0];
            piece->violate_forbidden((void*)ExceptionInfo->ExceptionRecord->ExceptionInformation[1], operation == 0 ? "read" : (operation == 1 ? "write" : (operation == 8 ? "execute" : "unknown")));
            return EXCEPTION_CONTINUE_EXECUTION;
         }
      }
      h2_debug(0, "");
      return EXCEPTION_EXECUTE_HANDLER;
   }

   static void install()
   {
      SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)segment_fault_handler);
   }
#else
   // https://www.gnu.org/software/libsigsegv/
   static void segment_fault_handler(int sig, siginfo_t* si, void* unused)
   {
      // si->si_code == SEGV_ACCERR
      h2_piece* piece = h2_stack::I().host_piece(si->si_addr);
      if (piece) {
         piece->violate_forbidden(si->si_addr);
         return;
      }
      h2_debug(0, "");
      abort();
   }

   static void install()
   {
      struct sigaction action;
      action.sa_sigaction = segment_fault_handler;
      action.sa_flags = SA_SIGINFO;
      sigemptyset(&action.sa_mask);

      if (sigaction(SIGSEGV, &action, nullptr) == -1) perror("Register SIGSEGV handler failed");
      if (sigaction(SIGBUS, &action, nullptr) == -1) perror("Register SIGBUS handler failed");
   }
#endif
};
// source/except/h2_exception.cpp
struct h2_exception {
   h2_singleton(h2_exception);
   h2_stubs stubs;
   h2_backtrace last_bt;
   char last_type[1024];

#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
   static void RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments)
   {
      I().last_bt = h2_backtrace::dump(1);
      if (O.exception_as_fail) h2_fail_g(h2_fail::new_exception("was thrown", "", I().last_bt));
      h2::h2_stub_temporary_restore t((void*)::RaiseException);
      ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
   }
#else
   static void __cxa_throw(void* thrown_exception, std::type_info* tinfo, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      I().last_bt = h2_backtrace::dump(1);
      h2_cxa::demangle(tinfo->name(), I().last_type);
      if (O.exception_as_fail) h2_fail_g(h2_fail::new_exception("was thrown", I().last_type, I().last_bt));
      h2::h2_stub_temporary_restore t((void*)abi::__cxa_throw);
      abi::__cxa_throw(thrown_exception, tinfo, dest);
   }
#endif

   static void initialize()
   {
#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
      I().stubs.add((void*)::RaiseException, (void*)RaiseException, {__FILE__, __LINE__, "RaiseException"});
#else
      I().stubs.add((void*)abi::__cxa_throw, (void*)__cxa_throw, {__FILE__, __LINE__, "__cxa_throw"});
#endif
      if (!O.debug) h2_crash::install();
   }
};
// source/stub/h2_e9.cpp
// https://github.com/microsoft/Detours/blob/master/src/detours.cpp

#if defined __i386__ || defined _M_IX86
static constexpr int h2_e9_size = 1 + 4;
#elif defined __x86_64__ || defined _M_X64
static constexpr int h2_e9_size = 2 + 8 + 2;
#elif defined __arm64__ || defined __aarch64__
static constexpr int h2_e9_size = 4 + 4 + 8;
#endif

static inline bool h2_e9_save(void* srcfp, unsigned char* opcode)
{
#if defined _WIN32
   DWORD t;
   if (!VirtualProtect(srcfp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t)) return false;  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
#else
   static unsigned long long page_size = (unsigned long long)h2_page_size(); // uintptr_t
   unsigned long long page_start = reinterpret_cast<unsigned long long>(srcfp) & ~(page_size - 1);
   int page_count = ::ceil((reinterpret_cast<unsigned long long>(srcfp) + h2_e9_size - page_start) / (double)page_size);
   if (mprotect(reinterpret_cast<void*>(page_start), page_count * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) return false;
#endif
   if (opcode) memcpy(opcode, srcfp, h2_e9_size);
   return true;
}

static inline void h2_e9_set(void* srcfp, void* dstfp)
{
   unsigned char* I = static_cast<unsigned char*>(srcfp);

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
   long long delta = (unsigned char*)dstfp - I - 5;
   if (delta < INT_MIN || INT_MAX < delta) {  //x86_64 asm("movq $dstfp, %rax; jmpq %rax")
      unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
      memcpy(C + 2, &dstfp, sizeof(void*));
      memcpy(I, C, sizeof(C));
   } else {  //i386 asm("jmp offset")
      unsigned char C[] = {0xE9, 0, 0, 0, 0};
      *(long*)(&C[1]) = (long)delta;
      memcpy(I, C, sizeof(C));
   }
   // ::FlushInstructionCache(GetCurrentProcess(), srcfp, h2_e9_size);

#elif defined __arm64__ || defined __aarch64__

#   pragma pack(push, 1)
   struct ldr_br_dst {
      unsigned int ldr;
      unsigned int br;
      void* dstfp;
   };
   struct b_offset {
      unsigned int b : 8;
      unsigned int offset : 24;
   };
#   pragma pack(pop)
   long long offset = (unsigned char*)dstfp - I;

   if (1 || offset < -8388607 - 1 || 0x7fffff < offset) {  // signed 2^24
      struct ldr_br_dst* p = static_cast<ldr_br_dst*>(srcfp);
      p->ldr = 0x58000000 | ((8 / 4) << 5) | 17;  // x17 register store dstfp
      p->br = 0xD61F0000 | (17 << 5);             // jmp x17
      p->dstfp = dstfp;
   } else {
      struct b_offset* p = static_cast<b_offset*>(srcfp);
      p->b = offset >= 0 ? 0x14 : 0x17;  //b 14 forward 17 backward
      p->offset = (unsigned long)((offset / 4) & 0xffffff);
   }

   __builtin___clear_cache((char*)I, (char*)I + h2_e9_size);
#endif
}

static inline void h2_e9_reset(void* srcfp, unsigned char* opcode)
{
   memcpy(srcfp, opcode, h2_e9_size);
   // ::FlushInstructionCache(GetCurrentProcess(), srcfp, h2_e9_size);
#if defined __arm64__ || defined __aarch64__
   __builtin___clear_cache(static_cast<char*>(srcfp), static_cast<char*>(srcfp) + h2_e9_size);
#endif
}
// source/stub/h2_source.cpp
struct h2_source : h2_libc {
   h2_list x;
   unsigned char origin_opcode[32];
   void* source_fp;
   int reference_count = 0;
   h2_source(void* source_fp_, const h2_fs& fs) : source_fp(source_fp_)
   {
      if (!h2_e9_save(source_fp, origin_opcode)) {
         h2_color::prints("yellow", "STUB %s by %s() failed %s:%d\n", fs.func, O.os == 'W' ? "VirtualProtect" : "mprotect", fs.file, fs.line);
         if (O.os == 'm') ::printf("try: "), h2_color::prints("green", "printf '\\x07' | dd of=%s bs=1 seek=160 count=1 conv=notrunc\n", O.path);
         if (O.os == 'L') ::printf("try: "), h2_color::prints("green", "objcopy --writable-text %s\n", O.path);
      }
   }
   ~h2_source() { h2_e9_reset(source_fp, origin_opcode); }
   void set(void* dstfp) { h2_e9_set(source_fp, dstfp); }
   void save(unsigned char opcode[32]) { h2_e9_save(source_fp, opcode); }
   void reset(unsigned char opcode[32]) { h2_e9_reset(source_fp, opcode); }
   void reset() { h2_e9_reset(source_fp, origin_opcode); }
};

struct h2_sources {
   h2_singleton(h2_sources);
   h2_list sources;

   h2_source* __find(void* fp)
   {
      h2_list_for_each_entry (p, sources, h2_source, x)
         if (p->source_fp == fp)
            return p;
      return nullptr;
   }

   void* __follow(void* fp)
   {
#if defined __arm64__ || defined __aarch64__
#else
      for (int i = 0; i < 1; ++i) {  // follow PLT(Linux) or ILT (Incremental Link Table /Windows)
         if (__find(fp)) break;
         void* next = h2_cxa::follow_jmp(fp, 1);
         if (next == fp) break;
         fp = next;
      }
#endif
      return fp;
   }

   h2_source* get(void* fp) { return __find(__follow(fp)); }

   h2_source* add(void* fp, const h2_fs& fs)
   {
      void* source_fp = __follow(fp);
      h2_source* source = __find(source_fp);
      if (!source) {
         source = new h2_source(source_fp, fs);
         sources.push(source->x);
      }
      source->reference_count++;
      return source;
   }

   void del(h2_source* source)
   {
      if (source && --source->reference_count == 0) {
         source->x.out();
         delete source;
      }
   }
};
// source/stub/h2_stubs.cpp
struct h2_stub : h2_libc {
   h2_list x;
   unsigned char saved_opcode[32];
   void *srcfp, *dstfp;
   h2_source* source;

   h2_stub(void* srcfp_, const h2_fs& fs) : srcfp(srcfp_)
   {
      source = h2_sources::I().add(srcfp, fs);
      if (source) source->save(saved_opcode);
   }
   ~h2_stub()
   {
      if (source) {
         source->reset(saved_opcode);
         h2_sources::I().del(source);
      }
   }
   void stub(void* dstfp_)
   {
      if (source) source->set((dstfp = dstfp_));
   }
};

static inline h2_stub* h2_stubs_get(h2_stubs* stubs, void* srcfp)
{
   h2_list_for_each_entry (p, stubs->stubs, h2_stub, x)
      if (p->srcfp == srcfp)
         return p;
   return nullptr;
}

h2_inline bool h2_stubs::add(void* srcfp, void* dstfp, const h2_fs& fs)
{
   h2_stub* stub = h2_stubs_get(this, srcfp);
   if (!stub) {
      stub = new h2_stub(srcfp, fs);
      stubs.push(stub->x);
   }
   stub->stub(dstfp);
   return true;
}

h2_inline void h2_stubs::clear(void* srcfp)
{
   h2_stub* stub = h2_stubs_get(this, srcfp);
   if (stub) {
      stub->x.out();
      delete stub;
   }
}

h2_inline void h2_stubs::clear()
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      p->x.out();
      delete p;
   }
}
// source/stub/h2_temporary.cpp
h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* srcfp_) : srcfp(srcfp_)
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) {
      source->save(current_opcode);
      source->reset();
   }
}

h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) source->reset(current_opcode);
}
// source/mock/h2_checkin.cpp
h2_inline h2_fail* h2_checkin::check(size_t index, size_t total, const h2_fs& fs) const
{
   if (is_satisfied() || is_saturated()) return nullptr;
   h2_line t = fs.func + gray("()") + " expected " + delta(expect(), "green") + " but actually " + delta(actual(), "red,bold") + " called";
   if (1 < total) t += gray(" when ") + h2_numeric::sequence_number(index) + " " + color(expr, "cyan");
   return h2_fail::new_normal(t, {nullptr, 0});
}

h2_inline const char* h2_checkin::actual() const
{
   static char st[64];
   if (call > 0)
      sprintf(st, "%d times", call);
   else
      sprintf(st, "never");
   return st;
}

h2_inline const char* h2_checkin::expect() const
{
   static char st[128];
   if (least == 0) {
      if (most == 0)
         sprintf(st, "never called");
      else if (most == INT_MAX)
         sprintf(st, "any number of times");
      else
         sprintf(st, "at most %d times", most);
   } else if (least == most) {
      sprintf(st, "exactly %d times", least);
   } else if (most == INT_MAX) {
      sprintf(st, "at least %d times", least);
   } else {  // 0 < least < most < INT_MAX
      sprintf(st, "between %d and %d times", least, most);
   }
   return st;
}
// source/mock/h2_mocker.cpp
h2_inline h2_line h2_mocker_base::argument(int seq, const char* def) const
{
   h2_line t;
   for (int i = 0; i < (int)argument_types.size(); ++i)
      t += (i ? gray(", ") : "") + color(argument_types[i], seq == i ? "red,bold" : def);
   return gray("(") + t + gray(")");
}

h2_inline h2_line h2_mocker_base::signature() const
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(fs.func, "green") + argument(-1, "cyan") + gray(">");
}

h2_inline void h2_mocker_base::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(srcfp, dstfp, fs);
}

h2_inline h2_fail* h2_mocker_base::check() const
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(i, checkin_array.size(), fs);
      if (fail) fail->seqno = (int)i;
      h2_fail::append_subling(fails, fail);
      h2_assert_g();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), fs);
   h2_fail::append_child(fail, fails);
   return fail;
}
// source/mock/h2_mocks.cpp
h2_inline bool h2_mocks::add(void* mock)
{
   h2_mocker_base* mocker = (h2_mocker_base*)mock;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x)
      if (p == mocker) return false;
   mocks.push(mocker->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(bool check)
{
   h2_fail* fails = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x) {
      if (check) h2_fail::append_subling(fails, p->check());
      p->reset();
      p->x.out();
   }
   return fails;
}
// source/stdio/h2_stdio.cpp
struct h2_stdio {
   h2_singleton(h2_stdio);
   h2_string* buffer;
   bool stdout_capturable = false, stderr_capturable = false, syslog_capturable = false;
   long long capture_length = 0;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      h2::h2_stub_temporary_restore t((void*)LIBC__write);
      if ((fd == fileno(stdout) || fd == fileno(stderr)) && h2_report::I().escape_length == I().capture_length && !h2_report::I().in)
         LIBC__write(fd, "\n", 1); // fall printf/cout into new line from report title
      LIBC__write(fd, buf, count);
      if (fd == fileno(stdout) || fd == fileno(stderr))
         I().capture_length += count;
      if ((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr)))
         I().buffer->append((char*)buf, count);
      return (ssize_t)count;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap)
   {
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      return (int)write(fileno(stream), alloca_str, strlen(alloca_str));
   }

   static int fprintf(FILE* stream, const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stream, format, a);
      va_end(a);
      return ret;
   }

   static int fputc(int c, FILE* stream)
   {
      unsigned char t = c;
      int ret = write(fileno(stream), &t, 1);
      return ret == 1 ? c : EOF;
   }

   static int fputs(const char* s, FILE* stream)
   {
      return write(fileno(stream), s, strlen(s));
   }

   static size_t fwrite(const void* ptr, size_t size, size_t nitems, FILE* stream)
   {
      return write(fileno(stream), ptr, size * nitems);
   }

   static int printf(const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stdout, format, a);
      va_end(a);
      return ret;
   }

   static int vprintf(const char* format, va_list ap)
   {
      return vfprintf(stdout, format, ap);
   }

   static int putchar(int c)
   {
      unsigned char t = c;
      write(fileno(stdout), &t, 1);
      return c;
   }

   static int puts(const char* s)
   {
      write(fileno(stdout), s, strlen(s));
      write(fileno(stdout), "\n", 1);
      return 1;
   }

   static void vsyslog(int priority, const char* format, va_list ap)
   {
      if (!I().syslog_capturable) return;
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      I().buffer->append(alloca_str, strlen(alloca_str));
   }

   static void syslog(int priority, const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      vsyslog(priority, format, a);
      va_end(a);
   }

   size_t test_count = 0;
   static ssize_t test_write(int fd, const void* buf, size_t count) { return I().test_count += count, (ssize_t)count; }

   static void initialize()
   {
      ::setbuf(stdout, 0);  // unbuffered
      I().buffer = new h2_string();
      static h2_stubs stubs;

#if !defined _WIN32
      stubs.add((void*)LIBC__write, (void*)test_write, {__FILE__, __LINE__, "write"});
      ::printf("\r"), ::fwrite("\r", 1, 1, stdout);
      stubs.clear();
#endif
      if (I().test_count != 2) {
         stubs.add((void*)::printf, (void*)printf, {__FILE__, __LINE__, "printf"});
         stubs.add((void*)::vprintf, (void*)vprintf, {__FILE__, __LINE__, "vprintf"});
         stubs.add((void*)::putchar, (void*)putchar, {__FILE__, __LINE__, "putchar"});
         stubs.add((void*)::puts, (void*)puts, {__FILE__, __LINE__, "puts"});
         stubs.add((void*)::fprintf, (void*)fprintf, {__FILE__, __LINE__, "fprintf"});
         // stubs.add((void*)::vfprintf, (void*)vfprintf, {__FILE__, __LINE__, "vfprintf"});
         stubs.add((void*)::fputc, (void*)fputc, {__FILE__, __LINE__, "fputc"});
         stubs.add((void*)::putc, (void*)fputc, {__FILE__, __LINE__, "fputc"});
         stubs.add((void*)::fputs, (void*)fputs, {__FILE__, __LINE__, "fputs"});
         stubs.add((void*)::fwrite, (void*)fwrite, {__FILE__, __LINE__, "fwrite"});
#if defined __GNUC__
         struct streambuf : public std::streambuf {
            FILE* f;
            int sync() override { return 0; }
            int overflow(int c) override { return (c != EOF) && h2_stdio::fputc(c, f), 0; }
            streambuf(FILE* _f) : f(_f) { setp(nullptr, 0); }
         };
         static streambuf sb_out(stdout);
         static streambuf sb_err(stderr);
         std::cout.rdbuf(&sb_out); /* internal fwrite() called, but */
         std::cerr.rdbuf(&sb_err);
         std::clog.rdbuf(&sb_err); /* print to stderr */
#endif
      }
      stubs.add((void*)LIBC__write, (void*)write, {__FILE__, __LINE__, "write"});
#if !defined _WIN32
      stubs.add((void*)::syslog, (void*)syslog, {__FILE__, __LINE__, "syslog"});
      stubs.add((void*)::vsyslog, (void*)vsyslog, {__FILE__, __LINE__, "vsyslog"});
#endif
   }

   void start_capture(bool stdout_capturable_, bool stderr_capturable_, bool syslog_capturable_)
   {
      stdout_capturable = stdout_capturable_;
      stderr_capturable = stderr_capturable_;
      syslog_capturable = syslog_capturable_;
      buffer->clear();
   }

   const char* stop_capture()
   {
      stdout_capturable = stderr_capturable = syslog_capturable = false;
      buffer->push_back('\0');
      return buffer->c_str();
   }
};

h2_inline h2_cout::h2_cout(h2_matcher<const char*> m_, const char* e_, const char* type_, const h2_fs& fs_) : fs(fs_), m(m_), e(e_), type(type_)
{
   bool all = !strlen(type);
   h2_stdio::I().start_capture(all || h2_extract::has(type, "out"), all || h2_extract::has(type, "err"), all || h2_extract::has(type, "syslog"));
}

h2_inline h2_cout::~h2_cout()
{
   h2_assert_g();
   h2_fail* fail = m.matches(h2_stdio::I().stop_capture(), 0);
   if (fail) {
      fail->fs = fs;
      fail->assert_type = "OK";
      fail->e_expression = e;
      fail->a_expression = "";
      fail->explain = "COUT";
      h2_fail_g(fail);
   }
}
// source/net/h2_dns.cpp
struct h2_name : h2_libc {
   h2_list x, y;
   h2_string name;
   h2_vector<h2_string> resolves;
   h2_name(const char* _hostname) : name(_hostname) {}
};

struct h2_resolver {
   h2_singleton(h2_resolver);
   h2_list dnses;

   static bool inet_addr(const char* str, struct sockaddr_in* addr)
   {
      struct sockaddr_in a;
      if (!addr) addr = &a;
      return 1 == inet_pton(AF_INET, str, &addr->sin_addr);
   }

   h2_name* find(const char* hostname)
   {
      h2_list_for_each_entry (p, dnses, h2_name, y)
         if (p->name == "*" || p->name == hostname)
            return p;
      return nullptr;
   }

   static int h2__stdcall getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res)
   {
      h2_name* name = I().find(hostname);
      if (!name) return -1;

      static struct addrinfo addrinfos[32];
      static struct sockaddr_in sockaddrs[32];
      memset(addrinfos, 0, sizeof(addrinfos));
      memset(sockaddrs, 0, sizeof(sockaddrs));

      struct addrinfo** pp = res;
      for (size_t i = 0; i < name->resolves.size(); ++i) {
         struct addrinfo* a = &addrinfos[i];
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(name->resolves[i].c_str(), b)) {
            a->ai_addr = (struct sockaddr*)b;
            a->ai_addrlen = sizeof(struct sockaddr_in);
         } else
            a->ai_canonname = (char*)name->resolves[i].c_str();
         if (hints) {
            a->ai_family = hints->ai_family;
            a->ai_socktype = hints->ai_socktype;
            a->ai_protocol = hints->ai_protocol;
         } else {
            a->ai_family = AF_INET;
            a->ai_socktype = SOCK_STREAM;
            a->ai_protocol = IPPROTO_TCP;
         }
         a->ai_next = nullptr;
         *pp = a, pp = &a->ai_next;
      }
      *pp = nullptr;
      return 0;
   }

   static void h2__stdcall freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* h2__stdcall gethostbyname(char* hostname)
   {
      h2_name* name = I().find(hostname);
      if (!name) return nullptr;

      static struct sockaddr_in sockaddrs[32];
      static char* h_aliases[32];
      static char* h_addr_list[32];
      static struct hostent h;

      h.h_name = hostname;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      memset(sockaddrs, 0, sizeof(sockaddrs));
      memset(h_aliases, 0, sizeof(h_aliases));
      memset(h_addr_list, 0, sizeof(h_addr_list));

      for (size_t i = 0, a = 0, c = 0; i < name->resolves.size(); ++i) {
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(name->resolves[i].c_str(), b))
            h_addr_list[a++] = (char*)&b->sin_addr;
         else
            h_aliases[c++] = (char*)name->resolves[i].c_str();
      }
      return &h;
   }

   h2_stubs stubs;
   h2_resolver()
   {
      stubs.add((void*)::getaddrinfo, (void*)getaddrinfo, {__FILE__, __LINE__, "getaddrinfo"});
      stubs.add((void*)::freeaddrinfo, (void*)freeaddrinfo, {__FILE__, __LINE__, "freeaddrinfo"});
      stubs.add((void*)::gethostbyname, (void*)gethostbyname, {__FILE__, __LINE__, "gethostbyname"});
   }
   ~h2_resolver() { stubs.clear(); }
};

h2_inline void h2_dnses::add(h2_list& name)
{
   dnses.push(name);
}

h2_inline void h2_dnses::clear()
{
   h2_list_for_each_entry (p, dnses, h2_name, x) {
      p->x.out();
      p->y.out();
      delete p;
   }
}

h2_inline void h2_dns::setaddrinfo(int n, ...)
{
   if (n == 0) return;
   va_list a, b;

   const char *hostname = "*", *p;
   va_start(a, n);
   for (int i = 0; i < n; ++i)
      if ((p = va_arg(a, const char*)))
         if (!h2_resolver::inet_addr(p, nullptr))
            if (strlen(hostname) < 2 || strlen(p) < strlen(hostname))
               hostname = p;
   va_end(a);

   h2_name* name = new h2_name(hostname);
   va_start(b, n);
   for (int i = 0; i < n; ++i)
      if ((p = va_arg(b, const char*)))
         if (strcmp(hostname, p))
            name->resolves.push_back(p);
   va_end(b);

   h2_resolver::I().dnses.push(name->y);
   if (h2_runner::I().current_case) h2_runner::I().current_case->dnses.add(name->x);
}

h2_inline void h2_dns::initialize()
{
   setaddrinfo(1, "127.0.0.1");
}
// source/net/h2_socket.cpp
struct h2_socket {
   h2_singleton(h2_socket);

   static bool is_block(int sockfd)
   {
#if defined _WIN32
      return true;
#else
      return !(fcntl(sockfd, F_GETFL) & O_NONBLOCK);
#endif
   }
   static bool set_block(int sockfd, bool block)
   {
#if defined _WIN32
      u_long op = block ? 0 : 1;
      if (ioctlsocket(sockfd, FIONBIO, &op) == SOCKET_ERROR) {
         return false;
      }
      return true;
#else
      int flags = fcntl(sockfd, F_GETFL);
      if (flags < 0) {
         return false;
      }

      if (block) {
         if ((flags & O_NONBLOCK) != 0) {
            flags ^= O_NONBLOCK;
         }
      } else {
         flags |= O_NONBLOCK;
      }
      if (fcntl(sockfd, F_SETFL, flags) < 0) {
         return false;
      }

      return true;
#endif
   }

   static void iport_parse(const char* str, struct sockaddr_in* addr)
   {
      char temp[1024];
      strcpy(temp, str);
      addr->sin_family = AF_INET;
      addr->sin_port = 0;
      char* colon = strchr(temp, ':');
      if (colon) {
         *colon = '\0';
         addr->sin_port = htons(atoi(colon + 1));
      }
      inet_pton(AF_INET, temp, &addr->sin_addr);
   }

   static const char* iport_tostring(struct sockaddr_in* addr, char* str)
   {
      char t[256];
      sprintf(str, "%s:%d", inet_ntop(AF_INET, &addr->sin_addr, t, sizeof(t)), ntohs(addr->sin_port));
      return str;
   }

   struct temporary_noblock : h2_once {
      int sockfd, flags;
      temporary_noblock(int sockfd_) : sockfd(sockfd_)
      {
         flags = is_block(sockfd);
         set_block(sockfd, false);
      }
      ~temporary_noblock() { set_block(sockfd, flags); }
   };

   static const char* getsockname(int sockfd, char* s, struct sockaddr_in* a = nullptr)
   {
      struct sockaddr_in b;
      if (!a) a = &b;
      socklen_t l = sizeof(struct sockaddr_in);
      ::getsockname(sockfd, (struct sockaddr*)a, &l);
      return iport_tostring(a, s);
   }

   static h2_packet* read_incoming(int sockfd)
   {
      bool block = is_block(sockfd);
      const char* local = getsockname(sockfd, (char*)alloca(64));

      do {
         h2_list_for_each_entry (p, I().incoming, h2_packet, x) {
            if (h2_pattern::wildcard_match(p->to.c_str(), local)) {
               p->x.out();
               return p;
            }
         }
         if (block) h2_sleep(100);
      } while (block);
      return nullptr;
   }

   static int h2__stdcall accept(int socket, struct sockaddr* address, socklen_t* address_len)
   {
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      iport_parse(tcp->from.c_str(), (struct sockaddr_in*)address);
      *address_len = sizeof(struct sockaddr_in);

      int fd = (int)::socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in a;
      const char* c = getsockname(socket, (char*)alloca(64), &a);
      ::bind(fd, (struct sockaddr*)&a, sizeof(a));
      I().sockets.push_back({fd, c, tcp->from.c_str()});
      if (tcp->data.size())
         I().incoming.push(tcp->x);
      else
         delete tcp;

      return fd;
   }

   static int h2__stdcall connect(int socket, const struct sockaddr* address, socklen_t address_len)
   {
      I().sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      if (tcp->data.size())
         I().incoming.push(tcp->x);
      else
         delete tcp;
      return 0;
   }

   static ssize_t h2__stdcall send(int socket, const void* buffer, size_t length, int flags)
   {
      I().put_outgoing(socket, (const char*)buffer, length);
      return (ssize_t)length;
   }
   static ssize_t h2__stdcall recv(int socket, void* buffer, size_t length, int flags)
   {
      ssize_t ret = 0;
      h2_packet* tcp = read_incoming(socket);
      if (tcp) {
         ret = (ssize_t)tcp->data.copy((char*)buffer, tcp->data.size(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t h2__stdcall sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len)
   {
      I().put_outgoing(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return (ssize_t)length;
   }
   static ssize_t h2__stdcall recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len)
   {
      ssize_t ret = 0;
      h2_packet* udp = read_incoming(socket);

      if (udp) {
         ret = (ssize_t)udp->data.copy((char*)buffer, udp->data.size(), 0);
         iport_parse(udp->from.c_str(), (struct sockaddr_in*)address);
         *address_len = sizeof(struct sockaddr_in);
         delete udp;
      }
      return ret;
   }
#if !defined _WIN32
   static ssize_t h2__stdcall sendmsg(int socket, const struct msghdr* message, int flags)
   {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
   static ssize_t h2__stdcall recvmsg(int socket, struct msghdr* message, int flags)
   {
      return recvfrom(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, &message->msg_namelen);
   }
#endif

   h2_stubs stubs;

   struct socket {
      int fd;
      h2_string from, to;
      socket(int fd_, const char* from_, const char* to_) : fd(fd_), from(from_), to(to_) {}
   };

   h2_vector<socket> sockets;

   void put_outgoing(const char* from, const char* to, const char* data, size_t size)
   {
      strcpy(last_to, to);
      outgoing.push_back((new h2_packet(from, to, data, size))->x);
   }
   void put_outgoing(int fd, const char* data, size_t size)
   {
      char from[128] = "", to[128] = "";
      for (auto& t : sockets) {
         if (t.fd == fd) {
            strcpy(from, t.from.c_str());
            strcpy(to, t.to.c_str());
            break;
         }
      }
      put_outgoing(from, to, data, size);
   }
   void put_incoming(const char* from, const char* to, const char* data, size_t size)
   {
      incoming.push_back((new h2_packet(strlen(from) ? from : last_to, to, data, size))->x);
   }

   char last_to[128];
   h2_list incoming, outgoing;

   void start()
   {
      strcpy(last_to, "0.0.0.0:0");

      stubs.add((void*)::sendto, (void*)sendto, {__FILE__, __LINE__, "sendto"});
      stubs.add((void*)::recvfrom, (void*)recvfrom, {__FILE__, __LINE__, "recvfrom"});
#if !defined _WIN32
      stubs.add((void*)::sendmsg, (void*)sendmsg, {__FILE__, __LINE__, "sendmsg"});
      stubs.add((void*)::recvmsg, (void*)recvmsg, {__FILE__, __LINE__, "recvmsg"});
#endif
      stubs.add((void*)::send, (void*)send, {__FILE__, __LINE__, "send"});
      stubs.add((void*)::recv, (void*)recv, {__FILE__, __LINE__, "recv"});
      stubs.add((void*)::accept, (void*)accept, {__FILE__, __LINE__, "accept"});
      stubs.add((void*)::connect, (void*)connect, {__FILE__, __LINE__, "connect"});
   }
   void stop()
   {
      stubs.clear();
      h2_list_for_each_entry (p, incoming, h2_packet, x) {
         p->x.out();
         delete p;
      }
      h2_list_for_each_entry (p, outgoing, h2_packet, x) {
         p->x.out();
         delete p;
      }
   }
};

h2_inline void h2_sock::inject(const void* packet, size_t size, const char* attributes)
{
   char from[256] = "", to[256] = "*";
   h2_extract::iport(attributes, "from", from);
   h2_extract::iport(attributes, "to", to);
   h2_socket::I().put_incoming(from, to, (const char*)packet, size);
}

h2_inline h2_packet* h2_sock::fetch()
{
   return h2_list_pop_entry(h2_socket::I().outgoing, h2_packet, x);
}

h2_inline void h2_sock::clear()
{
   h2_socket::I().stop();
}

h2_inline h2_sock::h2_sock()
{
   h2_socket::I().start();
}

h2_inline h2_sock::~h2_sock()
{
   h2_socket::I().stop();
}
// source/core/h2_case.cpp
h2_inline void h2_case::clear()
{
   h2_sock::clear();
   h2_memory::hook();
   if (fails) delete fails;
   fails = nullptr;
   asserts = 0;
}

h2_inline void h2_case::prev_setup()
{
   failed = false;
   h2_memory::stack::push(fs);
}

h2_inline void h2_case::post_cleanup()
{
   footprint = h2_memory::stack::footprint();
   dnses.clear();
   stubs.clear();
   do_fail(mocks.clear(true), true, O.verbose >= 5);
   do_fail(h2_memory::stack::pop(), true, O.verbose >= 5);
}

h2_inline void h2_case::do_fail(h2_fail* fail, bool defer, bool append)
{
   if (fail) {
      failed = true;
      if (fails && !append)
         delete fail;
      else
         h2_fail::append_subling(fails, fail);
      if (!defer) ::longjmp(ctx, 1);
   }
}
// source/core/h2_suite.cpp
h2_inline h2_suite::h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const h2_fs& fs_) : fs(fs_), name(name_), test_code(test_code_)
{
   memset(ctx, 0, sizeof(jmp_buf));
   h2_runner::I().suites.push_back(x);
}

h2_inline void h2_suite::clear()
{
   stats.clear();
}

h2_inline void h2_suite::setup()
{
   h2_memory::stack::push(fs);
}

h2_inline void h2_suite::cleanup()
{
   stubs.clear();
   mocks.clear(false);
   stats.footprint = h2_memory::stack::footprint();
   h2_memory::stack::pop();
}

h2_inline void h2_suite::enumerate()
{
   test_code(this, nullptr); /* enumerate case by static local h2_case variable inside of h2_suite_test_CmLn() */
}

h2_inline void h2_suite::test(h2_case* c)
{
   c->prev_setup();
   try {
      test_code(this, c); /* include Setup(); c->post_setup() and c->prev_cleanup(); Cleanup() */
   } catch (...) {
      c->do_fail(h2_fail::new_exception("was thrown but uncaught", h2_exception::I().last_type, h2_exception::I().last_bt), true, O.verbose >= 5);
   }
   c->post_cleanup();
}

h2_inline h2_suite::registor::registor(h2_suite* s, h2_case* c)
{
   static int s_auto_increment = INT_MAX / 4;
   s->cases.push_back(c->x);
   s->seq = c->seq = ++s_auto_increment;
}

h2_inline h2_suite::cleaner::~cleaner()
{
   static const jmp_buf zero = {0};
   if (memcmp((const void*)thus->ctx, (const void*)zero, sizeof(jmp_buf)))
      ::longjmp(thus->ctx, 1);
}
// source/core/h2_runner.cpp
static inline void drop_last_order() { ::remove(".last_order"); }

static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, "%s\n%s\n%d\n", ss(s->name), c->name, (int)c->failed);
   ::fclose(f);
}

static inline void __mark(h2_list& suites, char* suitename, char* casename, bool failed)
{
   static int seq = INT_MIN / 4;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, ss(s->name)))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name))
               s->seq = c->seq = ++seq, c->last_failed = failed;
}

static inline int mark_last_order(h2_list& suites)
{
   int count = 0;
   char suitename[1024], casename[1024], failed[32];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (::fgets(suitename, sizeof(suitename), f) && ::fgets(casename, sizeof(casename), f) && ::fgets(failed, sizeof(failed), f)) {
      suitename[strlen(suitename) - 1] = '\0'; /* remove \n in save_last_order */
      casename[strlen(casename) - 1] = '\0';
      failed[strlen(failed) - 1] = '\0';
      __mark(suites, suitename, casename, !!atoi(failed));
      count++;
   }
   ::fclose(f);
   return count;
}

struct h2_compare_wrapper {
   static int suite_cmp(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_suite, x)->seq - h2_list_entry(b, h2_suite, x)->seq;
   }
   static int case_cmp(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_case, x)->seq - h2_list_entry(b, h2_case, x)->seq;
   }
};

h2_inline void h2_runner::shuffle()
{
   last = mark_last_order(suites);
   ::srand(::clock());
   if (O.shuffle_cases && last == 0)
      h2_list_for_each_entry (s, suites, h2_suite, x)
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            s->seq = c->seq = ::rand();

   suites.sort(h2_compare_wrapper::suite_cmp);
   h2_list_for_each_entry (s, suites, h2_suite, x)
      s->cases.sort(h2_compare_wrapper::case_cmp);
}

h2_inline void h2_runner::shadow()
{
   if (stats.failed == 0)
      drop_last_order();
   else if (last == 0)
      save_last_order(suites);
}

h2_inline void h2_runner::enumerate()
{
   int cases = 0, i = 0;
   if (O.progressing) ::printf("enumerating...");
   h2_list_for_each_entry (s, suites, h2_suite, x) {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& cleanup : global_suite_cleanups) cleanup();
      int unfiltered = 0;
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         if (!(c->filtered = O.filter(ss(s->name), c->name, c->fs.file, c->fs.line)))
            unfiltered++;
      if (unfiltered == 0) s->filtered = O.filter(ss(s->name), "", s->fs.file, s->fs.line);
      cases += s->cases.count();
      if (O.progressing && 10 * i + i * i < cases && i < (int)h2_shell::I().cww - 20) i += ::printf(".");
   }
   if (O.progressing) ::printf("\33[2K\r");
}

h2_inline int h2_runner::main(int argc, const char** argv)
{
#if defined _MSC_VER
   SetConsoleOutputCP(65001);  // set console code page to utf-8
   SymInitialize(GetCurrentProcess(), NULL, TRUE);
#endif
   h2_option::I().parse(argc, argv);
   h2_report::initialize();
   h2_memory::initialize();
   h2_exception::initialize();
   h2_stdio::initialize();
   h2_dns::initialize();

   for (auto& setup : global_setups) setup();
   enumerate();

   h2_report::I().on_runner_start(this);
   for (rounds = 0; rounds < O.run_rounds; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if ((0 < O.break_after_fails && O.break_after_fails <= stats.failed) || (O.last_failed && !c->last_failed)) c->ignored = true;
            if (c->ignored)
               stats.ignored++, s->stats.ignored++;
            else if (c->filtered)
               stats.filtered++, s->stats.filtered++;
            else if (c->todo)
               stats.todo++, s->stats.todo++;

            current_case = c;
            h2_report::I().on_case_start(s, c);
            if (!O.list_cases && !c->todo && !c->filtered && !c->ignored) {
               for (auto& setup : global_case_setups) setup();
               s->test(c);
               for (auto& cleanup : global_case_cleanups) cleanup();
               c->failed ? (stats.failed++, s->stats.failed++) : (stats.passed++, s->stats.passed++);
            }
            h2_report::I().on_case_endup(s, c);
            c->clear();
         }
         s->cleanup();
         for (auto& cleanup : global_suite_cleanups) cleanup();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      shadow();
   }
   h2_report::I().on_runner_endup(this);
   for (auto& cleanup : global_cleanups) cleanup();

   stubs.clear();
   mocks.clear(false);
   h2_memory::finalize();
   return O.verbose >= 6 ? stats.failed : 0;
}
// source/assert/h2_assert.cpp
static inline const char* find_op(const char* src, const char* op)
{
   bool quote = false;
   if (strlen(op) == 2) {
      for (const char* p = src; *p; p++) {
         if (*p == '\"') quote = !quote;
         if (!quote && !strncmp(op, p, 2)) return p;
      }
   } else {
      int stacks = 0;
      if (*op == '>')
         for (const char* p = src; *p; p++) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '<') ++stacks;
            if (!quote && *p == '>' && 0 == stacks--) return p;
         }
      if (*op == '<')
         for (const char* p = src + strlen(src); src <= p; p--) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '>') ++stacks;
            if (!quote && *p == '<' && 0 == stacks--) return p;
         }
   }
   return nullptr;
}

h2_inline h2_defer_failure::~h2_defer_failure()
{
   if (fails) {
      fails->fs = fs;
      fails->assert_type = assert_type;
      fails->assert_op = assert_op;
      fails->e_expression = e_expression;
      fails->a_expression = a_expression;
      fails->user_explain = oss.str().c_str();

      if (!strcmp("CP", assert_type) && strcmp(",", assert_op)) {
         const char* p_op = find_op(a_expression, assert_op);
         if (p_op) {
            const char *p, *q;
            for (p = p_op - 1; a_expression <= p && ::isspace(*p);) p--;
            fails->e_expression.assign(a_expression, (p + 1) - a_expression);
            for (q = p_op + strlen(assert_op); ::isspace(*q);) q++;
            fails->a_expression.assign(q, (a_expression + strlen(a_expression)) - q);
         }
      }
      h2_fail_g(fails);
   }
}
// source/assert/h2_timer.cpp
h2_inline h2_timer::h2_timer(int ms_, const h2_fs& fs_) : fs(fs_), cpu_ms(ms_)
{
   start = ::clock();
}

h2_inline h2_timer::~h2_timer()
{
   h2_assert_g();
   double delta = (::clock() - start) * 1000.0 / CLOCKS_PER_SEC;
   if (cpu_ms < delta) {
      h2_line line = "performance expect < ";
      line.printf("green", "%d", cpu_ms).printf("", " ms, but actually cost ").printf("red", "%d", (int)delta).printf("", " ms");
      h2_fail_g(h2_fail::new_normal(line, fs));
   }
}
// source/render/h2_failure.cpp
h2_inline void h2_fail::append_subling(h2_fail*& fail, h2_fail* nf)
{
   if (!fail) {
      fail = nf;
   } else {
      h2_fail** pp = &fail->subling_next;
      while (*pp) pp = &(*pp)->subling_next;
      *pp = nf;
   }
}

h2_inline void h2_fail::append_child(h2_fail*& fail, h2_fail* nf)
{
   if (!fail) {
      fail = nf;
   } else {
      h2_fail** pp = &fail->child_next;
      while (*pp) pp = &(*pp)->child_next;
      *pp = nf;
   }
}

h2_inline h2_fail::~h2_fail()
{
   if (child_next) delete child_next;
   if (subling_next) delete subling_next;
}

h2_inline h2_line h2_fail::locate()
{
   if (!fs.empty())
      return gray(" at ") + h2_string("%s:%d", fs.basefile(), fs.line);
   return {};
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, size_t, size_t)> cb, size_t si, size_t ci)
{
   cb(this, si, ci);
   if (child_next) child_next->foreach(cb, 0, ci + 1);
   if (subling_next) subling_next->foreach(cb, si + 1, ci);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_line& explain_ = {}, const h2_fs& fs_ = h2_fs()) : h2_fail(explain_, fs_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      line += explain;
      h2_color::printl(line + locate());
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   static const char* s_null[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0", "00000000", "0000000000000000", nullptr};
   static const char* s_true[] = {"IsTrue", "true", "TRUE", "True", "1", nullptr};
   static const char* s_false[] = {"IsFalse", "false", "FALSE", "False", "0", nullptr};
   static const char** S[] = {s_null, s_true, s_false};
   h2_string a1 = a.escape(), b1 = b.escape();
   if (a1 == b1) return true;
   for (size_t i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
      if (h2_in(a1.c_str(), S[i]) && h2_in(b1.c_str(), S[i]))
         return true;
   return false;
}

struct h2_fail_unexpect : h2_fail {
   h2_line expection, represent;
   int c = 0;
   h2_fail_unexpect(const h2_line& expection_ = {}, const h2_line& represent_ = {}, const h2_line& explain_ = {}, const h2_fs& fs_ = h2_fs()) : h2_fail(explain_, fs_), expection(expection_), represent(represent_) {}
   void print_OK1(h2_line& line)
   {
      h2_line a = h2_line(a_expression).gray_quote().brush("cyan");
      line += "OK" + gray("(") + a + gray(")") + " is " + color("false", "bold,red");
   }
   void print_OK2_CP(h2_line& line, const char* assert_type)
   {
      h2_line e, a;
      if (!expection.width()) {
         e = h2_line(e_expression).abbreviate(10000, 3).gray_quote().brush("green");
      } else if (is_synonym(e_expression, expection.string())) {
         e = expection.abbreviate(10000, 3).brush("green");
      } else {
         e = h2_line(e_expression).abbreviate(O.verbose >= 4 ? 10000 : 120, 3).gray_quote().brush("cyan") + gray("==>") + expection.abbreviate(10000, 3).brush("green");
      }  // https://unicode-table.com/en/sets/arrow-symbols/

      if (!represent.width()) {
         a = h2_line(a_expression).abbreviate(10000, 3).gray_quote().brush("bold,red");
      } else if (is_synonym(a_expression, represent.string()) || !a_expression.length()) {
         a = represent.abbreviate(10000, 3).brush("bold,red");
      } else {
         a = represent.abbreviate(10000, 3).brush("bold,red") + gray("<==") + h2_line(a_expression).abbreviate(O.verbose >= 4 ? 10000 : 120, 3).gray_quote().brush("cyan");
      }

      line += assert_type + gray("(") + e + " " + assert_op + " " + a + gray(")");
   }
   void print_JE(h2_line& line)
   {
      h2_line e = h2_line(e_expression.unquote('\"').unquote('\'')).abbreviate(O.verbose >= 4 ? 10000 : 30, 2).brush("cyan");
      h2_line a = h2_line(a_expression.unquote('\"').unquote('\'')).abbreviate(O.verbose >= 4 ? 10000 : 30, 2).brush("bold,red");
      line += "JE" + gray("(") + e + ", " + a + gray(")");
   }
   void print_Inner(h2_line& line)
   {
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      if (expection.width()) {
         line.printf("", "%sexpect is ", comma_if(c++));
         line += expection.abbreviate(O.verbose >= 4 ? 10000 : 120, 3).brush("green");
      }
      if (represent.width()) {
         line.printf("", "%sactual is ", comma_if(c++));
         line += represent.abbreviate(O.verbose >= 4 ? 10000 : 120, 3).brush("bold,red");
      }
   }

   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (!strcmp("Inner", assert_type)) print_Inner(line);
      if (!strcmp("OK1", assert_type)) print_OK1(line);
      if (!strcmp("OK", assert_type) || !strcmp("CP", assert_type)) print_OK2_CP(line, assert_type);
      if (!strcmp("JE", assert_type)) print_JE(line);
      if (explain.width()) line += comma_if(c++, ", ", " ") + explain;
      if (user_explain.size()) line += {comma_if(c++, ", ", " "), user_explain};
      h2_color::printl(line + locate());
   }
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   const h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_line& expection, const h2_line& explain = {}) : h2_fail_unexpect(expection, h2_representify(a_value_), explain), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   h2_line fmt_char(h2_string& c, bool eq, const char* style)
   {
      if (c.equals(" ") && O.colorful) return gray("‧");
      if (eq) return c.escape();
      return color(c.escape(), style);
   }
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (16 < e_value.width() || 16 < a_value.width()) {
         h2_line e_line, a_line;
         h2_vector<h2_string> e_chars = e_value.disperse(), a_chars = a_value.disperse();
         auto lcs = h2_LCS(e_chars, a_chars, caseless).lcs();
         for (size_t i = 0; i < lcs.first.size(); i++) e_line += fmt_char(e_chars[i], lcs.first[i], "green");
         for (size_t i = 0; i < lcs.second.size(); i++) a_line += fmt_char(a_chars[i], lcs.second[i], "red");
         h2_color::printl(h2_layout::unified(e_line, a_line, "expect", "actual", h2_shell::I().cww));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   const h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_line& expection, const h2_line& explain) : h2_fail_unexpect(expection, h2_representify(a_value_), explain), e_value(e_value_), a_value(a_value_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (16 < e_value.width() || 16 < a_value.width()) {
         h2_line e_line = e_value.escape(), a_line = a_value.escape();
         h2_color::printl(h2_layout::seperate(e_line, a_line, "expect", "actual", h2_shell::I().cww));
      }
   }
};

struct h2_fail_json : h2_fail_unexpect {
   const bool caseless;
   const h2_string e_value, a_value;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_line& expection_, bool caseless_, const h2_line& explain_) : h2_fail_unexpect(expection_, a_value_.squash(), explain_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      e_expression = e_expression.squash(true);
      a_expression = a_expression.squash(true);

      h2_lines e_lines, a_lines;
      h2_fail_unexpect::print(si, ci);
      if (O.copy_paste_json || !h2_json::diff(e_value, a_value, e_lines, a_lines, caseless)) {
         e_lines = h2_json::dump(e_value);
         a_lines = h2_json::dump(a_value);
         for (size_t i = 0; i < e_lines.size(); ++i)
            if (i) e_lines[i].indent(8);
         for (size_t i = 0; i < a_lines.size(); ++i)
            if (i) a_lines[i].indent(8);
         h2_color::prints("dark gray", "expect");
         h2_color::prints("green", "> ");
         h2_color::printl(e_lines);
         h2_color::prints("dark gray", "actual");
         h2_color::prints("red", "> ");
         h2_color::printl(a_lines);
      } else {
         h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual", 0, 'd', h2_shell::I().cww - 1);
         for (auto& line : lines) line.indent(1);
         h2_color::printl(lines);
      }
   }
};

struct h2_fail_memcmp : h2_fail_unexpect {
   const h2_vector<unsigned char> e_value, a_value;
   const size_t length, width;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, const size_t length_, const size_t width_) : h2_fail_unexpect({}, {}, {}), e_value(e_value_, e_value_ + (length_ * width_ + 7) / 8), a_value(a_value_, a_value_ + (length_ * width_ + 7) / 8), length(length_), width(width_) {}
   void print(size_t si, size_t ci) override
   {
      expection.printf("", "memcmp %d %s", (int)length, format_width());
      h2_fail_unexpect::print(si, ci);
      h2_lines e_lines, a_lines;
      size_t bytes_per_row = 0;
      switch (width) {
         case 1: print_bits(e_lines, a_lines, bytes_per_row = 4); break;
         case 8: print_ints<unsigned char>(e_lines, a_lines, bytes_per_row = (h2_shell::I().cww < 108 ? 8 : 16)); break;
         case 16: print_ints<unsigned short>(e_lines, a_lines, bytes_per_row = 16); break;
         case 32: print_ints<unsigned int>(e_lines, a_lines, bytes_per_row = 16); break;
         case 64: print_ints<unsigned long long>(e_lines, a_lines, bytes_per_row = 16); break;
         default: break;
      }
      h2_color::printl(h2_layout::split(e_lines, a_lines, "expect", "actual", bytes_per_row * 8 / width, 'x', h2_shell::I().cww));
   }

   const char* format_width()
   {
      switch (width) {
         case 1: return "bits";
         case 8: return "bytes";
         case 16: return "uint16";
         case 32: return "uint32";
         case 64: return "uint64";
      }
      return "";
   }

   void print_bits(h2_lines& e_lines, h2_lines& a_lines, size_t bytes_per_row)
   {
      size_t rows = (size_t)::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (size_t i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (size_t j = 0; j < bytes_per_row; ++j) {
            if (j) e_line.push_back(" ");
            if (j) a_line.push_back(" ");
            for (size_t k = 0; k < 8; ++k) {
               if ((i * bytes_per_row + j) * 8 + k < length) {
                  unsigned char e_val = (e_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  unsigned char a_val = (a_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  if (e_val == a_val) {
                     e_line.push_back(h2_string(e_val ? "1" : "0"));
                     a_line.push_back(h2_string(a_val ? "1" : "0"));
                  } else {
                     e_line.printf("green", e_val ? "1" : "0");
                     a_line.printf("bold,red", a_val ? "1" : "0");
                  }
               }
            }
         }
         e_lines.push_back(e_line);
         a_lines.push_back(a_line);
      }
   }

   template <typename T>
   void print_ints(h2_lines& e_lines, h2_lines& a_lines, size_t bytes_per_row)
   {
      char fmt[32];
      sprintf(fmt, "%%s%%0%dX", (int)sizeof(T) * 2);

      size_t rows = (size_t)::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (size_t i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (size_t j = 0; j < bytes_per_row; j += sizeof(T)) {
            if (i * bytes_per_row + j < e_value.size()) {
               T e_val = *(T*)(e_value.data() + (i * bytes_per_row + j));
               T a_val = *(T*)(a_value.data() + (i * bytes_per_row + j));
               char e_str[32], a_str[32];
               sprintf(e_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", e_val);
               sprintf(a_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", a_val);
               if (e_val == a_val) {
                  e_line.push_back(e_str);
                  a_line.push_back(a_str);
               } else {
                  e_line.printf("green", e_str);
                  a_line.printf("bold,red", a_str);
               }
            }
         }
         e_lines.push_back(e_line);
         a_lines.push_back(a_line);
      }
   }
};

struct h2_fail_memory : h2_fail {
   const void* ptr;
   const size_t size;
   const h2_backtrace bt_allocate, bt_release;
   h2_fail_memory(const void* ptr_, const size_t size_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_fs& fs_ = h2_fs()) : h2_fail({}, fs_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   const h2_vector<std::pair<size_t, size_t>> sizes;
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, const size_t size_, const h2_vector<std::pair<size_t, size_t>>& sizes_, const h2_backtrace& bt_allocate_, const char* where_, const h2_fs& fs_) : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), fs_), sizes(sizes_), where(where_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line = h2_stringify(ptr) + color(" memory leak ", "bold,red") + h2_stringify(size).brush("red") + " ";
      size_t i = 0, c = 0, n = 3;
      h2_line sl;
      for (auto& p : sizes) {
         sl += gray(comma_if(i++));
         if (O.verbose <= 1 && n < i) {
            sl += gray("..." + h2_stringify(sizes.size() - n));
            break;
         }
         sl += h2_stringify(p.first);
         if (1 < p.second) sl += gray("x") + h2_stringify(p.second);
         c += p.second;
      }
      if (1 < c) line += gray("[") + sl + gray("] ");
      h2_color::printl(" " + line + "bytes in " + where + " totally" + locate());
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_color::prints("", " %p", ptr);
      h2_color::prints("bold,red", " double free");
      h2_color::prints("", " at backtrace:\n", ptr), bt_double_free.print(2);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::prints("", "  already free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_color::prints("", " %p allocate with ", ptr);
      h2_color::prints("bold,red", "%s", who_allocate);
      h2_color::prints("", ", release by ");
      h2_color::prints("bold,red", "%s", who_release);
      h2_color::prints("", " asymmetrically at backtrace:\n"), bt_release.print(2);
      if (0 < bt_allocate.count) h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_overflow : h2_fail_memory {
   const void* violate_ptr;             /* 犯罪地点 */
   const char* action;                  /* 犯罪行为 */
   const h2_vector<unsigned char> spot; /* 犯罪现场 */
   const h2_backtrace bt_trample;       /* 犯罪过程 */
   h2_fail_overflow(const void* ptr_, const size_t size_, const void* violate_ptr_, const char* action_, const h2_vector<unsigned char>& spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_) : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace()), violate_ptr(violate_ptr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      long long offset = ptr < violate_ptr ? (long long)violate_ptr - ((long long)ptr + size) : (long long)violate_ptr - (long long)ptr;
      h2_line t = h2_stringify(ptr) + " " + color(h2_string("%+d", (int)offset), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + " " + (offset >= 0 ? "overflow" : "underflow") + " ";
      for (size_t i = 0; i < spot.size(); ++i) t.printf("bold,red", "%02X ", spot[i]);
      h2_color::printl(" " + t + locate() + (bt_trample.count ? " at backtrace:" : ""));
      if (bt_trample.count) bt_trample.print(3);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_use_after_free : h2_fail_memory {
   const void* violate_ptr;   /* 犯罪地点 */
   const char* action;        /* 犯罪行为 */
   const h2_backtrace bt_use; /* 犯罪过程 */
   h2_fail_use_after_free(const void* ptr_, const void* violate_ptr_, const char* action_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_use_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), violate_ptr(violate_ptr_), action(action_), bt_use(bt_use_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line t = h2_stringify(ptr) + " " + color(h2_string("%+d", (long long)violate_ptr - (long long)ptr), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + color(" after free", "bold,red");
      h2_color::printl(" " + t + " at backtrace:"), bt_use.print(2);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::prints("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_exception : h2_fail {
   const char* type;
   const h2_backtrace bt_throw;
   h2_fail_exception(const h2_line& explain_, const char* type_, const h2_backtrace& bt_throw_) : h2_fail(explain_, h2_fs()), type(type_), bt_throw(bt_throw_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_color::printl(" exception " + color(type, "red") + " " + explain + " at backtrace:");
      bt_throw.print(3);
   }
};

struct h2_fail_symbol : h2_fail {
   const h2_string symbol;
   const h2_vector<h2_string> candidates;
   h2_fail_symbol(const h2_string& symbol_, const h2_vector<h2_string>& candidates_, const h2_line& explain_) : h2_fail(explain_, h2_fs()), symbol(symbol_), candidates(candidates_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_color::printl(color(candidates.size() ? " Find multiple " : " Not found ", "yellow") + color(symbol, "bold,red"));
      for (size_t i = 0; i < candidates.size(); ++i)
         h2_color::printl("  " + gray(h2_stringify(i) + ". ") + color(candidates[i], "yellow"));
      if (explain.width()) h2_color::printl(explain);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_line& explain_, const h2_fs& fs_) { return new h2_fail_normal(explain_, fs_); }
h2_inline h2_fail* h2_fail::new_unexpect(const h2_line& expection_, const h2_line& represent_, const h2_line& explain_) { return new h2_fail_unexpect(expection_, represent_, explain_); }
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strcmp(e_value, a_value, caseless, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strfind(e_value, a_value, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, bool caseless, const h2_line& explain_) { return new h2_fail_json(e_value, a_value, expection_, caseless, explain_); }
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t length, const size_t width) { return new h2_fail_memcmp(e_value, a_value, length, width); }
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const h2_fs& fs_) { return new h2_fail_memory_leak(ptr, size, sizes, bt_allocate, where, fs_); }
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free) { return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free); }
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release) { return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release); }
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample) { return new h2_fail_overflow(ptr, size, violate_ptr, action, spot, bt_allocate, bt_trample); }
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use) { return new h2_fail_use_after_free(ptr, violate_ptr, action, bt_allocate, bt_release, bt_use); }
h2_inline h2_fail* h2_fail::new_exception(const char* explain_, const char* type, const h2_backtrace& bt_throw) { return new h2_fail_exception(explain_, type, bt_throw); }
h2_inline h2_fail* h2_fail::new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_line& explain_) { return new h2_fail_symbol(symbol, candidates, explain_); };
// source/render/h2_report.cpp
struct h2_report_impl {
   h2_list x;
   int suites = 0, cases = 0;
   int suite_index = 0, suite_case_index = 0, runner_case_index = 0;
   long long runner_cost, suite_cost, case_cost;

   virtual void on_runner_start(h2_runner* r)
   {
      suites = r->suites.count();
      h2_list_for_each_entry (s, r->suites, h2_suite, x)
         cases += s->cases.count();
      runner_cost = h2_now();
   }
   virtual void on_runner_endup(h2_runner* r)
   {
      runner_cost = h2_now() - runner_cost;
   }
   virtual void on_suite_start(h2_suite* s)
   {
      suite_case_index = 0;
      suite_cost = h2_now();
   }
   virtual void on_suite_endup(h2_suite* s)
   {
      suite_cost = h2_now() - suite_cost;
   }
   virtual void on_case_start(h2_suite* s, h2_case* c)
   {
      ++suite_case_index;
      case_cost = h2_now();
   }
   virtual void on_case_endup(h2_suite* s, h2_case* c)
   {
      ++runner_case_index;
      case_cost = h2_now() - case_cost;
   }
};

struct h2_report_list : h2_report_impl {
   int unfiltered_suite_index = 0, unfiltered_suite_case_index = 0, unfiltered_runner_case_index = 0;

   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      h2_color::prints("bold,green", "Listing <%d suites, %d cases", unfiltered_suite_index, unfiltered_runner_case_index);
      if (runner_case_index - unfiltered_runner_case_index) h2_color::prints("bold,green", ", %d filtered", runner_case_index - unfiltered_runner_case_index);
      h2_color::prints("bold,green", ">\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      unfiltered_suite_case_index = 0;
      if (!s->name) return;  // CASE
      if (s->filtered) return;
      ++unfiltered_suite_index;
      if (O.list_cases & 1) {
         h2_color::prints("dark gray", "SUITE-%d. ", unfiltered_suite_index);
         h2_color::prints("bold,blue", "%s", s->name);
         h2_color::prints("dark gray", " %s:%d\n", s->fs.file, s->fs.line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (s->filtered) return;
      const char* type = nullptr;
      if (c->todo) {
         if (O.list_cases & 4) type = s->name ? "Todo" : "TODO";
      } else {
         if (O.list_cases & 2) type = s->name ? "Case" : "CASE";
      }

      if (type) {
         ++unfiltered_runner_case_index, ++unfiltered_suite_case_index;
         if (O.list_cases & 1)
            h2_color::prints("dark gray", " %s/%d-%d. ", type, unfiltered_suite_case_index, unfiltered_runner_case_index);
         else
            h2_color::prints("dark gray", " %s-%d. ", type, unfiltered_runner_case_index);
         h2_color::prints("cyan", "%s", c->name);
         h2_color::prints("dark gray", " %s:%d\n", c->fs.basefile(), c->fs.line);
      }
   }
};

struct h2_report_console : h2_report_impl {
   void comma_status(int n, const char* style, const char* name, int& c)
   {
      if (c++) h2_color::prints("dark gray", ", ");
      h2_color::prints(style, "%d", n);
      h2_color::prints("", " %s", name);
   }
   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   h2_line format_title(const char* suite_name, const char* case_name, const char* file, int line)
   {
      h2_line title;
      title.printf("dark gray", "┊ ");
      if (strlen(case_name))
         title.printf("", "%s ", case_name);
      else
         title.printf("dark gray", "case ");
      if (suite_name) {
         title.printf("dark gray", "┊ ");
         if (strlen(suite_name))
            title.printf("", "%s ", suite_name);
         else
            title.printf("dark gray", "suite ");
      }
      if (file) {
         title.printf("dark gray", "┊ ");
         title.printf("", "%s:%d ", file, line);
      } else {
         title = title.abbreviate(h2_shell::I().cww - 20);
      }
      return title;
   }
   void format_percentage(h2_line& bar)
   {
      bar.printf("dark gray", "[");
      bar.printf("", "%3d%%", cases ? (int)(runner_case_index * 100 / cases) : 100);
      bar.printf("dark gray", "] ");
   }
   void print_bar(bool percentage, const char* status_style, const char* status, h2_suite* s, h2_case* c, bool returnable)
   {
      if (!O.progressing && returnable) return;
      if (h2_report::I().escape_length == h2_stdio::I().capture_length)
         ::printf("\33[2K\r"); /* clear line */
      else
         ::printf("\n"); /* user output, new line */
      h2_line bar;
      if (percentage && O.progressing) format_percentage(bar);
      if (status && status_style) bar.printf(status_style, "%s", status);
      if (s && c) bar += format_title(s->name, c->name, returnable ? nullptr : c->fs.basefile(), c->fs.line);
      h2_color::printl(bar, false);
      if (returnable) h2_report::I().escape_length = h2_stdio::I().capture_length;
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      print_bar(false, nullptr, nullptr, nullptr, nullptr, false);
      if (0 < r->stats.failed)
         h2_color::prints("bold,red", "Failure ");
      else
         h2_color::prints("bold,green", "Success ");

      if (0 < nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored))
         h2_color::prints("dark gray", "(");

      int c = 0;
      comma_status(r->stats.passed, "green", "passed", c);
      if (r->stats.failed) comma_status(r->stats.failed, "red", "failed", c);
      if (r->stats.todo) comma_status(r->stats.todo, "yellow", "todo", c);
      if (r->stats.filtered) comma_status(r->stats.filtered, "blue", "filtered", c);
      if (r->stats.ignored) comma_status(r->stats.ignored, "blue", "ignored", c);
      if (0 < nonzero_count(r->stats.failed, r->stats.todo, r->stats.filtered, r->stats.ignored)) {
         h2_color::prints("dark gray", ")");
         h2_color::prints("", " %d", cases);
      }
      h2_color::prints("", " case%s", 1 < cases ? "s" : "");
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%d assert%s", r->stats.asserts, 1 < r->stats.asserts ? "s" : "");
      if (1 < r->rounds) {
         h2_color::prints("dark gray", ", ");
         h2_color::prints("", "%d rounds", r->rounds);
      }
      h2_color::prints("dark gray", ", ");
      h2_color::prints("", "%s \n", format_duration(runner_cost));
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (O.verbose >= 9 && O.includes.size() + O.excludes.size() == 0) {
         print_bar(false, nullptr, nullptr, nullptr, nullptr, false);
         h2_color::prints("dark gray", "suite ");
         h2_color::prints("", "%s", ss(s->name));
         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_color::prints("dark gray", " (");
         else
            h2_color::prints("dark gray", " - ");

         int c = 0;
         if (s->stats.passed) comma_status(s->stats.passed, "", "passed", c);
         if (s->stats.failed) comma_status(s->stats.failed, "", "failed", c);
         if (s->stats.todo) comma_status(s->stats.todo, "", "todo", c);
         if (s->stats.filtered) comma_status(s->stats.filtered, "", "filtered", c);
         if (s->stats.ignored) comma_status(s->stats.ignored, "", "ignored", c);

         if (1 < nonzero_count(s->stats.passed, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored))
            h2_color::prints("dark gray", ")");
         if (0 < s->cases.count())
            h2_color::prints("", " case%s", 1 < s->cases.count() ? "s" : "");

         if (0 < s->stats.asserts) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%d assert%s", s->stats.asserts, 1 < s->stats.asserts ? "s" : "");
         }
         if (0 < s->stats.footprint) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%s footprint", format_volume(s->stats.footprint));
         }
         if (1 < suite_cost) {
            h2_color::prints("dark gray", ", ");
            h2_color::prints("", "%s", format_duration(suite_cost));
         }
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_start(s, c);
      if (c->filtered || c->ignored || c->todo) return;
      print_bar(true, "light blue", "Testing", s, c, true);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (c->filtered || c->ignored) return;
      if (c->todo) {
         if (O.verbose >= 3) print_bar(true, "yellow", s->name ? "Todo   " : "TODO   ", s, c, false);
      } else if (c->failed) {
         if (O.verbose >= 1) {
            print_bar(true, "bold,red", "Failed ", s, c, false);
            if (O.verbose >= 2) {
               h2_color::prints("", "\n");
               if (c->fails) c->fails->foreach([](h2_fail* fail, size_t si, size_t ci) { fail->print(si, ci); });
            }
         }
      } else {  // Passed
         if (O.verbose >= 3) {
            print_bar(true, "green", "Passed ", s, c, false);
            h2_line ad;
            if (0 < c->asserts) ad.printf("dark gray", ad.width() ? ", " : "").printf("", "%d assert%s", c->asserts, 1 < c->asserts ? "s" : "");
            if (0 < c->footprint) ad.printf("dark gray", ad.width() ? ", " : "").printf("", "%s footprint", format_volume(c->footprint));
            if (0 < case_cost) ad.printf("dark gray", ad.width() ? ", " : "").printf("", "%s", format_duration(case_cost));
            if (ad.width()) h2_color::printl(gray("- ") + ad, false);
         }
      }
   }
};

struct h2_report_junit : h2_report_impl {
   FILE* f;
   void on_runner_start(h2_runner* r) override
   {
      h2_report_impl::on_runner_start(r);
      f = fopen(O.junit_path, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      h2_report_impl::on_suite_start(s);
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats.failed, s->name, s->stats.todo + s->stats.filtered, s->cases.count(), 0, "");
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      h2_report_impl::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, c->todo ? "TODO" : (c->filtered ? "Filtered" : (c->ignored ? "Ignored" : (c->failed ? "Failed" : "Passed"))), case_cost / 1000.0);
      if (c->failed) {
         fprintf(f, "<failure message=\"%s:%d:", c->fs.file, c->fs.line);
         if (c->fails) c->fails->foreach([&](h2_fail* fail, size_t si, size_t ci) {fprintf(f, "{newline}"); fail->print(f); });
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "<system-out></system-out><system-err></system-err>\n");
      fprintf(f, "</testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override
   {
      h2_report_impl::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_runner_endup(h2_runner* r) override
   {
      h2_report_impl::on_runner_endup(r);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_report_tap : h2_report_impl {
   /* */
};

h2_inline void h2_report::initialize()
{
   static h2_report_list list_report;
   static h2_report_console console_report;
   static h2_report_junit junit_report;
   static h2_report_tap tap_report;
   if (O.list_cases) {
      I().reports.push_back(list_report.x);
   } else {
      I().reports.push_back(console_report.x);
      if (strlen(O.junit_path)) I().reports.push_back(junit_report.x);
      if (strlen(O.tap_path)) I().reports.push_back(tap_report.x);
   }
}

struct in_report {
   in_report() { h2_report::I().in = true; }
   ~in_report() { h2_report::I().in = false; }
};
/* clang-format off */
h2_inline void h2_report::on_runner_start(h2_runner* r) { in_report t; h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_runner_start(r); }
h2_inline void h2_report::on_runner_endup(h2_runner* r) { in_report t; h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_runner_endup(r); }
h2_inline void h2_report::on_suite_start(h2_suite* s) { in_report t; h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_suite_start(s); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { in_report t; h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_suite_endup(s); }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { in_report t; h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_case_start(s, c); }
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c) { in_report t; h2_list_for_each_entry (p, reports, h2_report_impl, x) p->on_case_endup(s, c); }
// source/render/h2_layout.cpp
static inline h2_lines line_break(const h2_line& line, size_t width)
{
   h2_lines lines;
   h2_string current_style;
   h2_line wrap;

   for (auto& word : line) {
      if (h2_color::isctrl(word.c_str())) {  // + - style , issue
         wrap.push_back(word);
         current_style = word;
         continue;
      }
      for (auto& c : word.disperse()) {
         if (width < wrap.width() + c.width()) {
            lines.push_back(wrap.padding(width - wrap.width()));
            wrap.clear();
            if (current_style.size()) wrap.push_back(current_style);
         }
         wrap.push_back(c);
      }
   }
   lines.push_back(wrap.padding(width - wrap.width()));
   return lines;
}

static inline h2_lines lines_merge(const h2_lines& left_lines, const h2_lines& right_lines, size_t left_width, size_t right_width, int step, char scale, size_t seq_width)
{
   h2_lines lines;
   char seq_fmt[32];
   sprintf(seq_fmt, "%%%d%c┊ ", (int)seq_width, scale);
   const h2_string left_empty(left_width, ' '), right_empty(right_width, ' ');
   for (size_t i = 0; i < std::max(left_lines.size(), right_lines.size()); ++i) {
      auto left_wrap_lines = line_break(i < left_lines.size() ? left_lines[i] : left_empty, left_width);
      auto right_wrap_lines = line_break(i < right_lines.size() ? right_lines[i] : right_empty, right_width);
      for (size_t j = 0; j < std::max(left_wrap_lines.size(), right_wrap_lines.size()); ++j) {
         h2_line line;
         if (step) {
            if (j == 0)
               line.printf("dark gray", seq_fmt, step * i);
            else
               line.indent(seq_width + 2);
         }
         line += j < left_wrap_lines.size() ? left_wrap_lines[j].brush("reset") : color(left_empty, "reset");
         line.printf("dark gray", j < left_wrap_lines.size() - 1 ? "\\│ " : " │ ");
         line += j < right_wrap_lines.size() ? right_wrap_lines[j].brush("reset") : color(right_empty, "reset");
         line.printf("dark gray", j < right_wrap_lines.size() - 1 ? "\\" : " ");
         lines.push_back(line);
      }
   }
   return lines;
}

h2_inline h2_lines h2_layout::split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, size_t step, char scale, size_t width)
{
   size_t seq_width = number_strlen((unsigned long long)step * std::max(left_lines.size(), right_lines.size()), scale == 'x' ? 16 : 10);
   size_t valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

   size_t left_width = std::max(left_lines.width(), strlen(left_title));
   size_t right_width = std::max(right_lines.width(), strlen(right_title));

   if (left_width < valid_width / 2)
      right_width = std::min(valid_width - left_width, right_width);
   else if (right_width < valid_width / 2)
      left_width = std::min(valid_width - right_width, left_width);
   else
      left_width = right_width = valid_width / 2;

   h2_line title = gray((step ? h2_string(seq_width + 2, ' ') : "") + h2_string(left_title).center(left_width)) + "   " + gray(h2_string(right_title).center(right_width));
   h2_lines lines = {title};

   return lines += lines_merge(left_lines, right_lines, left_width, right_width, step, scale, seq_width);
}

h2_inline h2_lines h2_layout::unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
{
   h2_lines lines;
   h2_line up_title_line = gray(up_title) + color("> ", "green");
   h2_line down_title_line = gray(down_title) + color("> ", "red");

   h2_lines up_lines = line_break(up_line, width - up_title_line.width());
   h2_lines down_lines = line_break(down_line, width - down_title_line.width());

   for (size_t i = 0; i < std::max(up_lines.size(), down_lines.size()); ++i) {
      if (i < up_lines.size()) lines.push_back(up_title_line + up_lines[i]);
      if (i < down_lines.size()) lines.push_back(down_title_line + down_lines[i]);
   }
   return lines;
}

static inline h2_lines prefix_break(const h2_line& line, const h2_line& title, size_t width)
{
   h2_lines lines = line_break(line, width - title.width());

   for (size_t i = 0; i < lines.size(); ++i) {
      if (i == 0)
         lines[i] = title + lines[i];
      else
         lines[i].indent(title.width());
   }
   return lines;
}

h2_inline h2_lines h2_layout::seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
{
   h2_line up_title_line = gray(up_title) + color("> ", "green");
   h2_line down_title_line = gray(down_title) + color("> ", "red");
   h2_lines lines = prefix_break(up_line, up_title_line, width);
   return lines += prefix_break(down_line, down_title_line, width);
}
// source/render/h2_option.cpp
/* clang-format off */
static inline void usage()
{
#define H2_USAGE_SP "\033[90m│\033[0m"
#define H2_USAGE_BR "\033[90m├─────┼───────────┼────────────────────────────────────────────────────────────┤\033[0m\n"
   char b[] = "\033[90m┌─────┬───────────┬────────────────────────────────────────────────────────────┐\033[0m\n"
              H2_USAGE_SP " -\033[36ma\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36ma\033[0mttend to last failed cases                                " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mb\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mb\033[0mreak test once n (default 1) cases failed                 " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mc\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Output in black-white \033[36mc\033[0molor style                          " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36md\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36md\033[0mebug with gdb once failure occurred                       " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mf\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mf\033[0mold JSON object or array, bigger n more folded            " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mi\033[0m/\033[36me\033[0m" H2_USAGE_SP "\033[90m[\033[0mpattern .\033[90m]\033[0m" H2_USAGE_SP " \033[36mi\033[0mnclude/\033[36me\033[0mxclude case, suite or file by substr/wildcard     " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mj\033[0m  " H2_USAGE_SP "  \033[90m[\033[0mpath\033[90m]\033[0m   " H2_USAGE_SP " Generate \033[36mj\033[0munit report, default is <executable>.junit.xml   " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36ml\033[0m  " H2_USAGE_SP "  \033[90m[\033[0mtype .\033[90m]\033[0m " H2_USAGE_SP " \033[36ml\033[0mist out suites and cases, type [suite case todo]          " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mm\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Test cases without \033[36mm\033[0memory check                            " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mp\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Disable test percentage \033[36mp\033[0mrogressing bar                    " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mr\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " Repeat test n (default 2) \033[36mr\033[0mounds                           " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36ms\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " \033[36ms\033[0mhuffle cases then test in random order if no last failed  " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mv\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " \033[36mv\033[0merbose, 0:compact 2:normal 4:abbreviate default:all       " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36mx\033[0m  " H2_USAGE_SP "           " H2_USAGE_SP " Thrown e\033[36mx\033[0mception is considered as failure                  " H2_USAGE_SP "\n" H2_USAGE_BR
              H2_USAGE_SP " -\033[36my\033[0m  " H2_USAGE_SP "    \033[90m[\033[0mn\033[90m]\033[0m    " H2_USAGE_SP " Cop\033[36my\033[0m-paste-able C/C++ source code formatted JSON           " H2_USAGE_SP "\n"
              "\033[90m└─────┴───────────┴────────────────────────────────────────────────────────────┘\033[0m\n";

   ::printf(" \033[90mhttps://github.com/lingjf/\033[0m\033[32mh2unit\033[0m \033[90mv\033[0m%-5s \n%s", H2PP_STR(H2UNIT_VERSION), b);
}
/* clang-format on */

struct getopt {
   int argc;
   const char** args;  // argv + 1
   int i = -1;
   const char* j = nullptr;

   getopt(int argc_, const char** args_) : argc(argc_), args(args_) {}

   const char* extract_next() { return ++i < argc ? args[i] : nullptr; }
   const char* extract_string() { return (i + 1 < argc && args[i + 1] && args[i + 1][0] != '-') ? args[++i] : nullptr; }

   const char next_option()
   {
      do {
         for (; j && *++j;) return *j;
         for (; (j = extract_next()) && j[0] != '-';) {}
      } while (j);
      return '\0';
   }

   void extract_number(int& value)
   {
      if (j) {  // j always not null
         auto l = strspn(j + 1, "0123456789");
         if (l) {
            value = atoi(j + 1);
            j += l;
            return;
         }
      }

      if (i + 1 < argc) {
         auto l = strlen(args[i + 1]);
         if (l && strspn(args[i + 1], "0123456789") == l)
            value = atoi(args[++i]);
      }
   }

   void arguments(char* s)
   {
      for (int k = 0; k < argc; ++k)
         s += sprintf(s, " %s", args[k]);
   }
};

h2_inline void h2_option::parse(int argc, const char** argv)
{
   path = argv[0];
   getopt get(argc - 1, argv + 1);
   get.arguments(args);

   for (const char* t;;) {
      switch (get.next_option()) {
         case '\0': return;
         case 'a': last_failed = true; break;
         case 'b': break_after_fails = 1, get.extract_number(break_after_fails); break;
         case 'c': colorful = !colorful; break;
         case 'd': debug = true; break;
         case 'e':
            while ((t = get.extract_string())) excludes.push_back(t);
            break;
         case 'f': fold_json = 0, get.extract_number(fold_json); break;
         case 'i':
            while ((t = get.extract_string())) includes.push_back(t);
            break;
         case 'j':
            sprintf(junit_path, "%s.junit.xml", path);
            if ((t = get.extract_string())) strcpy(junit_path, t);
            break;
         case 'l':
            while ((t = get.extract_string())) {
               if (!strcasecmp("suite", t)) list_cases |= 1;
               if (!strcasecmp("case", t)) list_cases |= 2;
               if (!strcasecmp("todo", t)) list_cases |= 4;
            }
            if (!list_cases) list_cases = 1 | 2 | 4;
            break;
         case 'm': memory_check = !memory_check; break;
         case 'p': progressing = !progressing; break;
         case 'r': run_rounds = 2, get.extract_number(run_rounds); break;
         case 's': shuffle_cases = true; break;
         case 'v': verbose = 8, get.extract_number(verbose); break;
         case 'x': exception_as_fail = true; break;
         case 'y': copy_paste_json = 3, get.extract_number(copy_paste_json); break;
         case 'h':
         case '?': usage(); exit(0);
      }
   }
   if (list_cases) memory_check = false;
}

static inline bool match3(const std::vector<const char*>& patterns, const char* subject)
{
   for (auto pattern : patterns) {
      if (strcasestr(subject, pattern)) return true;
      if (strcspn(pattern, "?*[]") < strlen(pattern) && h2_pattern::wildcard_match(pattern, subject, true)) return true;
      // if (strcspn(pattern, "?*[]+^$\\.") < strlen(pattern) && h2_pattern::regex_match(pattern, subject, true)) return true;
   }
   return false;
}

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* file, int line) const
{
   char filename[1024];
   sprintf(filename, "%s:%d", file, line);
   if (!includes.empty())
      if (!match3(includes, suitename) && !match3(includes, casename) && !match3(includes, filename))
         return true;
   if (!excludes.empty())
      if (match3(excludes, suitename) || match3(excludes, casename) || match3(excludes, filename))
         return true;
   return false;
}
}
#endif
