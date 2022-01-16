
#include "../h2unit.h"
#include "test.h"

SUITE(fibonacci)
{
   Case(fibonacci 5)
   {
      OK(7, fibonacci(5));
   }

   Case(fibonacci 5)
   {
      OK(8, fibonacci(5));
   }

   Todo(fibonacci 5)
   {
      OK(7, fibonacci(5));
   }

   Todos(fibonacci, 1, 2, 3)
   {
      OK(7, fibonacci(5));
   }
}

TODO(fibonacci)
{
   OK(7, fibonacci(5));
}

TODOS(fibonacci, 1, 2, 3)
{
   OK(7, fibonacci(5));
}
