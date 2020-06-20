#include "../source/h2_unit.cpp"

SUITE(Polymorphic Matcher)
{
   h2::h2_equation<int> a__(65);
   h2::h2_polymorphic_matcher<h2::h2_equation<int>> a(a__);

   Case(int)
   {
      h2::h2_matcher<int> b(a);
      OK(nullptr == b.matches(65));
   }

   Case(reverse)
   {
      h2::h2_matcher<int> b(!a);
      OK(nullptr != b.matches(65));
   }
}
