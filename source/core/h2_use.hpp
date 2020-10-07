
#define __H2SUITE(name, Q)                                                 \
   static void Q(h2::h2_suite*, h2::h2_case*);                             \
   static h2::h2_suite H2PP_UNIQUE(s_suite)(name, &Q, __FILE__, __LINE__); \
   static void Q(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5)

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2PP_UNIQUE(h2_suite_test))

#define H2Cleanup()                                \
   if (::setjmp(suite_2_0_1_3_0_1_0_2->jump) == 0) \
      suite_2_0_1_3_0_1_0_2->jumpable = true;      \
   if (!case_2_0_1_7_0_3_2_5)

#define __H2Case(name, status, Q)                                                                             \
   static h2::h2_case Q(name, status, __FILE__, __LINE__);                                                    \
   static h2::h2_suite::installer H2PP_UNIQUE(s_installer)(suite_2_0_1_3_0_1_0_2, &Q);                        \
   if (&Q == case_2_0_1_7_0_3_2_5)                                                                            \
      for (h2::h2_suite::cleaner _1_9_8_0_(suite_2_0_1_3_0_1_0_2); _1_9_8_0_; case_2_0_1_7_0_3_2_5 = nullptr) \
         for (h2::h2_case::cleaner _1_9_8_1_(&Q); _1_9_8_1_;)                                                 \
            if (::setjmp(Q.jump) == 0)

#define H2Case(...) __H2Case(#__VA_ARGS__, h2::h2_case::initial, H2PP_UNIQUE(s_case))
#define H2Todo(...) __H2Case(#__VA_ARGS__, h2::h2_case::todo, H2PP_UNIQUE(s_case))

#define __H2CASE(name, status, QC, QS)                                                    \
   static void QC();                                                                      \
   static void QS(h2::h2_suite* suite_2_0_1_3_0_1_0_2, h2::h2_case* case_2_0_1_7_0_3_2_5) \
   {                                                                                      \
      static h2::h2_case c(name, status, __FILE__, __LINE__);                             \
      static h2::h2_suite::installer i(suite_2_0_1_3_0_1_0_2, &c);                        \
      if (&c == case_2_0_1_7_0_3_2_5)                                                     \
         for (h2::h2_case::cleaner t(&c); t;)                                             \
            if (::setjmp(c.jump) == 0)                                                    \
               QC();                                                                      \
   }                                                                                      \
   static h2::h2_suite H2PP_UNIQUE(s_suite)("", &QS, __FILE__, __LINE__);                 \
   static void QC()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, h2::h2_case::initial, H2PP_UNIQUE(h2_case_test), H2PP_UNIQUE(h2_suite_test))
#define H2TODO(...) __H2CASE(#__VA_ARGS__, h2::h2_case::todo, H2PP_UNIQUE(h2_case_test), H2PP_UNIQUE(h2_suite_test))

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

#define ___H2Cases_Macro(Qj, Qb, Qx, Ql, i, x) \
   H2Case(i. x)                                \
   {                                           \
      if (::setjmp(Qj) == 0) {                 \
         Qx = x;                               \
         Qb = true;                            \
         goto Ql;                              \
      }                                        \
   };                                          \
   Qb = false;
#define __H2Cases_Macro(...) ___H2Cases_Macro(__VA_ARGS__)
#define _H2Cases_Macro(args, i, x) __H2Cases_Macro(H2PP_REMOVE_PARENTHESES(args), i, x)
#define __H2Cases(Qj, Qb, Qx, Ql, ...)                            \
   jmp_buf Qj;                                                    \
   bool Qb = false;                                               \
   auto Qx = _H2_An_ForEach(__VA_ARGS__);                         \
   H2PP_FOREACH(, _H2Cases_Macro, (Qj, Qb, Qx, Ql), __VA_ARGS__); \
   Ql:                                                            \
   for (auto x = Qx; Qb; Qb = false, ::longjmp(Qj, 1))
#define H2Cases(...) __H2Cases(H2PP_UNIQUE(j), H2PP_UNIQUE(b), H2PP_UNIQUE(v), H2PP_UNIQUE(l), __VA_ARGS__)

#define ___H2Casess_Macro(Qj, Qb, Ql, Qx, Qy, i, j, x, y) \
   H2Case(i.j. x, y)                                      \
   {                                                      \
      if (::setjmp(Qj) == 0) {                            \
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
      } H2PP_UNIQUE(Global_Callback);                      \
   }                                                       \
   void Q::name()

#define H2GlobalSetup() __H2GlobalCallback(global_setup, H2PP_UNIQUE(Global_Setup))
#define H2GlobalTeardown() __H2GlobalCallback(global_teardown, H2PP_UNIQUE(Global_Teardown))

#define H2GlobalSuiteSetup() __H2GlobalCallback(global_suite_setup, H2PP_UNIQUE(Global_Suite_Setup))
#define H2GlobalSuiteTeardown() __H2GlobalCallback(global_suite_teardown, H2PP_UNIQUE(Global_Suite_Teardown))

#define H2GlobalCaseSetup() __H2GlobalCallback(global_case_setup, H2PP_UNIQUE(Global_Case_Setup))
#define H2GlobalCaseTeardown() __H2GlobalCallback(global_case_teardown, H2PP_UNIQUE(Global_Case_Teardown))
