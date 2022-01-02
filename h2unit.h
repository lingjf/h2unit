
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */

#ifndef __H2UNIT_H__
#define __H2UNIT_H__
#define H2UNIT_VERSION 5.17
#define H2UNIT_DATE 2022-01-02
#define H2UNIT_REVISION branches/v5/9b29b4d30ff8a25bf1c579e0b260ac68e123ef78
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

#include <cctype>    /* tolower, isspace */
#include <cstdarg>   /* va_list */
#include <errno.h>   /* strerror */
#include <exception> /* std::exception */
#include <iostream>  /* std::cout, std::streambuf */
#include <regex>     /* std::regex */
#include <signal.h>  /* sigaction */
#include <time.h>    /* clock */
#include <typeinfo>  /* std::typeid, std::type_info */

#if !defined _MSC_VER
#include <cxxabi.h> /* abi::__cxa_demangle, abi::__cxa_throw */
#endif

#if defined __linux || defined __APPLE__  // -MSVC -Cygwin -MinGW
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
#define strncasecmp _strnicmp
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
#define h2_stdcall __stdcall
#else
#define h2_stdcall
#endif

#if defined _WIN32 || defined __CYGWIN__  // +MinGW
#if defined __H2UNIT_HPP__ || defined H2UNIT_IMPORT_MAIN
int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#else
int main(int argc, const char** argv);
#endif
#else
__attribute__((weak)) int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#endif

namespace h2 {
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
   while (!sorted.empty()) push_back(sorted.pop());
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
struct h2_similarity {
   static size_t levenshtein(const char* s1, const char* s2, size_t n1, size_t n2, bool caseless)
   {
      h2_vector<size_t> col(n2 + 1), prevCol(n2 + 1);
      for (h2_vector<size_t>::size_type i = 0; i < prevCol.size(); ++i)
         prevCol[i] = (int)i;
      for (size_t i = 0; i < n1; ++i) {
         col[0] = i + 1;
         for (size_t j = 0; j < n2; ++j) {
            bool eq = caseless ? ::tolower(s1[i]) == ::tolower(s2[j]) : s1[i] == s2[j];
            col[j + 1] = std::min(std::min(1 + col[j], 1 + prevCol[1 + j]), prevCol[j] + (eq ? 0 : 1));
         }
         col.swap(prevCol);
      }
      return prevCol[n2];
   }

   // 1 absolute match, 0 absolute not match
   static double estimate(const char* s1, const char* s2, bool caseless)
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
   struct matrix {
      unsigned e : 1, p : 1, d : 6, c : 24;
      matrix() : e(0), p(0), d(0), c(0) {}
   };
   h2_vector<h2_vector<matrix>> m;

   h2_LCS(h2_vector<h2_string> s1_, h2_vector<h2_string> s2_, bool caseless_ = false) : s1(s1_), s2(s2_), caseless(caseless_) {}

   void LCS_cacluate()
   {
      for (size_t i = 1; i < s1.size() + 1; ++i) {
         for (size_t j = 1; j < s2.size() + 1; ++j) {
            if (s1[i - 1].equals(s2[j - 1], caseless)) {
               m[i][j].c = m[i - 1][j - 1].c + 1;
               m[i][j].e = 1;
            } else {
               if (m[i - 1][j].c > m[i][j - 1].c) {
                  m[i][j].c = m[i - 1][j].c;
               } else {
                  m[i][j].c = m[i][j - 1].c;
               }
               m[i][j].e = 0;
            }
         }
      }
   }

   void LCS_traceback(size_t i, size_t j)
   {
      if (i == 0 || j == 0) return;

      if (m[i][j].e || (m[i - 1][j - 1].c >= m[i - 1][j].c && m[i - 1][j - 1].c >= m[i][j - 1].c)) {
         m[i][j].d = 10;  // 10:30 upper left
      } else if (m[i - 1][j].c > m[i][j - 1].c) {
         m[i][j].d = 12;  // 12:00 upper
      } else {
         m[i][j].d = 9;  // 9:00 left
      }

      if (m[i][j].d == 10) {  // 10:30
         m[i][j].p = m[i][j].e;
         LCS_traceback(i - 1, j - 1); // i--, j--
      } else if (m[i][j].d == 12) {  // 12:00 upper i--
         LCS_traceback(i - 1, j);
      } else {  // 9:00 left j--
         LCS_traceback(i, j - 1);
      }
   }

   std::pair<h2_vector<unsigned char>, h2_vector<unsigned char>> lcs()
   {
      for (size_t i = 0; i < s1.size() + 1; ++i) m.push_back(h2_vector<matrix>(s2.size() + 1));
      LCS_cacluate();
      LCS_traceback(s1.size(), s2.size());

      h2_vector<unsigned char> l1(s1.size()), l2(s2.size());
      for (size_t i = 1; i < s1.size() + 1; ++i) {
         l1[i - 1] = 0;
         for (size_t j = 1; j < s2.size() + 1; ++j)
            if (m[i][j].p) l1[i - 1] = 1;
      }
      for (size_t j = 1; j < s2.size() + 1; ++j) {
         l2[j - 1] = 0;
         for (size_t i = 1; i < s1.size() + 1; ++i)
            if (m[i][j].p) l2[j - 1] = 1;
      }
      return {l1, l2};
   }
};
// source/utils/h2_misc.cpp
static inline bool h2_blank(const char* str)
{
   for (; str && *str; ++str)
      if (!::isspace(*str)) return false;
   return true;
}

static inline const char* h2_basefile(const char* path)
{
   if (path)
      for (const char* p = path + strlen(path); path <= p; --p)
         if (*p == '/' || *p == '\\')
            return p + 1;
   return path;
}

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

static inline char* strcpyn(char* dst, const char* src, size_t len)
{
   strncpy(dst, src, len);
   dst[len] = '\0';
   return dst;
}

static inline const char* strip_left(const char* left, const char* right = nullptr)  // [left, right)
{
   while ((right ? left < right : true) && *left && ::isspace(*left)) ++left;
   return left;
}

static inline const char* strip_right(const char* left, const char* right = nullptr)  // [left, right)
{
   if (!right) right = left + strlen(left);
   for (--right; left <= right && (!*right || ::isspace(*right));) --right;
   return right + 1;
}

static inline const char* get_keyvalue(const char* attributes, const char* key)
{
   const char* p_key = strcasestr(attributes, key);
   if (!p_key) return nullptr;
   const char* p_eq = strip_left(p_key + strlen(key));
   if (*p_eq != '=') return "";
   return strip_left(p_eq + 1);
}

static inline int hex2byte(char c)
{
   return '0' <= c && c <= '9' ? c - '0' : ('A' <= c && c <= 'F' ? c - 'A' + 10 : ('a' <= c && c <= 'f' ? c - 'a' + 10 : -1));
}

static inline size_t hex2bytes(const char* hex, unsigned char* bytes)
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
         bytes[i / 2] = (unsigned char)((hex2byte(b) << 4) + hex2byte(hex[i]));
      b = hex[i];
   }
   return c / 2;
}

static inline const char* index_th(size_t sequence, size_t shift = 1)
{
   static const char* st[] = {"0th", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th"};
   static char ss[64];

   sequence += shift;
   if (sequence < sizeof(st) / sizeof(st[0])) return st[sequence];
   sprintf(ss, "%dth", (int)sequence);
   return ss;
}

static inline size_t number_strlen(size_t number, int base)
{
   unsigned long long _10000000 = 1;
   for (size_t i = 1;; ++i) {
      _10000000 *= base;
      if (number < _10000000) return i;
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

static inline bool h2_in(const char* a, int n, ...)
{
   va_list ap;
   va_start(ap, n);
   for (int i = 0; i < n; ++i)
      if (!strcmp(va_arg(ap, const char*), a)) return true;
   va_end(ap);
   return false;
}

static inline const char* h2_candidate(const char* a, int n, ...)
{
   int c1 = 0, c2 = 0;
   const char *totals[32], *matches[32];

   va_list ap;
   va_start(ap, n);
   for (; c1 < n; ++c1) {
      totals[c1] = va_arg(ap, const char*);
      if (!strncasecmp(totals[c1], a, strlen(a))) matches[c2++] = totals[c1];
   }
   va_end(ap);

   if (c2 == 1) return matches[0];

   static char ss[1024];
   sprintf(ss, "%s argument: \033[31m%s\033[0m, %s: ", c2 ? "ambiguous" : "invalid", a, c2 ? "candidates" : "availables");
   for (int i = 0; i < (c2 ? c2 : c1); ++i)
      sprintf(ss + strlen(ss), "%s\033[32m%s\033[0m", comma_if(i, " | "), c2 ? matches[i] : totals[i]);
   return ss;
}

#define h2_sprintvf(str, fmt, ap)             \
   va_list bp;                                \
   va_copy(bp, ap);                           \
   int _len = vsnprintf(nullptr, 0, fmt, bp); \
   str = (char*)alloca(_len + 1);             \
   va_end(bp);                                \
   _len = vsnprintf(str, _len + 1, fmt, ap);

#define h2_sprintf(str, fmt)  \
   va_list ap;                \
   va_start(ap, fmt);         \
   h2_sprintvf(str, fmt, ap); \
   va_end(ap);
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

h2_inline h2_string& h2_string::sprintf(const char* fmt, ...)
{
   char* alloca_str;
   h2_sprintf(alloca_str, fmt);
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

h2_inline size_t h2_string::width(size_t fat) const  // wcwidth()/wcswidth()
{
   size_t w = 0, n = 0;
   for (const char* p = c_str(); *p != '\0'; p += n) {
      n = utf8len(p);
      w += (n == 1 ? 1 : fat);
   }
   return w;
}

h2_inline bool h2_string::equals(const h2_string& fulstr, bool caseless) const
{
   if (!caseless) return *this == fulstr;
   return tolower() == fulstr.tolower();
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

h2_inline bool h2_string::enclosed(char left, char right) const
{
   if (right == '\0') right = left;
   return front() == left && back() == right;
}

h2_inline h2_string h2_string::unenclose(char left, char right) const
{
   if (!enclosed(left, right)) return *this;
   return h2_string(size() - 2, c_str() + 1);
}

h2_inline h2_string h2_string::escape(bool utf8) const
{
   h2_string s;
   for (auto& c : *this)
      switch (c) {
         case '\n': s.append(utf8 ? "␍" : "\\n"); break;
         case '\r': s.append(utf8 ? "␊" : "\\r"); break;
         case '\t': s.append(utf8 ? "␉" : "\\t"); break;
         case '\0': s.append(utf8 ? "␀" : "\0"); break;
         default: s.push_back(c); break;
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
   return s;
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

h2_inline h2_string h2_string::centre(size_t width) const
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
// source/utils/h2_color.cpp
struct h2_color {
   h2_singleton(h2_color);
   char current[8][32]{{'\0'}};

   void clear_style()
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i) current[i][0] = '\0';
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
      char a[256], *p = a;
      p += sprintf(p, "\033[%d;", style2value("reset"));
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            p += sprintf(p, "%d;", style2value(current[i]));
      *(p - 1) = 'm';
      LIBC__write(-21371647, a, (size_t)(p - a));
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
   { /* Windows PowerShell works, but CMD not, refer to v5.11 SetConsoleTextAttribute */
      if (isctrl(str)) {
         if (h2_option::I().colorful) {
            I().parse(str);
            I().change();
         }
      } else LIBC__write(-21371647, str, strlen(str));
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

   static bool isctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
// source/utils/h2_line.cpp
h2_inline size_t h2_line::width(bool ignore_indent) const
{
   size_t w = 0;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         if (!ignore_indent || !h2_blank(word.c_str()))
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

h2_inline bool h2_line::enclosed(char left, char right) const
{
   if (right == '\0') right = left;
   bool f = false, ff = false, b = false;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str())) {
         if (!ff) f = word.front() == left;
         ff = true;
         b = word.back() == right;
      }
   return f && b;
}

h2_inline bool h2_line::has(const char* word) const
{
   for (auto& t : *this)
      if (t == word) return true;
   return false;
}

h2_inline h2_line h2_line::gray_quote() const
{
   if (!enclosed('\"', '\"') && !enclosed('\'', '\'')) return *this;

   h2_line line;
   size_t i = 0, w = width();

   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         line.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i == 0) h.append(c.c_str());
         else if (i == w - 1) t.append(c.c_str());
         else m.append(c.c_str());
         i += c.width();
      }
      if (h.size()) line += delta(h, "dark gray");
      if (m.size()) line.push_back(m);
      if (t.size()) line += delta(t, "dark gray");
   }

   return line;
}

h2_inline h2_line h2_line::abbreviate(size_t width, size_t tail) const
{
   h2_line line1, line2;
   for (auto& word : *this)
      if (h2_color::isctrl(word.c_str())) line1.push_back(word);
      else line1.push_back(word.escape());

   size_t i = 0, line1_width = line1.width();
   if (line1_width <= width) return line1;

   for (auto& word : line1) {
      if (h2_color::isctrl(word.c_str())) {
         line2.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i < width - 3 - tail) h.append(c.c_str());
         else if (i == width - 3 - tail) m = "...";
         else if (line1_width - tail <= i) t.append(c.c_str());
         i += c.width();
      }
      if (h.size()) line2.push_back(h);
      if (m.size()) line2 += delta(m, "dark gray");
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
         if (!h2_blank(word.c_str()) && !h2_color::isctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.size();
   return sum < width;
}

h2_inline h2_line h2_lines::folds() const
{
   h2_line folded_line;
   for (auto& line : *this)
      for (auto& word : line)
         if (!h2_blank(word.c_str()))  // drop indent
            folded_line.push_back(word);
   return folded_line;
}

h2_inline h2_string h2_lines::string() const
{
   h2_string s;
   for (auto& line : *this)
      for (auto& word : line)
         if (!h2_blank(word.c_str()) && !h2_color::isctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_lines::sequence(size_t indent, size_t start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = delta(h2_string("%d. ", (int)(i + start)), "dark gray") + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_lines::samesizify(h2_lines& a, h2_lines& b)
{
   auto size = std::max(a.size(), b.size());
   for (auto i = a.size(); i < size; ++i) a.push_back(h2_line());
   for (auto i = b.size(); i < size; ++i) b.push_back(h2_line());
}
// source/stdio/h2_console.cpp
struct h2_console {
   static size_t width()
   {
      static size_t s_width = 0;
      if (s_width == 0) {
         s_width = 120;
#if defined _WIN32
         CONSOLE_SCREEN_BUFFER_INFO csbi;
         GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
         auto columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
         if (16 < columns) s_width = columns;
#else
         struct winsize w;
         if (-1 != ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
            if (16 < w.ws_col) s_width = w.ws_col;
#endif
      }
      return s_width;
   }

   static void prints(const char* style, const char* format, ...)
   {
      if (style && strlen(style)) {
         char t[128];
         sprintf(t, "\033{%s}", style);
         h2_color::I().print(t);
      }

      char* alloca_str;
      h2_sprintf(alloca_str, format);
      h2_color::I().print(alloca_str);

      if (style && strlen(style)) h2_color::I().print("\033{reset}");
   }

   static void printl(const h2_line& line, bool cr = true)
   {
      for (auto& word : line) h2_color::I().print(word.c_str());
      if (cr) h2_color::I().print("\n");
   }

   static void printl(const h2_lines& lines, bool cr = true)
   {
      for (auto& line : lines) printl(line, cr);
   }
};
// source/stdio/h2_stdio.cpp
struct h2_stdio {
   h2_singleton(h2_stdio);
   h2_string* buffer;
   bool stdout_capturable = false, stderr_capturable = false, syslog_capturable = false;
   size_t capture_length = 0;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      if (O.verbose >= VerboseNormal || (fd != fileno(stdout) && fd != fileno(stderr))) {
         h2::h2_stub_temporary_restore _((void*)LIBC__write);
         if ((fd == fileno(stdout) || fd == fileno(stderr)) && h2_report::I().backable) {
            LIBC__write(fd, "\n", 1);  // fall printf/cout into new line from report title
            h2_report::I().backable = false;
         }
         LIBC__write(fd == -21371647 ? fileno(stdout) : fd, buf, count);
         if (fd == fileno(stdout) || fd == fileno(stderr)) I().capture_length += count;
      }
      if ((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr))) I().buffer->append((char*)buf, count);
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

   static void initialize()
   {
      ::setbuf(stdout, 0);  // unbuffered
      I().buffer = new h2_string();
      static h2_list stubs;

      h2_stubs::add(stubs, (void*)::printf, (void*)printf, "printf", H2_FILINE);
      h2_stubs::add(stubs, (void*)::vprintf, (void*)vprintf, "vprintf", H2_FILINE);
      h2_stubs::add(stubs, (void*)::putchar, (void*)putchar, "putchar", H2_FILINE);
      h2_stubs::add(stubs, (void*)::puts, (void*)puts, "puts", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fprintf, (void*)fprintf, "fprintf", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::vfprintf, (void*)vfprintf,  "vfprintf",H2_FILINE);
      h2_stubs::add(stubs, (void*)::fputc, (void*)fputc, "fputc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::putc, (void*)fputc, "fputc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fputs, (void*)fputs, "fputs", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fwrite, (void*)fwrite, "fwrite", H2_FILINE);
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
      h2_stubs::add(stubs, (void*)LIBC__write, (void*)write, "write", H2_FILINE);
#if !defined _WIN32
      h2_stubs::add(stubs, (void*)::syslog, (void*)syslog, "syslog", H2_FILINE);
      h2_stubs::add(stubs, (void*)::vsyslog, (void*)vsyslog, "vsyslog", H2_FILINE);
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

h2_inline h2_cout::h2_cout(h2_matcher<const char*> m_, const char* e_, const char* type_, const char* filine_) : filine(filine_), m(m_), e(e_), type(type_)
{
   bool all = !strlen(type);
   h2_stdio::I().start_capture(all || get_keyvalue(type, "out"), all || get_keyvalue(type, "err"), all || get_keyvalue(type, "syslog"));
}

h2_inline h2_cout::~h2_cout()
{
   h2_runner::asserts();
   h2_fail* fail = m.matches(h2_stdio::I().stop_capture(), 0);
   if (fail) {
      fail->filine = filine;
      fail->assert_type = "OK";
      fail->e_expression = e;
      fail->a_expression = "";
      fail->explain = "COUT";
      h2_runner::failing(fail);
   }
}
// source/symbol/h2_nm.cpp
#if !defined _MSC_VER
static inline void __nm(h2_list& symbols, bool demangle)
{
   h2_memory::hook(false);
   char cmd[256], line[2048], addr[128], type, symbol[2048];
   sprintf(cmd, "nm --defined-only %s -n %s", demangle ? "-f bsd --demangle" : "", O.path);
   FILE* f = ::popen(cmd, "r");
   if (f) {
      while (::fgets(line, sizeof(line) - 1, f)) {
         if (3 != sscanf(line, "%s %c %[^\n]", addr, &type, symbol)) continue;
         if (strchr("bBcCdDiIuU", type)) continue;  // reject bBcCdDiIuU, accept tTwWsSvV, sS for vtable
         symbols.push_back((new h2_symbol(symbol, strtoull(addr, nullptr, 16)))->x);
      }
      ::pclose(f);
   }
   h2_memory::hook();
}
static inline bool __leading_underscore(h2_list& symbols)
{
   bool _main = false;
   h2_list_for_each_entry (p, symbols, h2_symbol, x) {
      if (!strcmp("main", p->symbol)) return false;
      if (!strcmp("_main", p->symbol)) _main = true;
   }
   return _main;
}
static inline void nm(bool demangle, h2_list& symbols, bool& leading_underscore)
{
   __nm(symbols, demangle);
   h2_once_if() leading_underscore = __leading_underscore(symbols);
}
#endif

h2_inline char* h2_symbol::name()
{
   return h2_nm::I().leading_underscore && symbol[0] == '_' ? symbol + 1 : symbol;  // remove leading underscore '_' in MacOS
}

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
   if (I().demangle_symbols.empty()) nm(true, I().demangle_symbols, I().leading_underscore);

   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      if (!strcmp(p->name(), name)) {
         res[0] = p;
         return 1;
      }
   }
   int count = 0;
   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      char* parentheses = strchr(p->name(), '(');
      if (!parentheses) continue;
      if (!strncmp_reverse(p->name(), parentheses, name, name + len, len)) continue;  // compare function name
      char* func = parentheses - len;
      if (p->name() < func && (func[-1] != ':' || func[-2] != ':')) continue;  // strip namespace
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
   strcpy(s_symbol.symbol, symbol->Name);
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
   if (I().mangle_symbols.empty()) nm(false, I().mangle_symbols, I().leading_underscore);

   h2_list_for_each_entry (p, I().mangle_symbols, h2_symbol, x)
      if (!strcmp(p->name(), name)) return p->addr;

   return 0;
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
   if (relative_vtable == 0) h2_console::prints("yellow", "\nDon't find vtable for h2::h2_vtable_test %s\n", vtable_symbol);
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
#if !defined _WIN32  // -MinGW
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
   if (O.os == OsMacOS ? !memcmp(buf, "0x", 2) : !!strstr(buf, "??:")) return nullptr;
   *(char*)strip_right(buf) = '\0';
#endif
   return buf;
}
#endif

#if !(defined _MSC_VER || defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__)
static inline bool backtrace_extract(const char* line, char* mangle_name, unsigned long long* displacement = nullptr)
{
   unsigned long long _t;
#if defined __APPLE__
   // MAC: `3   a.out  0x000000010e777f3d _ZN2h24unit6mallocEm + 45
   if (2 == ::sscanf(line, "%*s%*s%*s%s + %llu", mangle_name, displacement ? displacement : &_t)) return true;
#else
   // Linux: `./a.out(_ZN2h24unit7executeEv+0x131)[0x55aa6bb840ef]
   if (2 == ::sscanf(line, "%*[^(]%*[^_a-zA-Z]%1023[^)+]+0x%llx", mangle_name, displacement ? displacement : &_t)) return true;
   // Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   mangle_name[0] = '\0';
   if (1 == ::sscanf(line, "%*[^(]%*[^+]+0x%llx", displacement ? displacement : &_t)) return true;
#endif
   return false;
}
#endif

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
      if (O.verbose >= VerboseDetail || O.os != OsMacOS) p = addr2line(h2_load::ptr_to_addr(frames[i])); /* atos is slow */
      if (!p) p = h2_cxa::demangle(mangle_name, demangle_name);
      if (!p || !strlen(p)) {
         p = symbols[i];
         h2_once_if() h2_console::prints("yellow", "\nAdd -g to compiler options, -rdynamic to linker options\n");
      }
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
   for (auto& c : stacks) lines.push_back(c.startswith("h2::") || c.contains(": h2::") ? color(c, "dark gray") : h2_line(c));
   lines.sequence(pad);
   h2_console::printl(lines);
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
struct tinyexpr {
   static bool evaluate(const char* expression, double& value)
   {
      syntax_tree* st = compile(expression);
      if (!st) return false;
      value = st->evaluate();
      delete st;
      return true;
   }

   struct function_spec {
      int parameters;
      const char* fn;
      const void* fp;
   };

   struct syntax_tree : h2_libc {
      double value = 0;
      const function_spec* function = nullptr;
      syntax_tree* parameters[4]{nullptr};
      syntax_tree(double value_) : value(value_) {}
      syntax_tree(const function_spec* function_, syntax_tree* _0 = nullptr, syntax_tree* _1 = nullptr, syntax_tree* _2 = nullptr, syntax_tree* _3 = nullptr) : function(function_), parameters{_0, _1, _2, _3} {}
      ~syntax_tree()
      {
         for (int i = 0; i < 4; ++i)
            if (parameters[i]) delete parameters[i];
      }
      double evaluate()
      {
         if (!function) return value;
         switch (function->parameters) {
            case 0: return ((double (*)())function->fp)();
            case 1: return ((double (*)(double))function->fp)(parameters[0]->evaluate());
            case 2: return ((double (*)(double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate());
            case 3: return ((double (*)(double, double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate(), parameters[2]->evaluate());
            case 4: return ((double (*)(double, double, double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate(), parameters[2]->evaluate(), parameters[3]->evaluate());
            default: return NAN;
         }
      }
   };

   static constexpr int TOK_NULL = 0, TOK_ERROR = 1, TOK_END = 2, TOK_COMMA = 3, TOK_OPEN = 4, TOK_CLOSE = 5, TOK_NUMBER = 6, TOK_FUNCTION = 7, TOK_ADD = 8, TOK_SUB = 9, TOK_MMD = 10, TOK_POW = 11, TOK_FAC = 12;
   struct parser_state {
      const char *start, *next;
      int type;
      double value;
      const function_spec* function;
   };

   static double _add(double a, double b) { return a + b; }
   static double _sub(double a, double b) { return a - b; }
   static double _mul(double a, double b) { return a * b; }
   static double _div(double a, double b) { return a / b; }
   static double _negate(double a) { return -a; }
   static double _comma(double, double b) { return b; }
   static double __pi() { return 3.14159265358979323846; }
   static double __e() { return 2.71828182845904523536; }
   static double __nan() { return NAN; }
   static double __inf() { return INFINITY; }

   static double _factorial(double n)
   {  // n! = 1*2*3...*n , 0!=1
      if (n < 0.0) return NAN;
      unsigned long result = 1;
      for (unsigned long i = 1; i <= (unsigned long)(n); ++i) result *= i;
      return (double)result;
   }
   static double Pnm(double n, double m)
   {  // P(n,m) = n(n-1)(n-2)...(n-m+1)
      if (n < 0.0 || m < 0.0) return NAN;
      unsigned long result = 1;
      for (unsigned long i = std::max(n, m) - std::min(n, m) + 1; i <= std::max(n, m); ++i) result *= i;
      return result;
   }
   static double Cnm(double n, double m)
   {  // C(n,m) = n(n-1)(n-2)...(n-m+1)/m!
      return Pnm(n, m) / _factorial(std::min(n, m));
   }

#ifdef _MSC_VER
#pragma function(ceil)
#pragma function(floor)
#endif

   static const function_spec* get_function(const char* name, int len = 0)
   {
      static const function_spec functions[] = {
        {2, "+", (const void*)_add},
        {2, "-", (const void*)_sub},
        {2, "*", (const void*)_mul},
        {2, "/", (const void*)_div},
        {1, "negate", (const void*)_negate},
        {2, "comma", (const void*)_comma},
        {0, "pi", (const void*)__pi},
        {0, "e", (const void*)__e},
        {0, "nan", (const void*)__nan},
        {0, "inf", (const void*)__inf},
        {1, "factorial", (const void*)_factorial},
        {2, "C", (const void*)Cnm},
        {2, "P", (const void*)Pnm},
        {2, "A", (const void*)Pnm},
        {1, "abs", (const void*)(double (*)(double))::fabs},
        {1, "sqrt", (const void*)(double (*)(double))::sqrt},
        {1, "ln", (const void*)(double (*)(double))::log},
        {1, "log", (const void*)(double (*)(double))::log},
        {1, "log10", (const void*)(double (*)(double))::log10},
        {1, "log2", (const void*)(double (*)(double))::log2},
        {1, "floor", (const void*)(double (*)(double))::floor},
        {1, "ceil", (const void*)(double (*)(double))::ceil},
        {2, "fmod", (const void*)(double (*)(double, double))::fmod},
        {2, "pow", (const void*)(double (*)(double, double))::pow},
        {1, "exp", (const void*)(double (*)(double))::exp},
        {1, "cos", (const void*)(double (*)(double))::cos},
        {1, "acos", (const void*)(double (*)(double))::acos},
        {1, "cosh", (const void*)(double (*)(double))::cosh},
        {1, "sin", (const void*)(double (*)(double))::sin},
        {1, "asin", (const void*)(double (*)(double))::asin},
        {1, "sinh", (const void*)(double (*)(double))::sinh},
        {1, "tan", (const void*)(double (*)(double))::tan},
        {1, "atan", (const void*)(double (*)(double))::atan},
        {2, "atan2", (const void*)(double (*)(double, double))::atan2},
        {1, "tanh", (const void*)(double (*)(double))::tanh},
      };
      if (len == 0) len = strlen(name);
      for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i)
         if (!strncmp(name, functions[i].fn, len) && '\0' == functions[i].fn[len])
            return &functions[i];
      return nullptr;
   }

   static void lexical_token(parser_state* ps)
   {
      ps->type = TOK_NULL;
      do {
         if (!ps->next[0]) {
            ps->type = TOK_END;
         } else if (::isdigit(ps->next[0]) || ps->next[0] == '.') { /* Reading a number. */
            ps->value = strtod(ps->next, (char**)&ps->next);
            ps->type = TOK_NUMBER;
         } else {
            if (::isalpha(ps->next[0])) { /* Look for a builtin function call. */
               const char* fn = ps->next;
               while (::isalpha(ps->next[0]) || ::isdigit(ps->next[0]) || (ps->next[0] == '_')) ps->next++;
               ps->function = get_function(fn, (int)(ps->next - fn));
               ps->type = TOK_FUNCTION;
               if (!ps->function) ps->type = TOK_ERROR;
            } else { /* Look for an operator or special character. */
               switch (ps->next++[0]) {
                  case '+': ps->type = TOK_ADD, ps->function = get_function("+"); break;
                  case '-': ps->type = TOK_SUB, ps->function = get_function("-"); break;
                  case '*': ps->type = TOK_MMD, ps->function = get_function("*"); break;
                  case '/': ps->type = TOK_MMD, ps->function = get_function("/"); break;
                  case '%': ps->type = TOK_MMD, ps->function = get_function("fmod"); break;
                  case '^': ps->type = TOK_POW, ps->function = get_function("pow"); break;
                  case '!': ps->type = TOK_FAC, ps->function = get_function("factorial"); break;
                  case '(': ps->type = TOK_OPEN; break;
                  case ')': ps->type = TOK_CLOSE; break;
                  case ',': ps->type = TOK_COMMA; break;
                  case ' ':
                  case '\t': break;  //  case '\n': case '\r':
                  default: ps->type = TOK_ERROR; break;
               }
            }
         }
      } while (ps->type == TOK_NULL);
   }

   static syntax_tree* base(parser_state* ps)
   { /* <base> = <constant> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
      syntax_tree* st;
      if (ps->type == TOK_NUMBER) {
         st = new syntax_tree(ps->value);
         lexical_token(ps);
      } else if (ps->type == TOK_OPEN) {
         lexical_token(ps);
         st = list(ps);
         if (ps->type != TOK_CLOSE) ps->type = TOK_ERROR;
         else lexical_token(ps);
      } else if (ps->type == TOK_FUNCTION) {
         st = new syntax_tree(ps->function);
         lexical_token(ps);
         if (st->function->parameters == 0) {
            if (ps->type == TOK_OPEN) {
               lexical_token(ps);
               if (ps->type != TOK_CLOSE) ps->type = TOK_ERROR;
               else lexical_token(ps);
            }
         } else if (st->function->parameters == 1) {
            st->parameters[0] = power(ps);
         } else {
            if (ps->type != TOK_OPEN) {
               ps->type = TOK_ERROR;
            } else {
               int i = 0;
               while (i < 4) {
                  lexical_token(ps);
                  st->parameters[i++] = expr(ps);
                  if (ps->type != TOK_COMMA) break;
               }
               if (ps->type != TOK_CLOSE || i != st->function->parameters) ps->type = TOK_ERROR;
               else lexical_token(ps);
            }
         }
      } else {
         st = new syntax_tree(NAN);
         ps->type = TOK_ERROR;
      }
      return st;
   }

   static syntax_tree* factorial(parser_state* ps)
   { /* <factorial> = <base> {"!"} */
      syntax_tree* st = base(ps);
      while (ps->type == TOK_FAC) {
         st = new syntax_tree(get_function("factorial"), st);
         lexical_token(ps);
      }
      return st;
   }

   static syntax_tree* power(parser_state* ps)
   { /* <power> = {("-" | "+")} <factorial> */
      int sign = 1;
      while (ps->type == TOK_ADD || ps->type == TOK_SUB) {
         if (ps->type == TOK_SUB) sign = -sign;
         lexical_token(ps);
      }
      syntax_tree* st = factorial(ps);
      if (sign == -1) st = new syntax_tree(get_function("negate"), st);
      return st;
   }

   static syntax_tree* factor(parser_state* ps)
   { /* <factor> = <power> {"^" <power>} */
      syntax_tree* st = power(ps);
      while (ps->type == TOK_POW) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, power(ps));
      }
      return st;
   }

   static syntax_tree* term(parser_state* ps)
   { /* <term> = <factor> {("*" | "/" | "%") <factor>} */
      syntax_tree* st = factor(ps);
      while (ps->type == TOK_MMD) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, factor(ps));
      }
      return st;
   }

   static syntax_tree* expr(parser_state* ps)
   { /* <expr> = <term> {("+" | "-") <term>} */
      syntax_tree* st = term(ps);
      while (ps->type == TOK_ADD || ps->type == TOK_SUB) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, term(ps));
      }
      return st;
   }

   static syntax_tree* list(parser_state* ps)
   { /* <list> = <expr> {"," <expr>} */
      syntax_tree* st = expr(ps);
      while (ps->type == TOK_COMMA) {
         lexical_token(ps);
         st = new syntax_tree(get_function("comma"), st, expr(ps));
      }
      return st;
   }

   static syntax_tree* compile(const char* expression)
   {
      parser_state ps;
      ps.start = ps.next = expression;
      lexical_token(&ps);
      syntax_tree* st = list(&ps);
      if (ps.type != TOK_END) delete st, st = nullptr;
      return st;
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

   h2_string format_value(const char* quote) const
   {
      switch (type) {
         case t_null: return "null";
         case t_boolean: return value_boolean ? "true" : "false";
         case t_number: return (value_double - ::floor(value_double) == 0) ? std::to_string((long long)value_double).c_str() : std::to_string(value_double).c_str();
         case t_string: return quote + value_string + quote;
         case t_pattern: return "/" + value_string + "/";
         case t_array:
         case t_object:
         default: return "";
      }
   }

   void format(int& _type, h2_string& _key, h2_string& _value, const char* quote = "") const
   {
      _type = type;
      if (key_string.size()) _key = quote + key_string + quote;
      _value = format_value(quote);
   }

   h2_lines format(bool fold, const char* quote = "", size_t depth = 0, int next = 0) const
   {
      h2_lines lines;
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back(quote + key_string + quote + ": ");
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
      const char* left = strip_left(start, start + size);
      const char* right = strip_right(left, start + size);
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
      for (p = json_string; *p && json_length--; ++p) {
         switch (state) {
            case st_idle:
               if (::isspace(*p)) continue;
               else if (strchr("{:}[,]", *p)) new_lexis(lexical, p, 1);
               else {
                  pending = p;
                  state = st_normal;
                  if ('\"' == *p) state = st_double_quote;
                  else if ('\'' == *p) state = st_single_quote;
                  else if ('/' == *p) state = st_pattern;
                  else if ('\\' == *p) stash_state = state, state = st_escape;
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
      if (s.enclosed('\"')) s = s.unenclose('\"');
      else if (s.enclosed('\'')) s = s.unenclose('\'');
      s = s.unescape();
      return s;
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
      if (node.value_string.enclosed('/')) node.value_string = node.value_string.unenclose('/');
      node.type = h2_json_node::t_pattern;
      return true;
   }

   bool parse_string_or_number(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (tinyexpr::evaluate(node.value_string.c_str(), node.value_double)) {
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
         if (i < lexical.size() && lexical[i].equals(",")) i++;
         else break;
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
         if (i < lexical.size() && lexical[i].equals(",")) ++i;
         else break;
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
      enum {
         st_idle,
         st_in_dot,
         st_in_bracket
      };
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
                  if (s < p) add(s, p, true);
                  // restart a new part
                  state = st_in_dot;
                  s = p + 1;
               } else if (*p == '[') {  // end a part
                  if (s < p) add(s, p, true);
                  // restart a new part
                  state = st_in_bracket;
                  s = p + 1;
               } else if (*p == '\0') {
                  if (s < p) add(s, p, true);
                  state = st_idle;
               }
               break;
            case st_in_bracket:
               if (*p == ']') {
                  if (s < p) add(s, p, false);
                  state = st_idle;
               }
               break;
         }
      } while (*p++);
   }

   void add(const char* start, const char* end, bool only_key)  // [start, end)
   {
      start = strip_left(start, end);
      end = strip_right(start, end);
      if (start < end) {
         if (!only_key) {
            if (strspn(start, "-0123456789") == (size_t)(end - start)) {
               values.push_back({atoi(start), ""});
               return;
            } else if ((*start == '\"' && *(end - 1) == '\"') || (*start == '\'' && *(end - 1) == '\'')) {
               ++start, --end;
            }
         }
         if (start < end) values.push_back({0, h2_string(end - start, start)});
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
      for (auto& c : select.values) node = c.key.size() ? node->get(c.key, caseless) : node->get(c.index);
      if (node) node->key_string = "";
      return node;
   }

   h2_line serialize() const
   {
      h2_line line;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i) line.printf("yellow,bold,underline", "%s%s ", comma_if(j, " "), lexical[j].c_str());
         else line.push_back(comma_if(j, " ") + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) line.printf("yellow,bold,underline", " ... ");
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
            if (!a->is_number()) return false;
            if (std::isnan(e->value_double) && std::isnan(a->value_double)) return true;
            if (std::isinf(e->value_double) && std::isinf(a->value_double)) return true;
            return ::fabs(e->value_double - a->value_double) < 0.00001;
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
         score += h2_similarity::estimate(e->value_string.c_str(), a->value_string.c_str(), caseless);
      } else {
      }
      if (e->key_string.size() || a->key_string.size()) {
         score = score * 0.5 + 0.5 * h2_similarity::estimate(e->key_string.c_str(), a->key_string.c_str(), caseless);
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
   int relationship, index = 0x7fffffff;
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
      if (e) e->format(e_type, e_key, e_value, "\"");
      if (a) a->format(a_type, a_key, a_value, "\"");
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

      if ((O.fold_json >= FoldSame && key_equal && value_match) || (O.fold_json >= FoldSingle && relationship < 0)) {
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
struct h2_json {
   static h2_lines dump(const h2_string& json_string)
   {
      h2_json_tree tree(json_string.c_str());
      if (tree.illformed) return {tree.serialize()};
      h2_lines lines = tree.format(O.fold_json, O.json_source_quote);
      if (!h2_blank(O.json_source_quote)) {
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

   static h2_string select(const h2_string& json_string, const h2_string& selector, bool caseless)
   {
      h2_json_tree tree(json_string.c_str());
      if (tree.illformed) return json_string;
      h2_json_node* node = tree.select(selector.c_str(), caseless);
      if (!node) return "";
      return node->format(O.fold_json, "\"").string();
   }

   static int match(const h2_string& expect, const h2_string& actual, bool caseless)
   {  // < 0 illformed json; = 0 matched; > 0 unmatched
      h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
      if (e_tree.illformed || a_tree.illformed) return -1;
      return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
   }

   static bool diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
   {
      h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
      if (e_tree.illformed || a_tree.illformed) return false;
      h2_json_dual dual(&e_tree, &a_tree, caseless);
      dual.align(e_lines, a_lines);
      return true;
   }
};
// source/matcher/h2_matches_memcmp.cpp
h2_inline bool h2_memcmp_util::bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits)
{
   for (size_t n = 0; n < nbits; ++n) {
      size_t i = n / 8, j = 7 - n % 8;
      if (((b1[i] >> j) & 1) != ((b2[i] >> j) & 1)) return false;
   }
   return true;
}

h2_inline bool h2_memcmp_util::is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; ++p)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

h2_inline bool h2_memcmp_util::is_bin_string(const char* s)
{
   for (const char* p = s; *p; ++p)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

h2_inline size_t h2_memcmp_util::bin_to_bits(const char* bin, unsigned char* bytes)
{
   memset(bytes, 0, strlen(bin));
   size_t c = 0;
   for (const char* p = bin; *p; ++p) {
      if (*p == ' ') continue;
      size_t i = c / 8, j = 7 - c % 8;
      ++c;
      unsigned char ebit = *p == '1' ? 1 : 0;
      bytes[i] = bytes[i] | (ebit << j);
   }
   return c;
}

h2_inline size_t h2_memcmp_util::hex_to_bytes(const char* hex, unsigned char* bytes) { return hex2bytes(hex, bytes); }
// source/matcher/h2_matches_strcmp.cpp
h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, const C& c) const
{
   h2_string _e = e, _a = a;
   if (c.squash_whitespace) _e = e.squash(), _a = a.squash();
   if (c.fit(_a.equals(_e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strcmp(_e, a, c.case_insensitive, expection(c));
}
h2_inline h2_line h2_matches_strcmp::expection(const C& c) const
{
   return c.pre("≠") + h2_stringify(c.squash_whitespace ? e.squash() : e, true);
}

h2_inline h2_line h2_matches_string::expection(const C& c) const
{
   return c.pre() + name + gray("(") + h2_stringify(e, true) + gray(")");
}

h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::regex_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::wildcard_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.contains(e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.startswith(e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.endswith(e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
// source/matcher/h2_matches_json.cpp
h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, c.case_insensitive);
   int ret = h2_json::match(e, _a, c.case_insensitive);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(c), c.case_insensitive, "illformed json");
   if (c.fit(ret == 0)) return nullptr;
   return h2_fail::new_json(e, _a, expection(c), c.case_insensitive);
}
h2_inline h2_line h2_matches_json::expection(const C& c) const
{
   return c.pre("≠") + h2_stringify(e);
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
      page_ptr = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_COMMIT, PAGE_READWRITE);
      if (page_ptr == NULL) h2_console::prints("yellow", "VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      page_ptr = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (page_ptr == MAP_FAILED) h2_console::prints("yellow", "mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      user_ptr = page_ptr + page_size * page_count - user_size_plus + alignment;

      mark_snowfield();
   }

   ~h2_piece()
   {
#if defined _WIN32
      VirtualFree(page_ptr, 0, MEM_RELEASE);
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
      if (permission & readable) new_permission = PAGE_READONLY;
      if (permission & writable) new_permission = PAGE_READWRITE;
      if (!VirtualProtect(forbidden_page, forbidden_size, new_permission, &old_permission)) h2_console::prints("yellow", "VirtualProtect failed %lu\n", GetLastError());
#else
      int new_permission = PROT_NONE;
      if (permission & readable) new_permission = PROT_READ;
      if (permission & writable) new_permission = PROT_READ | PROT_WRITE;
      if (::mprotect(forbidden_page, forbidden_size, new_permission) != 0) h2_console::prints("yellow", "mprotect failed %s\n", strerror(errno));
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
      if (fail && O.as_waring_memory_violate) fail->warning = true;
      return fail;
   }

   h2_fail* violate_fail()
   {
      if (violate_after_free) return h2_fail::new_use_after_free(user_ptr, violate_ptr, violate_action, bt_allocate, bt_release, violate_backtrace);
      else return h2_fail::new_overflow(user_ptr, user_size, violate_ptr, violate_action, h2_vector<unsigned char>(), bt_allocate, violate_backtrace);
   }

   h2_fail* check_asymmetric_free(const char* who_release)
   {
      if (h2_in(who_allocate, 5, "malloc", "calloc", "realloc", "posix_memalign", "aligned_alloc") && h2_in(who_release, 1, "free")) return nullptr;
      // if (h2_in(who_allocate, 2, "new", "new nothrow") && h2_in(who_release, 2, "delete", "delete nothrow")) return nullptr;
      // if (h2_in(who_allocate, 2, "new[]", "new[] nothrow") && h2_in(who_release, 2, "delete[]", "delete[] nothrow")) return nullptr;
      // some compiler new but delete[]
      if (h2_in(who_allocate, 4, "new", "new nothrow", "new[]", "new[] nothrow") && h2_in(who_release, 4, "delete", "delete nothrow", "delete[]", "delete[] nothrow")) return nullptr;

      if (bt_allocate.in(h2_exempt::I().fps)) return nullptr;
      auto fail = h2_fail::new_asymmetric_free(user_ptr, who_allocate, who_release, bt_allocate, bt_release);
      if (O.as_waring_memory_asymmetric_free) fail->warning = true;
      return fail;
   }

   h2_fail* check_double_free(h2_backtrace& bt)
   {
      if (!free_times++) {
         bt_release = bt;
         return nullptr;
      }
      auto fail = h2_fail::new_double_free(user_ptr, bt_allocate, bt_release, bt);
      if (O.as_waring_memory_double_free) fail->warning = true;
      return fail;
   }

   h2_fail* free(const char* who_release)
   {
      h2_fail* fail = check_double_free(h2_backtrace::dump(4));
      if (!fail && who_release) fail = check_asymmetric_free(who_release);
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
         for (auto it = sizes.begin(); it != sizes.end(); ++it)
            if (it->first == size)
               return it;
         return sizes.end();
      }

      void add(size_t size)
      {
         if (sizes.end() == find(size)) sizes.push_back({size, 0});
         find(size)->second++;
      }

      h2_fail* check(const char* where, const char* filine)
      {
         size_t s = 0;
         for (auto& p : sizes) s += p.first * p.second;
         return h2_fail::new_memory_leak(ptr, s, sizes, bt, where, filine);
      }
   };

   h2_vector<leak> leaks;

   h2_vector<leak>::iterator find(const h2_backtrace& bt)
   {
      for (auto it = leaks.begin(); it != leaks.end(); ++it)
         if (it->bt == bt)
            return it;
      return leaks.end();
   }

   void add(void* ptr, size_t size, const h2_backtrace& bt)
   {
      if (leaks.end() == find(bt)) leaks.push_back({ptr, bt});
      find(bt)->add(size);
   }

   h2_fail* check(const char* where, const char* filine)
   {
      h2_fail* fails = nullptr;
      for (auto& p : leaks) h2_fail::append_subling(fails, p.check(where, filine));
      return fails;
   }
};
// source/memory/h2_block.cpp
struct h2_block_attributes {
   unsigned long long limit = 0xffffffffffffull;
   size_t alignment = sizeof(void*);
   size_t n_fill = 0;
   unsigned char s_fill[32];
   bool noleak = false;

   size_t parse_fill(const char* p, unsigned char bytes[])
   {
      if (p[0] == '0' && ::tolower(p[1]) == 'x') {
         return hex2bytes(p + 2, bytes);
      } else {
         unsigned long long v = strtoull(p, nullptr, 10);
         if (v <= 0xFFULL) return *((unsigned char*)bytes) = (unsigned char)v, 1;
         else if (v <= 0xFFFFULL) return *((unsigned short*)bytes) = (unsigned short)v, 2;
         else if (v <= 0xFFFFFFFFULL) return *((unsigned int*)bytes) = (unsigned int)v, 4;
         else return *((unsigned long long*)bytes) = (unsigned long long)v, 8;
      }
   }

   h2_block_attributes(const char* attributes)
   {
      const char* p;
      if (get_keyvalue(attributes, "noleak")) noleak = true;
      if ((p = get_keyvalue(attributes, "limit"))) limit = strtod(p, nullptr);
      if ((p = get_keyvalue(attributes, "align"))) alignment = strtod(p, nullptr);
      if ((p = get_keyvalue(attributes, "fill"))) n_fill = parse_fill(p, s_fill);
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list pieces;

   h2_block_attributes attributes;
   unsigned long long footprint = 0, allocated = 0;
   const char* where;
   const char* filine;

   h2_block(const char* attributes_, const char* where_, const char* filine_) : attributes(attributes_), where(where_), filine(filine_) {}

   h2_fail* check()
   {
      h2_fail* fails = nullptr;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->violate_times)
            h2_fail::append_subling(fails, p->violate_fail());

      if (fails) {
         if (O.as_waring_memory_violate) fails->warning = true;
         return fails;
      }
      h2_leaky leaky;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (!attributes.noleak && !p->free_times)
            leaky.add(p->user_ptr, p->user_size, p->bt_allocate);

      fails = leaky.check(where, filine);
      if (fails) {
         if (O.as_waring_memory_leak) fails->warning = true;
         return fails;
      }
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
      size_t n_fill = attributes.n_fill;
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

   void push(const char* block_attributes, const char* where, const char* filine)
   {
      h2_block* b = new h2_block(block_attributes, where, filine);
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
      if (!at_exit && O.os != OsWindows)
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

   void rel_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->host_piece(ptr);
         if (piece) p->rel_piece(nullptr, piece);
      }
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
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("free", ptr));
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
      if (size == 0 && O.os == OsMacOS) size = 1;
      if (ptr) old_p = h2_stack::I().get_piece(ptr);
      if (size) new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (old_p && new_p) memcpy(new_p->user_ptr, old_p->user_ptr, std::min(old_p->user_size, size));
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("free", ptr));
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
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete(void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete nothrow", ptr));
   }
   static void operator delete[](void* ptr)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete[]", ptr));
   }
   static void operator delete[](void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete[] nothrow", ptr));
   }
};

struct h2_override_stdlib {
   h2_list stubs;

   static char* strdup(char* s)
   {
      char* ret = (char*)h2_override::malloc(strlen(s) + 1);
      if (ret) strcpy(ret, s);
      return ret;
   }

   void set()
   {
      h2_stubs::add(stubs, (void*)::free, (void*)h2_override::free, "free", H2_FILINE);
      h2_stubs::add(stubs, (void*)::malloc, (void*)h2_override::malloc, "malloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::realloc, (void*)h2_override::realloc, "realloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::calloc, (void*)h2_override::calloc, "calloc", H2_FILINE);
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
      h2_stubs::add(stubs, (void*)::posix_memalign, (void*)h2_override::posix_memalign, "posix_memalign", H2_FILINE);
#endif
#if defined _ISOC11_SOURCE
      h2_stubs::add(stubs, (void*)::aligned_alloc, (void*)h2_override::aligned_alloc, "aligned_alloc", H2_FILINE);
#endif
      // valloc pvalloc memalign deprecated
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), "new", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), "new nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), "new[]", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), "new[] nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), "delete", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), "delete nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), "delete[]", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), "delete[] nothrow", H2_FILINE);
   }

   void reset() { h2_stubs::clear(stubs); }
};
#if defined __linux
// source/memory/platform/h2_override_linux.cpp
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
// source/memory/platform/h2_override_macos.cpp
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
// source/memory/platform/h2_override_windows.cpp
// https://github.com/microsoft/mimalloc
// https://github.com/gperftools/gperftools

struct h2_override_platform {
   h2_list stubs;

   // windows specific free_base, free_dbg called by CRT internal functions or operator delete
   // windows specific _msize, _expand
   // A MS CRT "internal" function, implemented using _calloc_impl
   // obscured: _aligned_offset_malloc, _aligned_realloc, _aligned_recalloc, _aligned_offset_realloc, _aligned_offset_recalloc, _malloca, _freea ,_recalloc

   static void _free_base(void* ptr) { h2_override::free(ptr); }
   static void* _expand(void* memblock, size_t size) { return NULL; }
   static void* _aligned_malloc(size_t size, size_t alignment) { return h2_override::aligned_alloc(size, alignment); }
   static void _aligned_free(void* memblock) { h2_override::free(memblock); }
   // When _DEBUG _CRTDBG_MAP_ALLOC (default undefined) is defined CRT maps all to _*_dbg, bug CRT Debug version enabled.
   void set()
   {
      h2_stubs::add(stubs, (void*)::_free_base, (void*)_free_base, "_free_base", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_msize, (void*)h2_override::size, "_msize", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_expand, (void*)_expand, "_expand", H2_FILINE);
      //// h2_stubs::add(stubs,(void*)::_calloc_crt, (void*)h2_override::calloc, "_calloc_crt", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_aligned_malloc, (void*)_aligned_malloc, "_aligned_malloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_aligned_free, (void*)_aligned_free, "_aligned_free", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_strdup, (void*)h2_override_stdlib::strdup, "_strdup", H2_FILINE);  // strdup call to _strdup
#ifndef NDEBUG
      h2_stubs::add(stubs, (void*)::_free_dbg, (void*)free_dbg, "_free_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_malloc_dbg, (void*)malloc_dbg, "_malloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_realloc_dbg, (void*)realloc_dbg, "_realloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_calloc_dbg, (void*)calloc_dbg, "_calloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_expand_dbg, (void*)expand_dbg, "_expand_dbg", H2_FILINE);
#endif
   }
   static void free_dbg(void* userData, int blockType) { h2_override::free(userData); }
   static void* malloc_dbg(size_t size, int blockType, const char* filename, int linenumber) { return h2_override::malloc(size); }
   static void* realloc_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return h2_override::realloc(userData, newSize); }
   static void* calloc_dbg(size_t num, size_t size, int blockType, const char* filename, int linenumber) { return h2_override::calloc(num, size); }
   static size_t msize_dbg(void* userData, int blockType) { return h2_override::size(userData); }
   static void* expand_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return NULL; }

   void reset() { h2_stubs::clear(stubs); }
};
#else  // +MinGW
// source/memory/platform/h2_override_cygwin.cpp
struct h2_override_platform {
   h2_list stubs;

   static char* strndup(char* s, size_t n)
   {
      char* ret = (char*)h2_override::malloc(n + 1);
      if (ret) {
         strncpy(ret, s, n);
         ret[n] = '\0';
      }
      return ret;
   }

   void set()
   {
      h2_stubs::add(stubs, (void*)::strdup, (void*)h2_override_stdlib::strdup, "strdup", H2_FILINE);
#if defined __CYGWIN__
      h2_stubs::add(stubs, (void*)::strndup, (void*)strndup, "strndup", H2_FILINE);
#endif
   }
   void reset() { h2_stubs::clear(stubs); }
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

h2_inline void h2_memory::try_free(void* ptr)
{
   h2_stack::I().rel_piece(ptr);
}

h2_inline void h2_memory::stack::root()
{
   h2_stack::I().push("", "root", H2_FILINE);
}
h2_inline void h2_memory::stack::push(const char* filine)
{
   h2_stack::I().push("", "case", filine);
}
h2_inline h2_fail* h2_memory::stack::pop()
{
   return h2_stack::I().pop();
}
h2_inline long long h2_memory::stack::footprint()
{
   return h2_stack::I().top()->footprint;
}

h2_inline h2_memory::stack::block::block(const char* attributes, const char* filine)
{
   unmem = !!get_keyvalue(attributes, "unmem");
   if (unmem) h2_memory::hook(false);
   h2_stack::I().push(attributes, "block", filine);
}
h2_inline h2_memory::stack::block::~block()
{
   h2_runner::failing(h2_stack::I().pop());
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
   static h2_list stubs;

   h2_stubs::add(stubs, (void*)::gmtime, (void*)h2_exempt_stub::gmtime, "gmtime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::ctime, (void*)h2_exempt_stub::ctime, "ctime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::asctime, (void*)h2_exempt_stub::asctime, "asctime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::localtime, (void*)h2_exempt_stub::localtime, "localtime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::mktime, (void*)h2_exempt_stub::mktime, "mktime", H2_FILINE);

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
   add_by_fp(h2_numberfy<void*>(&std::type_info::name));
#else
   h2_stubs::add(stubs, (void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, "gmtime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, "ctime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, "asctime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, "localtime_r", H2_FILINE);
   add_by_fp((void*)::sscanf);
   add_by_fp((void*)::sprintf);
   add_by_fp((void*)::vsnprintf);

#if defined __APPLE__ && defined __clang__
   add_by_fp((void*)::snprintf);
   add_by_fp((void*)::snprintf_l);
   add_by_fp((void*)::strftime_l);
   add_by_fp((void*)::strtod_l);
   add_by_fp((void*)::strtold);
   add_by_fp((void*)::strtof_l);
#endif
#endif

#if defined __GNUC__
   add_by_fp((void*)abi::__cxa_demangle);
   add_by_fp((void*)abi::__cxa_throw);
   add_by_fp((void*)abi::__cxa_allocate_exception);
#endif

   add_by_fp((void*)h2_pattern::regex_match);
}

h2_inline void h2_exempt::add_by_name(const char* fn)
{
   h2_symbol* res[16];
   int n = h2_nm::get_by_name(fn, res, 16);
   for (int i = 0; i < n; ++i) add_by_fp(h2_load::addr_to_ptr(res[i]->addr));
}

h2_inline void h2_exempt::add_by_fp(void* fp)
{
   h2_array_append(I().fps, h2_cxa::follow_jmp(fp));
}
// source/except/h2_debug.cpp
static inline bool in_debugging()
{
   bool ret = false;
#if defined __linux
   char t[1024];
   FILE* f = ::fopen("/proc/self/status", "r");
   if (f) {
      while (::fgets(t, sizeof(t) - 1, f)) {
         if (strncmp(t, "TracerPid:\t", 11) == 0) {
            ret = t[11] != '\0' && t[11] != '0';
            break;
         }
      }
      ::fclose(f);
   }
#elif defined __APPLE__
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", (int)getpid());
   FILE* f = ::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r");
   if (f) {
      while (::fgets(t, sizeof(t) - 1, f)) {
         if (strstr(t, h2_basefile(O.path)) || strstr(t, attach_pid)) {
            ret = true;
            break;
         }
      }
      ::pclose(f);
   }
#endif
   return ret;
}

static inline bool h2_attach_debugger()
{
#if defined __linux || defined __APPLE__
   while (!in_debugging()) {
      h2_once_if()
      {
         char cmd[512];
         ::printf("\nEnter \033[33mpassword\033[0m for connecting \033[33m%s\033[0m. \n", O.os == OsMacOS ? "GDB" : "LLDB");
#if defined __linux
         sprintf(cmd, "sudo gdb --silent -ex cont --pid=%d", (int)getpid());
#elif defined __APPLE__
         sprintf(cmd, "sudo lldb -o 'continue' --attach-pid %d", (int)getpid());
#endif
         if (fork() == 0) exit(system(cmd));
      }
      h2_sleep(100);
   }
#endif
   return true;
}

#if defined __linux
#if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#define h2_raise_trap() asm volatile("int $3")
#else
#define h2_raise_trap() raise(SIGTRAP)
#endif
#elif defined __APPLE__
#if defined(__i386__) || defined(__x86_64__)
#define h2_raise_trap() __asm__("int $3\n" : :)
#elif defined(__aarch64__)
#define h2_raise_trap() __asm__(".inst 0xd4200000")
#endif
#else
#define h2_raise_trap() in_debugging()
#endif
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
struct h2_exception_handler {
#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
   static void RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments)
   {
      h2_exception::I().last_bt = h2_backtrace::dump(1);
      if (O.as_waring_exception) h2_runner::failing(h2_fail::new_exception("was thrown", "", h2_exception::I().last_bt, true));
      h2::h2_stub_temporary_restore t((void*)::RaiseException);
      ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
   }
#else
   static void __cxa_throw(void* thrown_exception, std::type_info* type_info, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      h2_exception::I().thrown_exception = thrown_exception;
      h2_exception::I().type_info = type_info;
      h2_exception::I().last_bt = h2_backtrace::dump(1);
      h2_cxa::demangle(type_info->name(), h2_exception::I().last_type);
      if (h2_exception::I().catching) ::longjmp(h2_exception::I().catch_hole, 1);
      if (O.as_waring_exception) h2_runner::failing(h2_fail::new_exception("was thrown", h2_exception::I().last_type, h2_exception::I().last_bt, true));
      h2::h2_stub_temporary_restore t((void*)abi::__cxa_throw);
      abi::__cxa_throw(thrown_exception, type_info, dest);
   }
#endif
};

h2_inline void h2_exception::initialize()
{
   static h2_list stubs;
#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
   h2_stubs::add(stubs, (void*)::RaiseException, (void*)h2_exception_handler::RaiseException, "RaiseException", H2_FILINE);
#else
   h2_stubs::add(stubs, (void*)abi::__cxa_throw, (void*)h2_exception_handler::__cxa_throw, "__cxa_throw", H2_FILINE);
#endif
   if (!O.debugger_trap) h2_crash::install();
}

h2_inline h2_once h2_exception::try_catch()
{
   h2_exception::I().catching = true;
   h2_exception::I().thrown_exception = nullptr;
   h2_exception::I().type_info = nullptr;

   return h2_once();
}

h2_inline void h2_exception::try_catch_finally(h2_fail* fail, const char* what)
{
   // abi::__cxa_free_exception(h2_exception::I().thrown_exception);
   if (what) h2_memory::try_free((void*)what);
   h2_exception::I().thrown_exception = nullptr;
   h2_exception::I().type_info = nullptr;
   h2_exception::I().catching = false;

   h2_runner::failing(fail);
}
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
   if (delta < -0x7fffffff || 0x7fffffff < delta) {  //x86_64 asm("movq $dstfp, %rax; jmpq %rax")
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
   h2_source(void* source_fp_, const char* srcfn, const char* filine) : source_fp(source_fp_)
   {
      if (!h2_e9_save(source_fp, origin_opcode)) {
         h2_console::prints("yellow", "STUB %s by %s() failed %s\n", srcfn, O.os == OsWindows ? "VirtualProtect" : "mprotect", filine);
         if (O.os == OsMacOS) h2_console::prints("", "try: "), h2_console::prints("green", "printf '\\x07' | dd of=%s bs=1 seek=160 count=1 conv=notrunc\n", O.path);
         if (O.os == OsLinux) h2_console::prints("", "try: "), h2_console::prints("green", "objcopy --writable-text %s\n", O.path);
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

   h2_source* add(void* fp, const char* srcfn, const char* filine)
   {
      void* source_fp = __follow(fp);
      h2_source* source = __find(source_fp);
      if (!source) {
         source = new h2_source(source_fp, srcfn, filine);
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

   h2_stub(void* srcfp_, const char* srcfn, const char* filine) : srcfp(srcfp_)
   {
      source = h2_sources::I().add(srcfp, srcfn, filine);
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

static inline h2_stub* h2_stubs_get(h2_list& stubs, void* srcfp)
{
   h2_list_for_each_entry (p, stubs, h2_stub, x)
      if (p->srcfp == srcfp)
         return p;
   return nullptr;
}

h2_inline bool h2_stubs::add(h2_list& stubs, void* srcfp, void* dstfp, const char* srcfn, const char* filine)
{
   h2_stub* stub = h2_stubs_get(stubs, srcfp);
   if (!stub) {
      stub = new h2_stub(srcfp, srcfn, filine);
      stubs.push(stub->x);
   }
   stub->stub(dstfp);
   return true;
}

h2_inline void h2_stubs::clear(h2_list& stubs, void* srcfp)
{
   h2_stub* stub = h2_stubs_get(stubs, srcfp);
   if (stub) {
      stub->x.out();
      delete stub;
   }
}

h2_inline void h2_stubs::clear(h2_list& stubs)
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
h2_inline h2_fail* h2_checkin::check(size_t index, size_t total, const char* srcfn) const
{
   if (is_satisfied() || is_saturated()) return nullptr;
   h2_line t = srcfn + gray("()") + " expected " + delta(expect(), "green") + " but actually " + delta(actual(), "red,bold") + " called";
   if (1 < total) t += gray(" when ") + index_th(index) + " " + color(expr, "cyan");
   return h2_fail::new_normal(t);
}

h2_inline const char* h2_checkin::actual() const
{
   static char st[64];
   if (call > 0) sprintf(st, "%d times", call);
   else sprintf(st, "never");
   return st;
}

h2_inline const char* h2_checkin::expect() const
{
   static char st[128];
   if (least == 0) {
      if (most == 0) sprintf(st, "never called");
      else if (most == 0x7fffffff) sprintf(st, "any number of times");
      else sprintf(st, "at most %d times", most);
   } else if (least == most) sprintf(st, "exactly %d times", least);
   else if (most == 0x7fffffff) sprintf(st, "at least %d times", least);
   else sprintf(st, "between %d and %d times", least, most);  // 0 < least < most < 0x7fffffff
   return st;
}
// source/mock/h2_mocker.cpp
h2_inline h2_line h2_mocker_base::argument(int seq, const char* def) const
{
   h2_line t;
   for (int i = 0; i < (int)argument_types.size(); ++i)
      t += gray(comma_if(i)) + color(argument_types[i], seq == i ? "red,bold" : def);
   return gray("(") + t + gray(")");
}

h2_inline h2_line h2_mocker_base::signature() const
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(srcfn, "green") + argument(-1, "cyan") + gray(">");
}

h2_inline void h2_mocker_base::mock()
{
   x.out();
   h2_runner::mock(this);
   h2_runner::stub(srcfp, dstfp, srcfn, filine);
}

h2_inline h2_fail* h2_mocker_base::check() const
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(i, checkin_array.size(), srcfn);
      if (fail) fail->seqno = (int)i;
      h2_fail::append_subling(fails, fail);
      h2_runner::asserts();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), filine);
   h2_fail::append_child(fail, fails);
   return fail;
}

h2_inline void h2_mocker_base::failing(h2_fail* fail, int checkin_offset) const
{
   fail->foreach([this, checkin_offset](h2_fail* f, size_t, size_t) {
      f->explain += gray("on ") + (srcfn + argument(f->seqno));
      if (1 < checkin_array.size()) f->explain += gray(" when ") + index_th((size_t)checkin_offset) + " " + color(checkin_array[checkin_offset].expr, "cyan");
   });
   h2_fail* fails = h2_fail::new_normal(signature(), filine);
   h2_fail::append_child(fails, fail);
   h2_runner::failing(fails);
}
// source/mock/h2_mocks.cpp
h2_inline bool h2_mocks::add(h2_list& mocks, void* mocker)
{
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x)
      if (p == (h2_mocker_base*)mocker) return false;
   mocks.push(((h2_mocker_base*)mocker)->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(h2_list& mocks, bool check)
{
   h2_fail* fails = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x) {
      if (check) h2_fail::append_subling(fails, p->check());
      p->reset();
      p->x.out();
   }
   return fails;
}
// source/net/h2_dns.cpp
struct h2_domain : h2_libc {
   h2_list x, y;
   h2_string name;
   h2_vector<h2_string> resolves;
   h2_domain(const char* _hostname) : name(_hostname) {}
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

   h2_domain* find(const char* hostname)
   {
      h2_list_for_each_entry (p, dnses, h2_domain, y)
         if (p->name == "*" || p->name == hostname)
            return p;
      return nullptr;
   }

   static int h2_stdcall getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res)
   {
      h2_domain* domain = I().find(hostname);
      if (!domain) return -1;

      static struct addrinfo addrinfos[32];
      static struct sockaddr_in sockaddrs[32];
      memset(addrinfos, 0, sizeof(addrinfos));
      memset(sockaddrs, 0, sizeof(sockaddrs));

      struct addrinfo** pp = res;
      for (size_t i = 0; i < domain->resolves.size(); ++i) {
         struct addrinfo* a = &addrinfos[i];
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(domain->resolves[i].c_str(), b)) {
            a->ai_addr = (struct sockaddr*)b;
            a->ai_addrlen = sizeof(struct sockaddr_in);
         } else
            a->ai_canonname = (char*)domain->resolves[i].c_str();
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

   static void h2_stdcall freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* h2_stdcall gethostbyname(char* hostname)
   {
      h2_domain* domain = I().find(hostname);
      if (!domain) return nullptr;

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

      for (size_t i = 0, a = 0, c = 0; i < domain->resolves.size(); ++i) {
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(domain->resolves[i].c_str(), b)) h_addr_list[a++] = (char*)&b->sin_addr;
         else h_aliases[c++] = (char*)domain->resolves[i].c_str();
      }
      return &h;
   }

   h2_list stubs;
   h2_resolver()
   {
      h2_stubs::add(stubs, (void*)::getaddrinfo, (void*)getaddrinfo, "getaddrinfo", H2_FILINE);
      h2_stubs::add(stubs, (void*)::freeaddrinfo, (void*)freeaddrinfo, "freeaddrinfo", H2_FILINE);
      h2_stubs::add(stubs, (void*)::gethostbyname, (void*)gethostbyname, "gethostbyname", H2_FILINE);
   }
   ~h2_resolver() { h2_stubs::clear(stubs); }
};

h2_inline void h2_dnses::add(h2_list& dnses, h2_list& name)
{
   dnses.push(name);
}

h2_inline void h2_dnses::clear(h2_list& dnses)
{
   h2_list_for_each_entry (p, dnses, h2_domain, x) {
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

   h2_domain* domain = new h2_domain(hostname);
   va_start(b, n);
   for (int i = 0; i < n; ++i)
      if ((p = va_arg(b, const char*)))
         if (strcmp(hostname, p))
            domain->resolves.push_back(p);
   va_end(b);

   h2_resolver::I().dnses.push(domain->y);
   if (h2_runner::I().current_case)
      h2_dnses::add(h2_runner::I().current_case->dnses, domain->x);
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

   static int h2_stdcall accept(int socket, struct sockaddr* address, socklen_t* address_len)
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
      if (tcp->data.size()) I().incoming.push(tcp->x);
      else delete tcp;

      return fd;
   }

   static int h2_stdcall connect(int socket, const struct sockaddr* address, socklen_t address_len)
   {
      I().sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      if (tcp->data.size()) I().incoming.push(tcp->x);
      else delete tcp;
      return 0;
   }

   static ssize_t h2_stdcall send(int socket, const void* buffer, size_t length, int flags)
   {
      I().put_outgoing(socket, (const char*)buffer, length);
      return (ssize_t)length;
   }
   static ssize_t h2_stdcall recv(int socket, void* buffer, size_t length, int flags)
   {
      ssize_t ret = 0;
      h2_packet* tcp = read_incoming(socket);
      if (tcp) {
         ret = (ssize_t)tcp->data.copy((char*)buffer, tcp->data.size(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t h2_stdcall sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len)
   {
      I().put_outgoing(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return (ssize_t)length;
   }
   static ssize_t h2_stdcall recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len)
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
   static ssize_t h2_stdcall sendmsg(int socket, const struct msghdr* message, int flags)
   {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
   static ssize_t h2_stdcall recvmsg(int socket, struct msghdr* message, int flags)
   {
      return recvfrom(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, &message->msg_namelen);
   }
#endif

   h2_list stubs;

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

      h2_stubs::add(stubs, (void*)::sendto, (void*)sendto, "sendto", H2_FILINE);
      h2_stubs::add(stubs, (void*)::recvfrom, (void*)recvfrom, "recvfrom", H2_FILINE);
#if !defined _WIN32
      h2_stubs::add(stubs, (void*)::sendmsg, (void*)sendmsg, "sendmsg", H2_FILINE);
      h2_stubs::add(stubs, (void*)::recvmsg, (void*)recvmsg, "recvmsg", H2_FILINE);
#endif
      h2_stubs::add(stubs, (void*)::send, (void*)send, "send", H2_FILINE);
      h2_stubs::add(stubs, (void*)::recv, (void*)recv, "recv", H2_FILINE);
      h2_stubs::add(stubs, (void*)::accept, (void*)accept, "accept", H2_FILINE);
      h2_stubs::add(stubs, (void*)::connect, (void*)connect, "connect", H2_FILINE);
   }
   void stop()
   {
      h2_stubs::clear(stubs);
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

h2_inline h2_packet* h2_sock::fetch()
{
   return h2_list_pop_entry(h2_socket::I().outgoing, h2_packet, x);
}

static inline void parse_iport(const char* s, char* iport)
{
   for (const char* p = s; p && *p; ++p) {
      if (::isdigit(*p) || *p == '.' || *p == ':' || *p == '*' || *p == '?') {
         *iport++ = *p;
         *iport = '\0';
      } else {
         if (!(::isspace(*p) || *p == '\"')) break;
      }
   }
}

h2_inline void h2_sock::inject(const void* packet, size_t size, const char* attributes)
{
   char from[256] = "", to[256] = "*";
   parse_iport(get_keyvalue(attributes, "from"), from);
   parse_iport(get_keyvalue(attributes, "to"), to);
   h2_socket::I().put_incoming(from, to, (const char*)packet, size);
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

h2_inline void h2_sock::failing(h2_fail* fail)
{
   h2_runner::asserts();
   h2_runner::failing(fail);
}
// source/core/h2_test.cpp
static inline void __split_describe(const char* describe, char* name_buf, char* tags_buf)
{
   strcpy(name_buf, describe);
   strcpy(tags_buf, "");

   const char* p = describe;
   for (bool quote = false; *p; ++p) {
      if (*p == '\"') quote = !quote;
      if (!quote && *p == '[') break;
   }
   if (*p != '[') return;  // left brace
   const char* q = p;
   for (bool quote = false; *q; ++q) {
      if (*q == '\"') quote = !quote;
      if (!quote && *q == ']') break;
   }
   if (*q != ']') return;  // right brace

   strncpy(tags_buf, p + 1, (size_t)(q - p - 1));
   tags_buf[(size_t)(q - p - 1)] = '\0';

   strcpy(&name_buf[(size_t)(p - describe)], q + 1);
}

h2_inline h2_test::h2_test(const char* filine_, const char* file_, int line_, const char* describe_) : filine(filine_), file(h2_basefile(file_)), line(line_), describe(describe_)
{
   if (describe) {
      __split_describe(describe, name_buf, tags_buf);
      name = strip_left(name_buf);
      for (char* t = strtok(tags_buf, " ,"); t; t = strtok(nullptr, " ,"))
         h2_array_append(tags, t);
   }
}

h2_inline bool h2_test::tagged(const char* tag_pattern) const
{
   for (int i = 0; tags[i]; ++i) {
      if (!strcasecmp(tags[i], tag_pattern)) return true;
      if (strcspn(tag_pattern, "?*[]") < strlen(tag_pattern) && h2_pattern::wildcard_match(tag_pattern, tags[i], true)) return true;
   }
   return false;
}
// source/core/h2_case.cpp
h2_inline void h2_case::clear()
{
   h2_sock::clear();
   h2_memory::hook();
   if (fails) delete fails, fails = nullptr;
   stats.clear();
}

h2_inline void h2_case::prev_setup()
{
   failed = warning = false;
   h2_memory::stack::push(filine);
   stats.timecost = h2_now();
}

h2_inline void h2_case::post_cleanup()
{
   stats.timecost = h2_now() - stats.timecost;
   stats.footprint = h2_memory::stack::footprint();
   h2_dnses::clear(dnses);
   h2_stubs::clear(stubs);
   failing(h2_mocks::clear(mocks, true), true, O.continue_assert);
   failing(h2_memory::stack::pop(), true, O.continue_assert);
}

h2_inline void h2_case::failing(h2_fail* fail, bool defer, bool append)
{
   if (fail) {
      if (fail->warning) warning = true;
      else failed = true;
      if (fails && !append) delete fail;
      else h2_fail::append_subling(fails, fail);
      if (!defer && !fail->warning) ::longjmp(fail_hole, 1);
   }
}
// source/core/h2_suite.cpp
h2_inline h2_suite::h2_suite(const char* filine, const char* file, int line, const char* describe, void (*test_fp_)(h2_suite*, h2_case*)) : h2_test(filine, file, line, describe), test_fp(test_fp_)
{
   memset(cleanup_hole, 0, sizeof(jmp_buf));
   h2_runner::I().suites.push_back(x);
}

h2_inline void h2_suite::clear()
{
   stats.clear();
}

h2_inline void h2_suite::setup()
{
   h2_memory::stack::push(filine);
   stats.timecost = h2_now();
}

h2_inline void h2_suite::cleanup()
{
   stats.timecost = h2_now() - stats.timecost;
   h2_stubs::clear(stubs);
   h2_mocks::clear(mocks, false);
   stats.footprint = h2_memory::stack::footprint();
   h2_memory::stack::pop();
}

h2_inline void h2_suite::enumerate()
{
   test_fp(this, nullptr); /* enumerate case by static local h2_case variable inside of h2_suite_test_CmLn() */
}

h2_inline void h2_suite::test(h2_case* c)
{
   bool uncaught = false;
   h2_exception::I().last_bt.clear();
   c->prev_setup();
   try {
      test_fp(this, c); /* include Setup(); c->post_setup() and c->prev_cleanup(); Cleanup() */
   } catch (...) {
      uncaught = true;
   }
   if (uncaught) c->failing(h2_fail::new_exception("was thrown but uncaught", h2_exception::I().last_type, h2_exception::I().last_bt, O.as_waring_uncaught), true, O.continue_assert);
   c->post_cleanup();
}

h2_inline h2_suite::registor::registor(h2_suite* s, h2_case* c)
{
   static int seq = 0x0fffffff;
   s->cases.push_back(c->x);
   s->seq = c->seq = ++seq;
}

h2_inline h2_suite::cleaner::~cleaner()
{
   static const unsigned char zero[sizeof(jmp_buf)] = {0};
   if (memcmp((const void*)thus->cleanup_hole, (const void*)zero, sizeof(jmp_buf)))
      ::longjmp(thus->cleanup_hole, 1);
}
// source/core/h2_filter.cpp
static inline bool match_names(const char* const patterns[], const char* subject)
{
   for (int i = 0; patterns[i]; ++i) {
      if (strcasestr(subject, patterns[i])) return true;
      if (strcspn(patterns[i], "?*[]") < strlen(patterns[i]) && h2_pattern::wildcard_match(patterns[i], subject, true)) return true;
   }
   return false;
}

static inline bool match_tags(const char* const patterns[], const h2_test* test)
{
   for (int i = 0; patterns[i]; ++i)
      if (test->tagged(patterns[i])) return true;
   return false;
}

static inline bool __filter(const h2_test* s, const h2_test* c, const char* const includes[], const char* const excludes[], const char* const file_includes[], const char* const file_excludes[], const char* const suite_includes[], const char* const suite_excludes[], const char* const case_includes[], const char* const case_excludes[], const char* const tags_includes[], const char* const tags_excludes[])
{
   if (includes[0] || file_includes[0] || suite_includes[0] || case_includes[0] || tags_includes[0]) {
      bool m = false;
      if (includes[0]) m = m || (match_names(includes, s->filine) || c && match_names(includes, c->filine) || match_names(includes, s->name) || c && match_names(includes, c->name) || match_tags(includes, s) || c && match_tags(includes, c));
      if (file_includes[0]) m = m || (match_names(file_includes, s->filine) || c && match_names(file_includes, c->filine));
      if (suite_includes[0]) m = m || match_names(suite_includes, s->name);
      if (case_includes[0]) m = m || c && match_names(case_includes, c->name);  // priority && > ||
      if (tags_includes[0]) m = m || (match_tags(tags_includes, s) || c && match_tags(tags_includes, c));
      if (!m) return true;
   }
   if (excludes[0])
      if ((match_names(excludes, s->filine) || c && match_names(excludes, c->filine)) ||
          (match_names(excludes, s->name) || c && match_names(excludes, c->name)) ||
          (match_tags(excludes, s) || c && match_tags(excludes, c))) return true;
   if (file_excludes[0] && (match_names(file_excludes, s->filine) || c && match_names(file_excludes, c->filine))) return true;
   if (suite_excludes[0] && match_names(suite_excludes, s->name)) return true;
   if (case_excludes[0] && c && match_names(case_excludes, c->name)) return true;
   if (tags_excludes[0] && (match_tags(tags_excludes, s) || c && match_tags(tags_excludes, c))) return true;
   return false;
}

static inline void h2_filter_suite(h2_suite* s)
{
   s->filtered = __filter(s, nullptr, O.includes, O.excludes, O.file_includes, O.file_excludes, O.suite_includes, O.suite_excludes, O.case_includes, O.case_excludes, O.tags_includes, O.tags_excludes);
}

static inline void h2_filter_case(h2_suite* s, h2_case* c)
{
   c->filtered = __filter(s, c, O.includes, O.excludes, O.file_includes, O.file_excludes, O.suite_includes, O.suite_excludes, O.case_includes, O.case_excludes, O.tags_includes, O.tags_excludes);
   if (!c->filtered) s->filtered = false;
}
// source/core/h2_runner.cpp
static inline void drop_last_order() { ::remove(".last_order"); }

static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, " file: %s\nsuite: %s\n case: %s\n test: %s\n\n", c->file, s->name, c->name, c->failed ? "failed" : "passed");
   ::fclose(f);
}

static inline void __find_case(h2_list& suites, char* filename, char* suitename, char* casename, bool failed)
{
   static int seq = 0;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, s->name))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name) && !strcmp(filename, c->file)) {
               s->seq = c->seq = ++seq;
               if (failed) c->last_failed = true;
            }
}

static inline bool fgets(FILE* f, char* b)
{
   if (!::fgets(b, 512, f)) return false;
   b[strlen(b) - 1] = '\0'; /* remove \n in save_last_order */
   return true;
}

static inline int sequence_last_order(h2_list& suites)
{
   int count = 0;
   char filename[512], suitename[512], casename[512], test[512], br[512];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (fgets(f, filename) && fgets(f, suitename) && fgets(f, casename) && fgets(f, test) && fgets(f, br)) {
      __find_case(suites, filename + 7, suitename + 7, casename + 7, !strcmp("failed", test + 7));
      count++;
   }
   ::fclose(f);
   return count;
}

h2_inline void h2_runner::enumerate()
{
   double dots = 0, dps = (h2_console::width() - 11) / (suites.count() + 0.000001);
   if (O.progressing) h2_console::prints("dark gray", "Collecting ");
   h2_list_for_each_entry (s, i, suites, h2_suite, x) {
      if (O.progressing)
         for (; dots <= i * dps; ++dots) h2_console::prints("dark gray", ".");

      for (int i = 0; global_suite_setups[i]; ++i) global_suite_setups[i]();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (int i = 0; global_suite_cleanups[i]; ++i) global_suite_cleanups[i]();
   }
   if (O.progressing) h2_console::prints("", "\33[2K\r");
}

h2_inline void h2_runner::filter()
{
   h2_list_for_each_entry (s, i, suites, h2_suite, x) {
      h2_filter_suite(s);
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         h2_filter_case(s, c);
   }
}

struct shuffle_comparison {
   static int seq(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_test, x)->seq - h2_list_entry(b, h2_test, x)->seq;
   }
   static int name(h2_list* a, h2_list* b)
   {
      return strcasecmp(h2_list_entry(a, h2_test, x)->name, h2_list_entry(b, h2_test, x)->name);
   }
   static int file(h2_list* a, h2_list* b)
   {
      int t = strcasecmp(h2_list_entry(a, h2_test, x)->file, h2_list_entry(b, h2_test, x)->file);
      return t != 0 ? t : h2_list_entry(a, h2_test, x)->line - h2_list_entry(b, h2_test, x)->line;
   }
   static int cmp(h2_list* a, h2_list* b)
   {
      int reverse = O.shuffles & ShuffleReverse ? -1 : 1;
      if (O.shuffles & ShuffleName) return name(a, b) * reverse;
      if (O.shuffles & ShuffleFile) return file(a, b) * reverse;
      return seq(a, b) * reverse;  // Random or Code Declare
   }
};

h2_inline void h2_runner::shuffle()
{
   if ((lasts = sequence_last_order(suites))) {
      suites.sort(shuffle_comparison::seq);
      h2_list_for_each_entry (s, suites, h2_suite, x)
         s->cases.sort(shuffle_comparison::seq);
      return;  // run in last order if last failed
   }

   if (O.shuffles) {
      ::srand(::clock());
      if (O.shuffles & ShuffleRandom)
         h2_list_for_each_entry (s, suites, h2_suite, x)
            h2_list_for_each_entry (c, s->cases, h2_case, x)
               s->seq = c->seq = ::rand();

      suites.sort(shuffle_comparison::cmp);
      h2_list_for_each_entry (s, suites, h2_suite, x)
         s->cases.sort(shuffle_comparison::cmp);
   }
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

   stats.timecost = h2_now();
   for (int i = 0; global_setups[i]; ++i) global_setups[i]();
   enumerate();
   filter();

   h2_report::I().on_runner_start(this);
   for (rounds = 0; rounds < O.run_rounds; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (int i = 0; global_suite_setups[i]; ++i) global_suite_setups[i]();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if ((0 < O.break_after_fails && O.break_after_fails <= stats.failed) || (O.only_last_failed && !c->last_failed)) c->ignored = true;
            if (c->ignored) stats.ignored++, s->stats.ignored++;
            else if (c->filtered) stats.filtered++, s->stats.filtered++;
            else if (c->todo) stats.todo++, s->stats.todo++;

            current_case = c;
            h2_report::I().on_case_start(s, c);
            if (!O.lists && !c->todo && !c->filtered && !c->ignored) {
               for (int i = 0; global_case_setups[i]; ++i) global_case_setups[i]();
               s->test(c);
               for (int i = 0; global_case_cleanups[i]; ++i) global_case_cleanups[i]();
               if (c->failed) stats.failed++, s->stats.failed++;
               else if (c->warning) stats.warning++, s->stats.warning++;
               else stats.passed++, s->stats.passed++;
            }
            h2_report::I().on_case_endup(s, c);
            c->clear();
         }
         s->cleanup();
         for (int i = 0; global_suite_cleanups[i]; ++i) global_suite_cleanups[i]();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      if (stats.failed == 0) drop_last_order();
      else if (lasts == 0) save_last_order(suites);
   }
   stats.timecost = h2_now() - stats.timecost;
   h2_report::I().on_runner_endup(this);
   for (int i = 0; global_cleanups[i]; ++i) global_cleanups[i]();

   h2_stubs::clear(stubs);
   h2_mocks::clear(mocks, false);
   h2_memory::finalize();
   return O.exit_with_fails ? stats.failed : 0;
}

h2_inline void h2_runner::stub(void* srcfp, void* dstfp, const char* srcfn, const char* filine)
{
   if (!srcfp || !dstfp) return;
   if (h2_runner::I().current_case) h2_stubs::add(h2_runner::I().current_case->stubs, srcfp, dstfp, srcfn, filine);
   else if (h2_runner::I().current_suite) h2_stubs::add(h2_runner::I().current_suite->stubs, srcfp, dstfp, srcfn, filine);
   else h2_stubs::add(h2_runner::I().stubs, srcfp, dstfp, srcfn, filine);
}

h2_inline void h2_runner::unstub(void* srcfp)
{
   if (!srcfp) return;
   if (h2_runner::I().current_case) h2_stubs::clear(h2_runner::I().current_case->stubs, srcfp);
   else if (h2_runner::I().current_suite) h2_stubs::clear(h2_runner::I().current_suite->stubs, srcfp);
   else h2_stubs::clear(h2_runner::I().stubs, srcfp);
}

h2_inline void h2_runner::mock(void* mocker)
{
   if (h2_runner::I().current_case) h2_mocks::add(h2_runner::I().current_case->mocks, mocker);
   else if (h2_runner::I().current_suite) h2_mocks::add(h2_runner::I().current_suite->mocks, mocker);
   else h2_mocks::add(h2_runner::I().mocks, mocker);
}

h2_inline void h2_runner::failing(h2_fail* fail)
{
   if (!fail) return;
   if (O.debugger_trap && h2_attach_debugger()) h2_raise_trap();
   if (h2_runner::I().current_case) h2_runner::I().current_case->failing(fail, O.continue_assert, true);
}

h2_inline void h2_runner::asserts()
{
   if (h2_runner::I().current_case) h2_runner::I().current_case->stats.asserts += 1;
   if (h2_runner::I().current_suite) h2_runner::I().current_suite->stats.asserts += 1;
   h2_runner::I().stats.asserts += 1;
}
// source/assert/h2_assert.cpp
static inline const char* __find_compare(const char* expression, const char* op)
{
   bool quote = false;
   if (strlen(op) == 2) {
      for (const char* p = expression; *p; ++p) {
         if (*p == '\"') quote = !quote;
         if (!quote && !strncmp(op, p, 2)) return p;
      }
   } else {
      int stacks = 0;
      if (*op == '>')
         for (const char* p = expression; *p; ++p) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '<') ++stacks;
            if (!quote && *p == '>' && 0 == stacks--) return p;
         }
      if (*op == '<')
         for (const char* p = expression + strlen(expression); expression <= p; --p) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '>') ++stacks;
            if (!quote && *p == '<' && 0 == stacks--) return p;
         }
   }
   return nullptr;
}

static inline void __split_compare(const char* expression, const char* op, h2_string& e_expression, h2_string& a_expression)
{
   const char* p = __find_compare(expression, op);
   if (p) {
      e_expression.assign(expression, strip_right(expression, p) - expression);
      a_expression = strip_left(p + strlen(op));
   }
}

h2_inline h2_ostringstream& h2_assert::stash(h2_fail* fail, const char* assert_type, const h2_line& expection, const h2_line& represent, const char* assert_op)
{
   if (oppose) {
      if (fail) delete fail, fail = nullptr;
      else fail = h2_fail::new_unexpect(expection, represent);
   }
   h2_runner::asserts();
   fails = fail;
   if (fail && fail->subling_next) {
      fails = h2_fail::new_unexpect();
      h2_fail::append_child(fails, fail);
   }
   if (fails) {
      fails->assert_type = assert_type;
      fails->assert_op = assert_op;
   }
   return oss;
}

h2_inline void h2_assert::failing(const char* e_expression, const char* a_expression, const char* filine)
{
   bool as_warning = h2_warning::I().swap(false);
   if (fails) {
      fails->warning = as_warning;
      fails->user_explain = oss.str().c_str();
      fails->filine = filine;
      fails->e_expression = e_expression;
      fails->a_expression = a_expression;
      if (*fails->assert_op != ',') __split_compare(a_expression, fails->assert_op, fails->e_expression, fails->a_expression);
   }
   h2_runner::failing(fails);
}
// source/assert/h2_timer.cpp
h2_inline h2_timer::h2_timer(int ms_, const char* filine_) : filine(filine_), cpu_ms(ms_)
{
   start = ::clock();
}

h2_inline h2_timer::~h2_timer()
{
   h2_runner::asserts();
   double delta = (::clock() - start) * 1000.0 / CLOCKS_PER_SEC;
   if (cpu_ms < delta) {
      h2_line line = "performance expect < ";
      line.printf("green", "%d", cpu_ms).printf("", " ms, but actually cost ").printf("red", "%d", (int)delta).printf("", " ms");
      h2_runner::failing(h2_fail::new_normal(line, filine));
   }
}
// source/report/h2_layout.cpp
struct h2_layout {
   static h2_lines line_break(const h2_line& line, size_t width)
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

   static h2_lines lines_merge(const h2_lines& left_lines, const h2_lines& right_lines, size_t left_width, size_t right_width, int step, char scale, size_t seq_width)
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
               if (j == 0) line.printf("dark gray", seq_fmt, step * i);
               else line.indent(seq_width + 2);
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

   static h2_lines prefix_break(const h2_line& line, const h2_line& title, size_t width)
   {
      h2_lines lines = line_break(line, width - title.width());

      for (size_t i = 0; i < lines.size(); ++i) {
         if (i == 0) lines[i] = title + lines[i];
         else lines[i].indent(title.width());
      }
      return lines;
   }

   static h2_lines split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, size_t step, char scale, size_t width)
   {
      size_t seq_width = number_strlen(step * std::max(left_lines.size(), right_lines.size()), scale == 'x' ? 16 : 10);
      size_t valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

      size_t left_width = std::max(left_lines.width(), strlen(left_title));
      size_t right_width = std::max(right_lines.width(), strlen(right_title));

      if (left_width < valid_width / 2) right_width = std::min(valid_width - left_width, right_width);
      else if (right_width < valid_width / 2) left_width = std::min(valid_width - right_width, left_width);
      else left_width = right_width = valid_width / 2;

      h2_line title = (step ? h2_string(seq_width + 2, ' ') : "") + h2_string(left_title).centre(left_width) + "   " + h2_string(right_title).centre(right_width);
      h2_lines lines = {title.brush("dark gray")};

      return lines += lines_merge(left_lines, right_lines, left_width, right_width, step, scale, seq_width);
   }

   static h2_lines unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
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

   static h2_lines seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
   {
      h2_line up_title_line = gray(up_title) + color("> ", "green");
      h2_line down_title_line = gray(down_title) + color("> ", "red");
      h2_lines lines = prefix_break(up_line, up_title_line, width);
      return lines += prefix_break(down_line, down_title_line, width);
   }
};
// source/report/h2_failure.cpp
#define H2_FAIL_APPEND(next)         \
   if (fails) {                      \
      h2_fail** pp = &fails->next;   \
      while (*pp) pp = &(*pp)->next; \
      *pp = fail;                    \
   } else fails = fail

h2_inline void h2_fail::append_subling(h2_fail*& fails, h2_fail* fail) { H2_FAIL_APPEND(subling_next); }
h2_inline void h2_fail::append_child(h2_fail*& fails, h2_fail* fail) { H2_FAIL_APPEND(child_next); }

h2_inline h2_fail::~h2_fail()
{
   if (child_next) delete child_next;
   if (subling_next) delete subling_next;
}

h2_inline h2_line h2_fail::locate()
{
   if (h2_blank(filine)) return {};
   return gray(" at ") + h2_basefile(filine);
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, size_t, size_t)> cb, size_t si, size_t ci)
{
   cb(this, si, ci);
   if (child_next) child_next->foreach(cb, 0, ci + 1);
   if (subling_next) subling_next->foreach(cb, si + 1, ci);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_line& explain_ = {}, const char* filine_ = nullptr) : h2_fail(explain_, filine_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      line += explain;
      h2_console::printl(line + locate());
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   h2_string _a = a.escape(), _b = b.escape();
   if (_a == _b) return true;

#define H2_NULL_SYNONYM "NULL", "nullptr", "null", "__null", "(null)", "(nil)", "((void *)0)", "0", "0x0", "00000000", "0000000000000000"
#define H2_NOTNULL_SYNONYM "!NULL", "!nullptr", "Not(NULL)", "Not(nullptr)", "Nq(NULL)", "Nq(nullptr)", "Not(0)", "Nq(0)"
#define H2_TRUE_SYNONYM "true", "TRUE", "True", "1"
#define H2_FALSE_SYNONYM "false", "FALSE", "False", "0"

   if (h2_in(_a.c_str(), 11, H2_NULL_SYNONYM) && h2_in(_b.c_str(), 11, H2_NULL_SYNONYM)) return true;
   if (h2_in(_a.c_str(), 8, H2_NOTNULL_SYNONYM) && h2_in(_b.c_str(), 8, H2_NOTNULL_SYNONYM)) return true;
   if (h2_in(_a.c_str(), 4, H2_TRUE_SYNONYM) && h2_in(_b.c_str(), 4, H2_TRUE_SYNONYM)) return true;
   if (h2_in(_a.c_str(), 4, H2_FALSE_SYNONYM) && h2_in(_b.c_str(), 4, H2_FALSE_SYNONYM)) return true;

   if (_a == "Eq(" + _b + ")") return true;
   if (_a == "ListOf(" + _b.unenclose('[', ']') + ")") return true;
   return false;
}

struct h2_fail_unexpect : h2_fail {
   h2_line expection, represent;
   int c = 0;
   h2_fail_unexpect(const h2_line& expection_ = {}, const h2_line& represent_ = {}, const h2_line& explain_ = {}, const char* file_ = nullptr) : h2_fail(explain_, file_), expection(expection_), represent(represent_) {}
   void print_OK1(const char* type, h2_line& line)
   {
      h2_line a = h2_line(a_expression).gray_quote().brush("cyan");
      line += type + gray("(") + a + gray(")");
      if (!is_synonym(a_expression, represent.string()))
         line += " is " + represent.abbreviate(10000, 3).brush("bold,red");
   }
   void print_OK2(const char* type, h2_line& line)
   {
      static const char* R_Arrows[] = {"==>", "»", "⇛", "⟹  ", "⇒ "};  // →►
      static const char* L_Arrows[] = {"<==", "«", "⇚", "⟸  ", "⇐ "};  // ←◄
      static constexpr int Arrow = 3;
      h2_line e, a;
      if (!expection.width()) {
         e = h2_line(e_expression).abbreviate(10000, 3).gray_quote().brush("green");
      } else if (is_synonym(e_expression, expection.string())) {
         e = h2_line(e_expression).abbreviate(10000, 3).brush("green");
      } else {
         e = h2_line(e_expression).abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).gray_quote().brush("cyan") + gray(R_Arrows[Arrow]) + expection.abbreviate(10000, 3).brush("green");
      }

      if (!represent.width()) {
         a = h2_line(a_expression).abbreviate(10000, 3).gray_quote().brush("bold,red");
      } else if (is_synonym(a_expression, represent.string()) || !a_expression.length()) {
         a = represent.abbreviate(10000, 3).brush("bold,red");
      } else {
         a = represent.abbreviate(10000, 3).brush("bold,red") + gray(L_Arrows[Arrow]) + h2_line(a_expression).abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).gray_quote().brush("cyan");
      }

      line += type + gray("(") + e + " " + assert_op + " " + a + gray(")");
   }
   void print_JE(h2_line& line)
   {
      h2_line e = h2_line(e_expression.unenclose('\"').unenclose('\'')).abbreviate(O.verbose >= VerboseDetail ? 10000 : 30, 2).brush("cyan");
      h2_line a = h2_line(a_expression.unenclose('\"').unenclose('\'')).abbreviate(O.verbose >= VerboseDetail ? 10000 : 30, 2).brush("bold,red");
      line += "JE" + gray("(") + e + ", " + a + gray(")");
   }
   void print_In(h2_line& line)
   {
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      if (expection.width()) {
         line.printf("", "%sexpect is ", comma_if(c++));
         line += expection.abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).brush("green");
      }
      if (represent.width()) {
         line.printf("", "%sactual is ", comma_if(c++));
         line += represent.abbreviate(O.verbose >= VerboseDetail ? 10000 : 120, 3).brush("bold,red");
      }
   }

   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (!strcmp("In", assert_type)) print_In(line);
      if (!strcmp("OK1", assert_type)) print_OK1("OK", line);
      if (!strcmp("KO1", assert_type)) print_OK1("KO", line);
      if (!strcmp("OK2", assert_type)) print_OK2("OK", line);
      if (!strcmp("KO2", assert_type)) print_OK2("KO", line);
      if (!strcmp("JE", assert_type)) print_JE(line);
      if (explain.width()) line += comma_if(c++, ", ", " ") + explain;
      if (user_explain.size()) line += {comma_if(c++, ", ", " "), user_explain};
      h2_console::printl(line + locate());
   }
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   const h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_line& expection, const h2_line& explain = {}) : h2_fail_unexpect(expection, h2_stringify(a_value_, true), explain), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   h2_line fmt_char(h2_string& c, bool eq, const char* style)
   {
      if (c.equals(" ") && O.colorful) return gray("‧");
      if (eq) return c.escape();
      return color(c.escape(), style);
   }
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (O.verbose >= VerboseDetail || 8 < e_value.width() || 8 < a_value.width()) {
         h2_line e_line, a_line;
         h2_vector<h2_string> e_chars = e_value.disperse(), a_chars = a_value.disperse();
         auto lcs = h2_LCS(e_chars, a_chars, caseless).lcs();
         for (size_t i = 0; i < lcs.first.size(); ++i) e_line += fmt_char(e_chars[i], lcs.first[i], "green");
         for (size_t i = 0; i < lcs.second.size(); ++i) a_line += fmt_char(a_chars[i], lcs.second[i], "red");
         h2_console::printl(h2_layout::unified(e_line, a_line, "expect", "actual", h2_console::width()));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   const h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_line& expection, const h2_line& explain) : h2_fail_unexpect(expection, h2_stringify(a_value_, true), explain), e_value(e_value_), a_value(a_value_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (16 < e_value.width() || 16 < a_value.width()) {
         h2_line e_line = e_value.escape(), a_line = a_value.escape();
         h2_console::printl(h2_layout::seperate(e_line, a_line, "expect", "actual", h2_console::width()));
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
      if (e_value.width() < 16 || a_value.width() < 16) return;  // TLDR
      if (!h2_blank(O.json_source_quote) || !h2_json::diff(e_value, a_value, e_lines, a_lines, caseless)) {
         e_lines = h2_json::dump(e_value);
         a_lines = h2_json::dump(a_value);
         for (size_t i = 0; i < e_lines.size(); ++i)
            if (i) e_lines[i].indent(8);
         for (size_t i = 0; i < a_lines.size(); ++i)
            if (i) a_lines[i].indent(8);
         h2_console::prints("dark gray", "expect");
         h2_console::prints("green", "> ");
         h2_console::printl(e_lines);
         h2_console::prints("dark gray", "actual");
         h2_console::prints("red", "> ");
         h2_console::printl(a_lines);
      } else {
         h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual", 0, 'd', h2_console::width() - 1);
         for (auto& line : lines) line.indent(1);
         h2_console::printl(lines);
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
         case 8: print_ints<unsigned char>(e_lines, a_lines, bytes_per_row = (h2_console::width() < 108 ? 8 : 16)); break;
         case 16: print_ints<unsigned short>(e_lines, a_lines, bytes_per_row = 16); break;
         case 32: print_ints<unsigned int>(e_lines, a_lines, bytes_per_row = 16); break;
         case 64: print_ints<unsigned long long>(e_lines, a_lines, bytes_per_row = 16); break;
         default: break;
      }
      h2_console::printl(h2_layout::split(e_lines, a_lines, "expect", "actual", bytes_per_row * 8 / width, 'x', h2_console::width()));
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

   template <typename T> void print_ints(h2_lines& e_lines, h2_lines& a_lines, size_t bytes_per_row)
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
   h2_fail_memory(const void* ptr_, const size_t size_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const char* file_ = nullptr) : h2_fail({}, file_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   const h2_vector<std::pair<size_t, size_t>> sizes;
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, const size_t size_, const h2_vector<std::pair<size_t, size_t>>& sizes_, const h2_backtrace& bt_allocate_, const char* where_, const char* file_) : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_), sizes(sizes_), where(where_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line = h2_stringify(ptr) + color(" memory leak ", "bold,red") + h2_stringify(size).brush("red") + " ";
      size_t i = 0, c = 0, n = 3;
      h2_line sl;
      for (auto& p : sizes) {
         sl += gray(comma_if(i++));
         if (O.verbose <= VerboseCompactPassed && n < i) {
            sl += color("..." + h2_stringify(sizes.size() - n), "dark gray");
            break;
         }
         sl += h2_stringify(p.first);
         if (1 < p.second) sl += gray("x") + h2_stringify(p.second);
         c += p.second;
      }
      if (1 < c) line += gray("[") + sl + gray("] ");
      h2_console::printl(" " + line + "bytes in " + where + " totally" + locate());
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::prints("", " %p", ptr);
      h2_console::prints("bold,red", " double free");
      h2_console::prints("", " at backtrace:\n", ptr), bt_double_free.print(2);
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_console::prints("", "  already free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::prints("", " %p allocate with ", ptr);
      h2_console::prints("bold,red", "%s", who_allocate);
      h2_console::prints("", ", release by ");
      h2_console::prints("bold,red", "%s", who_release);
      h2_console::prints("", " asymmetrically at backtrace:\n"), bt_release.print(2);
      if (0 < bt_allocate.count) h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
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
      h2_console::printl(" " + t + locate() + (bt_trample.count ? " at backtrace:" : ""));
      if (bt_trample.count) bt_trample.print(3);
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
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
      h2_console::printl(" " + t + " at backtrace:"), bt_use.print(2);
      h2_console::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_console::prints("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_exception : h2_fail {
   const char* type;
   const h2_backtrace bt_throw;
   h2_fail_exception(const h2_line& explain_, const char* type_, const h2_backtrace& bt_throw_, bool as_warning, const char* filine_) : h2_fail(explain_, filine_), type(type_), bt_throw(bt_throw_) { warning = as_warning; }
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::printl(" exception " + color(type, "red") + " " + explain + locate() + " at backtrace:");
      bt_throw.print(3);
   }
};

struct h2_fail_symbol : h2_fail {
   const h2_string symbol;
   const h2_vector<h2_string> candidates;
   h2_fail_symbol(const h2_string& symbol_, const h2_vector<h2_string>& candidates_, const h2_line& explain_) : h2_fail(explain_, nullptr), symbol(symbol_), candidates(candidates_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_console::printl(color(candidates.size() ? " Find multiple " : " Not found ", "yellow") + color(symbol, "bold,red"));
      for (size_t i = 0; i < candidates.size(); ++i)
         h2_console::printl("  " + color(h2_stringify(i) + ". ", "dark gray") + color(candidates[i], "yellow"));
      if (explain.width()) h2_console::printl(explain);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_line& explain_, const char* filine_) { return new h2_fail_normal(explain_, filine_); }
h2_inline h2_fail* h2_fail::new_unexpect(const h2_line& expection_, const h2_line& represent_, const h2_line& explain_) { return new h2_fail_unexpect(expection_, represent_, explain_); }
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strcmp(e_value, a_value, caseless, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strfind(e_value, a_value, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, bool caseless, const h2_line& explain_) { return new h2_fail_json(e_value, a_value, expection_, caseless, explain_); }
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t length, const size_t width) { return new h2_fail_memcmp(e_value, a_value, length, width); }
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* filine_) { return new h2_fail_memory_leak(ptr, size, sizes, bt_allocate, where, filine_); }
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free) { return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free); }
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release) { return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release); }
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample) { return new h2_fail_overflow(ptr, size, violate_ptr, action, spot, bt_allocate, bt_trample); }
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use) { return new h2_fail_use_after_free(ptr, violate_ptr, action, bt_allocate, bt_release, bt_use); }
h2_inline h2_fail* h2_fail::new_exception(const h2_line& explain_, const char* type, const h2_backtrace& bt_throw, bool as_warning, const char* filine_) { return new h2_fail_exception(explain_, type, bt_throw, as_warning, filine_); }
h2_inline h2_fail* h2_fail::new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_line& explain_) { return new h2_fail_symbol(symbol, candidates, explain_); };
// source/report/h2_report_console.cpp
#define H2_UNITS(count, unit) ((count > 1) ? (unit "s") : unit)

struct h2_report_console : h2_report_interface {
   size_t cases = 0, index = 0, last_capture_length = 0;

   int nonzero_count(int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0)
   {
      return !!a1 + !!a2 + !!a3 + !!a4 + !!a5 + !!a6;
   }
   h2_line format_title(const char* suite_name, const char* case_name, const char* file_line)
   {
      h2_line title;
      title.printf("dark gray", "┊ ");
      if (strlen(case_name)) title.printf("", "%s ", case_name);
      else title.printf("dark gray", "case ");
      if (suite_name) {
         title.printf("dark gray", "┊ ");
         if (strlen(suite_name)) title.printf("", "%s ", suite_name);
         else title.printf("dark gray", "suite ");
      }
      if (file_line) {
         title.printf("dark gray", "┊ ");
         title.printf("", "%s ", file_line);
      }
      return title;
   }
   void format_percentage(h2_line& bar)
   {
      bar.printf("dark gray", "[");
      bar.printf("", "%3d%%", cases ? (int)(index * 100 / cases) : 100);
      bar.printf("dark gray", "] ");
   }
   static const char* format_volume(long long footprint, char* s = (char*)alloca(128))
   {
      if (footprint < 1024LL) sprintf(s, "%lld", footprint);
      else if (footprint < 1024LL * 1024LL) sprintf(s, "%.1fKB", footprint / 1024.0);
      else if (footprint < 1024LL * 1024LL * 1024LL) sprintf(s, "%.2fMB", footprint / (1024.0 * 1024.0));
      else sprintf(s, "%.3fGB", footprint / (1024.0 * 1024.0 * 1024.0));
      return s;
   }
   static const char* format_duration(long long ms, char* s = (char*)alloca(128))
   {
      if (ms < 100) sprintf(s, "%lld milliseconds", ms);
      else if (ms < 1000 * 60) sprintf(s, "%.1f second%s", ms / 1000.0, ms == 1000 ? "" : "s");
      else if (ms < 1000 * 60 * 60) sprintf(s, "%.2f minute%s", ms / 60000.0, ms == 60000 ? "" : "s");
      else sprintf(s, "%.3f hour%s", ms / 3600000.0, ms == 3600000 ? "" : "s");
      return s;
   }
   static const char* format_units(int count, const char* unit1, const char* unit2 = nullptr, char* s = (char*)alloca(128))
   {
      bool multiple = count > 1;
      sprintf(s, "%d%s%s", count, multiple && unit2 ? unit2 : unit1, multiple && !unit2 ? "s" : "");
      return s;
   }
   void print_bar(bool percentage, const char* status_style, const char* status, h2_suite* s, h2_case* c, bool backable)
   {
      const char* new_line = last_capture_length == h2_stdio::I().capture_length ? "\33[2K\r" /* clear line */ : /* user output */ "\n";
      h2_console::prints("", new_line);
      last_capture_length = h2_stdio::I().capture_length;
      h2_report::I().backable = O.progressing && backable;

      if (!O.progressing && backable) return;
      h2_line bar;
      if (percentage && O.progressing) format_percentage(bar);
      if (status && status_style) bar.printf(status_style, "%s", status);
      if (s && c) bar += format_title(s->name, c->name, backable ? nullptr : h2_basefile(c->filine));
      if (backable) {
         if (h2_console::width() > bar.width()) bar.padding(h2_console::width() - bar.width());
         else bar = bar.abbreviate(h2_console::width());
      }
      h2_console::printl(bar, false);
   }
   void on_runner_start(h2_runner* r) override
   {
      h2_list_for_each_entry (s, r->suites, h2_suite, x)
         cases += s->cases.count();
   }
   void on_runner_endup(h2_runner* r) override
   {
      print_bar(false, nullptr, nullptr, nullptr, nullptr, false);

      int n = nonzero_count(r->stats.failed, r->stats.warning, r->stats.todo, r->stats.filtered, r->stats.ignored);
      h2_line line = (0 < r->stats.failed) ? color("Failure ", "bold,red") : color("Success ", "bold,green");
      if (0 < n) line += gray("(");
      line += color(h2_stringify(r->stats.passed), "green") + " passed";  // always print
      if (r->stats.failed) line += gray(", ") + color(h2_stringify(r->stats.failed), "red") + " failed";
      if (r->stats.warning) line += gray(", ") + color(h2_stringify(r->stats.warning), "cyan") + " warning";
      if (r->stats.todo) line += gray(", ") + color(h2_stringify(r->stats.todo), "yellow") + " todo";
      if (r->stats.filtered) line += gray(", ") + color(h2_stringify(r->stats.filtered), "blue") + " filtered";
      if (r->stats.ignored) line += gray(", ") + color(h2_stringify(r->stats.ignored), "blue") + " ignored";
      if (0 < n) line += gray(") ") + h2_stringify(cases);
      line += H2_UNITS(cases, " case") + gray(", ") + format_units(r->stats.asserts, " assert");
      if (1 < r->rounds) line += gray(", ") + format_units(r->rounds, " round");
      line += gray(", ") + format_duration(r->stats.timecost);

      h2_console::printl(line);
   }
   void on_suite_start(h2_suite* s) override {}
   void on_suite_endup(h2_suite* s) override
   {
      if (O.verbose >= 9 && !(O.includes[0] || O.excludes[0])) {
         print_bar(true, nullptr, nullptr, nullptr, nullptr, false);

         int n = nonzero_count(s->stats.passed, s->stats.warning, s->stats.failed, s->stats.todo, s->stats.filtered, s->stats.ignored);
         h2_line line;
         if (s->stats.passed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.passed) + " passed";
         if (s->stats.failed) line += gray(comma_if(line.width())) + h2_stringify(s->stats.failed) + " failed";
         if (s->stats.warning) line += gray(comma_if(line.width())) + h2_stringify(s->stats.warning) + " warning";
         if (s->stats.todo) line += gray(comma_if(line.width())) + h2_stringify(s->stats.todo) + " todo";
         if (s->stats.filtered) line += gray(comma_if(line.width())) + h2_stringify(s->stats.filtered) + " filtered";
         if (s->stats.ignored) line += gray(comma_if(line.width())) + h2_stringify(s->stats.ignored) + " ignored";
         line = gray("suite ") + s->name + gray(1 < n ? " (" : " - ") + line + gray(1 < n ? ")" : "");
         if (0 < s->cases.count()) line += H2_UNITS(s->cases.count(), " case");
         if (0 < s->stats.asserts) line += gray(", ") + format_units(s->stats.asserts, " assert");
         if (0 < s->stats.footprint) line += gray(", ") + format_volume(s->stats.footprint) + " footprint";
         if (1 < s->stats.timecost) line += gray(", ") + format_duration(s->stats.timecost);

         h2_console::printl(line);
      }
   }
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      ++index;
      if (c->filtered || c->ignored || c->todo) return;
      print_bar(true, "light blue", "Testing", s, c, true);
   }
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      if (c->filtered || c->ignored) return;
      if (c->todo) {
         if (O.verbose >= VerboseDetail) print_bar(true, "yellow", s->absent() ? "TODO   " : "Todo   ", s, c, false);
      } else if (c->failed) {
         if (O.verbose >= VerboseCompactFailed) {
            print_bar(true, "bold,red", "Failed ", s, c, false);
            h2_console::prints("", "\n");
            if (O.verbose >= VerboseNormal && c->fails) {
               c->fails->foreach([](h2_fail* fail, size_t si, size_t ci) { fail->print(si, ci); });
               h2_console::prints("", "\n");
            }
         }
      } else if (c->warning) {
         if (O.verbose >= VerboseCompactWarning) {
            print_bar(true, "bold,cyan", "Warning", s, c, false);
            h2_console::prints("", "\n");
            if (O.verbose > VerboseNormal && c->fails) {
               c->fails->foreach([](h2_fail* fail, size_t si, size_t ci) { fail->print(si, ci); });
               h2_console::prints("", "\n");
            }
         }
      } else {  // Passed
         if (O.verbose >= VerboseDetail || O.verbose == VerboseCompactPassed) {
            print_bar(true, "green", "Passed ", s, c, false);
            if (O.verbose >= VerboseDetail) {
               h2_line line;
               if (0 < c->stats.asserts) line += gray(comma_if(line.width())) + format_units(c->stats.asserts, " assert");
               if (0 < c->stats.footprint) line += gray(comma_if(line.width())) + format_volume(c->stats.footprint) + " footprint";
               if (0 < c->stats.timecost) line += gray(comma_if(line.width())) + format_duration(c->stats.timecost);
               if (line.width()) h2_console::printl(gray("- ") + line, false);
            }
            h2_console::prints("", "\n");
         }
      }
   }
};
// source/report/h2_report_list.cpp
struct h2_report_list : h2_report_interface {
   int suites = 0, cases = 0, todos = 0;
   int unfiltered_suites = 0, unfiltered_cases = 0, unfiltered_todos = 0;
   int suite_cases = 0, suite_todos = 0;

   struct tag {
      const char* name;
      int suites = 0, cases = 0;
   } tags[1024], unfiltered_tags[1024];
   int tagc = 0, unfiltered_tagc = 0;

   static tag* get_tag(tag tags[], int& tagc, const char* name)
   {
      for (int i = 0; i < tagc; ++i)
         if (!strcmp(name, tags[i].name)) return &tags[i];
      return nullptr;
   }
   static tag* add_tag(tag tags[], int& tagc, const char* name)
   {
      tag* t = get_tag(tags, tagc, name);
      if (t) return t;
      if (tagc >= 1024) return nullptr;
      t = &tags[tagc++];
      t->name = name;
      return t;
   }
   static void add_suite_tag(tag tags[], int& tagc, const char* name)
   {
      auto t = add_tag(tags, tagc, name);
      if (t) t->suites++;
   }
   static void add_case_tag(tag tags[], int& tagc, const char* name)
   {
      auto t = add_tag(tags, tagc, name);
      if (t) t->cases++;
   }

   void on_runner_start(h2_runner* r) override {}
   void on_runner_endup(h2_runner* r) override
   {
      if (O.lists & ListTags) {
         for (int i = 0; i < unfiltered_tagc; ++i) {
            h2_line line;
            line.printf("dark gray", "TAG-%d. ", i).printf("bold,light purple", "%s ", unfiltered_tags[i].name);
            if (unfiltered_tags[i].suites) line.printf("", " %d ", unfiltered_tags[i].suites).printf("dark gray", H2_UNITS(unfiltered_tags[i].suites, "suite"));
            if (unfiltered_tags[i].cases) line.printf("", " %d ", unfiltered_tags[i].cases).printf("dark gray", H2_UNITS(unfiltered_tags[i].cases, "case"));
            h2_console::printl(line);
         }
      }

      h2_line line;
      if (O.lists & ListSuite) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_suites), "green") + " " + gray(H2_UNITS(unfiltered_suites, "suite"));
      if (O.lists & ListCase) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_cases), "green") + " " + gray(H2_UNITS(unfiltered_cases, "case"));
      if (O.lists & ListTodo) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_todos), "green") + " " + gray(H2_UNITS(unfiltered_todos, "todo"));
      if (O.lists & ListTags) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_tagc), "green") + " " + gray(H2_UNITS(unfiltered_tagc, "tag"));
      if (O.lists & ListSuite && suites > unfiltered_suites) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), suites - unfiltered_suites, H2_UNITS(suites - unfiltered_suites, "suite"));
      if (O.lists & ListCase && cases > unfiltered_cases) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), cases - unfiltered_cases, H2_UNITS(cases - unfiltered_cases, "case"));
      if (O.lists & ListTodo && todos > unfiltered_todos) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), todos - unfiltered_todos, H2_UNITS(todos - unfiltered_todos, "todo"));
      if (O.lists & ListTags && tagc > unfiltered_tagc) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), tagc - unfiltered_tagc, H2_UNITS(tagc - unfiltered_tagc, "tag"));
      h2_console::printl("Listing " + line);
   }
   h2_line format_tags(const char* tags[])
   {
      h2_line line;
      if (O.lists & ListTags)
         for (int i = 0; tags[i]; ++i) line.printf("purple", " %s", tags[i]);
      return line;
   }
   void on_suite_start(h2_suite* s) override
   {
      suite_cases = 0;
      suite_todos = 0;
      if (s->absent()) return;  // CASE
      for (int i = 0; s->tags[i]; ++i) add_suite_tag(tags, tagc, s->tags[i]);
      if (!s->filtered)
         for (int i = 0; s->tags[i]; ++i) add_suite_tag(unfiltered_tags, unfiltered_tagc, s->tags[i]);
      ++suites;
      if (!s->filtered && O.lists & ListSuite) {
         ++unfiltered_suites;
         h2_line line;
         line.printf("dark gray", "SUITE-%d. ", unfiltered_suites);
         h2_console::printl(line + color(s->name, "bold,blue") + " " + gray(h2_basefile(s->filine)) + format_tags(s->tags));
      }
   }
   void on_suite_endup(h2_suite* s) override {}
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      for (int i = 0; c->tags[i]; ++i) add_case_tag(tags, tagc, c->tags[i]);
      if (!c->filtered)
         for (int i = 0; c->tags[i]; ++i) add_case_tag(unfiltered_tags, unfiltered_tagc, c->tags[i]);

      const char* type = nullptr;
      if (c->todo) {
         ++todos;
         if (!c->filtered && O.lists & ListTodo) {
            type = s->absent() ? "TODO" : "Todo";
            ++unfiltered_todos, ++suite_todos;
         }
      } else {
         ++cases;
         if (!c->filtered && O.lists & ListCase) {
            type = s->absent() ? "CASE" : "Case";
            ++unfiltered_cases, ++suite_cases;
         }
      }

      if (type) {
         h2_line line;
         if (O.lists & ListSuite) line.printf("dark gray", " %s/%d-%d. ", type, suite_cases + suite_todos, unfiltered_cases + unfiltered_todos);
         else line.printf("dark gray", " %s-%d. ", type, unfiltered_cases + unfiltered_todos);

         h2_console::printl(line + color(c->name, "cyan") + " " + gray(h2_basefile(c->filine)) + format_tags(c->tags));
      }
   }
   void on_case_endup(h2_suite* s, h2_case* c) override {}
};
// source/report/h2_report_junit.cpp
struct h2_report_junit : h2_report_interface {
   FILE* f;
   void on_runner_start(h2_runner* r) override
   {
      f = fopen(O.junit_path, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   }
   void on_suite_start(h2_suite* s) override
   {
      if (!f) return;
      fprintf(f, "<testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n", s->stats.failed, s->name, s->stats.todo + s->stats.filtered, s->cases.count(), 0, "");
   }
   void on_case_start(h2_suite* s, h2_case* c) override {}
   void on_case_endup(h2_suite* s, h2_case* c) override
   {
      if (!f) return;
      fprintf(f, "<testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n", s->name, c->name, c->todo ? "TODO" : (c->filtered ? "Filtered" : (c->ignored ? "Ignored" : (c->failed ? "Failed" : (c->warning ? "Warning" : "Passed")))), c->stats.timecost / 1000.0);
      if (c->failed) {
         fprintf(f, "<failure message=\"%s:", c->filine);
         if (c->fails) c->fails->foreach([&](h2_fail* fail, size_t si, size_t ci) {fprintf(f, "{newline}"); fail->print(f); });
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "<system-out></system-out><system-err></system-err>\n");
      fprintf(f, "</testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override
   {
      if (!f) return;
      fprintf(f, "</testsuite>\n");
   }
   void on_runner_endup(h2_runner* r) override
   {
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};
// source/report/h2_report.cpp
h2_inline void h2_report::initialize()
{
   if (O.lists) {
      h2_array_append(I().reports, new h2_report_list);
   } else {
      h2_array_append(I().reports, new h2_report_console);
      if (strlen(O.junit_path)) h2_array_append(I().reports, new h2_report_junit);
   }
}

h2_inline h2_report::registor::registor(h2_report_interface* report) { h2_array_append(h2_report::I().reports, report); }

/* clang-format off */
h2_inline void h2_report::on_runner_start(h2_runner* r) { for (int i = 0; reports[i]; ++i) reports[i]->on_runner_start(r); }
h2_inline void h2_report::on_runner_endup(h2_runner* r) { for (int i = 0; reports[i]; ++i) reports[i]->on_runner_endup(r); }
h2_inline void h2_report::on_suite_start(h2_suite* s) { for (int i = 0; reports[i]; ++i) reports[i]->on_suite_start(s); }
h2_inline void h2_report::on_suite_endup(h2_suite* s) { for (int i = 0; reports[i]; ++i) reports[i]->on_suite_endup(s); }
h2_inline void h2_report::on_case_start(h2_suite* s, h2_case* c) { for (int i = 0; reports[i]; ++i) reports[i]->on_case_start(s, c); }
h2_inline void h2_report::on_case_endup(h2_suite* s, h2_case* c) { for (int i = 0; reports[i]; ++i) reports[i]->on_case_endup(s, c); }
// source/option/h2_option.cpp
static inline int usage(const char* option)
{
   if (option && (!strcmp("i", option) || !strcmp("e", option))) {
      return ::printf("\033[36m-i/e pattern\033[0m: include in file:line, suite name, case name, suite tags, case tags\n"
                      "\033[36m-i/e file=pattern\033[0m: include in file:line\n"
                      "\033[36m-i/e suite=pattern\033[0m: include in suite name\n"
                      "\033[36m-i/e case=pattern\033[0m: include in case name\n"
                      "\033[36m-i/e tags=pattern\033[0m: include in suite tags or case tags\n");
   }
   if (option && !strcmp("W", option)) {
      return ::printf("\033[36mexception\033[0m: consider thrown exception as warning\n"
                      "\033[36muncaught\033[0m: consider uncaught exception as warning\n"
                      "\033[36mleak\033[0m: consider memory leak as warning\n"
                      "\033[36mviolate\033[0m: consider memory violate access as warning\n"
                      "\033[36mdouble_free\033[0m: consider memory double free as warning\n"
                      "\033[36masymmetric_free\033[0m: consider memory asymmetric free as warning\n");
   }
   ::printf(" \033[32mh₂unit\033[0m \033[90mv\033[0m%s  \033[90m%s\033[0m  \033[90m%s\033[0m\n", H2PP_STR(H2UNIT_VERSION), H2PP_STR(H2UNIT_DATE), H2PP_STR(H2UNIT_REVISION));
   /* clang-format off */
#define H2_USAGE_BR "\033[90m├─────┼───────────┼────────────────────────────────────────────────────────────┤\033[0m\n"
   return ::printf("\033[90m┌─────┬───────────┬────────────────────────────────────────────────────────────┐\033[0m\n"
                   "\033[90m│\033[0m" " -\033[36mb\033[0m  "                               "\033[90m│\033[0m" "   \033[90m[\033[0mn=1\033[90m]\033[0m   "     "\033[90m│\033[0m" " \033[36mb\033[0mreak test once n (default 1) cases failed                 "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mc\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36mc\033[0montinue asserts even if failure occurred                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mC\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Console output in black-white \033[36mC\033[0molor style                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36md\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " \033[36md\033[0mebug with gdb/lldb once failure occurred                  "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mf\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Only test last \033[36mf\033[0mailed cases                                "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mF\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mn=max\033[90m]\033[0m  "     "\033[90m│\033[0m" " \033[36mF\033[0mold json print, 0:unfold 1:short 2:same 3:single          "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mi\033[0m\033[90m/\033[0m\033[36me\033[0m" "\033[90m│\033[0m" "\033[90m[\033[0mpattern .\033[90m]\033[0m"     "\033[90m│\033[0m" " \033[36mi\033[0mnclude\033[90m/\033[0m\033[36me\033[0mxclude by case suite file or tags, -h i...        " "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36ml\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mtype .\033[90m]\033[0m "     "\033[90m│\033[0m" " \033[36ml\033[0mist suites cases and tags, type [suite case todo tags]    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mm\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Test cases without \033[36mm\033[0memory check                            "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mo\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mpath\033[90m]\033[0m   "     "\033[90m│\033[0m" " \033[36mo\033[0mutput junit report, default is <executable>.junit.xml     "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mp\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " Disable test percentage \033[36mp\033[0mrogressing bar                    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mr\033[0m  "                               "\033[90m│\033[0m" "   \033[90m[\033[0mn=2\033[90m]\033[0m   "     "\033[90m│\033[0m" " Repeat test n (default 2) \033[36mr\033[0mounds                           "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36ms\033[0m  "                               "\033[90m│\033[0m" "\033[90m[\033[0mtype=rand\033[90m]\033[0m"     "\033[90m│\033[0m" " \033[36ms\033[0mhuffle cases random/alphabet/reverse if no last failed    "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mS\033[0m  "                               "\033[90m│\033[0m" " \033[90m[\033[0mtype=\\\"\033[90m]\033[0m "   "\033[90m│\033[0m" " JSON C/C++ \033[36mS\033[0mource code, type [\\\'/single \\\"/double \\\\\\\"]    "                       "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mv\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mn=max\033[90m]\033[0m  "     "\033[90m│\033[0m" " \033[36mv\033[0merbose, 0:quiet 1/2/3:compact 4:normal 5:details          "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mx\033[0m  "                               "\033[90m│\033[0m" "           "                                   "\033[90m│\033[0m" " e\033[36mx\033[0mit code as failed cases count                            "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mW\033[0m  "                               "\033[90m│\033[0m" "  \033[90m[\033[0mtype .\033[90m]\033[0m "     "\033[90m│\033[0m" " Configure failure as \033[36mW\033[0marning, exception, leak, -h W...     "                               "\033[90m│\033[0m\n" H2_USAGE_BR
                   "\033[90m│\033[0m" " -\033[36mh\033[0m\033[90m/\033[0m\033[36m?\033[0m" "\033[90m│\033[0m" "  \033[90m[\033[0moption\033[90m]\033[0m "     "\033[90m│\033[0m" " https://github.com/lingjf/h2unit/.../document/option.md    "                                              "\033[90m│\033[0m\n"
                   "\033[90m└─────┴───────────┴────────────────────────────────────────────────────────────┘\033[0m\n");
   /* clang-format on */
}

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
      for (int k = 0; k < argc; ++k) s += sprintf(s, " %s", args[k]);
   }
};

h2_inline void h2_option::parse(int argc, const char** argv)
{
   path = argv[0];
   getopt get(argc - 1, argv + 1);
   get.arguments(args);

   char t2[64];
   for (const char* t;;) {
      switch (get.next_option()) {
         case '\0': return;
         case 'b': get.extract_number(break_after_fails = 1); break;
         case 'c': continue_assert = true; break;
         case 'C': colorful = !colorful; break;
         case 'd': debugger_trap = true; break;
         case 'e':
            while ((t = get.extract_string())) {
               const char* eq = strchr(t, '=');
               if (eq) {
                  const char* r = h2_candidate(strcpyn(t2, t, eq - t), 4, "file", "suite", "case", "tags");
                  if (!strcmp("file", r)) h2_array_append(file_excludes, eq + 1);
                  else if (!strcmp("suite", r)) h2_array_append(suite_excludes, eq + 1);
                  else if (!strcmp("case", r)) h2_array_append(case_excludes, eq + 1);
                  else if (!strcmp("tags", r)) h2_array_append(tags_excludes, eq + 1);
                  else ::printf("-e %s\n", r), exit(-1);
               } else h2_array_append(excludes, t);
            }
            break;
         case 'f': only_last_failed = true; break;
         case 'F': get.extract_number(fold_json = 0); break;
         case 'i':
            while ((t = get.extract_string())) {
               const char* eq = strchr(t, '=');
               if (eq) {
                  const char* r = h2_candidate(strcpyn(t2, t, eq - t), 4, "file", "suite", "case", "tags");
                  if (!strcmp("file", r)) h2_array_append(file_includes, eq + 1);
                  else if (!strcmp("suite", r)) h2_array_append(suite_includes, eq + 1);
                  else if (!strcmp("case", r)) h2_array_append(case_includes, eq + 1);
                  else if (!strcmp("tags", r)) h2_array_append(tags_includes, eq + 1);
                  else ::printf("-i %s\n", r), exit(-1);
               } else h2_array_append(includes, t);
            }
            break;
         case 'l':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 4, "suite", "case", "todo", "tags");
               if (!strcmp("suite", r)) lists |= ListSuite;
               else if (!strcmp("case", r)) lists |= ListCase;
               else if (!strcmp("todo", r)) lists |= ListTodo;
               else if (!strcmp("tags", r)) lists |= ListTags;
               else ::printf("-l %s\n", r), exit(-1);
            }
            if (!lists) lists = ListSuite | ListCase | ListTodo;
            break;
         case 'm': memory_check = !memory_check; break;
         case 'o':
            sprintf(junit_path, "%s.junit.xml", path);
            if ((t = get.extract_string())) strcpy(junit_path, t);
            break;
         case 'p': progressing = !progressing; break;
         case 'r': get.extract_number(run_rounds = 2); break;
         case 's':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 4, "random", "name", "file", "reverse");
               if (!strcmp("random", r)) shuffles |= ShuffleRandom;
               else if (!strcmp("name", r)) shuffles |= ShuffleName;
               else if (!strcmp("file", r)) shuffles |= ShuffleFile;
               else if (!strcmp("reverse", r)) shuffles |= ShuffleReverse;
               else ::printf("-s %s\n", r), exit(-1);
            }
            if (!shuffles) shuffles = ShuffleRandom;
            break;
         case 'S':
            json_source_quote = "\\\"";
            if ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 5, "\'", "single", "\"", "double", "\\\"");
               if (!strcmp("\'", r) || !strcmp("single", r)) json_source_quote = "\'";
               else if (!strcmp("\"", r) || !strcmp("double", r)) json_source_quote = "\"";
               else if (!strcmp("\\\"", r)) json_source_quote = "\\\"";
               else ::printf("-S %s\n", r), exit(-1);

               if (!h2_in(json_source_quote, 3, "\'", "\"", "\\\"")) json_source_quote = "\\\"";
            }
            break;
         case 'v': get.extract_number(verbose = 8); break;
         case 'x': exit_with_fails = true; break;
         case 'W':
            while ((t = get.extract_string())) {
               const char* r = h2_candidate(t, 5, "exception", "uncaught", "leak", "violate", "asymmetric_free");
               if (!strcmp("exception", r)) as_waring_exception = true;
               else if (!strcmp("uncaught", r)) as_waring_uncaught = true;
               else if (!strcmp("leak", r)) as_waring_memory_leak = true;
               else if (!strcmp("violate", r)) as_waring_memory_violate = true;
               else if (!strcmp("double_free", r)) as_waring_memory_double_free = true;
               else if (!strcmp("asymmetric_free", r)) as_waring_memory_asymmetric_free = true;
               else ::printf("-W %s\n", r), exit(-1);
            }
            break;
         case 'h':
         case '?': usage(get.extract_string()); exit(0);
      }
   }
}
}
#endif
