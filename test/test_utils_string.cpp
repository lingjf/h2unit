#include "../source/h2_unit.cpp"

SUITE(string assign)
{
   Case(init with std_string)
   {
      std::string s = "hello";
      h2::h2_string a(s);
      OK("hello", a);
   }

   Case(init with literal string)
   {
      h2::h2_string a("hello");
      OK("hello", a);
   }

   Case(init with printf)
   {
      h2::h2_string a("hello %s", "world");
      OK("hello world", a);
   }

   Case(init assign)
   {
      h2::h2_string a = "hello";
      OK("hello", a);
   }
}

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
      OK(!a.endswith("Hello  World"));
   }

   Case(enclosed)
   {
      h2::h2_string a1 = "abc";
      OK(!a1.enclosed('\"'));
      h2::h2_string a2 = "\"abc\"";
      OK(a2.enclosed('\"'));
   }

   Case(escape)
   {
      h2::h2_string a1 = "a\rb\nc";
      OK("a\\rb\\nc", a1.escape());
   }

   Case(unescape)
   {
      h2::h2_string a1 = "a\\rb\\nc";
      OK("a\rb\nc", a1.unescape());
   }

   Case(unquote)
   {
      h2::h2_string a1 = "abc";
      OK("abc", a1.unquote());

      h2::h2_string a2 = "\"abc\"";
      OK("abc", a2.unquote());
   }

   Case(replace_all)
   {
      h2::h2_string a1 = "hello abc, worldabc.";
      OK("hello xz, worldxz.", a1.replace_all("abc", "xz"));

      h2::h2_string a2 = "hello \\n, world\\n.";
      OK("hello \n, world\n.", a2.replace_all("\\n", "\n"));
   }

   Case(sprintf)
   {
      h2::h2_string a;
      a.sprintf("%s %s", "Hello", "World");
      OK(a.equals("Hello World"));
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

   Case(center abnormal)
   {
      OK("", h2::h2_string("").center(0));
      OK("abc", h2::h2_string("abc").center(2));
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

   Case(convertable)
   {
      char char_array[128];
      OK((std::is_convertible<char*, h2::h2_string>::value));
      OK((std::is_convertible<char*&, h2::h2_string>::value));
      OK((std::is_convertible<const char*, h2::h2_string>::value));
      OK((std::is_convertible<char* const, h2::h2_string>::value));
      OK((std::is_convertible<const char* const, h2::h2_string>::value));
      OK((std::is_convertible<std::string, h2::h2_string>::value));
      OK((std::is_convertible<const std::string, h2::h2_string>::value));
      OK((std::is_convertible<std::string&, h2::h2_string>::value));
      OK((std::is_convertible<decltype(char_array), h2::h2_string>::value));
      OK((std::is_convertible<h2::h2_string, h2::h2_string>::value));

      OK((!std::is_convertible<int, h2::h2_string>::value));
      OK((!std::is_convertible<char, h2::h2_string>::value));
      OK((!std::is_convertible<unsigned char, h2::h2_string>::value));
      OK((!std::is_convertible<unsigned char*, h2::h2_string>::value));
   }
}
