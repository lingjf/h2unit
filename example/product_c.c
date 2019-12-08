
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "product_c.h"

int getEven(int var)
{
   return var & (~1);
}

double getCeil(double var)
{
   return (double)((int)var);
}

Point* create_point(int x, int y)
{
   Point* p = (Point*)malloc(sizeof(Point));
   p->x = x;
   p->y = y;
   return p;
}

const char* Point_toString(Point* point)
{
   static char buffer[1024];
   sprintf(buffer, "Point(%d, %d)", point->x, point->y);
   return buffer;
}

const char* Point_tojson(Point* point)
{
   static char buffer[1024];
   sprintf(buffer, "{\"x\": %d, \"y\": %d}", point->x, point->y);
   return buffer;
}

int orig_foo(int a)
{
   return a;
}

int orig_bar(int b)
{
   return b + 1;
}

char* orig_varg(const char* format, ...)
{
   static char t[1024 * 8];
   va_list args;
   va_start(args, format);

   vsprintf(t, format, args);
   va_end(args);
   return t;
}

int getSum(int x)
{
   return orig_foo(x) + orig_bar(x);
}

int isLegal(const char* str)
{
   return atoi(str);
}
