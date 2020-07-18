
h2_inline h2_lines h2_json::format(const h2_string& json_string)
{
   h2_lines lines;
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) {
      lines.push_back(tree.serialize());
      return lines;
   }
   lines = tree.format();
   if (O.program) {
      if (0 < lines.size()) lines.front().concat_front("\""), lines.back().push_back("\"");
      unsigned max_width = lines.width();
      for (auto& line : lines) {
         line.padding(max_width - line.width() + 3);
         line.indent(1);
         line.push_back("\\");
      }
   }
   return lines;
}

h2_inline bool h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   return h2_json_match::match(&e_tree, &a_tree, caseless);
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) {
      return false;
   }
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_lines, a_lines);
   return true;
}
