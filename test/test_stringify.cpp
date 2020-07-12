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

namespace {

struct Foo1 {
   int bar(int) { return 0; }
};

struct Foo2 {
   h2::h2_string toString() { return "Foo2"; }
};

struct Foo3 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, Foo3 a)
{
   return os << "Foo3";
}

struct Foo4 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, Foo4& a)
{
   return os << "Foo4";
}

struct Foo5 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, const Foo5& a)
{
   return os << "Foo5";
}

SUITE(stringable)
{
   Case(is_ostreamable)
   {
      OK(h2::h2_is_ostreamable<int>::value);
      OK(!h2::h2_is_ostreamable<std::nullptr_t>::value);
      OK(!h2::h2_is_ostreamable<Foo1>::value);
      OK(!h2::h2_is_ostreamable<Foo2>::value);
      OK(h2::h2_is_ostreamable<Foo3>::value);
      OK(h2::h2_is_ostreamable<Foo4>::value);
      OK(h2::h2_is_ostreamable<Foo5>::value);
   }

   Case(is_ostreamable ex)
   {
      OK(h2::h2_is_ostreamable<unsigned char>::value);
      OK(h2::h2_is_ostreamable<uint8_t>::value);

      OK(!h2::h2_is_ostreamable<std::nullptr_t>::value);
   }
}

SUITE(pair)
{
   Case(direct)
   {
      OK(!h2::h2_is_pair<int>::value);
      OK(h2::h2_is_pair<std::pair<int, int>>::value);
      OK(h2::h2_is_pair<std::pair<int, std::string>>::value);
   }

   Case(value_type)
   {
      std::map<std::string, int> a1 = {{std::string("th1"), 1},
                                       {std::string("th2"), 2},
                                       {std::string("th3"), 3}};
      OK(h2::h2_is_pair<typename std::decay<decltype(*a1.begin())>::type>::value);
      OK(h2::h2_is_pair<decltype(a1)::value_type>::value);
      OK(h2::h2_is_pair<std::map<std::string, int>::value_type>::value);

      const std::map<std::string, int>& a2 = a1;
      OK(h2::h2_is_pair<typename std::decay<decltype(*a2.begin())>::type>::value);
      // OK(h2::h2_is_pair<decltype(a2)::value_type>::value);
      // OK(h2::h2_is_pair<std::map<std::string, int>::value_type>::value);
   }
}

SUITE(container)
{
   Case(is_container)
   {
      OK(!h2::h2_is_container<int>::value);
      OK(h2::h2_is_container<std::array<int, 8>>::value);
      OK(h2::h2_is_container<std::vector<int>>::value);
      OK(h2::h2_is_container<std::deque<int>>::value);
      OK(h2::h2_is_container<std::list<int>>::value);
      OK(h2::h2_is_container<std::forward_list<int>>::value);
      OK(h2::h2_is_container<std::set<int>>::value);
      OK(h2::h2_is_container<std::multiset<int>>::value);
      OK(h2::h2_is_container<std::unordered_set<int>>::value);
      OK(h2::h2_is_container<std::unordered_multiset<int>>::value);
      OK(h2::h2_is_container<std::map<int, int>>::value);
      OK(h2::h2_is_container<std::multimap<int, int>>::value);
      OK(h2::h2_is_container<std::unordered_map<int, int>>::value);
      OK(h2::h2_is_container<std::unordered_multimap<int, int>>::value);

      OK(!h2::h2_is_container<std::valarray<int>>::value);

      OK(h2::h2_is_container<h2::h2_vector<int>>::value);

      OK(h2::h2_is_container<std::string>::value);
      OK(h2::h2_is_container<h2::h2_string>::value);
   }
}

SUITE(stringify simple)
{
   Case(int)
   {
      OK("1", h2::h2_stringify<int>(1));
   }

   Case(double)
   {
      OK("1.41421", h2::h2_stringify<double>(sqrt(2)));
   }

   Case(bool)
   {
      OK("true", h2::h2_stringify<bool>(true));
      OK("false", h2::h2_stringify<bool>(false));
   }

   Case(long long)
   {
      long long a = 12345678;
      OK("12345678", h2::h2_stringify<long long>(a));
   }

   Case(char*)
   {
      char* a = "hello world";
      OK("hello world", h2::h2_stringify<char*>(a));
   }

   Case(unsigned char*)
   {
      unsigned char a[] = {'h', 'e', 'l', 'l', 'o', 0};
      OK("hello", h2::h2_stringify<unsigned char*>(a));
   }

   Case(void*)
   {
      void* a = (void*)0x12345678;
      OK("0x12345678", h2::h2_stringify<void*>(a));
   }

   // Case(NULL)
   // {
   //    OK("0", h2::h2_stringify<decltype(NULL)>(NULL));
   // }

   Case(nullptr)
   {
      OK("nullptr", h2::h2_stringify<std::nullptr_t>(nullptr));
   }

   // https://en.cppreference.com/w/cpp/string/byte/isprint
   Case(uint8_t)
   {
      unsigned char a1 = 7;
      OK("7", h2::h2_stringify<unsigned char>(a1));
      uint8_t a2 = 7;
      OK("7", h2::h2_stringify<uint8_t>(a2));
   }
}

SUITE(stringify user)
{
   Case(toString)
   {
      Foo2 f2;
      OK("Foo2", h2::h2_stringify<Foo2>(f2));
   }

   Case(operator<<)
   {
      Foo3 f3;
      OK("Foo3", h2::h2_stringify<Foo3>(f3));
   }
}

SUITE(stringify complex)
{
   Case(pair)
   {
      std::pair<int, std::string> a1 = std::make_pair(9, std::string("nine"));
      OK("(9, nine)", h2::h2_stringify<std::pair<int, std::string>>(a1));

      std::pair<std::string, int> a2 = std::make_pair(std::string("nine"), 9);
      OK("(nine, 9)", h2::h2_stringify<std::pair<std::string, int>>(a2));
   }

   Case(tuple)
   {
      std::tuple<> a0;
      std::tuple<int> a1 = std::make_tuple(42);
      std::tuple<const char*, int> a2 = std::make_tuple("nine", 9);
      std::tuple<const char*, int, std::pair<std::string, double>> a3 = std::make_tuple("nine", 9, std::make_pair(std::string("pai"), 3.14));

      OK("()", h2::h2_stringify<std::tuple<>>(a0));
      OK("(42)", h2::h2_stringify<std::tuple<int>>(a1));
      OK("(nine, 9)", h2::h2_stringify<std::tuple<const char*, int>>(a2));
      OK("(nine, 9, (pai, 3.14))", h2::h2_stringify<std::tuple<const char*, int, std::pair<std::string, double>>>(a3));
   }
}

// https://en.cppreference.com/w/cpp/container

SUITE(stringify Sequence containers)
{
   // Sequence containers can be accessed sequentially.

   //    array static contiguous array
   //    vector dynamic contiguous array
   //    deque double-ended queue
   //    forward_list singly-linked list
   //    list doubly-linked list

   Case(array)
   {
      std::array<int, 3> a1 = {1, 2, 3};
      OK("[1, 2, 3]", h2::h2_stringify(a1));
   }

   Case(vector)
   {
      std::vector<int> a1 = {1, 2, 3};
      OK("[1, 2, 3]", h2::h2_stringify<std::vector<int>>(a1));
   }

   Case(deque)
   {
      std::deque<int> a1 = {1, 2, 3};
      OK("[1, 2, 3]", h2::h2_stringify<std::deque<int>>(a1));
   }

   Case(forward_list)
   {
      std::forward_list<int> a1 = {1, 2, 3};
      OK("[1, 2, 3]", h2::h2_stringify<std::forward_list<int>>(a1));
   }

   Case(list)
   {
      std::list<int> a1 = {1, 2, 3};
      OK("[1, 2, 3]", h2::h2_stringify<std::list<int>>(a1));
   }
}

SUITE(stringify Associative containers)
{
   // Associative containers can be quickly searched (O(log n) complexity).

   //    set
   //    map
   //    multiset
   //    multimap

   Case(set)
   {
      std::set<bool> a1 = {false, true};
      OK("[false, true]", h2::h2_stringify<std::set<bool>>(a1));
   }

   Case(map)
   {
      std::map<std::string, int> a1 = {{std::string("th1"), 1},
                                       {std::string("th2"), 2},
                                       {std::string("th3"), 3},
                                       {std::string("th3"), -3}};

      OK("[(th1, 1), (th2, 2), (th3, 3)]", h2::h2_stringify<std::map<std::string, int>>(a1));
   }

   Case(multiset)
   {
      std::multiset<bool> a1 = {false, false, true};
      OK("[false, false, true]", h2::h2_stringify<std::multiset<bool>>(a1));
   }

   Case(multimap)
   {
      std::multimap<std::string, int> a1 = {{std::string("th1"), 1},
                                            {std::string("th2"), 2},
                                            {std::string("th3"), 3},
                                            {std::string("th3"), -3}};

      OK("[(th1, 1), (th2, 2), (th3, 3), (th3, -3)]", h2::h2_stringify<std::multimap<std::string, int>>(a1));
   }
}

SUITE(stringify Unordered Associative containers)
{
   // Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched (O(1) amortized, O(n) worst-case complexity).

   //    unordered_set
   //    unordered_map
   //    unordered_multiset
   //    unordered_multimap

   Case(unordered_set)
   {
      std::unordered_set<bool> a1 = {false, true, true};
      OK(AnyOf("[false, true]", "[true, false]"), h2::h2_stringify<std::unordered_set<bool>>(a1));
   }

   Case(unordered_multiset)
   {
      std::unordered_multiset<bool> a1 = {false, true, true};
      OK(AnyOf("[false, true, true]", "[true, true, false]"), h2::h2_stringify<std::unordered_multiset<bool>>(a1));
   }

   Case(unordered_map)
   {
      std::unordered_map<std::string, int> a1 = {{std::string("th1"), 1},
                                                 {std::string("th2"), 2}};

      OK(AnyOf("[(th1, 1), (th2, 2)]", "[(th2, 2), (th1, 1)]"), h2::h2_stringify<std::unordered_map<std::string, int>>(a1));
   }

   Case(unordered_multimap)
   {
      std::unordered_multimap<std::string, int> a1 = {{std::string("th1"), 1},
                                                      {std::string("th2"), 2}};

      OK(AnyOf("[(th1, 1), (th2, 2)]", "[(th2, 2), (th1, 1)]"), h2::h2_stringify<std::unordered_multimap<std::string, int>>(a1));
   }
}

SUITE(array with count)
{
   Case(int)
   {
      int a1[] = {1, 2, 3};
      OK("[1, 2, 3]", h2::h2_stringify(a1, 3));
   }

   Case(valarray)
   {
      std::valarray<int> a1(3);
      a1[0] = 1;
      a1[1] = 2;
      a1[2] = 3;
      OK("[1, 2, 3]", h2::h2_stringify(a1, 3));
   }
}

}  // namespace
