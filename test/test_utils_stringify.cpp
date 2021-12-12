#include "../source/h2_unit.cpp"
#include "test_cplusplus.hpp"
#include "test_types.hpp"

struct Stringify11 {
   int bar(int) { return 0; }
};

struct Stringify12 {
   h2::h2_string tostring() { return "tostring"; }
   h2::h2_string toString() { return "toString"; }
};

struct Stringify13 {
   std::string tostring() { return "std::string toString()"; }
   h2::h2_string toString() { return "h2::h2_string toString"; }
   std::string Tostring(int) { return "std::string Tostring"; }
   int ToString() { return 0; }
   const char* to_string() { return "const char* to_string"; }
};

struct Stringify21 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, Stringify21 a)
{
   return os << "Stringify21";
}

struct Stringify22 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, Stringify22& a)
{
   return os << "Stringify22";
}

struct Stringify23 {
   int bar(int) { return 0; }
};
std::ostream& operator<<(std::ostream& os, const Stringify23& a)
{
   return os << "Stringify23";
}

CASE(tostring able)
{
   bool ret;

   ret = h2::h2_toString_able<Stringify12>::value;
   OK(ret);

   ret = h2::h2_tostring_able<Stringify13>::value;
   OK(ret);
   ret = h2::h2_toString_able<Stringify13>::value;
   OK(ret);
   ret = h2::h2_Tostring_able<Stringify13>::value;
   OK(!ret);
   ret = h2::h2_ToString_able<Stringify13>::value;
   OK(!ret);
   ret = h2::h2_to_string_able<Stringify13>::value;
   OK(ret);

   ret = h2::h2_tostring_able<Stringify11>::value;
   OK(!ret);
   ret = h2::h2_toString_able<Stringify11>::value;
   OK(!ret);
   ret = h2::h2_Tostring_able<Stringify11>::value;
   OK(!ret);
   ret = h2::h2_ToString_able<Stringify11>::value;
   OK(!ret);
   ret = h2::h2_to_string_able<Stringify11>::value;
   OK(!ret);
}

SUITE(ostream able)
{
   bool ret;
   Case(arithmetic type)
   {
      ret = h2::h2_is_ostreamable<char>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<signed char>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<unsigned char>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<uint8_t>::value;
      OK(ret);

      ret = h2::h2_is_ostreamable<short int>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<unsigned short int>::value;
      OK(ret);

      ret = h2::h2_is_ostreamable<int>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<unsigned int>::value;
      OK(ret);

      ret = h2::h2_is_ostreamable<long int>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<unsigned long int>::value;
      OK(ret);

      ret = h2::h2_is_ostreamable<long long int>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<unsigned long long int>::value;
      OK(ret);
   }

   Case(void)
   {
      ret = h2::h2_is_ostreamable<void>::value;
      OK(!ret);
   }

   Case(void*)
   {
      ret = h2::h2_is_ostreamable<void*>::value;
      OK(ret);
   }

   Case(user type)
   {
      ret = h2::h2_is_ostreamable<Stringify11>::value;
      OK(!ret);
      ret = h2::h2_is_ostreamable<Stringify12>::value;
      OK(!ret);
      ret = h2::h2_is_ostreamable<Stringify13>::value;
      OK(!ret);
      ret = h2::h2_is_ostreamable<Stringify21>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<Stringify22>::value;
      OK(ret);
      ret = h2::h2_is_ostreamable<Stringify23>::value;
      OK(ret);
   }

   Case(nullptr_t)
   {
      // https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
      // basic_ostream& operator<<(std::nullptr_t); (since C++ 17)

#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)
      ret = h2::h2_is_ostreamable<std::nullptr_t>::value;
      // OK(ret);
#else
      ret = h2::h2_is_ostreamable<std::nullptr_t>::value;
      // OK(!ret);
#endif
   }
}

template <typename T, typename = void>
struct h2_is_wstring : std::false_type {
};

template <typename T>
struct h2_is_wstring<T, typename std::enable_if<h2::h2_is_string<T>::value && std::is_same<wchar_t, typename T::value_type>::value>::type> : std::true_type {
};

SUITE(h2_is_wstring)
{
   Case(std::wstring)
   {
      OK((h2_is_wstring<std::wstring>::value));
   }

   Case(std::string)
   {
      OK(!(h2_is_wstring<std::string>::value));
   }

   Case(Fundamental types)
   {
      OK(!(h2_is_wstring<int>::value));
   }

#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)
   Case(string_view)
   {
      OK(!(h2_is_wstring<std::string_view>::value));
   }

   Case(wstring_view)
   {
      OK((h2_is_wstring<std::wstring_view>::value));
   }
#endif
}

SUITE(stringify simple)
{
   Case(int)
   {
      OK(ListOf("0"), h2::h2_stringify<int>(0));
      OK(ListOf("1"), h2::h2_stringify<int>(1));
      OK(ListOf("42"), h2::h2_stringify<int>(42));
      OK(ListOf("100"), h2::h2_stringify<int>(100, true));
   }

   Case(-int)
   {
      OK(ListOf("0"), h2::h2_stringify<int>(-0));
      OK(ListOf("-1"), h2::h2_stringify<int>(-1));
      OK(ListOf("-42"), h2::h2_stringify<int>(-42));
      OK(ListOf("-100"), h2::h2_stringify<int>(-100, true));
   }

   Case(+int)
   {
      OK(ListOf("0"), h2::h2_stringify<int>(+0));
      OK(ListOf("1"), h2::h2_stringify<int>(+1));
      OK(ListOf("42"), h2::h2_stringify<int>(+42));
      OK(ListOf("100"), h2::h2_stringify<int>(+100, true));
   }

   Case(hex oct)
   {
      OK(ListOf("0"), h2::h2_stringify<int>(0x0));
      OK(ListOf("1"), h2::h2_stringify<int>(01));
      OK(ListOf("42"), h2::h2_stringify<int>(0x2a));
      OK(ListOf("100"), h2::h2_stringify<int>(0144, true));
   }

   Case(double)
   {
      OK(ListOf("1"), h2::h2_stringify<double>(1.0));
      OK(ListOf("10"), h2::h2_stringify<double>(10.0));

      OK(ListOf("3.14"), h2::h2_stringify<double>(3.14));
      OK(ListOf("3.14"), h2::h2_stringify<double>(3.14, true));
      OK(ListOf("3.14"), h2::h2_stringify<double>(3.140));
      OK(ListOf("3.141593"), h2::h2_stringify<double>(3.1415926));
      OK(ListOf("3.141593"), h2::h2_stringify<double>(3.1415926, true));
   }

   Case(-double)
   {
      OK(ListOf("-3.14"), h2::h2_stringify<double>(-3.14));
      OK(ListOf("-3.14"), h2::h2_stringify<double>(-3.14, true));
      OK(ListOf("-3.141593"), h2::h2_stringify<double>(-3.1415926));
      OK(ListOf("-3.141593"), h2::h2_stringify<double>(-3.1415926, true));
   }

   Case(+double)
   {
      OK(ListOf("3.14"), h2::h2_stringify<double>(+3.14));
      OK(ListOf("3.14"), h2::h2_stringify<double>(+3.14, true));
      OK(ListOf("3.141593"), h2::h2_stringify<double>(+3.1415926));
      OK(ListOf("3.141593"), h2::h2_stringify<double>(+3.1415926, true));
   }

   Case(bool)
   {
      OK(ListOf("true"), h2::h2_stringify<bool>(true));
      OK(ListOf("true"), h2::h2_stringify<bool>(true, true));
      OK(ListOf("false"), h2::h2_stringify<bool>(false));
      OK(ListOf("false"), h2::h2_stringify<bool>(false, true));

      bool bool_false = false;
      const bool const_bool_false = false;
      const bool& const_bool_ref_false = false;
      bool&& bool_rref_false = false;

      h2::h2_stringify(bool_false);
      h2::h2_stringify(const_bool_false);
      h2::h2_stringify(const_bool_ref_false);
      h2::h2_stringify(bool_rref_false);
   }

   Case(long long)
   {
      OK(ListOf("12345678"), h2::h2_stringify<long long>(12345678LL));
      OK(ListOf("123456780"), h2::h2_stringify<long long>(123456780LL));
      OK(ListOf("1234567800"), h2::h2_stringify<long long>(1234567800LL, true));
   }

   Case(short)
   {
      OK(ListOf("1234"), h2::h2_stringify<short>(1234));
   }

   Case(short int)
   {
      OK(ListOf("1234"), h2::h2_stringify<short int>(1234));
   }

   Case(long int)
   {
      OK(ListOf("12345678"), h2::h2_stringify<long int>(12345678));
   }

   Case(long long int)
   {
      OK(ListOf("12345678"), h2::h2_stringify<long long int>(12345678LL));
   }

   Case(float)
   {
      OK(ListOf("3.14"), h2::h2_stringify<float>(3.14));
   }

   Case(long double)
   {
#if !(defined WIN32 && defined __clang__)  //! failed at msys2-clang64
      OK(ListOf("3.14"), h2::h2_stringify<long double>(3.14));
#endif
   }

   Case(enum)
   {
      enum { enum_1234 = 1234 };
      OK(ListOf("1234"), h2::h2_stringify(enum_1234));
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

   Case(signed char)
   {
      signed char a = 'A';
      OK(ListOf("65"), h2::h2_stringify<signed char>(a));
   }

   Case(char*)
   {
      char* a = (char*)"hello world";
      OK("hello world", h2::h2_stringify<char*>(a));
      OK(ListOf("\033{+dark gray}", "\"", "\033{-dark gray}",
                "hello world",
                "\033{+dark gray}", "\"", "\033{-dark gray}"),
         h2::h2_stringify<char*>(a, true));
   }

   Case("char[]")
   {
      char a[100];
      strcpy(a, "hello world");

      OK("hello world", h2::h2_stringify(a));
      OK(ListOf("\033{+dark gray}", "\"", "\033{-dark gray}",
                "hello world",
                "\033{+dark gray}", "\"", "\033{-dark gray}"),
         h2::h2_stringify(a, true));
   }

   Case(char* null)
   {
      char* a = nullptr;
      OK(ListOf("(null)"), h2::h2_stringify<char*>(a));
   }

   Case(const char* null)
   {
      const char* a = nullptr;
      OK(ListOf("(null)"), h2::h2_stringify<const char*>(a));
   }

   Case(string)
   {
      STRING_DECL_LIST;

#define TheCheck(x) OK(ListOf("h2unit"), h2::h2_stringify(x));
      H2Foreach(TheCheck, (STRING_VAR_LIST));
#undef TheCheck
   }

   Case(pointers)
   {
      PTR_FILL_DECL_LIST;
      PTR_NULL_DECL_LIST;

      // #define TheCheck(x) h2::h2_stringify(x);
      //       H2Foreach(TheCheck, PTR_NULL_VAR_LIST2);
      // #undef TheCheck
   }

   Case(unsigned char*)
   {
      unsigned char a1[] = {'h', 'e', 'l', 'l', 'o', 0};
      OK(ListOf("hello"), h2::h2_stringify<unsigned char*>(a1));
      OK(ListOf("hello"), h2::h2_stringify<unsigned char*>(a1, true));

      unsigned char a2[] = {1, 2, 3, 4, 5, 0};
      OK(ListOf("\1\2\3\4\5\0"), h2::h2_stringify<unsigned char*>(a2));
      OK(ListOf("\1\2\3\4\5\0"), h2::h2_stringify<unsigned char*>(a2, true));
   }

   Case(void*)
   {
      void* a = (void*)0x12345678;
      OK(AnyOf(ListOf("0x12345678"), ListOf("0000000012345678"), ListOf("12345678")), h2::h2_stringify<void*>(a));
      OK(AnyOf(ListOf("0x12345678"), ListOf("0000000012345678"), ListOf("12345678")), h2::h2_stringify<void*>(a, true));
   }

   Case(nullptr)
   {
      OK(ListOf("nullptr"), h2::h2_stringify<std::nullptr_t>(nullptr));
      OK(ListOf("nullptr"), h2::h2_stringify<std::nullptr_t>(nullptr, true));
   }

   // https://en.cppreference.com/w/cpp/string/byte/isprint
   Case(uint8_t)
   {
      unsigned char a1 = 7;
      OK(ListOf("7"), h2::h2_stringify<unsigned char>(a1));
      OK(ListOf("7"), h2::h2_stringify<unsigned char>(a1, true));
      uint8_t a2 = 7;
      OK(ListOf("7"), h2::h2_stringify<uint8_t>(a2, true));
   }

   Case(const)
   {
      const int a1 = 42;
      const double a2 = 3.14;
      OK(ListOf("42"), h2::h2_stringify(a1));
      OK(ListOf("3.14"), h2::h2_stringify(a2));
   }

   Case(reference)
   {
      int a1 = 42;
      int& r1 = a1;
      OK(ListOf("42"), h2::h2_stringify(a1));
      OK(ListOf("42"), h2::h2_stringify(r1));

      double a2 = 3.14;
      double& r2 = a2;
      OK(ListOf("3.14"), h2::h2_stringify(a2));
      OK(ListOf("3.14"), h2::h2_stringify(r2));
   }

   Case(wchar_t)
   {
      wchar_t a1 = L'A';
      OK(ListOf("A"), h2::h2_stringify(a1));
   }

   Case(const wchar_t*)
   {
      const wchar_t* a1 = L"ABC";
      OK(ListOf("ABC"), h2::h2_stringify(a1));
   }

   Case(wchar_t*)
   {
      wchar_t* a1 = (wchar_t*)L"ABC";
      OK(ListOf("ABC"), h2::h2_stringify(a1));
   }

   Case(std::wstring)
   {
      std::wstring a1 = L"ABC";
      OK(ListOf("ABC"), h2::h2_stringify(a1));
   }

#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)
   Case(string_view)
   {
      std::string_view a1 = "abc";
      OK("abc", a1);
   }

   Case(wstring_view)
   {
      std::wstring_view a1 = L"abc";
      OK(ListOf("abc"), h2::h2_stringify(a1));
   }
#endif

#if __cplusplus >= 202002L || (defined _MSVC_LANG && _MSVC_LANG >= 202002L)
   Todo(std::u8string)
   {
      std::u8string a1;
      OK(ListOf(""), h2::h2_stringify(a1));
   }
#endif
}

SUITE(stringify exception)
{
   Case(std::exception)
   {
      std::exception a1;
      OK(ListOf(Substr("exception")), h2::h2_stringify(a1));
   }

   Case(std::bad_exception)
   {
      std::bad_exception a1;
      OK(ListOf(Substr("exception")), h2::h2_stringify(a1));
   }

   Case(std::runtime_error)
   {
      UNMEM()
      {
         std::runtime_error a1("test1");
         OK(ListOf("test1"), h2::h2_stringify(a1));
      }
   }

   Case(a_exception)
   {
      a_exception a1;
      OK(ListOf("Test Exception"), h2::h2_stringify(a1));
   }
}

SUITE(stringify user)
{
   Case(toString)
   {
      Stringify12 f2;
      OK(ListOf("tostring"), h2::h2_stringify<Stringify12>(f2));
      OK(ListOf("\033{+dark gray}", "\"", "\033{-dark gray}",
                "tostring",
                "\033{+dark gray}", "\"", "\033{-dark gray}"),
         h2::h2_stringify<Stringify12>(f2, true));
   }

   Case(operator<<)
   {
      Stringify21 f3;
      OK(ListOf("Stringify21"), h2::h2_stringify<Stringify21>(f3, true));
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
         (h2::h2_stringify<std::pair<int, std::string>>(a1)));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         (h2::h2_stringify<std::pair<int, std::string>>(a1, true)));

      std::pair<std::string, int> a2 = std::make_pair(std::string("nine"), 9);
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         (h2::h2_stringify<std::pair<std::string, int>>(a2)));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         (h2::h2_stringify<std::pair<std::string, int>>(a2, true)));
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
         (h2::h2_stringify<std::tuple<const char*, int>>(a2)));
      OK(ListOf("\033{+dark gray}", "(", "\033{-dark gray}",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "nine",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "9",
                "\033{+dark gray}", ")", "\033{-dark gray}"),
         (h2::h2_stringify<std::tuple<const char*, int>>(a2, true)));

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
         (h2::h2_stringify<std::tuple<const char*, int, std::pair<std::string, double>>>(a3)));
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
         (h2::h2_stringify<std::tuple<const char*, int, std::pair<std::string, double>>>(a3, true)));
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
         (h2::h2_stringify<std::map<std::string, int>>(a1)));
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
         (h2::h2_stringify<std::map<std::string, int>>(a1, true)));
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
         (h2::h2_stringify<std::multimap<std::string, int>>(a1)));

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
         (h2::h2_stringify<std::multimap<std::string, int>>(a1, true)));
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

      OK(AnyOf("[(th1, 1), (th2, 2)]", "[(th2, 2), (th1, 1)]"), (h2::h2_stringify<std::unordered_map<std::string, int>>(a1).string()));
   }

   Case(unordered_multimap)
   {
      std::unordered_multimap<std::string, int> a1 = {{std::string("th1"), 1},
                                                      {std::string("th2"), 2}};

      OK(AnyOf("[(th1, 1), (th2, 2)]", "[(th2, 2), (th1, 1)]"), (h2::h2_stringify<std::unordered_multimap<std::string, int>>(a1).string()));
   }
}

SUITE(stringify Container adaptors)
{
   // Container adaptors

   //    stack
   //    queue
   //    priority_queue

   Case(Container adaptors / stack)
   {
      // std::vector<int> a1 = {1, 2, 3};
      std::stack<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

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
   }

   Case(Container adaptors / queue)
   {
      std::queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

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
   }

   Case(Container adaptors / priority_queue)
   {
      std::priority_queue<int> a1;
      a1.push(1);
      a1.push(2);
      a1.push(3);

      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1));

      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",
                "3",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "1",
                "\033{+dark gray}", ", ", "\033{-dark gray}",
                "2",
                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, true));
   }
}

SUITE(h2_array)
{
   int b[] = {1, 2, 3};

   Case(3)
   {
      h2::h2_array<int> a1(b, 3);
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

   Case(0)
   {
      h2::h2_array<int> a1(b, 0);
      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",

                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1));

      OK(ListOf("\033{+dark gray}", "[", "\033{-dark gray}",

                "\033{+dark gray}", "]", "\033{-dark gray}"),
         h2::h2_stringify(a1, true));

      OK("[]", h2::h2_stringify(a1).string());
   }
}
