struct h2_json {
   static h2_lines dump(const h2_string& json_string)
   {
      h2_json_tree tree(json_string.c_str());
      if (tree.illformed) return {tree.serialize()};
      h2_lines lines = tree.format(O.fold_json, O.json_source_quote);
      if (!h2_blank(O.json_source_quote)) {
         if (!lines.empty()) {
            lines.front() = "\"" + lines.front();
            lines.back() = lines.back() + "\"";
         }
         size_t max_width = lines.width();
         for (size_t i = 0; i < lines.size(); ++i) {
            lines[i].padding(max_width - lines[i].width() + 3);
            if (i < lines.size() - 1) lines[i].push_back("\\");
         }
      }
      return lines;
   }

   static h2_string select(const h2_string& json_string, const h2_string& selector, bool caseless)
   {
      h2_json_tree tree(json_string.c_str());
      if (tree.illformed) return json_string;
      h2_json_node* node = tree.select(selector.c_str(), caseless);
      if (!node) return "";
      return node->format(O.fold_json, "\"").string();
   }

   static int match(const h2_string& expect, const h2_string& actual, bool caseless)
   {  // < 0 illformed json; = 0 matched; > 0 unmatched
      h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
      if (e_tree.illformed || a_tree.illformed) return -1;
      return h2_json_match::match(&e_tree, &a_tree, caseless) ? 0 : 1;
   }

   static bool diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless)
   {
      h2_json_tree e_tree(expect.c_str()), a_tree(actual.c_str());
      if (e_tree.illformed || a_tree.illformed) return false;
      h2_json_dual dual(&e_tree, &a_tree, caseless);
      dual.align(e_lines, a_lines);
      return true;
   }
};
