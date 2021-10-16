#include "../source/h2_unit.cpp"

SUITE(h2_line)
{
   Case(init empty)
   {
      h2::h2_line line;
      OK(0, line.size());
   }

   Case(init string)
   {
      h2::h2_line line("hello");
      OK(1, line.size());
      OK(ListOf("hello"), line);
   }

   Case(init string initializer_list)
   {
      h2::h2_line line = {"hello", "world"};
      OK(2, line.size());
      OK(ListOf("hello", "world"), line);
   }

   Case(line width)
   {
      h2::h2_line line = {" 123 ", " 456 "};
      OK(10, line.width());
   }

   Case(line width with color)
   {
      h2::h2_line line = {"123", "\033{red}", "456"};
      OK(6, line.width());
   }

   Case(indent)
   {
      h2::h2_line line = {"123", "456"};
      line.indent(3);
      OK(ListOf("   ", "123", "456"), line);
   }

   Case(padding)
   {
      h2::h2_line line = {"123", "456"};
      line.padding(3);
      OK(ListOf("123", "456", "   "), line);
   }

   Case(printf)
   {
      h2::h2_line line;
      line.printf("red", "%d", 123);
      OK(ListOf("\033{red}", "123", "\033{reset}"), line);
   }

   Case(printf with null style)
   {
      h2::h2_line line;
      line.printf(nullptr, "%d", 123);
      OK(ListOf("123"), line);
   }

   Case(printf with empty style)
   {
      h2::h2_line line;
      line.printf("", "%d", 123);
      OK(ListOf("123"), line);
   }

   Case(+=)
   {
      h2::h2_line s = {"456", "789"};
      h2::h2_line line = "123";
      line += s;
      OK(ListOf("123", "456", "789"), line);
   }

   Case(gray_quote)
   {
      h2::h2_line l1({"012", "\033{red}", "3456789", "\033{reset}"});
      OK(ListOf("012", "\033{red}", "3456789", "\033{reset}"), l1.gray_quote());

      h2::h2_line l2({"\"012", "\033{red}", "3456789\"", "\033{reset}"});
      OK(ListOf("\033{+dark gray}", "\"", "\033{-dark gray}",
                "012", "\033{red}", "3456789",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{reset}"),
         l2.gray_quote());
   }

   Case(stringify)
   {
      h2::h2_line line({"012", "\033{red}", "3456789", "\033{reset}"});
      OK("0123456789", line.string());
   }

   Case(samesizify line)
   {
      h2::h2_line e = {"123", "456"};
      h2::h2_line a = {"12345"};
      h2::h2_line::samesizify(e, a);
      OK(e.width() == a.width());
   }

   Case(enclosed)
   {
      h2::h2_line a1 = {"123"};
      OK(!a1.enclosed('\"'));
      h2::h2_line a2 = {"123", "456"};
      OK(!a2.enclosed('\"'));
      h2::h2_line a3 = {"123", "\033{red}", "456", "\033{reset}"};
      OK(!a3.enclosed('\"'));

      h2::h2_line b1 = {"\"123\""};
      OK(b1.enclosed('\"'));
      h2::h2_line b2 = {"\"123", "456\""};
      OK(b2.enclosed('\"'));
      h2::h2_line b3 = {"\"123", "\033{red}", "456\"", "\033{reset}"};
      OK(b3.enclosed('\"'));
   }

   Case(samesizify lines)
   {
      h2::h2_lines e = {{"123", "456"}, {"1234"}};
      h2::h2_lines a = {{"12345"}};
      h2::h2_lines::samesizify(e, a);
      OK(e.size() == a.size());
   }
}

SUITE(h2_line operator+)
{
   Case(const char*)
   {
      h2::h2_line s1 = {"456", "789"};

      h2::h2_line st1 = s1 + "123";
      OK(ListOf("456", "789", "123"), st1);

      h2::h2_line st2 = "123" + s1;
      OK(ListOf("123", "456", "789"), st2);
   }

   Case(h2_string)
   {
      h2::h2_line s1 = {"456", "789"};
      h2::h2_string s2 = "123";

      h2::h2_line st1 = s1 + s2;
      OK(ListOf("456", "789", "123"), st1);

      h2::h2_line st2 = s2 + s1;
      OK(ListOf("123", "456", "789"), st2);
   }

   Case(line)
   {
      h2::h2_line s1 = {"456", "789"};
      h2::h2_line s2 = "123";

      h2::h2_line st1 = s1 + s2;
      OK(ListOf("456", "789", "123"), st1);

      h2::h2_line st2 = s2 + s1;
      OK(ListOf("123", "456", "789"), st2);
   }
}

SUITE(h2_line abbreviate without style)
{
   Case(no abbreviate with 1 string)
   {
      h2::h2_line line("0123456789");
      OK(ListOf("0123456789"), line.abbreviate(10));
      OK(ListOf("0123456789"), line.abbreviate(11));
   }

   Case(no abbreviate with 2 string)
   {
      h2::h2_line line({"012", "3456789"});
      OK(ListOf("012", "3456789"), line.abbreviate(10));
      OK(ListOf("012", "3456789"), line.abbreviate(11));
   }

   Case(abbreviate 1 string)
   {
      h2::h2_line line("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(3));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(4));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(5));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(6));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(7));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(8));
      OK(ListOf("012345", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(9));
   }

   Case(abbreviate 2 string)
   {
      h2::h2_line line({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(3));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(4));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(5));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(6));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(7));
      OK(ListOf("012", "34", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(8));
      OK(ListOf("012", "345", "\033{+dark gray}", "...", "\033{-dark gray}"), line.abbreviate(9));
   }

   Case(abbreviate 1 string with tail 1)
   {
      h2::h2_line line("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(4, 1));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(5, 1));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(6, 1));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(7, 1));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(8, 1));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(9, 1));
   }

   Case(abbreviate 2 string with tail 1)
   {
      h2::h2_line line({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(4, 1));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(5, 1));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(6, 1));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(7, 1));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(8, 1));
      OK(ListOf("012", "34", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), line.abbreviate(9, 1));
   }

   Case(abbreviate 1 string with tail 2)
   {
      h2::h2_line line("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(7, 2));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(8, 2));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(9, 2));
   }

   Case(abbreviate 2 string with tail 2)
   {
      h2::h2_line line({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(7, 2));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(8, 2));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), line.abbreviate(9, 2));
   }
}

SUITE(h2_line abbreviate with style)
{
   Case(no abbreviate with 2 string)
   {
      h2::h2_line line({"\033{red}", "012", "3456789", "\033{reset}"});
      OK(ListOf("\033{red}", "012", "3456789", "\033{reset}"), line.abbreviate(10));
      OK(ListOf("\033{red}", "012", "3456789", "\033{reset}"), line.abbreviate(11));
   }

   Case(abbreviate 1 string)
   {
      h2::h2_line line({"\033{red}", "0123456789", "\033{reset}"});
      OK(ListOf("\033{red}", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), line.abbreviate(3));
      OK(ListOf("\033{red}", "0", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), line.abbreviate(4));
      OK(ListOf("\033{red}", "01", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), line.abbreviate(5));
      OK(ListOf("\033{red}", "012", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), line.abbreviate(6));
      OK(ListOf("\033{red}", "0123", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), line.abbreviate(7));
      OK(ListOf("\033{red}", "01234", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), line.abbreviate(8));
      OK(ListOf("\033{red}", "012345", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), line.abbreviate(9));
   }

   Case(abbreviate 2 string with tail 2)
   {
      h2::h2_line line({"012", "\033{red}", "3456789", "\033{reset}"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), line.abbreviate(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), line.abbreviate(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), line.abbreviate(7, 2));
      OK(ListOf("012", "\033{red}", "\033{+dark gray}", "...", "\033{-dark gray}", "89", "\033{reset}"), line.abbreviate(8, 2));
      OK(ListOf("012", "\033{red}", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "89", "\033{reset}"), line.abbreviate(9, 2));
   }
}

SUITE(h2_line abbreviate escape)
{
   Case(abbreviate with CRLF)
   {
      h2::h2_line line("A\r1\t\n");
      OK(ListOf("A\\r1\\t\\n"), line.abbreviate(10));
   }
}
