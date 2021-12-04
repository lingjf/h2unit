#if defined __clang__
#pragma GCC diagnostic ignored "-Wrange-loop-analysis"
#endif

#include "../source/h2_unit.cpp"
#include "test_cplusplus.hpp"

SUITE(h2_pair_matches)
{
   Case(int, double)
   {
      h2::h2_pair_matches<int, double> m1(1, 3.14);

      std::pair<int, double> a1 = std::make_pair(1, 3.14);
      std::pair<int, double> a2 = std::make_pair(2, 3.14);

      OK(nullptr, m1.matches(a1, {}));
      OK(Not(NULL), m1.matches(a2, {}));
   }

   Case(string, double)
   {
      h2::h2_pair_matches<std::string, double> m1("a", 3.14);

      std::pair<std::string, double> a1 = std::make_pair("a", 3.14);
      std::pair<std::string, double> a2 = std::make_pair("b", 3.14);

      OK(nullptr, m1.matches(a1, {}));
      OK(Not(NULL), m1.matches(a2, {}));
   }
}

SUITE(h2_is_pair_matches)
{
   Case(h2_pair_matches)
   {
      OK((h2::h2_is_pair_matches<h2::h2_pair_matches<int, double>>::value));

      h2::h2_pair_matches<int, double> m1(1, 3.14);
      OK((h2::h2_is_pair_matches<decltype(m1)>::value));
   }

   Case(Fundamental types)
   {
      OK(!(h2::h2_is_pair_matches<int>::value));
   }

   Case(Sequence containers)
   {
      OK(!(h2::h2_is_pair_matches<std::array<int, 8>>::value));
      OK(!(h2::h2_is_pair_matches<std::vector<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::deque<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::list<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::forward_list<int>>::value));

      OK(!(h2::h2_is_pair_matches<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK(!(h2::h2_is_pair_matches<std::set<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::multiset<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::unordered_set<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::unordered_multiset<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::map<int, int>>::value));
      OK(!(h2::h2_is_pair_matches<std::multimap<int, int>>::value));
      OK(!(h2::h2_is_pair_matches<std::unordered_map<int, int>>::value));
      OK(!(h2::h2_is_pair_matches<std::unordered_multimap<int, int>>::value));
   }

   Case(Container adaptors)
   {
      OK(!(h2::h2_is_pair_matches<std::stack<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::queue<int>>::value));
      OK(!(h2::h2_is_pair_matches<std::priority_queue<int>>::value));
   }

   Case(string)
   {
      OK(!(h2::h2_is_pair_matches<std::string>::value));
      OK(!(h2::h2_is_pair_matches<h2::h2_string>::value));
   }

   Case(valarray)
   {
      OK(!(h2::h2_is_pair_matches<std::valarray<int>>::value));
   }
}

SUITE(h2_is_polymorphic_matcher_pair_matches)
{
   Case(Pair)
   {
      auto m1 = Pair(1, 2.0);
      OK((h2::h2_is_polymorphic_matcher_pair_matches<decltype(m1)>::value));
   }

   Case(Any)
   {
      OK(!(h2::h2_is_polymorphic_matcher_pair_matches<decltype(_)>::value));
   }

   Case(Eq)
   {
      OK(!(h2::h2_is_polymorphic_matcher_pair_matches<decltype(Eq(1))>::value));
   }

   Case(Gt)
   {
      OK(!(h2::h2_is_polymorphic_matcher_pair_matches<decltype(Gt(1.0))>::value));
   }

   Case(ListOf)
   {
      auto m1 = ListOf(1, 2.0);
      OK(!(h2::h2_is_polymorphic_matcher_pair_matches<decltype(m1)>::value));
   }

   Case(Fundamental types)
   {
      OK(!(h2::h2_is_polymorphic_matcher_pair_matches<int>::value));
   }
}

SUITE(Pair Primitive [api])
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

SUITE(h2_listof_matches)
{
   Case(eq)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_equation<int>>;

      T1 a1_ = T1(h2::h2_equation<int>(1));
      T1 a2_ = T1(h2::h2_equation<int>(2));
      T1 a3_ = T1(h2::h2_equation<int>(3));

      h2::h2_listof_matches<T1, T1, T1> a(a1_, a2_, a3_);

      int b1[] = {1, 2, 3};
      OK(nullptr == a.matches(b1, {}));

      std::vector<int> b2 = {1, 2, 3};
      OK(nullptr == a.matches(b2, {}));
   }
}

SUITE(ListOf primitive [api])
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

   Case(Container adaptors / stack)
   {
      std::stack<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(ListOf(1, 2, 3), a1);
   }

   Case(Container adaptors / queue)
   {
      std::queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(ListOf(1, 2, 3), a1);
   }

   Case(Container adaptors / priority_queue)
   {
      std::priority_queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(ListOf(1, 2, 3) || ListOf(1, 3, 2) || ListOf(2, 1, 3) || ListOf(2, 3, 1) || ListOf(3, 1, 2) || ListOf(3, 2, 1), a1);
   }

   Case(string)
   {
      std::string a1 = "abc";
      OK(ListOf('a', 'b', 'c'), a1);
   }

   Case(empty)
   {
      std::array<int, 3> a1 = {1, 2, 3};
      OK(ListOf(), a1);
   }
}

SUITE(h2_has1_matches)
{
   Case()
   {
   }
}

SUITE(h2_has2_matches)
{
   Case()
   {
   }
}

SUITE(Has primitive [api])
{
   Case(C/C++ generic array)
   {
      int a1[] = {1, 2, 3};
      OK(Has(1), a1, 3);
      OK(Has(1), a1);
      OK(Has(2), a1, 3);
      OK(Has(2), a1);
      OK(Has(3), a1, 3);
      OK(Has(3), a1);
      OK(!Has(4), a1, 3);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / array / static contiguous array)
   {
      std::array<int, 3> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / vector / dynamic contiguous array)
   {
      std::vector<int> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / deque / double-ended queue)
   {
      std::deque<int> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / forward_list / singly-linked list)
   {
      std::forward_list<int> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Sequence containers / list / doubly-linked list)
   {
      std::list<int> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / set)
   {
      std::set<int> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / multiset)
   {
      std::multiset<int> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / unordered_set)
   {
      std::unordered_set<int> a1 = {1, 2, 3};
      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / map)
   {
      std::map<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(1, 111)), a1);
      OK(Has(Pair(2, 222)), a1);
      OK(Has(Pair(3, 333)), a1);
      OK(!Has(Pair(1, 444)), a1);
      OK(!Has(Pair(4, 111)), a1);
      OK(!Has(Pair(4, 444)), a1);

      OK(Has(1, 111), a1);
      OK(Has(2, 222), a1);
      OK(Has(3, 333), a1);
      OK(!Has(1, 444), a1);
      OK(!Has(4, 111), a1);
      OK(!Has(4, 444), a1);

      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / multimap)
   {
      std::multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(1, 111)), a1);
      OK(Has(Pair(2, 222)), a1);
      OK(Has(Pair(3, 333)), a1);
      OK(!Has(Pair(1, 444)), a1);
      OK(!Has(Pair(4, 111)), a1);
      OK(!Has(Pair(4, 444)), a1);

      OK(Has(1, 111), a1);
      OK(Has(2, 222), a1);
      OK(Has(3, 333), a1);
      OK(!Has(1, 444), a1);
      OK(!Has(4, 111), a1);
      OK(!Has(4, 444), a1);

      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Associative containers / unordered_multimap)
   {
      std::unordered_multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(1, 111)), a1);
      OK(Has(Pair(2, 222)), a1);
      OK(Has(Pair(3, 333)), a1);
      OK(!Has(Pair(1, 444)), a1);
      OK(!Has(Pair(4, 111)), a1);
      OK(!Has(Pair(4, 444)), a1);

      OK(Has(1, 111), a1);
      OK(Has(2, 222), a1);
      OK(Has(3, 333), a1);
      OK(!Has(1, 444), a1);
      OK(!Has(4, 111), a1);
      OK(!Has(4, 444), a1);

      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Container adaptors / stack)
   {
      std::stack<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Container adaptors / queue)
   {
      std::queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(Container adaptors / priority_queue)
   {
      std::priority_queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(Has(1), a1);
      OK(Has(2), a1);
      OK(Has(3), a1);
      OK(!Has(4), a1);
   }

   Case(string)
   {
      std::string a1 = "abc";
      OK(Has('a'), a1);
      OK(Has('b'), a1);
      OK(Has('c'), a1);
      OK(!Has('d'), a1);
   }
}

SUITE(HasKey HasValue primitive)
{
   Case(Associative containers / map)
   {
      std::map<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(HasKey(1), a1);
      OK(!HasKey(4), a1);

      OK(HasValue(111), a1);
      OK(!HasValue(444), a1);
   }

   Case(Associative containers / multimap)
   {
      std::multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(HasKey(3), a1);
      OK(!HasKey(4), a1);

      OK(HasValue(222), a1);
      OK(!HasValue(444), a1);
   }

   Case(Associative containers / unordered_multimap)
   {
      std::unordered_multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(HasKey(3), a1);
      OK(!HasKey(4), a1);

      OK(HasValue(333), a1);
      OK(!HasValue(444), a1);
   }
}

SUITE(h2_countof_matches)
{
   Case()
   {
   }
}

SUITE(CountOf primitive [api])
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
      std::vector<int> a0;
      OK(CountOf(0), a0);
      std::vector<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Sequence containers / deque / double-ended queue)
   {
      std::deque<int> a0;
      OK(CountOf(0), a0);
      std::deque<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Sequence containers / forward_list / singly-linked list)
   {
      std::forward_list<int> a0;
      OK(CountOf(0), a0);
      std::forward_list<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Sequence containers / list / doubly-linked list)
   {
      std::list<int> a0;
      OK(CountOf(0), a0);
      std::list<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / set)
   {
      std::set<int> a0;
      OK(CountOf(0), a0);
      std::set<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / multiset)
   {
      std::multiset<int> a0;
      OK(CountOf(0), a0);
      std::multiset<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / unordered_set)
   {
      std::unordered_set<int> a0;
      OK(CountOf(0), a0);
      std::unordered_set<int> a1 = {1, 2, 3};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / map)
   {
      std::map<int, int> a0;
      OK(CountOf(0), a0);
      std::map<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / multimap)
   {
      std::multimap<int, int> a0;
      OK(CountOf(0), a0);
      std::multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(CountOf(3), a1);
   }

   Case(Associative containers / unordered_multimap)
   {
      std::unordered_multimap<int, int> a0;
      OK(CountOf(0), a0);
      std::unordered_multimap<int, int> a1 = {{1, 111}, {2, 222}, {3, 333}};
      OK(CountOf(3), a1);
   }

   Case(Container adaptors / stack)
   {
      std::stack<int> a1;
      OK(CountOf(0), a1);

      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(CountOf(3), a1);
   }

   Case(Container adaptors / queue)
   {
      std::queue<int> a1;
      OK(CountOf(0), a1);

      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(CountOf(3), a1);
   }

   Case(Container adaptors / priority_queue)
   {
      std::priority_queue<int> a1;
      OK(CountOf(0), a1);

      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(CountOf(3), a1);
   }

   Case(string)
   {
      std::string a0 = "";
      OK(CountOf(0), a0);

      std::string a1 = "abc";
      OK(CountOf(3), a1);

      h2::h2_string a2 = "abc";
      OK(CountOf(3), a2);
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
