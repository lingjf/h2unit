#include "../source/h2_unit.cpp"

SUITE(bugfix)
{
   Case(Eq integer with float)
   {
      float uv = 2.00001;
      OK(Eq(2, 0.0001), uv);
   }
}
