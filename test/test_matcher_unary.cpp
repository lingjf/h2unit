#include "../source/h2_unit.cpp"

SUITE(matches unary)
{
   int int65 = 65, int66 = 66;

   Case(Any)
   {
      h2::h2_matches_any a;
      OK(nullptr == a.matches(65, {}));
      OK(nullptr == a.matches(65.000000001, {}));
      OK(nullptr == a.matches(true, {}));
      OK(nullptr == a.matches("abc", {}));
      OK(nullptr == a.matches(NULL, {}));
      OK(nullptr == a.matches(nullptr, {}));
   }

   Case(OK Any)
   {
      OK(_, 0);
      OK(_, 1);
      OK(Any, 0);
      OK(Any, 1);
   }

   Case(Pointee)
   {
      h2::h2_pointee_matches<h2::h2_polymorphic_matcher<h2::h2_equation<int>>> a1(h2::h2_polymorphic_matcher<h2::h2_equation<int>>{h2::h2_equation<int>(65)});

      OK(nullptr == a1.matches(&int65, {}));
      OK(nullptr != a1.matches(&int66, {}));
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

   Case(OK smart pointee)
   {
      auto a1 = std::make_shared<int>(42);
      OK(Pointee(42), a1);
   }
}
