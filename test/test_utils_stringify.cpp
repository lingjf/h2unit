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

struct Stringify1 {
   int bar(int) { return 0; }
};

struct Stringify2 {
   h2::h2_string tostring() { return "tostring"; }
   h2::h2_string toString() { return "toString"; }
};

struct Stringify3 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, Stringify3 a)
{
   return os << "Stringify3";
}

struct Stringify4 {
   std::string tostring() { return "std::string toString()"; }
   h2::h2_string toString() { return "h2::h2_string toString"; }
   std::string Tostring(int) { return "std::string Tostring"; }
   int ToString() { return 0; }
   const char *to_string() { return "const char* to_string"; }
};

CASE(tostring able)
{
   OK(h2::h2_toString_able<Stringify2>::value);

   OK(h2::h2_tostring_able<Stringify4>::value);
   OK(h2::h2_toString_able<Stringify4>::value);
   OK(!h2::h2_Tostring_able<Stringify4>::value);
   OK(!h2::h2_ToString_able<Stringify4>::value);
   OK(h2::h2_to_string_able<Stringify4>::value);

   OK(!h2::h2_tostring_able<Stringify1>::value);
   OK(!h2::h2_toString_able<Stringify1>::value);
   OK(!h2::h2_Tostring_able<Stringify1>::value);
   OK(!h2::h2_ToString_able<Stringify1>::value);
   OK(!h2::h2_to_string_able<Stringify1>::value);
}

SUITE(stringify simple)
{
   Case(int)
   {
      OK(ListOf("1"), h2::h2_stringify<int>(1));
      OK(ListOf("1"), h2::h2_stringify<int>(1, true));
   }

   Case(double)
   {
      OK(ListOf("1.41421"), h2::h2_stringify<double>(sqrt(2)));
      OK(ListOf("1.41421"), h2::h2_stringify<double>(sqrt(2), true));
   }

   Case(bool)
   {
      OK(ListOf("true"), h2::h2_stringify<bool>(true));
      OK(ListOf("true"), h2::h2_stringify<bool>(true, true));
      OK(ListOf("false"), h2::h2_stringify<bool>(false));
      OK(ListOf("false"), h2::h2_stringify<bool>(false, true));
   }

   Case(long long)
   {
      long long a = 12345678;
      OK(ListOf("12345678"), h2::h2_stringify<long long>(a));
      OK(ListOf("12345678"), h2::h2_stringify<long long>(a, true));
   }

   Case(char)
   {
      char a = 'A';
      OK(ListOf("A"), h2::h2_stringify<char>(a));
      OK(ListOf("\033{+dark gray}", "\'", "\033{-dark gray}",
                "A",
                "\033{+dark gray}", "\'", "\033{-dark gray}"),
         h2::h2_stringify<char>(a, true));
   }

   Case(char*)
   {
      char* a = "hello world";
      OK("hello world", h2::h2_stringify<char*>(a));
      OK(ListOf("\033{+dark gray}", "\"", "\033{-dark gray}",
                "hello world",
                "\033{+dark gray}", "\"", "\033{-dark gray}"),
         h2::h2_stringify<char*>(a, true));
   }

   Case(unsigned char*)
   {
      unsigned char a[] = {'h', 'e', 'l', 'l', 'o', 0};
      OK("hello", h2::h2_stringify<unsigned char*>(a));
      OK("hello", h2::h2_stringify<unsigned char*>(a, true));
   }

   Case(void*)
   {
      void* a = (void*)0x12345678;
      OK("0x12345678", h2::h2_stringify<void*>(a));
      OK("0x12345678", h2::h2_stringify<void*>(a, true));
   }

   Case(nullptr)
   {
      OK("nullptr", h2::h2_stringify<std::nullptr_t>(nullptr));
      OK("nullptr", h2::h2_stringify<std::nullptr_t>(nullptr, true));
   }

   // https://en.cppreference.com/w/cpp/string/byte/isprint
   Case(uint8_t)
   {
      unsigned char a1 = 7;
      OK("7", h2::h2_stringify<unsigned char>(a1));
      OK("7", h2::h2_stringify<unsigned char>(a1, true));
      uint8_t a2 = 7;
      OK("7", h2::h2_stringify<uint8_t>(a2, true));
   }
}

SUITE(stringify user)
{
   Case(toString)
   {
      Stringify2 f2;
      OK("tostring", h2::h2_stringify<Stringify2>(f2));
      OK("tostring", h2::h2_stringify<Stringify2>(f2, true));
   }

   Case(operator<<)
   {
      Stringify3 f3;
      OK("Stringify3", h2::h2_stringify<Stringify3>(f3, true));
   }
}

SUITE(stringify complex)
{
   Case(pair)
   {
      std::pair<int, std::string> a1 = std::make_pair(9, std::string("nine"));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::pair<int, std::string>>(a1));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::pair<int, std::string>>(a1, true));

      std::pair<std::string, int> a2 = std::make_pair(std::string("nine"), 9);
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::pair<std::string, int>>(a2));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::pair<std::string, int>>(a2, true));
   }

   Case(tuple)
   {
      std::tuple<> a0;
      std::tuple<int> a1 = std::make_tuple(42);
      std::tuple<const char*, int> a2 = std::make_tuple("nine", 9);
      std::tuple<const char*, int, std::pair<std::string, double>> a3 = std::make_tuple("nine", 9, std::make_pair(std::string("pai"), 3.14));

      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<>>(a0));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<>>(a0, true));

      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "42",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<int>>(a1));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "42",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<int>>(a1, true));

      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<const char*, int>>(a2));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<const char*, int>>(a2, true));

      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "pai",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3.14",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<const char*, int, std::pair<std::string, double>>>(a3));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "pai",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3.14",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         h2::h2_stringify<std::tuple<const char*, int, std::pair<std::string, double>>>(a3, true));
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
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, true));
      OK("[1, 2, 3]", h2::h2_stringify(a1).string());
   }

   Case(vector)
   {
      std::vector<const char*> a1 = {"1", "2", "3"};
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, true));
   }

   Case(deque)
   {
      std::deque<char> a1 = {'1', '2', '3'};
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::deque<char>>(a1));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "\'", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", "\'", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\'", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", "\'", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\'", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "\'", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::deque<char>>(a1, true));
   }

   Case(forward_list)
   {
      std::forward_list<const char*> a1 = {"1", "2", "3"};
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::forward_list<const char*>>(a1));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::forward_list<const char*>>(a1, true));
   }

   Case(list)
   {
      std::list<const char*> a1 = {"1", "2", "3"};
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, true));
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
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "false",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "true",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::set<bool>>(a1));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "false",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "true",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::set<bool>>(a1, true));
   }

   Case(map)
   {
      std::map<std::string, int> a1 = {{std::string("th1"), 1},
                                       {std::string("th2"), 2},
                                       {std::string("th3"), 3},
                                       {std::string("th3"), -3}};

      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "th1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "th2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "th3",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::map<std::string, int>>(a1));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "th1",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "th2",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "th3",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::map<std::string, int>>(a1, true));
   }

   Case(multiset)
   {
      std::multiset<bool> a1 = {false, false, true};
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "false",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "false",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "true",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::multiset<bool>>(a1));
   }

   Case(multimap)
   {
      std::multimap<std::string, int> a1 = {{std::string("th1"), 1},
                                            {std::string("th2"), 2},
                                            {std::string("th3"), 3},
                                            {std::string("th3"), -3}};

      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "th1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "th2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "th3",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "th3",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "-3",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::multimap<std::string, int>>(a1));

      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "th1",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "th2",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "th3",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "th3",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "-3",
                "\033{+dark gray}", ")", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify<std::multimap<std::string, int>>(a1, true));
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
      OK(AnyOf("[false, true]", "[true, false]"), h2::h2_stringify<std::unordered_set<bool>>(a1).string());
   }

   Case(unordered_multiset)
   {
      std::unordered_multiset<bool> a1 = {false, true, true};
      OK(AnyOf("[false, true, true]", "[true, true, false]"), h2::h2_stringify<std::unordered_multiset<bool>>(a1).string());
   }

   Case(unordered_map)
   {
      std::unordered_map<std::string, int> a1 = {{std::string("th1"), 1},
                                                 {std::string("th2"), 2}};

      OK(AnyOf("[(th1, 1), (th2, 2)]", "[(th2, 2), (th1, 1)]"), h2::h2_stringify<std::unordered_map<std::string, int>>(a1).string());
   }

   Case(unordered_multimap)
   {
      std::unordered_multimap<std::string, int> a1 = {{std::string("th1"), 1},
                                                      {std::string("th2"), 2}};

      OK(AnyOf("[(th1, 1), (th2, 2)]", "[(th2, 2), (th1, 1)]"), h2::h2_stringify<std::unordered_multimap<std::string, int>>(a1).string());
   }
}

SUITE(array with count)
{
   Case(int)
   {
      int a1[] = {1, 2, 3};
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, 3, false));
   }

   Case(valarray)
   {
      std::valarray<std::string> a1(3);
      a1[0] = "1";
      a1[1] = "2";
      a1[2] = "3";

      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, 3, false));
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, 3, true));
   }
}
