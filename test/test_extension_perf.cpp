#include "../source/h2_unit.cpp"

CASE(performance)
{
   PF(11)
   {
      int a = 1;
      for (int i = 0; i < 1000; i++) {
         ++a;
      }
   }
}
