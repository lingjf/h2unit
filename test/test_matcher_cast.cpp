#include "../source/h2_unit.cpp"

SUITE(Cast)
{
   Case()
   {
   }
}

SUITE(Same Number Type)
{
   Case(signed)
   {
      OK(std::is_same<short, short int>::value);
      OK(std::is_same<long, long int>::value);
      OK(std::is_same<long long, long long int>::value);
      OK(std::is_same<signed, int>::value || std::is_same<signed, long int>::value || std::is_same<signed, long long int>::value);
      OK(std::is_same<intptr_t, int>::value || std::is_same<intptr_t, long int>::value || std::is_same<intptr_t, long long int>::value);
      OK(std::is_same<ssize_t, int>::value || std::is_same<ssize_t, long int>::value || std::is_same<ssize_t, long long int>::value);
   }

   Case(unsigned)
   {
      OK((std::is_same<unsigned short, unsigned short int>::value));
      OK((std::is_same<unsigned long, unsigned long int>::value));
      OK((std::is_same<unsigned long long, unsigned long long int>::value));
      OK((std::is_same<unsigned, unsigned int>::value) || (std::is_same<unsigned, unsigned long int>::value) || (std::is_same<unsigned, unsigned long long int>::value));
      OK((std::is_same<uintptr_t, unsigned int>::value) || (std::is_same<uintptr_t, unsigned long int>::value) || (std::is_same<uintptr_t, unsigned long long int>::value));
      OK((std::is_same<size_t, unsigned int>::value) || (std::is_same<size_t, unsigned long int>::value) || (std::is_same<size_t, unsigned long long int>::value));
   }

   Case(const)
   {
      // OK((std::is_same<int, const int>::value));
      // OK((std::is_same<int, int&>::value));
   }
}
