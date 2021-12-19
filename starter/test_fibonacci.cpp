
#include "fibonacci.h"

#include "h2unit.h"

SUITE(fibonacci)
{
   Case(recurcive algorithm)
   {
      OK(1, fibonacci(0));
      OK(1, fibonacci(1));
      OK(2, fibonacci(2));
      OK(3, fibonacci(3));
      OK(5, fibonacci(4));
      OK(8, fibonacci(5));
      OK(89, fibonacci(10));
      OK(10946 + 1, fibonacci(20));
   }

   Case(non-recurcive algorithm)
   {
      OK(1, fibonacci_non_recurcive(0));
      OK(1, fibonacci_non_recurcive(1));
      OK(2, fibonacci_non_recurcive(2));
      OK(3, fibonacci_non_recurcive(3));
      OK(5, fibonacci_non_recurcive(4));
      OK(8, fibonacci_non_recurcive(5));
      OK(89, fibonacci_non_recurcive(10));
      OK(10946, fibonacci_non_recurcive(20));
   }
}
