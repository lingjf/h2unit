#include "../source/h2_unit.cpp"

SUITE(matches c)
{
   Case(default)
   {
      h2::h2_mc c;
      OK(!c.caseless);
      OK(!c.dont);
      OK(!c.ncop);
      OK(!c.spaceless);
   }

   Case(fit)
   {
      h2::h2_mc c(false, false, false, false);
      OK(c.fit(true));
      OK(!c.fit(false));
   }

   Case(fit negative)
   {
      h2::h2_mc c(false, true, false, false);
      OK(!c.fit(true));
      OK(c.fit(false));
   }

   Case(update)
   {
      h2::h2_mc c;
      h2::h2_mc d = c.update_caseless(true);
      OK(d.caseless);
      OK(!d.dont);
      OK(!d.ncop);
      OK(!d.spaceless);
   }
}
