#include "../source/h2_unit.cpp"

CASE(JSON failure)
{
   JE("{ 'width': 110,                   \
         'height': 222 * sqrt(4),        \
         'corners': [1,2,3,'4'],         \
         'links': {'left': 1},           \
         'name': /#[1-9]+/,              \
      }",
      "{ 'width': 110,                   \
         'height': 222,                  \
         'corners': [1,2,3,4],           \
         'links': {'left': 1},           \
         'name': 'rect',                 \
      }");
}
