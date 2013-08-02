#include "h2unit.h"
#include <stdlib.h>

/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "demo_clanguage.h"
}

int gv;

/*
 * H2UNIT is a unit test Suite, which contains several test cases.
 */
H2UNIT(hello_unit)
{
   /*
    * This variable is accessible in the test cases belong this unit.
    */
   int uv;

   /*
    * setup() will be executed before every test case belong to this suite.
    * Typically it is used to prepare preconditions.
    */
   void setup()
   {
      uv = 2;
   }
   /*
    * teardown() will be executed after every test case belong to this suite.
    * whatever test case passed or failed.
    * Typically it is used to release resource.
    */
   void teardown()
   {
      gv = 0;
   }

   /*
    * This function is also accessible in the test cases belong this unit.
    */

   int plusOne()
   {
      return uv + 1;
   }
};

/*
 * H2CASE is a unit test case
 */
H2CASE(hello_unit, "hello case")
{
   H2EQ_TRUE(2 == uv);
}

H2CASE(hello_unit, "a failure case")
{
   H2EQ_TRUE(1 == gv);
}

H2CASE(hello_unit, "a success case")
{
   H2EQ_TRUE(3 == plusOne());
}



H2UNIT(the_2nd_unit)
{
  /*
   * setup() and teardown() can be omitted. The default is doing nothing
   */
};

H2CASE(the_2nd_unit, "0 is equal with -0")
{
   bool result = 0 != -0;
   H2EQ_TRUE(!result);
}

H2TODO(the_2nd_unit, "alive for ever")
{
   H2EQ_TRUE(false);
}

/*
 * If no setup() and teardown(), H2UNIT can be omitted.
 * H2UNIT_CASE act H2UNIT plus H2CASE.
 */

H2UNIT_CASE("0 is equal with -0")
{
   bool result = 0 != -0;
   H2EQ_TRUE(!result);
}

H2UNIT_TODO("alive for ever")
{
   H2EQ_TRUE(false);
}
