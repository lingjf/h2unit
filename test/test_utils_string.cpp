#include "../source/h2_unit.cpp"

SUITE(utf8len)
{
   Case(ascii)
   {
      OK(1, h2::utf8len("a"));
      OK(1, h2::utf8len("ab"));
   }

   Case(symbol)
   {
      OK(3, h2::utf8len("┊"));
   }

   Case(Chinese)
   {
      OK(3, h2::utf8len("中"));
      OK(3, h2::utf8len("中国"));
   }

   Case(japanese)
   {
      OK(3, h2::utf8len("い"));
      OK(3, h2::utf8len("いち"));
   }

   Case(korea)
   {
      OK(3, h2::utf8len("귀"));
      OK(3, h2::utf8len("귀하"));
   }
}

SUITE(string constructor)
{
   Case(n char)
   {
      h2::h2_string a0(0, 'a');
      OK("", a0);
      h2::h2_string a1(1, 'a');
      OK("a", a1);
      h2::h2_string a2(2, 'a');
      OK("aa", a2);
   }

   Case(n string)
   {
      h2::h2_string a0(0, "ab");
      OK("", a0);
      h2::h2_string a1(1, "ab");
      OK("a", a1);
      h2::h2_string a2(2, "ab");
      OK("ab", a2);
   }

   Case(NULL)
   {
      const char* s0 = nullptr;
      h2::h2_string a0(s0);
      OK("(null)", a0);
   }
}

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

   Case(NULL)
   {
      const char* s0 = nullptr;
      h2::h2_string a0 = s0;
      OK("(null)", a0);
      h2::h2_string a1 = nullptr;
      OK("(null)", a1);
   }
}

CASE(h2_string convertable)
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
   OK((std::is_convertible<std::nullptr_t, h2::h2_string>::value));
   OK((std::is_convertible<decltype(nullptr), h2::h2_string>::value));

   OK((!std::is_convertible<int, h2::h2_string>::value));
   OK((!std::is_convertible<char, h2::h2_string>::value));
   OK((!std::is_convertible<unsigned char, h2::h2_string>::value));
   OK((!std::is_convertible<unsigned char*, h2::h2_string>::value));
   OK((!std::is_convertible<void*, h2::h2_string>::value));
}

SUITE(string)
{
   Case(width ascii)
   {
      h2::h2_string a0 = "";
      OK(0, a0.width());
      h2::h2_string a1 = "a";
      OK(1, a1.width());
      h2::h2_string a2 = "ab";
      OK(2, a2.width());
   }

   Case(width symbol)
   {
      h2::h2_string a1 = "┊";
      OK(2, a1.width());
   }

   Case(width Chinese)
   {
      h2::h2_string a1 = "中";
      OK(2, a1.width());
      h2::h2_string a2 = "中国";
      OK(4, a2.width());
   }

   Case(width japanese)
   {
      h2::h2_string a1 = "い";
      OK(2, a1.width());
      h2::h2_string a2 = "いち";
      OK(4, a2.width());
   }

   Case(width korea)
   {
      h2::h2_string a1 = "귀";
      OK(2, a1.width());
      h2::h2_string a2 = "귀하";
      OK(4, a2.width());
   }

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

   Case(centre)
   {
      OK("ab", h2::h2_string("ab").centre(2));
      OK("ab ", h2::h2_string("ab").centre(3));
      OK(" ab ", h2::h2_string("ab").centre(4));
      OK(" ab  ", h2::h2_string("ab").centre(5));
      OK("  ab  ", h2::h2_string("ab").centre(6));

      OK("abc", h2::h2_string("abc").centre(3));
      OK("abc ", h2::h2_string("abc").centre(4));
      OK(" abc ", h2::h2_string("abc").centre(5));
      OK(" abc  ", h2::h2_string("abc").centre(6));
   }

   Case(centre abnormal)
   {
      OK("", h2::h2_string("").centre(0));
      OK("abc", h2::h2_string("abc").centre(2));
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
}

SUITE(string enclosed)
{
   Case(none)
   {
      h2::h2_string a1 = "abc";
      OK(!a1.enclosed('\"'));
   }

   Case(double quote)
   {
      h2::h2_string a1 = "\"abc\"";
      OK(a1.enclosed('\"'));
   }

   Case(single quote)
   {
      h2::h2_string a1 = "\'abc\'";
      OK(a1.enclosed('\''));
   }

   Case(parentheses)
   {
      h2::h2_string a1 = "(abc)";
      OK(a1.enclosed('(', ')'));
   }

   Case(braces)
   {
      h2::h2_string a1 = "[abc]";
      OK(a1.enclosed('[', ']'));
   }

   Case(brace parenthesis)
   {
      h2::h2_string a1 = "[abc)";
      OK(a1.enclosed('[', ')'));
   }
}

SUITE(string unenclose)
{
   Case(none)
   {
      h2::h2_string a1 = "abc";
      OK("abc", a1.unenclose('\"'));
   }

   Case(double quote)
   {
      h2::h2_string a1 = "\"abc\"";
      OK("abc", a1.unenclose('\"'));
   }

   Case(single quote)
   {
      h2::h2_string a1 = "\'abc\'";
      OK("abc", a1.unenclose('\''));
   }

   Case(parentheses)
   {
      h2::h2_string a1 = "(abc)";
      OK("abc", a1.unenclose('(', ')'));
   }

   Case(braces)
   {
      h2::h2_string a1 = "[abc]";
      OK("abc", a1.unenclose('[', ']'));
   }

   Case(brace parenthesis)
   {
      h2::h2_string a1 = "[abc)";
      OK("abc", a1.unenclose('[', ')'));
   }
}

SUITE(string disperse)
{
   Case(ascii)
   {
      h2::h2_string a0 = "";
      OK(ListOf(), a0.disperse());
      h2::h2_string a1 = "a";
      OK(ListOf("a"), a1.disperse());
      h2::h2_string a2 = "ab";
      OK(ListOf("a", "b"), a2.disperse());
   }

   Case(Chinese)
   {
      h2::h2_string C1 = "中";
      OK(ListOf("中"), C1.disperse());
      h2::h2_string C2 = "中国";
      OK(ListOf("中", "国"), C2.disperse());
   }

   Case(CJK)
   {
      h2::h2_string j1 = "い";
      OK(ListOf("い"), j1.disperse());
      h2::h2_string j2 = "いち";
      OK(ListOf("い", "ち"), j2.disperse());
   }
}

SUITE(string trim)
{
   Case(1 space leading)
   {
      h2::h2_string a = " abc  def";
      OK("abc  def", a.trim());
   }

   Case(2 spaces leading)
   {
      h2::h2_string a = "  abc  def";
      OK("abc  def", a.trim());
   }

   Case(1 space leading and trailing)
   {
      h2::h2_string a = " abc  def ";
      OK("abc  def", a.trim());
   }

   Case(2 spaces leading and trailing)
   {
      h2::h2_string a = "  abc  def  ";
      OK("abc  def", a.trim());
   }
}

SUITE(string squash)
{
   Case(2 spaces to 1 space)
   {
      h2::h2_string a = "abc  def";
      OK("abc def", a.squash());
   }

   Case(3 spaces to 1 space)
   {
      h2::h2_string a = "abc   def";
      OK("abc def", a.squash());
   }

   Case(1 space and 1 newline to 1 space)
   {
      h2::h2_string a = "abc \ndef";
      OK("abc def", a.squash());
   }

   Case(1 tab and 1 space and 1 newline to 1 space)
   {
      h2::h2_string a = "abc\t \ndef";
      OK("abc def", a.squash());
   }

   Case(ignore in double quote)
   {
      h2::h2_string a = "abc\"\t \n  \"def";
      OK("abc\"     \"def", a.squash());
      OK("abc\"     \"def", a.squash(false));
   }

   Case(also in double quote)
   {
      h2::h2_string a = "abc\"\t \n  \"def";
      OK("abc\" \"def", a.squash(true));
   }

   Case(ignore in single quote)
   {
      h2::h2_string a = "abc'\t \n  'def";
      OK("abc'     'def", a.squash());
      OK("abc'     'def", a.squash(false));
   }

   Case(also in single quote)
   {
      h2::h2_string a = "abc'\t \n  'def";
      OK("abc' 'def", a.squash(true));
   }

   Case(2 leading and trailing)
   {
      h2::h2_string a = "  abc\"\t \n  \"def \n";
      OK("abc\"     \"def", a.squash(false));
      OK("abc\" \"def", a.squash(true));
   }
}
