
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
