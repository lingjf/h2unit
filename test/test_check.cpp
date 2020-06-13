#include "../source/h2_unit.cpp"

SUITE(Check Utility)
{
   Case(find_outer_comma)
   {
      OK(",b", h2::find_outer_comma("a,b"));
      OK(",b", h2::find_outer_comma("\"a\",b"));
      OK(",b", h2::find_outer_comma("\"a,\",b"));
      OK(",b", h2::find_outer_comma("'a',b"));
      OK(",b", h2::find_outer_comma("(a,),b"));
      OK(",b", h2::find_outer_comma("<a,>,b"));
   }

   Case(split_expression)
   {
      h2::h2_string e_expression = "__null";
      h2::h2_string a_expression = "((void *)0)";
      h2::split_expression(e_expression, a_expression, "NULL, NULL");
      OK("NULL", e_expression);
      OK("NULL", a_expression);
   }
}

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
      OK(!1, 0);  // normal !
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
      OK(CaseLess(Substr("AbCd")), "ABCD");
      OK(CaseLess(Re("A.*")), "abcdef");
      OK(CaseLess(We("A*")), "abcdef");
   }

   Case(CaseLess ~)
   {
      // OK(~"AbCd", "abcd");
      OK(!CaseLess("AbCd"), "cba");
      OK(~Substr("AbCd"), "ABCD");
      OK(~Re("A.*"), "abcdef");
      OK(~We("A*"), "abcdef");
   }

   Case(ListOf)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 2, 3), a);
   }

   Case(In)
   {
      OK(In(1, 2, 3), 2);
   }

   Case(Has)
   {
      std::vector<int> a1 = {1, 2, 3};
      OK(Has(1, 3), a1);

      std::map<int, int> a2 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(1, 111), Pair(3, 333)), a2);
   }
}

SUITE(bugfix)
{
   Case(Eq integer with float)
   {
      float uv = 2.00001;
      OK(Eq(2, 0.0001), uv);
   }
}
