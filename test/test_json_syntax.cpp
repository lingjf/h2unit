#include "../source/h2_unit.cpp"

extern char* node_tojson(h2::h2_json_node* node, char* b);

SUITE(json syntax simple)
{
   char t1[1024 * 128];

   Case(empty)
   {
      const char* json = "";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_absent, node.type);
   }

   Cases(true, "null", " null ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_null, node.type);

      OK("null", node_tojson(&node, t1));
   }

   Cases(true, "true", " true ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_boolean, node.type);

      OK("true", node_tojson(&node, t1));
   }

   Cases(true, "false", " false ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_boolean, node.type);

      OK("false", node_tojson(&node, t1));
   }

   Case(pattern)
   {
      const char* json = "/hello/";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_pattern, node.type);

      OK("\"/hello/\"", node_tojson(&node, t1));
   }

   Case(double quote)
   {
      const char* json = "\"hello\"";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_string, node.type);

      OK("\"hello\"", node_tojson(&node, t1));
   }

   Case(single quote)
   {
      const char* json = "'hello'";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_string, node.type);

      OK("\"hello\"", node_tojson(&node, t1));
   }

   Case(no quote string)
   {
      const char* json = "hello world ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_string, node.type);

      OK("\"hello world\"", node_tojson(&node, t1));
   }

   Case(direct number)
   {
      const char* json = "1234 ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_number, node.type);

      OK("1234", node_tojson(&node, t1));
   }

   Case(math expression number)
   {
      const char* json = " 2 +1 ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_number, node.type);

      OK("3", node_tojson(&node, t1));
      OK("2 +1", node.value_string);
   }

   Cases({}, "{}", " { } ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_object, node.type);

      OK("{}", node_tojson(&node, t1));
   }

   Cases({}, "[]", " [ ]  ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      h2::h2_json_node node;
      h2::h2_json_syntax syntax(lexical);
      syntax.parse(node);
      OK(h2::h2_json_node::t_array, node.type);

      OK("[]", node_tojson(&node, t1));
   }
}
