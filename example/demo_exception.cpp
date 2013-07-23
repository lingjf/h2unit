
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "demo_exception.h"

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

