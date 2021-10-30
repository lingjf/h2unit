#include "../source/h2_unit.cpp"

#if !defined __MINGW32__

class a_exception : public std::exception {
   virtual const char* what() const noexcept override
   {
      return "Test Exception";
   }
};

void throw_a_exception()
{
   throw a_exception();
}

void throw_a_integer(int a)
{
   throw a;
}

SUITE(exception)
{
   Case(catch)
   {
      try {
         throw_a_exception();
      } catch (...) {
      }
   }

   Cases(integer, (1, 2))
   {
      try {
         throw_a_integer(x);
      } catch (...) {
      }
   }
}

CASE(exception)
{
   try {
      throw_a_integer(42);
   } catch (...) {
   }
}

CASES(exception, (1, 2))
{
   try {
      throw_a_integer(x);
   } catch (...) {
   }
}
#endif
