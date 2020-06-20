#include "../source/h2_unit.cpp"

SUITE(json node dual)
{
   Case(simple plain)
   {
      const char* json = "{\"abc\": 123}";
      h2::h2_json_parse parse(json);

      int type;
      const char* cls;
      h2::h2_string key;
      h2::h2_string value;

      parse.root_node.dual(type, cls, key, value);
      OK(h2::h2_json_node::t_object, type);
      OK("", key);
      OK("object", cls);
      OK("", value);

      parse.root_node.get(0)->dual(type, cls, key, value);
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
      h2::h2_json_parse parse(json);

      h2::h2_lines lines;
      parse.root_node.print(lines, 0, 0, false);

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
      h2::h2_json_parse parse(json);

      h2::h2_lines lines;
      parse.root_node.print(lines, 0, 0, true);

      OK(1, lines.size());
      OK(ListOf(ListOf("", "{ ", "\"abc\": ", "123", " }")), lines);
   }
}
