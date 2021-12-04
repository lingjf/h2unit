#include "../source/h2_unit.cpp"
#include "test_cplusplus.hpp"

CASE(__cplusplus)
{
   // g++ -x c++ -std=c++11 -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 201103L

   // g++ -x c++ -std=c++14 -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 201402L

   // g++ -x c++ -std=c++17 -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 201703L

   // g++ -x c++ -std=c++2a -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 202002L

   // g++ -x c++ -std=c++2b -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 202102L

   // https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
   ::printf("__cplusplus = %s\n", H2PP_STR(__cplusplus));
#if defined _MSC_VER
   ::printf("__cplusplus = %s\n", H2PP_STR(_MSVC_LANG));
#endif
}

SUITE(array initialization)
{
   Case(C++ array elements with missing values will be initialized to 0)
   {
      int a[10] = {1, 2};  // initialize to 1,2,0,0,0...
      OK(1, a[0]);
      OK(2, a[1]);
      for (int i = 2; i < 10; i++) {
         OK(0, a[i]);
      }
   }

   Case(initialize all elements to 0)
   {
      int a[10] = {0};  // all elements 0
      for (int i = 0; i < 10; i++) {
         OK(0, a[i]);
      }
   }

   Case(C++ an empty initialization list will initialize every element to 0)
   {
      int a[10] = {};  // all elements 0 in C++, is not allowed with C
      for (int i = 0; i < 10; i++) {
         OK(0, a[i]);
      }
   }

   Case(dim2 missing zero)
   {
      char a[10][10] = {{'0', '1', '2'}, {'0', '1'}, {'0'}, {}};
      int i = 0;
      for (; i < 1; i++) {
         OK('0', a[i][0]);
         OK('1', a[i][1]);
         OK('2', a[i][2]);
         for (int j = 3; j < 10; j++) {
            OK('\0', a[i][j]);
         }
      }
      for (; i < 2; i++) {
         OK('0', a[i][0]);
         OK('1', a[i][1]);
         for (int j = 2; j < 10; j++) {
            OK('\0', a[i][j]);
         }
      }
      for (; i < 3; i++) {
         OK('0', a[i][0]);
         for (int j = 1; j < 10; j++) {
            OK('\0', a[i][j]);
         }
      }
      for (; i < 10; i++) {
         for (int j = 0; j < 10; j++) {
            OK('\0', a[i][j]);
         }
      }
   }

   Case(dim2 all zero)
   {
      char a[10][10] = {{'\0'}};
      for (int i = 0; i < 10; i++) {
         for (int j = 0; j < 10; j++) {
            OK('\0', a[i][j]);
         }
      }
   }
}

SUITE(Fundamental types)  // https://en.cppreference.com/w/cpp/language/types
{
   Case(void*)
   {
      OK(!(std::is_arithmetic<void*>::value));
   }

   Case(nullptr)
   {
      OK(!(std::is_pointer<std::nullptr_t>::value));
      OK(!(std::is_integral<std::nullptr_t>::value));
      OK(!(std::is_arithmetic<std::nullptr_t>::value));

      OK((std::is_convertible<std::nullptr_t, std::string>::value));
      OK(!(std::is_convertible<std::nullptr_t, int>::value));

      // implicitly converted to any pointer and pointer to member type
      OK((std::is_convertible<std::nullptr_t, void*>::value));
      OK((std::is_convertible<std::nullptr_t, int*>::value));
      OK((std::is_convertible<std::nullptr_t, int (*)(int)>::value));
      OK((std::is_convertible<std::nullptr_t, int (std::string::*)(int)>::value));
   }

   Case(NULL)
   {
      // #define NULL /*implementation-defined*/
      // gcc: long
      // clang: long
      // MSVC: int
      // msys2: long long / int

      OK((std::is_same<decltype(NULL), long>::value || std::is_same<decltype(NULL), int>::value || std::is_same<decltype(NULL), long long>::value));
   }

   Case(char)
   {
      OK(!(std::is_same<char, signed char>::value));  //! Common Sense
      OK(!(std::is_same<char, unsigned char>::value));
      OK(!(std::is_same<signed char, unsigned char>::value));
   }

   Case(integer-short)
   {
      // signed
      OK((std::is_same<short, short int>::value));
      OK((std::is_same<signed short, short int>::value));
      OK((std::is_same<signed short int, short int>::value));
      // unsigned
      OK((std::is_same<unsigned short, unsigned short int>::value));
   }

   Case(integer-int)
   {
      // signed
      OK((std::is_same<signed, int>::value));
      OK((std::is_same<signed int, int>::value));
      // unsigned
      OK((std::is_same<unsigned, unsigned int>::value));
   }

   Case(integer-long)
   {
      // signed
      OK((std::is_same<long, long int>::value));
      OK((std::is_same<signed long, long int>::value));
      OK((std::is_same<signed long int, long int>::value));
      // unsigned
      OK((std::is_same<unsigned long, unsigned long int>::value));
   }

   Case(integer-long long)
   {
      // signed
      OK((std::is_same<long long, long long int>::value));
      OK((std::is_same<signed long long, long long int>::value));
      OK((std::is_same<signed long long int, long long int>::value));
      // unsigned
      OK((std::is_same<unsigned long long, unsigned long long int>::value));
   }

   Case(bool)
   {
      OK(!(std::is_pointer<bool>::value));
      OK((std::is_integral<bool>::value));
      OK((std::is_arithmetic<bool>::value));
      OK(!(std::is_convertible<bool, std::string>::value));
      OK((std::is_convertible<bool, int>::value));
      OK(!(std::is_convertible<bool, void*>::value));

      OK(!(std::is_same<bool, char>::value));
      OK(!(std::is_same<bool, short>::value));
      OK(!(std::is_same<bool, int>::value));
      OK(!(std::is_same<bool, long>::value));
      OK(!(std::is_same<bool, long long>::value));
   }

   Case(stl)
   {
      OK((std::is_signed<ssize_t>::value));
      OK((std::is_unsigned<size_t>::value));
      OK((std::is_signed<intptr_t>::value));
      OK((std::is_unsigned<uintptr_t>::value));
   }
}

SUITE(type promotion)
{
   Case(int/int)
   {
      int a = 1, b = 2;
      OK((std::is_same<int, decltype(a + b)>::value));
      OK((std::is_same<int, decltype(a - b)>::value));
      OK((std::is_same<int, decltype(a * b)>::value));
      OK((std::is_same<int, decltype(a / b)>::value));
   }

   Case(size_t/size_t)
   {
      size_t a = 1, b = 2;
      OK((std::is_same<size_t, decltype(a + b)>::value));
      OK((std::is_same<size_t, decltype(a - b)>::value));
      OK((std::is_same<size_t, decltype(a * b)>::value));
      OK((std::is_same<size_t, decltype(a / b)>::value));
   }
}

SUITE(const-qualified type)
{
   Case(Common Sense)
   {
      OK((std::is_const<const int>::value));
      OK(!(std::is_const<int>::value));

      OK(!(std::is_same<int, const int>::value));
      OK(!(std::is_same<int, int&>::value));
      OK(!(std::is_same<int, const int&>::value));
   }

   Case(Pointee)
   {
      OK(!(std::is_const<const int*>::value));
      OK(!(std::is_const<int const*>::value));

      // not pointee
      OK((std::is_const<int* const>::value));
   }

   Case(Reference)
   {
      OK(!(std::is_const<const int&>::value));
   }
}

template <typename T, typename = void>
struct TC6 {
   static const char* name() { return "primary"; }
};

template <typename T>
struct TC6<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
   static const char* name() { return "number"; }
};

template <typename T>
struct TC6<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
   static const char* name() { return "float"; }
};

template <typename T>
struct TC6<T*> {
   static const char* name() { return "pointer"; }
};

template <>
struct TC6<const char*> {
   static const char* name() { return "const char*"; }
};

template <typename T, typename = void>
struct TC7 {
   static const char* name() { return "primary"; }
};

template <typename T>
struct TC7<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
   static const char* name() { return "number"; }
};

template <typename T>
struct TC7<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
   static const char* name() { return "float"; }
};

template <typename T>
struct TC7<T, typename std::enable_if<std::is_pointer<T>::value>::type> {
   static const char* name() { return "ptr"; }
};

template <typename T>
struct TC7<T*> {
   static const char* name() { return "pointer"; }
};

template <>
struct TC7<const char*> {
   static const char* name() { return "const char*"; }
};

template <>
struct TC7<char*> {
   static const char* name() { return "char*"; }
};

SUITE(template class candidates)
{
   Case(default)
   {
      OK("primary", TC6<void>::name());
      OK("primary", TC7<std::vector<int>>::name());
   }

   Case(successfull)
   {
      OK("number", TC6<int>::name());
      OK("pointer", TC6<int*>::name());

      OK("number", TC7<int>::name());
   }

   Case(Full specialization is HIGHEST priority)
   {
      OK("const char*", TC6<const char*>::name());
      OK("const char*", TC7<const char*>::name());
   }

   Case(const char* / char* is different)
   {
      OK("pointer", TC6<char*>::name());  // Can't match 'const char*'
      OK("char*", TC7<char*>::name());
   }

   Case(ambiguous, Partial specialization is SAME priority)
   {
      // OK("float", TC7<double>::name());
      // OK("pointer", TC7<int*>::name());
   }
}

template <typename T, typename = void>
const char* tf7(T a) { return "primary"; }

template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type>
const char* tf7(T a) { return "pointer"; }

template <>
const char* tf7<const char*>(const char* a) { return "const char*"; }

template <>
const char* tf7<char*>(char* a) { return "char*"; }

SUITE(template function candidates)
{
   Case(primary)
   {
      OK("primary", tf7(true));
   }

   Case(Partial specialization is not allowed)
   {
      OK(Not("pointer"), tf7((void*)12345678));
   }

   Case(const char* / char* is different)
   {
      OK("const char*", tf7((const char*)""));
      OK("char*", tf7((char*)""));
   }
}

struct TC8 {
   template <typename T, typename = void>
   const char* tf7(T a) { return "primary"; }

   template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type>
   const char* tf7(T a) { return "pointer"; }

   const char* tf7(const char* a) { return "const char*"; }

   const char* tf7(char* a) { return "char*"; }
};

SUITE(template member candidates)
{
   TC8 tc;

   Case(primary)
   {
      OK("primary", tc.tf7(true));
   }

   Case(Partial specialization is not allowed)
   {
      OK(Not("pointer"), tc.tf7((void*)12345678));
   }

   Case(const char* / char* is different)
   {
      OK("const char*", tc.tf7((const char*)""));
      OK("char*", tc.tf7((char*)""));
   }
}

template <typename T>
static const char* type_name(char* name)
{
   strcpy(name, "");
   auto t1 = typeid(T).name();
#if !defined _MSC_VER
   int status = -1;
   char t2[256];
   size_t t3 = 256;
   abi::__cxa_demangle(t1, t2, &t3, &status);
   t1 = t2;
#endif
   strcat(name, t1);
   if (std::is_lvalue_reference<T>::value)
      strcat(name, "_&");
   else if (std::is_rvalue_reference<T>::value)
      strcat(name, "_&&");
   if (std::is_const<T>::value) strcat(name, "_const");
   return name;
}

static char s1[256], s2[256], s3[256];

template <typename T>
static const char* f1(T t) { return sprintf(s1, "t=%s, T=%s", type_name<decltype(t)>(s2), type_name<T>(s3)), s1; }

template <typename T>
static const char* f2(const T t) { return sprintf(s1, "t=%s, T=%s", type_name<decltype(t)>(s2), type_name<T>(s3)), s1; }

template <typename T>
static const char* f3(T& t) { return sprintf(s1, "t=%s, T=%s", type_name<decltype(t)>(s2), type_name<T>(s3)), s1; }

template <typename T>
static const char* f4(const T& t) { return sprintf(s1, "t=%s, T=%s", type_name<decltype(t)>(s2), type_name<T>(s3)), s1; }

CASE(template parameter type qualifiers)
{
   int a = 1;
   int* ap = &a;  // auto ap = &a;
   int& ar = a;
   int&& arr = 2;

   const int ca = 1;      // same as int const ca = 1;
   const int* cap = &ca;  // auto cap = &ca;
   const int& car = ca;
   const int&& carr = 2;

   int* const acp = nullptr;
   const int* const cacp = nullptr;

   int bf0 = 0;
   int bf1 = 0;
   int bf2 = 0;
   int bf3 = 0;
   int bf4 = 0;

   int ba = 0;
   int bap = 0;
   int bar = 0;
   int barr = 0;
   int bca = 0;
   int bcap = 0;
   int bcar = 0;
   int bcarr = 0;

   int bacp = 0;
   int bcacp = 0;

   OK("int", type_name<decltype(a)>(s1));
   OK("t=int, T=int", f1(a));
   OK("t=int_const, T=int", f2(a));
   OK("t=int_&, T=int", f3(a));
   OK("t=int_&, T=int", f4(a));

   if (bf0 || ba) ::printf("                                int a: a=%s\n", type_name<decltype(a)>(s1));
   if (bf1 || ba) ::printf("           f1(T t)              int a: %s\n", f1(a));
   if (bf2 || ba) ::printf("     f2(const T t)              int a: %s\n", f2(a));
   if (bf3 || ba) ::printf("          f3(T& t)              int a: %s\n", f3(a));
   if (bf4 || ba) ::printf("    f4(const T& t)              int a: %s\n", f4(a));
   if (ba) ::printf("\n");

#if defined __APPLE__ || defined __linux__
   OK(Se("int*"), type_name<decltype(ap)>(s1));
   OK(Se("t=int*, T=int*"), f1(ap));
   OK(Se("t=int*_const, T=int*"), f2(ap));
   OK(Se("t=int*_&, T=int*"), f3(ap));
   OK(Se("t=int*_&, T=int*"), f4(ap));
#endif

   if (bf0 || bap) ::printf("                               int* a: %s\n", type_name<decltype(ap)>(s1));
   if (bf1 || bap) ::printf("           f1(T t)             int* a: %s\n", f1(ap));
   if (bf2 || bap) ::printf("     f2(const T t)             int* a: %s\n", f2(ap));
   if (bf3 || bap) ::printf("          f3(T& t)             int* a: %s\n", f3(ap));
   if (bf4 || bap) ::printf("    f4(const T& t)             int* a: %s\n", f4(ap));
   if (bap) ::printf("\n");

   OK("int_&", type_name<decltype(ar)>(s1));
   OK("t=int, T=int", f1(ar));
   OK("t=int_const, T=int", f2(ar));
   OK("t=int_&, T=int", f3(ar));
   OK("t=int_&, T=int", f4(ar));

   if (bf0 || bar) ::printf("                               int& a: %s\n", type_name<decltype(ar)>(s1));
   if (bf1 || bar) ::printf("           f1(T t)             int& a: %s\n", f1(ar));
   if (bf2 || bar) ::printf("     f2(const T t)             int& a: %s\n", f2(ar));
   if (bf3 || bar) ::printf("          f3(T& t)             int& a: %s\n", f3(ar));
   if (bf4 || bar) ::printf("    f4(const T& t)             int& a: %s\n", f4(ar));
   if (bar) ::printf("\n");

   OK("int_&&", type_name<decltype(arr)>(s1));
   OK("t=int, T=int", f1(arr));
   OK("t=int_const, T=int", f2(arr));
   OK("t=int_&, T=int", f3(arr));
   OK("t=int_&, T=int", f4(arr));

   if (bf0 || barr) ::printf("                              int&& a: %s\n", type_name<decltype(arr)>(s1));
   if (bf1 || barr) ::printf("           f1(T t)            int&& a: %s\n", f1(arr));
   if (bf2 || barr) ::printf("     f2(const T t)            int&& a: %s\n", f2(arr));
   if (bf3 || barr) ::printf("          f3(T& t)            int&& a: %s\n", f3(arr));
   if (bf4 || barr) ::printf("    f4(const T& t)            int&& a: %s\n", f4(arr));
   if (barr) ::printf("\n");

   OK("int_const", type_name<decltype(ca)>(s1));
   OK("t=int, T=int", f1(ca));
   OK("t=int_const, T=int", f2(ca));
   OK("t=int_&, T=int_const", f3(ca));
   OK("t=int_&, T=int", f4(ca));

   if (bf0 || bca) ::printf("                          const int a: %s\n", type_name<decltype(ca)>(s1));
   if (bf1 || bca) ::printf("           f1(T t)        const int a: %s\n", f1(ca));
   if (bf2 || bca) ::printf("     f2(const T t)        const int a: %s\n", f2(ca));
   if (bf3 || bca) ::printf("          f3(T& t)        const int a: %s\n", f3(ca));
   if (bf4 || bca) ::printf("    f4(const T& t)        const int a: %s\n", f4(ca));
   if (bca) ::printf("\n");

#if defined __APPLE__ || defined __linux__
   OK(Se("int const*"), type_name<decltype(cap)>(s1));
   OK(Se("t=int const*, T=int const*"), f1(cap));
   OK(Se("t=int const*_const, T=int const*"), f2(cap));
   OK(Se("t=int const*_&, T=int const*"), f3(cap));
   OK(Se("t=int const*_&, T=int const*"), f4(cap));
#endif

   if (bf0 || bcap) ::printf("                         const int* a: %s\n", type_name<decltype(cap)>(s1));
   if (bf1 || bcap) ::printf("           f1(T t)       const int* a: %s\n", f1(cap));
   if (bf2 || bcap) ::printf("     f2(const T t)       const int* a: %s\n", f2(cap));
   if (bf3 || bcap) ::printf("          f3(T& t)       const int* a: %s\n", f3(cap));
   if (bf4 || bcap) ::printf("    f4(const T& t)       const int* a: %s\n", f4(cap));
   if (bcap) ::printf("\n");

   OK("int_&", type_name<decltype(car)>(s1));
   OK("t=int, T=int", f1(car));
   OK("t=int_const, T=int", f2(car));
   OK("t=int_&, T=int_const", f3(car));
   OK("t=int_&, T=int", f4(car));

   if (bf0 || bcar) ::printf("                         const int& a: %s\n", type_name<decltype(car)>(s1));
   if (bf1 || bcar) ::printf("           f1(T t)       const int& a: %s\n", f1(car));
   if (bf2 || bcar) ::printf("     f2(const T t)       const int& a: %s\n", f2(car));
   if (bf3 || bcar) ::printf("          f3(T& t)       const int& a: %s\n", f3(car));
   if (bf4 || bcar) ::printf("    f4(const T& t)       const int& a: %s\n", f4(car));
   if (bcar) ::printf("\n");

   OK("int_&&", type_name<decltype(carr)>(s1));
   OK("t=int, T=int", f1(carr));
   OK("t=int_const, T=int", f2(carr));
   OK("t=int_&, T=int_const", f3(carr));
   OK("t=int_&, T=int", f4(carr));

   if (bf0 || bcarr) ::printf("                        const int&& a: %s\n", type_name<decltype(carr)>(s1));
   if (bf1 || bcarr) ::printf("           f1(T t)      const int&& a: %s\n", f1(carr));
   if (bf2 || bcarr) ::printf("     f2(const T t)      const int&& a: %s\n", f2(carr));
   if (bf3 || bcarr) ::printf("          f3(T& t)      const int&& a: %s\n", f3(carr));
   if (bf4 || bcarr) ::printf("    f4(const T& t)      const int&& a: %s\n", f4(carr));
   if (bcarr) ::printf("\n");

#if defined __APPLE__ || defined __linux__
   OK(Se("int*_const"), type_name<decltype(acp)>(s1));
   OK(Se("t=int*, T=int*"), f1(acp));
   OK(Se("t=int*_const, T=int*"), f2(acp));
   OK(Se("t=int*_&, T=int*_const"), f3(acp));
   OK(Se("t=int*_&, T=int*"), f4(acp));
#endif

   if (bf0 || bacp) ::printf("                         int* const a: %s\n", type_name<decltype(acp)>(s1));
   if (bf1 || bacp) ::printf("           f1(T t)       int* const a: %s\n", f1(acp));
   if (bf2 || bacp) ::printf("     f2(const T t)       int* const a: %s\n", f2(acp));
   if (bf3 || bacp) ::printf("          f3(T& t)       int* const a: %s\n", f3(acp));
   if (bf4 || bacp) ::printf("    f4(const T& t)       int* const a: %s\n", f4(acp));
   if (bacp) ::printf("\n");

#if defined __APPLE__ || defined __linux__
   OK(Se("int const*_const"), type_name<decltype(cacp)>(s1));
   OK(Se("t=int const*, T=int const*"), f1(cacp));
   OK(Se("t=int const*_const, T=int const*"), f2(cacp));
   OK(Se("t=int const*_&, T=int const*_const"), f3(cacp));
   OK(Se("t=int const*_&, T=int const*"), f4(cacp));
#endif

   if (bf0 || bcacp) ::printf("                   const int* const a: %s\n", type_name<decltype(cacp)>(s1));
   if (bf1 || bcacp) ::printf("           f1(T t) const int* const a: %s\n", f1(cacp));
   if (bf2 || bcacp) ::printf("     f2(const T t) const int* const a: %s\n", f2(cacp));
   if (bf3 || bcacp) ::printf("          f3(T& t) const int* const a: %s\n", f3(cacp));
   if (bf4 || bcacp) ::printf("    f4(const T& t) const int* const a: %s\n", f4(cacp));
   if (bcacp) ::printf("\n");
}
