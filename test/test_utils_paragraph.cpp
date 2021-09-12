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
   Case(max sts length)
   {
      h2::h2_paragraph sts = {{"123", "456"}, {"1234"}};
      OK(6, sts.width());
   }

   Case(max sts length with color)
   {
      h2::h2_paragraph sts = {{"123", "\033{red}", "456"}, {"1234"}};
      OK(6, sts.width());
   }

   Case(+=)
   {
      h2::h2_paragraph s = {{"abc", "def"}, {"ghi"}};
      h2::h2_paragraph sts = {{"123", "456"}, {"789"}};

      sts += s;
      OK(ListOf(ListOf("123", "456"), ListOf("789"), ListOf("abc", "def"), ListOf("ghi")), sts);
   }

   Case(concat front sentences)
   {
      h2::h2_paragraph s = {{"abc", "def"}, {"ghi"}};
      h2::h2_paragraph sts = {{"123", "456"}, {"789"}};

      concat_front(sts, s);
      OK(ListOf(ListOf("abc", "def"), ListOf("ghi"), ListOf("123", "456"), ListOf("789")), sts);
   }

   Case(concat front sentence)
   {
      h2::h2_sentence s = {"abc", "def"};
      h2::h2_paragraph sts = {{"123", "456"}, {"789"}};

      concat_front(sts, s);
      OK(ListOf(ListOf("abc", "def"), ListOf("123", "456"), ListOf("789")), sts);
   }

   Case(foldable)
   {
      h2::h2_paragraph sts = {{"123", "456"}, {"789"}};
      OK(sts.foldable(10));
   }

   Case(foldable with style)
   {
      h2::h2_paragraph sts = {{"123", "\033{red}", "456", "\033{reset}"}, {"789"}};
      OK(sts.foldable(10));
   }

   Case(foldable with indent)
   {
      h2::h2_paragraph sts = {{"123", "456"}, {"   ", "789"}};
      OK(sts.foldable(10));
   }

   Case(fold)
   {
      h2::h2_paragraph sts = {{"123", "456"}, {"   ", "789"}};
      OK(ListOf("123", "456", "789"), sts.folds());
   }

   Case(sequence)
   {
      h2::h2_paragraph sts = {{"123", "456"}, {"789"}};
      sts.sequence();
      OK(ListOf(ListOf("\033{+dark gray}", "0. ", "\033{-dark gray}", "123", "456"), ListOf("\033{+dark gray}", "1. ", "\033{-dark gray}", "789")), sts);
   }

   Case(sequence indent and start from 1)
   {
      h2::h2_paragraph sts = {{"123", "456"}, {"789"}};
      sts.sequence(2, 1);
      OK(ListOf(ListOf("  ", "\033{+dark gray}", "1. ", "\033{-dark gray}", "123", "456"), ListOf("  ", "\033{+dark gray}", "2. ", "\033{-dark gray}", "789")), sts);
   }

   Case(samesizify)
   {
      h2::h2_paragraph sts1 = {{"abc", "def"}};
      h2::h2_paragraph sts2 = {{"123", "456"}, {"789"}};

      sts2.samesizify(sts1);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), sts2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), sts1);
   }

   Case(samesizify 2)
   {
      h2::h2_paragraph sts1 = {{"abc", "def"}};
      h2::h2_paragraph sts2 = {{"123", "456"}, {"789"}};

      sts1.samesizify(sts2);
      OK(ListOf(ListOf("123", "456"), ListOf("789")), sts2);
      OK(ListOf(ListOf("abc", "def"), ListOf()), sts1);
   }
}
