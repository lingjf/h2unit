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

   Case(concat_back)
   {
      h2::h2_line s = {"456", "789"};
      h2::h2_line line = "123";
      line.concat_back(s);
      OK(ListOf("123", "456", "789"), line);
   }

   Case(concat_back with style)
   {
      h2::h2_line s = {"456", "789"};
      h2::h2_line line = "123";
      line.concat_back(s, "red");
      OK(ListOf("123", "\033{red}", "456", "789", "\033{reset}"), line);
   }

   Case(concat_front)
   {
      h2::h2_line s = {"456", "789"};
      h2::h2_line line = "123";
      line.concat_front(s);
      OK(ListOf("456", "789", "123"), line);
   }

   Case(concat_front with style)
   {
      h2::h2_line s = {"456", "789"};
      h2::h2_line line = "123";
      line.concat_front(s, "red");
      OK(ListOf("\033{red}", "456", "789", "\033{reset}", "123"), line);
   }

   Case(samesizify line)
   {
      h2::h2_line e = {"123", "456"};
      h2::h2_line a = {"12345"};
      e.samesizify(a);
      OK(e.width() == a.width());
   }

   Case(samesizify lines)
   {
      h2::h2_lines e = {{"123", "456"}, {"1234"}};
      h2::h2_lines a = {{"12345"}};
      e.samesizify(a);
      OK(e.size() == a.size());
   }
}

SUITE(h2_lines)
{
   Case(max lines length)
   {
      h2::h2_lines lines = {{"123", "456"}, {"1234"}};
      OK(6, lines.width());
   }

   Case(max lines length with color)
   {
      h2::h2_lines lines = {{"123", "\033{red}", "456"}, {"1234"}};
      OK(6, lines.width());
   }

   Case(concat back)
   {
      h2::h2_lines s = {{"abc", "def"}, {"ghi"}};
      h2::h2_lines lines = {{"123", "456"}, {"789"}};

      lines += s;
      OK(ListOf(ListOf("123", "456"), ListOf("789"), ListOf("abc", "def"), ListOf("ghi")), lines);
   }

   Case(concat front lines)
   {
      h2::h2_lines s = {{"abc", "def"}, {"ghi"}};
      h2::h2_lines lines = {{"123", "456"}, {"789"}};

      lines.concat_front(s);
      OK(ListOf(ListOf("abc", "def"), ListOf("ghi"), ListOf("123", "456"), ListOf("789")), lines);
   }

   Case(concat front line)
   {
      h2::h2_line s = {"abc", "def"};
      h2::h2_lines lines = {{"123", "456"}, {"789"}};

      lines.concat_front(s);
      OK(ListOf(ListOf("abc", "def"), ListOf("123", "456"), ListOf("789")), lines);
   }

   Case(foldable)
   {
      h2::h2_lines lines = {{"123", "456"}, {"789"}};
      OK(lines.foldable(10));
   }

   Case(foldable with style)
   {
      h2::h2_lines lines = {{"123", "\033{red}", "456", "\033{reset}"}, {"789"}};
      OK(lines.foldable(10));
   }

   Case(foldable with indent)
   {
      h2::h2_lines lines = {{"123", "456"}, {"   ", "789"}};
      OK(lines.foldable(10));
   }

   Case(fold)
   {
      h2::h2_lines lines = {{"123", "456"}, {"   ", "789"}};
      OK(ListOf("123", "456", "789"), lines.folds());
   }

   Case(sequence)
   {
      h2::h2_lines lines = {{"123", "456"}, {"789"}};
      lines.sequence();
      OK(ListOf(ListOf("\033{dark gray}", "0. ", "\033{reset}", "123", "456"), ListOf("\033{dark gray}", "1. ", "\033{reset}", "789")), lines);
   }

   Case(sequence indent and start from 1)
   {
      h2::h2_lines lines = {{"123", "456"}, {"789"}};
      lines.sequence(2, 1);
      OK(ListOf(ListOf("  ", "\033{dark gray}", "1. ", "\033{reset}", "123", "456"), ListOf("  ", "\033{dark gray}", "2. ", "\033{reset}", "789")), lines);
   }

   Case(samesizify)
   {
      h2::h2_lines line1 = {{"abc", "def"}};
      h2::h2_lines line2 = {{"123", "456"}, {"789"}};

      line2.samesizify(line1);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), line2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), line1);
   }

   Case(samesizify 2)
   {
      h2::h2_lines line1 = {{"abc", "def"}};
      h2::h2_lines line2 = {{"123", "456"}, {"789"}};

      line1.samesizify(line2);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), line2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), line1);
   }
}
