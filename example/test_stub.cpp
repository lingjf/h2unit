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
      STUB(rectangle_area, int, (rectangle_t * rectangle)) { return 111; };
      OK(111, rectangle_area(&p1));
   }
}

SUITE(Stub member method)
{
   Case(normal member function successful)
   {
      STUB(Rect, move, int, (int x, int y))
      {
         OK(1, x);
         OK(2, y);
         return 11;
      };

      Rect rect(0, 0, 1, 1);
      OK(11, rect.move(1, 2));
   }

   Case(virtual member function successful)
   {
      STUB(Rect, print, const char*, ())
      {
         OK("Rect", that->name);
         static char s[256];
         sprintf(s, "Rect(%d,%d,%d,%d)", that->x, that->y, that->width, that->height);
         return s;
      };
      Rect rect(0, 0, 1, 1);
      OK("Rect(0,0,1,1)", rect.print());
   }
}
