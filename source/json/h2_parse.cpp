
struct h2_json_parse {
   const char* json;
   int size;
   int i = 0;

   h2_json_node root_node;

   h2_json_parse(const char* json_string, int json_length = 0) : json(json_string), size(json_length)
   {
      if (size == 0) size = strlen(json);
      parse_value(&root_node);
   }

   bool leadwith(const char* s)  // left skip and start with
   {
      while (i < size && ::isspace(json[i])) i++;
      int n = strlen(s);
      if (size - i < n) return false;
      return ::strncmp(json + i, s, n) == 0;
   }

   bool parse_number(h2_json_node* node)
   {
      int len = 0;
      for (; i + len < size; ++len) {
         const char c = json[i + len];
         if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0')
            break;
      }

      node->value_string.assign(json + i, len);

      int err = 0;
      node->value_double = tinyexpr::te_interp(node->value_string.c_str(), &err);
      if (0 != err) return false;

      node->type = h2_json_node::t_number;
      i += len;

      return true;
   }

   bool parse_string(h2_json_node* node)
   {
      const char bound = json[i];
      i++;

      if (size <= i) return false;

      const char* src = json + i;
      int len = 0;
      for (; json[i] != bound; ++len) {
         if (json[i++] == '\\') i++;
         if (size <= i) return false;
      }

      for (; len > 0; ++src, --len)
         if (*src != '\\')
            node->value_string.push_back(*src);
         else
            switch (*++src) {
            case 'b': node->value_string.push_back('\b'); break;
            case 'f': node->value_string.push_back('\f'); break;
            case 'n': node->value_string.push_back('\n'); break;
            case 'r': node->value_string.push_back('\r'); break;
            case 't': node->value_string.push_back('\t'); break;
            case '\"': node->value_string.push_back('\"'); break;
            case '\\': node->value_string.push_back('\\'); break;
            case '/': node->value_string.push_back('/'); break;
            default: return false;
            }

      node->type = h2_json_node::t_string;
      i++;

      return true;
   }

   bool parse_pattern(h2_json_node* node)
   {
      bool ret = parse_string(node);
      node->type = h2_json_node::t_pattern;
      return ret;
   }

   bool parse_value(h2_json_node* node)
   {
      /* t_null */
      if (leadwith("null")) {
         node->type = h2_json_node::t_null;
         i += 4;
         return true;
      }
      /* false */
      if (leadwith("false")) {
         node->type = h2_json_node::t_boolean;
         node->value_boolean = false;
         i += 5;
         return true;
      }
      /* true */
      if (leadwith("true")) {
         node->type = h2_json_node::t_boolean;
         node->value_boolean = true;
         i += 4;
         return true;
      }
      /* string */
      if (leadwith("\"") || leadwith("\'")) return parse_string(node);
      /* pattern */
      if (leadwith("/")) return parse_pattern(node);

      /* array */
      if (leadwith("[")) return parse_array(node);
      /* object */
      if (leadwith("{")) return parse_object(node);

      /* number */
      return parse_number(node);
   }

   bool parse_array(h2_json_node* node)
   {
      i++;  //pass [

      while (!leadwith("]")) {
         h2_json_node* new_node = new h2_json_node();
         node->children.push_back(new_node->x);

         if (!parse_value(new_node)) return false;

         if (leadwith(",")) i++;
      }

      node->type = h2_json_node::t_array;
      i++;

      return true;
   }

   bool parse_object(h2_json_node* node)
   {
      i++;  //pass {

      while (!leadwith("}")) {
         h2_json_node* new_node = new h2_json_node();
         node->children.push_back(new_node->x);

         if (!parse_string(new_node)) return false;

         new_node->key_string = new_node->value_string;
         new_node->value_string = "";

         if (!leadwith(":")) return false;
         i++;

         if (!parse_value(new_node)) return false;

         if (leadwith(",")) i++;
      }

      node->type = h2_json_node::t_object;
      i++;

      return true;
   }
};
