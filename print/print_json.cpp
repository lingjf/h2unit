#include "../build/h2unit.hpp"

SUITE(json)
{
   Case(object)
   {
      const char* json =
        "{                                \
            'width': '110',               \
            'Height': 444,                \
            'corners': [1,2,3,4],         \
            'links': {'left': 1},         \
            'nanes': [                    \
               'circle',                  \
               'triangle',                \
               'rectangle',               \
               'ちょくせん',                \
            ],                            \
            'score': 100                  \
        }";

      JE("{                               \
            width: 116,                   \
            height: 222 * sqrt(2!+2),     \
            weight: 100 kg,               \
            corners : [1,2,3,'4'],        \
            links : { left : 1},          \
            names : [                     \
               'circle',                  \
               '三角形',                   \
               '장방형',                    \
               'きょくせん'                 \
            ]                             \
         }",
         json);
   }

   Case(caseless)
   {
      OK(~Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 20}");
   }

   Case(donot)
   {
      OK(!Je("{'name': /hello.*world/, 'age': 18}"), "{'name': \"hello world\", 'age': 18}");
   }

   Case(illformed json)
   {
      const char* json = "{'name': \"Hello World\", 'age': 18, week: [\"Sunday\": \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]}";
      OK(Je("{name: /hello.*world/, Age: 18, Week:[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]"), json);
   }
}
