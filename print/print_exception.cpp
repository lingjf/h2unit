#include "../build/h2unit.hpp"

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

void throw_a_integer()
{
   throw 42;
}

SUITE(exception)
{
   Case(integer)
   {
      throw 42;
   }

   Case(catched)
   {
      throw_a_exception();
   }
}

CASE(exception uncaught)
{
   throw_a_integer();
}

CASE(exception)
{
   try {
      throw_a_exception();
   } catch (...) {
   }
}

#endif

