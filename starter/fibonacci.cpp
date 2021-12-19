
#include "fibonacci.h"

int fibonacci(int n)
{
   if (n <= 1) return 1;
   return fibonacci(n - 1) + fibonacci(n - 2);
}

int fibonacci_non_recurcive(int n)
{
   int current = 1;
   int previous = 1;
   for (int i = 0; i + 1 < n; ++i) {
      int temp = current;
      current = current + previous;
      previous = temp;
   }
   return current;
}
