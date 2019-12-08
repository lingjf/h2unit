/* v3.1 */

#ifndef ___H2UNIT_H_3_0__
#define ___H2UNIT_H_3_0__

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <cstring>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <regex>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>
#include <utility>
#include <memory>
#include <sstream>
#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <execinfo.h>
#include <time.h>
#include <errno.h>

#include "h2_tool.h"
#include "h2_cfg.h"
#include "h2_alloc.h"
#include "h2_expr.h"
#include "h2_json.h"
#include "h2_bt.h"
#include "h2_fail.h"
#include "h2_leak.h"
#include "h2_stub.h"
#include "h2_mfp.h"
#include "h2_matcher.h"
#include "h2_count.h"
#include "h2_return.h"
#include "h2_mock.h"
#include "h2_suite.h"
#include "h2_case.h"
#include "h2_log.h"
#include "h2_extra.h"
#include "h2_task.h"

#define __H2UNIT_UNIT_TYPE(name) H2_PP_CAT(h2_unit_, name, _type)
#define __H2UNIT_UNIT_INST(name) H2_PP_CAT(h2_unit_, name, _inst)

#define H2UNIT(_suite_)                                                              \
   static struct h2_suite __H2UNIT_UNIT_INST(_suite_)(#_suite_, __FILE__, __LINE__); \
   struct __H2UNIT_UNIT_TYPE(_suite_) : public h2_case

#define __H2UNIT_CASE_TYPE(name) H2_PP_CAT(h2_unit_, name, _case_, __COUNTER__, _, __LINE__, _type)
#define __H2UNIT_CASE_INST(name) H2_PP_CAT(h2_unit_, name, _case_, __COUNTER__, _, __LINE__, _inst)

#define __H2CASE(_suitetype_, _suiteinst_, _casetype_, _casename_, _todo_) \
   namespace {                                                             \
   class _casetype_ : public _suitetype_                                   \
   {                                                                       \
    public:                                                                \
      _casetype_()                                                         \
      {                                                                    \
         _init_(_suiteinst_, _casename_, _todo_, __FILE__, __LINE__);      \
      }                                                                    \
      void testcase();                                                     \
   } __H2UNIT_CASE_INST(any);                                              \
   }                                                                       \
   void _casetype_::testcase()

#define H2CASE(_suite_, ...) \
   __H2CASE(__H2UNIT_UNIT_TYPE(_suite_), &__H2UNIT_UNIT_INST(_suite_), __H2UNIT_CASE_TYPE(_suite_), #__VA_ARGS__, false)

#define H2TODO(_suite_, ...) \
   __H2CASE(__H2UNIT_UNIT_TYPE(_suite_), &__H2UNIT_UNIT_INST(_suite_), __H2UNIT_CASE_TYPE(_suite_), #__VA_ARGS__, true)

#define H2UNIT_CASE(...) \
   __H2CASE(h2_case, h2_suite::A(), __H2UNIT_CASE_TYPE(____), #__VA_ARGS__, false)

#define H2UNIT_TODO(...) \
   __H2CASE(h2_case, h2_suite::A(), __H2UNIT_CASE_TYPE(____), #__VA_ARGS__, true)

#define __H2EQ1(condition)                                       \
   do {                                                          \
      if (!(condition)) {                                        \
         h2_fail* fail = new h2_fail_normal(__FILE__, __LINE__); \
         fail->kprintf("%s is false", #condition);               \
         h2_fail_g(fail);                                        \
      }                                                          \
   } while (0)

#define __H2EQ2(expect, actual)                                                                              \
   do {                                                                                                      \
      auto t__a = actual;                                                                                    \
      typedef typename std::decay<decltype(t__a)>::type actualtype;                                          \
      typedef typename std::conditional<std::is_enum<actualtype>::value, int, actualtype>::type matchertype; \
      h2_matcher<matchertype> t__e(expect);                                                                  \
      h2_fail* fail = t__e.matches(t__a);                                                                    \
      if (fail) {                                                                                            \
         fail->locate(__FILE__, __LINE__, nullptr);                                                                   \
         h2_fail_g(fail);                                                                                    \
      }                                                                                                      \
   } while (0)

#define H2EQ(...) H2_PP_VARIADIC_CALL(__H2EQ, __VA_ARGS__)

#define H2JE(expect, actual) __H2EQ2(Je(expect), actual)

#define __H2STUB2(BeFunc, ToFunc)                                                    \
   do {                                                                              \
      h2_stub_g((void*)BeFunc, (void*)ToFunc, #BeFunc, #ToFunc, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB3(Return, BeFunc, Args)                                                 \
   struct                                                                               \
   {                                                                                    \
      void operator=(Return(*toF) Args)                                                 \
      {                                                                                 \
         h2_stub_g((void*)BeFunc, (void*)(toF), #BeFunc, "lambda", __FILE__, __LINE__); \
      }                                                                                 \
   } H2_PP_CAT2(_stub3_, __LINE__);                                                     \
   H2_PP_CAT2(_stub3_, __LINE__) = [] Args -> Return

#define __H2STUB40(Return, Class, Method, Args)                                                                                                                                                    \
   struct                                                                                                                                                                                          \
   {                                                                                                                                                                                               \
      void operator=(Return (*toF)(Class * that))                                                                                                                                                  \
      {                                                                                                                                                                                            \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, "H2STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                                                                            \
   } H2_PP_CAT2(_stub40_, __LINE__);                                                                                                                                                               \
   H2_PP_CAT2(_stub40_, __LINE__) = [](Class * that) -> Return

#define __H2STUB41(Return, Class, Method, Args)                                                                                                                                                    \
   struct                                                                                                                                                                                          \
   {                                                                                                                                                                                               \
      void operator=(Return (*toF)(Class * that, H2_PP_REMOVE_PARENTHESES(Args)))                                                                                                                  \
      {                                                                                                                                                                                            \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, "H2STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                                                                            \
   } H2_PP_CAT2(_stub41_, __LINE__);                                                                                                                                                               \
   H2_PP_CAT2(_stub41_, __LINE__) = [](Class * that, H2_PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB4(Return, Class, Method, Args) H2_PP_IF(H2_PP_0ARGS Args, __H2STUB40(Return, Class, Method, Args), __H2STUB41(Return, Class, Method, Args))

#define __H2STUB50(Return, Class, Method, Args, Instance)                                                                                     \
   struct                                                                                                                                     \
   {                                                                                                                                          \
      void operator=(Return (*toF)(Class * that))                                                                                             \
      {                                                                                                                                       \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                       \
   } H2_PP_CAT2(_stub50_, __LINE__);                                                                                                          \
   H2_PP_CAT2(_stub50_, __LINE__) = [](Class * that) -> Return

#define __H2STUB51(Return, Class, Method, Args, Instance)                                                                                     \
   struct                                                                                                                                     \
   {                                                                                                                                          \
      void operator=(Return (*toF)(Class * that, H2_PP_REMOVE_PARENTHESES(Args)))                                                             \
      {                                                                                                                                       \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                       \
   } H2_PP_CAT2(_stub51_, __LINE__);                                                                                                          \
   H2_PP_CAT2(_stub51_, __LINE__) = [](Class * that, H2_PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB5(Return, Class, Method, Args, Instance) H2_PP_IF(H2_PP_0ARGS Args, __H2STUB50(Return, Class, Method, Args, Instance), __H2STUB51(Return, Class, Method, Args, Instance))

#define H2STUB(...) H2_PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__)

#define __H2MOCK2(BeFunc, ReturnArgs) \
   h2_mocker<__COUNTER__, __LINE__, std::false_type, ReturnArgs>::I((void*)BeFunc, #BeFunc, __FILE__)

#define __H2MOCK3(Class, Method, ReturnArgs) \
   h2_mocker<__COUNTER__, __LINE__, Class, ReturnArgs>::I(h2_mfp<Class, ReturnArgs>::A(&Class::Method, "H2MOCK", "", #Class, #Method, #ReturnArgs, __FILE__, __LINE__), #Class "::" #Method, __FILE__)

#define __H2MOCK4(Class, Method, ReturnArgs, Instance) \
   h2_mocker<__COUNTER__, __LINE__, Class, ReturnArgs>::I(h2_mfp<Class, ReturnArgs>::A(&Class::Method, Instance), #Class "::" #Method, __FILE__)

#define H2MOCK(...) H2_PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)

#define __H2BLOCK0() \
   for (bool t = _leak_push_(__FILE__, __LINE__, "block"); t; _leak_pop_(), t = false)

#define __H2BLOCK1(...) \
   for (bool t = _leak_push_(__FILE__, __LINE__, "block", __VA_ARGS__); t; _leak_pop_(), t = false)

#define H2BLOCK(...) H2_PP_IF(H2_PP_0ARGS(__VA_ARGS__), __H2BLOCK0(), __H2BLOCK1(__VA_ARGS__))
// #define H2BLOCK(...) for (bool t = _leak_push_(__FILE__, __LINE__, "block", ##__VA_ARGS__); t; _leak_pop_(), t = false)
// #define H2BLOCK(...) for (bool t = _leak_push_(__FILE__, __LINE__, "block" __VA_OPT__(,) __VA_ARGS__); t; _leak_pop_(), t = false)

#if defined(_WIN32)
__declspec(selectany)
#else
__attribute__((weak))
#endif

  int main(int argc, char** argv)
{
   h2_task::I().configure(argc, argv);
   h2_task::I().prepare();
   h2_task::I().run();
   h2_task::I().roundoff();
   return 0;
}

#endif
