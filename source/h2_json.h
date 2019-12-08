
#ifndef ___H2_JSON__H___
#define ___H2_JSON__H___

struct h2json
{
   static const int absent = 0;

   static const int null = 1;
   static const int boolean = 2;
   static const int number = 3;
   static const int string = 4;
   static const int regexp = 5;
   static const int array = 6;
   static const int object = 7;

   static constexpr char indent_char = ' ';
   static constexpr char samelength_char = ' ';
   static constexpr char occupy_char = ' ';
   static constexpr char columns_char = ' ';

   struct X
   {
      const char* text;
      int length;
      int offset;

      struct X& strip()
      {
         while (offset < length && ::isspace(text[offset])) offset++;
         return *this;
      }
      bool startwith(const char* s, int n)
      {
         if (length - offset < n) return false;
         return ::strncmp(text + offset, s, n) == 0;
      }
      bool startwith(char from, char to = '\0')
      {
         if (length - offset < 1) return false;
         if (to == '\0') to = from;
         return from <= text[offset] && text[offset] <= to;
      }
   };

   struct node
   {
      int type;

      h2_string key_string;
      h2_string value_string;
      double value_double;
      bool value_boolean;
      std::vector<node*, h2_allocator<node*>> child_array_or_object;

      node()
        : type(null), value_double(0), value_boolean(false) {}

      int size()
      {
         return child_array_or_object.size();
      }

      node* get(int index)
      {
         if (index < 0 || (int)child_array_or_object.size() <= index) {
            return nullptr;
         }

         return child_array_or_object[index];
      }

      node* get(const char* name)
      {
         if (!name) {
            return nullptr;
         }

         for (auto it = child_array_or_object.begin(); it != child_array_or_object.end(); it++) {
            if (!(*it)->key_string.compare(name)) {
               return *it;
            }
         }

         return nullptr;
      }

      void del(node* child)
      {
         for (auto it = child_array_or_object.begin(); it != child_array_or_object.end(); it++) {
            if (child == *it) {
               child_array_or_object.erase(it);
               delete child;
               return;
            }
         }
      }

      bool is_null()
      {
         return null == type;
      }
      bool is_bool()
      {
         return boolean == type;
      }
      bool is_number()
      {
         return number == type;
      }
      bool is_string()
      {
         return string == type;
      }
      bool is_regexp()
      {
         return regexp == type;
      }
      bool is_array()
      {
         return array == type;
      }
      bool is_object()
      {
         return object == type;
      }

      bool parse_number(X& x)
      {
         int i;
         for (i = 0; x.offset + i < x.length; ++i) {
            const char c = x.text[x.offset + i];
            if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0') {
               break;
            }
         }

         this->value_string.assign(x.text + x.offset, i);

         int err = 0;
         this->value_double = h2_tinyexpr::te_interp(this->value_string.c_str(), &err);
         type = number;
         x.offset += i;

         return 0 == err;
      }

      bool parse_string(X& x)
      {
         const char bound = x.text[x.offset];
         x.offset++;

         if (x.length <= x.offset) {
            return false;
         }

         const char* src = x.text + x.offset;
         int len = 0;
         for (; x.text[x.offset] != bound; ++len) {
            if (x.text[x.offset++] == '\\') {
               x.offset++;
            }
            if (x.length <= x.offset) {
               return false;
            }
         }

         for (; len > 0; ++src, --len) {
            if (*src != '\\') {
               this->value_string.push_back(*src);
            }
            else {
               switch (*++src) {
               case 'b': this->value_string.push_back('\b'); break;
               case 'f': this->value_string.push_back('\f'); break;
               case 'n': this->value_string.push_back('\n'); break;
               case 'r': this->value_string.push_back('\r'); break;
               case 't': this->value_string.push_back('\t'); break;
               case '\"': this->value_string.push_back('\"'); break;
               case '\\': this->value_string.push_back('\\'); break;
               case '/': this->value_string.push_back('/'); break;
               default: return false;
               }
            }
         }

         this->type = string;
         x.offset++;

         return true;
      }

      bool parse_regexp(X& x)
      {
         bool ret = parse_string(x);
         this->type = regexp;
         return ret;
      }

      bool parse_value(X& x)
      {
         /* null */
         if (x.startwith("null", 4)) {
            this->type = null;
            x.offset += 4;
            return true;
         }
         /* false */
         if (x.startwith("false", 5)) {
            this->type = boolean;
            this->value_boolean = false;
            x.offset += 5;
            return true;
         }
         /* true */
         if (x.startwith("true", 4)) {
            this->type = boolean;
            this->value_boolean = true;
            x.offset += 4;
            return true;
         }
         /* string */
         if (x.startwith('\"') || x.startwith('\'')) {
            return parse_string(x);
         }
         /* regexp */
         if (x.startwith('/')) {
            return parse_regexp(x);
         }

         /* array */
         if (x.startwith('[')) {
            return parse_array(x);
         }
         /* object */
         if (x.startwith('{')) {
            return parse_object(x);
         }

         /* number */
         if (1 /* x.startwith('-') || x.startwith('0', '9') */) {
            return parse_number(x);
         }

         return false;
      }

      bool parse_array(X& x)
      {
         x.offset++;  //pass [

         while (!x.strip().startwith(']')) {
            node* new_node = new node();
            if (!new_node) {
               return false;
            }

            child_array_or_object.push_back(new_node);

            if (!new_node->parse_value(x)) {
               return false;
            }

            if (x.strip().startwith(',')) {
               x.offset++;
            }
         }

         this->type = array;
         x.offset++;

         return true;
      }

      bool parse_object(X& x)
      {
         x.offset++;  //pass {

         while (!x.strip().startwith('}')) {
            node* new_node = new node();
            if (!new_node) {
               return false;
            }

            child_array_or_object.push_back(new_node);

            if (!new_node->parse_string(x)) {
               return false;
            }

            new_node->key_string = new_node->value_string;
            new_node->value_string = "";

            if (!x.strip().startwith(':')) {
               return false;
            }
            x.offset++;

            if (!new_node->parse_value(x.strip())) {
               return false;
            }

            if (x.strip().startwith(',')) {
               x.offset++;
            }
         }

         this->type = object;
         x.offset++;

         return true;
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }
   };

   static node* parse(const char* json_string, int length = 0)
   {
      if (length == 0) {
         length = strlen(json_string);
      }

      if (!json_string || length == 0) {
         return nullptr;
      }

      X x;

      x.text = json_string;
      x.length = length;
      x.offset = 0;

      node* root = new node();
      if (!root->parse_value(x.strip())) {
         return nullptr;
      }

      return root;
   }

   static void frees(node* root)
   {
      for (auto it = root->child_array_or_object.begin(); it != root->child_array_or_object.end(); it++) {
         frees(*it);
      }
      delete root;
   }

   static bool match_array(node* e, node* a)
   {
      if (!e || !a) {
         return false;
      }
      if (e->child_array_or_object.size() != a->child_array_or_object.size()) {
         return false;
      }

      for (size_t i = 0; i < e->child_array_or_object.size(); ++i) {
         if (!match(e->child_array_or_object[i], a->child_array_or_object[i])) return false;
      }
      return true;
   }

   static bool match_object(node* e, node* a)
   {
      if (!e || !a) {
         return false;
      }
      if (e->child_array_or_object.size() > a->child_array_or_object.size()) {
         return false;
      }
      for (size_t i = 0; i < e->child_array_or_object.size(); ++i) {
         if (!match(e->child_array_or_object[i], a->get(e->child_array_or_object[i]->key_string.c_str()))) return false;
      }

      return true;
   }

   static bool match(node* e, node* a)
   {
      if (!e || !a) {
         return false;
      }

      switch (e->type) {
      case null:
         if (a->is_null()) return true;
         break;
      case boolean:
         if (a->is_bool() && e->value_boolean == a->value_boolean) return true;
         break;
      case number:
         if (a->is_number() && fabs(e->value_double - a->value_double) < 0.00001) return true;
         break;
      case string:
         if (a->is_string() && e->value_string == a->value_string) return true;
         break;
      case regexp:
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
      case array:
         if (a->is_array() && match_array(e, a)) return true;
         break;
      case object:
         if (a->is_object() && match_object(e, a)) return true;
         break;
      };

      return false;
   }

   static bool match(const char* expect, const char* actual)
   {
      node* e = parse(expect);
      node* a = parse(actual);

      bool result = match(e, a);

      frees(e);
      frees(a);

      return result;
   }

   struct dual
   {
      int depth;
      int e_type, a_type;
      h2_string e_key, a_key;
      h2_string e_value, a_value;
      std::vector<dual*, h2_allocator<dual*>> child;
      dual* perent;

      dual(int depth_, dual* perent_)
        : depth(depth_), e_type(absent), a_type(absent), perent(perent_) {}

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }
   };

   static void frees(dual* root)
   {
      for (size_t i = 0; i < root->child.size(); ++i) {
         frees(root->child[i]);
      }
      delete root;
   }

   static void node2dual(node* n, int& type, h2_string& key, h2_string& value)
   {
      if (!n) return;

      char t[128];
      type = string;

      if (n->key_string.size()) {
         key = "\"" + n->key_string + "\"";
      }

      switch (n->type) {
      case null:
         type = string;
         value = "null";
         return;
      case boolean:
         type = string;
         value = n->value_boolean ? "true" : "false";
         return;
      case number:
         type = string;
         sprintf(t, "%1.15g", n->value_double);
         value = t;
         return;
      case string:
         type = string;
         value = "\"" + n->value_string + "\"";
         return;
      case regexp:
         type = string;
         value = n->value_string;
         return;
      case array:
         type = array;
         return;
      case object:
         type = object;
         return;
      }
      return;
   }

   static void samelengthify(h2_string& e, h2_string& a)
   {
      int e_l = e.length();
      int a_l = a.length();
      int m_l = std::max(e_l, a_l);

      e.append(m_l - e_l, samelength_char);
      a.append(m_l - a_l, samelength_char);
   }

   static void __dual(node* e, node* a, dual* d)
   {
      node2dual(e, d->e_type, d->e_key, d->e_value);
      node2dual(a, d->a_type, d->a_key, d->a_value);
      samelengthify(d->e_key, d->a_key);
      samelengthify(d->e_value, d->a_value);

      if (d->e_type != d->a_type) {
         if (d->e_type == object) {
            d->e_type = string;
            d->e_value = "{ ... }";
         }
         if (d->e_type == array) {
            d->e_type = string;
            d->e_value = "[ ... ]";
         }
         if (d->a_type == object) {
            d->a_type = string;
            d->a_value = "{ ... }";
         }
         if (d->a_type == array) {
            d->a_type = string;
            d->a_value = "[ ... ]";
         }
         samelengthify(d->e_value, d->a_value);
         return;
      }

      if (d->e_type == object) {
         for (auto i = e->child_array_or_object.begin(); i != e->child_array_or_object.end();) {
            node* e1 = *i;
            node* a1 = a->get(e1->key_string.c_str());
            if (!a1) {
               for (auto j = a->child_array_or_object.begin(); j != a->child_array_or_object.end(); j++) {
                  if (match(e1, *j)) {
                     a1 = *j;
                     break;
                  }
               }
            }
            if (a1) {
               dual* d1 = new dual(d->depth + 1, d);
               d->child.push_back(d1);
               __dual(e1, a1, d1);
               a->del(a1);
               i = e->child_array_or_object.erase(i);
               delete e1;
            }
            else {
               i++;
            }
         }

         size_t K = std::max(e->child_array_or_object.size(), a->child_array_or_object.size());
         for (size_t i = 0; i < K; ++i) {
            dual* d1 = new dual(d->depth + 1, d);
            d->child.push_back(d1);
            node* e1 = e->get(i);
            node* a1 = a->get(i);
            __dual(e1, a1, d1);
         }
      }

      if (d->e_type == array) {
         size_t K = std::max(e->child_array_or_object.size(), a->child_array_or_object.size());
         for (size_t i = 0; i < K; ++i) {
            dual* d1 = new dual(d->depth + 1, d);
            d->child.push_back(d1);
            node* e1 = e->get(i);
            node* a1 = a->get(i);
            __dual(e1, a1, d1);
         }
      }
   }

   static h2_string indent(int depth)
   {
      return h2_string(depth * 2, indent_char);
   }
   static h2_string occupy(h2_string p)
   {
      return h2_string(p.length(), occupy_char);
   }

   typedef std::vector<h2_string, h2_allocator<h2_string>> list;

   static void diff(dual* d, list& e, list& a)
   {
      if (!d) return;
      e.push_back("\n");
      e.push_back(indent(d->depth));
      a.push_back("\n");
      a.push_back(indent(d->depth));

      if (d->e_type != absent) {
         if (d->a_type == absent) {  // only e-side exist
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
         }
         else {
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
      }
      else {
         if (d->a_key.size()) {
            e.push_back(occupy(d->a_key + ": "));
         }
         if (d->a_value.size()) {
            e.push_back(occupy(d->a_value));
         }
      }

      if (d->a_type != absent) {
         if (d->e_type == absent) {  // only a-side exist
            const char* style = "#red,bold";
            if (d->perent && d->perent->a_type == object) {
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
         }
         else {
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
      }
      else {
         if (d->e_key.size()) {
            a.push_back(occupy(d->e_key + ": "));
         }
         if (d->e_value.size()) {
            a.push_back(occupy(d->e_value));
         }
      }

      /* e/a type shoud be same */

      if (d->e_type == object && d->a_type == object) {
         e.push_back("{");
         a.push_back("{");
      }
      if (d->e_type == array && d->a_type == array) {
         e.push_back("[");
         a.push_back("[");
      }

      if ((d->e_type == object && d->a_type == object) ||
          (d->e_type == array && d->a_type == array)) {
         for (size_t i = 0; i < d->child.size(); i++) {
            diff(d->child[i], e, a);

            bool e_not_last = false, a_not_last = false;
            for (size_t j = i + 1; j < d->child.size(); j++) {
               e_not_last = e_not_last || (d->child[j]->e_type != absent);
               a_not_last = a_not_last || (d->child[j]->a_type != absent);
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

      if (d->e_type == object && d->a_type == object) {
         e.push_back("}");
         a.push_back("}");
      }
      if (d->e_type == array && d->a_type == array) {
         e.push_back("]");
         a.push_back("]");
      }
   }

   typedef std::vector<h2_string, h2_allocator<h2_string>> line;
   typedef std::vector<line, h2_allocator<line>> lines;

   static void merge_line(list& x_list, lines& x_lines)
   {
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

   static int lines_most(lines& x_lines)
   {
      int most = 0;
      for (auto i = x_lines.begin(); i != x_lines.end(); i++) {
         auto x_line = *i;
         int curr = 0;
         for (auto j = x_line.begin(); j != x_line.end(); j++) {
            auto word = *j;
            if (word[0] == '#') {
               continue;
            }
            else {
               curr += word.length();
            }
         }
         most = std::max(most, curr);
      }
      return most;
   }

   static int line_wrap(line& x_line, int columns)
   {
      int char_count = 0;
      for (auto i = x_line.begin(); i != x_line.end(); i++) {
         auto word = *i;
         if (word[0] == '#') {
            continue;
         }
         else {
            char_count += word.length();
         }
      }

      int num_of_line = H2_DIV_ROUND_UP(char_count, columns);
      return num_of_line;
   }

   static h2_string line_wrap(line& x_line, int index, int columns, h2_string& current_style)
   {
      int s = 0, u = 0;
      h2_string wrap;
      for (auto i = x_line.begin(); i != x_line.end(); i++) {
         auto word = *i;
         if (word[0] == '#') {
            if (index * columns <= s && s < (index + 1) * columns) {
               const char* style = h2_cfg::style(word.c_str() + 1);
               wrap.append(style);
               current_style = style;
            }
         }
         else {
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

   static void print(lines& e_lines, lines& a_lines, int side_columns)
   {
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
                     e_last_style.c_str(), e_wrap.c_str(), h2_cfg::style("reset"), 
                     h2_cfg::style("dark gray"), j == K - 1 ? " " : "\\", h2_cfg::style("reset"),
                     a_last_style.c_str(), a_wrap.c_str(), h2_cfg::style("reset"), 
                     h2_cfg::style("dark gray"), j == K - 1 ? " " : "\\", h2_cfg::style("reset"));

            e_last_style = e_current_style;
            a_last_style = a_current_style;
         }
      }
   }

   static void diff_print(const char* expect, const char* actual, int terminal_columns)
   {
      node* e_node = parse(expect);
      node* a_node = parse(actual);

      dual* d = new dual(0, nullptr);
      __dual(e_node, a_node, d);

      frees(e_node);
      frees(a_node);

      list e_list, a_list;
      diff(d, e_list, a_list);
      frees(d);

      lines e_lines, a_lines;
      merge_line(e_list, e_lines);
      merge_line(a_list, a_lines);

      int e_most = lines_most(e_lines);
      int a_most = lines_most(a_lines);

      int fav_columns = std::max(std::max(e_most, a_most) + 3, 30);

      int side_columns = std::min(terminal_columns / 2 - 4, fav_columns);

      char t1[256], t2[256];
      ::printf("%s%s%s%s│%s%s%s%s\n",
               h2_cfg::style("dark gray"),
               h2_center_string("expect", side_columns, t1),
               h2_cfg::style("reset"),
               h2_cfg::style("dark gray"), h2_cfg::style("reset"),
               h2_cfg::style("dark gray"),
               h2_center_string("actual", side_columns, t2),
               h2_cfg::style("reset"));

      print(e_lines, a_lines, side_columns);
   }
};

#endif
