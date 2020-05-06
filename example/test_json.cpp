#include "h2unit.h"

extern "C"
{
#include "product_c.h"
}

SUITE(Json)
{
  Case(point failure)
  {
    rectangle_t p1 = { 111, 222 };

    const char* e1 = "{\"width\": 123, \"y\": 222}";
    OK(Je(e1), rectangle_tojson(&p1));
    JE(e1, rectangle_tojson(&p1));
  }

  Case(point 2 failure)
  {
    rectangle_t p1 = { 111, 222 };

    JE("{'width': 0, 'height': [{}]}", rectangle_tojson(&p1)) << "JSON !!";
  }

  Case(expression successful)
  {
    rectangle_t p1 = { 111, 222 };

    JE("{'width': 100 + (10 + 1), 'height':  111 * sqrt(4)}",
       rectangle_tojson(&p1));
  }

  Case(Regex failure)
  {
    OK(Je("{'age': 0, 'name': /he[4-9]+/}"), "{'age': 0, 'name': 'he123'}");
  }

  Case(format failure)
  {
    const char* e1 =
      "{\"app\": 1234.5678, \"c++\": [1,2,3], \"d\": \"UK\", \"go\": 1.23 }";
    const char* a1 = "{\"app\": \"abcd\", \"b\": 1970, \"c++\": [1,2]}";
    JE(e1, a1);
  }

  Case(format 2 failure)
  {
    const char* e1 =
      "{\"widget\": {                                                     \
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
      "{\"widget\": {                                                     \
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

    JE(e1, a1);
  }
}
