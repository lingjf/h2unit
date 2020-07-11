
struct h2_json_tree : h2_json_node {
   h2_vector<h2_string> lexical;
   bool parsed;
   h2_json_tree(const char* json_string, int json_length = -1)
   {
      h2_json_lexical::parse(lexical, json_string, json_length);
      parsed = h2_json_syntax::parse(*this, lexical);
   }
};
