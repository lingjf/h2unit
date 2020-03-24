#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

SUITE(string)
{
   Case(equals) 
   {
      h2::h2_string a = "Hello World";
      h2::h2_string b1 = "Hello World";
      h2::h2_string b2 = "hello world";
      h2::h2_string b3 = "Hello_World";
      std::string c1 = "Hello World";
      std::string c2 = "hello world";
      std::string c3 = "Hello_World";

      OK(a.equals("Hello World"));
      OK(a.equals("hello world", true));
      OK(!a.equals("Hello_World"));
      OK(a.equals(b1));
      OK(a.equals(b2, true));
      OK(!a.equals(b3));
      OK(a.equals(c1));
      OK(a.equals(c2, true));
      OK(!a.equals(c3));
   }

   Case(contains) 
   {
      h2::h2_string a = "Hello World";
      h2::h2_string b1 = "Hello";
      h2::h2_string b2 = "world";
      h2::h2_string b3 = "Hill";
      std::string c1 = "Hello";
      std::string c2 = "world";
      std::string c3 = "Hill";

      OK(a.contains("Hello"));
      OK(a.contains("world", true));
      OK(!a.contains("Hill"));
      OK(a.contains(b1));
      OK(a.contains(b2, true));
      OK(!a.contains(b3));
      OK(a.contains(c1));
      OK(a.contains(c2, true));
      OK(!a.contains(c3));
   }

   Case(startswith) 
   {
      h2::h2_string a = "Hello World";
      h2::h2_string b1 = "Hello";
      h2::h2_string b2 = "hello";
      h2::h2_string b3 = "Hill";
      std::string c1 = "Hello";
      std::string c2 = "hello";
      std::string c3 = "Hill";

      OK(a.startswith("Hello"));
      OK(a.startswith("hello", true));
      OK(!a.startswith("Hill"));
      OK(a.startswith(b1));
      OK(a.startswith(b2, true));
      OK(!a.startswith(b3));
      OK(a.startswith(c1));
      OK(a.startswith(c2, true));
      OK(!a.startswith(c3));
   }

   Case(endswith) 
   {
      h2::h2_string a = "Hello World";
      h2::h2_string b1 = "World";
      h2::h2_string b2 = "world";
      h2::h2_string b3 = "Hill";
      std::string c1 = "World";
      std::string c2 = "world";
      std::string c3 = "Hill";

      OK(a.endswith("World"));
      OK(a.endswith("World", true));
      OK(!a.endswith("Hill"));
      OK(a.endswith(b1));
      OK(a.endswith(b2, true));
      OK(!a.endswith(b3));
      OK(a.endswith(c1));
      OK(a.endswith(c2, true));
      OK(!a.endswith(c3));
   }

   Case(sprintf) 
   {
      h2::h2_string a;
      a.sprintf("%s %s", "Hello", "World");
      OK(a.equals("Hello World"));
   }

   Case(acronym)
   {
      h2::h2_string s1("01234");
      OK("01234", s1.acronym(10));
      h2::h2_string s2("0123456789ABCDEF");
      OK(AllOf(StartsWith("0123456789"), Contains("...")), s2.acronym(10));
   }

   Case(center)
   {
      OK("ab", h2::h2_string("ab").center(2));
      OK("ab ", h2::h2_string("ab").center(3));
      OK(" ab ", h2::h2_string("ab").center(4));
      OK(" ab  ", h2::h2_string("ab").center(5));
      OK("  ab  ", h2::h2_string("ab").center(6));

      OK("abc", h2::h2_string("abc").center(3));
      OK("abc ", h2::h2_string("abc").center(4));
      OK(" abc ", h2::h2_string("abc").center(5));
      OK(" abc  ", h2::h2_string("abc").center(6));
   }

   Case(concat) 
   {
      h2::h2_string t;
      h2::h2_string a = "Hello";
      h2::h2_string b = "World";

      std::string c = "Hello";
      std::string d = "World";

      t = a + b;
      OK(t.equals("HelloWorld"));

      t = a + "World";
      OK(t.equals("HelloWorld"));

      t = "Hello" + b;
      OK(t.equals("HelloWorld"));

      t = a + d;
      OK(t.equals("HelloWorld"));

      t = c + b;
      OK(t.equals("HelloWorld"));

      t = a + 'W';
      OK(t.equals("HelloW"));

      t = 'H' + b;
      OK(t.equals("HWorld"));
   }

   Case(append)
   {
      h2::h2_string t;
      t.append("hello");
      OK(t.equals("hello"));

      // using h2_string as byte buffer
      t.append("\0world\0", 7);
      OK(Me("hello\0world\0", 12), t.data());
      OK(12, t.length());
   }

   Case(regex)
   {
      // gcc 4.8 not support regex caseless match ! it fails
      h2::h2_string pattern("A.*");
      h2::h2_string subject("abcdef");
      OK(subject.regex_match(pattern, true));
   }

   Case(wildcard)
   {
      h2::h2_string pattern("A*");
      h2::h2_string subject("abcdef");
      OK(subject.wildcard_match(pattern, true));
   }
}