#include "../source/h2_unit.cpp"

static int __node_tojson(h2::h2_json_node* node, char* b)
{
   int l = 0;

   if (node->key_string.size()) {
      l += sprintf(b + l, "\"%s\":", node->key_string.c_str());
   }
   if (node->is_null()) {
      l += sprintf(b + l, "null");
   }
   if (node->is_bool()) {
      l += sprintf(b + l, "%s", node->value_boolean ? "true" : "false");
   }
   if (node->is_number()) {
      l += sprintf(b + l, "%.15g", node->value_double);
   }
   if (node->is_string()) {
      l += sprintf(b + l, "\"%s\"", node->value_string.c_str());
   }
   if (node->is_pattern()) {
      l += sprintf(b + l, "\"/%s/\"", node->value_string.c_str());
   }
   if (node->is_array()) {
      l += sprintf(b + l, "[");
      h2_list_for_each_entry (p, node->children, h2::h2_json_node, x) {
         if (li)
            l += sprintf(b + l, ",");
         l += __node_tojson(p, b + l);
      }
      l += sprintf(b + l, "]");
   }
   if (node->is_object()) {
      l += sprintf(b + l, "{");
      h2_list_for_each_entry (p, node->children, h2::h2_json_node, x) {
         if (li)
            l += sprintf(b + l, ",");
         l += __node_tojson(p, b + l);
      }
      l += sprintf(b + l, "}");
   }

   return l;
}

char* node_tojson(h2::h2_json_node* node, char* b)
{
   __node_tojson(node, b);
   return b;
}

static const char* node_type_tostring(const int type)
{
   switch (type) {
   case h2::h2_json_node::t_absent: return "absent";
   case h2::h2_json_node::t_null: return "null";
   case h2::h2_json_node::t_boolean: return "boolean";
   case h2::h2_json_node::t_number: return "number";
   case h2::h2_json_node::t_string: return "string";
   case h2::h2_json_node::t_pattern: return "pattern";
   case h2::h2_json_node::t_array: return "array";
   case h2::h2_json_node::t_object: return "object";
   }
   return "?";
}

SUITE(json node get)
{
   char t2[1024 * 128];

   Case(array)
   {
      const char* week = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";

      h2::h2_json_tree c1(week);

      OK(c1.is_array());
      OK(0, c1.key_string.size());
      OK(7, c1.size());

      h2::h2_json_node* d[7];
      d[0] = c1.get(0);
      OK("Sunday", d[0]->value_string);

      d[1] = c1.get(1);
      OK("Monday", d[1]->value_string);

      d[2] = c1.get(2);
      OK("Tuesday", d[2]->value_string);

      d[3] = c1.get(3);
      OK("Wednesday", d[3]->value_string);

      d[4] = c1.get(4);
      OK("Thursday", d[4]->value_string);

      d[5] = c1.get(5);
      OK("Friday", d[5]->value_string);

      d[6] = c1.get(6);
      OK("Saturday", d[6]->value_string);

      JE(week, node_tojson(&c1, t2));

      OK(IsNull, c1.get("ling", false));

      for (int i = 0; i < c1.size(); i++) {
         OK(d[i]->value_string, c1.get(i)->value_string);
      }
   }

   Case(object)
   {
      const char* obj = "{                                              \
        \"data\": 'Click Here',                                         \
        'size': 36,                                                     \
        \"alignment\": true,                                            \
        \"bold\": false,                                                \
        \"token\": null,                                                \
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"    \
      }";

      h2::h2_json_tree c(obj);

      OK(c.is_object());
      OK(6, c.size());

      h2::h2_json_node* c0 = c.get(0);
      OK(c0->is_string());
      OK("data", c0->key_string);
      OK("Click Here", c0->value_string);

      h2::h2_json_node* c1 = c.get(1);
      OK(c1->is_number());
      OK("size", c1->key_string);
      OK(36, c1->value_double);

      h2::h2_json_node* c2 = c.get(2);
      OK(c2->is_bool());
      OK("alignment", c2->key_string);
      OK(c2->value_boolean);

      h2::h2_json_node* c3 = c.get(3);
      OK(c3->is_bool());
      OK("bold", c3->key_string);
      OK(!c3->value_boolean);

      h2::h2_json_node* c4 = c.get(4);
      OK(c4->is_null());
      OK("token", c4->key_string);

      h2::h2_json_node* c5 = c.get(5);
      OK(c5->is_string());
      OK("onMouseUp", c5->key_string);
      OK("sun1.opacity = (sun1.opacity / 100) * 90;", c5->value_string);

      JE(obj, node_tojson(&c, t2));
   }
}

SUITE(json node dual)
{
   Case(simple plain)
   {
      const char* json = "{\"abc\": 123}";
      h2::h2_json_tree parse(json);

      int type;
      const char* cls;
      h2::h2_string key;
      h2::h2_string value;

      parse.dual(type, cls, key, value);
      OK(h2::h2_json_node::t_object, type);
      OK("", key);
      OK("object", cls);
      OK("", value);

      parse.get(0)->dual(type, cls, key, value);
      OK(h2::h2_json_node::t_number, type);
      OK("\"abc\"", key);
      OK("atomic", cls);
      OK("123", value);
   }
}

SUITE(json node print)
{
   Case(print simple plain)
   {
      const char* json = "{\"abc\": 123}";
      h2::h2_json_tree parse(json);

      h2::h2_lines lines;
      parse.print(lines, 0, 0, false);

      OK(3, lines.size());
      OK(ListOf(
           ListOf("", "{ "),
           ListOf("  ", "\"abc\": ", "123"),
           ListOf("", "}")),
         lines);
   }

   Case(print simple fold)
   {
      const char* json = "{\"abc\": 123}";
      h2::h2_json_tree parse(json);

      h2::h2_lines lines;
      parse.print(lines, 0, 0, true);

      OK(1, lines.size());
      OK(ListOf(ListOf("", "{ ", "\"abc\": ", "123", " }")), lines);
   }
}
