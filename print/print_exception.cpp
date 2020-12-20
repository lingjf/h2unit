#include "../build/h2unit.hpp"

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

#ifndef __APPLE__

SUITE(exception)
{
   Case(integer)
   {
      throw_a_integer();
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

#endif

CASE(exception)
{
   try {
      throw_a_exception();
   } catch (...) {
   }
}
