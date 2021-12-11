#include "../source/h2_unit.cpp"
#include "test_types.hpp"
#include <thread>

#if !defined __MINGW32__

void throw_a_exception()
{
   throw a_exception();
}

void throw_a_integer(int a)
{
   throw a;
}

SUITE(try-catch)
{
   Case(exception)
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

CASE(try-catch int)
{
   try {
      throw_a_integer(42);
   } catch (...) {
   }
}

CASES(try-catch int, (1, 2))
{
   try {
      throw_a_integer(x);
   } catch (...) {
   }
}

#if !defined _MSC_VER

SUITE(throw)
{
   Case(no throw)
   {
      Catch()
      {
         int a = 1;
         a++;
      }
   }

   Case(no throw)
   {
      Catch(nothrow)
      {
         int a = 1;
         a++;
      }
   }

   Case(check throw type int)
   {
      Catch(int)
      {
         throw_a_integer(42);
      }
   }

   Case(check throw type and matcher)
   {
      Catch(int, 42)
      {
         throw_a_integer(42);
      }
   }

   Case(check throw type a_exception)
   {
      Catch(a_exception)
      {
         throw_a_exception();
      }
   }

   Case(check throw type a_exception and matcher streq)
   {
      Catch(a_exception, "Test Exception")
      {
         throw_a_exception();
      }
   }

   Case(std::out_of_range)
   {
      Catch(std::out_of_range)
      {
         std::string().at(1);  // generates an std::out_of_range
      }
   }

   Case(std::system_error)
   {
      Catch(std::system_error)
      {
         std::thread().detach();  // attempt to detach a non-thread
      }
   }

   Case(std::invalid_argument)
   {
      Catch(std::invalid_argument)
      {
         std::bitset<4>{"012"};  // Throws: only '0' or '1' expected
      }
   }

   Case(std::overflow_error)
   {
      Catch(std::overflow_error)
      {
         std::bitset<128> x(42);
         x.to_ulong();
         x.flip();
         x.to_ulong();  // throws
      }
   }

   Case(std::regex_error)
   {
      BLOCK(unmem)
      {
         Catch(std::regex_error)
         {
            std::regex re("[a-b][a");
         }
      }
   }

   Todo(std::bad_cast)
   {
      Catch(std::bad_cast)
      {
      }
   }
}
#endif

#endif
