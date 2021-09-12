#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(json syntax single)
{
   Case(empty)
   {
      const char* json = "";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));
      JE("{                         \
            'type': 'absent',       \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("", node_dump(&node));
   }

   Cases("null", " null ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'null',         \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("null", node_dump(&node));
   }

   Cases("true", " true ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'boolean',      \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 0,      \
            'value_boolean': 1,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("true", node_dump(&node));
   }

   Cases("false", " false ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'boolean',      \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("false", node_dump(&node));
   }

   Case(pattern)
   {
      const char* json = "/hello/";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'pattern',      \
            'key_string': '',       \
            'value_string': 'hello',\
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("\"/hello/\"", node_dump(&node));
   }

   Case(double quote)
   {
      const char* json = "\"hello\"";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'string',       \
            'key_string': '',       \
            'value_string': 'hello',\
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("\"hello\"", node_dump(&node));
   }

   Case(single quote)
   {
      const char* json = "'hello'";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'string',       \
            'key_string': '',       \
            'value_string': 'hello',\
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("\"hello\"", node_dump(&node));
   }

   Case(no quote string)
   {
      const char* json = "hello world ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                               \
            'type': 'string',             \
            'key_string': '',             \
            'value_string': 'hello world',\
            'value_double': 0,            \
            'value_boolean': 0,           \
            'children':[]                 \
         }",
         node_tojson(&node));

      OK("\"hello world\"", node_dump(&node));
   }

   Case(direct number)
   {
      const char* json = "1234 ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'number',       \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 1234,   \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("1234", node_dump(&node));
   }

   Case(math expression number)
   {
      const char* json = " 2 +1 ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'number',       \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 3,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));
      OK("3", node_dump(&node));
   }
}

SUITE(json syntax array)
{
   Cases("[]", " [ ]  ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'array',        \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("[]", node_dump(&node));
   }

   Case(normal)
   {
      const char* json = "[123, \"abc\", true]";

      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                               \
            'type': 'array',              \
            'key_string': '',             \
            'value_string': '',           \
            'value_double': 0,            \
            'value_boolean': 0,           \
            'children':[                  \
                {                         \
                  'type': 'number',       \
                  'index': 0,             \
                  'key_string': '',       \
                  'value_string': '',     \
                  'value_double': 123,    \
                  'value_boolean': 0,     \
                  'children':[]           \
               },                         \
               {                          \
                  'type': 'string',       \
                  'index': 1,             \
                  'key_string': '',       \
                  'value_string': 'abc',  \
                  'value_double': 0,      \
                  'value_boolean': 0,     \
                  'children':[]           \
               },                         \
               {                          \
                  'type': 'boolean',      \
                  'index': 2,             \
                  'key_string': '',       \
                  'value_string': '',     \
                  'value_double': 0,      \
                  'value_boolean': 1,     \
                  'children':[]           \
               }                          \
            ]                             \
         }",
         node_tojson(&node));

      JE(json, node_dump(&node));
   }
}

SUITE(json syntax object)
{
   Cases("{}", " { } ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                         \
            'type': 'object',       \
            'key_string': '',       \
            'value_string': '',     \
            'value_double': 0,      \
            'value_boolean': 0,     \
            'children':[]           \
         }",
         node_tojson(&node));

      OK("{}", node_dump(&node));
   }

   Case(normal)
   {
      const char* json =
        "{                          \
            'name': 'jeff',         \
            'age': 28,              \
         }";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                               \
            'type': 'object',             \
            'key_string': '',             \
            'value_string': '',           \
            'value_double': 0,            \
            'value_boolean': 0,           \
            'children':[                  \
                {                         \
                  'type': 'string',       \
                  'index': 0,             \
                  'key_string': 'name',   \
                  'value_string': 'jeff', \
                  'value_double': 0,      \
                  'value_boolean': 0,     \
                  'children':[]           \
               },                         \
               {                          \
                  'type': 'number',       \
                  'index': 1,             \
                  'key_string': 'age',    \
                  'value_string': '',     \
                  'value_double': 28,     \
                  'value_boolean': 0,     \
                  'children':[]           \
               }                          \
            ]                             \
         }",
         node_tojson(&node));

      JE(json, node_dump(&node));
   }
}

SUITE(json syntax complex)
{
   Case(normal)
   {
      const char* json =
        "{                          \
            'name': 'jeff',         \
            'age': 28,              \
            'lang': [               \
                  'java',           \
                  {                 \
                     'python': 3,   \
                     'c++': 11,     \
                  },                \
                  ['lua', null],    \
             ],                     \
         }";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(syntax.parse(node));

      JE("{                                                                                     \
            'type': 'object',                                                                   \
            'key_string': '',                                                                   \
            'value_string': '',                                                                 \
            'value_double': 0,                                                                  \
            'value_boolean': 0,                                                                 \
            'children':[                                                                        \
                {                                                                               \
                  'type': 'string',                                                             \
                  'key_string': 'name',                                                         \
                  'value_string': 'jeff',                                                       \
                  'value_double': 0,                                                            \
                  'value_boolean': 0,                                                           \
                  'children':[]                                                                 \
               },                                                                               \
               {                                                                                \
                  'type': 'number',                                                             \
                  'key_string': 'age',                                                          \
                  'value_string': '',                                                           \
                  'value_double': 28,                                                           \
                  'value_boolean': 0,                                                           \
                  'children':[]                                                                 \
               },                                                                               \
               {                                                                                \
                  'type': 'array',                                                              \
                  'key_string': 'lang',                                                         \
                  'value_string': '',                                                           \
                  'value_double': 0,                                                            \
                  'value_boolean': 0,                                                           \
                  'children':[                                                                  \
                     {                                                                          \
                        'type': 'string',                                                       \
                        'key_string': '',                                                       \
                        'value_string': 'java',                                                 \
                        'value_double': 0,                                                      \
                        'value_boolean': 0,                                                     \
                        'children':[]                                                           \
                     },                                                                         \
                     {                                                                          \
                        'type': 'object',                                                       \
                        'key_string': '',                                                       \
                        'value_string': '',                                                     \
                        'value_double': 0,                                                      \
                        'value_boolean': 0,                                                     \
                        'children':[                                                            \
                           {                                                                    \
                              'type': 'number',                                                 \
                              'key_string': 'python',                                           \
                              'value_string': '',                                               \
                              'value_double': 3,                                                \
                              'value_boolean': 0,                                               \
                              'children':[]                                                     \
                           },                                                                   \
                           {                                                                    \
                              'type': 'number',                                                 \
                              'key_string': 'c++',                                              \
                              'value_string': '',                                               \
                              'value_double': 11,                                               \
                              'value_boolean': 0,                                               \
                              'children':[]                                                     \
                           }                                                                    \
                        ]                                                                       \
                     },                                                                         \
                     {                                                                          \
                        'type': 'array',                                                       \
                        'key_string': '',                                                       \
                        'value_string': '',                                                     \
                        'value_double': 0,                                                      \
                        'value_boolean': 0,                                                     \
                        'children':[                                                            \
                           {                                                                    \
                              'type': 'string',                                                 \
                              'key_string': '',                                                 \
                              'value_string': 'lua',                                            \
                              'value_double': 0,                                                \
                              'value_boolean': 0,                                               \
                              'children':[]                                                     \
                           },                                                                   \
                           {                                                                    \
                              'type': 'null',                                                   \
                              'key_string': '',                                                 \
                              'value_string': '',                                               \
                              'value_double': 0,                                                \
                              'value_boolean': 0,                                               \
                              'children':[]                                                     \
                           }                                                                    \
                        ]                                                                       \
                     }                                                                          \
                  ]                                                                             \
               }                                                                                \
            ]                                                                                   \
         }",
         node_tojson(&node));

      JE(json, node_dump(&node));
   }
}

SUITE(json syntax error)
{
   Case(not object closed)
   {
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, "{'a' : 1, 'b': 3");
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(!syntax.parse(node));
   }

   Case(not array closed)
   {
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, "[1, 2, 3");
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(!syntax.parse(node));
   }

   Case(extra tokens)
   {
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, "{}, {}");
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      OK(!syntax.parse(node));
   }
}
