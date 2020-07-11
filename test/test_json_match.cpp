#include "../source/h2_unit.cpp"

SUITE(json match)
{
   Case(match null)
   {
      h2::h2_json_tree e("null"), a("null");
      OK(h2::h2_json_match::match(&e, &a, false));
      OK(h2::h2_json_match::match(&e, &a, true));
   }

   Case(match bool)
   {
      h2::h2_json_tree true1("true"), true2(" true "), false1("false ");
      OK(h2::h2_json_match::match(&true1, &true2, false));
      OK(h2::h2_json_match::match(&true1, &true2, true));

      OK(!h2::h2_json_match::match(&true1, &false1, false));
      OK(!h2::h2_json_match::match(&true1, &false1, true));
   }

   Case(match number)
   {
      h2::h2_json_tree e1("0"), a1("1 - 1 * 1 / sqrt(1)");
      OK(h2::h2_json_match::match(&e1, &a1, false));
      OK(h2::h2_json_match::match(&e1, &a1, true));

      h2::h2_json_tree e2("-123.456"), a2("-123 - 0.456");
      OK(h2::h2_json_match::match(&e2, &a2, false));
      OK(h2::h2_json_match::match(&e2, &a2, true));
   }

   Case(match string)
   {
      h2::h2_json_tree e1("\"hello world\""), a1(" 'hello world' ");
      OK(h2::h2_json_match::match(&e1, &a1, false));
      OK(h2::h2_json_match::match(&e1, &a1, true));

      h2::h2_json_tree e2("\"hello world\""), a2(" 'Hello World' ");
      OK(!h2::h2_json_match::match(&e2, &a2, false));
      OK(h2::h2_json_match::match(&e2, &a2, true));
   }

   Case(match pattern)
   {
      h2::h2_json_tree e1("/.*/"), a1(" 'hello world' ");
      OK(h2::h2_json_match::match(&e1, &a1, false));
      OK(h2::h2_json_match::match(&e1, &a1, true));

      h2::h2_json_tree e2("/hello.*/world"), a2(" 'Hello World' ");
      OK(!h2::h2_json_match::match(&e2, &a2, false));
      OK(h2::h2_json_match::match(&e2, &a2, true));
   }

   Case(match array)
   {
      h2::h2_json_tree e1("[]"), a1(" [] ");
      OK(h2::h2_json_match::match(&e1, &a1, false));
      OK(h2::h2_json_match::match(&e1, &a1, true));

      h2::h2_json_tree e2("[1]"), a2(" [1-0] ");
      OK(h2::h2_json_match::match(&e2, &a2, false));
      OK(h2::h2_json_match::match(&e2, &a2, true));

      h2::h2_json_tree e3("[1, null, 'hello']"), a3(" [1, null, \"hello\"] ");
      OK(h2::h2_json_match::match(&e3, &a3, false));
      OK(h2::h2_json_match::match(&e3, &a3, true));
   }

   Case(match object)
   {
      h2::h2_json_tree e1("{}"), a1(" {} ");
      OK(h2::h2_json_match::match(&e1, &a1, false));
      OK(h2::h2_json_match::match(&e1, &a1, true));

      h2::h2_json_tree e2("{'a': 1}"), a2(" {\"a\": 1} ");
      OK(h2::h2_json_match::match(&e2, &a2, false));
      OK(h2::h2_json_match::match(&e2, &a2, true));

      h2::h2_json_tree e3("{'a': 1}"), a3(" {\"A\": 1, \"b\": false} ");
      OK(!h2::h2_json_match::match(&e3, &a3, false));
      OK(h2::h2_json_match::match(&e3, &a3, true));
   }

   Case(search)
   {
   }
}
