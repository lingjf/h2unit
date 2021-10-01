struct h2_json_syntax {
   size_t i = 0;
   const h2_vector<h2_string>& lexical;
   h2_json_syntax(const h2_vector<h2_string>& _lexical) : lexical(_lexical) {}

   bool parse(h2_json_node& root_node)
   {
      if (!parse_value(root_node)) return false;
      return lexical.size() <= i;  // nothing more, "{},{}"
   }

   h2_string& filter_string(h2_string& s) const
   {
      if (s.enclosed('\"'))
         s = s.unquote('\"');
      else if (s.enclosed('\''))
         s = s.unquote('\'');
      s = s.unescape();
      return s;
   }

   bool interpret_number(const h2_string& s, double& value) const
   {
      int err = 0;
      value = tinyexpr::te_interp(s.c_str(), &err);
      return 0 == err;
   }

   bool desire(const char* s)
   {
      if (lexical.size() <= i || !lexical[i].equals(s)) return false;
      ++i;
      return true;
   }

   bool parse_value(h2_json_node& node)
   {
      if (lexical.size() <= i) return true;

      if (lexical[i].equals("null")) {
         ++i;
         node.type = h2_json_node::t_null;
         return true;
      }
      if (lexical[i].equals("false")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = false;
         return true;
      }
      if (lexical[i].equals("true")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = true;
         return true;
      }
      if (lexical[i].equals("[")) return parse_array(node);
      if (lexical[i].equals("{")) return parse_object(node);
      if (lexical[i].startswith("/")) return parse_pattern(node);

      if (lexical[i].equals(":")) return false;
      if (lexical[i].equals(",")) return false;

      return parse_string_or_number(node);
   }

   bool parse_key(h2_json_node& node)
   {
      node.key_string = lexical[i++];
      filter_string(node.key_string);
      return true;
   }

   bool parse_pattern(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (node.value_string.enclosed('/'))
         node.value_string = node.value_string.unquote('/');
      node.type = h2_json_node::t_pattern;
      return true;
   }

   bool parse_string_or_number(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (interpret_number(node.value_string, node.value_double)) {
         node.type = h2_json_node::t_number;
         node.value_string = "";
         return true;
      }
      node.value_double = 0;
      filter_string(node.value_string);
      node.type = h2_json_node::t_string;
      return true;
   }

   bool parse_array(h2_json_node& node)
   {
      if (!desire("[")) return false;
      int n = 0;
      while (i < lexical.size() && !lexical[i].equals("]")) {
         h2_json_node* new_node = new h2_json_node(n++);
         node.children.push_back(new_node->x);
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            i++;
         else
            break;
      }

      if (!desire("]")) return false;
      node.type = h2_json_node::t_array;
      return true;
   }

   bool parse_object(h2_json_node& node)
   {
      if (!desire("{")) return false;
      int n = 0;
      while (i < lexical.size() && !lexical[i].equals("}")) {
         h2_json_node* new_node = new h2_json_node(n++);
         node.children.push_back(new_node->x);
         if (!parse_key(*new_node)) return false;
         if (!desire(":")) return false;
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            ++i;
         else
            break;
      }

      if (!desire("}")) return false;
      node.type = h2_json_node::t_object;
      return true;
   }
};
