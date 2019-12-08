#include "h2unit.h"
#include <stdlib.h>

extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

/*
 * h2unit can replace function at runtime dynamically.
 * Which this feature, it is helpful to stub functions.
 *
 * H2STUB(function_pointer, new_function)
 *
 */

H2UNIT (stub_in_c) {
   void setup() {}

   void teardown() {}
};

int stub_foo1(int a)
{
   return a + 1;
}
int stub_foo2(int a)
{
   return a + 2;
}
int stub_foo3(int a)
{
   return a + 3;
}

H2CASE(stub_in_c, stub local extern function)
{
   H2EQ(1, getSum(0));
   H2STUB(orig_foo, stub_foo1);
   H2EQ(2, getSum(0));
}

int stub_bar(int b)
{
   return b + 2;
}
H2CASE(stub_in_c, stub local static function)
{
   H2EQ(1, getSum(0));
   H2STUB(orig_bar, stub_bar);
   H2EQ(2, getSum(0));
}

int stub_atoi(const char* s)
{
   return 1;
}

H2CASE(stub_in_c, stub dynamic libc function)
{
   H2EQ(0, isLegal("0"));
   H2STUB(atoi, stub_atoi);
   H2EQ(1, isLegal("0"));
}

char* stub_varg(const char* format, ...)
{
   return (char*)"h2unit hello";
}

H2CASE(stub_in_c, stub variable arguments function)
{
   H2EQ((char*)"hello h2unit", orig_varg("hello %s", "h2unit"));
   H2STUB(orig_varg, stub_varg);
   H2EQ((char*)"h2unit hello", orig_varg("hello %s", "h2unit"));
}

H2UNIT (check_with_stub) {
   void setup() {}
   void teardown() {}
};

int stub_foo_mock1(int a)
{
   H2EQ(1, a); /* check the input parameter */
   return 5;   /* return wanted return value */
}

H2CASE(check_with_stub, act mock)
{
   H2STUB(orig_foo, stub_foo_mock1);
   H2EQ(7, getSum(1));
   H2EQ(8, getSum(2));
}

int stub_foo_mock2(int a)
{
   static int call_seq = 0;
   call_seq++;

   if (call_seq == 1) {
      H2EQ(1, a); /* check the input parameter */
      return 1;   /* return wanted return value */
   }
   else if (call_seq == 2) {
      H2EQ(2, a); /* check the input parameter */
      return 2;   /* return wanted return value */
   }
   else {
      H2EQ(call_seq, a); /* check the input parameter */
      return 3;          /* return wanted return value */
   }

   return 0;
}

H2CASE(check_with_stub, act mock n call)
{
   H2STUB(orig_foo, stub_foo_mock2);
   H2EQ(3, getSum(1));
   H2EQ(5, getSum(2));

   for (int i = 3; i < 10; i++) {
      H2EQ(i + 1 + 3, getSum(i));
   }
}

H2UNIT (Stub_Member) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(Stub_Member, normal member function)
{
   H2STUB(int, Dog, go, (int x, int y))
   {
      H2EQ(1, x);
      H2EQ(2, y);
      return 11;
   };
   H2STUB(void, Dog, run, ())
   {
      H2EQ(1, that->age);
   };
   Dog dog(1);
   H2EQ(11, dog.go(1, 2));
   dog.run();
}

H2CASE(Stub_Member, virtual member function)
{
   H2STUB(const char*, Cat, say, ())
   {
      return "mmm...";
   };
   Cat cat(nullptr, nullptr);
   H2EQ("mmm...", cat.say());
}

H2CASE(Stub_Member, virtual member function failed)
{
   H2STUB(const char*, Centipede, say, ()) /* Failed here */
   {
      return "...";
   };
   Centipede centipede(1, 2, 3, 4, 5, 6, 7, 8, 9);
   H2EQ("...", centipede.say());
}

H2CASE(Stub_Member, virtual member function ok)
{
   H2STUB(const char*, Centipede, say, (), Centipede(1, 2, 3, 4, 5, 6, 7, 8, 9))
   {
      return "...";
   };
   Centipede centipede(1, 2, 3, 4, 5, 6, 7, 8, 9);
   H2EQ("...", centipede.say());
}

H2CASE(Stub_Member, abstract class failed)
{
   H2STUB(int, Ovipara, cry, ()) /* Failed here */
   {
      return 6;
   };

   Bird bird;
   H2EQ(6, bird.cry());
}
