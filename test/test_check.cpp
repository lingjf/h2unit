
SUITE(OK Primitive)
{
  Case(Any)
  {
    OK(_, 1);
    OK(Any, "A");
    OK((_), "A");
    OK((Any), "A");
  }

  Case(Not)
  {
    OK(Not(1), 2);
    OK(Not("A"), "a");
    OK(Not(Not(2)), 2);
    OK((Not(1)), 2);
  }

  Case(Not !)
  {
    OK(!1, 0); // normal !
    OK(!(Not("A")), "A");
    OK(!(Not(1)), 1);
  }

  Case(IsNull NotNull)
  {
    OK(IsNull, 0);
    OK((NotNull), 1);
  }

  Case(&&Primitive)
  {
    OK(Ge(-1) && Le(1), 0);
    OK(CaseLess("AbCd") && Eq("abcd"), "abcd");
    OK(Ge(-1) && Le(1) && Eq(0), 0);
  }

  Case(&&naive)
  {
    OK(Ge(-1) && 1, 1);
    OK(1 && Ge(-1), 1);
    OK(CaseLess("AbCd") && "abcd", "abcd");
    OK(CaseLess("AbCd") && "abcd" && StartsWith("ab"), "abcd");
  }

  Case(|| Primitive)
  {
    OK(Ge(-1) || Le(1), 0);
    OK(CaseLess("AbCd") || Eq("abcd"), "abcd");
  }

  Case(|| naive)
  {
    OK(Ge(-1) || 1, 1);
    OK(1 || Ge(-1), 1);
    OK(CaseLess("AbCd") || "abcd", "abcd");
  }

  Case(&& || !)
  {
    OK(Ge(0) || -1 && Eq(1), 1);
    OK(!(Ge(0) || -1), -2);
  }

  Case(Re)
  {
    OK(Re("abc.*"), "abcdef");
    OK(!Re("A.*"), "abcdef");
  };

  Case(We)
  {
    OK(We("abc*"), "abcdef");
    OK(!We("abc?yz"), "abcdef");
  };

  Case(CaseLess)
  {
    OK(CaseLess("AbCd"), "abcd");
    OK(!CaseLess("AbCd"), "cba");
    OK(CaseLess(Contains("AbCd")), "ABCD");
    OK(CaseLess(Re("A.*")), "abcdef");
    OK(CaseLess(We("A*")), "abcdef");
  }

  Case(CaseLess ~)
  {
    // OK(~"AbCd", "abcd");
    OK(!CaseLess("AbCd"), "cba");
    OK(~Contains("AbCd"), "ABCD");
    OK(~Re("A.*"), "abcdef");
    OK(~We("A*"), "abcdef");
  }
}
