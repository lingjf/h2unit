#include "../source/h2_unit.cpp"
#include "test_cplusplus.hpp"
#include "test_types.hpp"

CASE(nth_type)
{
   OK((std::is_same<char, typename h2::h2_nth_type<0, char, short, int>::type>::value));
   OK((std::is_same<short, typename h2::h2_nth_type<1, char, short, int>::type>::value));
   OK((std::is_same<int, typename h2::h2_nth_type<2, char, short, int>::type>::value));
}

SUITE(is smart pointer)
{
   Case(shared_ptr)
   {
      auto a1 = std::make_shared<int>(42);
      OK((h2::h2_is_smart_ptr<decltype(a1)>::value));
   }

   Case(unique_ptr)
   {
      std::string* s1 = new std::string("hello");
      std::unique_ptr<std::string> a1(s1);

      OK((h2::h2_is_smart_ptr<decltype(a1)>::value));
   }

   Case(weak_ptr)
   {
      std::weak_ptr<int> a1;
      a1 = std::make_shared<int>(42);

      OK((h2::h2_is_smart_ptr<decltype(a1)>::value));
   }
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

   Case(shared_ptr)
   {
      auto a1 = std::make_shared<int>(42);
      OK(sizeof(int), h2::h2_sizeof_pointee<decltype(a1)>::value);
   }

   Case(unique_ptr)
   {
      std::unique_ptr<int> a1(new int(42));
      OK(sizeof(int), h2::h2_sizeof_pointee<decltype(a1)>::value);
   }
}

SUITE(pointee type)
{
   Case(int*)
   {
      OK((std::is_same<int, typename h2::h2_pointee_type<int*>::type>::value));
   }

   Case(int**)
   {
      OK((std::is_same<int*, typename h2::h2_pointee_type<int**>::type>::value));
   }

   Case(int***)
   {
      OK((std::is_same<int**, typename h2::h2_pointee_type<int***>::type>::value));
   }

   Case(char*)
   {
      OK((std::is_same<char, typename h2::h2_pointee_type<char*>::type>::value));
   }

   Case(short*)
   {
      OK((std::is_same<short, typename h2::h2_pointee_type<short*>::type>::value));
   }

   Case(unsigned long long*)
   {
      OK((std::is_same<unsigned long long, typename h2::h2_pointee_type<unsigned long long*>::type>::value));
   }

   Case(B_ClassStruct*)
   {
      OK((std::is_same<B_ClassStruct, typename h2::h2_pointee_type<B_ClassStruct*>::type>::value));
   }

   Case(void*)
   {
      OK((std::is_same<void, typename h2::h2_pointee_type<void*>::type>::value));
   }

   Case(void**)
   {
      OK((std::is_same<void*, typename h2::h2_pointee_type<void**>::type>::value));
   }

   Case(shared_ptr)
   {
      auto a1 = std::make_shared<int>(42);
      OK((std::is_same<int, typename h2::h2_pointee_type<decltype(a1)>::type>::value));
   }

   Case(unique_ptr)
   {
      std::unique_ptr<int> a1(new int(42));
      OK((std::is_same<int, typename h2::h2_pointee_type<decltype(a1)>::type>::value));
   }

   Case(weak_ptr)
   {
      std::weak_ptr<int> a1(std::make_shared<int>(42));
      OK((std::is_same<int, typename h2::h2_pointee_type<decltype(a1)>::type>::value));
   }
}

SUITE(pointer_if)
{
   Case(normal ptr nullptr)
   {
      const char* a1 = nullptr;
      OK(nullptr == h2::h2_pointer_if(a1));

      int* a2 = nullptr;
      OK(nullptr == h2::h2_pointer_if(a2));

      std::string* a3 = nullptr;
      OK(nullptr == h2::h2_pointer_if(a3));
   }

   Case(normal ptr)
   {
      char s1[1024];
      const char* a1 = s1;
      OK(s1 == h2::h2_pointer_if(a1));

      int s2 = 1;
      int* a2 = &s2;
      OK(&s2 == h2::h2_pointer_if(a2));

      std::string s3;
      std::string* a3 = &s3;
      OK(&s3 == h2::h2_pointer_if(a3));
   }

   Case(smart ptr)
   {
      std::string* s1 = new std::string("hello");
      std::shared_ptr<std::string> a1(s1);

      OK(!(std::is_pointer<std::shared_ptr<std::string>>::value));
      OK(s1 == (std::string*)h2::h2_pointer_if(a1));
   }

   Case(object)
   {
      char s1[1024];
      OK(s1 == h2::h2_pointer_if(s1));

      int s2 = 1;
      OK(&s2 == h2::h2_pointer_if(s2));

      std::string s3;
      OK(&s3 == h2::h2_pointer_if(s3));
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

SUITE(is_string)
{
   Case(Fundamental types)
   {
      OK(!(h2::h2_is_string<int>::value));
   }

   Case(std::string)
   {
      OK((h2::h2_is_string<std::string>::value));
   }

   Case(h2_string)
   {
      OK((h2::h2_is_string<h2::h2_string>::value));
   }

   Case(Sequence containers)
   {
      OK(!(h2::h2_is_string<std::array<int, 8>>::value));
      OK(!(h2::h2_is_string<std::vector<int>>::value));
      OK(!(h2::h2_is_string<std::deque<int>>::value));
      OK(!(h2::h2_is_string<std::list<int>>::value));
      OK(!(h2::h2_is_string<std::forward_list<int>>::value));

      OK(!(h2::h2_is_string<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK(!(h2::h2_is_string<std::set<int>>::value));
      OK(!(h2::h2_is_string<std::multiset<int>>::value));
      OK(!(h2::h2_is_string<std::unordered_set<int>>::value));
      OK(!(h2::h2_is_string<std::unordered_multiset<int>>::value));
      OK(!(h2::h2_is_string<std::map<int, int>>::value));
      OK(!(h2::h2_is_string<std::multimap<int, int>>::value));
      OK(!(h2::h2_is_string<std::unordered_map<int, int>>::value));
      OK(!(h2::h2_is_string<std::unordered_multimap<int, int>>::value));
   }

   Case(Container adaptors)
   {
      OK(!(h2::h2_is_string<std::stack<int>>::value));
      OK(!(h2::h2_is_string<std::queue<int>>::value));
      OK(!(h2::h2_is_string<std::priority_queue<int>>::value));
   }
}

SUITE(is_iterable)
{
   Case(Fundamental types)
   {
      OK(!(h2::h2_is_iterable<int>::value));
   }

   Case(Sequence containers)
   {
      OK((h2::h2_is_iterable<std::array<int, 8>>::value));
      OK((h2::h2_is_iterable<std::vector<int>>::value));
      OK((h2::h2_is_iterable<std::deque<int>>::value));
      OK((h2::h2_is_iterable<std::list<int>>::value));
      OK((h2::h2_is_iterable<std::forward_list<int>>::value));

      OK((h2::h2_is_iterable<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK((h2::h2_is_iterable<std::set<int>>::value));
      OK((h2::h2_is_iterable<std::multiset<int>>::value));
      OK((h2::h2_is_iterable<std::unordered_set<int>>::value));
      OK((h2::h2_is_iterable<std::unordered_multiset<int>>::value));
      OK((h2::h2_is_iterable<std::map<int, int>>::value));
      OK((h2::h2_is_iterable<std::multimap<int, int>>::value));
      OK((h2::h2_is_iterable<std::unordered_map<int, int>>::value));
      OK((h2::h2_is_iterable<std::unordered_multimap<int, int>>::value));
   }

   Case(Container adaptors)
   {
      OK(!(h2::h2_is_iterable<std::stack<int>>::value));
      OK(!(h2::h2_is_iterable<std::queue<int>>::value));
      OK(!(h2::h2_is_iterable<std::priority_queue<int>>::value));
   }

   Case(string)
   {
      OK((h2::h2_is_iterable<std::string>::value));
      OK((h2::h2_is_iterable<h2::h2_string>::value));
   }

   Case(valarray)
   {
      OK(!(h2::h2_is_iterable<std::valarray<int>>::value));
   }
}

SUITE(is_map)
{
   Case(Fundamental types)
   {
      OK(!(h2::h2_is_map<int>::value));
   }

   Case(Sequence containers)
   {
      OK(!(h2::h2_is_map<std::array<int, 8>>::value));
      OK(!(h2::h2_is_map<std::vector<int>>::value));
      OK(!(h2::h2_is_map<std::deque<int>>::value));
      OK(!(h2::h2_is_map<std::list<int>>::value));
      OK(!(h2::h2_is_map<std::forward_list<int>>::value));

      OK(!(h2::h2_is_map<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK(!(h2::h2_is_map<std::set<int>>::value));
      OK(!(h2::h2_is_map<std::multiset<int>>::value));
      OK(!(h2::h2_is_map<std::unordered_set<int>>::value));
      OK(!(h2::h2_is_map<std::unordered_multiset<int>>::value));
      OK((h2::h2_is_map<std::map<int, int>>::value));
      OK((h2::h2_is_map<std::multimap<int, int>>::value));
      OK((h2::h2_is_map<std::unordered_map<int, int>>::value));
      OK((h2::h2_is_map<std::unordered_multimap<int, int>>::value));
   }

   Case(Container adaptors)
   {
      OK(!(h2::h2_is_map<std::stack<int>>::value));
      OK(!(h2::h2_is_map<std::queue<int>>::value));
      OK(!(h2::h2_is_map<std::priority_queue<int>>::value));
   }

   Case(string)
   {
      OK(!(h2::h2_is_map<std::string>::value));
      OK(!(h2::h2_is_map<h2::h2_string>::value));
   }

   Case(valarray)
   {
      OK(!(h2::h2_is_map<std::valarray<int>>::value));
   }
}

SUITE(is_container)
{
   Case(Fundamental types)
   {
      OK(!(h2::h2_is_container<int>::value));
   }

   Case(Sequence containers)
   {
      OK((h2::h2_is_container<std::array<int, 8>>::value));
      OK((h2::h2_is_container<std::vector<int>>::value));
      OK((h2::h2_is_container<std::deque<int>>::value));
      OK((h2::h2_is_container<std::list<int>>::value));
      OK((h2::h2_is_container<std::forward_list<int>>::value));

      OK((h2::h2_is_container<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK((h2::h2_is_container<std::set<int>>::value));
      OK((h2::h2_is_container<std::multiset<int>>::value));
      OK((h2::h2_is_container<std::unordered_set<int>>::value));
      OK((h2::h2_is_container<std::unordered_multiset<int>>::value));
      OK((h2::h2_is_container<std::map<int, int>>::value));
      OK((h2::h2_is_container<std::multimap<int, int>>::value));
      OK((h2::h2_is_container<std::unordered_map<int, int>>::value));
      OK((h2::h2_is_container<std::unordered_multimap<int, int>>::value));
   }

   Case(Container adaptors)
   {
      OK(!(h2::h2_is_container<std::stack<int>>::value));
      OK(!(h2::h2_is_container<std::queue<int>>::value));
      OK(!(h2::h2_is_container<std::priority_queue<int>>::value));
   }

   Case(string)
   {
      OK(!(h2::h2_is_container<std::string>::value));
      OK(!(h2::h2_is_container<h2::h2_string>::value));
   }

   Case(valarray)
   {
      OK(!(h2::h2_is_container<std::valarray<int>>::value));
   }
}

SUITE(is_container_adaptor)
{
   Case(Fundamental types)
   {
      OK(!(h2::h2_is_container_adaptor<int>::value));
   }

   Case(stack)
   {
      OK((h2::h2_is_container_adaptor<std::stack<int>>::value));

      std::stack<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK((h2::h2_is_container_adaptor<decltype(a1)>::value));
   }

   Case(queue)
   {
      OK((h2::h2_is_container_adaptor<std::queue<int>>::value));
   }

   Case(priority_queue)
   {
      OK((h2::h2_is_container_adaptor<std::priority_queue<int>>::value));
   }

   Case(Sequence containers)
   {
      OK(!(h2::h2_is_container_adaptor<std::array<int, 8>>::value));
      OK(!(h2::h2_is_container_adaptor<std::vector<int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::deque<int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::list<int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::forward_list<int>>::value));

      OK(!(h2::h2_is_container_adaptor<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK(!(h2::h2_is_container_adaptor<std::set<int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::multiset<int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::unordered_set<int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::unordered_multiset<int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::map<int, int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::multimap<int, int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::unordered_map<int, int>>::value));
      OK(!(h2::h2_is_container_adaptor<std::unordered_multimap<int, int>>::value));
   }

   Case(string)
   {
      OK(!(h2::h2_is_container_adaptor<std::string>::value));
      OK(!(h2::h2_is_container_adaptor<h2::h2_string>::value));
   }
}

SUITE(sizable)
{
   Case(Fundamental types)
   {
      OK(!(h2::h2_is_sizable<int>::value));
   }

   Case(Sequence containers)
   {
      OK((h2::h2_is_sizable<std::array<int, 8>>::value));
      OK((h2::h2_is_sizable<std::vector<int>>::value));
      OK((h2::h2_is_sizable<std::deque<int>>::value));
      OK((h2::h2_is_sizable<std::list<int>>::value));
      OK(!(h2::h2_is_sizable<std::forward_list<int>>::value));

      OK((h2::h2_is_sizable<h2::h2_vector<int>>::value));
   }

   Case(Associative containers)
   {
      OK((h2::h2_is_sizable<std::set<int>>::value));
      OK((h2::h2_is_sizable<std::multiset<int>>::value));
      OK((h2::h2_is_sizable<std::unordered_set<int>>::value));
      OK((h2::h2_is_sizable<std::unordered_multiset<int>>::value));
      OK((h2::h2_is_sizable<std::map<int, int>>::value));
      OK((h2::h2_is_sizable<std::multimap<int, int>>::value));
      OK((h2::h2_is_sizable<std::unordered_map<int, int>>::value));
      OK((h2::h2_is_sizable<std::unordered_multimap<int, int>>::value));
   }

   Case(valarray)
   {
      OK((h2::h2_is_sizable<std::valarray<int>>::value));
   }

   Case(string)
   {
      OK((h2::h2_is_sizable<std::string>::value));
      OK((h2::h2_is_sizable<h2::h2_string>::value));
   }

   Case(Container adaptors)
   {
      OK((h2::h2_is_sizable<std::stack<int>>::value));
      OK((h2::h2_is_sizable<std::queue<int>>::value));
      OK((h2::h2_is_sizable<std::priority_queue<int>>::value));
   }
}

SUITE(underlying_container)
{
   Case(stack)
   {
      std::stack<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);
      OK(ListOf(1, 2, 3), h2::underlying_container(a1));
   }

   Case(queue)
   {
      std::queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);
      OK(ListOf(1, 2, 3), h2::underlying_container(a1));
   }

   Case(priority_queue)
   {
      std::priority_queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);
      OK(Has(1) && Has(2) && Has(3), h2::underlying_container(a1));
   }
}
