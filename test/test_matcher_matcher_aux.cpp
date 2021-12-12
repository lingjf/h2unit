#include "../source/h2_unit.cpp"

SUITE(IsEven IsOdd primitive [api])
{
   Case(zero)
   {
      OK(IsEven(), 0);
      OK(!IsOdd(), 0);
   }

   Case(positive integer)
   {
      OK(!IsEven(), 1);
      OK(IsOdd(), 1);
      OK(IsEven(), 2);
      OK(!IsOdd(), 2);
      OK(!IsEven(), 3);
      OK(IsOdd(), 3);
      OK(IsEven(), 4);
      OK(!IsOdd(), 4);
   }

   Case(negtive integer)
   {
      OK(!IsEven(), -1);
      OK(IsOdd(), -1);
      OK(IsEven(), -2);
      OK(!IsOdd(), -2);
   }

   // Case(double)
   // {
   //    OK(!IsEven(), 1.2);
   // }
}

SUITE(IsDivable primitive [api])
{
   Case(positive integer)
   {
      OK(IsDivable(2), 4);
      OK(!IsDivable(3), 4);
   }

   Case(negtive integer)
   {
      OK(IsDivable(2), -4);
      OK(!IsDivable(3), -4);
      OK(IsDivable(-2), -4);
      OK(!IsDivable(-3), -4);
   }

   // Case(double)
   // {
   //    OK(!IsEven(), 1.2);
   // }
}

SUITE(IsInteger primitive [api])
{
   Case(positive integer)
   {
      OK(IsInteger(), 4);
      OK(!IsInteger(), 4.2);
   }

   Case(negtive integer)
   {
      OK(IsInteger(), -4);
      OK(!IsInteger(), -4.2);
   }
}
