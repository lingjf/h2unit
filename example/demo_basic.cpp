
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "demo_basic.h"

int getEven(int var)
{
   return var & (~1);
}

double getCeil(double var)
{
   return (double) ((int) var);
}

Point::Point()
{
   m_x = 0;
   m_y = 0;
}

Point::Point(int x, int y)
{
   m_x = x;
   m_y = y;
}

Point::~Point()
{
}

char* Point::tuString()
{
   static char buffer[1024];
   sprintf(buffer, "Point(%d, %d)", m_x, m_y);
   return buffer;
}

char* getTail(char* str)
{
   assert(str != NULL);

   return str + strlen(str);
}

