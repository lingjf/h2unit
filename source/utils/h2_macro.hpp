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
