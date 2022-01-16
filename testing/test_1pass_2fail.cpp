
#include "../h2unit.h"
#include "test.h"

SUITE(fibonacci)
{
   Case(fibonacci 5)
   {
      OK(8, fibonacci(5));
   }

   Case(fibonacci 6)
   {
      OK(11, fibonacci(6));
   }

   Case(fibonacci 7)
   {
      OK(23, fibonacci(7));
   }
}
