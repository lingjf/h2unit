#include "../source/h2_unit.cpp"

SUITE(ListOf)
{
   Case(only one not same)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, 3), a);
   }
   Case(two not same)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, _), a);
   }
   Case(only one miss)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 2, 3, 4), a);
   }
   Case(Not)
   {
      std::vector<int> a = {1, 2, 3};
      OK(!ListOf(1, 2, 3), a);
   }

   Case(C / C++ native array)
   {
      int a1[] = {1, 2, 3};
      OK(ListOf(1, 7, 3), a1);
   }
}

SUITE(Has)
{
   Case(miss only one)
   {
      std::vector<int> a = {1, 2, 3};
      OK(Has(8), a);
   }
   Case(miss two)
   {
      std::vector<int> a = {1, 2, 3};
      OK(Has(8, Ge(9)), a);
   }
   Case(miss two)
   {
      std::vector<int> a = {1, 2, 3};
      OK(!Has(2), a);
   }
}

SUITE(In)
{
   Case(In)
   {
      OK(In(1, Le(2), Pair(2, 3)), 5);
   }

   Case(In)
   {
      std::pair<int, std::string> a1 = std::make_pair(9, std::string("nine"));
      OK(In(Pair(1, "one"), Pair(2, "two")), a1);
   }
}