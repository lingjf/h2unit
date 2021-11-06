
#if defined _WIN32 || defined __CYGWIN__
#define H2UNIT_IMPORT_MAIN
#endif

#include "../h2unit.h"

/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

int pv;

/*
 * SUITE is a unit test Suite, which Substr several test cases.
 */
SUITE(Basic Cases)
{
   /*
    * This variable is accessible in the test cases belong this unit.
    */
   int uv = 2;

   /*
    * Cleanup() will be executed after every test case belong to this suite.
    * whatever test case passed or failed.
    * Typically it is used to release resource.
    */
   Cleanup()
   {
      pv = 0;
      uv = 0;
   }

   /*
    * Case is a unit test case
    */
   Case(hello world successful)
   {
      OK(2, sqrt(4));
   }

   Case(a failure case failure)
   {
      OK(1, pv);
   }

   Case(a success case successful)
   {
      OK(2, uv);
   }

   Case(a c++ case successful)
   {
      Rect rect(0, 0, 1, 1);
      OK(0, rect.move(0));
   }
}

SUITE(Second Demo)
{
   Case(0 is equal with - 0 failure)
   {
      bool result = 0 != -0;
      OK(result) << "0 != -0";
   }

   Todo(alive for ever)
   {
      OK(false);
   }
}

/*
 * CASE act SUITE plus Case, without setup and cleanup.
 */

CASE(standalone case failure)
{
   bool result = true;
   OK(!result);
}

TODO(alive for ever)
{
   OK(false);
}

CASE(performance)
{
   PF(11)
   {
      double a = 1.0;
      for (int i = 0; i < 100000000; i++) {
         a = a / 3.0;
      }
   }
}
