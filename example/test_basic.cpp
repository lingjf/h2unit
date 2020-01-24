#include "h2unit.h"
#include <stdlib.h>

/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "product_c.h"
}

int gv;

/*
 * SUITE is a unit test Suite, which contains several test cases.
 */
SUITE(Basic Cases)
{
   /*
    * This variable is accessible in the test cases belong this unit.
    */
   int uv;

   /*
    * Setup() will be executed before every test case belong to this suite.
    * Typically it is used to prepare preconditions.
    */
   Setup()
   {
      uv = 2;
   };
   /*
    * Teardown() will be executed after every test case belong to this suite.
    * whatever test case passed or failed.
    * Typically it is used to release resource.
    */
   Teardown()
   {
      gv = 0;
   };

   /*
    * Case is a unit test case
    */
   Case(hello world-- successful)
   {
      OK(2, sqrt(4));
   };

   Case(a failure case --failure)
   {
      OK(1, gv);
   };

   Case(a success case --successful)
   {
      OK(3 == uv + 1);
   };
}

SUITE(Second Demo)
{
   /*
    * Setup() and Teardown() can be omitted. The default is doing nothing
    */

   Case(0 is equal with - 0 --successful)
   {
      bool result = 0 != -0;
      OK(result);
   };

   Todo(alive for ever)
   {
      OK(false);
   };
}

/*
 * If no setup() and teardown(), SUITE can be omitted.
 * TEST act SUITE plus CASE.
 */

CASE(standalone case --failure)
{
   bool result = true;
   OK(!result);
}

TODO(alive for ever)
{
   OK(false);
}
