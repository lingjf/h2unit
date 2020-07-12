#include "../source/h2_unit.cpp"

SUITE(matches unary)
{
   int int65 = 65, int66 = 66;

   Case(Any)
   {
      h2::h2_matches_any a;
      OK(nullptr == a.matches(65, false, false));
      OK(nullptr == a.matches(65.000000001, false, false));
      OK(nullptr == a.matches(true, false, false));
      OK(nullptr == a.matches("abc", false, false));
      OK(nullptr == a.matches(NULL, false, false));
      OK(nullptr == a.matches(nullptr, false, false));
   }

   Case(Null)
   {
      int int65 = 65;

      h2::h2_matches_null ee1(false);
      OK(nullptr == ee1.matches(NULL, false, false));
      OK(nullptr == ee1.matches(nullptr, false, false));
      OK(nullptr != ee1.matches(&int65, false, false));
      OK("IsNull", ee1.expects(false, false));
      OK("NotNull", ee1.expects(false, true));

      h2::h2_matches_null ee2(true);
      OK(nullptr == ee2.matches(&int65, false, false));
      OK(nullptr != ee2.matches(nullptr, false, false));
      OK("NotNull", ee2.expects(false, false));
      OK("IsNull", ee2.expects(false, true));
   }

   Case(Boolean)
   {
      h2::h2_matches_boolean<true> istrue;
      OK(nullptr == istrue.matches(true, false, false));
      OK(nullptr != istrue.matches(false, false, false));
      OK("true", istrue.expects(false, false));
      OK("false", istrue.expects(false, true));

      h2::h2_matches_boolean<false> isfalse;
      OK(nullptr == isfalse.matches(false, false, false));
      OK(nullptr != isfalse.matches(true, false, false));
      OK("false", isfalse.expects(false, false));
      OK("true", isfalse.expects(false, true));
   }

   Case(Pointee)
   {
      h2::h2_pointee_matches<h2::h2_polymorphic_matcher<h2::h2_equation<int>>> a1(h2::h2_polymorphic_matcher<h2::h2_equation<int>>{h2::h2_equation<int>(65)});

      OK(nullptr == a1.matches(&int65, false, false));
      OK(nullptr != a1.matches(&int66, false, false));
      OK("65", a1.expects(false, false));
   }
}
