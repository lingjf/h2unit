#include "../h2unit.h"

extern "C" {
#include "product_c.h"
}

SUITE(Logic)
{
   Case(Not successful)
   {
      rectangle_t p1 = {2, 3};
      OK(Not(5), rectangle_diag(&p1));  // successful
   }

   Case(Not Not failure)
   {
      OK(!Not(5), 6);
   }

   Case(&& || !failure)
   {
      OK(Ge(0) || -1 && Eq(1), 1);  // successful
      OK(!(Ge(0) || -1), -2);       // successful

      OK(Ge(0) && Eq(-2), -1);  // failure
   }

   Case(AllOf failure)
   {
      rectangle_t p1 = {0, 0};
      OK(AllOf("Rect(0, 0)", StartsWith("Rect")), rectangle_tostring(&p1));  // successful

      rectangle_t p2 = {1, 2};
      OK(AllOf("Rect(0, 0)", _, EndsWith("R(1, 2)")), rectangle_tostring(&p2));  // failure
   }

   Case(AnyOf successful)
   {
      rectangle_t p1 = {0, 0};
      OK(AnyOf("Rect(0, 0)", StartsWith("Rect")), rectangle_tostring(&p1));  // successful
      OK(AnyOf("Rect(0, 1)", StartsWith("Rect")), rectangle_tostring(&p1));  // successful
   }

   Case(NoneOf successful)
   {
      rectangle_t p1 = {0, 0};
      OK(NoneOf("Rect(0, 1)", EndsWith("R(1, 2)")), rectangle_tostring(&p1));  // successful
   }
}
