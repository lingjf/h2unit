#include "../source/h2_unit.cpp"

#include "test_types.hpp"

PTR_FILL_DECL_LIST;
PTR_NULL_DECL_LIST;

SUITE(cast ptr1)
{
   Case(OK IsNull)
   {
#define TheCheck(x) OK(IsNull, x);
      H2Foreach(TheCheck, (PTR_NULL_VALUE_LIST));
#undef TheCheck
   }

   Case(OK NotNull)
   {
#define TheCheck(x) OK(NotNull, x);
      H2Foreach(TheCheck, (PTR_FILL_VALUE_LIST));
#undef TheCheck
   }

   Case(OK Me)
   {
#define TheCheck(x, y) OK(Me(x, 1), y);
      H2Fullmesh(TheCheck, (PTR_FILL_VALUE_LIST));
#undef TheCheck
   }

   Case(OK AllOf, AnyOf, NoneOf)
   {
#define TheCheck(x, y)      \
   OK(AllOf(_, IsNull), y); \
   OK(AnyOf(_, IsNull), y); \
   OK(!!NoneOf(NotNull), y);

      H2Fullmesh(TheCheck, (PTR_NULL_VALUE_LIST));
#undef TheCheck
   }
}
