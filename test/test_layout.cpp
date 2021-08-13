#include "../source/h2_unit.cpp"

char* h2_row_tostring(h2::h2_row& row, char* b)
{
   sprintf(b + strlen(b), "[");
   int i = 0;
   for (auto& s : row) {
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

char* h2_rows_tostring(h2::h2_rows& rows, char* b)
{
   sprintf(b + strlen(b), "[");
   int i = 0;
   for (auto& row : rows) {
      if (i++)
         sprintf(b + strlen(b), ",");

      h2_row_tostring(row, b);
   }
   sprintf(b + strlen(b), "]");
   return b;
}

SUITE(h2_layout)
{
   char t1[1024] = {'\0'};

   Case(row break)
   {
      h2::h2_row row = {"123", "456", "  "};
      h2::h2_rows rows = h2::row_break(row, 3);
      OK(ListOf(ListOf("1", "2", "3"), ListOf("4", "5", "6"), ListOf(" ", " ")), rows);
   }

   Case(row break with color)
   {
      h2::h2_row row = {"123", "\033{red}", "456"};
      h2::h2_rows rows = h2::row_break(row, 3);
      OK(ListOf(ListOf("1", "2", "3"), ListOf("\033{red}", "4", "5", "6")), rows);
   }

   Case(merge rows)
   {
      h2::h2_rows left = {{"123", "456"}};
      h2::h2_rows right = {{"12345"}};

      h2::h2_rows rows;
      h2::rows_merge(rows, left, right, 10, 10, 0, 'x', 1);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "4", "5", "6", "    ", "\033{reset}",
                       "\033{dark gray}", " " H2_SP " ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "4", "5", "     ", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}")),
         rows);
   }

   Case(merge rows with wrap)
   {
      h2::h2_rows left = {{"123", "456"}};
      h2::h2_rows right = {{"12345"}};

      h2::h2_rows rows;
      h2::rows_merge(rows, left, right, 5, 5, 0, 'x', 1);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "4", "5", "\033{reset}",
                       "\033{dark gray}", "\\" H2_SP " ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "4", "5", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}"),
                ListOf("\033{reset}", "6", "    ", "\033{reset}",
                       "\033{dark gray}", " " H2_SP " ", "\033{reset}",
                       "\033{reset}", "     ", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}")),
         rows);
   }
}
