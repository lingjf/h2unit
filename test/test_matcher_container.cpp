#include "../source/h2_unit.cpp"

#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <valarray>

SUITE(container matches)
{
   Case(Pair)
   {
      h2::h2_pair_matches<int, double> e1(1, 3.14);

      std::pair<int, double> a1 = std::make_pair(1, 3.14);
      std::pair<int, double> a2 = std::make_pair(2, 3.14);

      OK(IsNull, e1.matches(a1, 0, {}));
      OK(NotNull, e1.matches(a2, 0, {}));
   }

   Case(ListOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_equation<int>>;

      T1 a1_ = T1(h2::h2_equation<int>(1));
      T1 a2_ = T1(h2::h2_equation<int>(2));
      T1 a3_ = T1(h2::h2_equation<int>(3));

      h2::h2_listof_matches<T1, T1, T1> a(a1_, a2_, a3_);

      int b1[] = {1, 2, 3};
      OK(nullptr == a.matches(b1, 0, {}));

      std::vector<int> b2 = {1, 2, 3};
      OK(nullptr == a.matches(b2, 0, {}));
   }
}

SUITE(Pair)
{
   Case(native)
   {
      std::pair<int, double> a1 = std::make_pair(1, 3.14);
      OK(Pair(1, 3.14), a1);
      OK(!Pair(2, 3.14), a1);
   }

   Case(Ge)
   {
      std::pair<int, double> a1 = std::make_pair(1, 3.14);
      OK(Pair(Ge(0), 3.14), a1);
   }
}

SUITE(ListOf primitive)
{
   Case(C/C++ generic array)
   {
      int a1[] = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1, 3);
      OK(ListOf(1, 2, 3), a1);
   }

   Case(Sequence containers / array / static contiguous array)
   {
      std::array<int, 3> a1 = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1);
      OK(!ListOf(1, 2, 7), a1);
   }

   Case(Sequence containers / vector / dynamic contiguous array)
   {
      std::vector<int> a1 = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1);
      OK(!ListOf(1, 2, 7), a1);
   }

   Case(Sequence containers / deque / double-ended queue)
   {
      std::deque<int> a1 = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1);
      OK(!ListOf(1, 2, 7), a1);
   }

   Case(Sequence containers / forward_list / singly-linked list)
   {
      std::forward_list<int> a1 = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1);
      OK(!ListOf(1, 2, 7), a1);
   }

   Case(Sequence containers / list / doubly-linked list)
   {
      std::list<int> a1 = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1);
      OK(!ListOf(1, 2, 7), a1);
   }

   Case(Associative containers / set)
   {
      std::set<int> a1 = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1);
   }

   Case(Associative containers / multiset)
   {
      std::multiset<int> a1 = {1, 2, 3};
      OK(ListOf(1, 2, 3), a1);
   }

   Case(Associative containers / unordered_set)
   {
      std::unordered_set<int> a1 = {1, 2, 3};
      // OK(ListOf(1, 2, 3), a1);
   }

   Case(Associative containers / map)
   {
      std::map<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(ListOf(Pair(1, 111), Pair(2, 222), Pair(3, 333)), a1);
   }

   Case(Associative containers / multimap)
   {
      std::multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(ListOf(Pair(1, 111), Pair(2, 222), Pair(3, 333)), a1);
   }

   Case(Associative containers / unordered_multimap)
   {
      std::unordered_multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      // OK(ListOf(Pair(1, 111), Pair(2, 222), Pair(3, 333)), a1);
   }

   Case(empty)
   {
      std::array<int, 3> a1 = {1, 2, 3};
      OK(ListOf(), a1);
   }
}

SUITE(Has primitive)
{
   Case(C/C++ generic array)
   {
      int a1[] = {1, 2, 3};
      OK(Has(2), a1, 3);
      OK(Has(2), a1);
      OK(!Has(4), a1, 3);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / array / static contiguous array)
   {
      std::array<int, 3> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / vector / dynamic contiguous array)
   {
      std::vector<int> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / deque / double-ended queue)
   {
      std::deque<int> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / forward_list / singly-linked list)
   {
      std::forward_list<int> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / list / doubly-linked list)
   {
      std::list<int> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / set)
   {
      std::set<int> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / multiset)
   {
      std::multiset<int> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / unordered_set)
   {
      std::unordered_set<int> a1 = {1, 2, 3};
      OK(Has(2), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / map)
   {
      std::map<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(1, 111)), a1);
      OK(!Has(Pair(4, 444)), a1);
   }

   Case(Associative containers / multimap)
   {
      std::multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(3, 333)), a1);
      OK(!Has(Pair(4, 444)), a1);
   }

   Case(Associative containers / unordered_multimap)
   {
      std::unordered_multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(3, 333)), a1);
      OK(!Has(Pair(4, 444)), a1);
   }
}

SUITE(CountOf primitive)
{
   Case(native array)
   {
      int a1[] = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Sequence containers / array / static contiguous array)
   {
      std::array<int, 3> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
      OK(CountOf(Gt(2) && Lt(4)), a1);
   }

   Case(Sequence containers / vector / dynamic contiguous array)
   {
      std::vector<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Sequence containers / deque / double-ended queue)
   {
      std::deque<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Sequence containers / forward_list / singly-linked list)
   {
      std::forward_list<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Sequence containers / list / doubly-linked list)
   {
      std::list<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / set)
   {
      std::set<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / multiset)
   {
      std::multiset<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / unordered_set)
   {
      std::unordered_set<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / map)
   {
      std::map<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / multimap)
   {
      std::multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / unordered_multimap)
   {
      std::unordered_multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(CountOf(3), a1);
   }
}

SUITE(container bool problem)
{
   // https://isocpp.org/blog/2012/11/on-vectorbool

   Case(C/C++ generic array)
   {
      bool a1[] = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(vector)
   {
      std::vector<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(array)
   {
      std::array<bool, 3> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(deque)
   {
      std::deque<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(forward_list)
   {
      std::forward_list<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(list)
   {
      std::list<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }
}
