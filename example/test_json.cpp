#include "../h2unit.h"

extern "C" {
#include "product_c.h"
}

CASE(JSON rectangle failure)
{
   rectangle_t p1 = {111, 222};

   JE("{ 'width': 110,                   \
         'height': 222 * sqrt(4),        \
         'corners': [1,2,3,'4'],         \
         'links': {'left': 1},           \
         'name': /#[1-9]+/,              \
      }",
      rectangle_tojson(&p1))
     << "rectangle error !!";
}
