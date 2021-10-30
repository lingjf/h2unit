#include "../source/h2_unit.cpp"
#include "test_types.hpp"

static h2::h2_string safes(h2::h2_string s)
{
   return s.replace_all("\"", "%").replace_all("'", "&").replace_all("\\\"", "$");
}

static h2::h2_string dual_tojson(h2::h2_json_dual* dual)
{
   h2::h2_string out;
   out.sprintf("{");
   out.sprintf("'depth': %d,", dual->depth);
   out.sprintf("'relationship': %d,", dual->relationship);
   out.sprintf("'index': %d,", dual->index);
   out.sprintf("'key_equal': %s,", dual->key_equal ? "true" : "false");
   out.sprintf("'value_match': %s,", dual->value_match ? "true" : "false");
   out.sprintf("'type': ['%s', '%s'],", node_type_tostring(dual->e_type), node_type_tostring(dual->a_type));
   out.sprintf("'key': ['%s', '%s'],", safes(dual->e_key).c_str(), safes(dual->a_key).c_str());
   out.sprintf("'value': ['%s', '%s'],", safes(dual->e_value).c_str(), safes(dual->a_value).c_str());
   out.sprintf("'children':[");
   h2_list_for_each_entry (p, i, dual->children, h2::h2_json_dual, x) {
      if (i) out.sprintf(",");
      out += dual_tojson(p);
   }
   out.sprintf("]");
   out.sprintf("}");
   return out;
}

SUITE(json dual)
{
   Case(object only)
   {
      const char* e = "{\"e\": 123}";
      const char* a = "{\"a\": 456}";

      h2::h2_json_tree e_json(e);
      h2::h2_json_tree a_json(a);

      h2::h2_json_dual dual(&e_json, &a_json, false);

      JE(
        "{                                      \
          'depth': 0,                           \
          'relationship': 0,                    \
          'index': 0,                           \
          'key_equal': true,                    \
          'value_match': false,                 \
          'type': ['object', 'object'],         \
          'key': ['', ''],                      \
          'value': ['', ''],                    \
          'children': [                         \
            {                                   \
              'depth': 1,                       \
              'relationship': 0,                \
              'index': 0,                       \
              'key_equal': false,               \
              'value_match': false,             \
              'type': ['number', 'number'],     \
              'key': [/?e?/, /?a?/],            \
              'value': ['123', '456'],          \
              'children': []                    \
            }                                   \
          ]                                     \
        }",
        dual_tojson(&dual));

      h2::h2_lines e_lines, a_lines;
      dual.align(e_lines, a_lines);
   }
}
