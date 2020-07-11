
struct h2_json_syntax {
   static bool parse(h2_json_node& root_node, h2_vector<h2_string> /* not reference, force a copy */ lexical)
   {
      return parse_value(root_node, lexical);
   }

   static h2_string& extract_string(h2_string& s)
   {
      if (s.enclosed('\"'))
         s = s.unquote('\"');
      else if (s.enclosed('\''))
         s = s.unquote('\'');

      s.replace_all("\\b", "\b");
      s.replace_all("\\f", "\f");
      s.replace_all("\\n", "\n");
      s.replace_all("\\r", "\r");
      s.replace_all("\\t", "\t");
      s.replace_all("\\\"", "\"");
      s.replace_all("\\\\", "\\");
      return s;
   }

   static bool parse_value(h2_json_node& node, h2_vector<h2_string>& lexical)
   {
      if (lexical.empty()) return true;

      /* t_null */
      if (lexical.front().equals("null")) {
         node.type = h2_json_node::t_null;
         lexical.erase(lexical.begin());
         return true;
      }
      /* false */
      if (lexical.front().equals("false")) {
         node.type = h2_json_node::t_boolean;
         node.value_boolean = false;
         lexical.erase(lexical.begin());
         return true;
      }
      /* true */
      if (lexical.front().equals("true")) {
         node.type = h2_json_node::t_boolean;
         node.value_boolean = true;
         lexical.erase(lexical.begin());
         return true;
      }
      /* array */
      if (lexical.front().equals("[")) return parse_array(node, lexical);
      /* object */
      if (lexical.front().equals("{")) return parse_object(node, lexical);
      /* pattern */
      if (lexical.front().startswith("/")) return parse_pattern(node, lexical);

      if (lexical.front().equals(":")) return false;
      if (lexical.front().equals(",")) return false;

      /* string or number */
      return parse_literal(node, lexical);
   }

   static bool parse_key(h2_json_node& node, h2_vector<h2_string>& lexical)
   {
      node.key_string = lexical.front();
      lexical.erase(lexical.begin());
      extract_string(node.key_string);
      return true;
   }

   static bool parse_pattern(h2_json_node& node, h2_vector<h2_string>& lexical)
   {
      node.value_string = lexical.front();
      lexical.erase(lexical.begin());
      if (node.value_string.enclosed('/'))
         node.value_string = node.value_string.unquote('/');
      node.type = h2_json_node::t_pattern;
      return true;
   }

   static bool parse_literal(h2_json_node& node, h2_vector<h2_string>& lexical)
   {
      node.value_string = lexical.front();
      lexical.erase(lexical.begin());
      if (parse_number(node)) return true;
      return parse_string(node);
   }

   static bool parse_number(h2_json_node& node)
   {
      int err = 0;
      node.value_double = tinyexpr::te_interp(node.value_string.c_str(), &err);
      if (0 != err) return false;
      node.type = h2_json_node::t_number;
      return true;
   }

   static bool parse_string(h2_json_node& node)
   {
      extract_string(node.value_string);
      node.type = h2_json_node::t_string;
      return true;
   }

   static bool parse_array(h2_json_node& node, h2_vector<h2_string>& lexical)
   {
      lexical.erase(lexical.begin());  // pop [

      while (!lexical.front().equals("]")) {
         h2_json_node* new_node = new h2_json_node();
         node.children.push_back(new_node->x);
         if (!parse_value(*new_node, lexical)) return false;
         if (lexical.front().equals(","))
            lexical.erase(lexical.begin());
         else
            break;
      }

      if (!lexical.front().equals("]")) return false;
      lexical.erase(lexical.begin());  // pop ]
      node.type = h2_json_node::t_array;

      return true;
   }

   static bool parse_object(h2_json_node& node, h2_vector<h2_string>& lexical)
   {
      lexical.erase(lexical.begin());  // pop {

      while (!lexical.front().equals("}")) {
         h2_json_node* new_node = new h2_json_node();
         node.children.push_back(new_node->x);

         if (!parse_key(*new_node, lexical)) return false;

         if (!lexical.front().equals(":")) return false;
         lexical.erase(lexical.begin());

         if (!parse_value(*new_node, lexical)) return false;

         if (lexical.front().equals(","))
            lexical.erase(lexical.begin());
         else
            break;
      }
      if (!lexical.front().equals("}")) return false;
      lexical.erase(lexical.begin());  // pop ]

      node.type = h2_json_node::t_object;
      return true;
   }
};
