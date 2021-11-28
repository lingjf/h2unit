#include "../source/h2_unit.cpp"
#include "test_types.hpp"

NUMBER0_DECL_LIST;
NUMBER1_DECL_LIST;

#define TheCheck(x) static void foo1##x(decltype(x) v){};
H2Foreach(TheCheck, (NUMBER0_VAR_LIST));
#undef TheCheck

CASE(cast number5)
{
#define TheCheck(x)                          \
   MOCK(foo1##x, void(decltype(x))).Once(x); \
   foo1##x(x);

   H2Foreach(TheCheck, (NUMBER0_VAR_LIST));
#undef TheCheck
}
