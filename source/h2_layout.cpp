
static inline h2_lines line_break(h2_line& line, int width)
{
   h2_lines lines;
   h2_string current_style;
   h2_line wrap;
   int length = 0;

   for (auto& word : line) {
      if (h2_color::is_ctrl(word.c_str())) {  // + - style , issue
         wrap.push_back(word);
         current_style = word;
      } else {
         for (auto& c : word) {
            if (width <= length) {  // terminate line as later as possible
               lines.push_back(wrap);
               wrap.clear();
               length = 0;
               if (current_style.size()) wrap.push_back(current_style);
            }
            wrap.push_back(h2_string(1, c));
            ++length;
         }
      }
   }
   if (length < width) wrap.push_back(h2_string(width - length, ' '));
   lines.push_back(wrap);
   return lines;
}

static inline h2_lines lines_merge(h2_lines& left_lines, h2_lines& right_lines, int width)
{
   left_lines.samesizify(right_lines);
   h2_lines lines;
   for (size_t i = 0; i < std::max(left_lines.size(), right_lines.size()); ++i) {
      left_lines[i].samesizify(right_lines[i]);
      auto left_wrap_lines = line_break(left_lines[i], width - 2);
      auto right_wrap_lines = line_break(right_lines[i], width - 2);
      for (size_t j = 0; j < std::max(left_wrap_lines.size(), right_wrap_lines.size()); ++j) {
         h2_line line;
         line.push_back("\033{reset}");
         line.concat_back("", left_wrap_lines[j]);
         line.push_back("\033{reset}");
         line.printf("dark gray", j == left_wrap_lines.size() - 1 ? "  │ " : " \\│ ");
         line.concat_back("", right_wrap_lines[j]);
         line.push_back("\033{reset}");
         line.printf("dark gray", j == right_wrap_lines.size() - 1 ? "  " : " \\");

         lines.push_back(line);
      }
   }
   return lines;
}

h2_inline h2_lines h2_layout::split(h2_lines& left_lines, h2_lines& right_lines, const char* left_title, const char* right_title)
{
   int max_line_width = std::max(left_lines.max_length(), right_lines.max_length());
   int half_width = std::min(h2_term_size() / 2 - 4, std::max(max_line_width + 3, 30));

   h2_line left_title_line = {"\033{dark gray}", h2_string(left_title).center(half_width - 3), "\033{reset}"};
   h2_line right_title_line = {"\033{dark gray}", h2_string(right_title).center(half_width - 3), "\033{reset}"};

   left_lines.insert(left_lines.begin(), left_title_line);
   right_lines.insert(right_lines.begin(), right_title_line);

   h2_lines lines = lines_merge(left_lines, right_lines, half_width);
   return lines;
}

h2_inline h2_lines h2_layout::unified(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title)
{
   h2_lines lines;

   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines up_lines = line_break(up_line, h2_term_size() - 12);
   h2_lines down_lines = line_break(down_line, h2_term_size() - 12);

   for (size_t i = 0; i < std::max(up_lines.size(), down_lines.size()); ++i) {
      if (i < up_lines.size()) {
         up_lines[i].concat_front("", up_title_line);
         lines.push_back(up_lines[i]);
      }
      if (i < down_lines.size()) {
         down_lines[i].concat_front("", down_title_line);
         lines.push_back(down_lines[i]);
      }
   }

   return lines;
}

h2_inline h2_lines h2_layout::seperate(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title)
{
   h2_lines lines;

   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines up_lines = line_break(up_line, h2_term_size() - 12);
   h2_lines down_lines = line_break(down_line, h2_term_size() - 12);

   for (size_t i = 0; i < up_lines.size(); ++i) {
      if (i == 0)
         up_lines[i].concat_front("", up_title_line);
      else
         up_lines[i].indent(12);
      lines.push_back(up_lines[i]);
   }
   for (size_t i = 0; i < down_lines.size(); ++i) {
      if (i == 0)
         down_lines[i].concat_front("", down_title_line);
      else
         down_lines[i].indent(12);
      lines.push_back(down_lines[i]);
   }
   return lines;
}
