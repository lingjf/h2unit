#include "../source/h2_unit.cpp"
#if !(defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__ || defined NO_CAST_TESTING)

#include "test_types.hpp"

PTR_FILL_DECL_LIST;
PTR_NULL_DECL_LIST;

#define TheCheck(x) static void foobar##x(decltype(x) v){};
H2Foreach(TheCheck, PTR_LIST);
#undef TheCheck

SUITE(Pointer)
{
   Case(OK IsNull)
   {
#define TheCheck(x) OK(IsNull, x);
      H2Foreach(TheCheck, PTR_NULL_VALUE_LIST);
#undef TheCheck
   }

   Case(OK NotNull)
   {
#define TheCheck(x) OK(NotNull, x);
      H2Foreach(TheCheck, PTR_FILL_VALUE_LIST);
#undef TheCheck
   }

   Case(OK Me)
   {
#define TheCheck(x, y) OK(Me(x, 1), y);
      H2Fullmesh(TheCheck, PTR_FILL_VALUE_LIST);
#undef TheCheck
   }

   Case(OK AllOf, AnyOf, NoneOf)
   {
#define TheCheck(x, y)      \
   OK(AllOf(_, IsNull), y); \
   OK(AnyOf(_, IsNull), y); \
   OK(!!NoneOf(NotNull), y);
      H2Fullmesh(TheCheck, PTR_NULL_VALUE_LIST);
#undef TheCheck
   }

   Case(MOCK)
   {
#define TheCheck(x, y)                              \
   MOCK(foobar##x, void(decltype(x))).Once(IsNull); \
   foobar##x((decltype(x))y);
      H2Fullmesh(TheCheck, (PTR_LIST), (PTR_NULL_VALUE_LIST));
#undef TheCheck
   }
}
#endif
