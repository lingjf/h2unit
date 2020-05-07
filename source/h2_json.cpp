
struct h2__json {
   static const int t_absent = 0;

   static const int t_null = 1;
   static const int t_boolean = 2;
   static const int t_number = 3;
   static const int t_string = 4;
   static const int t_regexp = 5;
   static const int t_array = 6;
   static const int t_object = 7;

   static constexpr char indent_char = ' ';
   static constexpr char samelength_char = ' ';
   static constexpr char occupy_char = ' ';
   static constexpr char columns_char = ' ';

   struct P {
      const char* text;
      int length;
      int offset;

      struct P& strip()
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

   struct Node : h2_libc {
      int type;

      h2_string key_string;
      h2_string value_string;
      double value_double;
      bool value_boolean;
      h2_vector<Node*> children; /* array or object */

      Node() : type(t_null), value_double(0), value_boolean(false) {}

      int size() { return children.size(); }

      Node* get(int index) { return 0 <= index && index < children.size() ? children[index] : nullptr; }

      Node* get(const char* name)
      {
         if (name)
            for (auto node : children)
               if (!node->key_string.compare(name))
                  return node;

         return nullptr;
      }

      void del(Node* child)
      {
         for (auto it = children.begin(); it != children.end(); it++)
            if (child == *it) {
               children.erase(it);
               delete child;
               return;
            }
      }

      bool is_null() { return t_null == type; }
      bool is_bool() { return t_boolean == type; }
      bool is_number() { return t_number == type; }
      bool is_string() { return t_string == type; }
      bool is_regexp() { return t_regexp == type; }
      bool is_array() { return t_array == type; }
      bool is_object() { return t_object == type; }

      bool parse_number(P& x)
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

      bool parse_string(P& x)
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

      bool parse_regexp(P& x)
      {
         bool ret = parse_string(x);
         type = t_regexp;
         return ret;
      }

      bool parse_value(P& x)
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

      bool parse_array(P& x)
      {
         x.offset++;  //pass [

         while (!x.strip().startswith(']')) {
            Node* new_node = new Node();
            if (!new_node) return false;

            children.push_back(new_node);

            if (!new_node->parse_value(x)) return false;

            if (x.strip().startswith(',')) x.offset++;
         }

         type = t_array;
         x.offset++;

         return true;
      }

      bool parse_object(P& x)
      {
         x.offset++;  //pass {

         while (!x.strip().startswith('}')) {
            Node* new_node = new Node();
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
   };

   static Node* parse(const char* json_string, int length = 0)
   {
      if (!json_string) return nullptr;
      if (length == 0) length = strlen(json_string);
      if (length == 0) return nullptr;

      P x;

      x.text = json_string;
      x.length = length;
      x.offset = 0;

      Node* root = new Node();
      if (!root->parse_value(x.strip())) return nullptr;

      return root;
   }

   static void frees(Node* root)
   {
      if (root) {
         for (auto node : root->children) frees(node);
         delete root;
      }
   }

   static bool match_array(Node* e, Node* a)
   {
      if (!e || !a) return false;
      if (e->children.size() != a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->children[i]))
            return false;
      return true;
   }

   static bool match_object(Node* e, Node* a)
   {
      if (!e || !a) return false;
      if (e->children.size() > a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->get(e->children[i]->key_string.c_str())))
            return false;
      return true;
   }

   static bool match(Node* e, Node* a)
   {
      if (!e || !a) return false;
      switch (e->type) {
      case t_null:
         return a->is_null();
      case t_boolean:
         return a->is_bool() && e->value_boolean == a->value_boolean;
      case t_number:
         return a->is_number() && ::fabs(e->value_double - a->value_double) < 0.00001;
      case t_string:
         return a->is_string() && e->value_string == a->value_string;
      case t_regexp:
         return a->is_string() && h2_regex_match(e->value_string.c_str(), a->value_string.c_str());
      case t_array:
         return a->is_array() && match_array(e, a);
      case t_object:
         return a->is_object() && match_object(e, a);
      default: return false;
      };
   }

   struct Dual : h2_libc {
      int depth;
      int e_type, a_type;
      h2_string e_key, a_key;
      h2_string e_value, a_value;
      h2_vector<Dual*> child;
      Dual* perent;

      Dual(int depth_, Dual* perent_) : depth(depth_), e_type(t_absent), a_type(t_absent), perent(perent_) {}
   };

   static void frees(Dual* root)
   {
      if (root) {
         for (auto dual : root->child) frees(dual);
         delete root;
      }
   }

   static void node2dual(Node* node, int& type, h2_string& key, h2_string& value)
   {
      if (!node) return;

      type = t_string;

      if (node->key_string.size()) key = "\"" + node->key_string + "\"";

      switch (node->type) {
      case t_null:
         type = t_string;
         value = "null";
         break;
      case t_boolean:
         type = t_string;
         value = node->value_boolean ? "true" : "false";
         break;
      case t_number:
         type = t_string;
         value.sprintf("%1.15g", node->value_double);
         break;
      case t_string:
         type = t_string;
         value = "\"" + node->value_string + "\"";
         break;
      case t_regexp:
         type = t_string;
         value = node->value_string;
         break;
      case t_array:
         type = t_array;
         break;
      case t_object:
         type = t_object;
         break;
      }
   }

   static void samelengthify(h2_string& e, h2_string& a)
   {
      int e_l = e.length(), a_l = a.length();

      e.append(std::max(e_l, a_l) - e_l, samelength_char);
      a.append(std::max(e_l, a_l) - a_l, samelength_char);
   }

   static void dual(Node* e, Node* a, Dual* d)
   {
      node2dual(e, d->e_type, d->e_key, d->e_value);
      node2dual(a, d->a_type, d->a_key, d->a_value);
      samelengthify(d->e_key, d->a_key);
      samelengthify(d->e_value, d->a_value);

      if (d->e_type != d->a_type) {
         if (d->e_type == t_object) d->e_type = t_string, d->e_value = "{ ... }";
         if (d->e_type == t_array) d->e_type = t_string, d->e_value = "[ ... ]";
         if (d->a_type == t_object) d->a_type = t_string, d->a_value = "{ ... }";
         if (d->a_type == t_array) d->a_type = t_string, d->a_value = "[ ... ]";
         samelengthify(d->e_value, d->a_value);
         return;
      }

      if (d->e_type == t_object) {
         for (auto i = e->children.begin(); i != e->children.end();) {
            Node *e1 = *i, *a1 = a->get(e1->key_string.c_str());
            if (!a1)
               for (auto& j : a->children)
                  if (match(e1, j)) {
                     a1 = j;
                     break;
                  }
            if (a1) {
               Dual* d1 = new Dual(d->depth + 1, d);
               d->child.push_back(d1);
               dual(e1, a1, d1);
               a->del(a1);
               i = e->children.erase(i);
               delete e1;
            } else
               i++;
         }

         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            Dual* d1 = new Dual(d->depth + 1, d);
            d->child.push_back(d1);
            Node *e1 = e->get(i), *a1 = a->get(i);
            dual(e1, a1, d1);
         }
      }

      if (d->e_type == t_array) {
         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            Dual* d1 = new Dual(d->depth + 1, d);
            d->child.push_back(d1);
            Node *e1 = e->get(i), *a1 = a->get(i);
            dual(e1, a1, d1);
         }
      }
   }

   static h2_string indent(int depth) { return h2_string(depth * 2, indent_char); }
   static h2_string occupy(h2_string p) { return h2_string(p.length(), occupy_char); }

   static void diff(Dual* d, h2_vector<h2_string>& e, h2_vector<h2_string>& a)
   {
      if (!d) return;
      e.push_back("\n");
      e.push_back(indent(d->depth));
      a.push_back("\n");
      a.push_back(indent(d->depth));

      if (d->e_type != t_absent) {
         if (d->a_type == t_absent) {  // only e-side exist
            if (d->e_key.size()) {
               e.push_back("#cyan");
               e.push_back(d->e_key + ": ");
               e.push_back("#reset");
            }
            if (d->e_value.size()) {
               e.push_back("#cyan");
               e.push_back(d->e_value);
               e.push_back("#reset");
            }
         } else {
            if (d->e_key.size()) {
               if (d->e_key != d->a_key) e.push_back("#green");
               e.push_back(d->e_key);
               if (d->e_key != d->a_key) e.push_back("#reset");
               e.push_back(": ");
            }
            if (d->e_value.size()) {
               if (d->e_value != d->a_value) e.push_back("#green");
               e.push_back(d->e_value);
               if (d->e_value != d->a_value) e.push_back("#reset");
            }
         }
      } else {
         if (d->a_key.size()) e.push_back(occupy(d->a_key + ": "));
         if (d->a_value.size()) e.push_back(occupy(d->a_value));
      }

      if (d->a_type != t_absent) {
         if (d->e_type == t_absent) {  // only a-side exist
            const char* style = "#red,bold";
            if (d->perent && d->perent->a_type == t_object) {
               style = "#yellow";
            }
            if (d->a_key.size()) {
               a.push_back(style);
               a.push_back(d->a_key + ": ");
               a.push_back("#reset");
            }
            if (d->a_value.size()) {
               a.push_back(style);
               a.push_back(d->a_value);
               a.push_back("#reset");
            }
         } else {
            if (d->a_key.size()) {
               if (d->a_key != d->e_key) a.push_back("#red,bold");
               a.push_back(d->a_key);
               if (d->a_key != d->e_key) a.push_back("#reset");
               a.push_back(": ");
            }
            if (d->a_value.size()) {
               if (d->a_value != d->e_value) a.push_back("#red,bold");
               a.push_back(d->a_value);
               if (d->a_value != d->e_value) a.push_back("#reset");
            }
         }
      } else {
         if (d->e_key.size()) a.push_back(occupy(d->e_key + ": "));
         if (d->e_value.size()) a.push_back(occupy(d->e_value));
      }

      /* e/a type shoud be same */

      if (d->e_type == t_object && d->a_type == t_object) {
         e.push_back("{");
         a.push_back("{");
      }
      if (d->e_type == t_array && d->a_type == t_array) {
         e.push_back("[");
         a.push_back("[");
      }

      if ((d->e_type == t_object && d->a_type == t_object) || (d->e_type == t_array && d->a_type == t_array)) {
         for (int i = 0; i < d->child.size(); i++) {
            diff(d->child[i], e, a);

            bool e_not_last = false, a_not_last = false;
            for (int j = i + 1; j < d->child.size(); j++) {
               e_not_last = e_not_last || (d->child[j]->e_type != t_absent);
               a_not_last = a_not_last || (d->child[j]->a_type != t_absent);
            }
            if (e_not_last) e.push_back(",");
            if (a_not_last) a.push_back(",");
         }
         if (d->child.size()) {
            e.push_back("\n");
            e.push_back(indent(d->depth));
            a.push_back("\n");
            a.push_back(indent(d->depth));
         }
      }

      if (d->e_type == t_object && d->a_type == t_object) {
         e.push_back("}");
         a.push_back("}");
      }
      if (d->e_type == t_array && d->a_type == t_array) {
         e.push_back("]");
         a.push_back("]");
      }
   }

   typedef h2_vector<h2_string> Line;
   typedef h2_vector<Line> Lines;

   static void merge_line(h2_vector<h2_string>& list, Lines& lines)
   {
      Line line;
      for (auto& s : list) {
         if (s == "\n") {
            lines.push_back(line);
            line.clear();
            continue;
         }
         line.push_back(s);
      }
      lines.push_back(line);
      line.clear();
   }

   static int lines_most(Lines& lines)
   {
      int most = 0;
      for (auto& line : lines) {
         int curr = 0;
         for (auto& word : line)
            if (word[0] != '#') curr += word.length();
         most = std::max(most, curr);
      }
      return most;
   }

   static int line_wrap(Line& line, int columns)
   {
      int char_count = 0;
      for (auto& word : line)
         if (word[0] != '#') char_count += word.length();

      return ::ceil(char_count / (double)columns);  // num_of_line
   }

   static h2_string line_wrap(Line& line, int index, int columns, h2_string& current_style)
   {
      int s = 0, u = 0;
      h2_string wrap;
      for (auto& word : line)
         if (word[0] == '#') {
            if (index * columns <= s && s < (index + 1) * columns) {
               const char* style = word.c_str() + 1;
               wrap.append(O.style(style));
               current_style = style;
            }
         } else {
            for (auto& c : word) {
               if (index * columns <= s && s < (index + 1) * columns) {
                  wrap.append(1, c);
                  ++u;
               }
               ++s;
            }
         }

      wrap.append(columns - u, columns_char);
      return wrap;
   }

   static void print(Lines& e_lines, Lines& a_lines, int side_width, h2_string& str)
   {
      h2_string e_last_style, a_last_style;
      // assert(e_lines.size() == a_lines.size());
      for (int i = 0; i < std::max(e_lines.size(), a_lines.size()); ++i) {
         auto e_line = e_lines[i];
         auto a_line = a_lines[i];
         int e_wraps = line_wrap(e_line, side_width - 2);
         int a_wraps = line_wrap(a_line, side_width - 2);
         // assert(e_wraps == a_wraps);
         int K = std::max(e_wraps, a_wraps);
         for (int j = 0; j < K; ++j) {
            h2_string e_current_style, a_current_style;
            auto e_wrap = line_wrap(e_line, j, side_width - 2, e_current_style);
            auto a_wrap = line_wrap(a_line, j, side_width - 2, a_current_style);
            str.sprintf("%s %s %s %s\n",
                        SF(e_last_style.c_str(), "%s", e_wrap.c_str()),
                        SF("dark gray", j == K - 1 ? " │" : "\\│"),
                        SF(a_last_style.c_str(), "%s", a_wrap.c_str()),
                        SF("dark gray", j == K - 1 ? " " : "\\"));

            e_last_style = e_current_style;
            a_last_style = a_current_style;
         }
      }
   }
};

h2_inline bool h2_json::match(const h2_string expect, const h2_string actual)
{
   h2__json::Node *e = h2__json::parse(expect.c_str()), *a = h2__json::parse(actual.c_str());

   bool result = h2__json::match(e, a);

   h2__json::frees(e);
   h2__json::frees(a);

   return result;
}

h2_inline int h2_json::diff(const h2_string expect, const h2_string actual, int terminal_width, h2_string& str)
{
   h2__json::Node *e_node = h2__json::parse(expect.c_str()), *a_node = h2__json::parse(actual.c_str());

   h2__json::Dual* d = new h2__json::Dual(0, nullptr);
   h2__json::dual(e_node, a_node, d);

   h2__json::frees(e_node);
   h2__json::frees(a_node);

   h2_vector<h2_string> e_list, a_list;
   h2__json::diff(d, e_list, a_list);
   h2__json::frees(d);

   h2__json::Lines e_lines, a_lines;
   h2__json::merge_line(e_list, e_lines);
   h2__json::merge_line(a_list, a_lines);

   int e_most = h2__json::lines_most(e_lines), a_most = h2__json::lines_most(a_lines);
   int fav_width = std::max(std::max(e_most, a_most) + 3, 30);
   int side_width = std::min(terminal_width / 2 - 4, fav_width);

   h2__json::print(e_lines, a_lines, side_width, str);
   return side_width;
}
