#include "../source/h2_unit.cpp"
#include "test_types.hpp"

PTR_FILL_DECL_LIST;
PTR_NULL_DECL_LIST;

SUITE(cast ptr1)
{
   Case(OK Is Null)
   {
#define TheCheck(x) OK(nullptr, x);
      H2Foreach(TheCheck, (PTR_NULL_VALUE_LIST));
#undef TheCheck
   }

   Case(OK Not Null)
   {
#define TheCheck(x) OK(Not(NULL), x);
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
#define TheCheck(x, y)              \
   OK(AllOf(_, NULL), y);           \
   OK(AnyOf(_, nullptr), y);        \
   OK(!!NoneOf(Not(nullptr)), y);

      H2Fullmesh(TheCheck, (PTR_NULL_VALUE_LIST));
#undef TheCheck
   }
}
