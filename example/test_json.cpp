
extern "C" {
#include "product_c.h"
}

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

SUITE(json)
{
   Case(object [fail])
   {
      rectangle_t p1 = {111, 222};

      JE("{                               \
            width: 116,                   \
            height: 222 * sqrt(2!+2),     \
            weight: 100 kg,               \
            corners : [1,2,3,'4'],        \
            edges : { left : /#[1-9]+/},  \
            names : [                     \
               'circle',                  \
               '三角形',                   \
               '장방형',                    \
               'きょくせん'                 \
            ]                             \
         }",
         rectangle_tojson(&p1))
        << "rectangle error !!";
   }

   Case(caseless [fail])
   {
      OK(~Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 20}");
   }

   Case(donot [fail])
   {
      OK(!Je("{'name': /hello.*world/, 'age': 18}"), "{'name': \"hello world\", 'age': 18}");
   }

   Case(illformed json [fail])
   {
      const char* json = "{'name': \"Hello World\", 'age': 18, week: [\"Sunday\": \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]}";
      OK(Je("{name: /hello.*world/, Age: 18, Week:[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]"), json);
   }
}
