#include "../source/h2_unit.h"

time_t STUB_time(time_t* x)
{
   return 1024;
}

time_t my_time(time_t* x)
{
   return time(NULL);
}

static int STUB_sum(int count, ...)
{
   int s = 0;
   int i = 0;
   va_list a;
   va_start(a, count);
   for (; i < count; ++i) {
      s += va_arg(a, int);
   }
   va_end(a);

   return -s;
}

static int my_sum(int count, ...)
{
   int s = 0;
   int i = 0;
   va_list a;
   va_start(a, count);
   for (; i < count; ++i) {
      s += va_arg(a, int);
   }
   va_end(a);

   return s;
}

time_t STUB1_time(time_t* x)
{
   return 111;
}
time_t STUB2_time(time_t* x)
{
   return 222;
}

static int foobar(int, int)
{
   return 1;
}

SUITE(stubs)
{
   class Shape
   {
    private:
      int x, y;

    public:
      Shape()
        : x(0), y(0) {}

      int value;

      static int fly(int x, int y)
      {
         return 0;
      }

      int go(int x, int y)
      {
         this->x += x;
         this->y += y;
         return 0;
      }

      int go(int xy)
      {
         this->x += xy;
         this->y += xy;
         return 0;
      }

      virtual int work(int x, int y)
      {
         this->x *= x;
         this->y *= y;
         return 0;
      }
   };

   Case(internal stub libc)
   {
      h2_stub s((void*)time);

      s.replace((void*)STUB_time);

      OK(1024, time(NULL));

      s.restore();
   };

   Case(internal stub local)
   {
      h2_stub s((void*)my_time);

      s.replace((void*)STUB_time);

      OK(1024, my_time(NULL));

      s.restore();
   };

   Case(local function)
   {
      STUB(my_time, STUB_time);

      OK(1024, my_time(NULL));
   };

   Case(stub libc function)
   {
      STUB(time, STUB_time);

      OK(1024, time(NULL));
   };

   Case(variadic parameters function)
   {
      OK(6, my_sum(3, 1, 2, 3));
      OK(-6, STUB_sum(3, 1, 2, 3));

      STUB(my_sum, STUB_sum);

      OK(-6, my_sum(3, 1, 2, 3));
   };

   Case(stub - chain)
   {
      STUB(time, STUB1_time);
      STUB(time, STUB2_time);

      OK(222, time(NULL));
   };

   Case(stub, overwrite)
   {
      STUB(time, STUB1_time);
      STUB(STUB1_time, STUB2_time);

      OK(222, time(NULL));
   };

   Case(lambdas normal function)
   {
      STUB(int, foobar, (int a, int b))
      {
         return a + b;
      };

      OK(222, foobar(111, 111));
   };

   //  Case( lambdas variadic parameters function)
   // {
   //    STUB(int, my_sum, (int count, ...))
   //    {
   //       return -111;
   //    };

   //    OK(-111, my_sum(3, 1, 2, 3));
   // }

   Case(lambdas normal member function)
   {
      STUB(int, Shape, go, (int a, int b))
      {
         return a + b;
      };

      Shape shape;
      OK(222, shape.go(111, 111));
   };

   Case(lambdas virtual member function)
   {
      STUB(int, Shape, work, (int a, int b))
      {
         return a + b;
      };

      Shape shape;
      OK(222, shape.work(111, 111));
   };

   Case(lambdas static member function)
   {
      STUB(int, Shape::fly, (int a, int b))
      {
         return a + b;
      };

      OK(222, Shape::fly(111, 111));
   };
}

SUITE(Stub in setup)
{
   Setup()
   {
      STUB(time, STUB_time);
      STUB(int, foobar, (int a, int b))
      {
         return a + b;
      };
   };

   Teardown(){};

   Case(a)
   {
      OK(1024, time(NULL));
      OK(222, foobar(111, 111));
   };

   Case(b)
   {
      OK(1024, time(NULL));
      OK(222, foobar(111, 111));
   };
}

SUITE(Stub in shared_code)
{
   STUB(time, STUB_time);
   STUB(int, foobar, (int a, int b))
   {
      return a + b;
   };

   Setup(){};

   Teardown(){};

   Case(a)
   {
      OK(1024, time(NULL));
      OK(222, foobar(111, 111));
   };

   Case(b)
   {
      OK(1024, time(NULL));
      OK(222, foobar(111, 111));
   };
}