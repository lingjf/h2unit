#include "../source/h2_unit.cpp"
#include "test_types.hpp"

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

CASE(nth_type)
{
   OK((std::is_same<char, typename h2::h2_nth_type<0, char, short>::type>::value));
   OK((std::is_same<short, typename h2::h2_nth_type<1, char, short>::type>::value));
}

SUITE(sizeof pointee)
{
   Case(char*)
   {
      OK(1, h2::h2_sizeof_pointee<char*>::value);
   }

   Case(unsigned short*)
   {
      OK(2, h2::h2_sizeof_pointee<unsigned short*>::value);
   }

   Case(unsigned long long*)
   {
      OK(8, h2::h2_sizeof_pointee<unsigned long long*>::value);
   }

   Case(B_ClassStruct*)
   {
      OK(sizeof(B_ClassStruct), h2::h2_sizeof_pointee<B_ClassStruct*>::value);
   }

   Case(void*)
   {
      OK(1, h2::h2_sizeof_pointee<void*>::value);
   }

   Case(void**)
   {
      OK(sizeof(void*), h2::h2_sizeof_pointee<void**>::value);
   }
}

SUITE(pair)
{
   Case(direct)
   {
      OK((!h2::h2_is_pair<int>::value));
      OK((h2::h2_is_pair<std::pair<int, int>>::value));
      OK((h2::h2_is_pair<std::pair<int, std::string>>::value));
   }

   Case(value_type)
   {
      std::map<std::string, int> a1 = {{std::string("th1"), 1},
                                       {std::string("th2"), 2},
                                       {std::string("th3"), 3}};
      OK((h2::h2_is_pair<typename std::decay<decltype(*a1.begin())>::type>::value));
      OK((h2::h2_is_pair<decltype(a1)::value_type>::value));
      OK((h2::h2_is_pair<std::map<std::string, int>::value_type>::value));

      const std::map<std::string, int>& a2 = a1;
      OK((h2::h2_is_pair<typename std::decay<decltype(*a2.begin())>::type>::value));
      // OK((h2::h2_is_pair<decltype(a2)::value_type>::value));
      // OK((h2::h2_is_pair<std::map<std::string, int>::value_type>::value));
   }
}

SUITE(container)
{
   Case(is_container)
   {
      OK((!h2::h2_is_container<int>::value));
      OK((h2::h2_is_container<std::array<int, 8>>::value));
      OK((h2::h2_is_container<std::vector<int>>::value));
      OK((h2::h2_is_container<std::deque<int>>::value));
      OK((h2::h2_is_container<std::list<int>>::value));
      OK((h2::h2_is_container<std::forward_list<int>>::value));
      OK((h2::h2_is_container<std::set<int>>::value));
      OK((h2::h2_is_container<std::multiset<int>>::value));
      OK((h2::h2_is_container<std::unordered_set<int>>::value));
      OK((h2::h2_is_container<std::unordered_multiset<int>>::value));
      OK((h2::h2_is_container<std::map<int, int>>::value));
      OK((h2::h2_is_container<std::multimap<int, int>>::value));
      OK((h2::h2_is_container<std::unordered_map<int, int>>::value));
      OK((h2::h2_is_container<std::unordered_multimap<int, int>>::value));

      OK((!h2::h2_is_container<std::valarray<int>>::value));

      OK((h2::h2_is_container<h2::h2_vector<int>>::value));

      OK((h2::h2_is_container<std::string>::value));
      OK((h2::h2_is_container<h2::h2_string>::value));
   }
}
