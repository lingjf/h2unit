
struct h2_json_tree : h2_json_node {
   h2_vector<h2_string> lexical;
   h2_json_syntax syntax{lexical};
   bool illformed;
   h2_json_tree(const char* json_string, int json_length = -1)
   {
      h2_json_lexical::parse(lexical, json_string, json_length);
      illformed = !syntax.parse(*this);
   }
   h2_line serialize()
   {
      h2_line line;
      for (size_t j = 0; j < lexical.size(); ++j) {
         if (j == syntax.i)
            line.printf("yellow,bold,underline", " %s ", lexical[j].c_str());
         else
            line.push_back(" " + lexical[j]);
      }
      if (illformed && lexical.size() <= syntax.i) {
         line.printf("yellow,bold", "...");
      }
      return line;
   }
   h2_lines format()
   {
      h2_lines lines;
      print(lines, O.fold, O.program);
      return lines;
   }
};
