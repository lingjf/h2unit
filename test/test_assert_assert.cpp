#include "../source/h2_unit.cpp"

SUITE(find_op)
{
   Case(sample)
   {
      OK("== b", h2::find_op("a == b", "=="));
      OK("!= b", h2::find_op("a != b", "!="));
      OK("< b", h2::find_op("a < b", "<"));
      OK("<= b", h2::find_op("a <= b", "<="));
      OK("> b", h2::find_op("a > b", ">"));
      OK(">= b", h2::find_op("a >= b", ">="));
   }

   Case(no spac3)
   {
      OK("==b", h2::find_op("a ==b", "=="));
      OK("!= b", h2::find_op("a!= b", "!="));
      OK("<b", h2::find_op("a<b", "<"));
      OK("<=b", h2::find_op("a <=b", "<="));
      OK("> b", h2::find_op("a> b", ">"));
      OK(">=b", h2::find_op("a>=b", ">="));
   }

   Case(in string)
   {
      OK("==b", h2::find_op("\"a==\" ==b", "=="));
      OK("!= b", h2::find_op("\"a!=\"!= b", "!="));
      OK("<b", h2::find_op("\"a<\"<b", "<"));
      OK("<=b", h2::find_op("\"a<=\" <=b", "<="));
      OK("> b", h2::find_op("\"a>\"> b", ">"));
      OK(">=b", h2::find_op("\"a>=\">=b", ">="));
   }

   Case(has template)
   {
      OK("== b", h2::find_op("sizeof(std::vector<int>::size_type) == b", "=="));
      OK("!= b", h2::find_op("sizeof(std::vector<int>::size_type)!= b", "!="));
      OK("<b", h2::find_op("sizeof(std::vector<int>::size_type)<b", "<"));
      OK("<=b", h2::find_op("sizeof(std::vector<int>::size_type) <=b", "<="));
      OK("> b", h2::find_op("sizeof(std::vector<int>::size_type)> b", ">"));
      OK(">=b", h2::find_op("sizeof(std::vector<int>::size_type)>=b", ">="));
   }
}
