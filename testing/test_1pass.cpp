
#include "../h2unit.h"
#include "test.h"

SUITE(fibonacci)
{
   Case(fibonacci 5)
   {
      OK(8, fibonacci(5));
   }
}