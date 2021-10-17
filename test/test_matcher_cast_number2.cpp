#include "../source/h2_unit.cpp"

#include "test_types.hpp"

CASE(cast number2)
{
   NUMBER0_DECL_LIST;
   NUMBER1_DECL_LIST;

#define TheCheck(x, y) OK(x, y);
   H2Fullmesh(TheCheck, NUMBER1_VAR_LIST);
#undef TheCheck
}
