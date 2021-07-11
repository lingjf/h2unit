#include "../source/h2_unit.cpp"
#include "test_types.hpp"
#include <time.h>

static time_t STUB_time(time_t* x)
{
   return 42;
}

SUITE(stub e9)
{
   Case(libc time)
   {
      OK(Nq(42), time(0));
      unsigned char saved[16];
      bool ret = h2::h2_e9_save((void*)time, saved);
      OK(ret);
      h2::h2_e9_set((void*)time, (void*)STUB_time);
      OK(42, time(0));
      h2::h2_e9_reset((void*)time, saved);
   }

   Case(normal foobar1)
   {
      OK(1, foobar1(0));
      unsigned char saved[16];
      bool ret = h2::h2_e9_save((void*)foobar1, saved);
      OK(ret);
      h2::h2_e9_set((void*)foobar1, (void*)foobar1_fake);
      OK(-1, foobar1(0));
      h2::h2_e9_reset((void*)foobar1, saved);
   }

   Case(variadic parameters function)
   {
      OK(6, foobar6("", 1, 2, 3));
      unsigned char saved[16];
      bool ret = h2::h2_e9_save((void*)foobar6, saved);
      OK(ret);
      h2::h2_e9_set((void*)foobar6, (void*)foobar6_fake);
      OK(-6, foobar6("", 1, 2, 3));
      h2::h2_e9_reset((void*)foobar6, saved);
   }
}
