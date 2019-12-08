#include "../source/h2_unit.h"

#include <time.h>

H2UNIT (h2_stub) {
   void setup()
   {
   }

   void teardown()
   {
   }

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

};

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

H2CASE(h2_stub, internal stub libc)
{
   h2_stub stub;

   H2EQ(stub.save((void*)time));
   stub.set((void*)STUB_time);

   H2EQ(1024, time(NULL));

   stub.restore();
}

H2CASE(h2_stub, internal stub local)
{
   h2_stub stub;

   H2EQ(stub.save((void*)my_time));
   stub.set((void*)STUB_time);

   H2EQ(1024, my_time(NULL));

   stub.restore();
}

H2CASE(h2_stub, local function)
{
   H2STUB(my_time, STUB_time);

   H2EQ(1024, my_time(NULL));
}

H2CASE(h2_stub, stub libc function)
{
   H2STUB(time, STUB_time);

   H2EQ(1024, time(NULL));
}

H2CASE(h2_stub, variadic parameters function)
{
   H2EQ(6, my_sum(3, 1, 2, 3));
   H2EQ(-6, STUB_sum(3, 1, 2, 3));

   H2STUB(my_sum, STUB_sum);

   H2EQ(-6, my_sum(3, 1, 2, 3));
}

time_t STUB1_time(time_t* x)
{
   return 111;
}
time_t STUB2_time(time_t* x)
{
   return 222;
}

H2CASE(h2_stub, stub - chain)
{
   H2STUB(time, STUB1_time);
   H2STUB(time, STUB2_time);

   H2EQ(222, time(NULL));
}

H2CASE(h2_stub, stub, overwrite)
{
   H2STUB(time, STUB1_time);
   H2STUB(STUB1_time, STUB2_time);

   H2EQ(222, time(NULL));
}

static int foobar(int, int)
{
   return 1;
}

H2CASE(h2_stub, lambdas normal function)
{
   H2STUB(int, foobar, (int a, int b))
   {
      return a + b;
   };

   H2EQ(222, foobar(111, 111));
}

// H2CASE(h2_stub, lambdas variadic parameters function)
// {
//    H2STUB(int, my_sum, (int count, ...))
//    {
//       return -111;
//    };

//    H2EQ(-111, my_sum(3, 1, 2, 3));
// }


H2CASE(h2_stub, lambdas normal member function)
{
   H2STUB(int, Shape, go, (int a, int b))
   {
      return a + b;
   };

   Shape shape;
   H2EQ(222, shape.go(111, 111));
}

H2CASE(h2_stub, lambdas virtual member function)
{
   H2STUB(int, Shape, work, (int a, int b))
   {
      return a + b;
   };

   Shape shape;
   H2EQ(222, shape.work(111, 111));
}

H2CASE(h2_stub, lambdas static member function)
{
   H2STUB(int, Shape::fly, (int a, int b))
   {
      return a + b;
   };

   H2EQ(222, Shape::fly(111, 111));
}
