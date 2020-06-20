
h2_inline bool h2_json::match(const h2_string& expect, const h2_string& actual)
{
   h2_json_parse e(expect.c_str()), a(actual.c_str());
   return h2_json_match::match(&e.root_node, &a.root_node);
}

h2_inline void h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines)
{
   h2_json_parse e(expect.c_str()), a(actual.c_str());
   h2_json_dual dual(&e.root_node, &a.root_node);
   dual.align(e_lines, a_lines);
}
