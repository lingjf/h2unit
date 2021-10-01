#include "../source/h2_unit.cpp"

SUITE(json match)
{
   Case(simple number)
   {
      const h2::h2_string expect = "123";
      const h2::h2_string actual = "123.0";
      OK(0 == h2::h2_json::match(expect, actual, false));
   }
}

SUITE(json diff)
{
   Case(simple number)
   {
      const h2::h2_string expect = "123";
      const h2::h2_string actual = "123.0";

      h2::h2_lines e_lines, a_lines;

      h2::h2_json::diff(expect, actual, e_lines, a_lines, false);
   }
}
