#include "../source/h2_unit.cpp"

SUITE(matches c)
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
      h2::h2_mc c(false, false, false, false);
      OK(c.fit(true));
      OK(!c.fit(false));
   }

   Case(fit negative)
   {
      h2::h2_mc c(true, false, false, false);
      OK(!c.fit(true));
      OK(c.fit(false));
   }

   Case(update negative)
   {
      h2::h2_mc c(false, false, false, false);
      h2::h2_mc d = c.update_negative(true);
      OK(d.negative);
      OK(!d.case_insensitive);
      OK(!d.squash_whitespace);
      OK(!d.no_compare_operator);
   }

   Case(update caseless)
   {
      h2::h2_mc c(false, false, false, false);
      h2::h2_mc d = c.update_caseless(true);
      OK(!d.negative);
      OK(d.case_insensitive);
      OK(!d.squash_whitespace);
      OK(!d.no_compare_operator);
   }

   Case(update spaceless)
   {
      h2::h2_mc c(false, false, false, false);
      h2::h2_mc d = c.update_spaceless(true);
      OK(!d.negative);
      OK(!d.case_insensitive);
      OK(d.squash_whitespace);
      OK(!d.no_compare_operator);
   }
}
