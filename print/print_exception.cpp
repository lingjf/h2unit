#include "../build/h2unit.hpp"

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

SUITE(exception)
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

CASE(exception uncaught)
{
   throw_a_integer(42);
}

CASE(exception)
{
   try {
      throw_a_exception();
   } catch (...) {
   }
}

#if !defined _MSC_VER

SUITE(exception catch)
{
   Case(no throw)
   {
      Catch()
      {
         throw_a_exception();
      }
   }

   Case(no throw)
   {
      Catch(nothrow)
      {
         throw_a_exception();
      }
   }

   Case(check throw type)
   {
      Catch(const char*)
      {
         throw_a_integer(42);
      }
   }

   Case(check throw type)
   {
      Catch(const char*)
      {
      }
   }

   Case(check throw type and matcher)
   {
      Catch(int, 16)
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
      Catch(a_exception, "hello")
      {
         throw_a_exception();
      }
   }
}

#endif

#endif
