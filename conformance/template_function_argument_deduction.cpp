// https://en.cppreference.com/w/cpp/language/function_template

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define H2PP_STR(...) _H2PP_STR(__VA_ARGS__)
#define _H2PP_STR(...) #__VA_ARGS__

const char* cc_version()
{
#ifdef _MSC_VER
   switch (_MSC_VER) {
      case 1200: return "Microsoft Visual Studio 6.0";
      case 1300: return "Microsoft Visual Studio .NET 2002 (7.0)";
      case 1310: return "Microsoft Visual Studio .NET 2003 (7.1)";
      case 1400: return "Microsoft Visual Studio 2005 (8.0)";
      case 1500: return "Microsoft Visual Studio 2008 (9.0)";
      case 1600: return "Microsoft Visual Studio 2010 (10.0)";
      case 1700: return "Microsoft Visual Studio 2012 (11.0)";
      case 1800: return "Microsoft Visual Studio 2013 (12.0)";
      case 1900: return "Microsoft Visual Studio 2015 (14.0)";
      case 1910: return "Microsoft Visual Studio 2017 RTW (15.0)";
      case 1911: return "Microsoft Visual Studio 2017 version 15.3";
      case 1912: return "Microsoft Visual Studio 2017 version 15.5";
      case 1913: return "Microsoft Visual Studio 2017 version 15.6";
      case 1914: return "Microsoft Visual Studio 2017 version 15.7";
      case 1915: return "Microsoft Visual Studio 2017 version 15.8";
      case 1916: return "Microsoft Visual Studio 2017 version 15.9";
      case 1920: return "Microsoft Visual Studio 2019 RTW (16.0)";
      case 1921: return "Microsoft Visual Studio 2019 version 16.1";
      case 1922: return "Microsoft Visual Studio 2019 version 16.2";
      case 1923: return "Microsoft Visual Studio 2019 version 16.3";
      case 1924: return "Microsoft Visual Studio 2019 version 16.4";
      case 1925: return "Microsoft Visual Studio 2019 version 16.5";
      case 1926: return "Microsoft Visual Studio 2019 version 16.6";
      case 1927: return "Microsoft Visual Studio 2019 version 16.7";
      case 1928: return "Microsoft Visual Studio 2019 version 16.8, 16.9";
      case 1929: return "Microsoft Visual Studio 2019 version 16.10, 16.11";
      case 1930: return "Microsoft Visual Studio 2022 RTW (17.0)";
      default: return "Microsoft Visual Studio ? version";
   }
#elif defined __clang__
   return "clang " H2PP_STR(__clang_major__) "." H2PP_STR(__clang_minor__) "." H2PP_STR(__clang_patchlevel__);
#else
   return "gcc " H2PP_STR(__GNUC__) "." H2PP_STR(__GNUC_MINOR__) "." H2PP_STR(__GNUC_PATCHLEVEL__);
#endif
}

const char* std_version()
{
   switch (
#ifdef _MSC_VER
     _MSVC_LANG
#else
     __cplusplus
#endif
   ) {
      case 201103L: return "c++11";
      case 201402L: return "c++14";
      case 201703L: return "c++17";
      case 202002L: return "c++20/2a";
      case 202102L: return "c++23/2b";
   }
}

template <typename T> std::string type_name()
{
   typedef typename std::remove_reference<T>::type U;
   std::unique_ptr<char, void (*)(void*)> own(
#ifndef _MSC_VER
     abi::__cxa_demangle(typeid(U).name(), nullptr, nullptr, nullptr),
#else
     nullptr,
#endif
     std::free);
   std::string r = own != nullptr ? own.get() : typeid(U).name();
   if (r.npos != r.find(" __ptr64")) r.replace(r.find(" __ptr64"), strlen(" __ptr64"), "");

   if (std::is_const<U>::value)
      r += " const";
   if (std::is_volatile<U>::value)
      r += " volatile";
   if (std::is_lvalue_reference<T>::value)
      r += "&";
   else if (std::is_rvalue_reference<T>::value)
      r += "&&";
   return r;
}

template <typename T> auto f1(T t) -> std::vector<std::string> { return {type_name<T>(), type_name<decltype(t)>()}; }
template <typename T> auto f2(const T t) -> std::vector<std::string> { return {type_name<T>(), type_name<decltype(t)>()}; }
template <typename T> auto f3(T& t) -> std::vector<std::string> { return {type_name<T>(), type_name<decltype(t)>()}; }
template <typename T> auto f4(const T& t) -> std::vector<std::string> { return {type_name<T>(), type_name<decltype(t)>()}; }
template <typename T> auto f5(T&& t) -> std::vector<std::string> { return {type_name<T>(), type_name<decltype(t)>()}; }

char c = '\0';
int n = 0;
bool exclude = false;

bool filter(size_t i, size_t j = 0)
{
   if (i > 0 && (c == 'a' || c == 'A')) {
      if (n == i) {
         if (exclude)
            return true;
      } else {
         if (!exclude)
            return true;
      }
   }
   if (j > 0 && (c == 'f' || c == 'F')) {
      if (n == j) {
         if (exclude)
            return true;
      } else {
         if (!exclude)
            return true;
      }
   }

   return false;
}

int main(int argc, const char** argv)
{
   printf("%s %s\n\n", cc_version(), std_version());

   if (argc == 2) {
      const char* p = argv[1];
      if (*p == '-') {
         exclude = true;
         p++;
      }
      if (2 != sscanf(p, "%c%d", &c, &n)) return -1;
   }

   int a1 = 1;
   int* a2 = &a1;
   int& a3 = a1;
   int&& a4 = 2;

   int const a5 = 1;
   int const* a6 = &a5;
   int const& a7 = a5;
   int const&& a8 = 2;

   int* const a9 = nullptr;
   int const* const a10 = nullptr;

   std::vector<std::string> as = {
     type_name<decltype(a1)>() + " a",
     type_name<decltype(a2)>() + " a",
     type_name<decltype(a3)>() + " a",
     type_name<decltype(a4)>() + " a",
     type_name<decltype(a5)>() + " a",
     type_name<decltype(a6)>() + " a",
     type_name<decltype(a7)>() + " a",
     type_name<decltype(a8)>() + " a",
     type_name<decltype(a9)>() + " a",
     type_name<decltype(a10)>() + " a",
     "\"foobar\"",
     "42",
     "(int)3.14",
   };

   std::vector<const char*> fs = {
     "f(\033[36mT\033[0m t)              ",
     "f(const \033[36mT\033[0m t)        ",
     "f(\033[36mT\033[0m& t)             ",
     "f(const \033[36mT\033[0m& t)       ",
     "f(\033[36mT\033[0m&& t)",
   };

#define CAF(a) f1(a), f2(a), f3(a), f4(a), f5(a)

   std::vector<std::vector<std::vector<std::string>>> afs = {
     {CAF(a1)},
     {CAF(a2)},
     {CAF(a3)},
     {CAF(a4)},
     {CAF(a5)},
     {CAF(a6)},
     {CAF(a7)},
     {CAF(a8)},
     {CAF(a9)},
     {CAF(a10)},
     {CAF("foobar")},
     {f1(42), f2(42), {"", ""}, f4(42), f5(42)},
     {f1((int)3.14), f2((int)3.14), {"", ""}, f4((int)3.14), f5((int)3.14)},
   };

   printf("\033[90m%-20s\033[0m", "argument");
   for (size_t j = 0; j < fs.size(); j++) {
      if (filter(0, j + 1)) continue;
      printf("\033[90m%-20s\033[0m", "template<class T>");
   }
   printf("\n");

   printf("\033[90m  ↓  function  →    \033[0m");
   for (size_t j = 0; j < fs.size(); j++) {
      if (filter(0, j + 1)) continue;
      printf("%s", fs[j]);
   }
   printf("\n");

   for (size_t i = 0; i < afs.size(); i++) {
      if (filter(i + 1, 0)) continue;
      printf("%-20s", as[i].c_str());
      for (size_t j = 0; j < afs[i].size(); j++) {
         if (filter(i + 1, j + 1)) continue;
         printf("\033[36m%-20s\033[0m", afs[i][j][0].c_str());
      }
      printf("\n");
      printf("\033[90m%3lu.%-16s\033[0m", i + 1, " f(a)");
      for (size_t j = 0; j < afs[i].size(); j++) {
         if (filter(i + 1, j + 1)) continue;
         printf("%-20s", afs[i][j][1].c_str());
      }
      printf("\n");
   }

   printf("\n");

   return 0;
}
