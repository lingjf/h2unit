static inline h2_paragraph sentence_break(const h2_sentence& st, size_t width)
{
   h2_paragraph paragraph;
   h2_string current_style;
   h2_sentence wrap;
   size_t length = 0;

   for (auto& word : st) {
      if (h2_color::isctrl(word.c_str())) {  // + - style , issue
         wrap.push_back(word);
         current_style = word;
      } else {
         for (auto& c : word) {
            if (width <= length) {  // terminate line as later as possible
               paragraph.push_back(wrap);
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
   paragraph.push_back(wrap);
   return paragraph;
}

static inline h2_paragraph sentences_merge(const h2_paragraph& left_paragraph, const h2_paragraph& right_paragraph, size_t left_width, size_t right_width, int step, char scale, size_t seq_width)
{
   h2_paragraph paragraph;
   char seq_fmt[32];
   sprintf(seq_fmt, "%%%d%c│ ", (int)seq_width, scale);
   const h2_string left_empty(left_width, ' '), right_empty(right_width, ' ');
   for (size_t i = 0; i < std::max(left_paragraph.size(), right_paragraph.size()); ++i) {
      auto left_wrap_sts = sentence_break(i < left_paragraph.size() ? left_paragraph[i] : left_empty, left_width);
      auto right_wrap_sts = sentence_break(i < right_paragraph.size() ? right_paragraph[i] : right_empty, right_width);
      for (size_t j = 0; j < std::max(left_wrap_sts.size(), right_wrap_sts.size()); ++j) {
         h2_sentence st;
         if (step) {
            if (j == 0)
               st.printf("dark gray", seq_fmt, step * i);
            else
               st.indent(seq_width + 2);
         }
         st += j < left_wrap_sts.size() ? left_wrap_sts[j].brush("reset") : color(left_empty, "reset");
         st.printf("dark gray", j < left_wrap_sts.size() - 1 ? "\\│ " : " │ ");
         st += j < right_wrap_sts.size() ? right_wrap_sts[j].brush("reset") : color(right_empty, "reset");
         st.printf("dark gray", j < right_wrap_sts.size() - 1 ? "\\" : " ");
         paragraph.push_back(st);
      }
   }
   return paragraph;
}

h2_inline h2_paragraph h2_layout::split(const h2_paragraph& left_paragraph, const h2_paragraph& right_paragraph, const char* left_title, const char* right_title, size_t step, char scale, size_t width)
{
   size_t seq_width = number_strlen((unsigned long long)step * std::max(left_paragraph.size(), right_paragraph.size()), scale == 'x' ? 16 : 10);
   size_t valid_width = width - (seq_width + 1 /* "|" */) - 1 /*|*/ - 4 /* spaces */;

   size_t left_width = std::max(left_paragraph.width(), strlen(left_title));
   size_t right_width = std::max(right_paragraph.width(), strlen(right_title));

   if (left_width < valid_width / 2)
      right_width = std::min(valid_width - left_width, right_width);
   else if (right_width < valid_width / 2)
      left_width = std::min(valid_width - right_width, left_width);
   else
      left_width = right_width = valid_width / 2;

   h2_sentence title = gray((step ? h2_string(seq_width + 2, ' ') : "") + h2_string(left_title).center(left_width)) + "   " + gray(h2_string(right_title).center(right_width));
   h2_paragraph page = {title};

   return page += sentences_merge(left_paragraph, right_paragraph, left_width, right_width, step, scale, seq_width);
}

h2_inline h2_paragraph h2_layout::unified(const h2_sentence& up_sentence, const h2_sentence& down_sentence, const char* up_title, const char* down_title, size_t width)
{
   h2_paragraph page;
   h2_sentence up_title_st = gray(up_title) + color("> ", "green");
   h2_sentence down_title_st = gray(down_title) + color("> ", "red");

   h2_paragraph up_paragraph = sentence_break(up_sentence, width - up_title_st.width());
   h2_paragraph down_paragraph = sentence_break(down_sentence, width - down_title_st.width());

   for (size_t i = 0; i < std::max(up_paragraph.size(), down_paragraph.size()); ++i) {
      if (i < up_paragraph.size())
         page.push_back(up_title_st + up_paragraph[i]);
      if (i < down_paragraph.size())
         page.push_back(down_title_st + down_paragraph[i]);
   }
   return page;
}

static inline h2_paragraph prefix_break(const h2_sentence& st, const h2_sentence& title, size_t width)
{
   h2_paragraph paragraph = sentence_break(st, width - title.width());

   for (size_t i = 0; i < paragraph.size(); ++i) {
      if (i == 0)
         paragraph[i] = title + paragraph[i];
      else
         paragraph[i].indent(title.width());
   }
   return paragraph;
}

h2_inline h2_paragraph h2_layout::seperate(const h2_sentence& up_sentence, const h2_sentence& down_sentence, const char* up_title, const char* down_title, size_t width)
{
   h2_sentence up_title_st = gray(up_title) + color("> ", "green");
   h2_sentence down_title_st = gray(down_title) + color("> ", "red");
   h2_paragraph page = prefix_break(up_sentence, up_title_st, width);
   return page += prefix_break(down_sentence, down_title_st, width);
}
