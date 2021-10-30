#include "../source/h2_unit.cpp"

SUITE(matches unary)
{
   int int65 = 65, int66 = 66;

   Case(Any)
   {
      h2::h2_matches_any a;
      OK(nullptr == a.matches(65, 0, {}));
      OK(nullptr == a.matches(65.000000001, 0, {}));
      OK(nullptr == a.matches(true, 0, {}));
      OK(nullptr == a.matches("abc", 0, {}));
      OK(nullptr == a.matches(NULL, 0, {}));
      OK(nullptr == a.matches(nullptr, 0, {}));
   }

   Case(OK Any)
   {
      OK(_, 0);
      OK(_, 1);
      OK(Any, 0);
      OK(Any, 1);
   }

   Case(Null)
   {
      int int65 = 65;

      h2::h2_matches_null ee1(false);
      OK(nullptr == ee1.matches(NULL, 0, {}));
      OK(nullptr == ee1.matches(nullptr, 0, {}));
      OK(nullptr != ee1.matches(&int65, 0, {}));
      OK("IsNull", ee1.expection({}));
      OK("NotNull", ee1.expection({true, false, false, false}));

      h2::h2_matches_null ee2(true);
      OK(nullptr == ee2.matches(&int65, 0, {}));
      OK(nullptr != ee2.matches(nullptr, 0, {}));
      OK("NotNull", ee2.expection({}));
      OK("IsNull", ee2.expection({true, false, false, false}));
   }

   Case(OK Null)
   {
      OK(IsNull, 0);
      OK(IsNull(), 0);
      OK(NotNull, 1234);
      OK(NotNull(), 1234);
   }

   Case(Boolean)
   {
      h2::h2_matches_boolean<true> istrue;
      OK(nullptr == istrue.matches(true, 0, {}));
      OK(nullptr != istrue.matches(false, 0, {}));
      OK("true", istrue.expection({}));
      OK("false", istrue.expection({true, false, false, false}));

      h2::h2_matches_boolean<false> isfalse;
      OK(nullptr == isfalse.matches(false, 0, {}));
      OK(nullptr != isfalse.matches(true, 0, {}));
      OK("false", isfalse.expection({}));
      OK("true", isfalse.expection({true, false, false, false}));
   }

   Case(OK Boolean)
   {
      OK(IsTrue, 1);
      OK(IsTrue(), 1);
      OK(IsFalse, 0);
      OK(IsFalse(), 0);
   }

   Case(Pointee)
   {
      h2::h2_pointee_matches<h2::h2_polymorphic_matcher<h2::h2_equation<int>>> a1(h2::h2_polymorphic_matcher<h2::h2_equation<int>>{h2::h2_equation<int>(65)});

      OK(nullptr == a1.matches(&int65, 0, {}));
      OK(nullptr != a1.matches(&int66, 0, {}));
      OK("65", a1.expection({}));
   }

   Case(OK Pointee)
   {
      char char_value = 'C';
      char* char_ptr = &char_value;
      const char* const_char_ptr = &char_value;
      int int_value = 1;
      int* int_ptr = &int_value;
      const int* const_int_ptr = &int_value;
      std::string stdstring_value = "S";
      std::string* stdstring_ptr = &stdstring_value;
      const std::string* const_stdstring_ptr = &stdstring_value;

      OK(Pointee(1), int_ptr);
      OK(Pointee(1), const_int_ptr);
      OK(Pointee('C'), char_ptr);
      OK(Pointee('C'), const_char_ptr);
      OK(Pointee("S"), stdstring_ptr);
      OK(Pointee("S"), const_stdstring_ptr);
   }
}
