#include "../source/h2_unit.cpp"

SUITE(assert utility)
{
   Case(find_outer_op)
   {
      OK("< b", h2::find_outer_op("a < b", "<"));
      OK("<=b", h2::find_outer_op("\"a\" <=b", "<="));
      OK(">b", h2::find_outer_op("\"a,\" >b", ">"));
      OK(">=b", h2::find_outer_op("a\\\">=b", ">="));
   }
}
