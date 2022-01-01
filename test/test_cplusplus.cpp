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

   ::printf("__cplusplus = %s\n", H2PP_STR(__cplusplus));
#if defined _MSC_VER
   // __cplusplus always 199711L.
   // _MSVC_LANG specifies the C++ language standard targeted by the compiler.
   // The macro is the integer literal value 201402L by default,
   // or when the /std:c++14 compiler option is specified.
   // The macro is set to 201703L if the /std:c++17 compiler option is specified.
   // The macro is set to 202002L if the /std:c++20 compiler option is specified.
   // It's set to a higher, unspecified value when the /std:c++latest option is specified.
   // Otherwise, the macro is undefined.
   // https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
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
