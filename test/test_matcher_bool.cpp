#include "../source/h2_unit.cpp"

CASE(bool type traits)
{
   OK((!std::is_pointer<bool>::value));
   OK((std::is_integral<bool>::value));
   OK((std::is_arithmetic<bool>::value));
   OK((!std::is_convertible<bool, h2::h2_string>::value));
   OK((std::is_convertible<bool, int>::value));
   OK((!std::is_convertible<bool, void*>::value));

   OK("bool", h2::h2_cxa::demangle(typeid(bool).name()));

   OK((!std::is_same<bool, char>::value));
   OK((!std::is_same<bool, short>::value));
   OK((!std::is_same<bool, int>::value));
   OK((!std::is_same<bool, long>::value));
}

SUITE(bool)
{
   Case(bool)
   {
      OK(true, true);
      OK(false, false);
   }

   Case(int)
   {
      OK(true, 1);
      OK(true, 2);
      OK(false, 0);

      OK(Not(true), 0);
      OK(Not(false), 1);
   }

   Case(ptr)
   {
      const char* a1 = "";
      OK(true, a1);
      a1 = nullptr;
      OK(false, a1);

      int* a2 = (int*)1234;
      OK(true, a2);
      a2 = nullptr;
      OK(false, a2);

      void* a3 = (void*)1234;
      OK(true, a3);
      a3 = nullptr;
      OK(false, a3);
   }
}
