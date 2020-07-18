#include "../source/h2_unit.cpp"

SUITE(kit)
{
   Case(H2Q generate unique symbol)
   {
      int H2Q() = 1;
      int H2Q() = 1;
      int H2Q(prefix) = 1;
      int H2Q(prefix) = 1;
   }

   Case(wildcard)
   {
      OK(!h2::h2_pattern::wildcard_match("hello?world*", "welcome"));
      OK(h2::h2_pattern::wildcard_match("hello?world*", "hello world"));
      OK(h2::h2_pattern::wildcard_match("hello?world*", "hello world h2unit"));
      OK(!h2::h2_pattern::wildcard_match("hello?world*", "one world"));
      OK(!h2::h2_pattern::wildcard_match("hello?world*", ""));
   }

   Case(wildcard caseless)
   {
      OK(h2::h2_pattern::wildcard_match("hello?world*", "Hello World", true));
   }

   Case(regex)
   {
      const char* subject = "abcdef";
      OK(h2::h2_pattern::regex_match("a.*", subject));
      OK(h2::h2_pattern::regex_match("A.*", subject, true));
   }

   Case(once)
   {
      h2::h2_once once;
      OK(once);
      OK(!once);
      OK(!once);
   }
}

SUITE(acronym)
{
   Case(acronym)
   {
      h2::h2_string s1("0123456789");
      OK(ListOf("", "\033{+dark gray}", "...", "\033{-dark gray}", ""), h2_acronym(s1, 3));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", ""), h2_acronym(s1, 4));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", ""), h2_acronym(s1, 5));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", ""), h2_acronym(s1, 6));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", ""), h2_acronym(s1, 7));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}", ""), h2_acronym(s1, 8));
      OK(ListOf("012345", "\033{+dark gray}", "...", "\033{-dark gray}", ""), h2_acronym(s1, 9));
      OK(ListOf("0123456789"), h2_acronym(s1, 10));
      OK(ListOf("0123456789"), h2_acronym(s1, 11));
   }

   Case(acronym with tail 1)
   {
      h2::h2_string s1("0123456789");
      OK(ListOf("", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), h2_acronym(s1, 4, 1));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), h2_acronym(s1, 5, 1));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), h2_acronym(s1, 6, 1));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), h2_acronym(s1, 7, 1));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), h2_acronym(s1, 8, 1));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), h2_acronym(s1, 9, 1));
      OK(ListOf("0123456789"), h2_acronym(s1, 10, 1));
      OK(ListOf("0123456789"), h2_acronym(s1, 11, 1));
   }

   Case(acronym with tail 2)
   {
      h2::h2_string s1("0123456789");
      OK(ListOf("", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), h2_acronym(s1, 5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), h2_acronym(s1, 6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), h2_acronym(s1, 7, 2));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), h2_acronym(s1, 8, 2));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), h2_acronym(s1, 9, 2));
      OK(ListOf("0123456789"), h2_acronym(s1, 10, 2));
      OK(ListOf("0123456789"), h2_acronym(s1, 11, 2));
   }

   Case(acronym with CRLF)
   {
      h2::h2_string s1("012\r34\t\n");
      OK(ListOf("012\\r34\\t\\n"), h2_acronym(s1, 10));
   }
}
