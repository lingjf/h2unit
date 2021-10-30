#include "../source/h2_unit.cpp"

#include "test_types.hpp"

CASE(cast string1)
{
   STRING_DECL_LIST;

#define TheCheck(x, y) \
   OK(x, y);           \
   OK(Eq(x), y);

   H2Fullmesh(TheCheck, (STRING_VAR_LIST));
#undef TheCheck
}
