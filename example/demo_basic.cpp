
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Parser::Parser()
{
}

int Parser::fromInt(const char* str)
{
   int s = 0;
   for(char* p = (char*)str; p && *p; p++) {
      if (*p == '-') {
         throw 1;
      }
      if (*p < '0' || *str > '9') {
         throw 2;
      }

      s = s * 10 + *p - '0';
   }
   return s;
}


