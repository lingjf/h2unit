
#define __H2SUITE(suite_name, suite_test)                                         \
   static void suite_test(h2::h2_suite*, h2::h2_case*);                           \
   static h2::h2_suite H2PP_UNIQUE(si)(H2_FILE, h2::ss(suite_name), &suite_test); \
   static void suite_test(h2::h2_suite* suite_1_5_2_8_0_1_1_9_8, h2::h2_case* case_1_1_0_2_6_0_0_2_4)

#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2PP_UNIQUE(suite_test_C))

#define H2Setup() if (case_1_1_0_2_6_0_0_2_4)
#define H2Cleanup() if (::setjmp(suite_1_5_2_8_0_1_1_9_8->ctx))

#define __H2Case(case_name, case_instance, suite_cleaner, case_cleaner, todo)                                             \
   static h2::h2_case case_instance(H2_FILE, h2::ss(case_name), todo);                                                    \
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
      static h2::h2_case case_instance(H2_FILE, h2::ss(case_name), todo);                             \
      static h2::h2_suite::registor suite_registor(suite_1_5_2_8_0_1_1_9_8, &case_instance);          \
      if (&case_instance == case_1_1_0_2_6_0_0_2_4)                                                   \
         for (h2::h2_case::cleaner case_cleaner(&case_instance); case_cleaner;)                       \
            if (!::setjmp(case_instance.ctx))                                                         \
               case_test();                                                                           \
   }                                                                                                  \
   static h2::h2_suite H2PP_UNIQUE(si)(H2_FILE, nullptr, &suite_test);                                \
   static void case_test()

#define H2CASE(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(case_test_C), H2PP_UNIQUE(suite_test_C), 0)
#define H2TODO(...) __H2CASE(#__VA_ARGS__, H2PP_UNIQUE(case_test_C), H2PP_UNIQUE(suite_test_C), 1)

/* clang-format off */
// S6: H2_An((a, b, c)) ==> a
#define H2_An(...) H2PP_EVAL(_H2_An(__VA_ARGS__))
#define _H2_An(...) H2PP_CAT2(_H2_An1_, H2PP_IS_EMPTY(__VA_ARGS__)) ((__VA_ARGS__))
#define _H2_An1_1(...) 0
#define _H2_An1_0(MSVC_Workaround) _H2_An1 MSVC_Workaround
#define _H2_An1(a, ...) H2PP_CAT2(_H2_An2_, H2PP_IS_BEGIN_PARENTHESIS(a)) (a)
#define _H2_An2_0(a) a
#define _H2_An2_1(a) H2PP_DEFER(_H2_An2)() a
#define _H2_An2() _H2_An

#define H2_XAn(...) H2_An(H2PP_HEAD(__VA_ARGS__))
#define H2_YAn(...) H2_An(H2PP_LAST(__VA_ARGS__))
/* clang-format on */

#define H2CASES(case_prefix, ...) __H2CASES(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASES(case_test, case_prefix, ...)                                                          \
   template <typename T>                                                                                \
   static void case_test(T x);                                                                          \
   H2PP_FOREACH(, __H2CASES_Macro1, (case_test, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   template <typename T>                                                                                \
   static void case_test(T x)

#define __H2CASES_Macro4(case_test, case_prefix, i, x) \
   H2CASE(case_prefix i. x) { case_test(x); }
#define __H2CASES_Macro3(MSVC_Workaround) H2PP_RESCAN(__H2CASES_Macro4 MSVC_Workaround)
#define __H2CASES_Macro2(...) __H2CASES_Macro3((__VA_ARGS__))
#define __H2CASES_Macro1(args, i, x) __H2CASES_Macro2(H2PP_REMOVE_PARENTHESES(args), i, x)

#define H2CASESS(case_prefix, ...) __H2CASESS(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASESS(case_test, case_prefix, ...)                               \
   template <typename Tx, typename Ty>                                        \
   static void case_test(Tx x, Ty y);                                         \
   H2PP_FULLMESH(, __H2CASESS_Macro1, (case_test, case_prefix), __VA_ARGS__); \
   template <typename Tx, typename Ty>                                        \
   static void case_test(Tx x, Ty y)

#define __H2CASESS_Macro4(case_test, case_prefix, i, j, x, y) \
   H2CASE(case_prefix i.j. x, y) { case_test(x, y); }
#define __H2CASESS_Macro3(MSVC_Workaround) H2PP_RESCAN(__H2CASESS_Macro4 MSVC_Workaround)
#define __H2CASESS_Macro2(...) __H2CASESS_Macro3((__VA_ARGS__))
#define __H2CASESS_Macro1(args, i, j, x, y) __H2CASESS_Macro2(H2PP_REMOVE_PARENTHESES(args), i, j, x, y)

#define H2Cases(case_prefix, ...) __H2Cases(H2PP_UNIQUE(x), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), case_prefix, __VA_ARGS__)
#define __H2Cases(Qx, suite_cleaner, case_cleaner, case_prefix, ...)                             \
   auto Qx = H2_An(__VA_ARGS__);                                                                 \
   H2PP_FOREACH(, __H2Cases_Macro1, (Qx, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   if (case_1_1_0_2_6_0_0_2_4 && case_1_1_0_2_6_0_0_2_4->scheduled)                              \
      for (auto x = Qx; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)                \
         for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner;)      \
            for (h2::h2_case::cleaner case_cleaner(case_1_1_0_2_6_0_0_2_4); case_cleaner;)       \
               if (!::setjmp(case_1_1_0_2_6_0_0_2_4->ctx))

#define __H2Cases_Macro4(case_instance, Qx, case_prefix, i, x)                             \
   static h2::h2_case case_instance(H2_FILE, H2PP_STR(case_prefix i. x), 0);                \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance); \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4) Qx = x, case_1_1_0_2_6_0_0_2_4->scheduled = true;

#define __H2Cases_Macro3(MSVC_Workaround) H2PP_RESCAN(__H2Cases_Macro4 MSVC_Workaround)
#define __H2Cases_Macro2(...) __H2Cases_Macro3((__VA_ARGS__))
#define __H2Cases_Macro1(args, i, x) __H2Cases_Macro2(H2PP_UNIQUE(ci), H2PP_REMOVE_PARENTHESES(args), i, x)

#define H2Casess(case_prefix, ...) __H2Casess(H2PP_UNIQUE(x), H2PP_UNIQUE(y), H2PP_UNIQUE(sc), H2PP_UNIQUE(cc), case_prefix, __VA_ARGS__)
#define __H2Casess(Qx, Qy, suite_cleaner, case_cleaner, case_prefix, ...)                      \
   auto Qx = H2_XAn(__VA_ARGS__);                                                              \
   auto Qy = H2_YAn(__VA_ARGS__);                                                              \
   H2PP_FULLMESH(, __H2Casess_Macro1, (Qx, Qy, case_prefix), __VA_ARGS__);                     \
   if (case_1_1_0_2_6_0_0_2_4 && case_1_1_0_2_6_0_0_2_4->scheduled)                            \
      for (auto x = Qx; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)              \
         for (auto y = Qy; case_1_1_0_2_6_0_0_2_4; case_1_1_0_2_6_0_0_2_4 = nullptr)           \
            for (h2::h2_suite::cleaner suite_cleaner(suite_1_5_2_8_0_1_1_9_8); suite_cleaner;) \
               for (h2::h2_case::cleaner case_cleaner(case_1_1_0_2_6_0_0_2_4); case_cleaner;)  \
                  if (!::setjmp(case_1_1_0_2_6_0_0_2_4->ctx))

#define __H2Casess_Macro4(case_instance, Qx, Qy, case_prefix, i, j, x, y)                  \
   static h2::h2_case case_instance(H2_FILE, H2PP_STR(case_prefix i.j. x, y), 0);           \
   static h2::h2_suite::registor H2PP_UNIQUE(sr)(suite_1_5_2_8_0_1_1_9_8, &case_instance); \
   if (&case_instance == case_1_1_0_2_6_0_0_2_4) Qx = x, Qy = y, case_1_1_0_2_6_0_0_2_4->scheduled = true;

#define __H2Casess_Macro3(MSVC_Workaround) H2PP_RESCAN(__H2Casess_Macro4 MSVC_Workaround)
#define __H2Casess_Macro2(...) __H2Casess_Macro3((__VA_ARGS__))
#define __H2Casess_Macro1(args, i, j, x, y) __H2Casess_Macro2(H2PP_UNIQUE(ci), H2PP_REMOVE_PARENTHESES(args), i, j, x, y)

// can't declare/define template class/function in function scope
#define H2Cases_t(...) static_assert(false, "Cases_t not implemented, use H2CASES_T outside of SUITE{}");
#define H2Casess_t(...) static_assert(false, "Casess_t not implemented, use H2CASESS_T outside of SUITE{}");

#define H2CASES_T(case_prefix, ...) __H2CASES_T(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASES_T(case_test, case_prefix, ...)                                                          \
   template <typename x>                                                                                  \
   static void case_test();                                                                               \
   H2PP_FOREACH(, __H2CASES_T_Macro1, (case_test, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   template <typename x>                                                                                  \
   static void case_test()

#define __H2CASES_T_Macro4(case_test, case_prefix, i, x) \
   H2CASE(case_prefix i. x) { case_test<x>(); }
#define __H2CASES_T_Macro3(MSVC_Workaround) H2PP_RESCAN(__H2CASES_T_Macro4 MSVC_Workaround)
#define __H2CASES_T_Macro2(...) __H2CASES_T_Macro3((__VA_ARGS__))
#define __H2CASES_T_Macro1(args, i, x) __H2CASES_T_Macro2(H2PP_REMOVE_PARENTHESES(args), i, x)

#define H2CASESS_T(case_prefix, ...) __H2CASESS_T(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASESS_T(case_test, case_prefix, ...)                               \
   template <typename x, typename y>                                            \
   static void case_test();                                                     \
   H2PP_FULLMESH(, __H2CASESS_T_Macro1, (case_test, case_prefix), __VA_ARGS__); \
   template <typename x, typename y>                                            \
   static void case_test()

#define __H2CASESS_T_Macro4(case_test, case_prefix, i, j, x, y) \
   H2CASE(case_prefix i.j. x, y) { case_test<x, y>(); }
#define __H2CASESS_T_Macro3(MSVC_Workaround) H2PP_RESCAN(__H2CASESS_T_Macro4 MSVC_Workaround)
#define __H2CASESS_T_Macro2(...) __H2CASESS_T_Macro3((__VA_ARGS__))
#define __H2CASESS_T_Macro1(args, i, j, x, y) __H2CASESS_T_Macro2(H2PP_REMOVE_PARENTHESES(args), i, j, x, y)

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
#define __H2TODOS(case_test, case_prefix, ...) \
   template <typename T>                       \
   static void case_test(T x);                 \
   H2TODO(case_prefix, __VA_ARGS__) {}         \
   template <typename T>                       \
   static void case_test(T x)

#define H2TODOSS(case_prefix, ...) __H2TODOSS(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2TODOSS(case_test, case_prefix, ...) \
   template <typename Tx, typename Ty>          \
   static void case_test(Tx x, Ty y);           \
   H2TODO(case_prefix, __VA_ARGS__) {}          \
   template <typename Tx, typename Ty>          \
   static void case_test(Tx x, Ty y)

#define H2TODOS_T(case_prefix, ...) __H2TODOS_T(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2TODOS_T(case_test, case_prefix, ...) \
   template <typename x>                         \
   static void case_test();                      \
   H2TODO(case_prefix, __VA_ARGS__) {}           \
   template <typename x>                         \
   static void case_test()

#define H2TODOSS_T(case_prefix, ...) __H2TODOSS_T(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2TODOSS_T(case_test, case_prefix, ...) \
   template <typename x, typename y>              \
   static void case_test();                       \
   H2TODO(case_prefix, __VA_ARGS__) {}            \
   template <typename x, typename y>              \
   static void case_test()
