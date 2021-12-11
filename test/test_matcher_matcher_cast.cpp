#define H2_NO_Redefine_private

#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(matcher cast)
{
   Case(1.1)
   {
      h2::h2_matcher<int> m2 = h2::h2_matcher_cast_impl<int, double>::cast(123.45);
      OK(nullptr == m2.matches(123));
   }

   Case(1.2)
   {
   }

   Case(1.3)
   {
   }

   Case(2)
   {
      h2::h2_matcher<double> m1(Eq(123));
      h2::h2_matcher<int> m2 = h2::h2_matcher_cast_impl<int, h2::h2_matcher<double>>::cast(m1);
      OK(nullptr == m2.matches(123));
   }

   Case(3)
   {
      h2::h2_matcher<double> m1(Eq(123.45));
      h2::h2_matcher<double> m2 = h2::h2_matcher_cast_impl<double, h2::h2_matcher<double>>::cast(m1);
      OK(nullptr == m2.matches(123.45));
   }
}

#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)
#include <any>
#include <variant>
#include <optional>

CASE(std::any)
{
   std::any a = 1;
   // OK(1, a);

   OK(1, std::any_cast<int>(a));
   a = 3.14;
   OK(3.14, std::any_cast<double>(a));
   a = true;
   OK(std::any_cast<bool>(a));
   a = "any";
   OK("any", std::any_cast<const char*>(a));

   try {
      a = 1;
      std::any_cast<float>(a);
   } catch (const std::bad_any_cast&) {
      // bad cast
   }
}

CASE(std::variant)
{
   std::variant<int, double> a1, a2;
   a1 = 42;  // a1 contains int
   OK(42, a1);
   OK(42 == std::get<int>(a1));

   a2 = std::get<int>(a1);
   a2 = std::get<0>(a1);  // same effect as the previous line
   a2 = a1;               // same effect as the previous line
   OK(42, a2);
   OK(42 == std::get<int>(a2));

   a2 = 3.14;
   OK(3.14, a2);
   OK(3.14 == std::get<double>(a2));

   try {
      std::get<double>(a1);
   } catch (const std::bad_variant_access&) {
      // bad cast
   }

   std::variant<std::string> a3("hello");  // converting constructors work when unambiguous
   a3 = "world";                           // converting assignment also works when unambiguous
   OK("world", a3);

   std::variant<std::string, void const*> a4("hello");  // casts to void const * when passed a char const *
   OK(std::holds_alternative<void const*>(a4));
   OK("hello", (const char*)std::get<1>(a4));

   using namespace std::literals;
   a4 = "world"s;
   OK(std::holds_alternative<std::string>(a4));
   OK("world", std::get<std::string>(a4));
}

std::optional<std::string> create_optional(bool b)
{
   if (b)
      return "hello optional";
   return {};
}

CASE(optional)
{
   auto a1 = create_optional(false).value_or("empty");
   OK("empty", a1);
   OK("empty", create_optional(false).value_or("empty"));

   auto a2 = create_optional(true).value_or("empty");
   OK("hello optional", a2);
   OK("hello optional", create_optional(true).value_or("empty"));
}

#endif
