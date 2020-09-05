
h2_inline h2_lines h2_json::format(const h2_string& json_string)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return {tree.serialize()};
   h2_lines lines = tree.format();
   if (O.copy_paste_json) {
      if (!lines.empty()) {
         lines.front() = "\"" + lines.front();
         lines.back() = lines.back() + "\"";
      }
      unsigned max_width = lines.width();
      for (size_t i = 0; i < lines.size(); ++i) {
         lines[i].padding(max_width - lines[i].width() + 3);
         if (i < lines.size() - 1) lines[i].push_back("\\");
      }
   }
   return lines;
}

h2_inline h2_string h2_json::select(const h2_string& json_string, const h2_string& selector, bool caseless)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return json_string;
   h2_json_node* node = tree.select(selector.c_str(), caseless);
   if (!node) return "";
   h2_lines lines;
   node->print(lines, O.fold_json, false);
   return lines.string();
}

h2_inline int h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return -1;
   return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return false;
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_lines, a_lines);
   return true;
}
