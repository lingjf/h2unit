#include "../source/h2_unit.cpp"
#include "libs/lib_dynamic.h"

static int add_fake(int a, int b)
{
   return 0;
}

SUITE(lib_dynamic)
{
   void* p;

   Case(plus)
   {
      OK(2, lib_dynamic_plus(1));
   }

   Case(stub)
   {
      STUB(lib_dynamic_add, add_fake);
      OK(0, lib_dynamic_add(1, 2));

#if !(defined NDEBUG && defined __APPLE__)  //TODO: macos release failed
#   if !(defined NDEBUG && defined _WIN32)  //TODO: msvc release failed
      OK(0, lib_dynamic_plus(1)) << "LD_BIND_NOW=1 ./a.out";
#   endif
#endif
   }

   Case(create)
   {
      p = lib_dynamic_create(100);
      lib_dynamic_destroy(p);
   }
}
