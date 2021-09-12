#include "../source/h2_unit.cpp"
#include "test_types.hpp"

h2::h2_string safes(h2::h2_string s)
{
   return s.replace_all("\"", "%").replace_all("'", "&").replace_all("\\\"", "$");
}

h2::h2_string dual_tojson(h2::h2_json_dual* dual)
{
   h2::h2_string out;
   out.sprintf("{");
   out.sprintf("'relationship': %d,", dual->relationship);
   out.sprintf("'depth': %d,", dual->depth);
   out.sprintf("'index': %d,", dual->index);
   out.sprintf("'key_equal': %s,", dual->key_equal ? "true" : "false");
   out.sprintf("'value_match': %s,", dual->value_match ? "true" : "false");
   out.sprintf("'type': ['%s', '%s'],", node_type_tostring(dual->e_type), node_type_tostring(dual->a_type));
   out.sprintf("'key': ['%s', '%s'],", safes(dual->e_key).c_str(), safes(dual->a_key).c_str());
   out.sprintf("'value': ['%s', '%s'],", safes(dual->e_value).c_str(), safes(dual->a_value).c_str());
   out.sprintf("'blob': [%d, %d],", dual->e_blob.size(), dual->a_blob.size());
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
   char t[1024 * 8];
   memset(t, 0, sizeof(t));
   Case(object only)
   {
      const char* e = "{\"e\": 123}";
      const char* a = "{\"a\": 456}";

      h2::h2_json_tree e_json(e);
      h2::h2_json_tree a_json(a);

      h2::h2_json_dual dual(&e_json, &a_json, false);

      JE(
        "{                                      \
          'relationship': 0,                    \
          'depth': 0,                           \
          'index': 0,                           \
          'key_equal': true,                    \
          'value_match': false,                 \
          'type': ['object', 'object'],         \
          'key': ['', ''],                      \
          'value': ['', ''],                    \
          'blob': [0, 0],                       \
          'children': [                         \
            {                                   \
              'relationship': 0,                \
              'depth': 1,                       \
              'index': 0,                       \
              'key_equal': false,               \
              'value_match': false,             \
              'type': ['number', 'number'],     \
              'key': [/?e?/, /?a?/],            \
              'value': ['123', '456'],          \
              'blob': [0, 0],                   \
              'children': []                    \
            }                                   \
          ]                                     \
        }",
        dual_tojson(&dual));

      h2::h2_paragraph e_paragraph, a_paragraph;
      dual.align(e_paragraph, a_paragraph);
   }
}
