
struct h2_json_match {
   static bool match_array(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return false;
      if (e->size() != a->size()) return false;
      h2_list_for_each_entry (p, e->children, h2_json_node, x)
         if (!match(p, a->get(li)))
            return false;
      return true;
   }

   static bool match_object(h2_json_node* e, h2_json_node* a)
   {
      if (!e || !a) return false;
      if (e->size() > a->size()) return false;
      h2_list_for_each_entry (p, e->children, h2_json_node, x)
         if (!match(p, a->get(p->key_string.c_str())))
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
         return a->is_string() && h2_pattern::regex_match(e->value_string.c_str(), a->value_string.c_str());
      case h2_json_node::t_array:
         return a->is_array() && match_array(e, a);
      case h2_json_node::t_object:
         return a->is_object() && match_object(e, a);
      default: return false;
      }
   }

   static h2_json_node* search(h2_list& haystack, h2_json_node* needle)
   {
      h2_list_for_each_entry (p, haystack, h2_json_node, x)
         if (match(needle, p))
            return p;
      return nullptr;
   }
};
