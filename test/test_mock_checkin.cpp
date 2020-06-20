#include "../source/h2_unit.cpp"

SUITE(checkin)
{
   Case(once)
   {
      h2::h2_checkin ci(1, 1);
      OK(1, ci.least);
      OK(1, ci.most);
      OK(0, ci.call);
      OK("exactly 1 times", ci.expect());

      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("never", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 1 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("1 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 2 times
      OK(!ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_overmuch());
      OK("2 times", ci.actual());
      OK(ci.check("", nullptr, 0));
   }

   Case(twice)
   {
      h2::h2_checkin ci(2, 2);
      OK(2, ci.least);
      OK(2, ci.most);
      OK(0, ci.call);
      OK("exactly 2 times", ci.expect());

      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("never", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 1 times
      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("1 times", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 2 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 3 times
      OK(!ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_overmuch());
      OK("3 times", ci.actual());
      OK(ci.check("", nullptr, 0));
   }

   Case(3 times)
   {
      h2::h2_checkin ci(3, 3);
      OK(3, ci.least);
      OK(3, ci.most);
      OK(0, ci.call);
      OK("exactly 3 times", ci.expect());

      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("never", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 1 times
      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("1 times", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 2 times
      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("2 times", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 3 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("3 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 4 times
      OK(!ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_overmuch());
      OK("4 times", ci.actual());
      OK(ci.check("", nullptr, 0));
   }

   Case(any)
   {
      h2::h2_checkin ci(0, INT_MAX);
      OK(0, ci.least);
      OK(INT_MAX, ci.most);
      OK(0, ci.call);
      OK("any number of times", ci.expect());

      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("never", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 1 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("1 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 2 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));
   }

   Case(at least)
   {
      h2::h2_checkin ci(2, INT_MAX);
      OK(2, ci.least);
      OK(INT_MAX, ci.most);
      OK(0, ci.call);
      OK("at least 2 times", ci.expect());

      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("never", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 1 times
      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("1 times", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 2 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 3 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("3 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));
   }

   Case(at most)
   {
      h2::h2_checkin ci(0, 2);
      OK(0, ci.least);
      OK(2, ci.most);
      OK(0, ci.call);
      OK("at most 2 times", ci.expect());

      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("never", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 1 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("1 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 2 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 3 times
      OK(!ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_overmuch());
      OK("3 times", ci.actual());
      OK(ci.check("", nullptr, 0));
   }

   Case(between)
   {
      h2::h2_checkin ci(2, 4);
      OK(2, ci.least);
      OK(4, ci.most);
      OK(0, ci.call);
      OK("between 2 and 4 times", ci.expect());

      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("never", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 1 times
      OK(ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("1 times", ci.actual());
      OK(ci.check("", nullptr, 0));

      ++ci;  // call 2 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("2 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 3 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("3 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 4 times
      OK(!ci.is_not_enough());
      OK(ci.is_satisfied());
      OK(ci.is_saturated());
      OK(!ci.is_overmuch());
      OK("4 times", ci.actual());
      OK(IsNull, ci.check("", nullptr, 0));

      ++ci;  // call 5 times
      OK(!ci.is_not_enough());
      OK(!ci.is_satisfied());
      OK(!ci.is_saturated());
      OK(ci.is_overmuch());
      OK("5 times", ci.actual());
      OK(ci.check("", nullptr, 0));
   }
}
