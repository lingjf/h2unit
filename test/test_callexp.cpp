#include "../source/h2_unit.cpp"

SUITE(callexp)
{
  Case(once)
  {
    h2::h2_callexp ce(1, 1);
    OK(1, ce.least);
    OK(1, ce.most);
    OK(0, ce.call);
    OK("exactly 1 times", ce.expect());

    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("never", ce.actual());
    OK(ce.check());

    ++ce; // call 1 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("1 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 2 times
    OK(!ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(ce.is_overmuch());
    OK("2 times", ce.actual());
    OK(ce.check());
  }

  Case(twice)
  {
    h2::h2_callexp ce(2, 2);
    OK(2, ce.least);
    OK(2, ce.most);
    OK(0, ce.call);
    OK("exactly 2 times", ce.expect());

    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("never", ce.actual());
    OK(ce.check());

    ++ce; // call 1 times
    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("1 times", ce.actual());
    OK(ce.check());

    ++ce; // call 2 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("2 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 3 times
    OK(!ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(ce.is_overmuch());
    OK("3 times", ce.actual());
    OK(ce.check());
  }

  Case(3 times)
  {
    h2::h2_callexp ce(3, 3);
    OK(3, ce.least);
    OK(3, ce.most);
    OK(0, ce.call);
    OK("exactly 3 times", ce.expect());

    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("never", ce.actual());
    OK(ce.check());

    ++ce; // call 1 times
    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("1 times", ce.actual());
    OK(ce.check());

    ++ce; // call 2 times
    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("2 times", ce.actual());
    OK(ce.check());

    ++ce; // call 3 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("3 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 4 times
    OK(!ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(ce.is_overmuch());
    OK("4 times", ce.actual());
    OK(ce.check());
  }

  Case(any)
  {
    h2::h2_callexp ce(0, INT_MAX);
    OK(0, ce.least);
    OK(INT_MAX, ce.most);
    OK(0, ce.call);
    OK("any number of times", ce.expect());

    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("never", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 1 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("1 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 2 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("2 times", ce.actual());
    OK(IsNull, ce.check());
  }

  Case(at least)
  {
    h2::h2_callexp ce(2, INT_MAX);
    OK(2, ce.least);
    OK(INT_MAX, ce.most);
    OK(0, ce.call);
    OK("at least 2 times", ce.expect());

    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("never", ce.actual());
    OK(ce.check());

    ++ce; // call 1 times
    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("1 times", ce.actual());
    OK(ce.check());

    ++ce; // call 2 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("2 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 3 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("3 times", ce.actual());
    OK(IsNull, ce.check());
  }

  Case(at most)
  {
    h2::h2_callexp ce(0, 2);
    OK(0, ce.least);
    OK(2, ce.most);
    OK(0, ce.call);
    OK("at most 2 times", ce.expect());

    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("never", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 1 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("1 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 2 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("2 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 3 times
    OK(!ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(ce.is_overmuch());
    OK("3 times", ce.actual());
    OK(ce.check());
  }

  Case(between)
  {
    h2::h2_callexp ce(2, 4);
    OK(2, ce.least);
    OK(4, ce.most);
    OK(0, ce.call);
    OK("between 2 and 4 times", ce.expect());

    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("never", ce.actual());
    OK(ce.check());

    ++ce; // call 1 times
    OK(ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("1 times", ce.actual());
    OK(ce.check());

    ++ce; // call 2 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("2 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 3 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("3 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 4 times
    OK(!ce.is_not_enough());
    OK(ce.is_satisfied());
    OK(ce.is_saturated());
    OK(!ce.is_overmuch());
    OK("4 times", ce.actual());
    OK(IsNull, ce.check());

    ++ce; // call 5 times
    OK(!ce.is_not_enough());
    OK(!ce.is_satisfied());
    OK(!ce.is_saturated());
    OK(ce.is_overmuch());
    OK("5 times", ce.actual());
    OK(ce.check());
  }
}
