struct h2_json_dual : h2_libc {  // combine two node into a dual
   h2_json_dual* perent;
   int relationship, depth, index = INT_MAX;
   bool key_equal = false, value_match = false;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   h2_string e_key, a_key;
   h2_string e_value, a_value;
   h2_paragraph e_blob, a_blob;
   h2_list children, x;

   static int index_cmp(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_json_dual, x)->index - h2_list_entry(b, h2_json_dual, x)->index;
   }

   ~h2_json_dual()
   {
      h2_list_for_each_entry (p, children, h2_json_dual, x) {
         p->x.out();
         delete p;
      }
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, bool caseless, h2_json_dual* perent_ = nullptr, int relationship_ = 0) : perent(perent_), relationship(relationship_), depth(perent_ ? perent_->depth + 1 : 0)
   {
      if (e) index = e->index;
      if (e) e->format(e_type, e_key, e_value, 2);
      if (a) a->format(a_type, a_key, a_value, 2);
      key_equal = e_key.equals(a_key, caseless);
      value_match = h2_json_match::match(e, a, caseless);

      if ((e_type & 0xf0) != (a_type & 0xf0)) {
         bool acronym = O.fold_json >= 3 && (!(e && a) /* 1 side */ || relationship < 0);
         if (e) e_blob = e->format(O.fold_json, acronym, 2, depth);
         if (a) a_blob = a->format(O.fold_json, acronym, 2, depth);
      } else if (e_type == h2_json_node::t_object) {
         h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
            h2_json_node* child_a = a->get(child_e->key_string, false);
            if (!child_a && caseless) child_a = a->get(child_e->key_string, true);
            if (child_a) move_dual(child_e, child_a, caseless);
         }
         h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
            h2_json_node* child_a = h2_json_match::search(a->children, child_e);
            if (child_a) move_dual(child_e, child_a, caseless);
         }
         for (int i = 0; i < std::max(e->size(), a->size()); ++i)
            children.push_back((new h2_json_dual(e->get(i), a->get(i), caseless, this, -1))->x);
         children.sort(index_cmp);
      } else if (e_type == h2_json_node::t_array) {
         for (int i = 0; i < std::max(e->size(), a->size()); ++i)
            children.push_back((new h2_json_dual(e->get(i), a->get(i), caseless, this))->x);
      }
   }

   void move_dual(h2_json_node* child_e, h2_json_node* child_a, bool caseless)
   {
      children.push_back((new h2_json_dual(child_e, child_a, caseless, this))->x);
      child_e->x.out();
      delete child_e;
      child_a->x.out();
      delete child_a;
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

   void align(h2_paragraph& e_paragraph, h2_paragraph& a_paragraph, h2_list* subling = nullptr)
   {
      if (e_blob.size() || a_blob.size()) {
         e_blob.samesizify(a_blob);
         for (auto& st : e_blob) st.brush("cyan");
         for (auto& st : a_blob) st.brush("yellow");

         e_paragraph += e_blob;
         a_paragraph += a_blob;
         return;
      }

      h2_sentence e_sentence, a_sentence;
      e_sentence.indent(depth * 2);
      a_sentence.indent(depth * 2);

      if (e_key.size()) {
         if (!key_equal) e_sentence.push_back("\033{green}");
         e_sentence.push_back(e_key);
         if (!key_equal) e_sentence.push_back("\033{reset}");
         e_sentence.push_back(": ");
      }

      if (a_key.size()) {
         if (!key_equal) a_sentence.push_back("\033{red,bold}");
         a_sentence.push_back(a_key);
         if (!key_equal) a_sentence.push_back("\033{reset}");
         a_sentence.push_back(": ");
      }

      if ((e_type & 0xf0) == 0x10) {  // null, boolean, number, string, pattern
         if (e_value.size()) {
            if (!value_match) e_sentence.push_back("\033{green}");
            e_sentence.push_back(e_value);
            if (!value_match) e_sentence.push_back("\033{reset}");
         }
         if (a_value.size()) {
            if (!value_match) a_sentence.push_back("\033{red,bold}");
            a_sentence.push_back(a_value);
            if (!value_match) a_sentence.push_back("\033{reset}");
         }
      } else if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) {
         h2_paragraph e_children_paragraph, a_children_paragraph;
         h2_list_for_each_entry (p, children, h2_json_dual, x)
            p->align(e_children_paragraph, a_children_paragraph, &children);

         e_sentence.push_back(e_type == h2_json_node::t_object ? "{" : "[");
         a_sentence.push_back(a_type == h2_json_node::t_object ? "{" : "[");
         if (((O.fold_json >= 2 && value_match) || (O.fold_json >= 3 && relationship < 0)) && (!e_children_paragraph.foldable() || !a_children_paragraph.foldable())) {
            e_sentence += e_children_paragraph.foldable() ? e_children_paragraph.folds() : gray(" ... ");
            a_sentence += a_children_paragraph.foldable() ? a_children_paragraph.folds() : gray(" ... ");
         } else if (O.fold_json && e_children_paragraph.foldable() && a_children_paragraph.foldable()) {
            e_sentence += e_children_paragraph.folds();
            a_sentence += a_children_paragraph.folds();
         } else {
            e_paragraph.push_back(e_sentence), e_sentence.clear();
            e_paragraph += e_children_paragraph;
            e_sentence.indent(depth * 2);
            a_paragraph.push_back(a_sentence), a_sentence.clear();
            a_paragraph += a_children_paragraph;
            a_sentence.indent(depth * 2);
         }
         e_sentence.push_back(e_type == h2_json_node::t_object ? "}" : "]");
         a_sentence.push_back(a_type == h2_json_node::t_object ? "}" : "]");
      }
      if (e_sentence.size()) {
         if (has_next(subling, true)) e_sentence.push_back(", ");
         e_paragraph.push_back(e_sentence), e_sentence.clear();
      }
      if (a_sentence.size()) {
         if (has_next(subling, false)) a_sentence.push_back(", ");
         a_paragraph.push_back(a_sentence), a_sentence.clear();
      }
   }
};
