
#define H2SUITE(...) __H2SUITE(#__VA_ARGS__, H2PP_UNIQUE(suite_test_C))
#define __H2SUITE(suite_describe, suite_test)                                                               \
   static void suite_test(h2::h2_suite*, h2::h2_case*);                                                     \
   static h2::h2_suite H2PP_UNIQUE(si)(H2_FILINE, __FILE__, __LINE__, h2::ss(suite_describe), &suite_test); \
   static void suite_test(h2::h2_suite* suite_1_5_2_8_0_1_1_9_8, h2::h2_case* case_1_1_0_2_6_0_0_2_4)

#define H2Setup() if (case_1_1_0_2_6_0_0_2_4)
#define H2Cleanup() if (::setjmp(suite_1_5_2_8_0_1_1_9_8->cleanup_hole))

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
#define _H2_An1(a, ...) H2PP_CAT2(_H2_An2_, H2PP_IS_BEGIN_PARENTHESIS(a)) (a)
#define _H2_An2_0(a) a
#define _H2_An2_1(a) H2PP_DEFER(_H2_An2)() a
#define _H2_An2() _H2_An

#define H2_XAn(...) H2_An(H2PP_HEAD(__VA_ARGS__))
#define H2_YAn(...) H2_An(H2PP_LAST(__VA_ARGS__))
/* clang-format on */

#define H2CASES(case_prefix, ...) __H2CASES1(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASES1(case_test, case_prefix, ...)                                                           \
   template <typename T>                                                                                  \
   static void case_test(T x);                                                                            \
   H2PP_FOREACH(, __H2CASES_Callback, (case_test, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   template <typename T>                                                                                  \
   static void case_test(T x)
#define __H2CASES2(case_test, case_prefix, i, x) \
   H2CASE(case_prefix i. x) { case_test(x); }
#define __H2CASES_Callback(Pack, i, x) H2PP_PROXY(__H2CASES2, (H2PP_REMOVE_PARENTHESES(Pack), i, x))

#define H2CASESS(case_prefix, ...) __H2CASESS1(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASESS1(case_test, case_prefix, ...)                                \
   template <typename Tx, typename Ty>                                          \
   static void case_test(Tx x, Ty y);                                           \
   H2PP_FULLMESH(, __H2CASESS_Callback, (case_test, case_prefix), __VA_ARGS__); \
   template <typename Tx, typename Ty>                                          \
   static void case_test(Tx x, Ty y)
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
   template <typename x>                                                                                    \
   static void case_test();                                                                                 \
   H2PP_FOREACH(, __H2CASES_T_Callback, (case_test, case_prefix), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__)); \
   template <typename x>                                                                                    \
   static void case_test()
#define __H2CASES_T2(case_test, case_prefix, i, x) \
   H2CASE(case_prefix i. x) { case_test<x>(); }
#define __H2CASES_T_Callback(Pack, i, x) H2PP_PROXY(__H2CASES_T2, (H2PP_REMOVE_PARENTHESES(Pack), i, x))

#define H2CASESS_T(case_prefix, ...) __H2CASESS_T1(H2PP_UNIQUE(case_test_C), case_prefix, __VA_ARGS__)
#define __H2CASESS_T1(case_test, case_prefix, ...)                                \
   template <typename x, typename y>                                              \
   static void case_test();                                                       \
   H2PP_FULLMESH(, __H2CASESS_T_Callback, (case_test, case_prefix), __VA_ARGS__); \
   template <typename x, typename y>                                              \
   static void case_test()
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
