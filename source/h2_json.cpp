
struct h2_json {
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

   struct X {
      const char* text;
      int length;
      int offset;

      struct X& strip() {
         while (offset < length && ::isspace(text[offset])) offset++;
         return *this;
      }
      bool startswith(const char* s, int n) {
         if (length - offset < n) return false;
         return ::strncmp(text + offset, s, n) == 0;
      }
      bool startswith(char from, char to = '\0') {
         if (length - offset < 1) return false;
         if (to == '\0') to = from;
         return from <= text[offset] && text[offset] <= to;
      }
   };

   struct node {
      int type;

      h2_string key_string;
      h2_string value_string;
      double value_double;
      bool value_boolean;
      h2_vector<node*> children; /* array or object */

      node() : type(t_null), value_double(0), value_boolean(false) {}

      int size() { return children.size(); }

      node* get(int index) { return 0 <= index && index < (int)children.size() ? children[index] : nullptr; }

      node* get(const char* name) {
         if (!name) return nullptr;

         for (auto it = children.begin(); it != children.end(); it++)
            if (!(*it)->key_string.compare(name)) return *it;

         return nullptr;
      }

      void del(node* child) {
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

      bool parse_number(X& x) {
         int i;
         for (i = 0; x.offset + i < x.length; ++i) {
            const char c = x.text[x.offset + i];
            if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0') {
               break;
            }
         }

         value_string.assign(x.text + x.offset, i);

         int err = 0;
         value_double = tinyexpr::te_interp(value_string.c_str(), &err);
         type = t_number;
         x.offset += i;

         return 0 == err;
      }

      bool parse_string(X& x) {
         const char bound = x.text[x.offset];
         x.offset++;

         if (x.length <= x.offset) return false;

         const char* src = x.text + x.offset;
         int len = 0;
         for (; x.text[x.offset] != bound; ++len) {
            if (x.text[x.offset++] == '\\') x.offset++;
            if (x.length <= x.offset) return false;
         }

         for (; len > 0; ++src, --len) {
            if (*src != '\\') {
               value_string.push_back(*src);
            } else {
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
            }
         }

         type = t_string;
         x.offset++;

         return true;
      }

      bool parse_regexp(X& x) {
         bool ret = parse_string(x);
         type = t_regexp;
         return ret;
      }

      bool parse_value(X& x) {
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
         /* t_string */
         if (x.startswith('\"') || x.startswith('\'')) return parse_string(x);
         /* t_regexp */
         if (x.startswith('/')) return parse_regexp(x);

         /* t_array */
         if (x.startswith('[')) return parse_array(x);
         /* t_object */
         if (x.startswith('{')) return parse_object(x);

         /* t_number */
         if (1 /* x.startswith('-') || x.startswith('0', '9') */) return parse_number(x);

         return false;
      }

      bool parse_array(X& x) {
         x.offset++;  //pass [

         while (!x.strip().startswith(']')) {
            node* new_node = new node();
            if (!new_node) return false;

            children.push_back(new_node);

            if (!new_node->parse_value(x)) return false;

            if (x.strip().startswith(',')) x.offset++;
         }

         type = t_array;
         x.offset++;

         return true;
      }

      bool parse_object(X& x) {
         x.offset++;  //pass {

         while (!x.strip().startswith('}')) {
            node* new_node = new node();
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

      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };

   static node* parse(const char* json_string, int length = 0) {
      if (length == 0) length = strlen(json_string);

      if (!json_string || length == 0) return nullptr;

      X x;

      x.text = json_string;
      x.length = length;
      x.offset = 0;

      node* root = new node();
      if (!root->parse_value(x.strip())) return nullptr;

      return root;
   }

   static void frees(node* root) {
      for (auto it = root->children.begin(); it != root->children.end(); it++) frees(*it);
      delete root;
   }

   static bool match_array(node* e, node* a) {
      if (!e || !a) return false;
      if (e->children.size() != a->children.size()) return false;
      for (size_t i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->children[i])) return false;
      return true;
   }

   static bool match_object(node* e, node* a) {
      if (!e || !a) return false;
      if (e->children.size() > a->children.size()) return false;
      for (size_t i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->get(e->children[i]->key_string.c_str()))) return false;
      return true;
   }

   static bool match(node* e, node* a) {
      if (!e || !a) return false;

      switch (e->type) {
      case t_null:
         if (a->is_null()) return true;
         break;
      case t_boolean:
         if (a->is_bool() && e->value_boolean == a->value_boolean) return true;
         break;
      case t_number:
         if (a->is_number() && fabs(e->value_double - a->value_double) < 0.00001) return true;
         break;
      case t_string:
         if (a->is_string() && e->value_string == a->value_string) return true;
         break;
      case t_regexp:
         if (a->is_string()) {
            bool result;
            try {
               std::regex re(e->value_string.c_str());
               result = std::regex_match(a->value_string.c_str(), re);
            }
            catch (const std::regex_error& e) {
               result = false;
            }
            return result;
         }
         break;
      case t_array:
         if (a->is_array() && match_array(e, a)) return true;
         break;
      case t_object:
         if (a->is_object() && match_object(e, a)) return true;
         break;
      };

      return false;
   }

   static bool match(const char* expect, const char* actual) {
      node *e = parse(expect), *a = parse(actual);

      bool result = match(e, a);

      frees(e);
      frees(a);

      return result;
   }

   struct dual {
      int depth;
      int e_type, a_type;
      h2_string e_key, a_key;
      h2_string e_value, a_value;
      h2_vector<dual*> child;
      dual* perent;

      dual(int depth_, dual* perent_) : depth(depth_), e_type(t_absent), a_type(t_absent), perent(perent_) {}

      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };

   static void frees(dual* root) {
      for (size_t i = 0; i < root->child.size(); ++i) frees(root->child[i]);
      delete root;
   }

   static void node2dual(node* n, int& type, h2_string& key, h2_string& value) {
      if (!n) return;

      char t[128];
      type = t_string;

      if (n->key_string.size()) key = "\"" + n->key_string + "\"";

      switch (n->type) {
      case t_null:
         type = t_string;
         value = "null";
         return;
      case t_boolean:
         type = t_string;
         value = n->value_boolean ? "true" : "false";
         return;
      case t_number:
         type = t_string;
         sprintf(t, "%1.15g", n->value_double);
         value = t;
         return;
      case t_string:
         type = t_string;
         value = "\"" + n->value_string + "\"";
         return;
      case t_regexp:
         type = t_string;
         value = n->value_string;
         return;
      case t_array:
         type = t_array;
         return;
      case t_object:
         type = t_object;
         return;
      }
      return;
   }

   static void samelengthify(h2_string& e, h2_string& a) {
      int e_l = e.length(), a_l = a.length();

      e.append(std::max(e_l, a_l) - e_l, samelength_char);
      a.append(std::max(e_l, a_l) - a_l, samelength_char);
   }

   static void __dual(node* e, node* a, dual* d) {
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
            node *e1 = *i, *a1 = a->get(e1->key_string.c_str());
            if (!a1) {
               for (auto j = a->children.begin(); j != a->children.end(); j++)
                  if (match(e1, *j)) {
                     a1 = *j;
                     break;
                  }
            }
            if (a1) {
               dual* d1 = new dual(d->depth + 1, d);
               d->child.push_back(d1);
               __dual(e1, a1, d1);
               a->del(a1);
               i = e->children.erase(i);
               delete e1;
            } else {
               i++;
            }
         }

         for (size_t i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            dual* d1 = new dual(d->depth + 1, d);
            d->child.push_back(d1);
            node *e1 = e->get(i), *a1 = a->get(i);
            __dual(e1, a1, d1);
         }
      }

      if (d->e_type == t_array) {
         for (size_t i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            dual* d1 = new dual(d->depth + 1, d);
            d->child.push_back(d1);
            node *e1 = e->get(i), *a1 = a->get(i);
            __dual(e1, a1, d1);
         }
      }
   }

   static h2_string indent(int depth) { return h2_string(depth * 2, indent_char); }
   static h2_string occupy(h2_string p) { return h2_string(p.length(), occupy_char); }

   static void diff(dual* d, h2_vector<h2_string>& e, h2_vector<h2_string>& a) {
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
         for (size_t i = 0; i < d->child.size(); i++) {
            diff(d->child[i], e, a);

            bool e_not_last = false, a_not_last = false;
            for (size_t j = i + 1; j < d->child.size(); j++) {
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

   typedef h2_vector<h2_string> line;
   typedef h2_vector<line> lines;

   static void merge_line(h2_vector<h2_string>& x_list, lines& x_lines) {
      line x_line;
      for (auto it = x_list.begin(); it != x_list.end(); it++) {
         if ((*it) == "\n") {
            x_lines.push_back(x_line);
            x_line.clear();
            continue;
         }
         x_line.push_back(*it);
      }
      x_lines.push_back(x_line);
      x_line.clear();
   }

   static int lines_most(lines& x_lines) {
      int most = 0;
      for (auto i = x_lines.begin(); i != x_lines.end(); i++) {
         int curr = 0;
         for (auto j = (*i).begin(); j != (*i).end(); j++)
            if ((*j)[0] != '#') curr += (*j).length();
         most = std::max(most, curr);
      }
      return most;
   }

   static int line_wrap(line& x_line, int columns) {
      int char_count = 0;
      for (auto i = x_line.begin(); i != x_line.end(); i++)
         if ((*i)[0] != '#') char_count += (*i).length();

      return H2_DIV_ROUND_UP(char_count, columns);  // num_of_line
   }

   static h2_string line_wrap(line& x_line, int index, int columns, h2_string& current_style) {
      int s = 0, u = 0;
      h2_string wrap;
      for (auto i = x_line.begin(); i != x_line.end(); i++) {
         auto word = *i;
         if (word[0] == '#') {
            if (index * columns <= s && s < (index + 1) * columns) {
               const char* style = h2_cfg().style(word.c_str() + 1);
               wrap.append(style);
               current_style = style;
            }
         } else {
            for (auto j = word.begin(); j != word.end(); j++) {
               if (index * columns <= s && s < (index + 1) * columns) {
                  wrap.append(1, *j);
                  ++u;
               }
               ++s;
            }
         }
      }

      wrap.append(columns - u, columns_char);
      return wrap;
   }

   static void print(lines& e_lines, lines& a_lines, int side_columns) {
      h2_string e_last_style, a_last_style;
      assert(e_lines.size() == a_lines.size());
      for (size_t i = 0; i < std::max(e_lines.size(), a_lines.size()); ++i) {
         auto e_line = e_lines[i];
         auto a_line = a_lines[i];
         int e_wraps = line_wrap(e_line, side_columns - 2);
         int a_wraps = line_wrap(a_line, side_columns - 2);
         assert(e_wraps == a_wraps);
         int K = std::max(e_wraps, a_wraps);
         for (int j = 0; j < K; ++j) {
            h2_string e_current_style, a_current_style;
            auto e_wrap = line_wrap(e_line, j, side_columns - 2, e_current_style);
            auto a_wrap = line_wrap(a_line, j, side_columns - 2, a_current_style);
            ::printf("%s%s %s%s%s│%s %s%s %s%s%s%s\n",
                     e_last_style.c_str(), e_wrap.c_str(), h2_cfg().style("reset"),
                     h2_cfg().style("dark gray"), j == K - 1 ? " " : "\\", h2_cfg().style("reset"),
                     a_last_style.c_str(), a_wrap.c_str(), h2_cfg().style("reset"),
                     h2_cfg().style("dark gray"), j == K - 1 ? " " : "\\", h2_cfg().style("reset"));

            e_last_style = e_current_style;
            a_last_style = a_current_style;
         }
      }
   }

   static void diff_print(const char* expect, const char* actual, int terminal_columns) {
      node *e_node = parse(expect), *a_node = parse(actual);

      dual* d = new dual(0, nullptr);
      __dual(e_node, a_node, d);

      frees(e_node);
      frees(a_node);

      h2_vector<h2_string> e_list, a_list;
      diff(d, e_list, a_list);
      frees(d);

      lines e_lines, a_lines;
      merge_line(e_list, e_lines);
      merge_line(a_list, a_lines);

      int e_most = lines_most(e_lines), a_most = lines_most(a_lines);

      int fav_columns = std::max(std::max(e_most, a_most) + 3, 30);

      int side_columns = std::min(terminal_columns / 2 - 4, fav_columns);

      char t1[256], t2[256];
      ::printf("%s%s%s%s│%s%s%s%s\n",
               h2_cfg().style("dark gray"),
               h2_center_string("expect", side_columns, t1),
               h2_cfg().style("reset"),
               h2_cfg().style("dark gray"), h2_cfg().style("reset"),
               h2_cfg().style("dark gray"),
               h2_center_string("actual", side_columns, t2),
               h2_cfg().style("reset"));

      print(e_lines, a_lines, side_columns);
   }
};
