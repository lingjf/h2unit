static inline h2_lines line_break(const h2_line& line, size_t width)
{
   h2_lines lines;
   h2_string current_style;
   h2_line wrap;

   for (auto& word : line) {
      if (h2_console::isctrl(word.c_str())) {  // + - style , issue
         wrap.push_back(word);
         current_style = word;
         continue;
      }
      for (auto& c : word.disperse()) {
         if (width < wrap.width() + c.width()) {
            lines.push_back(wrap.padding(width - wrap.width()));
            wrap.clear();
            if (current_style.size()) wrap.push_back(current_style);
         }
         wrap.push_back(c);
      }
   }
   lines.push_back(wrap.padding(width - wrap.width()));
   return lines;
}

static inline h2_lines lines_merge(const h2_lines& left_lines, const h2_lines& right_lines, size_t left_width, size_t right_width, int step, char scale, size_t seq_width)
{
   h2_lines lines;
   char seq_fmt[32];
   sprintf(seq_fmt, "%%%d%c┊ ", (int)seq_width, scale);
   const h2_string left_empty(left_width, ' '), right_empty(right_width, ' ');
   for (size_t i = 0; i < std::max(left_lines.size(), right_lines.size()); ++i) {
      auto left_wrap_lines = line_break(i < left_lines.size() ? left_lines[i] : left_empty, left_width);
      auto right_wrap_lines = line_break(i < right_lines.size() ? right_lines[i] : right_empty, right_width);
      for (size_t j = 0; j < std::max(left_wrap_lines.size(), right_wrap_lines.size()); ++j) {
         h2_line line;
         if (step) {
            if (j == 0)
               line.printf("dark gray", seq_fmt, step * i);
            else
               line.indent(seq_width + 2);
         }
         line += j < left_wrap_lines.size() ? left_wrap_lines[j].brush("reset") : color(left_empty, "reset");
         line.printf("dark gray", j < left_wrap_lines.size() - 1 ? "\\│ " : " │ ");
         line += j < right_wrap_lines.size() ? right_wrap_lines[j].brush("reset") : color(right_empty, "reset");
         line.printf("dark gray", j < right_wrap_lines.size() - 1 ? "\\" : " ");
         lines.push_back(line);
      }
   }
   return lines;
}

h2_inline h2_lines h2_layout::split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, size_t step, char scale, size_t width)
{
   size_t seq_width = number_strlen((unsigned long long)step * std::max(left_lines.size(), right_lines.size()), scale == 'x' ? 16 : 10);
   size_t valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

   size_t left_width = std::max(left_lines.width(), strlen(left_title));
   size_t right_width = std::max(right_lines.width(), strlen(right_title));

   if (left_width < valid_width / 2)
      right_width = std::min(valid_width - left_width, right_width);
   else if (right_width < valid_width / 2)
      left_width = std::min(valid_width - right_width, left_width);
   else
      left_width = right_width = valid_width / 2;

   h2_line title = gray((step ? h2_string(seq_width + 2, ' ') : "") + h2_string(left_title).center(left_width)) + "   " + gray(h2_string(right_title).center(right_width));
   h2_lines lines = {title};

   return lines += lines_merge(left_lines, right_lines, left_width, right_width, step, scale, seq_width);
}

h2_inline h2_lines h2_layout::unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
{
   h2_lines lines;
   h2_line up_title_line = gray(up_title) + color("> ", "green");
   h2_line down_title_line = gray(down_title) + color("> ", "red");

   h2_lines up_lines = line_break(up_line, width - up_title_line.width());
   h2_lines down_lines = line_break(down_line, width - down_title_line.width());

   for (size_t i = 0; i < std::max(up_lines.size(), down_lines.size()); ++i) {
      if (i < up_lines.size()) lines.push_back(up_title_line + up_lines[i]);
      if (i < down_lines.size()) lines.push_back(down_title_line + down_lines[i]);
   }
   return lines;
}

static inline h2_lines prefix_break(const h2_line& line, const h2_line& title, size_t width)
{
   h2_lines lines = line_break(line, width - title.width());

   for (size_t i = 0; i < lines.size(); ++i) {
      if (i == 0)
         lines[i] = title + lines[i];
      else
         lines[i].indent(title.width());
   }
   return lines;
}

h2_inline h2_lines h2_layout::seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width)
{
   h2_line up_title_line = gray(up_title) + color("> ", "green");
   h2_line down_title_line = gray(down_title) + color("> ", "red");
   h2_lines lines = prefix_break(up_line, up_title_line, width);
   return lines += prefix_break(down_line, down_title_line, width);
}
