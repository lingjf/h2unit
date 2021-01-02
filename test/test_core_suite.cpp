#include "../source/h2_unit.cpp"

static int value_a = 0;
static int value_b = 0;

SUITE(suite1)
{
   Setup()
   {
      value_a = 1;
   }

   Cleanup()
   {
      value_b = 0;
   }

   Case(test)
   {
      OK(1, value_a);
      OK(0, value_b);

      value_b = 1;
   }
}

SUITE(suite2)
{
   Cleanup()
   {
      value_b = 0;
   }

   Setup()
   {
      value_a = 1;
   }

   Case(test)
   {
      OK(1, value_a);
      OK(0, value_b);

      value_b = 1;
   }
}
