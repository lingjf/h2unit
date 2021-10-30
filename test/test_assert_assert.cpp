#include "../source/h2_unit.cpp"

#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>

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

   Case(no space)
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

   Case(with n)
   {
      int a[3] = {1, 2, 3};
      // OK(Has(1), a, 3);
      // OK(decltype(a, 3) == decltype(a));
      OK(!std::is_array<decltype(a, 3)>::value);

      int* b = a;
      // OK(Has(1), b, 3);
      // OK(decltype(b, 3) == decltype(b));
      OK(!std::is_array<decltype(b, 3)>::value);
   }

   Case(type)
   {
      int a1[] = {1, 2, 3};
      OK("True", ft(std::is_array<decltype(a1)>{}));

      int a2 = 123;
      OK("False", ft(std::is_array<decltype(a2)>{}));
   }
}

static int c0 = 0;
static int get0()
{
   ++c0;
   return 1;
}

static int c1 = 0;
static int get1()
{
   ++c1;
   return 1;
}

SUITE(nocall)
{
   Case(decltype)
   {
      decltype(get0()) b = 0;
      OK(0, c0);
   }

   Case(actual call one time)
   {
      OK(1, get1());
      OK(1, c1);
   }
}
