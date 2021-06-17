
struct h2_json_dual : h2_libc {  // combine two node into a dual
   bool key_equal = false, match = false;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   const char *e_class = "blob", *a_class = "blob";
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_rows e_blob, a_blob;
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
         if (e) e->print(e_blob, O.fold_json, false, depth);
         if (a) a->print(a_blob, O.fold_json, false, depth);
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

   void align(h2_rows& e_rows, h2_rows& a_rows, h2_list* subling = nullptr)
   {
      if (!strcmp(e_class, "blob")) {
         e_blob.samesizify(a_blob);
         for (auto& row : e_blob) row.brush("cyan");
         for (auto& row : a_blob) row.brush("yellow");

         e_rows += e_blob;
         a_rows += a_blob;
         return;
      }

      h2_row e_row, a_row;
      e_row.indent(depth * 2);
      a_row.indent(depth * 2);

      if (e_key.size()) {
         if (!key_equal) e_row.push_back("\033{green}");
         e_row.push_back(e_key);
         if (!key_equal) e_row.push_back("\033{reset}");
         e_row.push_back(": ");
      }

      if (a_key.size()) {
         if (!key_equal) a_row.push_back("\033{red,bold}");
         a_row.push_back(a_key);
         if (!key_equal) a_row.push_back("\033{reset}");
         a_row.push_back(": ");
      }

      if (!strcmp(e_class, "atomic")) {
         if (e_value.size()) {
            if (!match) e_row.push_back("\033{green}");
            e_row.push_back(e_value);
            if (!match) e_row.push_back("\033{reset}");
         }
         if (a_value.size()) {
            if (!match) a_row.push_back("\033{red,bold}");
            a_row.push_back(a_value);
            if (!match) a_row.push_back("\033{reset}");
         }
      } else if (!strcmp(e_class, "object") || !strcmp(e_class, "array")) {
         h2_rows e_children_rows, a_children_rows;
         h2_list_for_each_entry (p, children, h2_json_dual, x)
            p->align(e_children_rows, a_children_rows, &children);

         e_row.push_back(strcmp(e_class, "object") ? "[" : "{");
         a_row.push_back(strcmp(a_class, "object") ? "[" : "{");
         if (O.fold_json && e_children_rows.foldable() && a_children_rows.foldable()) {
            e_row += e_children_rows.folds();
            a_row += a_children_rows.folds();
         } else {
            e_rows.push_back(e_row), e_row.clear();
            e_rows += e_children_rows;
            e_row.indent(depth * 2);
            a_rows.push_back(a_row), a_row.clear();
            a_rows += a_children_rows;
            a_row.indent(depth * 2);
         }
         e_row.push_back(strcmp(e_class, "object") ? "]" : "}");
         a_row.push_back(strcmp(a_class, "object") ? "]" : "}");
      }
      if (e_row.size()) {
         if (has_next(subling, true)) e_row.push_back(", ");
         e_rows.push_back(e_row), e_row.clear();
      }
      if (a_row.size()) {
         if (has_next(subling, false)) a_row.push_back(", ");
         a_rows.push_back(a_row), a_row.clear();
      }
   }
};
