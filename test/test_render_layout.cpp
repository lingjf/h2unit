#include "../source/h2_unit.cpp"

char* h2_sentence_tostring(h2::h2_sentence& st, char* b)
{
   sprintf(b + strlen(b), "[");
   int i = 0;
   for (auto& s : st) {
      if (i++)
         sprintf(b + strlen(b), ",");

      sprintf(b + strlen(b), "\'");

      for (auto& c : s) {
         if (c == '\033') {
            sprintf(b + strlen(b), "\\033");
         } else {
            sprintf(b + strlen(b), "%c", c);
         }
      }
      sprintf(b + strlen(b), "\'");
   }
   sprintf(b + strlen(b), "]");
   return b;
}

char* h2_paragraph_tostring(h2::h2_paragraph& paragraph, char* b)
{
   sprintf(b + strlen(b), "[");
   int i = 0;
   for (auto& st : paragraph) {
      if (i++)
         sprintf(b + strlen(b), ",");

      h2_sentence_tostring(st, b);
   }
   sprintf(b + strlen(b), "]");
   return b;
}

SUITE(h2_layout)
{
   char t1[1024] = {'\0'};

   Case(sentence break)
   {
      h2::h2_sentence st = {"123", "456", "  "};
      h2::h2_paragraph paragraph = h2::sentence_break(st, 3);
      OK(ListOf(ListOf("1", "2", "3"), ListOf("4", "5", "6"), ListOf(" ", " ")), paragraph);
   }

   Case(sentence break with color)
   {
      h2::h2_sentence st = {"123", "\033{red}", "456"};
      h2::h2_paragraph paragraph = h2::sentence_break(st, 3);
      OK(ListOf(ListOf("1", "2", "3"), ListOf("\033{red}", "4", "5", "6")), paragraph);
   }

   Case(merge paragraph)
   {
      h2::h2_paragraph left = {{"123", "456"}};
      h2::h2_paragraph right = {{"12345"}};

      h2::h2_paragraph paragraph = h2::sentences_merge(left, right, 10, 10, 0, 'x', 1);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "4", "5", "6", "    ", "\033{reset}",
                       "\033{dark gray}", " │ ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "4", "5", "     ", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}")),
         paragraph);
   }

   Case(merge paragraph with wrap)
   {
      h2::h2_paragraph left = {{"123", "456"}};
      h2::h2_paragraph right = {{"12345"}};

      h2::h2_paragraph paragraph = h2::sentences_merge(left, right, 5, 5, 0, 'x', 1);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "4", "5", "\033{reset}",
                       "\033{dark gray}", "\\│ ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "4", "5", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}"),
                ListOf("\033{reset}", "6", "    ", "\033{reset}",
                       "\033{dark gray}", " │ ", "\033{reset}",
                       "\033{reset}", "     ", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}")),
         paragraph);
   }
}
