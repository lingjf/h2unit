#include <stdio.h>
#include "product_cpp.h"

int Shape::move(int xy)
{
   this->x += xy;
   this->y += xy;
   return 0;
}

int Rect::serialize(unsigned char* buffer, int length)
{
   return sprintf((char*)buffer, "{%d,%d,%d,%d}", x, y, width, height);
}

void Rect::dump(const char* filename)
{
   FILE* filp = ::fopen(filename, "wb");
   if (filp) {
      fprintf(filp, "{%d,%d,%d,%d}", x, y, width, height);
      fclose(filp);
   }
}
