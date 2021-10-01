#include "../source/h2_unit.cpp"

h2::h2_lines& concat_front(h2::h2_lines& thus, const h2::h2_lines& lines)
{
   thus.insert(thus.begin(), lines.begin(), lines.end());
   return thus;
}

h2::h2_lines& concat_front(h2::h2_lines& thus, const h2::h2_line& line)
{
   thus.insert(thus.begin(), line);
   return thus;
}

SUITE(h2_lines)
{
   Case(max lines length)
   {
      h2::h2_lines t = {{"123", "456"}, {"1234"}};
      OK(6, t.width());
   }

   Case(max lines length with color)
   {
      h2::h2_lines t = {{"123", "\033{red}", "456"}, {"1234"}};
      OK(6, t.width());
   }

   Case(+=)
   {
      h2::h2_lines p1 = {{"abc", "def"}, {"ghi"}};
      h2::h2_lines p2 = {{"123", "456"}, {"789"}};

      p2 += p1;
      OK(ListOf(ListOf("123", "456"), ListOf("789"), ListOf("abc", "def"), ListOf("ghi")), p2);
   }

   Case(concat front lines)
   {
      h2::h2_lines p1 = {{"abc", "def"}, {"ghi"}};
      h2::h2_lines p2 = {{"123", "456"}, {"789"}};

      concat_front(p2, p1);
      OK(ListOf(ListOf("abc", "def"), ListOf("ghi"), ListOf("123", "456"), ListOf("789")), p2);
   }

   Case(concat front line)
   {
      h2::h2_line p1 = {"abc", "def"};
      h2::h2_lines p2 = {{"123", "456"}, {"789"}};

      concat_front(p2, p1);
      OK(ListOf(ListOf("abc", "def"), ListOf("123", "456"), ListOf("789")), p2);
   }

   Case(foldable)
   {
      h2::h2_lines t = {{"123", "456"}, {"789"}};
      OK(t.foldable(10));
   }

   Case(foldable with style)
   {
      h2::h2_lines t = {{"123", "\033{red}", "456", "\033{reset}"}, {"789"}};
      OK(t.foldable(10));
   }

   Case(foldable with indent)
   {
      h2::h2_lines t = {{"123", "456"}, {"   ", "789"}};
      OK(t.foldable(10));
   }

   Case(fold)
   {
      h2::h2_lines t = {{"123", "456"}, {"   ", "789"}};
      OK(ListOf("123", "456", "789"), t.folds());
   }

   Case(sequence)
   {
      h2::h2_lines t = {{"123", "456"}, {"789"}};
      t.sequence();
      OK(ListOf(ListOf("\033{+dark gray}", "0. ", "\033{-dark gray}", "123", "456"), ListOf("\033{+dark gray}", "1. ", "\033{-dark gray}", "789")), t);
   }

   Case(sequence indent and start from 1)
   {
      h2::h2_lines t = {{"123", "456"}, {"789"}};
      t.sequence(2, 1);
      OK(ListOf(ListOf("  ", "\033{+dark gray}", "1. ", "\033{-dark gray}", "123", "456"), ListOf("  ", "\033{+dark gray}", "2. ", "\033{-dark gray}", "789")), t);
   }

   Case(samesizify)
   {
      h2::h2_lines p1 = {{"abc", "def"}};
      h2::h2_lines p2 = {{"123", "456"}, {"789"}};

      h2::h2_lines::samesizify(p1, p2);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), p2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), p1);
   }

   Case(samesizify 2)
   {
      h2::h2_lines p1 = {{"abc", "def"}};
      h2::h2_lines p2 = {{"123", "456"}, {"789"}};

      h2::h2_lines::samesizify(p1, p2);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), p2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), p1);
   }
}
