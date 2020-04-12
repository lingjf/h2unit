
H2MATCHER(IsEven, ("is not Even"))
{
  return a % 2 == 0;
}
H2MATCHER(IsEven2)
{
  return a % 2 == 0;
}
H2MATCHER(IsEven3, (""))
{
  return a % 2 == 0;
}

H2MATCHER(Divable, expect, ("not divable"))
{
  return a % expect == 0;
}

H2MATCHER(Divable2, expect)
{
  return a % expect == 0;
}

H2MATCHER(Between,
          left,
          right,
          (a << " not in [" << left << ", " << right << "]"))
{
  return left <= a && a <= right;
}

H2MATCHER(InRange, start, stop, step, ("not in range"))
{
  for (int i = start; i < stop; i += step)
    if (i == a)
      return true;
  return false;
}

H2MATCHER(InRect, left, top, right, bottom, ("not in rect"))
{
  return true;
}

H2MATCHER(InProtocol, ip1, port1, ip2, port2, proto, ("not in proto"))
{
  return true;
}

SUITE(User Defined Matcher)
{
  Case(IsEven)
  {
    OK(IsEven, 2);
    OK(IsEven2, 2);
    OK(IsEven3, 2);
    OK(!IsEven, 1);
    OK(!IsEven2, 1);
    OK(!IsEven3, 1);
  }
  Case(Divable)
  {
    OK(Divable(3), 6);
    OK(Divable2(3), 6);
    OK(!Divable(3), 7);
    OK(!Divable2(3), 7);
  }
  Case(Between)
  {
    OK(Between(1, 4), 2);
    OK(!Between(1, 4), 5);
  }
  Case(InRange)
  {
    OK(InRange(1, 10, 2), 5);
    OK(!InRange(1, 10, 2), 6);
  }
  Case(InRect) { OK(InRect(1, 10, 2, 7), 0); }
  Case(InProtocol) { OK(InProtocol(1, 10, 2, 7, 9), 0); }
}
