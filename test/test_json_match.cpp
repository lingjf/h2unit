#include "../source/h2_unit.cpp"

SUITE(json match)
{
   Case(match null)
   {
      OK(h2::h2_json::match("null", " null "));
   }

   Case(match bool)
   {
      OK(h2::h2_json::match("true", " true "));
      OK(h2::h2_json::match("false", " false "));
      OK(!h2::h2_json::match("true", " false "));
   }

   Case(match number)
   {
      OK(h2::h2_json::match("0", "1 - 1 * 1 / sqrt(1)"));
      OK(h2::h2_json::match("-123.456", "-123 - 0.456"));
   }

   Case(match string)
   {
      OK(h2::h2_json::match("\"hello world\"", " 'hello world' "));
   }

   Case(match regexp) { OK(h2::h2_json::match("/.*/", " 'hello world' ")); };

   Case(match array)
   {
      OK(h2::h2_json::match("[]", " [] "));
      OK(h2::h2_json::match("[1]", " [1-0] "));
      OK(h2::h2_json::match("[1, null, 'hello']", " [1, null, \"hello\"] "));
   }

   Case(match object)
   {
      OK(h2::h2_json::match("{}", " {} "));
      OK(h2::h2_json::match("{'a': 1}", " {\"a\": 1} "));
      OK(h2::h2_json::match("{'a': 1}", " {\"a\": 1, \"b\": false} "));
   }

   Case(search)
   {
   }
}
