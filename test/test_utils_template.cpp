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

   Case(smart ptr)
   {
      auto a1 = std::make_shared<int>(42);
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

   Case(smart ptr)
   {
      auto a1 = std::make_shared<int>(42);
      OK((std::is_same<int, typename h2::h2_pointee_type<decltype(a1)>::type>::value));

      std::unique_ptr<int> a2(new int(42));
      OK((std::is_same<int, typename h2::h2_pointee_type<decltype(a2)>::type>::value));

      std::weak_ptr<int> a3(std::make_shared<int>(42));
      OK((std::is_same<int, typename h2::h2_pointee_type<decltype(a3)>::type>::value));
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

#define FF()                                     \
   sprintf(z, "%9s: t=%s, T=%s", s,              \
           h2::h2_cxa::type_name<decltype(t)>(), \
           h2::h2_cxa::type_name<T>());          \
   return z

#define FF2()                                                \
   sprintf(z, "%9s: t=%s, T=%s", s,                          \
           h2::h2_cxa::demangle(typeid(decltype(t)).name()), \
           h2::h2_cxa::demangle(typeid(T).name()));          \
   return z

static char z[256];

template <typename T>
static const char* f1(const char* s, T t) { FF(); }

template <typename T>
static const char* f2(const char* s, const T t) { FF(); }

template <typename T>
static const char* f3(const char* s, T& t) { FF(); }

template <typename T>
static const char* f4(const char* s, const T& t) { FF(); }

TODO(template parameter type)
{
   int a = 1;
   int* ap = &a;  // auto ap = &a;
   int& ar = a;
   int&& arr = 2;

   const int ca = 1;
   const int* cap = &ca;  // auto cap = &ca;
   const int& car = ca;
   const int&& carr = 2;

   ::printf("        a: %s\n", h2::h2_cxa::type_name<decltype(a)>());
   ::printf("       ap: %s\n", h2::h2_cxa::type_name<decltype(ap)>());
   ::printf("       ar: %s\n", h2::h2_cxa::type_name<decltype(ar)>());
   ::printf("      arr: %s\n", h2::h2_cxa::type_name<decltype(arr)>());
   ::printf("       ca: %s\n", h2::h2_cxa::type_name<decltype(ca)>());
   ::printf("      cap: %s\n", h2::h2_cxa::type_name<decltype(cap)>());
   ::printf("      car: %s\n", h2::h2_cxa::type_name<decltype(car)>());
   ::printf("     carr: %s\n", h2::h2_cxa::type_name<decltype(carr)>());

   ::printf("%s\n", f1("f1(a)", a));
   ::printf("%s\n", f1("f1(ap)", ap));
   ::printf("%s\n", f1("f1(ar)", ar));
   ::printf("%s\n", f1("f1(arr)", arr));
   ::printf("%s\n", f1("f1(ca)", ca));
   ::printf("%s\n", f1("f1(cap)", cap));
   ::printf("%s\n", f1("f1(car)", car));
   ::printf("%s\n", f1("f1(carr)", carr));

   ::printf("%s\n", f2("f2(a)", a));
   ::printf("%s\n", f2("f2(ap)", ap));
   ::printf("%s\n", f2("f2(ar)", ar));
   ::printf("%s\n", f2("f2(arr)", arr));
   ::printf("%s\n", f2("f2(ca)", ca));
   ::printf("%s\n", f2("f2(cap)", cap));
   ::printf("%s\n", f2("f2(car)", car));
   ::printf("%s\n", f2("f2(carr)", carr));

   ::printf("%s\n", f3("f3(a)", a));
   ::printf("%s\n", f3("f3(ap)", ap));
   ::printf("%s\n", f3("f3(ar)", ar));
   ::printf("%s\n", f3("f3(arr)", arr));
   ::printf("%s\n", f3("f3(ca)", ca));
   ::printf("%s\n", f3("f3(cap)", cap));
   ::printf("%s\n", f3("f3(car)", car));
   ::printf("%s\n", f3("f3(carr)", carr));

   ::printf("%s\n", f4("f4(ca)", ca));
   ::printf("%s\n", f4("f4(cap)", cap));
   ::printf("%s\n", f4("f4(car)", car));
   ::printf("%s\n", f4("f4(carr)", carr));
}
