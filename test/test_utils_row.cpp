#include "../source/h2_unit.cpp"

SUITE(h2_row)
{
   Case(init empty)
   {
      h2::h2_row row;
      OK(0, row.size());
   }

   Case(init string)
   {
      h2::h2_row row("hello");
      OK(1, row.size());
      OK(ListOf("hello"), row);
   }

   Case(init string initializer_list)
   {
      h2::h2_row row = {"hello", "world"};
      OK(2, row.size());
      OK(ListOf("hello", "world"), row);
   }

   Case(row width)
   {
      h2::h2_row row = {" 123 ", " 456 "};
      OK(10, row.width());
   }

   Case(row width with color)
   {
      h2::h2_row row = {"123", "\033{red}", "456"};
      OK(6, row.width());
   }

   Case(indent)
   {
      h2::h2_row row = {"123", "456"};
      row.indent(3);
      OK(ListOf("   ", "123", "456"), row);
   }

   Case(padding)
   {
      h2::h2_row row = {"123", "456"};
      row.padding(3);
      OK(ListOf("123", "456", "   "), row);
   }

   Case(printf)
   {
      h2::h2_row row;
      row.printf("red", "%d", 123);
      OK(ListOf("\033{red}", "123", "\033{reset}"), row);
   }

   Case(printf with null style)
   {
      h2::h2_row row;
      row.printf(nullptr, "%d", 123);
      OK(ListOf("123"), row);
   }

   Case(printf with empty style)
   {
      h2::h2_row row;
      row.printf("", "%d", 123);
      OK(ListOf("123"), row);
   }

   Case(+=)
   {
      h2::h2_row s = {"456", "789"};
      h2::h2_row row = "123";
      row += s;
      OK(ListOf("123", "456", "789"), row);
   }

   Case(gray_quote)
   {
      h2::h2_row l1({"012", "\033{red}", "3456789", "\033{reset}"});
      OK(ListOf("012", "\033{red}", "3456789", "\033{reset}"), l1.gray_quote());

      h2::h2_row l2({"\"012", "\033{red}", "3456789\"", "\033{reset}"});
      OK(ListOf("\033{+dark gray}", "\"", "\033{-dark gray}",
                "012", "\033{red}", "3456789",
                "\033{+dark gray}", "\"", "\033{-dark gray}",
                "\033{reset}"),
         l2.gray_quote());
   }

   Case(stringify)
   {
      h2::h2_row row({"012", "\033{red}", "3456789", "\033{reset}"});
      OK("0123456789", row.string());
   }

   Case(samesizify row)
   {
      h2::h2_row e = {"123", "456"};
      h2::h2_row a = {"12345"};
      e.samesizify(a);
      OK(e.width() == a.width());
   }

   Case(enclosed)
   {
      h2::h2_row a1 = {"123"};
      OK(!a1.enclosed('\"'));
      h2::h2_row a2 = {"123", "456"};
      OK(!a2.enclosed('\"'));
      h2::h2_row a3 = {"123", "\033{red}", "456", "\033{reset}"};
      OK(!a3.enclosed('\"'));

      h2::h2_row b1 = {"\"123\""};
      OK(b1.enclosed('\"'));
      h2::h2_row b2 = {"\"123", "456\""};
      OK(b2.enclosed('\"'));
      h2::h2_row b3 = {"\"123", "\033{red}", "456\"", "\033{reset}"};
      OK(b3.enclosed('\"'));
   }

   Case(samesizify rows)
   {
      h2::h2_rows e = {{"123", "456"}, {"1234"}};
      h2::h2_rows a = {{"12345"}};
      e.samesizify(a);
      OK(e.size() == a.size());
   }
}

SUITE(h2_row operator+)
{
   Case(const char*)
   {
      h2::h2_row s1 = {"456", "789"};

      h2::h2_row row1 = s1 + "123";
      OK(ListOf("456", "789", "123"), row1);

      h2::h2_row row2 = "123" + s1;
      OK(ListOf("123", "456", "789"), row2);
   }

   Case(h2_string)
   {
      h2::h2_row s1 = {"456", "789"};
      h2::h2_string s2 = "123";

      h2::h2_row row1 = s1 + s2;
      OK(ListOf("456", "789", "123"), row1);

      h2::h2_row row2 = s2 + s1;
      OK(ListOf("123", "456", "789"), row2);
   }

   Case(row)
   {
      h2::h2_row s1 = {"456", "789"};
      h2::h2_row s2 = "123";

      h2::h2_row row1 = s1 + s2;
      OK(ListOf("456", "789", "123"), row1);

      h2::h2_row row2 = s2 + s1;
      OK(ListOf("123", "456", "789"), row2);
   }
}

SUITE(h2_row acronym without style)
{
   Case(no acronym with 1 string)
   {
      h2::h2_row row("0123456789");
      OK(ListOf("0123456789"), row.acronym(10));
      OK(ListOf("0123456789"), row.acronym(11));
   }

   Case(no acronym with 2 string)
   {
      h2::h2_row row({"012", "3456789"});
      OK(ListOf("012", "3456789"), row.acronym(10));
      OK(ListOf("012", "3456789"), row.acronym(11));
   }

   Case(acronym 1 string)
   {
      h2::h2_row row("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(3));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(4));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(5));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(6));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(7));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(8));
      OK(ListOf("012345", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(9));
   }

   Case(acronym 2 string)
   {
      h2::h2_row row({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(3));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(4));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(5));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(6));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(7));
      OK(ListOf("012", "34", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(8));
      OK(ListOf("012", "345", "\033{+dark gray}", "...", "\033{-dark gray}"), row.acronym(9));
   }

   Case(acronym 1 string with tail 1)
   {
      h2::h2_row row("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(4, 1));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(5, 1));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(6, 1));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(7, 1));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(8, 1));
      OK(ListOf("01234", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(9, 1));
   }

   Case(acronym 2 string with tail 1)
   {
      h2::h2_row row({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(4, 1));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(5, 1));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(6, 1));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(7, 1));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(8, 1));
      OK(ListOf("012", "34", "\033{+dark gray}", "...", "\033{-dark gray}", "9"), row.acronym(9, 1));
   }

   Case(acronym 1 string with tail 2)
   {
      h2::h2_row row("0123456789");
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(7, 2));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(8, 2));
      OK(ListOf("0123", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(9, 2));
   }

   Case(acronym 2 string with tail 2)
   {
      h2::h2_row row({"012", "3456789"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(7, 2));
      OK(ListOf("012", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(8, 2));
      OK(ListOf("012", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "89"), row.acronym(9, 2));
   }
}

SUITE(h2_row acronym with style)
{
   Case(no acronym with 2 string)
   {
      h2::h2_row row({"\033{red}", "012", "3456789", "\033{reset}"});
      OK(ListOf("\033{red}", "012", "3456789", "\033{reset}"), row.acronym(10));
      OK(ListOf("\033{red}", "012", "3456789", "\033{reset}"), row.acronym(11));
   }

   Case(acronym 1 string)
   {
      h2::h2_row row({"\033{red}", "0123456789", "\033{reset}"});
      OK(ListOf("\033{red}", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), row.acronym(3));
      OK(ListOf("\033{red}", "0", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), row.acronym(4));
      OK(ListOf("\033{red}", "01", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), row.acronym(5));
      OK(ListOf("\033{red}", "012", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), row.acronym(6));
      OK(ListOf("\033{red}", "0123", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), row.acronym(7));
      OK(ListOf("\033{red}", "01234", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), row.acronym(8));
      OK(ListOf("\033{red}", "012345", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{reset}"), row.acronym(9));
   }

   Case(acronym 2 string with tail 2)
   {
      h2::h2_row row({"012", "\033{red}", "3456789", "\033{reset}"});
      OK(ListOf("\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), row.acronym(5, 2));
      OK(ListOf("0", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), row.acronym(6, 2));
      OK(ListOf("01", "\033{+dark gray}", "...", "\033{-dark gray}", "\033{red}", "89", "\033{reset}"), row.acronym(7, 2));
      OK(ListOf("012", "\033{red}", "\033{+dark gray}", "...", "\033{-dark gray}", "89", "\033{reset}"), row.acronym(8, 2));
      OK(ListOf("012", "\033{red}", "3", "\033{+dark gray}", "...", "\033{-dark gray}", "89", "\033{reset}"), row.acronym(9, 2));
   }
}

SUITE(h2_row acronym escape)
{
   Case(acronym with CRLF)
   {
      h2::h2_row row("A\r1\t\n");
      OK(ListOf("A\\r1\\t\\n"), row.acronym(10));
   }
}

h2::h2_rows& concat_front(h2::h2_rows& thus, const h2::h2_rows& rows)
{
   thus.insert(thus.begin(), rows.begin(), rows.end());
   return thus;
}

h2::h2_rows& concat_front(h2::h2_rows& thus, const h2::h2_row& row)
{
   thus.insert(thus.begin(), row);
   return thus;
}

SUITE(h2_rows)
{
   Case(max rows length)
   {
      h2::h2_rows rows = {{"123", "456"}, {"1234"}};
      OK(6, rows.width());
   }

   Case(max rows length with color)
   {
      h2::h2_rows rows = {{"123", "\033{red}", "456"}, {"1234"}};
      OK(6, rows.width());
   }

   Case(+=)
   {
      h2::h2_rows s = {{"abc", "def"}, {"ghi"}};
      h2::h2_rows rows = {{"123", "456"}, {"789"}};

      rows += s;
      OK(ListOf(ListOf("123", "456"), ListOf("789"), ListOf("abc", "def"), ListOf("ghi")), rows);
   }

   Case(concat front rows)
   {
      h2::h2_rows s = {{"abc", "def"}, {"ghi"}};
      h2::h2_rows rows = {{"123", "456"}, {"789"}};

      concat_front(rows, s);
      OK(ListOf(ListOf("abc", "def"), ListOf("ghi"), ListOf("123", "456"), ListOf("789")), rows);
   }

   Case(concat front row)
   {
      h2::h2_row s = {"abc", "def"};
      h2::h2_rows rows = {{"123", "456"}, {"789"}};

      concat_front(rows, s);
      OK(ListOf(ListOf("abc", "def"), ListOf("123", "456"), ListOf("789")), rows);
   }

   Case(foldable)
   {
      h2::h2_rows rows = {{"123", "456"}, {"789"}};
      OK(rows.foldable(10));
   }

   Case(foldable with style)
   {
      h2::h2_rows rows = {{"123", "\033{red}", "456", "\033{reset}"}, {"789"}};
      OK(rows.foldable(10));
   }

   Case(foldable with indent)
   {
      h2::h2_rows rows = {{"123", "456"}, {"   ", "789"}};
      OK(rows.foldable(10));
   }

   Case(fold)
   {
      h2::h2_rows rows = {{"123", "456"}, {"   ", "789"}};
      OK(ListOf("123", "456", "789"), rows.folds());
   }

   Case(sequence)
   {
      h2::h2_rows rows = {{"123", "456"}, {"789"}};
      rows.sequence();
      OK(ListOf(ListOf("\033{+dark gray}", "0. ", "\033{-dark gray}", "123", "456"), ListOf("\033{+dark gray}", "1. ", "\033{-dark gray}", "789")), rows);
   }

   Case(sequence indent and start from 1)
   {
      h2::h2_rows rows = {{"123", "456"}, {"789"}};
      rows.sequence(2, 1);
      OK(ListOf(ListOf("  ", "\033{+dark gray}", "1. ", "\033{-dark gray}", "123", "456"), ListOf("  ", "\033{+dark gray}", "2. ", "\033{-dark gray}", "789")), rows);
   }

   Case(samesizify)
   {
      h2::h2_rows row1 = {{"abc", "def"}};
      h2::h2_rows row2 = {{"123", "456"}, {"789"}};

      row2.samesizify(row1);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), row2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), row1);
   }

   Case(samesizify 2)
   {
      h2::h2_rows row1 = {{"abc", "def"}};
      h2::h2_rows row2 = {{"123", "456"}, {"789"}};

      row1.samesizify(row2);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), row2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), row1);
   }
}
