h2_inline h2_paragraph h2_json::dump(const h2_string& json_string)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return {tree.serialize()};
   h2_paragraph paragraph = tree.format(O.fold_json, O.copy_paste_json);
   if (O.copy_paste_json) {
      if (!paragraph.empty()) {
         paragraph.front() = "\"" + paragraph.front();
         paragraph.back() = paragraph.back() + "\"";
      }
      unsigned max_width = paragraph.width();
      for (size_t i = 0; i < paragraph.size(); ++i) {
         paragraph[i].padding(max_width - paragraph[i].width() + 3);
         if (i < paragraph.size() - 1) paragraph[i].push_back("\\");
      }
   }
   return paragraph;
}

h2_inline h2_string h2_json::select(const h2_string& json_string, const h2_string& selector, bool caseless)
{
   h2_json_tree tree(json_string.c_str());
   if (tree.illformed) return json_string;
   h2_json_node* node = tree.select(selector.c_str(), caseless);
   if (!node) return "";
   return node->format(O.fold_json, 2).string();
}

h2_inline int h2_json::match(const h2_string& expect, const h2_string& actual, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return -1;
   return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
}

h2_inline bool h2_json::diff(const h2_string& expect, const h2_string& actual, h2_paragraph& e_paragraph, h2_paragraph& a_paragraph, bool caseless)
{
   h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
   if (e_tree.illformed || a_tree.illformed) return false;
   h2_json_dual dual(&e_tree, &a_tree, caseless);
   dual.align(e_paragraph, a_paragraph);
   return true;
}
