struct h2_json_dual : h2_libc {  // Combine two node into a dual
   int depth, relationship, index = INT_MAX;
   bool key_equal = false, value_match = false;
   bool e_last = true, a_last = true;
   int e_type = h2_json_node::t_absent, a_type = h2_json_node::t_absent;
   h2_string e_key, a_key;
   h2_string e_value, a_value;
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

   void __move_dual(h2_json_node* child_e, h2_json_node* child_a, bool caseless)
   {
      children.push_back((new h2_json_dual(child_e, child_a, caseless, depth + 1))->x);
      child_e->x.out();
      delete child_e;
      child_a->x.out();
      delete child_a;
   }

   void __mark_last()
   {
      int e_count = 0, a_count = 0;
      for (h2_list* p = children.prev; p != &children; p = p->prev) {
         h2_json_dual* d = h2_list_entry(p, h2_json_dual, x);
         if (d->e_type != h2_json_node::t_absent) d->e_last = e_count++ == 0;
         if (d->a_type != h2_json_node::t_absent) d->a_last = a_count++ == 0;
      }
   }

   h2_json_dual(h2_json_node* e, h2_json_node* a, bool caseless, int depth_ = 0, int relationship_ = 0) : depth(depth_), relationship(relationship_)
   {
      if (e) index = e->index;
      if (e) e->format(e_type, e_key, e_value, 2);
      if (a) a->format(a_type, a_key, a_value, 2);
      key_equal = e_key.equals(a_key, caseless);
      value_match = h2_json_match::match(e, a, caseless);

      if (e_type == h2_json_node::t_object || a_type == h2_json_node::t_object) {
         if (e && a) {
            h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
               h2_json_node* child_a = a->get(child_e->key_string, false);
               if (!child_a && caseless) child_a = a->get(child_e->key_string, true);
               if (child_a) __move_dual(child_e, child_a, caseless);
            }
            h2_list_for_each_entry (child_e, e->children, h2_json_node, x) {
               h2_json_node* child_a = h2_json_match::search(a->children, child_e, caseless);
               if (child_a) __move_dual(child_e, child_a, caseless);
            }
         }
         for (int i = 0; i < std::max(e ? e->size() : 0, a ? a->size() : 0); ++i) {
            children.push_back((new h2_json_dual(e ? e->get(i) : nullptr, a ? a->get(i) : nullptr, caseless, depth + 1, -1))->x);
         }
         children.sort(index_cmp);
      } else if (e_type == h2_json_node::t_array || a_type == h2_json_node::t_array) {
         for (int i = 0; i < std::max(e ? e->size() : 0, a ? a->size() : 0); ++i)
            children.push_back((new h2_json_dual(e ? e->get(i) : nullptr, a ? a->get(i) : nullptr, caseless, depth + 1))->x);
      }
      __mark_last();
   }

   const char* __e_style() { return a_type == h2_json_node::t_absent || relationship < 0 ? "light blue" : "green"; }
   const char* __a_style() { return e_type == h2_json_node::t_absent || relationship < 0 ? "light purple" : "red,bold"; }

   void align(h2_paragraph& e_paragraph, h2_paragraph& a_paragraph)
   {
      h2_paragraph e_ph, a_ph;
      h2_sentence e_sentence, a_sentence;
      e_sentence.indent(depth * 2);
      a_sentence.indent(depth * 2);

      if (e_type != h2_json_node::t_absent && e_key.size())
         e_sentence += color(e_key, key_equal ? "" : __e_style()) + ": ";
      if (a_type != h2_json_node::t_absent && a_key.size())
         a_sentence += color(a_key, key_equal ? "" : __a_style()) + ": ";

      if (e_type != h2_json_node::t_absent && e_value.size())
         e_sentence += color(e_value, value_match ? "" : __e_style());
      if (a_type != h2_json_node::t_absent && a_value.size())
         a_sentence += color(a_value, value_match ? "" : __a_style());

      if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) e_sentence.push_back(e_type == h2_json_node::t_object ? "{" : "[");
      if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) a_sentence.push_back(a_type == h2_json_node::t_object ? "{" : "[");

      h2_paragraph e_children_paragraph, a_children_paragraph;
      h2_list_for_each_entry (p, children, h2_json_dual, x)
         p->align(e_children_paragraph, a_children_paragraph);

      if ((O.fold_json >= 2 && key_equal && value_match) || (O.fold_json >= 3 && relationship < 0)) {
         e_sentence += e_children_paragraph.foldable() ? e_children_paragraph.folds() : gray(" ... ");
         a_sentence += a_children_paragraph.foldable() ? a_children_paragraph.folds() : gray(" ... ");
      } else if (O.fold_json && e_children_paragraph.foldable() && a_children_paragraph.foldable()) {
         e_sentence += e_children_paragraph.folds();
         a_sentence += a_children_paragraph.folds();
      } else {
         if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) {
            e_ph.push_back(e_sentence), e_sentence.clear();
            e_ph += e_children_paragraph;
            e_sentence.indent(depth * 2);
         }
         if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) {
            a_ph.push_back(a_sentence), a_sentence.clear();
            a_ph += a_children_paragraph;
            a_sentence.indent(depth * 2);
         }
      }
      if (e_type == h2_json_node::t_object || e_type == h2_json_node::t_array) e_sentence.push_back(e_type == h2_json_node::t_object ? "}" : "]");
      if (a_type == h2_json_node::t_object || a_type == h2_json_node::t_array) a_sentence.push_back(a_type == h2_json_node::t_object ? "}" : "]");

      if (e_type != h2_json_node::t_absent && !e_last) e_sentence.push_back(", ");
      if (a_type != h2_json_node::t_absent && !a_last) a_sentence.push_back(", ");
      e_ph.push_back(e_sentence);
      a_ph.push_back(a_sentence);
      h2_paragraph::samesizify(e_ph, a_ph);
      e_paragraph += e_ph;
      a_paragraph += a_ph;
   }
};
