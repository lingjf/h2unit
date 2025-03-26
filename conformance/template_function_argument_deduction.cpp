// https://en.cppreference.com/w/cpp/language/function_template
#include "__common.hpp"

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

bool filter(int i, int j = 0)
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
