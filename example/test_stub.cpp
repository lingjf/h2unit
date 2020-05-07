#include "h2unit.h"

extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

static int rectangle_area_fake(rectangle_t* rectangle)
{
   return 100;
}

static int sum_rectangle_area_fake(int count, ...)
{
   return 300;
}

SUITE(Stub function by fake function)
{
   Case(stub local extern function successful)
   {
      rectangle_t p1 = {2, 3};
      OK(6, rectangle_area(&p1));
      STUB(rectangle_area, rectangle_area_fake);
      OK(100, rectangle_area(&p1));
   }

   Todo(stub local static function successful)
   {
      /* include impl source file, then static function is accessible from here */
   }

   Case(stub variable arguments function successful)
   {
      rectangle_t p1 = {1, 2};
      rectangle_t p2 = {2, 3};
      rectangle_t p3 = {3, 4};

      OK(1 * 2 + 2 * 3 + 3 * 4, sum_rectangle_area(3, &p1, &p2, &p3));
      STUB(sum_rectangle_area, sum_rectangle_area_fake);
      OK(300, sum_rectangle_area(3, &p1, &p2, &p3));
   }
}

SUITE(Stub function by lambda)
{
   Case(stub local extern function successful)
   {
      rectangle_t p1 = {2, 3};
      OK(6, rectangle_area(&p1));
      STUB(int, rectangle_area, (rectangle_t * rectangle)) { return 111; };
      OK(111, rectangle_area(&p1));
   }
}

#if (defined(__GNUC__) && __GNUC__ >= 5) || defined __clang__ || defined _WIN32

SUITE(Stub Member method)
{
   Case(normal member function successful)
   {
      STUB(int, Dog, go, (int x, int y))
      {
         OK(1, x);
         OK(2, y);
         return 11;
      };
      STUB(void, Dog, run, ()) { OK(1, that->age); };
      Dog dog(1);
      OK(11, dog.go(1, 2));
      dog.run();
   }

   Case(virtual member function successful)
   {
      STUB(const char*, Cat, say, ()) { return "mmm..."; };
      Cat cat(nullptr, nullptr);
      OK("mmm...", cat.say());
   }

   Case(virtual member function failure)
   {
      STUB(const char*, Centipede, say, ())  // failure
      {
         return "...";
      };
      Centipede centipede(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK("...", centipede.say());
   }

   Case(virtual member function successful)
   {
      STUB(const char*,
           Centipede,
           say,
           (),
           Centipede(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11))
      {
         return "...";
      };
      Centipede centipede(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK("...", centipede.say());
   }

   Case(abstract class failure)
   {
      STUB(int, Ovipara, cry, ())  // failure
      {
         return 6;
      };

      Bird bird;
      OK(6, bird.cry());
   }
}

#endif
