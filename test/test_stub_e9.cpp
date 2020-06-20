#include "../source/h2_unit.cpp"
#include <time.h>

namespace {

time_t STUB_time(time_t* x)
{
   return 42;
}

int foobar1(int a, int b)
{
   return 1;
}
int STUB_foobar1(int a, int b)
{
   return -1;
}

int foobar2(char* a, ...)
{
   return 2;
}
int STUB_foobar2(char* a, ...)
{
   return -2;
}

SUITE(stub e9)
{
   Case(libc time)
   {
      OK(Nq(42), time(0));
      unsigned char saved[16];
      bool ret = h2::h2_e9::save((void*)time, saved);
      OK(ret);
      h2::h2_e9::set((void*)time, (void*)STUB_time);
      OK(42, time(0));
      h2::h2_e9::reset((void*)time, saved);
   }

   Case(normal foobar1)
   {
      OK(1, foobar1(0, 0));
      unsigned char saved[16];
      bool ret = h2::h2_e9::save((void*)foobar1, saved);
      OK(ret);
      h2::h2_e9::set((void*)foobar1, (void*)STUB_foobar1);
      OK(-1, foobar1(0, 0));
      h2::h2_e9::reset((void*)foobar1, saved);
   }

   Case(variadic parameters function foobar2)
   {
      OK(2, foobar2("", 1, 2, 3));
      unsigned char saved[16];
      bool ret = h2::h2_e9::save((void*)foobar2, saved);
      OK(ret);
      h2::h2_e9::set((void*)foobar2, (void*)STUB_foobar2);
      OK(-2, foobar2("", 1, 2, 3));
      h2::h2_e9::reset((void*)foobar2, saved);
   }
}

}  // namespace
