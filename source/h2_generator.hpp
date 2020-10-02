
/* clang-format off */

#define _H2_Macro_II(_Func, _Args, ...) _Func(_Args, __VA_ARGS__)
#define _H2_Macro_I(...) _H2_Macro_II(__VA_ARGS__)


#define _H2_ForEachv(_Func, _Args, ...) H2PP_CAT(_H2_ForEachv_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, __VA_ARGS__)
#define _H2_ForEachv_I() _H2_ForEachv
#define _H2_ForEachv_0(_Func, _Args, ...) _H2_Macro_I(_Func, _Args, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachv_I)()(_Func, _Args, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachv_1(...)
#define H2ForEach(_Func, _Args, ...) H2PP_EVAL(_H2_ForEachv(_Func, _Args, __VA_ARGS__))

#define _H2_ForEachy(_Func, _Args, x, ...) H2PP_CAT(_H2_ForEachy_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, x, __VA_ARGS__)
#define _H2_ForEachy_I() _H2_ForEachy
#define _H2_ForEachy_0(_Func, _Args, x, ...) _H2_Macro_I(_Func, _Args, x, H2PP_HEAD(__VA_ARGS__)) H2PP_DEFER(_H2_ForEachy_I)()(_Func, _Args, x, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachy_1(...)
#define _H2_ForEachx(_Func, _Args, _Tupley, ...) H2PP_CAT(_H2_ForEachx_, H2PP_IS_EMPTY(__VA_ARGS__)) (_Func, _Args, _Tupley, __VA_ARGS__)
#define _H2_ForEachx_I() _H2_ForEachx
#define _H2_ForEachx_0(_Func, _Args, _Tupley, ...) _H2_ForEachy(_Func, _Args, H2PP_HEAD(__VA_ARGS__), H2PP_REMOVE_PARENTHESES(_Tupley)) H2PP_DEFER(_H2_ForEachx_I)()(_Func, _Args, _Tupley, H2PP_TAIL(__VA_ARGS__))
#define _H2_ForEachx_1(...)
#define _H2_Fullmesh_Impl(_Func, _Args, _Tuplex, _Tupley) H2PP_EVAL(_H2_ForEachx(_Func, _Args, _Tupley, H2PP_REMOVE_PARENTHESES(_Tuplex)))
#define __H2_Fullmesh_Prev0(_Func, _Args, ...) _H2_Fullmesh_Impl(_Func, _Args, (__VA_ARGS__), (__VA_ARGS__))
#define _H2_Fullmesh_Prev0(...) __H2_Fullmesh_Prev0(__VA_ARGS__)
#define _H2_Fullmesh_Prev1(...) _H2_Fullmesh_Impl(__VA_ARGS__)
#define _H2_Fullmesh_Prev(_Func, _Args, t, ...) H2PP_CAT(_H2_Fullmesh_Prev, H2PP_AND(H2PP_IBP(t), H2PP_EQ(1, H2PP_NARG(__VA_ARGS__)))) (_Func, _Args, t, __VA_ARGS__)
#define H2Fullmesh(_Func, _Args, ...) _H2_Fullmesh_Prev(_Func, _Args, __VA_ARGS__)


#define ForForEach_Adapter(_Args, x) H2PP_REMOVE_PARENTHESES(_Args)(x)
#define ForForEach(_Macro_x, ...) H2ForEach(ForForEach_Adapter, (_Macro_x), __VA_ARGS__)

#define ForFullmesh_Adapter(_Args, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)
#define ForFullmesh(_Macro_x_y, ...) H2Fullmesh(ForFullmesh_Adapter, (_Macro_x_y), __VA_ARGS__)


#define H2List_An(...) H2PP_CAT(_H2List_An_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2List_An_0(_1, ...) _1
#define _H2List_An_1(...) 0

#define H2ForEach_An(...) H2PP_CAT(_H2ForEach_An_, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
#define _H2ForEach_An_1(...) 0
#define _H2ForEach_An_0(_1, ...) H2PP_CAT(_H2ForEach_An_0_, H2PP_IBP(_1))(_1)
#define _H2ForEach_An_0_0(_1) H2List_An(_1)
#define _H2ForEach_An_0_1(_1) H2List_An(H2PP_REMOVE_PARENTHESES(_1))

#define H2Fullmesh_Ax(...) H2ForEach_An(H2PP_HEAD(__VA_ARGS__))
#define H2Fullmesh_Ay(...) H2ForEach_An(H2PP_LAST(__VA_ARGS__))

#define ___ForEach_CASE_Macro(Qc, x) CASE(x) { Qc(x); }
#define __ForEach_CASE_Macro(...) ___ForEach_CASE_Macro(__VA_ARGS__)
#define _ForEach_CASE_Macro(Args, x) __ForEach_CASE_Macro(H2PP_REMOVE_PARENTHESES(Args), x)
#define _ForEach_CASE_Impl(Qc, ...)                   \
   template <typename T>                              \
   void Qc(T x);                                      \
   H2ForEach(_ForEach_CASE_Macro, (Qc), __VA_ARGS__); \
   template <typename T>                              \
   void Qc(T x)
#define H2CASES(...) _ForEach_CASE_Impl(H2Q(f), __VA_ARGS__)

#define ___Fullmesh_CASE_Macro(Qc, x, y) CASE(x, y) { Qc(x, y); }
#define __Fullmesh_CASE_Macro(...) ___Fullmesh_CASE_Macro(__VA_ARGS__)
#define _Fullmesh_CASE_Macro(Args, x, y) __Fullmesh_CASE_Macro(H2PP_REMOVE_PARENTHESES(Args), x, y)
#define _Fullmesh_CASE_Impl(Qc, ...)                    \
   template <typename T, typename U>                    \
   void Qc(T x, U y);                                   \
   H2Fullmesh(_Fullmesh_CASE_Macro, (Qc), __VA_ARGS__); \
   template <typename T, typename U>                    \
   void Qc(T x, U y)
#define H2CASESS(...) _Fullmesh_CASE_Impl(H2Q(f), __VA_ARGS__)

#define ___ForEach_Case_Macro(Qj, Qb, Qx, Ql, x) \
   Case(x)                                       \
   {                                             \
      if (::setjmp(Qj) == 0) {                   \
         Qx = x;                                 \
         Qb = true;                              \
         goto Ql;                                \
      } else {                                   \
      }                                          \
   };                                            \
   Qb = false;
#define __ForEach_Case_Macro(...) ___ForEach_Case_Macro(__VA_ARGS__)
#define _ForEach_Case_Macro(Args, x) __ForEach_Case_Macro(H2PP_REMOVE_PARENTHESES(Args), x)
#define _ForEach_Case_Impl(Qj, Qb, Qx, Ql, ...)                   \
   jmp_buf Qj;                                                    \
   bool Qb = false;                                               \
   auto Qx = H2ForEach_An(__VA_ARGS__);                           \
   H2ForEach(_ForEach_Case_Macro, (Qj, Qb, Qx, Ql), __VA_ARGS__); \
   Ql:                                                            \
   for (auto x = Qx; Qb; Qb = false, ::longjmp(Qj, 1))
#define H2Cases(...) _ForEach_Case_Impl(H2Q(j), H2Q(b), H2Q(v), H2Q(l), __VA_ARGS__)

#define ___Fullmesh_Case_Macro(Qj, Qb, Ql, Qx, Qy, x, y) \
   Case(x, y)                                            \
   {                                                     \
      if (::setjmp(Qj) == 0) {                           \
         Qx = x;                                         \
         Qy = y;                                         \
         Qb = true;                                      \
         goto Ql;                                        \
      } else {                                           \
      }                                                  \
   };                                                    \
   Qb = false;
#define __Fullmesh_Case_Macro(...) ___Fullmesh_Case_Macro(__VA_ARGS__)
#define _Fullmesh_Case_Macro(Args, x, y) __Fullmesh_Case_Macro(H2PP_REMOVE_PARENTHESES(Args), x, y)
#define _Fullmesh_Case_Impl(Qj, Qb, Ql, Qx, Qy, ...)                    \
   jmp_buf Qj;                                                          \
   bool Qb = false;                                                     \
   auto Qx = H2Fullmesh_Ax(__VA_ARGS__);                                \
   auto Qy = H2Fullmesh_Ay(__VA_ARGS__);                                \
   H2Fullmesh(_Fullmesh_Case_Macro, (Qj, Qb, Ql, Qx, Qy), __VA_ARGS__); \
   Ql:                                                                  \
   for (auto x = Qx; Qb; ::longjmp(Qj, 1))                              \
      for (auto y = Qy; Qb; Qb = false)
#define H2Casess(...) _Fullmesh_Case_Impl(H2Q(j), H2Q(b), H2Q(l), H2Q(x), H2Q(y), __VA_ARGS__)

#define ___H2CASES_T_Macro(Qc, x) CASE(x) { Qc<x>(); }
#define __H2CASES_T_Macro(...) ___H2CASES_T_Macro(__VA_ARGS__)
#define _H2CASES_T_Macro(args, x) __H2CASES_T_Macro(H2PP_REMOVE_PARENTHESES(args), x)
#define __H2CASES_T(Qc, ...)                       \
   template <typename x>                           \
   void Qc();                                      \
   H2ForEach(_H2CASES_T_Macro, (Qc), __VA_ARGS__); \
   template <typename x>                           \
   void Qc()
#define H2CASES_T(...) __H2CASES_T(H2Q(f), __VA_ARGS__)

#define ___H2CASESS_T_Macro(Qc, x, y) CASE(x, y) { Qc<x, y>(); }
#define __H2CASESS_T_Macro(...) ___H2CASESS_T_Macro(__VA_ARGS__)
#define _H2CASESS_T_Macro(args, x, y) __H2CASESS_T_Macro(H2PP_REMOVE_PARENTHESES(args), x, y)
#define __H2CASESS_T(Qc, ...)                        \
   template <typename x, typename y>                 \
   void Qc();                                        \
   H2Fullmesh(_H2CASESS_T_Macro, (Qc), __VA_ARGS__); \
   template <typename x, typename y>                 \
   void Qc()
#define H2CASESS_T(...) __H2CASESS_T(H2Q(f), __VA_ARGS__)
