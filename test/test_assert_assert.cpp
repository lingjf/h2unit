#include "../source/h2_unit.cpp"
#include "test_cplusplus.hpp"

SUITE(find_compare)
{
   Case(simple)
   {
      OK("== b", h2::__find_compare("a == b", "=="));
      OK("!= b", h2::__find_compare("a != b", "!="));
      OK("< b", h2::__find_compare("a < b", "<"));
      OK("<= b", h2::__find_compare("a <= b", "<="));
      OK("> b", h2::__find_compare("a > b", ">"));
      OK(">= b", h2::__find_compare("a >= b", ">="));
   }

   Case(no space)
   {
      OK("==b", h2::__find_compare("a ==b", "=="));
      OK("!= b", h2::__find_compare("a!= b", "!="));
      OK("<b", h2::__find_compare("a<b", "<"));
      OK("<=b", h2::__find_compare("a <=b", "<="));
      OK("> b", h2::__find_compare("a> b", ">"));
      OK(">=b", h2::__find_compare("a>=b", ">="));
   }

   Case(in string)
   {
      OK("==b", h2::__find_compare("\"a==\" ==b", "=="));
      OK("!= b", h2::__find_compare("\"a!=\"!= b", "!="));
      OK("<b", h2::__find_compare("\"a<\"<b", "<"));
      OK("<=b", h2::__find_compare("\"a<=\" <=b", "<="));
      OK("> b", h2::__find_compare("\"a>\"> b", ">"));
      OK(">=b", h2::__find_compare("\"a>=\">=b", ">="));
   }

   Case(has template)
   {
      OK("== b", h2::__find_compare("sizeof(std::vector<int>::size_type) == b", "=="));
      OK("!= b", h2::__find_compare("sizeof(std::vector<int>::size_type)!= b", "!="));
      OK("<b", h2::__find_compare("sizeof(std::vector<int>::size_type)<b", "<"));
      OK("<=b", h2::__find_compare("sizeof(std::vector<int>::size_type) <=b", "<="));
      OK("> b", h2::__find_compare("sizeof(std::vector<int>::size_type)> b", ">"));
      OK(">=b", h2::__find_compare("sizeof(std::vector<int>::size_type)>=b", ">="));
   }
}

CASE(split_compare)
{
   h2::h2_string eexpr, aexpr;

   h2::__split_compare("a1 == b1", "==", eexpr, aexpr);
   OK("a1", eexpr);
   OK("b1", aexpr);

   h2::__split_compare("a2!= b2", "!=", eexpr, aexpr);
   OK("a2", eexpr);
   OK("b2", aexpr);

   h2::__split_compare("a3 <b3", "<", eexpr, aexpr);
   OK("a3", eexpr);
   OK("b3", aexpr);

   h2::__split_compare("a4<=b4", "<=", eexpr, aexpr);
   OK("a4", eexpr);
   OK("b4", aexpr);

   h2::__split_compare("a5 \t> \n b5", ">", eexpr, aexpr);
   OK("a5", eexpr);
   OK("b5", aexpr);

   h2::__split_compare("a6 >= b6", ">=", eexpr, aexpr);
   OK("a6", eexpr);
   OK("b6", aexpr);
}

static const char* ft(std::true_type)
{
   return "True";
}
static const char* ft(std::false_type)
{
   return "False";
}

SUITE(array detect)
{
   Case(native array)
   {
      int a[3] = {1, 2, 3};
      OK(std::is_array<decltype(a)>::value);
      OK(3, std::extent<decltype(a)>::value);

      int* b = a;
      OK(!std::is_array<decltype(b)>::value);
   }

   Case(std vector)
   {
      OK(!std::is_array<std::vector<int>>::value);
   }

   Case(std deque)
   {
      OK(!std::is_array<std::deque<int>>::value);
   }

   Case(std array)
   {
      OK(!(std::is_array<std::array<int, 3>>::value));
   }

   Case(std list)
   {
      OK(!std::is_array<std::list<int>>::value);
   }

   Case(std forward_list)
   {
      OK(!std::is_array<std::forward_list<int>>::value);
   }

   Case(type)
   {
      int a1[] = {1, 2, 3};
      OK("True", ft(std::is_array<decltype(a1)>{}));

      int a2 = 123;
      OK("False", ft(std::is_array<decltype(a2)>{}));

      OK("False", ft(std::is_array<std::array<int, 3>>{}));
   }
}

static int c1 = 0;
static int get1()
{
   ++c1;
   return 1;
}

SUITE(nocall in decltype)
{
   Case(decltype)
   {
      c1 = 0;
      decltype(get1()) b = 0;
      OK(0, c1);
      OK(0, b);
   }

   Case(actual call one time)
   {
      c1 = 0;
      OK(1, get1());
      OK(1, c1);
   }
}
