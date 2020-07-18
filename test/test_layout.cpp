#include "../source/h2_unit.cpp"

char* h2_line_tostring(h2::h2_line& line, char* b)
{
   sprintf(b + strlen(b), "[");
   int i = 0;
   for (auto& s : line) {
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

char* h2_lines_tostring(h2::h2_lines& lines, char* b)
{
   sprintf(b + strlen(b), "[");
   int i = 0;
   for (auto& line : lines) {
      if (i++)
         sprintf(b + strlen(b), ",");

      h2_line_tostring(line, b);
   }
   sprintf(b + strlen(b), "]");
   return b;
}

SUITE(h2_layout)
{
   char t1[1024] = {'\0'};

   Case(line break)
   {
      h2::h2_line line = {"123", "456", "  "};
      h2::h2_lines lines = h2::line_break(line, 3);
      OK(ListOf(ListOf("1", "2", "3"), ListOf("4", "5", "6"), ListOf(" ", " ")), lines);
   }
   Case(line break with color)
   {
      h2::h2_line line = {"123", "\033{red}", "456"};
      h2::h2_lines lines = h2::line_break(line, 3);
      OK(ListOf(ListOf("1", "2", "3"), ListOf("\033{red}", "4", "5", "6")), lines);
   }

   Case(merge lines)
   {
      h2::h2_lines left = {{"123", "456"}};
      h2::h2_lines right = {{"12345"}};

      h2::h2_lines lines;
      h2::lines_merge(lines, left, right, 10, 10, 0, 1);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "4", "5", "6", "    ", "\033{reset}",
                       "\033{dark gray}", " │ ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "4", "5", "     ", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}")),
         lines);
   }
   Case(merge lines with wrap)
   {
      h2::h2_lines left = {{"123", "456"}};
      h2::h2_lines right = {{"12345"}};

      h2::h2_lines lines;
      h2::lines_merge(lines, left, right, 5, 5, 0, 1);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "4", "5", "\033{reset}",
                       "\033{dark gray}", "\\│ ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "4", "5", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}"),
                ListOf("\033{reset}", "6", "    ", "\033{reset}",
                       "\033{dark gray}", " │ ", "\033{reset}",
                       "\033{reset}", "     ", "\033{reset}",
                       "\033{dark gray}", " ", "\033{reset}")),
         lines);
   }
}
