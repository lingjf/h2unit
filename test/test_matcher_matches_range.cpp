#include "../source/h2_unit.cpp"

SUITE(range matches)
{
   Case(start, end, step)
   {
      h2::h2_range_matches a(0, 9, 1);
      for (int i = 0; i < 9; i++) {
         OK(nullptr == a.matches(0, {}));
      }
      OK(nullptr != a.matches(9, {}));
      OK(nullptr != a.matches(10, {}));
   }

   Case(start, end)
   {
      h2::h2_range_matches a(0, 9);
      for (int i = 0; i < 9; i++) {
         OK(nullptr == a.matches(0, {}));
      }
      OK(nullptr != a.matches(9, {}));
      OK(nullptr != a.matches(10, {}));
   }

   Case(start)
   {
      h2::h2_range_matches a(9);
      for (int i = 0; i < 9; i++) {
         OK(nullptr == a.matches(0, {}));
      }
      OK(nullptr != a.matches(9, {}));
      OK(nullptr != a.matches(10, {}));
   }

   Case(start, end, step=2)
   {
      h2::h2_range_matches a(0, 9, 2);
      OK(nullptr == a.matches(0, {}));
      OK(nullptr != a.matches(1, {}));
      OK(nullptr == a.matches(2, {}));
      OK(nullptr != a.matches(3, {}));
      OK(nullptr == a.matches(4, {}));
      OK(nullptr != a.matches(5, {}));
      OK(nullptr == a.matches(6, {}));
      OK(nullptr != a.matches(7, {}));
      OK(nullptr == a.matches(8, {}));
      OK(nullptr != a.matches(9, {}));
      OK(nullptr != a.matches(10, {}));
   }
}

SUITE(Range primitive [api])
{
   Case(start, end, step)
   {
      for (int i = 0; i < 9; i++) {
         OK(Range(0, 9, 1), i);
      }
      OK(!Range(0, 9, 1), 9);
      OK(!Range(0, 9, 1), 10);
   }

   Case(start, end)
   {
      for (int i = 0; i < 9; i++) {
         OK(Range(0, 9), i);
      }
      OK(!Range(0, 9), 9);
      OK(!Range(0, 9), 10);
   }

   Case(start)
   {
      for (int i = 0; i < 9; i++) {
         OK(Range(9), i);
      }
      OK(!Range(9), 9);
      OK(!Range(9), 10);
   }

   Case(start, end, step=2)
   {
      OK(Range(0, 9, 2), 0);
      OK(!Range(0, 9, 2), 1);
      OK(Range(0, 9, 2), 2);
      OK(!Range(0, 9, 2), 3);
      OK(Range(0, 9, 2), 4);
      OK(!Range(0, 9, 2), 5);
      OK(Range(0, 9, 2), 6);
      OK(!Range(0, 9, 2), 7);
      OK(Range(0, 9, 2), 8);
      OK(!Range(0, 9, 2), 9);
      OK(!Range(0, 9, 2), 10);
   }
}
