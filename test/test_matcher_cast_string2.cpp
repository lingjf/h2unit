#include "../source/h2_unit.cpp"
#include "test_types.hpp"

CASE(cast string2 [re])
{
   STRING_DECL_LIST;

#define TheCheck(x, y) \
   OK(CaseLess(x), y); \
   OK(Substr(x), y);   \
   OK(Re(x), y);

   H2Fullmesh(TheCheck, (STRING_VAR_LIST));
#undef TheCheck
}
