
struct h2_json_node : h2_libc {
   static constexpr int t_absent = 0;
   static constexpr int t_null = 1;
   static constexpr int t_boolean = 2;
   static constexpr int t_number = 3;
   static constexpr int t_string = 4;
   static constexpr int t_pattern = 5; // regex or wildcard pattern
   static constexpr int t_array = 6;
   static constexpr int t_object = 7;

   int type = t_absent;
   h2_string key_string;
   h2_string value_string;
   double value_double = 0;
   bool value_boolean = false;
   h2_list children, x; /* array or object */

   ~h2_json_node()
   {
      h2_list_for_each_entry (p, children, h2_json_node, x) {
         p->x.out();
         delete p;
      }
   }

   int size()
   {
      return children.count();
   }

   h2_json_node* get(int index)
   {
      h2_list_for_each_entry (p, children, h2_json_node, x)
         if (li == index)
            return p;
      return nullptr;
   }

   h2_json_node* get(const h2_string& name, bool caseless)
   {
      h2_list_for_each_entry (p, children, h2_json_node, x)
         if (p->key_string.equals(name, caseless))
            return p;
      return nullptr;
   }

   bool is_null() { return t_null == type; }
   bool is_bool() { return t_boolean == type; }
   bool is_number() { return t_number == type; }
   bool is_string() { return t_string == type; }
   bool is_pattern() { return t_pattern == type; }
   bool is_array() { return t_array == type; }
   bool is_object() { return t_object == type; }

   void dual(int& _type, const char*& _class, h2_string& _key, h2_string& _value)
   {
      if (key_string.size()) _key = "\"" + key_string + "\"";
      _type = type;
      switch (type) {
      case t_null:
         _class = "atomic";
         _value = "null";
         break;
      case t_boolean:
         _class = "atomic";
         _value = value_boolean ? "true" : "false";
         break;
      case t_number:
         _class = "atomic";
         if (value_double - ::floor(value_double) == 0)
            _value = h2_stringify((long long)value_double);
         else
            _value = h2_stringify(value_double);
         break;
      case t_string:
         _class = "atomic";
         _value = "\"" + value_string + "\"";
         break;
      case t_pattern:
         _class = "atomic";
         _value = value_string;
         break;
      case t_array:
         _class = "array";
         break;
      case t_object:
         _class = "object";
         break;
      }
   }

   void print(h2_lines& lines, int depth = 0, int next = 0, bool fold = false)
   {
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back("\"" + key_string + "\": ");
      if (is_null())
         line.push_back("null");
      else if (is_bool())
         line.push_back(value_boolean ? "true" : "false");
      else if (is_number()) {
         if (value_double - ::floor(value_double) == 0)
            line.push_back(h2_stringify((long long)value_double));
         else
            line.push_back(h2_stringify(value_double));
      } else if (is_string())
         line.push_back("\"" + value_string + "\"");
      else if (is_pattern())
         line.push_back("\"/" + value_string + "/\"");
      else if (is_array() || is_object()) {
         line.push_back(is_array() ? "[ " : "{ ");
         h2_lines __lines;
         h2_list_for_each_entry (p, children, h2_json_node, x)
            p->print(__lines, depth + 1, children.count() - li - 1, fold);

         if (fold && __lines.foldable()) {
            line.fold(__lines);
            line.push_back(is_array() ? " ]" : " }");
         } else {
            lines.push_back(line), line.clear();
            lines.concat_back(__lines);
            line.indent(depth * 2);
            line.push_back(is_array() ? "]" : "}");
         }
      }
      if (line.size()) {
         if (next) line.push_back(",");
         lines.push_back(line), line.clear();
      }
   }
};
