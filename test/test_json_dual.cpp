#include "../source/h2_unit.cpp"

static int __dual_tostr(h2::h2_json_dual* dual, char* b)
{
   int l = 0;

   l += sprintf(b + l, "(%d;%s-%s;%s-%s;%s-%s)", dual->depth,
                dual->e_class, dual->a_class,
                dual->e_key.c_str(), dual->a_key.c_str(),
                dual->e_value.c_str(), dual->a_value.c_str());

   if (dual->children.count()) {
      l += sprintf(b + l, "[");
      h2_list_for_each_entry (p, dual->children, h2::h2_json_dual, x) {
         l += __dual_tostr(p, b + l);
      }
      l += sprintf(b + l, "]");
   }
   return l;
}

char* dual_tostr(h2::h2_json_dual* dual, char* b)
{
   __dual_tostr(dual, b);
   return b;
}

SUITE(json dual)
{
   char t[1024 * 8];

   Case(object only)
   {
      const char* e = "{\"e\": 123}";
      const char* a = "{\"a\": 456}";
      h2::h2_json_parse e_parse(e);
      h2::h2_json_parse a_parse(a);

      h2::h2_json_dual dual(&e_parse.root_node, &a_parse.root_node, false);

      OK("(0;object-object;-;-)[(1;atomic-atomic;\"e\"-\"a\";123-456)]", dual_tostr(&dual, t));

      h2::h2_lines e_lines, a_lines;
      dual.align(e_lines, a_lines);
   }
}
