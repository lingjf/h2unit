#include "../source/h2_unit.cpp"

SUITE(checkin)
{
   Case(once)
   {
      h2::h2_checkin ci = h2::h2_checkin::Once();
      OK(1, ci.least);
      OK(1, ci.most);
      OK(0, ci.call);
      OK("exactly 1 times", ci.expect());

      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("never", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 1 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_excessive());
      OK("1 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 2 times
      OK(!ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_excessive());
      OK("2 times", ci.actual());
      OK(ci.check(0, 1, ""));
   }

   Case(twice)
   {
      h2::h2_checkin ci = h2::h2_checkin::Twice();
      OK(2, ci.least);
      OK(2, ci.most);
      OK(0, ci.call);
      OK("exactly 2 times", ci.expect());

      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("never", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 1 times
      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("1 times", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 2 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_excessive());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 3 times
      OK(!ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_excessive());
      OK("3 times", ci.actual());
      OK(ci.check(0, 1, ""));
   }

   Case(3 times)
   {
      h2::h2_checkin ci = h2::h2_checkin::Times(3);
      OK(3, ci.least);
      OK(3, ci.most);
      OK(0, ci.call);
      OK("exactly 3 times", ci.expect());

      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("never", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 1 times
      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("1 times", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 2 times
      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("2 times", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 3 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_excessive());
      OK("3 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 4 times
      OK(!ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_excessive());
      OK("4 times", ci.actual());
      OK(ci.check(0, 1, ""));
   }

   Case(any)
   {
      h2::h2_checkin ci = h2::h2_checkin::Any();
      OK(0, ci.least);
      OK(INT_MAX, ci.most);
      OK(0, ci.call);
      OK("any number of times", ci.expect());

      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("never", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 1 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("1 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 2 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));
   }

   Case(at least)
   {
      h2::h2_checkin ci = h2::h2_checkin::Atleast(2);
      OK(2, ci.least);
      OK(INT_MAX, ci.most);
      OK(0, ci.call);
      OK("at least 2 times", ci.expect());

      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("never", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 1 times
      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("1 times", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 2 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 3 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("3 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));
   }

   Case(at most)
   {
      h2::h2_checkin ci = h2::h2_checkin::Atmost(2);
      OK(0, ci.least);
      OK(2, ci.most);
      OK(0, ci.call);
      OK("at most 2 times", ci.expect());

      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("never", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 1 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("1 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 2 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_excessive());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 3 times
      OK(!ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_excessive());
      OK("3 times", ci.actual());
      OK(ci.check(0, 1, ""));
   }

   Case(between)
   {
      h2::h2_checkin ci = h2::h2_checkin::Between(2, 4);
      OK(2, ci.least);
      OK(4, ci.most);
      OK(0, ci.call);
      OK("between 2 and 4 times", ci.expect());

      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("never", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 1 times
      OK(ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("1 times", ci.actual());
      OK(ci.check(0, 1, ""));

      ++ci.call;  // call 2 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 3 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_excessive());
      OK("3 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 4 times
      OK(!ci.insufficient());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_excessive());
      OK("4 times", ci.actual());
      OK(IsNull, ci.check(0, 1, ""));

      ++ci.call;  // call 5 times
      OK(!ci.insufficient());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_excessive());
      OK("5 times", ci.actual());
      OK(ci.check(0, 1, ""));
   }
}
