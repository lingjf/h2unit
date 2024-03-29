#include "../source/h2_unit.cpp"
#include "test_types.hpp"

const char* week_json = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
const char* day_json = "{\"am\": [0, 12], \"pm\": {\"afternoon\": 18, \"midnight\": 24}}";

SUITE(json parse)
{
   Case(number)
   {
      const char* n1 = "-123.456";
      h2::h2_json_tree c1(n1);
      OK(c1.is_number());
      OK(-123.456, c1.value_double);

      const char* n2 = "0";
      h2::h2_json_tree c2(n2);
      OK(c2.is_number());
      OK(0, c2.value_double);

      const char* n3 = "-1";
      h2::h2_json_tree c3(n3);
      OK(c3.is_number());
      OK(-1, c3.value_double);

      const char* n4 = "12345678";
      h2::h2_json_tree c4(n4, 4);
      OK(c4.is_number());
      OK(1234, c4.value_double);

      const char* n5 = "3+2*(4-sqrt(4))";
      h2::h2_json_tree c5(n5);
      OK(c5.is_number());
      OK(7, c5.value_double);
   }

   Case(number error)
   {
      const char* n1 = "";
      h2::h2_json_tree c1(n1);
      OK(h2::h2_json_node::t_absent, c1.type);

      const char* n2 = "-";
      h2::h2_json_tree c2(n2);
      OK(h2::h2_json_node::t_string, c2.type);
   }

   Case(string)
   {
      const char* n1 = "\"\"";
      h2::h2_json_tree c1(n1);
      OK(c1.is_string());
      OK("", c1.value_string);

      const char* n2 = "\"12345678\"";
      h2::h2_json_tree c2(n2);
      OK(c2.is_string());
      OK("12345678", c2.value_string);

      const char* n3 = "'12345678'";
      h2::h2_json_tree c3(n3);
      OK(c2.is_string());
      OK("12345678", c3.value_string);
   }

   Case(string error)
   {
      h2::h2_json_tree c1("\"12345678\"", 4);
      OK(h2::h2_json_node::t_string, c1.type);
   }

   Case(pattern)
   {
      h2::h2_json_tree c1("/123*567/");
      OK(c1.is_pattern());
      OK(Se("123*567"), c1.value_string);
   }

   Case(empty array)
   {
      h2::h2_json_tree c1(" []");
      OK(c1.is_array());
      OK(0, c1.size());
   }

   Case(normal array)
   {
      const char* week = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";

      h2::h2_json_tree c1(week);
      JE(week_json, node_dump(&c1));
   }

   Case(extend array)
   {
      const char* week = "['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday']";

      h2::h2_json_tree c1(week);
      JE(week_json, node_dump(&c1));
   }

   Case(javascript array)
   {
      const char* week = "[Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday]";

      h2::h2_json_tree c1(week);
      JE(week_json, node_dump(&c1));
   }

   Case(empty object)
   {
      h2::h2_json_tree c1(" {  }");
      OK(c1.is_object());
      OK(0, c1.size());
   }

   Case(normal object)
   {
      const char* obj = "{                                              \
        \"data\": 'Click Here',                                         \
        'size': 36,                                                     \
        \"alignment\": true,                                            \
        \"bold\": false,                                                \
        \"token\": null,                                                \
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"    \
      }";

      h2::h2_json_tree c(obj);

      JE(obj, node_dump(&c));
   }

   Case(javascript object)
   {
      const char* obj = "{                                     \
        data: Click Here,                                      \
        size: 36,                                              \
        alignment: true,                                       \
        bold: false,                                           \
        token: null,                                           \
        onMouseUp: sun1.opacity = (sun1.opacity / 100) * 90;   \
      }";

      h2::h2_json_tree c(obj);
      JE("{                                                                 \
            \"data\": \"Click Here\",                                       \
            \"size\": 36,                                                   \
            \"alignment\": true,                                            \
            \"bold\": false,                                                \
            \"token\": null,                                                \
            \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"    \
         }",
         node_dump(&c));
   }
}

SUITE(select json)
{
   const char* week = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
   const char* day = "{\"am\": [0, 12], \"pm\": {\"afternoon\": 18, \"midnight\": 24}}";

   Case(all)
   {
      h2::h2_json_tree c1(week);
      JE(week_json, node_dump(c1.select("", true)));

      h2::h2_json_tree c2(day);
      JE(day_json, node_dump(c2.select("", true)));
   }

   Case(.)
   {
      h2::h2_json_tree c2(day);
      JE("[0, 12]", node_dump(c2.select(".am", true)));
   }

   Case([])
   {
      h2::h2_json_tree c1(week);
      JE("Monday", node_dump(c1.select("[1]", true)));

      h2::h2_json_tree c2(day);
      JE("[0, 12]", node_dump(c2.select("[am]", true)));
   }

   Case(.[])
   {
      h2::h2_json_tree c2(day);
      JE("12", node_dump(c2.select(".am[1]", true)));
   }

   Case(..)
   {
      h2::h2_json_tree c2(day);
      JE("18", node_dump(c2.select(".pm.afternoon", true)));
   }

   Case(not found)
   {
      h2::h2_json_tree c2(day);
      OK(NULL, c2.select(".ap", true));
   }
}

SUITE(illformed json)
{
   Case(missing end closeure)
   {
      const char* sexy = "[male, female";

      h2::h2_json_tree c(sexy);
      OK(c.illformed);

      OK(ListOf("[", " male", " ,", " female", "\033{yellow,bold,underline}", " ... ", "\033{reset}"), c.serialize());
   }

   Case(array seperated by colon)
   {
      const char* sexy = "[male: female]";

      h2::h2_json_tree c(sexy);
      OK(c.illformed);

      OK(ListOf("[", " male", "\033{yellow,bold,underline}", " : ", "\033{reset}", " female", " ]"), c.serialize());
   }

   Case(object seperated by comma)
   {
      const char* sexy = "{male, female}";

      h2::h2_json_tree c(sexy);
      OK(c.illformed);

      OK(ListOf("{", " male", "\033{yellow,bold,underline}", " , ", "\033{reset}", " female", " }"), c.serialize());
   }

   // Case(missing double quote)
   // {
   //    const char* sexy = "{\"male: female}";

   //    h2::h2_json_tree c(sexy);
   //    OK(c.illformed);

   //    OK(ListOf("{", "\033{yellow,bold,underline}", "\"male"), c.illformed());
   // }
}
