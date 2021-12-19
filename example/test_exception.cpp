#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

#if !defined __MINGW32__

class a_exception : public std::exception {
 public:
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

SUITE(exception [fail])
{
   Case(integer)
   {
      throw 42;
   }

   Case(a_exception)
   {
      throw_a_exception();
   }

   Case(runtime_error)
   {
      throw std::runtime_error("test1");
   }
}

CASE(exception uncaught [fail])
{
   throw_a_integer(42);
}

CASE(exception [pass])
{
   try {
      throw_a_exception();
   } catch (...) {
   }
}

#if !defined _MSC_VER

SUITE(exception catch)
{
   Case(no throw [pass])
   {
      Catch()
      {
      }
   }

   Case(no throw [fail])
   {
      Catch()
      {
         throw_a_exception();
      }
   }

   Case(no throw [fail])
   {
      Catch(nothrow)
      {
         throw_a_exception();
      }
   }

   Case(check throw type [fail])
   {
      Catch(const char*)
      {
         throw_a_integer(42);
      }
   }

   Case(check throw type [fail])
   {
      Catch(const char*)
      {
      }
   }

   Case(check throw type and matcher [fail])
   {
      Catch(int, 16)
      {
         throw_a_integer(42);
      }
   }

   Case(check throw type a_exception [pass])
   {
      Catch(a_exception)
      {
         throw_a_exception();
      }
   }

   Case(check throw type a_exception and matcher streq [fail])
   {
      Catch(a_exception, "hello")
      {
         throw_a_exception();
      }
   }
}

#endif

#endif
