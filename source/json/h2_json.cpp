
h2_inline bool h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   return h2_json_match::match(&e_tree, &a_tree, caseless);
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (!e_tree.parsed || !a_tree.parsed) { /* illformed json */
      h2_line e_line, a_line;
      for (auto&c: e_tree.lexical) e_line.push_back(c);
      for (auto&c: a_tree.lexical) a_line.push_back(c);
      e_lines.push_back(e_line);
      a_lines.push_back(a_line);
      return false;
   }
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_lines, a_lines);
   return true;
}
