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
      if (node) node->key_string = "";
      return node;
   }

   h2_line serialize() const
   {
      h2_line line;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            line.printf("yellow,bold,underline", "%s%s ", comma_if(j, " "), lexical[j].c_str());
         else
            line.push_back(comma_if(j, " ") + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) {
         line.printf("yellow,bold,underline", " ... ");
      }
      return line;
   }
};
