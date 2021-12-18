#include "../source/h2_unit.cpp"

SUITE(range util)
{
   Case(Range<>)
   {
      int start = h2::h2_range<5413722, -1>::start;
      int end = h2::h2_range<5413722, -1>::end;
      OK(0, start);
      OK(5413722, end);

      bool left = h2::h2_range<5413722, -1>::in(-8);
      OK(false, left);

      bool inside = h2::h2_range<5413722, -1>::in(100);
      OK(true, inside);
   }

   Case(Range<3>)
   {
      int start = h2::h2_range<3, -1>::start;
      int end = h2::h2_range<3, -1>::end;
      OK(0, start);
      OK(3, end);

      bool left = h2::h2_range<3, -1>::in(-8);
      OK(false, left);

      bool inside = h2::h2_range<3, -1>::in(1);
      OK(true, inside);

      bool right = h2::h2_range<3, -1>::in(10);
      OK(false, right);
   }

   Case(Range<1, 3>)
   {
      int start = h2::h2_range<1, 3>::start;
      int end = h2::h2_range<1, 3>::end;
      OK(1, start);
      OK(3, end);

      bool left = h2::h2_range<1, 3>::in(-8);
      OK(false, left);

      bool inside = h2::h2_range<1, 3>::in(2);
      OK(true, inside);

      bool right = h2::h2_range<1, 3>::in(10);
      OK(false, right);
   }
}

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
