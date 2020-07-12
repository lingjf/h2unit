#include "../source/h2_unit.cpp"

SUITE(equation matches)
{
   Case(Equal)
   {
      h2::h2_equation<int> a1(123);
      OK(nullptr == a1.matches(123, false, false));
      h2::h2_equation<const int> a2(123);
      OK(nullptr == a2.matches(123, false, false));
      h2::h2_equation<unsigned long long> a3(123);
      OK(nullptr == a3.matches(123, false, false));

      h2::h2_equation<bool> b1(false);
      OK(nullptr == b1.matches(false, false, false));
      h2::h2_equation<const bool> b2(false);
      OK(nullptr == b2.matches(false, false, false));

      h2::h2_equation<float> c1(65.000000001);
      OK(nullptr == c1.matches(65, false, false));
      h2::h2_equation<double> c2(65.000000001);
      OK(nullptr == c2.matches(65, false, false));
      h2::h2_equation<double> c3(65.000000001, 0.001);
      OK(nullptr == c3.matches(65, false, false));
      h2::h2_equation<double> c4(65.001, 0.0001);
      OK(nullptr != c4.matches(65, false, false));

      h2::h2_equation<char*> d1("abc");
      OK(nullptr == d1.matches("abc", false, false));
      h2::h2_equation<h2::h2_string> d2("abc");
      OK(nullptr == d2.matches("abc", false, false));
      h2::h2_equation<std::string> d3("abc");
      OK(nullptr == d3.matches("abc", false, false));

      h2::h2_equation<char*> d4("*bc");
      OK(nullptr == d4.matches("abc", false, false));

      h2::h2_equation<char*> d5(".*bc");
      OK(nullptr == d5.matches("abc", false, false));
   }
}
