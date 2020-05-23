#include "../source/h2_unit.cpp"

SUITE(h2_layout)
{
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

      h2::h2_lines lines = h2::lines_merge(left, right, 10);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "4", "5", "6", "  ", "\033{reset}",
                       "\033{dark gray}", "  │ ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "4", "5", " ", "  ", "\033{reset}",
                       "\033{dark gray}", "  ", "\033{reset}")),
         lines);
   }
   Case(merge lines with wrap)
   {
      h2::h2_lines left = {{"123", "456"}};
      h2::h2_lines right = {{"12345"}};

      h2::h2_lines lines = h2::lines_merge(left, right, 5);

      OK(ListOf(ListOf("\033{reset}", "1", "2", "3", "\033{reset}",
                       "\033{dark gray}", " \\│ ", "\033{reset}",
                       "\033{reset}", "1", "2", "3", "\033{reset}",
                       "\033{dark gray}", " \\", "\033{reset}"),
                ListOf("\033{reset}", "4", "5", "6", "\033{reset}",
                       "\033{dark gray}", "  │ ", "\033{reset}",
                       "\033{reset}", "4", "5", " ", "\033{reset}",
                       "\033{dark gray}", "  ", "\033{reset}")),
         lines);
   }
}
