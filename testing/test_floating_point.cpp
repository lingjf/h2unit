
#include "../h2unit.h"
#include "test.h"

SUITE(fibonacci)
{
   Case(fibonacci 4)
   {
      OK(Eq(1.41), fibonacci(4));
   }

   Case(fibonacci 5)
   {
      OK(Eq(1.41, 1%), fibonacci(5));
   }

   Case(fibonacci 6)
   {
      OK(Eq(1.4142135, 0.000001), fibonacci(6));
   }
}
