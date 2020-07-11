#include "../source/h2_unit.cpp"

SUITE(json lexical simple)
{
   Case(empty)
   {
      const char* json = "";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(0, lexical.size());
   }

   Cases({}, "{}", " { } ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("{", "}"), lexical);
   }

   Cases([], "[]", " [ ] ")
   {
      const char* json = x;
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("[", "]"), lexical);
   }

   Case(comma)
   {
      const char* json = ",";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf(","), lexical);
   }

   Case(colon)
   {
      const char* json = ":";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf(":"), lexical);
   }

   Case(double quote)
   {
      const char* json = "\"hello\"";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("\"hello\""), lexical);
   }

   Case(single quote)
   {
      const char* json = "'hello'";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("'hello'"), lexical);
   }

   Case(pattern)
   {
      const char* json = "/hello/";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("/hello/"), lexical);
   }

   Case(normal string)
   {
      const char* json = "hello ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("hello"), lexical);
   }

   Case(direct number)
   {
      const char* json = "1234 ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("1234"), lexical);
   }

   Case(math expression)
   {
      const char* json = " 1 +2 ";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("1 +2"), lexical);
   }
}

SUITE(json lexical escape)
{
   Case(double quote)
   {
      const char* json = "\"he\\\"llo\"";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("\"he\\\"llo\""), lexical);
   }

   Case(single quote)
   {
      const char* json = "'he\\\'llo'";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("'he\\\'llo'"), lexical);
   }

   Case(normal)
   {
      const char* json = "\\\nhe\\\"llo";
      h2::h2_vector<h2::h2_string> lexical;
      h2::h2_json_lexical::parse(lexical, json);
      OK(ListOf("\\\nhe\\\"llo"), lexical);
   }
}

CASE(json lexical complex)
{
   const char* json = "{\"a1\": true, 'a2' : hello world, a3: [abc, 123, null]}";
   h2::h2_vector<h2::h2_string> lexical;
   h2::h2_json_lexical::parse(lexical, json);
   OK(ListOf("{", "\"a1\"", ":", "true", ",", "'a2'", ":", "hello world", ",", "a3", ":", "[", "abc", ",", "123", ",", "null", "]", "}"), lexical);
}
