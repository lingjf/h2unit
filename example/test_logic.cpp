
extern "C"
{
#include "product_c.h"
}

SUITE(Logic)
{
  Case(Not successful)
  {
    rectangle_t p1 = { 2, 3 };
    OK(Not(5), rectangle_diag(&p1)); // successful
  }

  Case(Not string equal failure)
  {
    const char* p = "a7af3452a5eee24a149f8993e48ce81b77b200b212b23c2402c221ddc4"
                    "260a608cf328a2133386477eb7a65958893726741cb9223419a7a0224f"
                    "e7af2866944197ee4ab18e25913cae8507bb306a284f";
    OK(Not(p), p);
  }

  Case(Not Not failure) { OK(!Not(5), 6); }

  Case(&& || !)
  {
    OK(Ge(0) || -1 && Eq(1), 1);
    OK(!(Ge(0) || -1), -2);
  }

  Case(AllOf failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(AllOf("Rect(0, 0)", StartsWith("Rect")),
       rectangle_tostring(&p1)); // successful

    rectangle_t p2 = { 1, 2 };
    OK(AllOf("Rect(0, 0)", _, EndsWith("R(1, 2)")),
       rectangle_tostring(&p2)); // failure
  }

  Case(AnyOf failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(AnyOf("Rect(0, 0)", StartsWith("Rect")),
       rectangle_tostring(&p1)); // successful
    OK(AnyOf("Rect(0, 1)", StartsWith("Rect")),
       rectangle_tostring(&p1)); // successful

    rectangle_t p2 = { 1, 2 };
    OK(AnyOf("Rect(0, 0)", EndsWith("R(1, 2)")),
       rectangle_tostring(&p2)); // failure
  }

  Case(NoneOf failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(NoneOf("Rect(0, 1)", EndsWith("R(1, 2)")),
       rectangle_tostring(&p1)); // successful

    OK(NoneOf("Rect(0, 1)", StartsWith("Rect")),
       rectangle_tostring(&p1)); // failure
  }
}
