#include "h2unit.h"

extern "C" {
#include "product_c.h"
}

H2UNIT (Json) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(Json, point)
{
   Point point = {111, 222};

   const char* e1 = "{\"x\": 123, \"y\": 222}";
   H2EQ(JsonEq(e1), Point_tojson(&point));
   H2EQ(Je(e1), Point_tojson(&point));
   H2JE(e1, Point_tojson(&point));
}

H2CASE(Json, point 2)
{
   Point point = {111, 222};

   H2JE("{'x': 0, 'y': [{}]}", Point_tojson(&point));
}

H2CASE(Json, expression)
{
   Point point = {111, 222};

   H2JE("{'x': 100 + (10 + 1), 'y':  111 * sqrt(4)}", Point_tojson(&point));
}

H2CASE(Json, Regex)
{
   H2EQ(Je("{'age': 0, 'name': /he[4-9]+/}"), "{'age': 0, 'name': 'he123'}");
}

H2CASE(Json, format)
{
   const char* e1 = "{\"app\": 1234.5678, \"c++\": [1,2,3], \"d\": \"UK\", \"go\": 1.23 }";
   const char* a1 = "{\"app\": \"abcd\", \"b\": 1970, \"c++\": [1,2]}";
   H2JE(e1, a1);
}

H2CASE(Json, format 2)
{
   const char* e1 =
     "{\"widget\": {                                                        \
    \"debug\": \"on\",                                                      \
    \"image\": {                                                            \
        \"src\": \"Images/Sun.png\",                                        \
        \"name\": \"sun1\",                                                 \
        \"hOffset\": 250,                                                   \
        \"vOffset\": 250,                                                   \
        \"alignment\": \"center\"                                           \
    },                                                                      \
    \"text\": {                                                             \
        \"data\": \"Click Here\",                                           \
        \"size\": 36,                                                       \
        \"style\": \"bold\",                                                \
        \"name\": \"text1\",                                                \
        \"hOffset\": 250,                                                   \
        \"vOffset\": 100,                                                   \
        \"alignment\": \"center\",                                          \
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"        \
    }                                                                       \
}}";

   const char* a1 =
     "{\"widget\": {                                                        \
    \"debug\": \"off\",                                                     \
    \"window\": {                                                           \
        \"title\": \"Sample Konfabulator Widget\",                          \
        \"name\": \"main_window\",                                          \
        \"width\": 500,                                                     \
        \"height\": 500                                                     \
    },                                                                      \
    \"image\": {                                                            \
        \"src\": \"Images/Sun.png\",                                        \
        \"name\": \"sun1\",                                                 \
        \"hOffset\": 250,                                                   \
        \"vOffset\": 250,                                                   \
        \"alignnent\": \"center\"                                           \
    },                                                                      \
    \"text\": {                                                             \
        \"data\": \"Click Here\",                                           \
        \"size\": 122,                                                      \
        \"style\": \"bold\",                                                \
        \"name\": \"text1\",                                                \
        \"hOffset\": 250,                                                   \
        \"vOffset\": 100,                                                   \
        \"alignment\": \"center\",                                          \
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"        \
    }                                                                       \
}}";

   H2JE(e1, a1);
}
