#include "../source/h2_unit.cpp"

#include "test_types.hpp"

CASE(cast string2)
{
   STRING_DECL_LIST;

#define TheCheck(x, y) \
   OK(AnyOf(x), y);    \
   OK(AllOf(_, x), y); \
   OK(Not(!NoneOf(Nq(x))), y);

   H2Fullmesh(TheCheck, (STRING_VAR_LIST));
#undef TheCheck
}
