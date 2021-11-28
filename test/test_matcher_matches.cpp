#include "../source/h2_unit.cpp"

SUITE(matches configure)
{
   Case(default)
   {
      h2::h2_mc c;
      OK(!c.negative);
      OK(!c.case_insensitive);
      OK(!c.squash_whitespace);
      OK(!c.no_compare_operator);
   }

   Case(fit)
   {
      h2::h2_mc c(0, false, false, false, false);
      OK(c.fit(true));
      OK(!c.fit(false));
   }

   Case(fit negative)
   {
      h2::h2_mc c(0, true, false, false, false);
      OK(!c.fit(true));
      OK(c.fit(false));
   }

   Case(update negative)
   {
      h2::h2_mc c(0, false, false, false, false);
      h2::h2_mc d = c.update_negative(true);
      OK(d.negative);
      OK(!d.case_insensitive);
      OK(!d.squash_whitespace);
      OK(!d.no_compare_operator);
   }

   Case(update caseless)
   {
      h2::h2_mc c(0, false, false, false, false);
      h2::h2_mc d = c.update_caseless(true);
      OK(!d.negative);
      OK(d.case_insensitive);
      OK(!d.squash_whitespace);
      OK(!d.no_compare_operator);
   }

   Case(update spaceless)
   {
      h2::h2_mc c(0, false, false, false, false);
      h2::h2_mc d = c.update_spaceless(true);
      OK(!d.negative);
      OK(!d.case_insensitive);
      OK(d.squash_whitespace);
      OK(!d.no_compare_operator);
   }
}

CASE(any matches)
{
   h2::h2_matches_any a;
   OK(nullptr == a.matches(65, {}));
   OK(nullptr == a.matches(65.000000001, {}));
   OK(nullptr == a.matches(true, {}));
   OK(nullptr == a.matches("abc", {}));
   OK(nullptr == a.matches(NULL, {}));
   OK(nullptr == a.matches(nullptr, {}));
}

CASE(null matches)
{
   int int65 = 65;

   h2::h2_matches_null a1;
   OK(nullptr == a1.matches(NULL, {}));
   OK(nullptr == a1.matches(nullptr, {}));
   OK(nullptr != a1.matches(&int65, {}));
   OK("NULL", a1.expection({}));
   OK("!NULL", a1.expection({0, true, false, false, false}));
}

CASE(bool matches)
{
   h2::h2_matches_bool IsTrue(true);
   OK(nullptr == IsTrue.matches(true, {}));
   OK(nullptr != IsTrue.matches(false, {}));
   OK("true", IsTrue.expection({}));
   OK("false", IsTrue.expection({0, true, false, false, false}));

   h2::h2_matches_bool IsFalse(false);
   OK(nullptr == IsFalse.matches(false, {}));
   OK(nullptr != IsFalse.matches(true, {}));
   OK("false", IsFalse.expection({}));
   OK("true", IsFalse.expection({0, true, false, false, false}));
}
