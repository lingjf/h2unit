#include "../source/h2_unit.cpp"

SUITE(array)
{
   int b[] = {1, 2, 3};
   h2::h2_array<int> a1(b, 3);
   h2::h2_array<int> a0(b, 0);

   Case(h2_is_iterable)
   {
      OK((h2::h2_is_iterable<h2::h2_array<int>>::value));
      OK((h2::h2_is_iterable<decltype(a1)>::value));
      OK(!(h2::h2_is_iterable<decltype(b)>::value));
   }

   Case(size)
   {
      OK(3, a1.size());
   }

   Case(max_size)
   {
      OK(3, a1.max_size());
   }

   Case(empty)
   {
      OK(!a1.empty());
      OK(a0.empty());
   }

   Case(at)
   {
      OK(1, a1[0]);
      OK(2, a1[1]);
      OK(3, a1[2]);

      OK(1, a1.at(0));
      OK(2, a1.at(1));
      OK(3, a1.at(2));
   }

   Case(front)
   {
      OK(1, a1.front());
   }

   Case(back)
   {
      OK(3, a1.back());
   }

   Case(data)
   {
      OK(b, a1.data());
   }

   Case(for)
   {
      int i = 0;
      for (auto v : a1) {
         OK(1 + i++, v);
      }
   }

   Case(iterator)
   {
      int i = 0;
      for (auto it = a1.begin(); it != a1.end(); ++it) {
         OK(1 + i++, *it);
      }
   }

   Case(const iterator)
   {
      int i = 0;
      for (auto it = a1.cbegin(); it != a1.cend(); ++it) {
         OK(1 + i++, *it);
      }
   }

   Case(const)
   {
      const int a7[] = {1, 2, 3};
      h2::h2_array<decltype(a7[0])> a8(a7, 3);
      OK(1, a8.front());
   }
}
