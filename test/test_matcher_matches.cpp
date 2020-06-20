#include "../source/h2_unit.cpp"

SUITE(matches matches)
{
   int int65 = 65, int66 = 66;

   Case(Any)
   {
      h2::h2_matches_any a;
      OK(nullptr == a.matches(65));
      OK(nullptr == a.matches(65.000000001));
      OK(nullptr == a.matches(true));
      OK(nullptr == a.matches("abc"));
      OK(nullptr == a.matches(NULL));
      OK(nullptr == a.matches(nullptr));
   }

   Case(Null)
   {
      int int65 = 65;

      h2::h2_matches_null ee1(false);
      OK(nullptr == ee1.matches(NULL));
      OK(nullptr == ee1.matches(nullptr));
      OK(nullptr != ee1.matches(&int65));
      OK("IsNull", ee1.expects(nullptr));
      OK("NotNull", ee1.expects(nullptr, false, true));

      h2::h2_matches_null ee2(true);
      OK(nullptr == ee2.matches(&int65));
      OK(nullptr != ee2.matches(nullptr));
      OK("NotNull", ee2.expects(nullptr));
      OK("IsNull", ee2.expects(nullptr, false, true));
   }

   Case(Boolean)
   {
      h2::h2_matches_boolean<true> istrue;
      OK(nullptr == istrue.matches(true));
      OK(nullptr != istrue.matches(false));
      OK("true", istrue.expects(false));
      OK("false", istrue.expects(false, false, true));

      h2::h2_matches_boolean<false> isfalse;
      OK(nullptr == isfalse.matches(false));
      OK(nullptr != isfalse.matches(true));
      OK("false", isfalse.expects(false));
      OK("true", isfalse.expects(false, false, true));
   }

   Case(Pointee)
   {
      h2::h2_pointee_matches<h2::h2_polymorphic_matcher<h2::h2_equation<int>>>
        a1(h2::h2_polymorphic_matcher<h2::h2_equation<int>>{h2::h2_equation<int>(65)});

      OK(nullptr == a1.matches(&int65));
      OK(nullptr != a1.matches(&int66));
      OK("65", a1.expects(&int65));
   }
}
