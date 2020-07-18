
static inline h2_lines line_break(const h2_line& line, unsigned width)
{
   h2_lines lines;
   h2_string current_style;
   h2_line wrap;
   unsigned length = 0;

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

static inline unsigned int_width(int maximum, char scale)
{
   char t1[64], t2[64];
   sprintf(t1, "%%%c", scale);
   sprintf(t2, t1, maximum);
   return strlen(t2);
}

static inline void lines_merge(h2_lines& lines, const h2_lines& left_lines, const h2_lines& right_lines, unsigned left_width, unsigned right_width, int step, char scale, int seq_width)
{
   char seq_fmt[32];
   sprintf(seq_fmt, "%%0%d%c│ ", seq_width, scale);
   const h2_line left_empty = {h2_string(left_width, ' ')}, right_empty = {h2_string(right_width, ' ')};
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
         line.concat_back(j < left_wrap_lines.size() ? left_wrap_lines[j] : left_empty, "reset");
         line.printf("dark gray", j < left_wrap_lines.size() - 1 ? "\\│ " : " │ ");
         line.concat_back(j < right_wrap_lines.size() ? right_wrap_lines[j] : right_empty, "reset");
         line.printf("dark gray", j < right_wrap_lines.size() - 1 ? "\\" : " ");
         lines.push_back(line);
      }
   }
}

h2_inline h2_lines h2_layout::split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, int step, char scale, unsigned width)
{
   unsigned seq_width = int_width(step * std::max(left_lines.size(), right_lines.size()), scale);
   unsigned valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

   unsigned left_width = std::max(left_lines.width(), 8u); /* at least title width */
   unsigned right_width = std::max(right_lines.width(), 8u);

   if (left_width < valid_width / 2)
      right_width = std::min(valid_width - left_width, right_width);
   else if (right_width < valid_width / 2)
      left_width = std::min(valid_width - right_width, left_width);
   else
      left_width = right_width = valid_width / 2;

   h2_lines lines;
   h2_line title_line = {"\033{reset}", "\033{dark gray}", (step ? h2_string(seq_width + 2, ' ') : ""), h2_string(left_title).center(left_width), "   ", h2_string(right_title).center(right_width), "\033{reset}"};
   lines.push_back(title_line);

   lines_merge(lines, left_lines, right_lines, left_width, right_width, step, scale, seq_width);
   return lines;
}

h2_inline h2_lines h2_layout::unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, unsigned width)
{
   h2_lines lines;

   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines up_lines = line_break(up_line, width - up_title_line.width());
   h2_lines down_lines = line_break(down_line, width - down_title_line.width());

   for (size_t i = 0; i < std::max(up_lines.size(), down_lines.size()); ++i) {
      if (i < up_lines.size()) {
         up_lines[i].concat_front(up_title_line);
         lines.push_back(up_lines[i]);
      }
      if (i < down_lines.size()) {
         down_lines[i].concat_front(down_title_line);
         lines.push_back(down_lines[i]);
      }
   }

   return lines;
}

static inline h2_lines prefix_break(const h2_line& line, const h2_line& title, unsigned width)
{
   h2_lines lines = line_break(line, width - title.width());

   for (size_t i = 0; i < lines.size(); ++i) {
      if (i == 0)
         lines[i].concat_front(title);
      else
         lines[i].indent(title.width());
   }
   return lines;
}

h2_inline h2_lines h2_layout::seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, unsigned width)
{
   h2_line up_title_line = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_line down_title_line = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_lines lines = prefix_break(up_line, up_title_line, width);
   return lines += prefix_break(down_line, down_title_line, width);
}
