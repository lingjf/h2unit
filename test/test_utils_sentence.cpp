#include "../source/h2_unit.cpp"

SUITE(h2_sentence)
{
   Case(init empty)
   {
      h2::h2_sentence st;
      OK(0, st.size());
   }

   Case(init string)
   {
      h2::h2_sentence st("hello");
      OK(1, st.size());
      OK(ListOf("hello"), st);
   }

   Case(init string initializer_list)
   {
      h2::h2_sentence st = {"hello", "world"};
      OK(2, st.size());
      OK(ListOf("hello", "world"), st);
   }

   Case(sentence width)
   {
      h2::h2_sentence st = {" 123 ", " 456 "};
      OK(10, st.width());
   }

   Case(sentence width with color)
   {
      h2::h2_sentence st = {"123", "\033{red}", "456"};
      OK(6, st.width());
   }

   Case(indent)
   {
      h2::h2_sentence st = {"123", "456"};
      st.indent(3);
      OK(ListOf("   ", "123", "456"), st);
   }

   Case(padding)
   {
      h2::h2_sentence st = {"123", "456"};
      st.padding(3);
      OK(ListOf("123", "456", "   "), st);
   }

   Case(printf)
   {
      h2::h2_sentence st;
      st.printf("red", "%d", 123);
      OK(ListOf("\033{red}", "123", "\033{reset}"), st);
   }

   Case(printf with null style)
   {
      h2::h2_sentence st;
      st.printf(nullptr, "%d", 123);
      OK(ListOf("123"), st);
   }

   Case(printf with empty style)
   {
      h2::h2_sentence st;
      st.printf("", "%d", 123);
      OK(ListOf("123"), st);
   }

   Case(+=)
   {
      h2::h2_sentence s = {"456", "789"};
      h2::h2_sentence st = "123";
      st += s;
      OK(ListOf("123", "456", "789"), st);
   }

   Case(gray_quote)
   {
      h2::h2_sentence l1({"012", "\033{red}", "3456789", "\033{reset}"});
      OK(ListOf("012", "\033{red}", "3456789", "\033{reset}"), l1.gray_quote());

      h2::h2_sentence l2({"\"012", "\033{red}", "3456789\"", "\033{reset}"});
      OK(ListOf("\033{+dark gray}", "\"", "\033{-dark gray}",
                "012", "\033{red}", "3456789",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{reset}"),
         l2.gray_quote());
   }

   Case(stringify)
   {
      h2::h2_sentence st({"012", "\033{red}", "3456789", "\033{reset}"});
      OK("0123456789", st.string());
   }

   Case(samesizify sentence)
   {
      h2::h2_sentence e = {"123", "456"};
      h2::h2_sentence a = {"12345"};
      e.samesizify(a);
      OK(e.width() == a.width());
   }

   Case(enclosed)
   {
      h2::h2_sentence a1 = {"123"};
      OK(!a1.enclosed('\"'));
      h2::h2_sentence a2 = {"123", "456"};
      OK(!a2.enclosed('\"'));
      h2::h2_sentence a3 = {"123", "\033{red}", "456", "\033{reset}"};
      OK(!a3.enclosed('\"'));

      h2::h2_sentence b1 = {"\"123\""};
      OK(b1.enclosed('\"'));
      h2::h2_sentence b2 = {"\"123", "456\""};
      OK(b2.enclosed('\"'));
      h2::h2_sentence b3 = {"\"123", "\033{red}", "456\"", "\033{reset}"};
      OK(b3.enclosed('\"'));
   }

   Case(samesizify sentences)
   {
      h2::h2_paragraph e = {{"123", "456"}, {"1234"}};
      h2::h2_paragraph a = {{"12345"}};
      e.samesizify(a);
      OK(e.size() == a.size());
   }
}

SUITE(h2_sentence operator+)
{
   Case(const char*)
   {
      h2::h2_sentence s1 = {"456", "789"};

      h2::h2_sentence st1 = s1 + "123";
      OK(ListOf("456", "789", "123"), st1);

      h2::h2_sentence st2 = "123" + s1;
      OK(ListOf("123", "456", "789"), st2);
   }

   Case(h2_string)
   {
      h2::h2_sentence s1 = {"456", "789"};
      h2::h2_string s2 = "123";

      h2::h2_sentence st1 = s1 + s2;
      OK(ListOf("456", "789", "123"), st1);

      h2::h2_sentence st2 = s2 + s1;
      OK(ListOf("123", "456", "789"), st2);
   }

   Case(sentence)
   {
      h2::h2_sentence s1 = {"456", "789"};
      h2::h2_sentence s2 = "123";

      h2::h2_sentence st1 = s1 + s2;
      OK(ListOf("456", "789", "123"), st1);

      h2::h2_sentence st2 = s2 + s1;
      OK(ListOf("123", "456", "789"), st2);
   }
}

SUITE(h2_sentence acronym without style)
{
   Case(no acronym with 1 string)
   {
      h2::h2_sentence st("0123456789");
      OK(ListOf("0123456789"), st.acronym(10));
      OK(ListOf("0123456789"), st.acronym(11));
   }

   Case(no acronym with 2 string)
   {
      h2::h2_sentence st({"012", "3456789"});
      OK(ListOf("012", "3456789"), st.acronym(10));
      OK(ListOf("012", "3456789"), st.acronym(11));
   }

   Case(acronym 1 string)
   {
      h2::h2_sentence st("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(3));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(4));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(5));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(6));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(7));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(8));
      OK(ListOf("012345", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(9));
   }

   Case(acronym 2 string)
   {
      h2::h2_sentence st({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(3));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(4));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(5));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(6));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(7));
      OK(ListOf("012", "34", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(8));
      OK(ListOf("012", "345", "\033{+dark gray}", "...", "\033{-dark gray}"), st.acronym(9));
   }

   Case(acronym 1 string with tail 1)
   {
      h2::h2_sentence st("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(4, 1));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(5, 1));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(6, 1));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(7, 1));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(8, 1));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(9, 1));
   }

   Case(acronym 2 string with tail 1)
   {
      h2::h2_sentence st({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(4, 1));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(5, 1));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(6, 1));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(7, 1));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(8, 1));
      OK(ListOf("012", "34", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), st.acronym(9, 1));
   }

   Case(acronym 1 string with tail 2)
   {
      h2::h2_sentence st("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(7, 2));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(8, 2));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(9, 2));
   }

   Case(acronym 2 string with tail 2)
   {
      h2::h2_sentence st({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(7, 2));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(8, 2));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), st.acronym(9, 2));
   }
}

SUITE(h2_sentence acronym with style)
{
   Case(no acronym with 2 string)
   {
      h2::h2_sentence st({"\033{red}", "012", "3456789", "\033{reset}"});
      OK(ListOf("\033{red}", "012", "3456789", "\033{reset}"), st.acronym(10));
      OK(ListOf("\033{red}", "012", "3456789", "\033{reset}"), st.acronym(11));
   }

   Case(acronym 1 string)
   {
      h2::h2_sentence st({"\033{red}", "0123456789", "\033{reset}"});
      OK(ListOf("\033{red}", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), st.acronym(3));
      OK(ListOf("\033{red}", "0", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), st.acronym(4));
      OK(ListOf("\033{red}", "01", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), st.acronym(5));
      OK(ListOf("\033{red}", "012", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), st.acronym(6));
      OK(ListOf("\033{red}", "0123", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), st.acronym(7));
      OK(ListOf("\033{red}", "01234", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), st.acronym(8));
      OK(ListOf("\033{red}", "012345", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), st.acronym(9));
   }

   Case(acronym 2 string with tail 2)
   {
      h2::h2_sentence st({"012", "\033{red}", "3456789", "\033{reset}"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), st.acronym(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), st.acronym(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), st.acronym(7, 2));
      OK(ListOf("012", "\033{red}", "\033{+dark gray}", "...", "\033{-dark gray}", "89", "\033{reset}"), st.acronym(8, 2));
      OK(ListOf("012", "\033{red}", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "89", "\033{reset}"), st.acronym(9, 2));
   }
}

SUITE(h2_sentence acronym escape)
{
   Case(acronym with CRLF)
   {
      h2::h2_sentence st("A\r1\t\n");
      OK(ListOf("A\\r1\\t\\n"), st.acronym(10));
   }
}
