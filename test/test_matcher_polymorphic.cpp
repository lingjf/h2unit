#include "../source/h2_unit.cpp"
#include "test_cplusplus.hpp"

SUITE(h2_is_polymorphic_matcher)
{
   Case(polymorphic_matcher)
   {
      OK((h2::h2_is_polymorphic_matcher<decltype(_)>::value));
   }

   Case(Fundamental types)
   {
      OK(!(h2::h2_is_polymorphic_matcher<int>::value));
   }

   Case(Sequence containers)
   {
      OK(!(h2::h2_is_polymorphic_matcher<std::array<int, 8>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::vector<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::deque<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::list<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::forward_list<int>>::value));

      OK(!(h2::h2_is_polymorphic_matcher<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK(!(h2::h2_is_polymorphic_matcher<std::set<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::multiset<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::unordered_set<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::unordered_multiset<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::map<int, int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::multimap<int, int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::unordered_map<int, int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::unordered_multimap<int, int>>::value));
   }

   Case(Container adaptors)
   {
      OK(!(h2::h2_is_polymorphic_matcher<std::stack<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::queue<int>>::value));
      OK(!(h2::h2_is_polymorphic_matcher<std::priority_queue<int>>::value));
   }

   Case(string)
   {
      OK(!(h2::h2_is_polymorphic_matcher<std::string>::value));
      OK(!(h2::h2_is_polymorphic_matcher<h2::h2_string>::value));
   }

   Case(valarray)
   {
      OK(!(h2::h2_is_polymorphic_matcher<std::valarray<int>>::value));
   }
}

SUITE(h2_polymorphic_matcher)
{
   h2::h2_equation<int> a__(65);
   h2::h2_polymorphic_matcher<h2::h2_equation<int>> a(a__);

   Case(int)
   {
      h2::h2_matcher<int> b(a);
      OK(nullptr == b.matches(65));
   }

   Case(reverse)
   {
      h2::h2_matcher<int> b(!a);
      OK(nullptr != b.matches(65));
   }

   Case(case insensitive)
   {
      ~a;
      OK(a.case_insensitive);
   }

   Case(squash whitespace)
   {
      *a;
      OK(a.squash_whitespace);
   }
}
