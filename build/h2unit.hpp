
/* v5.9 2021-06-17 21:10:07 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#ifndef __H2UNIT_HPP__
#define __H2UNIT_HPP__

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
#define H2PP_MAKE_CALL(_Macro, _Args) _Macro _Args

#define _H2PP_2ND(_1, _2, ...) _2
#ifdef _MSC_VER  // MSVC workaround
#   define H2PP_IS_PROBE(...) H2PP_MAKE_CALL(_H2PP_2ND, (__VA_ARGS__, 0))
#else
#   define H2PP_IS_PROBE(...) _H2PP_2ND(__VA_ARGS__, 0, )
#endif
#define H2PP_PROBE() ~, 1,

#define H2PP_EVAL(...) _H2PP_EVAL_128(__VA_ARGS__)
#define H2PP_RS1(...) _H2PP_RESCAN1_128(__VA_ARGS__)
#define H2PP_RS2(...) _H2PP_RESCAN2_128(__VA_ARGS__)
#define H2PP_RS3(...) _H2PP_RESCAN3_128(__VA_ARGS__)
#define H2PP_RS4(...) _H2PP_RESCAN4_128(__VA_ARGS__)

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

#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_CAT2(_Macro, H2PP_NARG(__VA_ARGS__)) (__VA_ARGS__)

#define H2PP_HEAD(...) H2PP_CAT2(_H2PP_HEAD_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_HEAD_0(_1, ...) _1
#define _H2PP_HEAD_1(...)

#define H2PP_TAIL(...) H2PP_CAT2(_H2PP_TAIL_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_TAIL_0(_1, ...) __VA_ARGS__
#define _H2PP_TAIL_1(...)

#define H2PP_LAST(...) H2PP_RS1(_H2PP_LAST(__VA_ARGS__))
#define _H2PP_LAST(...) H2PP_CAT2(_H2PP_LAST_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__) //is empty?
#define _H2PP_LAST_1(...) // 0 argument
#define _H2PP_LAST_0(...) H2PP_CAT2(_H2PP_LAST_0_, H2PP_EQ(1, H2PP_NARG(__VA_ARGS__))) (__VA_ARGS__) // at least 1 argument, one and only one?
#define _H2PP_LAST_0_1(...) __VA_ARGS__ // only 1 argument
#define _H2PP_LAST_0_0(...) H2PP_DEFER(__H2PP_LAST)()(H2PP_TAIL(__VA_ARGS__)) // shift first, and again
#define __H2PP_LAST() _H2PP_LAST_0

#define H2PP_REPEAT(_Split, _Macro, _Args, _Count) H2PP_RS2(_H2PP_REPEAT(_Split, _Macro, _Args, _Count))
#define _H2PP_REPEAT(_Split, _Macro, _Args, _Count) H2PP_CAT2(_H2PP_REPEAT_, H2PP_BOOL(_Count)) (_Split, _Macro, _Args, _Count)
#define _H2PP_REPEAT_0(...)
#define _H2PP_REPEAT_1(_Split, _Macro, _Args, _Count) H2PP_DEFER(__H2PP_REPEAT)()(_Split, _Macro, _Args, H2PP_CAT2(H2PP_DEC_, _Count)) _H2PP_REPEAT_CALL_MACRO(_Split, _Macro, _Args, H2PP_CAT2(H2PP_DEC_, _Count))
#define __H2PP_REPEAT() _H2PP_REPEAT
#define _H2PP_REPEAT_CALL_MACRO(_Split, _Macro, _Args, _i) H2PP_CAT2(_H2PP_REPEAT_CALL_MACRO_, H2PP_EQ(0, _i)) (_Split, _Macro, _Args, _i)
#define _H2PP_REPEAT_CALL_MACRO_1(_Split, _Macro, _Args, _i) _Macro(_Args, _i)
#define _H2PP_REPEAT_CALL_MACRO_0(_Split, _Macro, _Args, _i) H2PP_REMOVE_PARENTHESES_IF(_Split) _Macro(_Args, _i)

#define H2PP_FOREACH(_Split, _Macro, _Args, ...) H2PP_RS3(_H2PP_FOREACH(_Split, _Macro, _Args, 0, __VA_ARGS__))
#define _H2PP_FOREACH(_Split, _Macro, _Args, _i, ...) H2PP_CAT2(_H2PP_FOREACH_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Split, _Macro, _Args, _i, __VA_ARGS__)
#define _H2PP_FOREACH_1(...)
#define _H2PP_FOREACH_0(_Split, _Macro, _Args, _i, ...) _H2PP_FOREACH_CALL_MACRO(_Split, _Macro, _Args, _i, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(__H2PP_FOREACH)()(_Split, _Macro, _Args, H2PP_CAT2(H2PP_INC_, _i), H2PP_TAIL(__VA_ARGS__))
#define __H2PP_FOREACH() _H2PP_FOREACH
#define _H2PP_FOREACH_CALL_MACRO(_Split, _Macro, _Args, _i, _x) H2PP_CAT2(_H2PP_FOREACH_CALL_MACRO_, H2PP_EQ(0, _i)) (_Split, _Macro, _Args, _i, _x)
#define _H2PP_FOREACH_CALL_MACRO_1(_Split, _Macro, _Args, _i, _x) _Macro(_Args, _i, _x)
#define _H2PP_FOREACH_CALL_MACRO_0(_Split, _Macro, _Args, _i, _x) H2PP_REMOVE_PARENTHESES_IF(_Split) _Macro(_Args, _i, _x)

#define H2PP_FULLMESH(_Split, _Macro, _Args, t, ...) H2PP_CAT2(_H2PP_FULLMESH_, H2PP_AND(H2PP_IS_BEGIN_PARENTHESIS(t), H2PP_EQ(1, H2PP_NARG(__VA_ARGS__)))) (_Split, _Macro, _Args, t, __VA_ARGS__)
#define _H2PP_FULLMESH_0 H2PP_FULLMESH1 // Fullmesh(1,2,3) => (1,2,3) x (1,2,3)
#define _H2PP_FULLMESH_1 H2PP_FULLMESH2 // Fullmesh((1,2,3), (4,5,6)) => (1,2,3) x (4,5,6)
#define H2PP_FULLMESH1(_Split, _Macro, _Args, ...) H2PP_FULLMESH2(_Split, _Macro, _Args, (__VA_ARGS__), (__VA_ARGS__))
#define H2PP_FULLMESH2(_Split, _Macro, _Args, _Tuplex, _Tupley) H2PP_RS4(_H2PP_FOREACHx(_Split, _Macro, _Args, 0, _Tupley, H2PP_REMOVE_PARENTHESES(_Tuplex)))
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

#define H2PP_UNIQUE(...) H2PP_CAT5(__VA_ARGS__, _C, __COUNTER__, L, __LINE__)  // generate unique identifier [with(out) prefix]

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
#define H2PP_TH0(...) _H2PP_TH0(__VA_ARGS__, )
#define H2PP_TH1(...) _H2PP_TH1(__VA_ARGS__, , )
#define H2PP_TH2(...) _H2PP_TH2(__VA_ARGS__, , , )
#define H2PP_TH3(...) _H2PP_TH3(__VA_ARGS__, , , , )
#define H2PP_TH4(...) _H2PP_TH4(__VA_ARGS__, , , , , )
#define H2PP_TH5(...) _H2PP_TH5(__VA_ARGS__, , , , , , )
#define H2PP_TH6(...) _H2PP_TH6(__VA_ARGS__, , , , , , , )
#define H2PP_TH7(...) _H2PP_TH7(__VA_ARGS__, , , , , , , , )
#define H2PP_TH8(...) _H2PP_TH8(__VA_ARGS__, , , , , , , , , )
#define H2PP_TH9(...) _H2PP_TH9(__VA_ARGS__, , , , , , , , , , )
#define H2PP_TH10(...) _H2PP_TH10(__VA_ARGS__, , , , , , , , , , , )
#define H2PP_TH11(...) _H2PP_TH11(__VA_ARGS__, , , , , , , , , , , , )
#define H2PP_TH12(...) _H2PP_TH12(__VA_ARGS__, , , , , , , , , , , , , )
#define H2PP_TH13(...) _H2PP_TH13(__VA_ARGS__, , , , , , , , , , , , , , )
#define H2PP_TH14(...) _H2PP_TH14(__VA_ARGS__, , , , , , , , , , , , , , , )
#define H2PP_TH15(...) _H2PP_TH15(__VA_ARGS__, , , , , , , , , , , , , , , , )
#define H2PP_TH16(...) _H2PP_TH16(__VA_ARGS__, , , , , , , , , , , , , , , , , )
#define H2PP_TH17(...) _H2PP_TH17(__VA_ARGS__, , , , , , , , , , , , , , , , , , )
#define H2PP_TH18(...) _H2PP_TH18(__VA_ARGS__, , , , , , , , , , , , , , , , , , , )
#define H2PP_TH19(...) _H2PP_TH19(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH20(...) _H2PP_TH20(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH21(...) _H2PP_TH21(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH22(...) _H2PP_TH22(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH23(...) _H2PP_TH23(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH24(...) _H2PP_TH24(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH25(...) _H2PP_TH25(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH26(...) _H2PP_TH26(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH27(...) _H2PP_TH27(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH28(...) _H2PP_TH28(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH29(...) _H2PP_TH29(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH30(...) _H2PP_TH30(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , )
#define H2PP_TH31(...) _H2PP_TH31(__VA_ARGS__, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , )

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

#define _H2PP_EVAL_1024(...) _H2PP_EVAL_512(_H2PP_EVAL_512(__VA_ARGS__))
#define _H2PP_EVAL_512(...) _H2PP_EVAL_256(_H2PP_EVAL_256(__VA_ARGS__))
#define _H2PP_EVAL_256(...) _H2PP_EVAL_128(_H2PP_EVAL_128(__VA_ARGS__))
#define _H2PP_EVAL_128(...) _H2PP_EVAL_64(_H2PP_EVAL_64(__VA_ARGS__))
#define _H2PP_EVAL_64(...) _H2PP_EVAL_32(_H2PP_EVAL_32(__VA_ARGS__))
#define _H2PP_EVAL_32(...) _H2PP_EVAL_16(_H2PP_EVAL_16(__VA_ARGS__))
#define _H2PP_EVAL_16(...) _H2PP_EVAL_8(_H2PP_EVAL_8(__VA_ARGS__))
#define _H2PP_EVAL_8(...) _H2PP_EVAL_4(_H2PP_EVAL_4(__VA_ARGS__))
#define _H2PP_EVAL_4(...) _H2PP_EVAL_2(_H2PP_EVAL_2(__VA_ARGS__))
#define _H2PP_EVAL_2(...) _H2PP_EVAL_1(_H2PP_EVAL_1(__VA_ARGS__))
#define _H2PP_EVAL_1(...) __VA_ARGS__

#define _H2PP_RESCAN1_128(...) _H2PP_RESCAN1_64(_H2PP_RESCAN1_64(__VA_ARGS__))
#define _H2PP_RESCAN1_64(...) _H2PP_RESCAN1_32(_H2PP_RESCAN1_32(__VA_ARGS__))
#define _H2PP_RESCAN1_32(...) _H2PP_RESCAN1_16(_H2PP_RESCAN1_16(__VA_ARGS__))
#define _H2PP_RESCAN1_16(...) _H2PP_RESCAN1_8(_H2PP_RESCAN1_8(__VA_ARGS__))
#define _H2PP_RESCAN1_8(...) _H2PP_RESCAN1_4(_H2PP_RESCAN1_4(__VA_ARGS__))
#define _H2PP_RESCAN1_4(...) _H2PP_RESCAN1_2(_H2PP_RESCAN1_2(__VA_ARGS__))
#define _H2PP_RESCAN1_2(...) _H2PP_RESCAN1_1(_H2PP_RESCAN1_1(__VA_ARGS__))
#define _H2PP_RESCAN1_1(...) __VA_ARGS__

#define _H2PP_RESCAN2_128(...) _H2PP_RESCAN2_64(_H2PP_RESCAN2_64(__VA_ARGS__))
#define _H2PP_RESCAN2_64(...) _H2PP_RESCAN2_32(_H2PP_RESCAN2_32(__VA_ARGS__))
#define _H2PP_RESCAN2_32(...) _H2PP_RESCAN2_16(_H2PP_RESCAN2_16(__VA_ARGS__))
#define _H2PP_RESCAN2_16(...) _H2PP_RESCAN2_8(_H2PP_RESCAN2_8(__VA_ARGS__))
#define _H2PP_RESCAN2_8(...) _H2PP_RESCAN2_4(_H2PP_RESCAN2_4(__VA_ARGS__))
#define _H2PP_RESCAN2_4(...) _H2PP_RESCAN2_2(_H2PP_RESCAN2_2(__VA_ARGS__))
#define _H2PP_RESCAN2_2(...) _H2PP_RESCAN2_1(_H2PP_RESCAN2_1(__VA_ARGS__))
#define _H2PP_RESCAN2_1(...) __VA_ARGS__

#define _H2PP_RESCAN3_128(...) _H2PP_RESCAN3_64(_H2PP_RESCAN3_64(__VA_ARGS__))
#define _H2PP_RESCAN3_64(...) _H2PP_RESCAN3_32(_H2PP_RESCAN3_32(__VA_ARGS__))
#define _H2PP_RESCAN3_32(...) _H2PP_RESCAN3_16(_H2PP_RESCAN3_16(__VA_ARGS__))
#define _H2PP_RESCAN3_16(...) _H2PP_RESCAN3_8(_H2PP_RESCAN3_8(__VA_ARGS__))
#define _H2PP_RESCAN3_8(...) _H2PP_RESCAN3_4(_H2PP_RESCAN3_4(__VA_ARGS__))
#define _H2PP_RESCAN3_4(...) _H2PP_RESCAN3_2(_H2PP_RESCAN3_2(__VA_ARGS__))
#define _H2PP_RESCAN3_2(...) _H2PP_RESCAN3_1(_H2PP_RESCAN3_1(__VA_ARGS__))
#define _H2PP_RESCAN3_1(...) __VA_ARGS__

#define _H2PP_RESCAN4_128(...) _H2PP_RESCAN4_64(_H2PP_RESCAN4_64(__VA_ARGS__))
#define _H2PP_RESCAN4_64(...) _H2PP_RESCAN4_32(_H2PP_RESCAN4_32(__VA_ARGS__))
#define _H2PP_RESCAN4_32(...) _H2PP_RESCAN4_16(_H2PP_RESCAN4_16(__VA_ARGS__))
#define _H2PP_RESCAN4_16(...) _H2PP_RESCAN4_8(_H2PP_RESCAN4_8(__VA_ARGS__))
#define _H2PP_RESCAN4_8(...) _H2PP_RESCAN4_4(_H2PP_RESCAN4_4(__VA_ARGS__))
#define _H2PP_RESCAN4_4(...) _H2PP_RESCAN4_2(_H2PP_RESCAN4_2(__VA_ARGS__))
#define _H2PP_RESCAN4_2(...) _H2PP_RESCAN4_1(_H2PP_RESCAN4_1(__VA_ARGS__))
#define _H2PP_RESCAN4_1(...) __VA_ARGS__

#define __H2PP_64TH(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, ...) _64
#ifdef _MSC_VER
#   define H2PP_HAS_COMMA(...) H2PP_RESCAN(H2PP_MAKE_CALL(__H2PP_64TH, (__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)))
#   define H2PP_ARG_COUNT(...) H2PP_RESCAN(H2PP_MAKE_CALL(__H2PP_64TH, (__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)))
#else
#   define H2PP_HAS_COMMA(...) __H2PP_64TH(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#   define H2PP_ARG_COUNT(...) __H2PP_64TH(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#endif

//////// <<<<< generated by build/generate.py
// source/utils/h2_template.hpp

template <typename...>
using h2_void_t = void;

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
// source/utils/h2_misc.hpp

struct h2_once {
   unsigned long c = 0;
   operator bool() { return !c++; }
};

struct h2_pattern {
   static bool regex_match(const char* pattern, const char* subject, bool caseless = false);
   static bool wildcard_match(const char* pattern, const char* subject, bool caseless = false);
   static bool match(const char* pattern, const char* subject, bool caseless = false);
};

/* clang-format off */

#define h2_singleton(_Class) static _Class& I() { static _Class i; return i; }

#define H2Foreach(_Macro_x, ...) H2PP_FOREACH(, _H2ForeachMacro, (_Macro_x), __VA_ARGS__)
#define _H2ForeachMacro(_Args, i, x) H2PP_REMOVE_PARENTHESES(_Args)(x)

#define H2Fullmesh(_Macro_x_y, ...) H2PP_FULLMESH(, _H2FullmeshMacro, (_Macro_x_y), __VA_ARGS__)
#define _H2FullmeshMacro(_Args, i, j, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)
// source/utils/h2_numeric.hpp

struct h2_numeric {
   static bool not2n(unsigned x);
   static unsigned mask2n(unsigned x);

   static int hex_to_byte(char c);
   static bool is_bin_string(const char* s);
   static bool is_hex_string(const char* s);

   static int bin_to_bits(const char* bin, unsigned char* bytes);

   static int hex_to_bits(const char* hex, unsigned char* bytes);
   static int hex_to_bytes(const char* hex, unsigned char* bytes);

   static bool bits_equal(const unsigned char* b1, const unsigned char* b2, int nbits);

   static const char* sequence_number(int sequence, int shift = 1);
};
// source/utils/h2_libc.hpp

struct h2_libc {
   static void* malloc(size_t size);
   static void free(void* ptr);
   static ssize_t write(int fd, const void* buf, size_t count);
   static void* operator new(std::size_t size) { return malloc(size); }
   static void operator delete(void* ptr) { free(ptr); }
};
// source/utils/h2_shared_ptr.hpp

template <typename T>
class h2_shared_ptr : h2_libc {
 public:
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

 private:
   void acquire(T* p, long* n)
   {
      pn = n;
      if (p) {
         if (!pn)
            pn = new (h2_libc::malloc(sizeof(long))) long(1);
         else
            ++*pn;
      }
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

   bool equals(const h2_string& str, bool caseless = false) const;
   bool contains(const h2_string& substr, bool caseless = false) const;
   bool startswith(const h2_string& prefix, bool caseless = false) const;
   bool endswith(const h2_string& suffix, bool caseless = false) const;

   bool isspace() const;
   bool enclosed(const char c = '\"') const;

   h2_string escape() const;
   h2_string unescape() const;
   h2_string unquote(const char c = '\"') const;
   h2_string tolower() const;
   h2_string center(int width) const;
};

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }
// source/utils/h2_row.hpp

struct h2_row : h2_vector<h2_string> {
   h2_row() {}
   h2_row(const char* a) : h2_vector<h2_string>({a}) {}
   h2_row(const h2_string& a) : h2_vector<h2_string>({a}) {}
   h2_row(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   unsigned width(bool ignore_indent = false) const;
   h2_row& indent(int n, const char c = ' ');
   h2_row& padding(int n, const char c = ' ');

   h2_row& printf(const char* style, const char* format, ...);
   h2_row& operator+=(const h2_row& row);
   h2_row& brush(const char* style);

   bool enclosed(const char c) const;
   h2_row gray_quote() const;
   h2_row acronym(int width = 16, int tail = 0) const;
   h2_string string() const;

   void samesizify(h2_row& b);
};

inline h2_row operator+(const h2_row& a, const h2_row& b)  // implicit conversion const char* / h2_string
{
   h2_row row;
   row.insert(row.end(), a.begin(), a.end());
   row.insert(row.end(), b.begin(), b.end());
   return row;
}

static inline const h2_row color(const h2_row& s, const char* style) { return ("\033{" + h2_string(style) + "}") + s + "\033{reset}"; }
static inline const h2_row delta(const h2_row& s, const char* style) { return ("\033{+" + h2_string(style) + "}") + s + ("\033{-" + h2_string(style) + "}"); }
static inline const h2_row gray(const h2_row& s) { return delta(s, "dark gray"); }

struct h2_rows : h2_vector<h2_row> {
   h2_rows() {}
   h2_rows(std::initializer_list<h2_row> il) : h2_vector<h2_row>(il) {}

   h2_rows& operator+=(const h2_rows& rows);

   unsigned width() const;
   bool foldable(unsigned width = 20);
   h2_row folds();

   h2_string string() const;

   void sequence(unsigned indent = 0, int start = 0);
   void samesizify(h2_rows& b);
};
// source/utils/h2_stringify.hpp

template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_row print(T a, bool represent = false) { return "?"; }
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
   static h2_row print(const T& a, bool represent = false) { return print__tostring(a, represent); }
   template <typename U>
   static auto print__tostring(const U& a, bool represent) -> typename std::enable_if<h2::h2_tostring_able<U>::value, h2_row>::type { return const_cast<U&>(a).tostring(); }
   template <typename U>
   static auto print__tostring(const U& a, bool represent) -> typename std::enable_if<!h2::h2_tostring_able<U>::value, h2_row>::type { return print__toString(a, represent); }
   template <typename U>
   static auto print__toString(const U& a, bool represent) -> typename std::enable_if<h2::h2_toString_able<U>::value, h2_row>::type { return const_cast<U&>(a).toString(); }
   template <typename U>
   static auto print__toString(const U& a, bool represent) -> typename std::enable_if<!h2::h2_toString_able<U>::value, h2_row>::type { return print__Tostring(a, represent); }
   template <typename U>
   static auto print__Tostring(const U& a, bool represent) -> typename std::enable_if<h2::h2_Tostring_able<U>::value, h2_row>::type { return const_cast<U&>(a).toString(); }
   template <typename U>
   static auto print__Tostring(const U& a, bool represent) -> typename std::enable_if<!h2::h2_Tostring_able<U>::value, h2_row>::type { return print__ToString(a, represent); }
   template <typename U>
   static auto print__ToString(const U& a, bool represent) -> typename std::enable_if<h2::h2_ToString_able<U>::value, h2_row>::type { return const_cast<U&>(a).ToString(); }
   template <typename U>
   static auto print__ToString(const U& a, bool represent) -> typename std::enable_if<!h2::h2_ToString_able<U>::value, h2_row>::type { return print__to_string(a, represent); }
   template <typename U>
   static auto print__to_string(const U& a, bool represent) -> typename std::enable_if<h2::h2_to_string_able<U>::value, h2_row>::type { return const_cast<U&>(a).to_string(); }
   template <typename U>
   static auto print__to_string(const U& a, bool represent) -> typename std::enable_if<!h2::h2_to_string_able<U>::value, h2_row>::type { return ""; }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_row print(const T& a, bool represent = false) { return ostream_print(a, represent); }

   template <typename U>
   static h2_row ostream_print(const U& a, bool represent)
   {
      h2_ostringstream os;
      os << std::boolalpha;
      os << const_cast<U&>(a);
      if (represent) {
         const char* quote = nullptr;
         if (std::is_same<char, U>::value) quote = "'";
         if (std::is_convertible<U, h2_string>::value) quote = "\"";
         if (quote) return gray(quote) + os.str().c_str() + gray(quote);
      }
      return {os.str().c_str()};
   }

   static h2_row ostream_print(unsigned char a, bool represent)
   {  // https://en.cppreference.com/w/cpp/string/byte/isprint
      return ostream_print<unsigned int>(static_cast<unsigned int>(a), represent);
   }
};

template <typename K, typename V>
struct h2_stringify_impl<std::pair<K, V>> {
   static h2_row print(const std::pair<K, V>& a, bool represent = false)
   {
      return gray("(") + h2_stringify_impl<K>::print(a.first, represent) + gray(", ") + h2_stringify_impl<V>::print(a.second, represent) + gray(")");
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value && !std::is_convertible<T, h2_string>::value>::type> {
   static h2_row print(const T& a, bool represent = false)
   {
      h2_row row;
      row += gray("[");
      for (auto it = a.begin(); it != a.end(); it++) {
         if (it != a.begin()) row += gray(", ");
         row += h2_stringify_impl<typename T::value_type>::print(*it, represent);
      }
      row += gray("]");
      return row;
   }
};

template <typename... Args>
struct h2_stringify_impl<std::tuple<Args...>> {
   static h2_row print(const std::tuple<Args...>& a, bool represent = false)
   {
      return gray("(") + tuple_print(a, represent, std::integral_constant<std::size_t, sizeof...(Args)>()) + gray(")");
   }

   static h2_row tuple_print(const std::tuple<Args...>& a, bool represent, std::integral_constant<std::size_t, 0>) { return {}; }
   template <std::size_t I>
   static h2_row tuple_print(const std::tuple<Args...>& a, bool represent, std::integral_constant<std::size_t, I>)
   {
      return tuple_print(a, represent, std::integral_constant<std::size_t, I - 1>()) + (1 < I ? gray(", ") : h2_row()) + h2_stringify_impl<typename std::decay<decltype(std::get<I - 1>(a))>::type>::print(std::get<I - 1>(a), represent);
   }
};

template <>
struct h2_stringify_impl<std::nullptr_t> {
   static h2_row print(std::nullptr_t a, bool represent = false) { return "nullptr"; }
};

template <typename T>
inline h2_row h2_stringify(const T& a, bool represent = false)
{
   return h2_stringify_impl<T>::print(a, represent);
}

template <typename T>
inline h2_row h2_stringify(T a, size_t n, bool represent)
{
   if (n == 0) return h2_stringify(a, represent);

   h2_row row;
   row += gray("[");
   for (size_t i = 0; i < n; ++i) {
      if (i) row += gray(", ");
      row += h2_stringify(a[i], represent);
   }
   row += gray("]");
   return row;
}

template <typename T>
inline h2_row h2_representify(const T& a) { return h2_stringify(a, true); }

template <typename T>
inline h2_row h2_representify(T a, size_t n) { return h2_stringify(a, n, true); }
// source/utils/h2_color.hpp

struct h2_color {
   static void prints(const char* style, const char* format, ...);
   static void printl(const h2_row& row);
   static void printl(const h2_rows& rows);

   static bool isctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
// source/utils/h2_nm.hpp

struct h2_symbol {
   h2_list x;
   std::string name;
   unsigned long long addr;
   h2_symbol(char* _name, unsigned long long _addr) : name(_name), addr(_addr) {}
};

struct h2_nm {
   h2_singleton(h2_nm);
   std::map<std::string, unsigned long long> *symbols_mangled;
   h2_list symbols_demangled;
   h2_nm(){};
   static unsigned long long get(const char* name); // by mangled name
   static int find(const char* name, h2_symbol* res[], int n); // by demangled name
   static long long text_offset();
   static long long vtable_offset();
   static bool in_main(unsigned long long addr);
};
// source/utils/h2_backtrace.hpp

struct h2_backtrace {
   int count = 0, shift = 0;
   void* array[100];

   h2_backtrace() = default;
   h2_backtrace(int shift_);

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(const h2_backtrace&);

   bool has(void* func, int size) const;
   void print(h2_vector<h2_string>& stacks) const;
   void print(int pad) const;
};
// source/other/h2_option.hpp

static constexpr unsigned linux = 0x0101;
static constexpr unsigned macos = 0x0102;
static constexpr unsigned winos = 0x0200;

struct h2_option {
   h2_singleton(h2_option);

#if defined linux || defined __linux || defined __linux__
   static constexpr unsigned os = linux;
#elif defined __APPLE__
   static constexpr unsigned os = macos;
#elif defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
   static constexpr unsigned os = winos;
#endif

   unsigned terminal_width;

   char args[256];
   const char* path;
   const char* debug = nullptr;
   bool verbose = false;
   bool compact = false;
   bool colorful = true;
   bool execute_progress = true;
   bool fold_json = true;
   bool copy_paste_json = false;
   bool only_execute_fails = false;
   bool shuffle_order = false;
   bool memory_check = true;
   bool exception_fails = false;
   char list_cases = '\0';
   int break_after_fails = 0;
   int rounds = 1;
   char junit[256]{'\0'};
   char tap[256]{'\0'};
   std::vector<const char*> includes, excludes;

   h2_option();
   void parse(int argc, const char** argv);
   bool filter(const char* suitename, const char* casename, const char* file, int line) const;
};

static const h2_option& O = h2_option::I();  // for pretty
// source/other/h2_layout.hpp

struct h2_layout {
   static h2_rows split(const h2_rows& left_rows, const h2_rows& right_rows, const char* left_title, const char* right_title, int step, char scale, unsigned width);
   static h2_rows unified(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width);
   static h2_rows seperate(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width);
};
// source/other/h2_debug.hpp

struct h2_debugger {
   static void trap();
};

#define h2_debug(...)                                                               \
   do {                                                                             \
      if (!O.debug) {                                                               \
         h2_color::prints("", __VA_ARGS__);                                         \
         h2_color::prints("", " %s : %d = %s\n", __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace bt(0);                                                        \
         bt.print(3);                                                               \
      }                                                                             \
   } while (0)
// source/other/h2_failure.hpp

struct h2_fail : h2_libc {
   h2_fail *subling_next{nullptr}, *child_next{nullptr};

   const char* assert_type = "Inner";  // Inner(Mock, AllOf, &&, ||)
   h2_string e_expression, a_expression;
   h2_row explain;
   h2_string user_explain;

   //           expression     expection      represent       value
   // number     Ge(var)        Ge(5)          5               5
   // string     We(var)        We("abc")      "abc"           abc
   // char       We(var)        We('a')        'a'             a

   int seqno = -1;
   const char* file;
   int line;

   h2_fail(const h2_row& explain_, const char* file_, int line_) : explain(explain_), file(file_), line(line_) {}
   virtual ~h2_fail();

   h2_row locate();

   virtual void print(int subling_index = 0, int child_index = 0) {}
   virtual void print(FILE* fp) {}

   void foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index = 0, int child_index = 0);
   static void append_subling(h2_fail*& fail, h2_fail* n);
   static void append_child(h2_fail*& fail, h2_fail* n);

   static h2_fail* new_normal(const h2_row& explain, const char* file = nullptr, int line = 0);
   static h2_fail* new_unexpect(const h2_row& expection = {}, const h2_row& represent = {}, const h2_row& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_row& expection, const h2_row& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_row& expection, const h2_row& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_json(const h2_string& e_value, const h2_string& a_value, const h2_row& expection, bool caseless, const h2_row& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& represent, const h2_row& explain = {}, const char* file = nullptr, int line = 0);
   static h2_fail* new_memory_leak(const void* ptr, int size, const h2_vector<std::pair<int, int>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* file, int line);
   static h2_fail* new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free);
   static h2_fail* new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release);
   static h2_fail* new_overflow(const void* ptr, const int size, const void* addr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file = nullptr, int line = 0);
   static h2_fail* new_use_after_free(const void* ptr, const void* addr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use);
};

static inline void h2_fail_g(h2_fail*, bool);
// source/json/h2_json.hpp

struct h2_json {
   static h2_rows format(const h2_string& json_string);
   static h2_string select(const h2_string& json_string, const h2_string& selector, bool caseless);
   // < 0 illformed json; = 0 matched; > 0 unmatched
   static int match(const h2_string& expect, const h2_string& actual, bool caseless);
   static bool diff(const h2_string& expect, const h2_string& actual, h2_rows& e_lines, h2_rows& a_lines, bool caseless);
};
// source/memory/h2_exempt.hpp

struct h2_exempt {
   h2_singleton(h2_exempt);
   h2_list exempts;
   static void setup();
   static void add(void* func);
   static bool in(const h2_backtrace& bt);
};

#define H2UNMEM(func) h2::h2_exempt::add((void*)func)
// source/memory/h2_memory.hpp

struct h2_memory {
   static void initialize();
   static void finalize();
   static void overrides();
   static void restores();

   struct stack {
      static void root();
      static void push(const char* file, int line);
      static h2_fail* pop();
      static long long footprint();

      struct block : h2_once {
         block(const char* attributes, const char* file, int line);
         ~block();
      };
   };
};

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(Attributes, __FILE__, __LINE__); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2PP_UNIQUE(t_block))
// source/exception/h2_exception.hpp

struct h2_exception {
   static void initialize();
};
// source/matcher/h2_matches.hpp

struct h2_matches {
   virtual h2_row expection(bool caseless, bool dont) const = 0;
};

static inline h2_string DS(bool match) { return match ? "should not match" : ""; }

static inline h2_row CD(const h2_row& s, bool caseless, bool dont, const char* dsym = "!")
{
   h2_row row;
   if (dont) row.push_back(dsym);
   if (caseless) row.push_back("~");
   row += s;
   return row;
}

template <typename T>
inline auto h2_matches_expection(const T& e, bool caseless, bool dont) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_row>::type { return e.expection(caseless, dont); }
template <typename T>
inline auto h2_matches_expection(const T& e, bool caseless, bool dont) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_row>::type { return CD(h2_representify(e), caseless, dont); }

#define H2_MATCHES_T2V2E(t_matchers)                                                                                                                                             \
   template <typename T>                                                                                                                                                         \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                                                               \
   template <typename T, std::size_t I>                                                                                                                                          \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                                                  \
   {                                                                                                                                                                             \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                                                             \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                                                     \
   }                                                                                                                                                                             \
   template <typename T>                                                                                                                                                         \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }                                  \
   h2_row t2e(bool caseless, bool dont, std::integral_constant<std::size_t, 0>) const { return {}; }                                                                             \
   template <std::size_t I>                                                                                                                                                      \
   h2_row t2e(bool caseless, bool dont, std::integral_constant<std::size_t, I>) const                                                                                            \
   {                                                                                                                                                                             \
      return t2e(caseless, dont, std::integral_constant<size_t, I - 1>()) + (1 < I ? gray(", ") : h2_row()) + h2_matches_expection(std::get<I - 1>(t_matchers), caseless, dont); \
   }                                                                                                                                                                             \
   h2_row t2e(bool caseless, bool dont) const { return t2e(caseless, dont, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
// source/matcher/h2_matcher.hpp

template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(T a, int n, bool caseless, bool dont) const = 0;
   virtual h2_row expection(bool caseless, bool dont) const override { return ""; }
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
   h2_fail* matches(const T& a, int n = 0, bool caseless = false, bool dont = false) const { return impl->matches(a, n, caseless, dont); }
   virtual h2_row expection(bool caseless = false, bool dont = false) const { return impl->expection(caseless, dont); };
};

template <typename Matches>
struct h2_polymorphic_matcher : h2_matches {
   const Matches m;
   explicit h2_polymorphic_matcher(const Matches& _matches) : m(_matches) {}

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(m), 0); }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      explicit internal_impl(const Matches& _matches) : m(_matches) {}
      h2_fail* matches(T a, int n = 0, bool caseless = false, bool dont = false) const override { return m.matches(a, n, caseless, dont); }
      h2_row expection(bool caseless, bool dont) const override { return m.expection(caseless, dont); }
   };

   virtual h2_row expection(bool caseless = false, bool dont = false) const override { return h2_matches_expection(m, caseless, dont); }
};
// source/matcher/h2_equation.hpp

template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& _e, const long double = 0) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a == e) == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return CD(h2_representify(e), false, dont, "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& _e, const long double = 0) : e(h2_string(_e)) {}

   h2_fail* matches(const h2_string& a, int, bool caseless, bool dont) const
   {
      if (a.equals(e, caseless) == !dont) return nullptr;
      if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
      if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;

      return h2_fail::new_strcmp(e, a, caseless, expection(caseless, dont));
   }
   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD(h2_representify(e), caseless, dont, "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& _e, const long double _epsilon = 0) : e(_e), epsilon(_epsilon) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
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
      if (result == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return CD(h2_representify(e), false, dont, "≠");
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
      h2_fail* matches(T a, int n, bool caseless, bool dont) const override { return from.matches(static_cast<U>(a), n, caseless, dont); }
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
   h2_fail* matches(const A& a, int, bool, bool) const { return nullptr; }
   virtual h2_row expection(bool, bool) const override { return "Any"; }
};

struct h2_matches_null : h2_matches {
   const bool reverse;
   explicit h2_matches_null(bool _reverse) : reverse(_reverse) {}
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool dont) const
   {
      bool _dont = reverse ? !dont : dont;
      if ((nullptr == (const void*)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect(expection(false, dont), h2_stringify((const void*)a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return (reverse ? !dont : dont) ? "NotNull" : "IsNull";
   }
};

template <bool E>
struct h2_matches_boolean : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool dont) const
   {
      bool _dont = E ? dont : !dont;
      if (((bool)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect(expection(false, dont), a ? "true" : "false");
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return (E ? dont : !dont) ? "false" : "true";
   }
};

template <typename Matcher>
struct h2_pointee_matches : h2_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher _m) : m(_m) {}

   template <typename SmartPointer>
   struct PointeeOf {
      typedef typename SmartPointer::element_type type;
   };
   template <typename T>
   struct PointeeOf<T*> {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, int, bool caseless, bool dont) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, 0, caseless, dont);
   }
   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return h2_matches_expection(m, caseless, dont);
   }
};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_any> Any{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_null> IsNull{h2_matches_null(false)};
const h2_polymorphic_matcher<h2_matches_null> NotNull{h2_matches_null(true)};
const h2_polymorphic_matcher<h2_matches_boolean<true>> IsTrue{h2_matches_boolean<true>()};
const h2_polymorphic_matcher<h2_matches_boolean<false>> IsFalse{h2_matches_boolean<false>()};

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
// source/matcher/h2_logic.hpp

template <typename Matcher>
struct h2_not_matches : h2_matches {
   const Matcher m; /* h2_polymorphic_matcher or immediate value or h2_matches */
   explicit h2_not_matches(const Matcher& _m) : m(_m) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      return h2_matcher_cast<A>(m).matches(a, n, caseless, !dont);
   }
   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return h2_matches_expection(m, caseless, !dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_and_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_and_matches(const Matcher1& _m1, const Matcher2& _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m1).matches(a, n, caseless, false));
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m2).matches(a, n, caseless, false));
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      h2_fail::append_child(fail, fails);

      return fail;
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD((dont ? gray("(") : h2_row()) + h2_matches_expection(m1, caseless, false) + " and " + h2_matches_expection(m2, caseless, false) + (dont ? gray(")") : h2_row()), false, dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_or_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_or_matches(const Matcher1& _m1, const Matcher2& _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, n, caseless, false);
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, n, caseless, false);
      bool result = !f1 || !f2;
      if (result == !dont) {
         if (f1) delete f1;
         if (f2) delete f2;
         return nullptr;
      }
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, f1);
      h2_fail::append_subling(fails, f2);

      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD((dont ? gray("(") : h2_row()) + h2_matches_expection(m1, caseless, false) + " or " + h2_matches_expection(m2, caseless, false) + (dont ? gray(")") : h2_row()), false, dont);
   }
};

template <typename... Matchers>
struct h2_allof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, caseless, false);  // dont not transfer down
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }

      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), "should not match all");
      } else {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("AllOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int c = 0;
      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, caseless, false);
         if (!fail) {
            c++;
            break;
         }
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }

      if ((0 < c) == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), "should not match any one");
      } else {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), "not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("AnyOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, caseless, false);
         if (fail)
            delete fail;
         else {
            fail = h2_fail::new_normal("should not match " + v_matchers[i].expection(caseless, false).brush("green"));
            fail->seqno = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("NoneOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher m)
{
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(m));
}
template <typename Matches>
inline h2_polymorphic_matcher<h2_not_matches<h2_polymorphic_matcher<Matches>>> operator!(const h2_polymorphic_matcher<Matches>& m) { return Not(m); }

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
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator&&(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator||(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
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
   explicit h2_matches_ge(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a >= e) == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return CD("≥" + h2_representify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_gt : h2_matches {
   const E e;
   explicit h2_matches_gt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a > e) == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return CD(">" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_le : h2_matches {
   const E e;
   explicit h2_matches_le(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a <= e) == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return CD("≤" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_lt : h2_matches {
   const E e;
   explicit h2_matches_lt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a < e) == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return CD("<" + h2_stringify(e), false, dont);
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
   explicit h2_matches_regex(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_wildcard : h2_matches {
   const h2_string e;
   explicit h2_matches_wildcard(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_strcmp : h2_matches {
   const h2_string e;
   explicit h2_matches_strcmp(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_substr : h2_matches {
   const h2_string substring;
   explicit h2_matches_substr(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_startswith : h2_matches {
   const h2_string prefix_string;
   explicit h2_matches_startswith(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_endswith : h2_matches {
   const h2_string suffix_string;
   explicit h2_matches_endswith(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_json : h2_matches {
   const h2_string e;
   const h2_string selector;
   explicit h2_matches_json(const h2_string& _e, const h2_string& _selector) : e(_e), selector(_selector) {}
   h2_fail* matches(const h2_string& a, int, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_caseless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(const h2_matcher<h2_string>& matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const { return m.matches(a, n, true, dont); }
   virtual h2_row expection(bool caseless, bool dont) const override { return m.expection(true, dont); }
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
inline h2_polymorphic_matcher<h2_caseless_matches> operator~(const M& m) { return CaseLess(m); }
// source/matcher/h2_memcmp.hpp

struct h2_matches_bytecmp : h2_matches {
   const int width;
   const void* e;
   const bool isstring;
   const int nbytes;
   explicit h2_matches_bytecmp(const int _width, const void* _e, const bool _isstring, const int _nbytes) : width(_width), e(_e), isstring(_isstring), nbytes(_nbytes) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_bitcmp : h2_matches {
   const void* e;
   const bool isstring;
   const int nbits;
   explicit h2_matches_bitcmp(const void* _e, const bool _isstring, const int _nbits) : e(_e), isstring(_isstring), nbits(_nbits) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

template <typename E>
struct h2_matches_memcmp : h2_matches {
   const E e;
   const int length;
   explicit h2_matches_memcmp(const E _e, const int _length) : e(_e), length(_length) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const
   {
      h2_fail* fail = (h2_fail*)1;

      if (std::is_convertible<E, h2_string>::value) { /* deduce */
         if (h2_numeric::is_bin_string((const char*)e)) {
            h2_matches_bitcmp t((const void*)e, true, length);
            fail = t.matches(a, n, false, false);
         }
      }

      if (fail) {
         h2_matches_bytecmp t(h2_sizeof_pointee<E>::value * 8, e, std::is_convertible<E, h2_string>::value, length * h2_sizeof_pointee<E>::value);
         fail = t.matches(a, n, false, false);
      }

      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
      }
      return fail;
   }
   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("Me()", caseless, dont);
   }
};

template <typename E, typename T = typename std::decay<E>::type>
inline h2_polymorphic_matcher<h2_matches_memcmp<T>> Me(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_memcmp<T>>(h2_matches_memcmp<T>((T)expect, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bitcmp> M1e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bitcmp>(h2_matches_bitcmp((const void*)expect, std::is_convertible<E, h2_string>::value, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M8e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(8, (const void*)expect, std::is_convertible<E, h2_string>::value, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M16e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(16, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 2));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M32e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(32, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 4));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M64e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(64, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 8));
}
// source/matcher/h2_container.hpp

template <typename EK, typename EV>
struct h2_pair_matches : h2_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(const EK& k_, const EV& v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, int, bool caseless, bool dont) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      using AK = typename std::decay<decltype(a.first)>::type;
      using AV = typename std::decay<decltype(a.second)>::type;
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<AK>(k).matches(a.first, 0, caseless, false));
      h2_fail::append_subling(fails, h2_matcher_cast<AV>(v).matches(a.second, 0, caseless, false));
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      if (dont) {
         fails = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
      }
      return fails;
   }

   template <typename A>
   auto matches(const A& a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD(gray("(") + h2_matches_expection(k, caseless, dont) + gray(", ") + h2_matches_expection(v, caseless, dont) + gray(")"), false, dont);
   }
};

template <typename... Matchers>
struct h2_listof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int, bool caseless, bool dont) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;

      h2_vector<h2_matcher<typename A::value_type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, c = 0;
         for (auto& k : a) {
            if (j++ == i) {
               ++c;
               fail = v_matchers[i].matches(k, 0, caseless, false);
               break;
            }
         }
         if (c == 0) {
            fail = h2_fail::new_unexpect(v_matchers[i].expection(caseless, false), "", "but out of range");
         }
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(A a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ generic array */
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<typename std::decay<decltype(a[0])>::type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a[i], n, caseless, false);
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("ListOf" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_has_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_has_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int n, bool caseless, bool dont) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_vector<h2_matcher<typename A::value_type>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         bool found = false;
         for (auto& j : a) {
            h2_fail* fail = v_matchers[i].matches(j, n, caseless, false);
            if (!fail) {
               found = true;
               break;
            }
         }
         if (!found) {
            h2_fail* fail = h2_fail::new_unexpect(v_matchers[i].expection(caseless, false), "", "but haven't");
            if (fail) fail->seqno = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(A a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_vector<h2_matcher<typename std::decay<decltype(a[0])>::type>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         bool found = false;
         for (int j = 0; j < n; ++j) {
            h2_fail* fail = v_matchers[i].matches(a[j], n, caseless, false);
            if (!fail) {
               found = true;
               break;
            }
         }
         if (!found) {
            h2_fail* fail = h2_fail::new_unexpect(v_matchers[i].expection(caseless, false), "", "but haven't");
            if (fail) fail->seqno = i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a, n), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("Has" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_in_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_in_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, bool caseless, bool dont) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, n, caseless, false);
         if (!fail) ++s;
         if (fail) delete fail;
      }
      bool result = 0 < s;
      if (result == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a), DS(result));
   }

   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("In" + gray("(") + t2e(caseless, false) + gray(")"), false, dont);
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_countof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_countof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int n, bool, bool dont) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      int count = 0;
      /* https://en.cppreference.com/w/cpp/container */
      /* container size() is best, but forward_list haven't. iterator works all, regardless speed */
      for (auto& c : a) count++;

      return __matches(count, h2_representify(a), dont);
   }

   template <typename A>
   auto matches(A a, int n, bool caseless, bool dont) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ generic array or not array , no meaningful just for continues */
      int count = 1;
      return __matches(count, h2_representify(a), dont);
   }

   h2_fail* __matches(int count, h2_row&& represent, bool dont) const
   {
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<int>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(count, 0, false, false);
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(false, dont), represent + "/" + h2_stringify(count), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_row expection(bool, bool dont) const override
   {
      return CD("CountOf" + gray("(") + t2e(false, dont) + gray(")"), false, false);
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_pair_matches<EK, EV>> Pair(const MK& mk, const MV& mv)
{
   return h2_polymorphic_matcher<h2_pair_matches<EK, EV>>(h2_pair_matches<EK, EV>(mk, mv));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>> Has(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>>(h2_has_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>> Have(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>>(h2_has_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>> In(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>>(h2_in_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matchers&>::type...>> CountOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matchers&>::type...>>(h2_countof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
// source/matcher/h2_customize.hpp

#define __Matches_Common(message)                                                                        \
   template <typename A>                                                                                 \
   bool __matches(const A& a) const;                                                                     \
   template <typename A>                                                                                 \
   h2::h2_fail* matches(const A& a, int, bool caseless, bool dont) const                                 \
   {                                                                                                     \
      h2::h2_fail* fail = h2::h2_fail::new_unexpect(h2::CD("", caseless, dont), h2::h2_representify(a)); \
      if (__matches(a) == !dont) return nullptr;                                                         \
      if (dont) {                                                                                        \
      } else {                                                                                           \
         h2::h2_ostringstream osm;                                                                       \
         osm << std::boolalpha << H2PP_REMOVE_PARENTHESES(message);                                      \
         fail->user_explain = osm.str().c_str();                                                         \
      }                                                                                                  \
      return fail;                                                                                       \
   }                                                                                                     \
   virtual h2::h2_row expection(bool, bool) const override { return ""; }

#define H2MATCHER0(name, message)                                                     \
   struct h2_##name##_matches : h2::h2_matches {                                      \
      explicit h2_##name##_matches() {}                                               \
      __Matches_Common(message)                                                       \
   };                                                                                 \
   const h2::h2_polymorphic_matcher<h2_##name##_matches> name{h2_##name##_matches()}; \
   template <typename A>                                                              \
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

/* clang-format off */

#define _H2MATCHER_1(name) H2MATCHER0(name, (""))

#define _H2MATCHER_2_0(name, e1) H2MATCHER1(name, e1, (""))
#define _H2MATCHER_2_1(name, message) H2MATCHER0(name, message)
#define _H2MATCHER_2(name, t) H2PP_CAT(_H2MATCHER_2_, H2PP_IS_BEGIN_PARENTHESIS(t)) (name, t)

#define _H2MATCHER_3_0(name, e1, e2) H2MATCHER2(name, e1, e2, (""))
#define _H2MATCHER_3_1(name, e1, message) H2MATCHER1(name, e1, message)
#define _H2MATCHER_3(name, e1, t) H2PP_CAT(_H2MATCHER_3_, H2PP_IS_BEGIN_PARENTHESIS(t)) (name, e1, t)

#define _H2MATCHER_4_0(name, e1, e2, e3) H2MATCHER3(name, e1, e2, e3, (""))
#define _H2MATCHER_4_1(name, e1, e2, message) H2MATCHER2(name, e1, e2, message)
#define _H2MATCHER_4(name, e1, e2, t) H2PP_CAT(_H2MATCHER_4_, H2PP_IS_BEGIN_PARENTHESIS(t)) (name, e1, e2, t)

#define _H2MATCHER_5_0(name, e1, e2, e3, e4) H2MATCHER4(name, e1, e2, e3, e4, (""))
#define _H2MATCHER_5_1(name, e1, e2, e3, message) H2MATCHER3(name, e1, e2, e3, message)
#define _H2MATCHER_5(name, e1, e2, e3, t) H2PP_CAT(_H2MATCHER_5_, H2PP_IS_BEGIN_PARENTHESIS(t)) (name, e1, e2, e3, t)

#define _H2MATCHER_6_0(name, e1, e2, e3, e4, e5) H2MATCHER5(name, e1, e2, e3, e4, e5, (""))
#define _H2MATCHER_6_1(name, e1, e2, e3, e4, message) H2MATCHER4(name, e1, e2, e3, e4, message)
#define _H2MATCHER_6(name, e1, e2, e3, e4, t) H2PP_CAT(_H2MATCHER_6_, H2PP_IS_BEGIN_PARENTHESIS(t)) (name, e1, e2, e3, e4, t)

#define _H2MATCHER_7_0(name, e1, e2, e3, e4, e5, e6)
#define _H2MATCHER_7_1(name, e1, e2, e3, e4, e5, message) H2MATCHER5(name, e1, e2, e3, e4, e5, message)
#define _H2MATCHER_7(name, e1, e2, e3, e4, e5, t) H2PP_CAT(_H2MATCHER_7_, H2PP_IS_BEGIN_PARENTHESIS(t)) (name, e1, e2, e3, e4, e5, t)

#define H2MATCHER(...) H2PP_VARIADIC_CALL(_H2MATCHER_, __VA_ARGS__)
// source/matcher/h2_matcher.cpp

template <typename T>
inline h2_matcher<T>::h2_matcher() { *this = Any; }

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = Eq(value); }
// source/stub/h2_fp.hpp

template <typename T>
void* h2_fp(T p)
{
   void* fp = (void*)p;
   if (std::is_convertible<T, h2::h2_string>::value) {
      h2_symbol* res[100];
      int n = h2_nm::find((const char*)p, res, 100);
      if (n != 1) {
         if (n == 0) {
            h2_color::prints("yellow", "\nDon't find %s\n", (const char*)p);
         } else {
            h2_color::prints("yellow", "\nFind multiple %s :\n", (const char*)p);
            for (int i = 0; i < n; ++i)
               h2_color::prints("yellow", "  %d. %s \n", i + 1, res[i]->name.c_str());
         }
         return nullptr;
      }
      fp = (void*)(res[0]->addr + h2_nm::text_offset());
   }
   return fp;
}
// source/stub/h2_mfp.hpp

/* clang-format off */

template <typename T, int I> struct h2_constructible_error {
   static T* O(void* m) { return static_cast<T*>(m = (void*)I); }
};
template <typename T> struct h2_constructible0 : std::true_type {
   static T* O(void* m) { return new (m) T(); }
};

template <typename, typename> struct h2_constructible1_impl : std::false_type {};
template <typename, typename> struct h2_constructible2_impl : std::false_type {};
template <typename, typename> struct h2_constructible3_impl : std::false_type {};
template <typename, typename> struct h2_constructible4_impl : std::false_type {};
template <typename, typename> struct h2_constructible5_impl : std::false_type {};
template <typename, typename> struct h2_constructible6_impl : std::false_type {};
template <typename, typename> struct h2_constructible7_impl : std::false_type {};
template <typename, typename> struct h2_constructible8_impl : std::false_type {};
template <typename, typename> struct h2_constructible9_impl : std::false_type {};

#if (defined(__GNUC__) && __GNUC__ >= 5) || defined __clang__ || defined _WIN32

template <typename T>
struct h2_constructible1_impl<h2_void_t<decltype(T({}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}); }
};

template <typename T>
struct h2_constructible2_impl<h2_void_t<decltype(T({}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}); }
};

template <typename T>
struct h2_constructible3_impl<h2_void_t<decltype(T({}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}); }
};

template <typename T>
struct h2_constructible4_impl<h2_void_t<decltype(T({}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible5_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible6_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible7_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible8_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}); }
};

template <typename T>
struct h2_constructible9_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}, {}, {}, {}, {}))>, T> : std::true_type {
   static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}, {}); }
};

#endif

template <typename T> using h2_constructible1 = h2_constructible1_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible2 = h2_constructible2_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible3 = h2_constructible3_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible4 = h2_constructible4_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible5 = h2_constructible5_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible6 = h2_constructible6_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible7 = h2_constructible7_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible8 = h2_constructible8_impl<h2_void_t<>, T>;
template <typename T> using h2_constructible9 = h2_constructible9_impl<h2_void_t<>, T>;

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

//  https://itanium-cxx-abi.github.io/cxx-abi/

//  g++ -std=c++11 -fdump-class-hierarchy

//  &Class::Method has separate representations for non-virtual and virtual functions.
//  For non-virtual functions, it is the address of the function.
//  For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
//  The least-significant bit therefore discriminates between virtual and non-virtual functions.

template <typename Class, typename Signature>
struct h2_mfp;

template <typename Class, typename ReturnType, typename... Args>
struct h2_mfp<Class, ReturnType(Args...)> {
   static constexpr bool is_static_member_function(ReturnType (*)(Args...)) { return true; }
   static constexpr bool is_static_member_function(ReturnType (Class::*)(Args...)) { return false; }

   static void* A(ReturnType (*f)(Args...))
   {
      return (void*)f;
   }

   static bool is_virtual_member_function(long long uv)
   {
      return (uv & 1) && (uv - 1) % sizeof(void*) == 0
             /* assumption: virtual member count less than 1000 */
             && (uv - 1) / sizeof(void*) < 1000;
   }

   static void* A(ReturnType (Class::*f)(Args...))
   {
      union {
         ReturnType (Class::*f)(Args...);
         void* p;
         long long v;
      } u{f};
      if (!is_virtual_member_function(u.v)) return u.p;
      void** vtable = nullptr;
      Class* object = h2_constructible<Class>::O(alloca(sizeof(Class)));
      if (0 == (long long)object || 1 == (long long)object || 2 == (long long)object) {
         char vtable_symbol[1024];
         sprintf(vtable_symbol, "_ZTV%s", typeid(Class).name());  // mangled for "vtable for Class"
         unsigned long long relative_vtable = h2_nm::get(vtable_symbol);
         if (relative_vtable) {
            vtable = (void**)(relative_vtable + h2_nm::vtable_offset());
         }
      } else {
         vtable = *(void***)object;
      }
      if (!vtable) return nullptr;
      return vtable[(u.v - 1) / sizeof(void*)];
   }
};
// source/stub/h2_stub.hpp

struct h2_stubs {
   h2_list stubs;
   bool add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line);
   void clear(void* origin_fp);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   unsigned char saved[32];
   void* origin_fp;
   h2_stub_temporary_restore(void* origin_fp);
   ~h2_stub_temporary_restore();
};
// source/mock/h2_routine.hpp

template <typename ReturnType>
struct h2_return : h2_libc {
   ReturnType value;
   h2_return() = delete;
   explicit h2_return(ReturnType _value) : value(_value){};
};

template <typename Class, typename Signature>
struct h2_routine;

template <typename Class, typename ReturnType, typename... Args>
struct h2_routine<Class, ReturnType(Args...)> {
   ReturnType (*fp)(Args...) = nullptr; // normal function pointer
   ReturnType (*mfp)(Class*, Args...) = nullptr; // member function pointer
   h2_shared_ptr<h2_return<ReturnType>> return_value;

   h2_routine() {}
   h2_routine(ReturnType r) : return_value(new h2_return<ReturnType>(r)) {}
   h2_routine(ReturnType (*f)(Args...)) : fp(f) {}
   h2_routine(ReturnType (*f)(Class*, Args...)) : mfp(f) {}

   ReturnType operator()(Class* This, Args... args)
   {
      if (mfp) {
         return mfp(This, args...);
      } else if (fp) {
         return fp(args...);
      } else if (return_value) {
         return return_value->value;
      }
      /* return uninitialized value */
   }
   operator bool()
   {
      return fp || mfp || return_value;
   }
   void clear()
   {
      fp = nullptr;
      mfp = nullptr;
      return_value.reset();
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
      if (mfp) {
         mfp(This, args...);
      } else if (fp) {
         fp(args...);
      }
   }
   operator bool()
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

/* 考勤 ; 函数被调次数期望 */
struct h2_checkin {
   int call = 0;
   const int least, most;
   const h2_string expr;

   h2_checkin(int _least, int _most, const h2_string& _expr) : least(_least), most(_most), expr(_expr) {}

   bool insufficient(/*不够*/) const { return call < least; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_excessive(/*过多*/) const { return most < call; }

   h2_fail* check(const char* func, int index, int total, const char* file, int line);
   const char* actual();
   const char* expect();

   static h2_checkin Once() { return h2_checkin(1, 1, "Once"); }
   static h2_checkin Twice() { return h2_checkin(2, 2, "Twice"); }
   static h2_checkin Times(int count) { return h2_checkin(count, count, "Times"); }
   static h2_checkin Any() { return h2_checkin(0, INT_MAX, "Any"); }
   static h2_checkin Atleast(int count) { return h2_checkin(count, INT_MAX, "Atleast"); }
   static h2_checkin Atmost(int count) { return h2_checkin(0, count, "Atmost"); }
   static h2_checkin Between(int left, int right) { return h2_checkin(left, right, "Between"); }
};
// source/mock/h2_match.hpp

static inline void h2_assert_g();

template <typename MatcherTuple, typename ArgumentTuple, std::size_t I>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, I>)
{
   h2_fail* fails = matches(matchers, arguments, std::integral_constant<std::size_t, I - 1>());
   h2_fail* fail = std::get<I - 1>(matchers).matches(std::get<I - 1>(arguments));
   if (fail) fail->seqno = I - 1;
   h2_fail::append_subling(fails, fail);
   h2_assert_g();
   return fails;
}

template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, 0>)
{
   return nullptr;
}

template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* h2_tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments)
{
   return matches(matchers, arguments, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}
// source/mock/h2_mock.hpp

struct h2_mockee : h2_libc {
   h2_list x;
   void *origin_fp, *substitute_fp;
   const char* return_type;
   const char* class_function;
   h2_vector<const char*> argument_type;
   const char* inspects;
   const char* file;
   int line;

   h2_row arguments(int seq = -1);
   h2_row signature();

   h2_vector<h2_checkin> checkin_array;
   int checkin_index = 0;

   virtual void reset() = 0;
   void mock();
   h2_fail* times_check();
};

namespace {

template <int Counter, typename Class, typename Signature>
class h2_mocker;

template <int Counter, typename Class, typename ReturnType, typename... Args>
class h2_mocker<Counter, Class, ReturnType(Args...)> : h2_mockee {
   using ArgumentTuple = std::tuple<Args...>;

#define H2_Typedef_Matcher(__, i) h2_matcher<h2_nth_decay<i, Args...>>
   using MatcherTuple = std::tuple<H2PP_REPEAT((, ), H2_Typedef_Matcher, , 20)>;
#undef H2_Typedef_Matcher

   h2_vector<MatcherTuple> matcher_array;
   h2_vector<h2_routine<Class, ReturnType(Args...)>> routine_array;
   bool greed_mode = true;

   static ReturnType normal_function_stub(Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().origin_fp);
      return I().routine_array[index](nullptr, std::forward<Args>(args)...);
   }

   static ReturnType member_function_stub(Class* This, Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().origin_fp);
      return I().routine_array[index](This, std::forward<Args>(args)...);
   }

   int matches(Args... args)
   {
      ArgumentTuple at = std::forward_as_tuple(std::forward<Args>(args)...);
      int checkin_offset = -1;
      for (int i = checkin_index; i < checkin_array.size(); ++i) {
         h2_fail* fails = h2_tuple_matches(matcher_array[i], at);
         if (fails) {
            if (checkin_offset != -1) {
               break;
            }
            if (checkin_array[i].is_satisfied()) { /* try next h2_checkin */
               delete fails;
               continue;
            }
            fails->foreach([this, i](h2_fail* f, int, int) {
               f->explain += gray("on ") + (class_function + arguments(f->seqno));
               if (1 < checkin_array.size()) {
                  f->explain += gray(" when ") + h2_numeric::sequence_number(i) + " checkin " + color(checkin_array[i].expr, "cyan");
               }
            });
            h2_fail* fail = h2_fail::new_normal(signature(), file, line);
            h2_fail::append_child(fail, fails);
            h2_fail_g(fail, false);
         } else {
            checkin_index = i;
            checkin_offset = i;
            if (checkin_array[i].is_saturated()) {
               continue;
            }
            if (checkin_array[i].insufficient()) {
               break;
            }
            /* satisfied */
            if (greed_mode) {
               break;
            }
            /* continue */
         }
      }
      if (checkin_offset != -1) {
         checkin_array[checkin_offset].call += 1;
      }
      if (checkin_offset == -1) {
         h2_fail* fail = h2_fail::new_normal(signature(), file, line);
         h2_fail* f = h2_fail::new_normal(class_function + h2_representify(at) + color(" unexpectedly", "red,bold") + " called");
         h2_fail::append_child(fail, f);
         h2_fail_g(fail, false);
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
      if (!i) i = new h2_mocker();
      return *i;
   }

   static h2_mocker& I(void* origin_fp, const char* return_type, const char* class_function, const h2_vector<const char*>& argument_type, const char* inspects, const char* file, int line)
   {
      I().origin_fp = origin_fp;
      I().substitute_fp = std::is_same<std::false_type, Class>::value ? (void*)normal_function_stub : (void*)member_function_stub;
      I().return_type = return_type;
      I().class_function = class_function;
      I().argument_type = argument_type;
      I().inspects = inspects;
      I().file = file;
      I().line = line;
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

   h2_mocker& Return()
   {
      if (checkin_array.empty()) Any();
      if (std::is_same<std::false_type, Class>::value)
         routine_array.back().fp = (ReturnType(*)(Args...))origin_fp;
      else
         routine_array.back().mfp = (ReturnType(*)(Class*, Args...))origin_fp;
      return *this;
   }

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

}  // namespace
// source/mock/h2_mocks.hpp

struct h2_mocks {
   h2_list mocks;
   bool add(void* mock);
   h2_fail* clear(bool check);
};
// source/extension/h2_dns.hpp

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
// source/extension/h2_socket.hpp

struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(size_, data_){};
};

struct h2_sock : h2_libc {
   h2_list x, y;
   h2_stubs stubs;

   struct socket {
      int fd;
      h2_string from, to;
      socket(int fd_, const char* from_, const char* to_) : fd(fd_), from(from_), to(to_) {}
   };

   h2_vector<socket> sockets;

   h2_sock();
   ~h2_sock();

   void put_outgoing(const char* from, const char* to, const char* data, size_t size);
   void put_outgoing(int fd, const char* data, size_t size);
   void put_incoming(const char* from, const char* to, const char* data, size_t size);

   char last_to[128];
   h2_list incoming, outgoing;
};

template <typename M1, typename M2, typename M3, typename M4>
struct h2_packet_matches : h2_matches {
   const M1 from;
   const M2 to;
   const M3 data;
   const M4 size;
   explicit h2_packet_matches(M1 from_, M2 to_, M3 data_, M4 size_) : from(from_), to(to_), data(data_), size(size_) {}

   h2_fail* matches(h2_packet* a, int, bool caseless = false, bool dont = false) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(from).matches(a->from.c_str(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(to).matches(a->to.c_str(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const unsigned char*>(data).matches((unsigned char*)a->data.data(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const int>(size).matches(a->data.size(), 0, caseless, dont));
      return fails;
   }

   virtual h2_row expection(bool, bool) const override { return ""; }
};

template <typename M1, typename M2, typename M3, typename M4>
inline h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>> PktEq(M1 from = Any, M2 to = Any, M3 data = Any, M4 size = Any)
{
   return h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>>(h2_packet_matches<M1, M2, M3, M4>(from, to, data, size));
}

struct h2_socket {
   static h2_packet* start_and_fetch();
   static void inject_received(const void* packet, size_t size, const char* attributes); // from=1.2.3.4:5678, to=4.3.2.1:8765
};

/* clang-format off */
#define __H2SOCK0(_Packet, _Size, ...) h2::h2_socket::inject_received(_Packet, _Size, #__VA_ARGS__)
#define __H2SOCK1(...) h2::h2_socket::start_and_fetch()
#define H2SOCK(...) H2PP_CAT(__H2SOCK, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
// source/extension/h2_stdio.hpp

struct h2_stdio {
   static void initialize();
   static size_t capture_length();
   static const char* capture_cout(const char* type);
   static void capture_cancel();
};

#define H2COUT(...) h2::h2_stdio::capture_cout(#__VA_ARGS__)
// source/core/h2_case.hpp

struct h2_case {
   static constexpr int initial = 0, passed = 1, failed = 2, ignored = 3;

   const char* name;
   const char* file;
   int line;
   bool todo = false, filtered = false;
   int status = initial, last_status = initial;
   int seq = 0;
   int asserts = 0;
   long long footprint = 0;
   h2_list x;
   jmp_buf ctx;
   h2_fail* fails{nullptr};
   h2_stubs stubs;
   h2_mocks mocks;
   h2_dnses dnses;
   h2_sock* sock{nullptr};

   h2_case(const char* name_, const char* file_, int line_, int todo_) : name(name_), file(file_), line(line_), todo(todo_) {}
   void clear();

   void prev_setup();
   void post_setup() {}
   void prev_cleanup() {}
   void post_cleanup(const h2_string& ex);

   void do_fail(h2_fail* fail, bool defer);

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
   const char* name;
   const char* file;
   int line;
   h2_list x;
   int seq = 0;
   h2_stats stats;
   bool filtered = false;
   jmp_buf ctx;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list cases;
   h2_stubs stubs;
   h2_mocks mocks;

   h2_suite(const char* name, void (*)(h2_suite*, h2_case*), const char* file, int line);
   void clear();

   void enumerate();
   void execute(h2_case* c);

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
// source/core/h2_task.hpp

struct h2_task {
   h2_singleton(h2_task);

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
   void execute();
};

static inline void h2_stub_g(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line)
{
   if (!origin_fp || !substitute_fp) return;
   if (h2_task::I().current_case)
      h2_task::I().current_case->stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
   else
      h2_task::I().stubs.add(origin_fp, substitute_fp, origin_fn, file, line);
}

static inline void h2_unstub_g(void* origin_fp)
{
   if (!origin_fp) return;
   if (h2_task::I().current_case)
      h2_task::I().current_case->stubs.clear(origin_fp);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->stubs.clear(origin_fp);
   else
      h2_task::I().stubs.clear(origin_fp);
}

static inline void h2_mock_g(void* mock)
{
   if (h2_task::I().current_case)
      h2_task::I().current_case->mocks.add(mock);
   else if (h2_task::I().current_suite)
      h2_task::I().current_suite->mocks.add(mock);
   else
      h2_task::I().mocks.add(mock);
}

static inline void h2_assert_g()
{
   if (h2_task::I().current_case) h2_task::I().current_case->asserts += 1;
   if (h2_task::I().current_suite) h2_task::I().current_suite->stats.asserts += 1;
   h2_task::I().stats.asserts += 1;
}

static inline void h2_fail_g(h2_fail* fail, bool defer)
{
   if (!fail) return;
   if (O.debug) h2_debugger::trap();
   if (h2_task::I().current_case) h2_task::I().current_case->do_fail(fail, defer);
}
// source/assert/h2_assert.hpp

struct h2_defer_failure : h2_once {
   const char* assert_type;
   const char *e_expression, *a_expression, *expression;
   const char* file;
   int line;
   h2_fail* fails{nullptr};
   h2_ostringstream oss;

   h2_defer_failure(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_) : e_expression(e_expression_), a_expression(a_expression_), expression(expression_), file(file_), line(line_) {}
   ~h2_defer_failure();
};

static inline h2_ostringstream& h2_OK(h2_defer_failure* d, bool a)
{
   d->assert_type = "OK1";
   if (!a) d->fails = h2_fail::new_unexpect("true", "false");
   h2_assert_g();
   return d->oss;
}

template <typename E, typename A>
static inline h2_ostringstream& h2_OK(h2_defer_failure* d, E e, A a, int n = 0)
{
   d->assert_type = "OK2";
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   h2_fail* fail = m.matches((typename h2_decay<A>::type)a, n);
   d->fails = fail;
   if (fail && fail->subling_next) {
      d->fails = h2_fail::new_unexpect();
      h2_fail::append_child(d->fails, fail);
   }
   h2_assert_g();
   return d->oss;
}

static inline h2_ostringstream& h2_JE(h2_defer_failure* d, h2_string e, h2_string a, h2_string selector)
{
   d->assert_type = "JE";
   h2::h2_matcher<h2_string> m = Je(e, selector);
   d->fails = m.matches(a);
   h2_assert_g();
   return d->oss;
}
// source/other/h2_report.hpp

struct h2_report {
   h2_singleton(h2_report);
   static void initialize();

   h2_list reports;
   void on_task_start(h2_task* t);
   void on_task_endup(h2_task* t);
   void on_suite_start(h2_suite* s);
   void on_suite_endup(h2_suite* s);
   void on_case_start(h2_suite* s, h2_case* c);
   void on_case_endup(h2_suite* s, h2_case* c);
};
}  // namespace h2

// source/stub/h2_use.hpp

#define __H2STUB2(Origin, Substitute) h2::h2_stub_g(h2::h2_fp(Origin), (void*)Substitute, #Origin, __FILE__, __LINE__)
#define __H2STUB4(Function, ReturnType, Arguments, Substitute) h2::h2_stub_g(h2::h2_fp((H2PP_REMOVE_PARENTHESES_IF(ReturnType)(*) Arguments)H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Substitute), #Function, __FILE__, __LINE__)
#define __H2STUB5(Class, Method, ReturnType, Arguments, Substitute) h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(Substitute), #Class "::" #Method, __FILE__, __LINE__)
#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__)

#define __H2UNSTUB1(Origin) h2::h2_unstub_g(h2::h2_fp(Origin))
#define __H2UNSTUB3(Function, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_fp((H2PP_REMOVE_PARENTHESES_IF(ReturnType)(*) Arguments)H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB4(Class, Method, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define H2UNSTUB(...) H2PP_VARIADIC_CALL(__H2UNSTUB, __VA_ARGS__)

////////////////////////////////////////////////////////////////

#define __H3STUB2(Origin, Substitute) \
   h2::h2_stub_g(h2::h2_fp(Origin), (void*)Substitute, #Origin, __FILE__, __LINE__)

#define ____H3STUB3(Function, ReturnType, Arguments, Q)                                             \
   struct {                                                                                         \
      void operator=(ReturnType(*substitute_fp) Arguments)                                          \
      {                                                                                             \
         h2::h2_stub_g(h2::h2_fp(Function), (void*)(substitute_fp), #Function, __FILE__, __LINE__); \
      }                                                                                             \
   } Q;                                                                                             \
   Q = [] Arguments -> ReturnType /* captureless lambda implicit cast to function pointer */

#define __H3STUB3(Function, ReturnType, Arguments) ____H3STUB3(Function, ReturnType, Arguments, H2PP_UNIQUE(t_stub3))

#define __H3STUB40(Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                              \
   struct {                                                                                                                                                                                                                              \
      void operator=(ReturnType (*substitute_fp)(H2PP_REMOVE_PARENTHESES_IF(Class) *))                                                                                                                                                   \
      {                                                                                                                                                                                                                                  \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                                  \
   } Q;                                                                                                                                                                                                                                  \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> ReturnType

#define __H3STUB41(Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                              \
   struct {                                                                                                                                                                                                                              \
      void operator=(ReturnType (*substitute_fp)(H2PP_REMOVE_PARENTHESES_IF(Class) *, H2PP_REMOVE_PARENTHESES(Arguments)))                                                                                                               \
      {                                                                                                                                                                                                                                  \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                                  \
   } Q;                                                                                                                                                                                                                                  \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> ReturnType

#define __H3STUB4(Class, Method, ReturnType, Arguments) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Arguments, __H3STUB40(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub40)), __H3STUB41(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub41)))

#define STUBS(...) H2PP_VARIADIC_CALL(__H3STUB, __VA_ARGS__)
// source/mock/h2_use.hpp

#define __H2ARGV(Args, i) H2PP_STR(H2PP_CAT(H2PP_TH, i) Args)
#define H2ARGV(...) H2PP_REPEAT((, ), __H2ARGV, __VA_ARGS__, H2PP_NARG __VA_ARGS__)

#define __H2MOCK_0(...) H2PP_VARIADIC_CALL(__H2MOCK_0_, __VA_ARGS__)  // normal function
#define __H2MOCK_1(...) H2PP_VARIADIC_CALL(__H2MOCK_1_, __VA_ARGS__)  // class member method

/* clang-format off */
//       0th       1th         2th          3th          4th
// MOCK( Function ,ReturnType ,Arguments  [,Inspect0     ... ] )
// MOCK( Class    ,Method     ,ReturnType  ,Arguments  [,Inspect0 ... ] )

// `ReturnType` MAY (...); `Arguments` MUST (...); `Inspect0` MUST NOT (...) or absent
// Distinguish between normal function and class member method :
//   if 3th enclosed with parentheses than is class member method else is normal function
#define H2MOCK(...) H2PP_CAT(__H2MOCK_, H2PP_IS_BEGIN_PARENTHESIS(H2PP_TH3(__VA_ARGS__))) (__VA_ARGS__)

#define H2UNMOCK H2UNSTUB

#define __H3MOCK_0(...) H2PP_VARIADIC_CALL(__H3MOCK_0_, __VA_ARGS__)
#define __H3MOCK_1(...) H2PP_VARIADIC_CALL(__H3MOCK_1_, __VA_ARGS__)

#define MOCKS(...) H2PP_CAT(__H3MOCK_, H2PP_IS_BEGIN_PARENTHESIS(H2PP_TH3(__VA_ARGS__))) (__VA_ARGS__)

//////// >>>>> generated by build/generate.py
#define __H2MOCK_0_3(Function, ReturnType, Arguments) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, "", __FILE__, __LINE__) = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_4(Function, ReturnType, Arguments, Inspect0) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0, __FILE__, __LINE__).Inspect0 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_5(Function, ReturnType, Arguments, Inspect0, Inspect1) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1, __FILE__, __LINE__).Inspect0.Inspect1 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_6(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_7(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_8(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_9(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_10(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_11(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_12(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_13(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_14(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_15(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_16(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_17(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_18(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_19(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_20(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_21(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_22(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_23(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_24(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_25(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_26(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_27(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_28(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_29(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_30(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_31(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_32(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_33(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_0_34(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29, Inspect30) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29 " " #Inspect30, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29.Inspect30 = [] Arguments -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_4_1(Class, Method, ReturnType, Arguments) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, "", __FILE__, __LINE__) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_4_0(Class, Method, ReturnType, Arguments) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, "", __FILE__, __LINE__) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_5_1(Class, Method, ReturnType, Arguments, Inspect0) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0, __FILE__, __LINE__).Inspect0 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_5_0(Class, Method, ReturnType, Arguments, Inspect0) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0, __FILE__, __LINE__).Inspect0 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_6_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1, __FILE__, __LINE__).Inspect0.Inspect1 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_6_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1, __FILE__, __LINE__).Inspect0.Inspect1 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_7_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_7_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_8_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_8_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_9_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_9_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_10_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_10_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_11_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_11_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_12_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_12_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_13_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_13_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_14_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_14_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_15_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_15_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_16_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_16_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_17_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_17_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_18_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_18_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_19_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_19_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_20_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_20_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_21_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_21_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_22_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_22_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_23_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_23_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_24_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_24_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_25_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_25_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_26_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_26_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_27_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_27_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_28_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_28_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_29_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_29_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_30_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_30_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_31_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_31_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_32_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_32_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_33_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_33_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_34_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_34_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_35_1(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29, Inspect30) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29 " " #Inspect30, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29.Inspect30 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_35_0(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29, Inspect30) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29 " " #Inspect30, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29.Inspect30 = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2MOCK_1_4(Class, Method, ReturnType, Arguments) H2PP_CAT(__H2MOCK_1_4_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments)
#define __H2MOCK_1_5(Class, Method, ReturnType, Arguments, Inspect0) H2PP_CAT(__H2MOCK_1_5_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0)
#define __H2MOCK_1_6(Class, Method, ReturnType, Arguments, Inspect0, Inspect1) H2PP_CAT(__H2MOCK_1_6_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1)
#define __H2MOCK_1_7(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2) H2PP_CAT(__H2MOCK_1_7_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2)
#define __H2MOCK_1_8(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3) H2PP_CAT(__H2MOCK_1_8_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3)
#define __H2MOCK_1_9(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4) H2PP_CAT(__H2MOCK_1_9_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4)
#define __H2MOCK_1_10(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) H2PP_CAT(__H2MOCK_1_10_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5)
#define __H2MOCK_1_11(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) H2PP_CAT(__H2MOCK_1_11_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6)
#define __H2MOCK_1_12(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) H2PP_CAT(__H2MOCK_1_12_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7)
#define __H2MOCK_1_13(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) H2PP_CAT(__H2MOCK_1_13_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8)
#define __H2MOCK_1_14(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) H2PP_CAT(__H2MOCK_1_14_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9)
#define __H2MOCK_1_15(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10) H2PP_CAT(__H2MOCK_1_15_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10)
#define __H2MOCK_1_16(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11) H2PP_CAT(__H2MOCK_1_16_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11)
#define __H2MOCK_1_17(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12) H2PP_CAT(__H2MOCK_1_17_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12)
#define __H2MOCK_1_18(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13) H2PP_CAT(__H2MOCK_1_18_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13)
#define __H2MOCK_1_19(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14) H2PP_CAT(__H2MOCK_1_19_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14)
#define __H2MOCK_1_20(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15) H2PP_CAT(__H2MOCK_1_20_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15)
#define __H2MOCK_1_21(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16) H2PP_CAT(__H2MOCK_1_21_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16)
#define __H2MOCK_1_22(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17) H2PP_CAT(__H2MOCK_1_22_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17)
#define __H2MOCK_1_23(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18) H2PP_CAT(__H2MOCK_1_23_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18)
#define __H2MOCK_1_24(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19) H2PP_CAT(__H2MOCK_1_24_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19)
#define __H2MOCK_1_25(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20) H2PP_CAT(__H2MOCK_1_25_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20)
#define __H2MOCK_1_26(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21) H2PP_CAT(__H2MOCK_1_26_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21)
#define __H2MOCK_1_27(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22) H2PP_CAT(__H2MOCK_1_27_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22)
#define __H2MOCK_1_28(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23) H2PP_CAT(__H2MOCK_1_28_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23)
#define __H2MOCK_1_29(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24) H2PP_CAT(__H2MOCK_1_29_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24)
#define __H2MOCK_1_30(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25) H2PP_CAT(__H2MOCK_1_30_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25)
#define __H2MOCK_1_31(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26) H2PP_CAT(__H2MOCK_1_31_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26)
#define __H2MOCK_1_32(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27) H2PP_CAT(__H2MOCK_1_32_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27)
#define __H2MOCK_1_33(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28) H2PP_CAT(__H2MOCK_1_33_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28)
#define __H2MOCK_1_34(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29) H2PP_CAT(__H2MOCK_1_34_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29)
#define __H2MOCK_1_35(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29, Inspect30) H2PP_CAT(__H2MOCK_1_35_, H2PP_IS_EMPTY Arguments)(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29, Inspect30)

#define __H3MOCK_0_3(Function, ReturnType, Arguments) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, "", __FILE__, __LINE__)
#define __H3MOCK_0_4(Function, ReturnType, Arguments, Inspect0) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0, __FILE__, __LINE__).Inspect0
#define __H3MOCK_0_5(Function, ReturnType, Arguments, Inspect0, Inspect1) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1, __FILE__, __LINE__).Inspect0.Inspect1
#define __H3MOCK_0_6(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2
#define __H3MOCK_0_7(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3
#define __H3MOCK_0_8(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4
#define __H3MOCK_0_9(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5
#define __H3MOCK_0_10(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6
#define __H3MOCK_0_11(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7
#define __H3MOCK_0_12(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8
#define __H3MOCK_0_13(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9
#define __H3MOCK_0_14(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10
#define __H3MOCK_0_15(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11
#define __H3MOCK_0_16(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12
#define __H3MOCK_0_17(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13
#define __H3MOCK_0_18(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14
#define __H3MOCK_0_19(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15
#define __H3MOCK_0_20(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16
#define __H3MOCK_0_21(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17
#define __H3MOCK_0_22(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18
#define __H3MOCK_0_23(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19
#define __H3MOCK_0_24(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20
#define __H3MOCK_0_25(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21
#define __H3MOCK_0_26(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22
#define __H3MOCK_0_27(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23
#define __H3MOCK_0_28(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24
#define __H3MOCK_0_29(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25
#define __H3MOCK_0_30(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26
#define __H3MOCK_0_31(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27
#define __H3MOCK_0_32(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28
#define __H3MOCK_0_33(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29
#define __H3MOCK_0_34(Function, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29, Inspect30) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_fp(Function), #ReturnType, #Function, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29 " " #Inspect30, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29.Inspect30
#define __H3MOCK_1_4(Class, Method, ReturnType, Arguments) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, "", __FILE__, __LINE__)
#define __H3MOCK_1_5(Class, Method, ReturnType, Arguments, Inspect0) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0, __FILE__, __LINE__).Inspect0
#define __H3MOCK_1_6(Class, Method, ReturnType, Arguments, Inspect0, Inspect1) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1, __FILE__, __LINE__).Inspect0.Inspect1
#define __H3MOCK_1_7(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2
#define __H3MOCK_1_8(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3
#define __H3MOCK_1_9(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4
#define __H3MOCK_1_10(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5
#define __H3MOCK_1_11(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6
#define __H3MOCK_1_12(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7
#define __H3MOCK_1_13(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8
#define __H3MOCK_1_14(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9
#define __H3MOCK_1_15(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10
#define __H3MOCK_1_16(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11
#define __H3MOCK_1_17(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12
#define __H3MOCK_1_18(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13
#define __H3MOCK_1_19(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14
#define __H3MOCK_1_20(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15
#define __H3MOCK_1_21(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16
#define __H3MOCK_1_22(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17
#define __H3MOCK_1_23(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18
#define __H3MOCK_1_24(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19
#define __H3MOCK_1_25(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20
#define __H3MOCK_1_26(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21
#define __H3MOCK_1_27(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22
#define __H3MOCK_1_28(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23
#define __H3MOCK_1_29(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24
#define __H3MOCK_1_30(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25
#define __H3MOCK_1_31(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26
#define __H3MOCK_1_32(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27
#define __H3MOCK_1_33(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28
#define __H3MOCK_1_34(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29
#define __H3MOCK_1_35(Class, Method, ReturnType, Arguments, Inspect0, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9, Inspect10, Inspect11, Inspect12, Inspect13, Inspect14, Inspect15, Inspect16, Inspect17, Inspect18, Inspect19, Inspect20, Inspect21, Inspect22, Inspect23, Inspect24, Inspect25, Inspect26, Inspect27, Inspect28, Inspect29, Inspect30) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #ReturnType, #Class "::" #Method, {H2ARGV(Arguments)}, #Inspect0 " " #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " #Inspect8 " " #Inspect9 " " #Inspect10 " " #Inspect11 " " #Inspect12 " " #Inspect13 " " #Inspect14 " " #Inspect15 " " #Inspect16 " " #Inspect17 " " #Inspect18 " " #Inspect19 " " #Inspect20 " " #Inspect21 " " #Inspect22 " " #Inspect23 " " #Inspect24 " " #Inspect25 " " #Inspect26 " " #Inspect27 " " #Inspect28 " " #Inspect29 " " #Inspect30, __FILE__, __LINE__).Inspect0.Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9.Inspect10.Inspect11.Inspect12.Inspect13.Inspect14.Inspect15.Inspect16.Inspect17.Inspect18.Inspect19.Inspect20.Inspect21.Inspect22.Inspect23.Inspect24.Inspect25.Inspect26.Inspect27.Inspect28.Inspect29.Inspect30
//////// <<<<< generated by build/generate.py
// source/core/h2_use.hpp

#define __H2SUITE(name, h2_suite_test)                                          \
   static void h2_suite_test(h2::h2_suite*, h2::h2_case*);                      \
   static h2::h2_suite H2PP_UNIQUE()(name, &h2_suite_test, __FILE__, __LINE__); \
   static void h2_suite_test(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5)

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2PP_UNIQUE(h2_suite_test))

#define H2Setup() if (case_2_0_1_7_0_3_2_5)
#define H2Teardown() if (::setjmp(suite_2_0_1_3_0_1_0_2->ctx))
#define H2Cleanup() if (::setjmp(suite_2_0_1_3_0_1_0_2->ctx))

#define __H2Case(name, c, todo)                                                                               \
   static h2::h2_case c(name, __FILE__, __LINE__, todo);                                                      \
   static h2::h2_suite::registor H2PP_UNIQUE()(suite_2_0_1_3_0_1_0_2, &c);                                    \
   if (&c == case_2_0_1_7_0_3_2_5)                                                                            \
      for (h2::h2_suite::cleaner _1_9_8_0_(suite_2_0_1_3_0_1_0_2); _1_9_8_0_; case_2_0_1_7_0_3_2_5 = nullptr) \
         for (h2::h2_case::cleaner _1_9_8_1_(&c); _1_9_8_1_;)                                                 \
            if (!::setjmp(c.ctx))

#define H2Case(...) __H2Case(#__VA_ARGS__, H2PP_UNIQUE(s_case), 0)
#define H2Todo(...) __H2Case(#__VA_ARGS__, H2PP_UNIQUE(s_case), 1)

#define __H2CASE(name, h2_case_test, h2_suite_test, todo)                                            \
   static void h2_case_test();                                                                       \
   static void h2_suite_test(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5) \
   {                                                                                                 \
      static h2::h2_case c(name, __FILE__, __LINE__, todo);                                          \
      static h2::h2_suite::registor r(suite_2_0_1_3_0_1_0_2, &c);                                    \
      if (&c == case_2_0_1_7_0_3_2_5)                                                                \
         for (h2::h2_case::cleaner t(&c); t;)                                                        \
            if (!::setjmp(c.ctx))                                                                    \
               h2_case_test();                                                                       \
   }                                                                                                 \
   static h2::h2_suite H2PP_UNIQUE(s_suite)(nullptr, &h2_suite_test, __FILE__, __LINE__);            \
   static void h2_case_test()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(h2_case_test), H2PP_UNIQUE(h2_suite_test), 0)
#define H2TODO(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(h2_case_test), H2PP_UNIQUE(h2_suite_test), 1)

/* clang-format off */

#define _H2_An(...) H2PP_CAT(_H2_An_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2_An_0(a, ...) a
#define _H2_An_1(...) 0

#define _H2_An_ForEach(...) H2PP_CAT(_H2_An_ForEach_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2_An_ForEach_1(...) 0
#define _H2_An_ForEach_0(a, ...) H2PP_CAT(_H2_An_ForEach_0_, H2PP_IS_BEGIN_PARENTHESIS(a))(a)
#define _H2_An_ForEach_0_0(a) _H2_An(a)
#define _H2_An_ForEach_0_1(a) _H2_An(H2PP_REMOVE_PARENTHESES(a))

#define _H2_An_Fullmeshx(...) _H2_An_ForEach(H2PP_HEAD(__VA_ARGS__))
#define _H2_An_Fullmeshy(...) _H2_An_ForEach(H2PP_LAST(__VA_ARGS__))

#define ___H2CASES_Macro(Qc, i, x) CASE(i. x) { Qc(x); }
#define __H2CASES_Macro(...) ___H2CASES_Macro(__VA_ARGS__)
#define _H2CASES_Macro(args, i, x) __H2CASES_Macro(H2PP_REMOVE_PARENTHESES(args), i, x)
#define __H2CASES(Qc, ...)                            \
   template <typename T>                              \
   void Qc(T x);                                      \
   H2PP_FOREACH(, _H2CASES_Macro, (Qc), __VA_ARGS__); \
   template <typename T>                              \
   void Qc(T x)
#define H2CASES(...) __H2CASES(H2PP_UNIQUE(f_cases), __VA_ARGS__)

#define ___H2CASESS_Macro(Qc, i, j, x, y) CASE(i.j. x, y) { Qc(x, y); }
#define __H2CASESS_Macro(...) ___H2CASESS_Macro(__VA_ARGS__)
#define _H2CASESS_Macro(args, i, j, x, y) __H2CASESS_Macro(H2PP_REMOVE_PARENTHESES(args), i, j, x, y)
#define __H2CASESS(Qc, ...)                             \
   template <typename Tx, typename Ty>                  \
   void Qc(Tx x, Ty y);                                 \
   H2PP_FULLMESH(, _H2CASESS_Macro, (Qc), __VA_ARGS__); \
   template <typename Tx, typename Ty>                  \
   void Qc(Tx x, Ty y)
#define H2CASESS(...) __H2CASESS(H2PP_UNIQUE(f_casess), __VA_ARGS__)

#define ___H2Cases_Macro(Qj, Qb, Ql, Qx, i, x) \
   H2Case(i. x)                                \
   {                                           \
      if (!::setjmp(Qj)) {                     \
         Qx = x;                               \
         Qb = true;                            \
         goto Ql;                              \
      }                                        \
   };                                          \
   Qb = false;
#define __H2Cases_Macro(...) ___H2Cases_Macro(__VA_ARGS__)
#define _H2Cases_Macro(args, i, x) __H2Cases_Macro(H2PP_REMOVE_PARENTHESES(args), i, x)
#define __H2Cases(Qj, Qb, Ql, Qx, ...)                            \
   jmp_buf Qj;                                                    \
   bool Qb = false;                                               \
   auto Qx = _H2_An_ForEach(__VA_ARGS__);                         \
   H2PP_FOREACH(, _H2Cases_Macro, (Qj, Qb, Ql, Qx), __VA_ARGS__); \
   Ql:                                                            \
   for (auto x = Qx; Qb; Qb = false, ::longjmp(Qj, 1))
#define H2Cases(...) __H2Cases(H2PP_UNIQUE(j), H2PP_UNIQUE(b), H2PP_UNIQUE(l), H2PP_UNIQUE(x), __VA_ARGS__)

#define ___H2Casess_Macro(Qj, Qb, Ql, Qx, Qy, i, j, x, y) \
   H2Case(i.j. x, y)                                      \
   {                                                      \
      if (!::setjmp(Qj)) {                                \
         Qx = x;                                          \
         Qy = y;                                          \
         Qb = true;                                       \
         goto Ql;                                         \
      }                                                   \
   };                                                     \
   Qb = false;
#define __H2Casess_Macro(...) ___H2Casess_Macro(__VA_ARGS__)
#define _H2Casess_Macro(args, i, j, x, y) __H2Casess_Macro(H2PP_REMOVE_PARENTHESES(args), i, j, x, y)
#define __H2Casess(Qj, Qb, Ql, Qx, Qy, ...)                             \
   jmp_buf Qj;                                                          \
   bool Qb = false;                                                     \
   auto Qx = _H2_An_Fullmeshx(__VA_ARGS__);                             \
   auto Qy = _H2_An_Fullmeshy(__VA_ARGS__);                             \
   H2PP_FULLMESH(, _H2Casess_Macro, (Qj, Qb, Ql, Qx, Qy), __VA_ARGS__); \
   Ql:                                                                  \
   for (auto x = Qx; Qb; ::longjmp(Qj, 1))                              \
      for (auto y = Qy; Qb; Qb = false)
#define H2Casess(...) __H2Casess(H2PP_UNIQUE(j), H2PP_UNIQUE(b), H2PP_UNIQUE(l), H2PP_UNIQUE(x), H2PP_UNIQUE(y), __VA_ARGS__)

#define ___H2CASES_T_Macro(Qc, i, x) CASE(i. x) { Qc<x>(); }
#define __H2CASES_T_Macro(...) ___H2CASES_T_Macro(__VA_ARGS__)
#define _H2CASES_T_Macro(args, i, x) __H2CASES_T_Macro(H2PP_REMOVE_PARENTHESES(args), i, x)
#define __H2CASES_T(Qc, ...)                            \
   template <typename x>                                \
   void Qc();                                           \
   H2PP_FOREACH(, _H2CASES_T_Macro, (Qc), __VA_ARGS__); \
   template <typename x>                                \
   void Qc()
#define H2CASES_T(...) __H2CASES_T(H2PP_UNIQUE(f_casest), __VA_ARGS__)

#define ___H2CASESS_T_Macro(Qc, i, j, x, y) CASE(i.j. x, y) { Qc<x, y>(); }
#define __H2CASESS_T_Macro(...) ___H2CASESS_T_Macro(__VA_ARGS__)
#define _H2CASESS_T_Macro(args, i, j, x, y) __H2CASESS_T_Macro(H2PP_REMOVE_PARENTHESES(args), i, j, x, y)
#define __H2CASESS_T(Qc, ...)                             \
   template <typename x, typename y>                      \
   void Qc();                                             \
   H2PP_FULLMESH(, _H2CASESS_T_Macro, (Qc), __VA_ARGS__); \
   template <typename x, typename y>                      \
   void Qc()
#define H2CASESS_T(...) __H2CASESS_T(H2PP_UNIQUE(f_casesst), __VA_ARGS__)

#define __H2GlobalCallback(name, Q)                        \
   namespace {                                             \
      static struct Q {                                    \
         Q() { h2::h2_task::I().name##s.push_back(name); } \
         static void name();                               \
      } H2PP_UNIQUE();                                     \
   }                                                       \
   void Q::name()

#define H2GlobalSetup() __H2GlobalCallback(global_setup, H2PP_UNIQUE(Global_Setup))
#define H2GlobalCleanup() __H2GlobalCallback(global_cleanup, H2PP_UNIQUE(Global_Cleanup))

#define H2GlobalSuiteSetup() __H2GlobalCallback(global_suite_setup, H2PP_UNIQUE(Global_Suite_Setup))
#define H2GlobalSuiteCleanup() __H2GlobalCallback(global_suite_cleanup, H2PP_UNIQUE(Global_Suite_Cleanup))

#define H2GlobalCaseSetup() __H2GlobalCallback(global_case_setup, H2PP_UNIQUE(Global_Case_Setup))
#define H2GlobalCaseCleanup() __H2GlobalCallback(global_case_cleanup, H2PP_UNIQUE(Global_Case_Cleanup))
// source/assert/h2_use.hpp

#define __H2OK(Qt, expression, ...) \
   for (h2::h2_defer_failure Qt("", "", expression, __FILE__, __LINE__); Qt;) h2::h2_OK(&Qt, __VA_ARGS__)

#define __H2JE3(Qt, expect, actual) \
   for (h2::h2_defer_failure Qt(#expect, #actual, "", __FILE__, __LINE__); Qt;) h2::h2_JE(&Qt, expect, actual, "")

#define __H2JE4(Qt, expect, actual, selector) \
   for (h2::h2_defer_failure Qt(#expect, #actual, "", __FILE__, __LINE__); Qt;) h2::h2_JE(&Qt, expect, actual, selector)

#define H2OK(...) __H2OK(H2PP_UNIQUE(t_defer_failure), (#__VA_ARGS__), __VA_ARGS__)

#define H2JE(...) H2PP_VARIADIC_CALL(__H2JE, H2PP_UNIQUE(t_defer_failure), __VA_ARGS__)

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
#endif
