#include "../source/h2_unit.cpp"
#include "test_types.hpp"

NUMBER0_DECL_LIST;
NUMBER1_DECL_LIST;

CASE(cast number1)
{
#define TheCheck(x) \
   OK(x);           \
   OK(true, x);

   H2Foreach(TheCheck, (NUMBER1_VAR_LIST));
#undef TheCheck
}
