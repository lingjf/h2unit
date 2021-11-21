#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(nm)
{
   Case(get_by_name)
   {
      h2::h2_symbol* res[16];
      int n = h2::h2_nm::get_by_name("foobar0", res, 16);
      OK(1, n);
      OK(Not(NULL), res[0]->addr);
   }
}
