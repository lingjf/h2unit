
struct h2_json_syntax {
   int i = 0;
   const h2_vector<h2_string>& lexical;
   h2_json_syntax(const h2_vector<h2_string>& _lexical) : lexical(_lexical) {}

   bool parse(h2_json_node& root_node)
   {
      return parse_value(root_node);
   }

   h2_string& extract_string(h2_string& s)
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

   bool extract_number(h2_string& s, double& value)
   {
      int err = 0;
      value = tinyexpr::te_interp(s.c_str(), &err);
      return 0 == err;
   }

   bool requires(const char* s)
   {
      if (lexical.size() <= i || !lexical[i].equals(s)) return false;
      ++i;
      return true;
   }

   bool parse_value(h2_json_node& node)
   {
      if (lexical.size() <= i) return true;

      /* t_null */
      if (lexical[i].equals("null")) {
         ++i;
         node.type = h2_json_node::t_null;
         return true;
      }
      /* false */
      if (lexical[i].equals("false")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = false;
         return true;
      }
      /* true */
      if (lexical[i].equals("true")) {
         ++i;
         node.type = h2_json_node::t_boolean;
         node.value_boolean = true;
         return true;
      }
      /* array */
      if (lexical[i].equals("[")) return parse_array(node);
      /* object */
      if (lexical[i].equals("{")) return parse_object(node);
      /* pattern */
      if (lexical[i].startswith("/")) return parse_pattern(node);

      if (lexical[i].equals(":")) return false;
      if (lexical[i].equals(",")) return false;

      /* string or number */
      return parse_literal(node);
   }

   bool parse_key(h2_json_node& node)
   {
      node.key_string = lexical[i++];
      extract_string(node.key_string);
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

   bool parse_literal(h2_json_node& node)
   {
      node.value_string = lexical[i++];
      if (extract_number(node.value_string, node.value_double)) {
         node.type = h2_json_node::t_number;
         return true;
      }
      extract_string(node.value_string);
      node.type = h2_json_node::t_string;
      return true;
   }

   bool parse_array(h2_json_node& node)
   {
      ++i;  // pop [
      while (i < lexical.size() && !lexical[i].equals("]")) {
         h2_json_node* new_node = new h2_json_node();
         node.children.push_back(new_node->x);
         if (!parse_value(*new_node)) return false;
         if (i < lexical.size() && lexical[i].equals(","))
            i++;
         else
            break;
      }

      if (!requires("]")) return false;
      node.type = h2_json_node::t_array;

      return true;
   }

   bool parse_object(h2_json_node& node)
   {
      ++i;  // pop {
      while (i < lexical.size() && !lexical[i].equals("}")) {
         h2_json_node* new_node = new h2_json_node();
         node.children.push_back(new_node->x);

         if (!parse_key(*new_node)) return false;

         if (!requires(":")) return false;

         if (!parse_value(*new_node)) return false;

         if (i < lexical.size() && lexical[i].equals(","))
            ++i;
         else
            break;
      }

      if (!requires("}")) return false;

      node.type = h2_json_node::t_object;
      return true;
   }
};
