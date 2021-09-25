
#define IMPORT_MAIN

#if H2UNIT == 1
#include "../h2unit.h"
#endif

#if H2UNIT == 2
#include "../build/h2unit.hpp"
#endif

SUITE(1v2)
{
   Cleanup()
   {
   }

   Case(1)
   {
   }
   Todo(2)
   {
   }
}
