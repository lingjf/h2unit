
struct h2_json_dual : h2_libc {  // combine two node into a dual
   bool key_equal = false, match = false;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   const char *e_class = "blob", *a_class = "blob";
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_lines e_blob, a_blob;
   h2_list children, x;
   h2_json_dual* perent;
   int depth;

   ~h2_json_dual()
   {
      h2_list_for_each_entry (p, children, h2_json_dual, x) {
         p->x.out();
         delete p;
      }
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, bool caseless, h2_json_dual* perent_ = nullptr) : perent(perent_), depth(perent_ ? perent_->depth + 1 : 0)
   {
      match = h2_json_match::match(e, a, caseless);
      if (e) e->dual(e_type, e_class, e_key, e_value);
      if (a) a->dual(a_type, a_class, a_key, a_value);
      key_equal = e_key.equals(a_key, caseless);

      if (strcmp(e_class, a_class)) {
         if (e) e->print(e_blob, O.fold, false, depth);
         if (a) a->print(a_blob, O.fold, false, depth);
         e_class = a_class = "blob";
      } else if (!strcmp("object", e_class)) {
         h2_list_for_each_entry (_e, e->children, h2_json_node, x) {
            h2_json_node* _a = a->get(_e->key_string, false);
            if (!_a) _a = a->get(_e->key_string, caseless);
            if (!_a) _a = h2_json_match::search(a->children, _e, caseless);
            if (_a) {
               children.push_back((new h2_json_dual(_e, _a, caseless, this))->x);
               _e->x.out();
               delete _e;
               _a->x.out();
               delete _a;
            }
         }

         for (int i = 0; i < std::max(e->size(), a->size()); ++i)
            children.push_back((new h2_json_dual(e->get(i), a->get(i), caseless, this))->x);
      } else if (!strcmp("array", e_class)) {
         for (int i = 0; i < std::max(e->size(), a->size()); ++i)
            children.push_back((new h2_json_dual(e->get(i), a->get(i), caseless, this))->x);
      }
   }

   bool has_next(h2_list* subling, bool expect) const
   {
      if (subling) {
         for (h2_list* p = x.next; p != subling; p = p->next) {
            h2_json_dual* d = h2_list_entry(p, h2_json_dual, x);
            if ((expect ? d->e_type : d->a_type) != h2_json_node::t_absent)
               return true;
         }
      }
      return false;
   }

   void align(h2_lines& e_lines, h2_lines& a_lines, h2_list* subling = nullptr)
   {
      if (!strcmp(e_class, "blob")) {
         e_blob.samesizify(a_blob);
         for (auto& line : e_blob) line.brush("cyan");
         for (auto& line : a_blob) line.brush("yellow");

         e_lines += e_blob;
         a_lines += a_blob;
         return;
      }

      h2_line e_line, a_line;
      e_line.indent(depth * 2);
      a_line.indent(depth * 2);

      if (e_key.size()) {
         if (!key_equal) e_line.push_back("\033{green}");
         e_line.push_back(e_key);
         if (!key_equal) e_line.push_back("\033{reset}");
         e_line.push_back(": ");
      }

      if (a_key.size()) {
         if (!key_equal) a_line.push_back("\033{red,bold}");
         a_line.push_back(a_key);
         if (!key_equal) a_line.push_back("\033{reset}");
         a_line.push_back(": ");
      }

      if (!strcmp(e_class, "atomic")) {
         if (e_value.size()) {
            if (!match) e_line.push_back("\033{green}");
            e_line.push_back(e_value);
            if (!match) e_line.push_back("\033{reset}");
         }
         if (a_value.size()) {
            if (!match) a_line.push_back("\033{red,bold}");
            a_line.push_back(a_value);
            if (!match) a_line.push_back("\033{reset}");
         }
      } else if (!strcmp(e_class, "object") || !strcmp(e_class, "array")) {
         h2_lines e_children_lines, a_children_lines;
         h2_list_for_each_entry (p, children, h2_json_dual, x)
            p->align(e_children_lines, a_children_lines, &children);

         e_line.push_back(strcmp(e_class, "object") ? "[" : "{");
         a_line.push_back(strcmp(a_class, "object") ? "[" : "{");
         if (O.fold && e_children_lines.foldable() && a_children_lines.foldable()) {
            e_line.concat_back(e_children_lines.folds());
            a_line.concat_back(a_children_lines.folds());
         } else {
            e_lines.push_back(e_line), e_line.clear();
            e_lines += e_children_lines;
            e_line.indent(depth * 2);
            a_lines.push_back(a_line), a_line.clear();
            a_lines += a_children_lines;
            a_line.indent(depth * 2);
         }
         e_line.push_back(strcmp(e_class, "object") ? "]" : "}");
         a_line.push_back(strcmp(a_class, "object") ? "]" : "}");
      }
      if (e_line.size()) {
         if (has_next(subling, true)) e_line.push_back(", ");
         e_lines.push_back(e_line), e_line.clear();
      }
      if (a_line.size()) {
         if (has_next(subling, false)) a_line.push_back(", ");
         a_lines.push_back(a_line), a_line.clear();
      }
   }
};
