#include "../source/h2_unit.cpp"

SUITE(json)
{
   Case(object)
   {
      JE("{ 'width': 110,                \
         'height': 222 * sqrt(4),        \
         'corners': [1,2,3,'4'],         \
         'links': {'left': 1},           \
         'name': /#[1-9]+/,              \
      }",
         "{ 'width': 110,                \
         'height': 222,                  \
         'corners': [1,2,3,4],           \
         'links': {'left': 1},           \
         'name': 'rect',                 \
      }");
   }

   Case(case sensitive)
   {
      OK(Je("{'name': /hello.*world/, 'Age': 18}"), "{'name': \"Hello World\", 'age': 18}");
   }

   Case(caseless)
   {
      OK(~Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 20}");
   }
}
