
h2_inline h2_rows h2_json::format(const h2_string& json_string)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return {tree.serialize()};
   h2_rows rows = tree.format();
   if (O.copy_paste_json) {
      if (!rows.empty()) {
         rows.front() = "\"" + rows.front();
         rows.back() = rows.back() + "\"";
      }
      unsigned max_width = rows.width();
      for (size_t i = 0; i < rows.size(); ++i) {
         rows[i].padding(max_width - rows[i].width() + 3);
         if (i < rows.size() - 1) rows[i].push_back("\\");
      }
   }
   return rows;
}

h2_inline h2_string h2_json::select(const h2_string& json_string, const h2_string& selector, bool caseless)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return json_string;
   h2_json_node* node = tree.select(selector.c_str(), caseless);
   if (!node) return "";
   h2_rows rows;
   node->print(rows, O.fold_json, false);
   return rows.string();
}

h2_inline int h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return -1;
   return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_rows& e_lines, h2_rows& a_lines, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return false;
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_lines, a_lines);
   return true;
}
