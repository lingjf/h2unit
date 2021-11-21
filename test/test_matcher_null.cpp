#include "../source/h2_unit.cpp"

CASE(nullptr type traits)
{
   OK((!std::is_pointer<std::nullptr_t>::value));
   OK((!std::is_integral<std::nullptr_t>::value));
   OK((!std::is_arithmetic<std::nullptr_t>::value));
   OK((std::is_convertible<std::nullptr_t, h2::h2_string>::value));
   OK((!std::is_convertible<std::nullptr_t, int>::value));
   OK((std::is_convertible<std::nullptr_t, void*>::value));
}

CASE(NULL type traits)
{
   // https://en.cppreference.com/w/cpp/types/NULL
   // #define NULL /*implementation-defined*/
   // gcc: long
   // clang: long
   // MSVC: int
   // msys2: long long / int

   OK((std::is_same<decltype(NULL), long>::value || std::is_same<decltype(NULL), int>::value || std::is_same<decltype(NULL), long long>::value));
}

SUITE(NULL)
{
   Case(NULL)
   {
      OK(NULL, 0);
      OK(nullptr, 0);

      const char* a1 = nullptr;
      OK(NULL, a1);
      OK(nullptr, a1);

      int* a2 = nullptr;
      OK(NULL, a2);
      OK(nullptr, a2);

      void* a3 = nullptr;
      OK(NULL, a3);
      OK(nullptr, a3);

      std::shared_ptr<int> a4;
      OK(NULL, a4);
      OK(nullptr, a4);
   }

   Case(Not NULL)
   {
      OK(Not(NULL), 1234);
      OK(Not(nullptr), 1234);

      const char* a1 = "";
      OK(Not(NULL), a1);
      OK(Not(nullptr), a1);

      int* a2 = (int*)1234;
      OK(Not(NULL), a2);
      OK(Not(nullptr), a2);

      void* a3 = (void*)1234;
      OK(Not(NULL), a3);
      OK(Not(nullptr), a3);
   }

   Case(Nq NULL)
   {
      OK(Nq(NULL), 1234);
      OK(Nq(nullptr), 1234);

      const char* a1 = "";
      OK(Nq(NULL), a1);
      OK(Nq(nullptr), a1);

      int* a2 = (int*)1234;
      OK(Nq(NULL), a2);
      OK(Nq(nullptr), a2);

      void* a3 = (void*)1234;
      OK(Nq(NULL), a3);
      OK(Nq(nullptr), a3);
   }

   Case(NULL nullptr)
   {
      OK(NULL, nullptr);
   }

   Case(nullptr NULL)
   {
      OK(nullptr, NULL);
   }
}
