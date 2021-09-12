struct h2_json_match {
   static bool match_array(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      if (e->size() != a->size()) return false;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         if (!match(p, a->get(i), caseless))
            return false;
      return true;
   }

   static bool match_object(h2_json_node* e, h2_json_node* a, bool caseless)
   {
      if (!e || !a) return false;
      if (e->size() > a->size()) return false;
      h2_list_for_each_entry (p, e->children, h2_json_node, x)
         if (!match(p, a->get(p->key_string, caseless), caseless))
            return false;
      return true;
   }

   static bool match(h2_json_node* e, h2_json_node* a, bool caseless)
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
         return a->is_string() && e->value_string.equals(a->value_string, caseless);
      case h2_json_node::t_pattern:
         return a->is_string() && h2_pattern::match(e->value_string.c_str(), a->value_string.c_str(), caseless);
      case h2_json_node::t_array:
         return a->is_array() && match_array(e, a, caseless);
      case h2_json_node::t_object:
         return a->is_object() && match_object(e, a, caseless);
      default: return false;
      }
   }

   static bool likes_array(h2_json_node* e, h2_json_node* a)
   {
      double score = 0.0, sub_score = 0.0;
      if (e->size() == a->size()) score += 0.3;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         sub_score += (sub_score * i + likes(p, a->get(i))) / (i + 1);
      return score + 0.7 * sub_score;
   }
   static bool likes_object(h2_json_node* e, h2_json_node* a)
   {
      double score = 0.0, sub_score = 0.0;
      if (e->size() == a->size()) score += 0.3;
      h2_list_for_each_entry (p, i, e->children, h2_json_node, x)
         sub_score += (sub_score * i + likes(p, a->get(p->key_string, false))) / (i + 1);
      return score + 0.7 * sub_score;
   }

   static double likes(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return 0;

      double score = 0.0;
      if (e->type == h2_json_node::t_array && a->type == h2_json_node::t_array) {
         score += likes_array(e, a);
      } else if (e->type == h2_json_node::t_object && a->type == h2_json_node::t_object) {
         score += likes_object(e, a);
      } else if (e->type == a->type) {
         score += h2_fuzzy::likes(e->value_string.c_str(), a->value_string.c_str());
      } else {
      }
      if (e->key_string.size() || a->key_string.size()) {
         score = score * 0.5 + 0.5 * h2_fuzzy::likes(e->key_string.c_str(), a->key_string.c_str());
      }
      return score;
   }

   static h2_json_node* search(const h2_list& haystack, h2_json_node* needle)
   {
      double max_score = 0.0;
      h2_json_node* straw = nullptr;
      h2_list_for_each_entry (p, haystack, h2_json_node, x) {
         double score = likes(needle, p);
         if (score > max_score) {
            max_score = score;
            straw = p;
         }
      }
      return straw;
   }
};
