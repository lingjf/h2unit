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
      h2::h2_mc c1(false, false, false, false);
      OK(c1.fit(true));
      OK(!c1.fit(false));

      h2::h2_mc c2(false, true, false, false);
      OK(!c2.fit(true));
      OK(c2.fit(false));
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
