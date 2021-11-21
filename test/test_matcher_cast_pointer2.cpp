#include "../source/h2_unit.cpp"
#include "test_types.hpp"

PTR_FILL_DECL_LIST;
PTR_NULL_DECL_LIST;

#define TheCheck(x) static void foobar##x(decltype(x) v){};
H2Foreach(TheCheck, (PTR_LIST));
#undef TheCheck

SUITE(cast ptr2)
{
   Case(MOCK)
   {
#define TheCheck(x, y)                              \
   MOCK(foobar##x, void(decltype(x))).Once(IsNull); \
   foobar##x((decltype(x))y);

      H2Fullmesh(TheCheck, (PTR_LIST), (PTR_NULL_VALUE_LIST));
#undef TheCheck
   }
}
