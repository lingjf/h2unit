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

struct Tpl1 {
   int a;
   int bar(int) { return 0; }
};

struct Tpl2 {
   h2::h2_string toString() { return "Tpl2"; }
};

struct Tpl3 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, Tpl3 a)
{
   return os << "Tpl3";
}

struct Tpl4 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, Tpl4& a)
{
   return os << "Tpl4";
}

struct Tpl5 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, const Tpl5& a)
{
   return os << "Tpl5";
}

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

   Case(Tpl1*)
   {
      OK(4, h2::h2_sizeof_pointee<Tpl1*>::value);
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

SUITE(stringable)
{
   Case(is_ostreamable)
   {
      OK(h2::h2_is_ostreamable<int>::value);
      OK(h2::h2_is_ostreamable<unsigned char>::value);
      OK(h2::h2_is_ostreamable<uint8_t>::value);
      OK(!h2::h2_is_ostreamable<Tpl1>::value);
      OK(!h2::h2_is_ostreamable<Tpl2>::value);
      OK(h2::h2_is_ostreamable<Tpl3>::value);
      OK(h2::h2_is_ostreamable<Tpl4>::value);
      OK(h2::h2_is_ostreamable<Tpl5>::value);
   }

   Case(nullptr_t)
   {
#if defined __clang__ && __clang_major__ >= 12 
      // OK(h2::h2_is_ostreamable<std::nullptr_t>::value);
#else
      OK(!h2::h2_is_ostreamable<std::nullptr_t>::value);
#endif
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
