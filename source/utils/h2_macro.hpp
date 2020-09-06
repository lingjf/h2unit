/* clang-format off */

#define H2PP_STR(...) _H2PP_STR(__VA_ARGS__)
#define _H2PP_STR(...) #__VA_ARGS__

#define H2PP__CAT2(_1, _2) _1##_2
#define H2PP_CAT2(_1, _2) H2PP__CAT2(_1, _2)
#define H2PP__CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define H2PP_CAT5(_1, _2, _3, _4, _5) H2PP__CAT5(_1, _2, _3, _4, _5)

#define H2PP_CAT(_1, _2) H2PP__CAT2(_1, _2)

#define H2PP_COMMA(...) ,
#define H2PP_EAT(...)
#define H2PP_DEFER(...) __VA_ARGS__ H2PP_EAT()

#define H2PP_EVAL(...) H2PP_EVAL128(__VA_ARGS__)
#define H2PP_EVAL1024(...) H2PP_EVAL512(H2PP_EVAL512(__VA_ARGS__))
#define H2PP_EVAL512(...) H2PP_EVAL256(H2PP_EVAL256(__VA_ARGS__))
#define H2PP_EVAL256(...) H2PP_EVAL128(H2PP_EVAL128(__VA_ARGS__))
#define H2PP_EVAL128(...) H2PP_EVAL64(H2PP_EVAL64(__VA_ARGS__))
#define H2PP_EVAL64(...) H2PP_EVAL32(H2PP_EVAL32(__VA_ARGS__))
#define H2PP_EVAL32(...) H2PP_EVAL16(H2PP_EVAL16(__VA_ARGS__))
#define H2PP_EVAL16(...) H2PP_EVAL8(H2PP_EVAL8(__VA_ARGS__))
#define H2PP_EVAL8(...) H2PP_EVAL4(H2PP_EVAL4(__VA_ARGS__))
#define H2PP_EVAL4(...) H2PP_EVAL2(H2PP_EVAL2(__VA_ARGS__))
#define H2PP_EVAL2(...) H2PP_EVAL1(H2PP_EVAL1(__VA_ARGS__))
#define H2PP_EVAL1(...) __VA_ARGS__

#define H2PP_1st(_1, ...) _1
#define H2PP_2nd(_1, _2, ...) _2
#define H2PP_3rd(_1, _2, _3, ...) _3
#define H2PP_4th(_1, _2, _3, _4, ...) _4

// #define H2PP_IS_PROBE(...) H2PP_2nd(__VA_ARGS__, 0, )
#define H2PP_IS_PROBE(...) _H2PP_MSVC_VA_ARGS_WORKAROUND(H2PP_2nd, (__VA_ARGS__, 0))
#define _H2PP_MSVC_VA_ARGS_WORKAROUND(_Define, _Args) _Define _Args
#define H2PP_PROBE() ~, 1,

#define H2PP_EQ(x, y) H2PP_IS_PROBE(H2PP_CAT5(_H2PP_EQ, __, x, __, y))
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
#define _H2PP_EQ__32__32 H2PP_PROBE()

#define H2PP_NOT(_Cond) H2PP_IS_PROBE(H2PP__CAT2(_H2PP_NOT_, _Cond))
#define _H2PP_NOT_0 H2PP_PROBE()

#define H2PP_COMPL(_Value) H2PP__CAT2(_H2PP_COMPL_, _Value)
#define _H2PP_COMPL_0 1
#define _H2PP_COMPL_1 0

#define H2PP_BOOL(_Cond) H2PP_COMPL(H2PP_NOT(_Cond))  // non-zero and thus evaluate to 1

#define H2PP_AND(x, y) H2PP_CAT2(_H2PP_AND_, H2PP_BOOL(x))(H2PP_BOOL(y))
#define _H2PP_AND_0(y) 0
#define _H2PP_AND_1(y) y

#define H2PP_OR(x, y) H2PP_CAT2(_H2PP_OR_, H2PP_BOOL(x))(H2PP_BOOL(y))
#define _H2PP_OR_0(y) y
#define _H2PP_OR_1(y) 1

#define H2PP_IF_ELSE(_Cond, _Then, _Else) H2PP_CAT2(_H2PP_IF_ELSE_, _Cond) (_Then, _Else)
#define _H2PP_IF_ELSE_1(_Then, _Else) _Then
#define _H2PP_IF_ELSE_0(_Then, _Else) _Else

#define H2PP_IF(_Cond) H2PP_CAT2(_H2PP_IF_, H2PP_BOOL(_Cond))
#define _H2PP_IF_1(...) __VA_ARGS__
#define _H2PP_IF_0(...)

#define H2PP_IBP(...) H2PP_IS_PROBE(_H2PP_IS_BEGIN_PARENTHESIS_PROBE __VA_ARGS__)
#define _H2PP_IS_BEGIN_PARENTHESIS_PROBE(...) H2PP_PROBE()

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define H2PP_RPS(...) H2PP_CAT2(_H2PP_REMOVE_PARENTHESES_IF_, H2PP_IBP(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_1(...) H2PP_REMOVE_PARENTHESES(__VA_ARGS__)
#define _H2PP_REMOVE_PARENTHESES_IF_0(...) __VA_ARGS__

#define __H2PP_100TH(_100, _99, _98, _97, _96, _95, _94, _93, _92, _91, _90, _89, _88, _87, _86, _85, _84, _83, _82, _81, _80, _79, _78, _77, _76, _75, _74, _73, _72, _71, _70, _69, _68, _67, _66, _65, _64, _63, _62, _61, _60, _59, _58, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, _41, _40, _39, _38, _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, _x, ...) _x
#define __H2PP_RESCAN(_1) _1
#define _H2PP_100TH(_Args) __H2PP_RESCAN(__H2PP_100TH _Args)

#define H2PP_IS_EMPTY(...)                                  \
  _H2PP_IS_EMPTY(_H2PP_HAS_COMMA(__VA_ARGS__),              \
                 _H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__),   \
                 _H2PP_HAS_COMMA(__VA_ARGS__()),            \
                 _H2PP_HAS_COMMA(H2PP_COMMA __VA_ARGS__()))
#define _H2PP_IS_EMPTY(_1, _2, _3, _4) _H2PP_HAS_COMMA(H2PP__CAT5(_H2PP_IS_EMPTY_CASE_, _1, _2, _3, _4))
#define _H2PP_IS_EMPTY_CASE_0001 ,
#define _H2PP_HAS_COMMA(...) _H2PP_100TH((__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))

#define H2PP_NARG(...) H2PP_IF_ELSE(H2PP_IS_EMPTY(__VA_ARGS__), 0, _H2PP_NARG(__VA_ARGS__))
#define _H2PP_NARG(...) _H2PP_100TH((__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define H2PP_HEAD(...) H2PP_CAT2(_H2PP_HEAD_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_HEAD_0(_1, ...) _1
#define _H2PP_HEAD_1(...)

#define H2PP_TAIL(...) H2PP_CAT2(_H2PP_TAIL_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2PP_TAIL_0(_1, ...) __VA_ARGS__
#define _H2PP_TAIL_1(...)

#define H2PP_TH0(...) H2PP_HEAD(__VA_ARGS__)
#define H2PP_TH1(...) H2PP_HEAD(H2PP_TAIL(__VA_ARGS__))
#define H2PP_TH2(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))
#define H2PP_TH3(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))
#define H2PP_TH4(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))
#define H2PP_TH5(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))
#define H2PP_TH6(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))
#define H2PP_TH7(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))
#define H2PP_TH8(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))
#define H2PP_TH9(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))
#define H2PP_TH10(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))
#define H2PP_TH11(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))
#define H2PP_TH12(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))
#define H2PP_TH13(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))
#define H2PP_TH14(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))
#define H2PP_TH15(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))
#define H2PP_TH16(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))
#define H2PP_TH17(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))
#define H2PP_TH18(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))
#define H2PP_TH19(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))))
#define H2PP_TH20(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))))
#define H2PP_TH21(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))))))
#define H2PP_TH22(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))))))
#define H2PP_TH23(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))))))))
#define H2PP_TH24(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))))))))
#define H2PP_TH25(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))))))))))
#define H2PP_TH26(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))))))))))
#define H2PP_TH27(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))))))))))))
#define H2PP_TH28(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))))))))))))
#define H2PP_TH29(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))))))))))))))
#define H2PP_TH30(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))))))))))))))
#define H2PP_TH31(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__))))))))))))))))))))))))))))))))
#define H2PP_TH32(...) H2PP_HEAD(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(H2PP_TAIL(__VA_ARGS__)))))))))))))))))))))))))))))))))

#define H2PP_LAST(...) H2PP_CAT2(_H2PP_LAST_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__) //is empty?
#define _H2PP_LAST_1(...) // empty
#define _H2PP_LAST_0(...) H2PP_EVAL(_H2PP_LAST_(__VA_ARGS__)) //at least 1 argument
#define _H2PP_LAST_(...) H2PP_CAT2(_H2PP_LAST__, H2PP_EQ(1, H2PP_NARG(__VA_ARGS__))) (__VA_ARGS__) // one and only one?
#define _H2PP_LAST__0(...) H2PP_DEFER(_H2PP_LAST_I)()(H2PP_TAIL(__VA_ARGS__)) // shift first, and again
#define _H2PP_LAST__1(...) __VA_ARGS__ // only 1 argument
#define _H2PP_LAST_I() _H2PP_LAST_

#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_CAT2(_Macro, H2PP_NARG(__VA_ARGS__))(__VA_ARGS__)

#define __H2PP_CALL_MACRO(_Macro, _i, ...) H2PP_CAT2(__H2PP_CALL_MACRO_, H2PP_IS_EMPTY (__VA_ARGS__)) (_Macro, _i, __VA_ARGS__)
#define __H2PP_CALL_MACRO_1(_Macro, _i, ...) _Macro(_i)
#define __H2PP_CALL_MACRO_0(_Macro, _i, ...) _Macro(_i, __VA_ARGS__)

#define _H2PP_REPEAT_0(_Split, _Macro, ...)
#define _H2PP_REPEAT_1(_Split, _Macro, ...) __H2PP_CALL_MACRO(_Macro, 0, __VA_ARGS__)
#define _H2PP_REPEAT_2(_Split, _Macro, ...) _H2PP_REPEAT_1(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 1, __VA_ARGS__)
#define _H2PP_REPEAT_3(_Split, _Macro, ...) _H2PP_REPEAT_2(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 2, __VA_ARGS__)
#define _H2PP_REPEAT_4(_Split, _Macro, ...) _H2PP_REPEAT_3(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 3, __VA_ARGS__)
#define _H2PP_REPEAT_5(_Split, _Macro, ...) _H2PP_REPEAT_4(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 4, __VA_ARGS__)
#define _H2PP_REPEAT_6(_Split, _Macro, ...) _H2PP_REPEAT_5(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 5, __VA_ARGS__)
#define _H2PP_REPEAT_7(_Split, _Macro, ...) _H2PP_REPEAT_6(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 6, __VA_ARGS__)
#define _H2PP_REPEAT_8(_Split, _Macro, ...) _H2PP_REPEAT_7(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 7, __VA_ARGS__)
#define _H2PP_REPEAT_9(_Split, _Macro, ...) _H2PP_REPEAT_8(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 8, __VA_ARGS__)
#define _H2PP_REPEAT_10(_Split, _Macro, ...) _H2PP_REPEAT_9(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 9, __VA_ARGS__)
#define _H2PP_REPEAT_11(_Split, _Macro, ...) _H2PP_REPEAT_10(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 10, __VA_ARGS__)
#define _H2PP_REPEAT_12(_Split, _Macro, ...) _H2PP_REPEAT_11(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 11, __VA_ARGS__)
#define _H2PP_REPEAT_13(_Split, _Macro, ...) _H2PP_REPEAT_12(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 12, __VA_ARGS__)
#define _H2PP_REPEAT_14(_Split, _Macro, ...) _H2PP_REPEAT_13(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 13, __VA_ARGS__)
#define _H2PP_REPEAT_15(_Split, _Macro, ...) _H2PP_REPEAT_14(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 14, __VA_ARGS__)
#define _H2PP_REPEAT_16(_Split, _Macro, ...) _H2PP_REPEAT_15(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 15, __VA_ARGS__)
#define _H2PP_REPEAT_17(_Split, _Macro, ...) _H2PP_REPEAT_16(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 16, __VA_ARGS__)
#define _H2PP_REPEAT_18(_Split, _Macro, ...) _H2PP_REPEAT_17(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 17, __VA_ARGS__)
#define _H2PP_REPEAT_19(_Split, _Macro, ...) _H2PP_REPEAT_18(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 18, __VA_ARGS__)
#define _H2PP_REPEAT_20(_Split, _Macro, ...) _H2PP_REPEAT_19(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 19, __VA_ARGS__)
#define _H2PP_REPEAT_21(_Split, _Macro, ...) _H2PP_REPEAT_20(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 20, __VA_ARGS__)
#define _H2PP_REPEAT_22(_Split, _Macro, ...) _H2PP_REPEAT_21(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 21, __VA_ARGS__)
#define _H2PP_REPEAT_23(_Split, _Macro, ...) _H2PP_REPEAT_22(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 22, __VA_ARGS__)
#define _H2PP_REPEAT_24(_Split, _Macro, ...) _H2PP_REPEAT_23(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 23, __VA_ARGS__)
#define _H2PP_REPEAT_25(_Split, _Macro, ...) _H2PP_REPEAT_24(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 24, __VA_ARGS__)
#define _H2PP_REPEAT_26(_Split, _Macro, ...) _H2PP_REPEAT_25(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 25, __VA_ARGS__)
#define _H2PP_REPEAT_27(_Split, _Macro, ...) _H2PP_REPEAT_26(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 26, __VA_ARGS__)
#define _H2PP_REPEAT_28(_Split, _Macro, ...) _H2PP_REPEAT_27(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 27, __VA_ARGS__)
#define _H2PP_REPEAT_29(_Split, _Macro, ...) _H2PP_REPEAT_28(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 28, __VA_ARGS__)
#define _H2PP_REPEAT_30(_Split, _Macro, ...) _H2PP_REPEAT_29(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 29, __VA_ARGS__)
#define _H2PP_REPEAT_31(_Split, _Macro, ...) _H2PP_REPEAT_30(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 30, __VA_ARGS__)
#define _H2PP_REPEAT_32(_Split, _Macro, ...) _H2PP_REPEAT_31(_Split, _Macro, __VA_ARGS__) H2PP_RPS(_Split) __H2PP_CALL_MACRO(_Macro, 31, __VA_ARGS__)

#define H2PP_REPEAT(_Count, _Split, _Macro, ...) H2PP_CAT2(_H2PP_REPEAT_, _Count) (_Split, _Macro, __VA_ARGS__)

#define H2Q(...) H2PP_CAT5(__VA_ARGS__, _C, __COUNTER__, L, __LINE__)  // generate unique identifier [with prefix]
