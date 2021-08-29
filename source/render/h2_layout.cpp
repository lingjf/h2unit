#if defined _MSC_VER
#   define H2_SP "|"
#else
#   define H2_SP "│"
#endif

static inline h2_rows row_break(const h2_row& row, unsigned width)
{
   h2_rows rows;
   h2_string current_style;
   h2_row wrap;
   unsigned length = 0;

   for (auto& word : row) {
      if (h2_color::isctrl(word.c_str())) {  // + - style , issue
         wrap.push_back(word);
         current_style = word;
      } else {
         for (auto& c : word) {
            if (width <= length) {  // terminate line as later as possible
               rows.push_back(wrap);
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
   rows.push_back(wrap);
   return rows;
}

static inline void rows_merge(h2_rows& rows, const h2_rows& left_rows, const h2_rows& right_rows, unsigned left_width, unsigned right_width, int step, char scale, int seq_width)
{
   char seq_fmt[32];
   sprintf(seq_fmt, "%%%d%c" H2_SP " ", seq_width, scale);
   const h2_string left_empty(left_width, ' '), right_empty(right_width, ' ');
   for (size_t i = 0; i < std::max(left_rows.size(), right_rows.size()); ++i) {
      auto left_wrap_rows = row_break(i < left_rows.size() ? left_rows[i] : left_empty, left_width);
      auto right_wrap_rows = row_break(i < right_rows.size() ? right_rows[i] : right_empty, right_width);
      for (size_t j = 0; j < std::max(left_wrap_rows.size(), right_wrap_rows.size()); ++j) {
         h2_row row;
         if (step) {
            if (j == 0)
               row.printf("dark gray", seq_fmt, step * i);
            else
               row.indent(seq_width + 2);
         }
         row += j < left_wrap_rows.size() ? left_wrap_rows[j].brush("reset") : color(left_empty, "reset");
         row.printf("dark gray", j < left_wrap_rows.size() - 1 ? "\\" H2_SP " " : " " H2_SP " ");
         row += j < right_wrap_rows.size() ? right_wrap_rows[j].brush("reset") : color(right_empty, "reset");
         row.printf("dark gray", j < right_wrap_rows.size() - 1 ? "\\" : " ");
         rows.push_back(row);
      }
   }
}

h2_inline h2_rows h2_layout::split(const h2_rows& left_rows, const h2_rows& right_rows, const char* left_title, const char* right_title, int step, char scale, unsigned width)
{
   unsigned seq_width = number_strlen(step * std::max(left_rows.size(), right_rows.size()), scale == 'x' ? 16 : 10);
   unsigned valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

   unsigned left_width = std::max(left_rows.width(), 8u); /* at least title width */
   unsigned right_width = std::max(right_rows.width(), 8u);

   if (left_width < valid_width / 2)
      right_width = std::min(valid_width - left_width, right_width);
   else if (right_width < valid_width / 2)
      left_width = std::min(valid_width - right_width, left_width);
   else
      left_width = right_width = valid_width / 2;

   h2_rows rows;
   h2_row title_row = {"\033{reset}", "\033{dark gray}", (step ? h2_string(seq_width + 2, ' ') : ""), h2_string(left_title).center(left_width), "   ", h2_string(right_title).center(right_width), "\033{reset}"};
   rows.push_back(title_row);

   rows_merge(rows, left_rows, right_rows, left_width, right_width, step, scale, seq_width);
   return rows;
}

h2_inline h2_rows h2_layout::unified(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width)
{
   h2_rows rows;

   h2_row up_title_row = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_row down_title_row = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_rows up_rows = row_break(up_row, width - up_title_row.width());
   h2_rows down_rows = row_break(down_row, width - down_title_row.width());

   for (size_t i = 0; i < std::max(up_rows.size(), down_rows.size()); ++i) {
      if (i < up_rows.size())
         rows.push_back(up_title_row + up_rows[i]);
      if (i < down_rows.size())
         rows.push_back(down_title_row + down_rows[i]);
   }

   return rows;
}

static inline h2_rows prefix_break(const h2_row& row, const h2_row& title, unsigned width)
{
   h2_rows rows = row_break(row, width - title.width());

   for (size_t i = 0; i < rows.size(); ++i) {
      if (i == 0)
         rows[i] = title + rows[i];
      else
         rows[i].indent(title.width());
   }
   return rows;
}

h2_inline h2_rows h2_layout::seperate(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width)
{
   h2_row up_title_row = {"\033{dark gray}", up_title, "\033{green}", "> ", "\033{reset}"};
   h2_row down_title_row = {"\033{dark gray}", down_title, "\033{red}", "> ", "\033{reset}"};

   h2_rows rows = prefix_break(up_row, up_title_row, width);
   return rows += prefix_break(down_row, down_title_row, width);
}
