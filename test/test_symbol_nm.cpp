#include "../source/h2_unit.cpp"
#include "test_types.hpp"

extern "C" {
int _main() { return 0; }
}

SUITE(nm)
{
   Case(get_by_name(main))
   {
      h2::h2_symbol* res[16];
      int n = h2::h2_nm::get_by_name("main", res, 16);
      OK(1, n);
      OK(Not(NULL), res[0]->addr);

#if defined __APPLE__
      OK(h2::h2_nm::I().leading_underscore);
#else
      OK(!h2::h2_nm::I().leading_underscore);
#endif
   }

   Case(get_by_name(foobar0))
   {
      h2::h2_symbol* res[16];
      int n = h2::h2_nm::get_by_name("foobar0", res, 16);
      OK(1, n);
      OK(Not(NULL), res[0]->addr);
   }
}
