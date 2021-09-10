#include "../source/h2_unit.cpp"
#if !(defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__ || defined NO_CAST_TESTING)

#include "test_types.hpp"
NUMBER0_DECL_LIST;
NUMBER1_DECL_LIST;

#define TheCheck(x) static void foo1##x(decltype(x) v){};
H2Foreach(TheCheck, NUMBER0_VAR_LIST);
#undef TheCheck

SUITE(number[integer, float, enum, bool])
{
   Case(OK1)
   {
#define TheCheck(x) \
   OK(x);           \
   OK(IsTrue, x);   \
   OK(!IsFalse, x);
      H2Foreach(TheCheck, NUMBER1_VAR_LIST);
#undef TheCheck
   }

   Case(OK2)
   {
#define TheCheck(x, y) \
   OK(x, y);           \
   OK(Ge(x) || Gt(x) || Le(x) || Lt(x) || Nq(x) || Eq(x) && Ge(0), y);
      H2Fullmesh(TheCheck, NUMBER1_VAR_LIST);
#undef TheCheck
   }

   Case(MOCK)
   {
#define TheCheck(x)                          \
   MOCK(foo1##x, void(decltype(x))).Once(x); \
   foo1##x(x);
      H2Foreach(TheCheck, NUMBER0_VAR_LIST);
#undef TheCheck
   }
}
#endif
