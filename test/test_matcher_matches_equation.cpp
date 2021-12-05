#include "../source/h2_unit.cpp"
#include <limits>

SUITE(equation matches)
{
   Case(integer)
   {
      h2::h2_equation<int> a1(123);
      OK(nullptr == a1.matches(123, {}));
      h2::h2_equation<const int> a2(123);
      OK(nullptr == a2.matches(123, {}));
      h2::h2_equation<unsigned long long> a3(123);
      OK(nullptr == a3.matches(123, {}));
   }

   Case(string)
   {
      // cannot convert argument 1 from 'const char [4]' to 'const char *'
      // <ConformanceMode>false</ConformanceMode>; /permissive- ==> /permissive
      h2::h2_equation<char*> a1((char*)"abc");
      OK(nullptr == a1.matches((char*)"abc", {}));
      h2::h2_equation<h2::h2_string> a2("abc");
      OK(nullptr == a2.matches("abc", {}));
      h2::h2_equation<std::string> a3("abc");
      OK(nullptr == a3.matches("abc", {}));
   }

   Case(float)
   {
      h2::h2_equation<float> a1(65.0);
      OK(nullptr == a1.matches(65, {}));
      h2::h2_equation<double> a2(65.0);
      OK(nullptr == a2.matches(65, {}));
   }
}

SUITE(approximate float)
{
   Case(base)
   {
      h2::h2_approximate a;

      auto a0 = a * 0.1;
      OK((a0 == 0.1 * h2::h2_approximate::A));
      OK((a0 + h2::h2_approximate::B == 0.1 * h2::h2_approximate::A + h2::h2_approximate::B));
      auto a1 = a * 1.0;
      OK((a1 == 1.0 * h2::h2_approximate::A));
      OK((a1 + h2::h2_approximate::B == 1.0 * h2::h2_approximate::A + h2::h2_approximate::B));
      auto a2 = a * 2.2;
      OK(Eq(2.2 * h2::h2_approximate::A, 1), a2);
      OK(Eq(2.2 * h2::h2_approximate::A + h2::h2_approximate::B, 1), a2 + h2::h2_approximate::B);
   }

   Case(absolute default approximate)
   {
      OK(Eq(100.0), 100.0);
      OK(Eq(3.14), 3.14);
      OK(Eq(1.0 / 3.0), 2.0 / 6.0);
   }

   Case(absolute approximate)
   {
      OK(Eq(100.0, 0.1), 100.1);
      OK(Eq(100.0, 0.1), 100.0999);
      OK(Eq(100.0, 0.1), 100);
      OK(Eq(100.0, 0.1), 99.9999);
      OK(Eq(100.0, 0.1), 99.9);
   }

   Case(absolute approximate)
   {
      OK(Eq(1.0 / 3.0, std::numeric_limits<double>::epsilon()), 2.0 / 6.0);
   }

   Case(percentage 1% approximate)
   {
      OK(Eq(100.0, 1 %), 101);
      OK(Eq(100.0, 1 %), 100.5);
      OK(Eq(100.0, 1 %), 100);
      OK(Eq(100.0, 1 %), 99.5);
      OK(Eq(100.0, 1 %), 99);
   }

   Case(percentage 0.1% approximate)
   {
      OK(Eq(100.0, 0.1 %), 100.1);
      OK(Eq(100.0, 0.1 %), 100.05);
      OK(Eq(100.0, 0.1 %), 100);
      OK(Eq(100.0, 0.1 %), 99.95);
      OK(Eq(100.0, 0.1 %), 99.9);
   }

   Case(percentage 5% approximate)
   {
      OK(Eq(100.0, 5 %), 105);
      OK(Eq(100.0, 5 %), 103);
      OK(Eq(100.0, 5 %), 100);
      OK(Eq(100.0, 5 %), 98);
      OK(Eq(100.0, 5 %), 95);
   }
}
