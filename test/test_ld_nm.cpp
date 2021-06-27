#include "../source/h2_unit.cpp"

SUITE(nm)
{
   Case(get_by_name)
   {
      h2::h2_symbol* res[16];
      int n = h2::h2_nm::get_by_name("time", res, 16);
      OK(1, n);
      OK(NotNull, res[0]->offset);
      // OK(Gt(0), res[0]->size);
   }
}
