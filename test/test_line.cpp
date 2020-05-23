#include "../source/h2_unit.cpp"

SUITE(h2_line)
{
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
      OK(6, lines.max_width());
   }

   Case(max lines length with color)
   {
      h2::h2_lines lines = {{"123", "\033{red}", "456"}, {"1234"}};
      OK(6, lines.max_width());
   }
}