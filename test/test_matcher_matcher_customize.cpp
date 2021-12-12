#include "../source/h2_unit.cpp"

H2MATCHER(Is_Even, ("is not Even"))
{
   return a % 2 == 0;
}
H2MATCHER(Is_Even2)
{
   return a % 2 == 0;
}
H2MATCHER(Is_Even3, (""))
{
   return a % 2 == 0;
}

H2MATCHER(Is_Divable, expect, ("not divable"))
{
   return a % expect == 0;
}

H2MATCHER(Is_Divable2, expect)
{
   return a % expect == 0;
}

H2MATCHER(Is_Between, left, right, (a << " not in [" << left << ", " << right << "]"))
{
   return left <= a && a <= right;
}

H2MATCHER(In_Range, start, stop, step, ("not in range"))
{
   for (int i = start; i < stop; i += step)
      if (i == a)
         return true;
   return false;
}

H2MATCHER(In_Rect, left, top, right, bottom, ("not in rect"))
{
   return true;
}

H2MATCHER(In_Protocol, ip1, port1, ip2, port2, proto, ("not in proto"))
{
   return true;
}

SUITE(User Defined Matcher)
{
   Case(Is_Even)
   {
      OK(Is_Even(), 2);
      OK(Is_Even2(), 2);
      OK(Is_Even3(), 2);
      OK(!Is_Even(), 1);
      OK(!Is_Even2(), 1);
      OK(!Is_Even3(), 1);
      OK(Is_Even(), 4);
      OK(Is_Even2(), 4);
      OK(Is_Even3(), 4);
      OK(!Is_Even(), 5);
      OK(!Is_Even2(), 5);
      OK(!Is_Even3(), 5);
   }

   Case(Is_Divable)
   {
      OK(Is_Divable(3), 6);
      OK(Is_Divable2(3), 6);
      OK(!Is_Divable(3), 7);
      OK(!Is_Divable2(3), 7);
   }

   Case(Is_Between)
   {
      OK(Is_Between(1, 4), 2);
      OK(!Is_Between(1, 4), 5);
   }

   Case(In_Range)
   {
      OK(In_Range(1, 10, 2), 5);
      OK(!In_Range(1, 10, 2), 6);
   }

   Case(In_Rect)
   {
      OK(In_Rect(1, 10, 2, 7), 0);
   }

   Case(In_Protocol)
   {
      OK(In_Protocol(1, 10, 2, 7, 9), 0);
   }
}
