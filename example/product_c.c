#ifdef _MSC_VER
#   ifndef _CRT_SECURE_NO_WARNINGS
#      define _CRT_SECURE_NO_WARNINGS
#   endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "product_c.h"

rectangle_t* rectangle_create(int width, int height)
{
   rectangle_t* rectangle = (rectangle_t*)malloc(sizeof(rectangle_t));
   if (rectangle == NULL) return NULL;

   rectangle->width = width;
   rectangle->height = height;
   return rectangle;
}

void rectangle_destroy(rectangle_t* rectangle)
{
   if (rectangle != NULL) free(rectangle);
}

int rectangle_area(rectangle_t* rectangle)
{
   return rectangle->width * rectangle->height;
}

double rectangle_diag(rectangle_t* rectangle)
{
   return sqrt(rectangle->width * rectangle->width + rectangle->height * rectangle->height);
}

const char* rectangle_tostring(rectangle_t* rectangle)
{
   static char buffer[1024];
   sprintf(buffer, "Rect(%d, %d)", rectangle->width, rectangle->height);
   return buffer;
}

const char* rectangle_tojson(rectangle_t* rectangle)
{
   static char buffer[1024];
   sprintf(buffer, "{\"width\": %d, \"height\": %d, \"corners\": [1,2,3,4],\"edges\": {}, \"name\": \"#123\"}", rectangle->width, rectangle->height);
   return buffer;
}

int sum_rectangle_area(int count, ...)
{
   int i, s = 0;
   rectangle_t* p;
   va_list a;
   va_start(a, count);
   for (i = 0; i < count; ++i) {
      p = va_arg(a, rectangle_t*);
      s += rectangle_area(p);
   }
   va_end(a);

   return s;
}
