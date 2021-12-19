
extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

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
   Case(stub local extern function successful [pass])
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

   Case(stub variable arguments function successful [pass])
   {
      rectangle_t p1 = {1, 2};
      rectangle_t p2 = {2, 3};
      rectangle_t p3 = {3, 4};

      OK(1 * 2 + 2 * 3 + 3 * 4, sum_rectangle_area(3, &p1, &p2, &p3));
      STUB(sum_rectangle_area, sum_rectangle_area_fake);
      OK(300, sum_rectangle_area(3, &p1, &p2, &p3));
   }
}

static int Rect_move_fake(Rect* rect, int x, int y)
{
   OK(1, x);
   OK(2, y);
   return 11;
}

static const char* Rect_print_fake(Rect* rect)
{
   OK("Rect", rect->name);
   static char s[256];
   sprintf(s, "Rect(%d,%d,%d,%d)", rect->x, rect->y, rect->width, rect->height);
   return s;
}

SUITE(Stub member method)
{
   Case(normal member function successful [pass])
   {
      STUB(Rect, move, int(int x, int y), Rect_move_fake);
      Rect rect(0, 0, 1, 1);
      OK(11, rect.move(1, 2));
   }

   Case(virtual member function successful [pass])
   {
      STUB(Rect, print, const char*(), Rect_print_fake);
      Rect rect(0, 0, 1, 1);
      OK("Rect(0,0,1,1)", rect.print());
   }
}
