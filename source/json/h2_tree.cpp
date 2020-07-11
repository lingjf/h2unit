
struct h2_json_tree : h2_json_node {
   h2_vector<h2_string> lexical;
   h2_json_syntax syntax{lexical};
   bool successful;
   h2_json_tree(const char* json_string, int json_length = -1)
   {
      h2_json_lexical::parse(lexical, json_string, json_length);
      successful = syntax.parse(*this);
   }
   h2_line illformed()
   {
      h2_line line;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            line.printf("red", "%s", lexical[j].c_str());
         else
            line.push_back(lexical[j]);
      }
      if (!successful && lexical.size() <= syntax.i) {
         line.printf("red", "...");
      }
      return line;
   }
};
