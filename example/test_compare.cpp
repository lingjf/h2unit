#include "h2unit.h"

extern "C"
{
#include "product_c.h"
}

SUITE(Number compares)
{
  Case(Calculate rectangle area successful)
  {
    rectangle_t p1 = { 2, 3 };

    OK(6, rectangle_area(&p1)); // successful
  }

  Case(get rectangle diag length failure)
  {
    rectangle_t p1 = { 3, 4 };
    OK(5, rectangle_diag(&p1)); // successful
    rectangle_t p2 = { 1, 1 };
    OK(1.41, rectangle_diag(&p2))
      << "float precision problem " << 1.41421356237; // failure
  }

   Case(test for great than failure)
   {
     rectangle_t p1 = { 2, 3 };

     OK(Gt(5), rectangle_area(&p1));  // successful
     OK(!Gt(5), rectangle_area(&p1)); // failure
   }
}

SUITE(String compares)
{
  /*
   * OK can be used to verify Case sensitive String.
   */
  Case(string equal failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK("Rect(0, 0)", rectangle_tostring(&p1)); // successful
    rectangle_t p2 = { 1, 2 };
    OK("Rect(0, 0)", rectangle_tostring(&p2)); // failure
  }

  Case(demo Contains failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(Contains("(0, 0)"), rectangle_tostring(&p1)); // successful
    rectangle_t p2 = { 1, 2 };
    OK(Contains("(1, 1)"), rectangle_tostring(&p2)); // failure
  }

  Case(demo StartsWith failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(StartsWith("Rect"), rectangle_tostring(&p1)); // successful
    OK(StartsWith("Pect"), rectangle_tostring(&p1)); // failure
  }

  Case(demo EndsWith failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(EndsWith("(0, 0)"), rectangle_tostring(&p1)); // successful
    OK(EndsWith("(1, 1)"), rectangle_tostring(&p1)); // failure
  }

  /*
   * h2unit WILDCARD/We can be used to verify string by WildCard. support:
   *  -- ? any one char
   *  -- * any char(s)
   */
  Case(wildcard string failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(We("Rect(0, ?)"), rectangle_tostring(&p1)); // successful
    rectangle_t p2 = { 1, 2 };
    OK(We("Rect(*]"), rectangle_tostring(&p2)); // failure
  }

  /*
   * h2unit REGEX/Re can be used to verify string by Regular express.
   * http://www.cplusplus.com/reference/regex/ECMAScript/
   */
  Case(regex string failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(Re("Rect\\(0, 0\\)"), rectangle_tostring(&p1)); // successful
    rectangle_t p2 = { 1, 2 };
    OK(Re("Rect\\(.*"), rectangle_tostring(&p2)); // successful
    rectangle_t p3 = { 3, 4 };
    OK(Re("Rect\\(.*\\)"), rectangle_tostring(&p3)); // successful
    rectangle_t p4 = { 5, 6 };
    OK(Re("^Rect\\(.*\\)$"), rectangle_tostring(&p4)); // successful
    rectangle_t p5 = { 7, 8 };
    OK(Re("Rect\\([789], [0-9]\\)"), rectangle_tostring(&p5)); // successful
    rectangle_t p6 = { 11, 88 };
    OK(Re("Rect\\([0-9]*, [0-9]*\\)"), rectangle_tostring(&p6)); // successful
    rectangle_t p7 = { 11, 88 };
    OK(Re("Rect\\([0-9]+, [^6-9]+\\)"), rectangle_tostring(&p7)); // failure
  }

  /*
   * Case-Less string compare
   */

  Case(caseless string equal failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(CaseLess("rect(0, 0)"), rectangle_tostring(&p1)); // successful
    rectangle_t p2 = { 1, 2 };
    OK(CaseLess("RECT(1, 1)"), rectangle_tostring(&p2)); // failure
  }

  Case(caseless starts / end with failure)
  {
    rectangle_t p1 = { 0, 0 };
    OK(CaseLess(StartsWith("Rect(0, 0)")),
       rectangle_tostring(&p1)); // successful
    rectangle_t p2 = { 1, 2 };
    OK(CaseLess(EndsWith("R(1, 2)")), rectangle_tostring(&p2)); // failure
  }
}

CASE(string compares failure)
{
  OK("a7af3452a5eee24a149f899\r3e48ce81b77b200b212b23c2402c221ddc4260\na608cf32"
     "8a2133386477eb7a65958893726741cb9223419a7a0224fe7af2866944197ee4ab18e2591"
     "3cae8507bb306a284f",
     "a7af3452a5eeX24a149f899\t3e48ce81b77b200b212b23c2402c2Y1ddc4260\na608cf32"
     "8a2133386477eb7a6595889372674Zcb9223419a7a0224fe7af2866944197ee4ab18e2591"
     "3cae8507bb306a284f");
}
/*
 * h2unit Me (Memory equal) can be used to verify memory block.
 */
CASE(memory compares failure)
{
  const unsigned char e00[] = "abcdefghijklmnopqrstuvwxyz";
  OK(Me(e00, sizeof(e00)), "abcdefghijklmnopqrstuvwxyz"); // successful
  OK(Me(e00, sizeof(e00)), "abcdEfghijklmnopqrsTuvwxyz"); // failure
}

/*
 * h2unit Pointee can be used to verify the data pointer point to.
 */
CASE(point to compares failure)
{
  int x = 1;
  OK(Pointee(2), &x); // failure
}

SUITE(zero operator)
{
  Case(Any)
  {
    OK(_, 0);
    OK(_, 1);
    OK(Any, 0);
    OK(Any, 1);
  }

  Case(IsNull failure)
  {
    int x;
    OK(IsNull, nullptr); // successful
    OK(IsNull, &x);      // failure
  }

  Case(NotNull failure)
  {
    int x;
    OK(NotNull, &x);      // successful
    OK(NotNull, nullptr); // failure
  }

  Case(IsTrue failure)
  {
    OK(IsTrue, true);  // successful
    OK(IsTrue, false); // failure
  }

  Case(IsFalse failure)
  {
    OK(IsFalse, false); // successful
    OK(IsFalse, true);  // failure
  }
}
