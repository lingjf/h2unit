
#include "product_cpp.h"

int Shape::move(int xy)
{
   this->x += xy;
   this->y += xy;
   return 0;
}
