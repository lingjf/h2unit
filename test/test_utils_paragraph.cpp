#include "../source/h2_unit.cpp"

h2::h2_paragraph& concat_front(h2::h2_paragraph& thus, const h2::h2_paragraph& sts)
{
   thus.insert(thus.begin(), sts.begin(), sts.end());
   return thus;
}

h2::h2_paragraph& concat_front(h2::h2_paragraph& thus, const h2::h2_sentence& st)
{
   thus.insert(thus.begin(), st);
   return thus;
}

SUITE(h2_paragraph)
{
   Case(max paragraph length)
   {
      h2::h2_paragraph t = {{"123", "456"}, {"1234"}};
      OK(6, t.width());
   }

   Case(max paragraph length with color)
   {
      h2::h2_paragraph t = {{"123", "\033{red}", "456"}, {"1234"}};
      OK(6, t.width());
   }

   Case(+=)
   {
      h2::h2_paragraph p1 = {{"abc", "def"}, {"ghi"}};
      h2::h2_paragraph p2 = {{"123", "456"}, {"789"}};

      p2 += p1;
      OK(ListOf(ListOf("123", "456"), ListOf("789"), ListOf("abc", "def"), ListOf("ghi")), p2);
   }

   Case(concat front paragraph)
   {
      h2::h2_paragraph p1 = {{"abc", "def"}, {"ghi"}};
      h2::h2_paragraph p2 = {{"123", "456"}, {"789"}};

      concat_front(p2, p1);
      OK(ListOf(ListOf("abc", "def"), ListOf("ghi"), ListOf("123", "456"), ListOf("789")), p2);
   }

   Case(concat front sentence)
   {
      h2::h2_sentence p1 = {"abc", "def"};
      h2::h2_paragraph p2 = {{"123", "456"}, {"789"}};

      concat_front(p2, p1);
      OK(ListOf(ListOf("abc", "def"), ListOf("123", "456"), ListOf("789")), p2);
   }

   Case(foldable)
   {
      h2::h2_paragraph t = {{"123", "456"}, {"789"}};
      OK(t.foldable(10));
   }

   Case(foldable with style)
   {
      h2::h2_paragraph t = {{"123", "\033{red}", "456", "\033{reset}"}, {"789"}};
      OK(t.foldable(10));
   }

   Case(foldable with indent)
   {
      h2::h2_paragraph t = {{"123", "456"}, {"   ", "789"}};
      OK(t.foldable(10));
   }

   Case(fold)
   {
      h2::h2_paragraph t = {{"123", "456"}, {"   ", "789"}};
      OK(ListOf("123", "456", "789"), t.folds());
   }

   Case(sequence)
   {
      h2::h2_paragraph t = {{"123", "456"}, {"789"}};
      t.sequence();
      OK(ListOf(ListOf("\033{+dark gray}", "0. ", "\033{-dark gray}", "123", "456"), ListOf("\033{+dark gray}", "1. ", "\033{-dark gray}", "789")), t);
   }

   Case(sequence indent and start from 1)
   {
      h2::h2_paragraph t = {{"123", "456"}, {"789"}};
      t.sequence(2, 1);
      OK(ListOf(ListOf("  ", "\033{+dark gray}", "1. ", "\033{-dark gray}", "123", "456"), ListOf("  ", "\033{+dark gray}", "2. ", "\033{-dark gray}", "789")), t);
   }

   Case(samesizify)
   {
      h2::h2_paragraph p1 = {{"abc", "def"}};
      h2::h2_paragraph p2 = {{"123", "456"}, {"789"}};

      h2::h2_paragraph::samesizify(p1, p2);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), p2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), p1);
   }

   Case(samesizify 2)
   {
      h2::h2_paragraph p1 = {{"abc", "def"}};
      h2::h2_paragraph p2 = {{"123", "456"}, {"789"}};

      h2::h2_paragraph::samesizify(p1, p2);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), p2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), p1);
   }
}
