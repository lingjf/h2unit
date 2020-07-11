#include "../source/h2_unit.cpp"

SUITE(json match)
{
   Case(match null)
   {
      h2::h2_json_parse e("null"), a("null");
      OK(h2::h2_json_match::match(&e.root_node, &a.root_node, false));
      OK(h2::h2_json_match::match(&e.root_node, &a.root_node, true));
   }

   Case(match bool)
   {
      h2::h2_json_parse true1("true"), true2(" true "), false1("false ");
      OK(h2::h2_json_match::match(&true1.root_node, &true2.root_node, false));
      OK(h2::h2_json_match::match(&true1.root_node, &true2.root_node, true));

      OK(!h2::h2_json_match::match(&true1.root_node, &false1.root_node, false));
      OK(!h2::h2_json_match::match(&true1.root_node, &false1.root_node, true));
   }

   Case(match number)
   {
      h2::h2_json_parse e1("0"), a1("1 - 1 * 1 / sqrt(1)");
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, false));
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, true));

      h2::h2_json_parse e2("-123.456"), a2("-123 - 0.456");
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, false));
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, true));
   }

   Case(match string)
   {
      h2::h2_json_parse e1("\"hello world\""), a1(" 'hello world' ");
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, false));
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, true));

      h2::h2_json_parse e2("\"hello world\""), a2(" 'Hello World' ");
      OK(!h2::h2_json_match::match(&e2.root_node, &a2.root_node, false));
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, true));
   }

   Case(match pattern)
   {
      h2::h2_json_parse e1("/.*/"), a1(" 'hello world' ");
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, false));
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, true));

      h2::h2_json_parse e2("/hello.*/world"), a2(" 'Hello World' ");
      OK(!h2::h2_json_match::match(&e2.root_node, &a2.root_node, false));
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, true));
   }

   Case(match array)
   {
      h2::h2_json_parse e1("[]"), a1(" [] ");
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, false));
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, true));

      h2::h2_json_parse e2("[1]"), a2(" [1-0] ");
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, false));
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, true));

      h2::h2_json_parse e3("[1, null, 'hello']"), a3(" [1, null, \"hello\"] ");
      OK(h2::h2_json_match::match(&e3.root_node, &a3.root_node, false));
      OK(h2::h2_json_match::match(&e3.root_node, &a3.root_node, true));
   }

   Case(match object)
   {
      h2::h2_json_parse e1("{}"), a1(" {} ");
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, false));
      OK(h2::h2_json_match::match(&e1.root_node, &a1.root_node, true));

      h2::h2_json_parse e2("{'a': 1}"), a2(" {\"a\": 1} ");
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, false));
      OK(h2::h2_json_match::match(&e2.root_node, &a2.root_node, true));

      h2::h2_json_parse e3("{'a': 1}"), a3(" {\"A\": 1, \"b\": false} ");
      OK(!h2::h2_json_match::match(&e3.root_node, &a3.root_node, false));
      OK(h2::h2_json_match::match(&e3.root_node, &a3.root_node, true));
   }

   Case(search)
   {
   }
}
