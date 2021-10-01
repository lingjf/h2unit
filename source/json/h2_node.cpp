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
   int index = 0;
   h2_string key_string;
   h2_string value_string;
   double value_double = 0;
   bool value_boolean = false;
   h2_list children, x; /* array or object */

   h2_json_node(int index_ = 0) : index(index_) {}
   ~h2_json_node()
   {
      h2_list_for_each_entry (p, children, h2_json_node, x) {
         p->x.out();
         delete p;
      }
   }

   int size() const
   {
      return children.count();
   }

   h2_json_node* get(int index) const
   {
      if (index < 0) index = children.count() + index;
      h2_list_for_each_entry (p, i, children, h2_json_node, x)
         if (i == index)
            return p;
      return nullptr;
   }

   h2_json_node* get(const h2_string& name, bool caseless) const
   {
      h2_list_for_each_entry (p, children, h2_json_node, x)
         if (p->key_string.equals(name, caseless))
            return p;
      return nullptr;
   }

   bool is_null() const { return t_null == type; }
   bool is_bool() const { return t_boolean == type; }
   bool is_number() const { return t_number == type; }
   bool is_string() const { return t_string == type; }
   bool is_pattern() const { return t_pattern == type; }
   bool is_array() const { return t_array == type; }
   bool is_object() const { return t_object == type; }

   h2_string quote_if(int quote) const
   {
      switch (quote) {
         case 1: return "'";
         case 2: return "\"";
         case 3: return "\\\"";
         default: return "";
      }
   }

   h2_string format_value(int quote) const
   {
      switch (type) {
         case t_null: return "null";
         case t_boolean: return value_boolean ? "true" : "false";
         case t_number: return (value_double - ::floor(value_double) == 0) ? std::to_string((long long)value_double).c_str() : std::to_string(value_double).c_str();
         case t_string: return quote_if(quote) + value_string + quote_if(quote);
         case t_pattern: return "/" + value_string + "/";
         case t_array:
         case t_object:
         default: return "";
      }
   }

   void format(int& _type, h2_string& _key, h2_string& _value, int quote = 0) const
   {
      _type = type;
      if (key_string.size()) _key = quote_if(quote) + key_string + quote_if(quote);
      _value = format_value(quote);
   }

   h2_lines format(bool fold, int quote = 0, size_t depth = 0, int next = 0) const
   {
      h2_lines lines;
      h2_line line;
      line.indent(depth * 2);
      if (key_string.size())
         line.push_back(quote_if(quote) + key_string + quote_if(quote) + ": ");
      if (is_array() || is_object()) {
         h2_lines children_lines;
         h2_list_for_each_entry (p, i, children, h2_json_node, x)
            children_lines += p->format(fold, quote, depth + 1, children.count() - i - 1);
         line.push_back(is_array() ? "[" : "{");
         if (fold && children_lines.foldable()) {
            line += children_lines.folds();
         } else {
            lines.push_back(line), line.clear();
            lines += children_lines;
            line.indent(depth * 2);
         }
         line.push_back(is_array() ? "]" : "}");
      } else {
         line.push_back(format_value(quote));
      }
      if (line.size()) {
         if (next) line.push_back(", ");
         lines.push_back(line), line.clear();
      }
      return lines;
   }
};
