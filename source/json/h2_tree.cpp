struct h2_json_tree : h2_json_node {
   h2_vector<h2_string> lexical;
   h2_json_syntax syntax{lexical};
   bool illformed;

   h2_json_tree(const char* json_string, int json_length = -1)
   {
      h2_json_lexical::parse(lexical, json_string, json_length);
      illformed = !syntax.parse(*this);
   }

   h2_json_node* select(const char* selector, bool caseless)
   {
      h2_json_select select(selector);
      h2_json_node* node = this;
      for (auto& c : select.values)
         node = c.key.size() ? node->get(c.key, caseless) : node->get(c.index);
      node->key_string = "";
      return node;
   }

   h2_row serialize()
   {
      h2_row row;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            row.printf("yellow,bold,underline", "%s%s ", comma_if(j, " "), lexical[j].c_str());
         else
            row.push_back(comma_if(j, " ") + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) {
         row.printf("yellow,bold,underline", " ... ");
      }
      return row;
   }

   h2_rows format()
   {
      h2_rows rows;
      print(rows, O.fold_json, O.copy_paste_json);
      return rows;
   }
};
