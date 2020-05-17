
struct h2_json_parse {
   const char* text;
   int length;
   int offset;

   h2_json_parse(const char* text_, int length_) : text(text_), length(length_), offset(0) {}
   struct h2_json_parse& strip()
   {
      while (offset < length && ::isspace(text[offset])) offset++;
      return *this;
   }
   bool startswith(const char* s, int n)
   {
      if (length - offset < n) return false;
      return ::strncmp(text + offset, s, n) == 0;
   }
   bool startswith(char from, char to = '\0')
   {
      if (length - offset < 1) return false;
      if (to == '\0') to = from;
      return from <= text[offset] && text[offset] <= to;
   }
};

struct h2_json_node : h2_libc {
   static const int t_absent = 0;
   static const int t_null = 1;
   static const int t_boolean = 2;
   static const int t_number = 3;
   static const int t_string = 4;
   static const int t_regexp = 5;
   static const int t_array = 6;
   static const int t_object = 7;

   int type;
   h2_string key_string;
   h2_string value_string;
   double value_double;
   bool value_boolean;
   h2_vector<h2_json_node*> children; /* array or object */

   ~h2_json_node()
   {
      for (auto it : children) delete it;
   }
   h2_json_node() : type(t_absent), value_double(0), value_boolean(false) {}
   h2_json_node(const char* json_string, int length = 0) : h2_json_node()
   {
      if (json_string) {
         if (length == 0) length = strlen(json_string);
         if (length) {
            h2_json_parse x(json_string, length);
            if (!parse_value(x.strip())) type = t_absent;
         }
      }
   }

   int size() { return children.size(); }

   h2_json_node* get(int index) { return 0 <= index && index < children.size() ? children[index] : nullptr; }

   h2_json_node* get(const char* name)
   {
      if (name)
         for (auto node : children)
            if (!node->key_string.compare(name))
               return node;

      return nullptr;
   }

   void del(h2_json_node* child)
   {
      for (auto it = children.begin(); it != children.end(); it++) {
         if (child == *it) {
            children.erase(it);
            delete child;
            return;
         }
      }
   }

   bool is_null() { return t_null == type; }
   bool is_bool() { return t_boolean == type; }
   bool is_number() { return t_number == type; }
   bool is_string() { return t_string == type; }
   bool is_regexp() { return t_regexp == type; }
   bool is_array() { return t_array == type; }
   bool is_object() { return t_object == type; }

   bool parse_number(h2_json_parse& x)
   {
      int i;
      for (i = 0; x.offset + i < x.length; ++i) {
         const char c = x.text[x.offset + i];
         if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0')
            break;
      }

      value_string.assign(x.text + x.offset, i);

      int err = 0;
      value_double = tinyexpr::eval(value_string.c_str(), &err);
      type = t_number;
      x.offset += i;

      return 0 == err;
   }

   bool parse_string(h2_json_parse& x)
   {
      const char bound = x.text[x.offset];
      x.offset++;

      if (x.length <= x.offset) return false;

      const char* src = x.text + x.offset;
      int len = 0;
      for (; x.text[x.offset] != bound; ++len) {
         if (x.text[x.offset++] == '\\') x.offset++;
         if (x.length <= x.offset) return false;
      }

      for (; len > 0; ++src, --len)
         if (*src != '\\')
            value_string.push_back(*src);
         else
            switch (*++src) {
            case 'b': value_string.push_back('\b'); break;
            case 'f': value_string.push_back('\f'); break;
            case 'n': value_string.push_back('\n'); break;
            case 'r': value_string.push_back('\r'); break;
            case 't': value_string.push_back('\t'); break;
            case '\"': value_string.push_back('\"'); break;
            case '\\': value_string.push_back('\\'); break;
            case '/': value_string.push_back('/'); break;
            default: return false;
            }

      type = t_string;
      x.offset++;

      return true;
   }

   bool parse_regexp(h2_json_parse& x)
   {
      bool ret = parse_string(x);
      type = t_regexp;
      return ret;
   }

   bool parse_value(h2_json_parse& x)
   {
      /* t_null */
      if (x.startswith("null", 4)) {
         type = t_null;
         x.offset += 4;
         return true;
      }
      /* false */
      if (x.startswith("false", 5)) {
         type = t_boolean;
         value_boolean = false;
         x.offset += 5;
         return true;
      }
      /* true */
      if (x.startswith("true", 4)) {
         type = t_boolean;
         value_boolean = true;
         x.offset += 4;
         return true;
      }
      /* string */
      if (x.startswith('\"') || x.startswith('\'')) return parse_string(x);
      /* regexp */
      if (x.startswith('/')) return parse_regexp(x);

      /* array */
      if (x.startswith('[')) return parse_array(x);
      /* object */
      if (x.startswith('{')) return parse_object(x);

      /* number */
      if (1 /* x.startswith('-') || x.startswith('0', '9') */) return parse_number(x);

      return false;
   }

   bool parse_array(h2_json_parse& x)
   {
      x.offset++;  //pass [

      while (!x.strip().startswith(']')) {
         h2_json_node* new_node = new h2_json_node();
         if (!new_node) return false;

         children.push_back(new_node);

         if (!new_node->parse_value(x)) return false;

         if (x.strip().startswith(',')) x.offset++;
      }

      type = t_array;
      x.offset++;

      return true;
   }

   bool parse_object(h2_json_parse& x)
   {
      x.offset++;  //pass {

      while (!x.strip().startswith('}')) {
         h2_json_node* new_node = new h2_json_node();
         if (!new_node) return false;

         children.push_back(new_node);

         if (!new_node->parse_string(x)) return false;

         new_node->key_string = new_node->value_string;
         new_node->value_string = "";

         if (!x.strip().startswith(':')) return false;
         x.offset++;

         if (!new_node->parse_value(x.strip())) return false;

         if (x.strip().startswith(',')) x.offset++;
      }

      type = t_object;
      x.offset++;

      return true;
   }

   void dual(int& _type, const char*& _class, h2_string& _key, h2_string& _value)
   {
      _type = t_string;

      if (key_string.size()) _key = "\"" + key_string + "\"";

      switch (type) {
      case t_null:
         _type = t_string;
         _class = "atomic";
         _value = "null";
         break;
      case t_boolean:
         _type = t_string;
         _class = "atomic";
         _value = value_boolean ? "true" : "false";
         break;
      case t_number:
         _type = t_string;
         _class = "atomic";
         _value = h2_stringify(value_double);
         break;
      case t_string:
         _type = t_string;
         _class = "atomic";
         _value = "\"" + value_string + "\"";
         break;
      case t_regexp:
         _type = t_string;
         _class = "atomic";
         _value = value_string;
         break;
      case t_array:
         _type = t_array;
         _class = "array";
         break;
      case t_object:
         _type = t_object;
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
      else if (is_number())
         line.push_back(h2_stringify(value_double));
      else if (is_string())
         line.push_back("\"" + value_string + "\"");
      else if (is_regexp())
         line.push_back("\"/" + value_string + "/\"");
      else if (is_array() || is_object()) {
         line.push_back(is_array() ? "[ " : "{ ");
         h2_lines __lines;
         for (size_t i = 0; i < children.size(); ++i)
            children[i]->print(__lines, depth + 1, children.size() - i - 1, fold);

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

struct h2_json_match {
   static bool match_array(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return false;
      if (e->children.size() != a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->children[i]))
            return false;
      return true;
   }

   static bool match_object(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return false;
      if (e->children.size() > a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->get(e->children[i]->key_string.c_str())))
            return false;
      return true;
   }

   static bool match(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return false;
      switch (e->type) {
      case h2_json_node::t_null:
         return a->is_null();
      case h2_json_node::t_boolean:
         return a->is_bool() && e->value_boolean == a->value_boolean;
      case h2_json_node::t_number:
         return a->is_number() && ::fabs(e->value_double - a->value_double) < 0.00001;
      case h2_json_node::t_string:
         return a->is_string() && e->value_string == a->value_string;
      case h2_json_node::t_regexp:
         return a->is_string() && h2_regex_match(e->value_string.c_str(), a->value_string.c_str());
      case h2_json_node::t_array:
         return a->is_array() && match_array(e, a);
      case h2_json_node::t_object:
         return a->is_object() && match_object(e, a);
      default: return false;
      };
   }

   static h2_json_node* search(h2_vector<h2_json_node*>& haystack, h2_json_node* needle)
   {
      for (auto node : haystack)
         if (match(needle, node)) return node;

      return nullptr;
   }
};

struct h2_json_dual : h2_libc {  // combine 2 Node into a Dual
   bool match;
   int e_type, a_type;
   const char *e_class, *a_class;
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_lines e_blob, a_blob;
   h2_vector<h2_json_dual*> children;
   h2_json_dual* perent;
   int depth;

   ~h2_json_dual()
   {
      for (auto it : children) delete it;
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, h2_json_dual* perent_ = nullptr) : match(false), e_type(h2_json_node::t_absent), a_type(h2_json_node::t_absent), e_class("blob"), a_class("blob"), perent(perent_), depth(perent_ ? perent_->depth + 1 : 0)
   {
      match = h2_json_match::match(e, a);
      if (e) e->dual(e_type, e_class, e_key, e_value);
      if (a) a->dual(a_type, a_class, a_key, a_value);

      if (e_type != a_type) {
         if (e) e->print(e_blob, depth, 0, true);
         if (a) a->print(a_blob, depth, 0, true);
         e_class = a_class = "blob";
      } else if (e_type == h2_json_node::t_object) {
         for (auto i = e->children.begin(); i != e->children.end();) {
            h2_json_node *e1 = *i, *a1 = a->get(e1->key_string.c_str());
            if (!a1) a1 = h2_json_match::search(a->children, e1);
            if (a1) {
               children.push_back(new h2_json_dual(e1, a1, this));
               a->del(a1);
               i = e->children.erase(i);
               delete e1;
            } else
               i++;
         }

         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i)
            children.push_back(new h2_json_dual(e->get(i), a->get(i), this));
      } else if (e_type == h2_json_node::t_array) {
         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i)
            children.push_back(new h2_json_dual(e->get(i), a->get(i), this));
      }
   }

   bool has_next_e(h2_vector<h2_json_dual*>& subling)
   {
      for (size_t i = 0; i < subling.size(); ++i) {
         if (subling[i] == this) {
            for (++i; i < subling.size(); ++i)
               if (subling[i]->e_type != h2_json_node::t_absent)
                  return true;
            break;
         }
      }
      return false;
   }

   bool has_next_a(h2_vector<h2_json_dual*>& subling)
   {
      for (size_t i = 0; i < subling.size(); ++i) {
         if (subling[i] == this) {
            for (++i; i < subling.size(); ++i)
               if (subling[i]->a_type != h2_json_node::t_absent)
                  return true;
            break;
         }
      }
      return false;
   }

   void align(h2_lines& e, h2_lines& a, h2_vector<h2_json_dual*>* subling = nullptr)
   {
      if (!strcmp(e_class, "blob")) {
         e_blob.samesizify(a_blob);
         for (auto& line : e_blob)
            line.insert(line.begin(), "\033{bold,cyan}"), line.push_back("\033{reset}");

         for (auto& line : a_blob)
            line.insert(line.begin(), "\033{yellow}"), line.push_back("\033{reset}");

         e.concat_back(e_blob);
         a.concat_back(a_blob);
         return;
      }

      h2_line e_line, a_line;
      e_line.indent(depth * 2);
      a_line.indent(depth * 2);

      if (e_key.size()) {
         if (!match && e_key != a_key) e_line.push_back("\033{green}");
         e_line.push_back(e_key);
         if (!match && e_key != a_key) e_line.push_back("\033{reset}");
         e_line.push_back(": ");
      }

      if (a_key.size()) {
         if (!match && a_key != e_key) a_line.push_back("\033{red,bold}");
         a_line.push_back(a_key);
         if (!match && a_key != e_key) a_line.push_back("\033{reset}");
         a_line.push_back(": ");
      }

      if (!strcmp(e_class, "atomic")) {
         if (e_value.size()) {
            if (!match && e_value != a_value) e_line.push_back("\033{green}");
            e_line.push_back(e_value);
            if (!match && e_value != a_value) e_line.push_back("\033{reset}");
         }
         if (a_value.size()) {
            if (!match && a_value != e_value) a_line.push_back("\033{red,bold}");
            a_line.push_back(a_value);
            if (!match && a_value != e_value) a_line.push_back("\033{reset}");
         }
      } else if (!strcmp(e_class, "object") || !strcmp(e_class, "array")) {
         e_line.push_back(strcmp(e_class, "object") ? "[ " : "{ ");
         a_line.push_back(strcmp(a_class, "object") ? "[ " : "{ ");

         e.push_back(e_line), e_line.clear();
         a.push_back(a_line), a_line.clear();
         for (size_t i = 0; i < children.size(); ++i)
            children[i]->align(e, a, &children);

         e_line.indent(depth * 2);
         e_line.push_back(strcmp(e_class, "object") ? "]" : "}");
         a_line.indent(depth * 2);
         a_line.push_back(strcmp(a_class, "object") ? "]" : "}");
      }
      if (e_line.size()) {
         if (subling && has_next_e(*subling)) e_line.push_back(",");
         e.push_back(e_line), e_line.clear();
      }
      if (a_line.size()) {
         if (subling && has_next_a(*subling)) a_line.push_back(",");
         a.push_back(a_line), a_line.clear();
      }
   }
};

h2_inline bool h2_json::match(const h2_string& expect, const h2_string& actual)
{
   h2_json_node e(expect.c_str());
   h2_json_node a(actual.c_str());
   return h2_json_match::match(&e, &a);
}

h2_inline void h2_json::diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines)
{
   h2_json_node e_node(expect.c_str());
   h2_json_node a_node(actual.c_str());
   h2_json_dual dual(&e_node, &a_node);
   dual.align(e_lines, a_lines);
}
