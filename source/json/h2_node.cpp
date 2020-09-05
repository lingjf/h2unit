
struct h2_json_node : h2_libc {
   static constexpr int t_absent = 0;
   static constexpr int t_null = 1;
   static constexpr int t_boolean = 2;
   static constexpr int t_number = 3;
   static constexpr int t_string = 4;
   static constexpr int t_pattern = 5;  // regex or wildcard pattern
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
      if (index < 0) index = children.count() + index;
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
            _value = std::to_string((long long)value_double).c_str();
         else
            _value = std::to_string(value_double).c_str();
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

   h2_string slash_if(bool slash) { return slash ? "\\" : ""; }

   void print(h2_lines& lines, bool fold = false, bool slash = false, int depth = 0, int next = 0)
   {
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back(slash_if(slash) + "\"" + key_string + slash_if(slash) + "\": ");
      if (is_null())
         line.push_back("null");
      else if (is_bool())
         line.push_back(value_boolean ? "true" : "false");
      else if (is_number()) {
         if (value_double - ::floor(value_double) == 0)
            line.push_back(std::to_string((long long)value_double).c_str());
         else
            line.push_back(std::to_string(value_double).c_str());
      } else if (is_string())
         line.push_back(slash_if(slash) + "\"" + value_string + slash_if(slash) + "\"");
      else if (is_pattern())
         line.push_back(slash_if(slash) + "\"/" + value_string + "/" + slash_if(slash) + "\"");
      else if (is_array() || is_object()) {
         h2_lines children_lines;
         h2_list_for_each_entry (p, children, h2_json_node, x)
            p->print(children_lines, fold, slash, depth + 1, children.count() - li - 1);

         line.push_back(is_array() ? "[" : "{");
         if (fold && children_lines.foldable()) {
            line += children_lines.folds();
         } else {
            lines.push_back(line), line.clear();
            lines += children_lines;
            line.indent(depth * 2);
         }
         line.push_back(is_array() ? "]" : "}");
      }
      if (line.size()) {
         if (next) line.push_back(", ");
         lines.push_back(line), line.clear();
      }
   }
};
