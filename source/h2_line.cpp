
h2_inline unsigned h2_line::width(bool ignore_indent) const
{
   unsigned w = 0;
   for (auto& word : *this)
      if (!h2_color::is_ctrl(word.c_str()))
         if (!ignore_indent || !word.isspace())
            w += word.size();
   return w;
}

h2_inline h2_line& h2_line::indent(int n, const char c)
{
   insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::padding(int n, const char c)
{
   push_back(h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_line& h2_line::concat_back(const h2_line& line, const char* style)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   insert(end(), line.begin(), line.end());
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_line& h2_line::concat_front(const h2_line& line_or_word, const char* style)
{
   if (style && strlen(style)) insert(begin(), "\033{reset}");
   insert(begin(), line_or_word.begin(), line_or_word.end());
   if (style && strlen(style)) insert(begin(), "\033{" + h2_string(style) + "}");
   return *this;
}

h2_inline void h2_line::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
}

h2_inline void h2_line::samesizify(h2_line& b)
{
   int w = width(), b_w = b.width();
   padding(std::max(w, b_w) - w);
   b.padding(std::max(w, b_w) - b_w);
}

h2_inline h2_lines& h2_lines::operator+=(const h2_lines& lines)
{
   insert(end(), lines.begin(), lines.end());
   return *this;
}

h2_inline h2_lines& h2_lines::concat_front(const h2_lines& lines)
{
   insert(begin(), lines.begin(), lines.end());
   return *this;
}

h2_inline h2_lines& h2_lines::concat_front(const h2_line& line)
{
   insert(begin(), line);
   return *this;
}

h2_inline unsigned h2_lines::width() const
{
   unsigned m = 0;
   for (auto& line : *this)
      m = std::max(m, line.width());
   return m;
}

h2_inline bool h2_lines::foldable(unsigned width)
{
   int sum = 0;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace() && !h2_color::is_ctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.length();

   return sum < width;
}

h2_inline h2_line h2_lines::folds()
{
   h2_line folded_line;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace())  // drop indent
            folded_line.push_back(word);
   return folded_line;
}

h2_inline void h2_lines::sequence(unsigned indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      h2_line t;
      t.printf("dark gray", "%lu. ", i + start);
      at(i).concat_front(t);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_lines::samesizify(h2_lines& b)
{
   int max_y = std::max(size(), b.size());
   for (size_t i = size(); i < max_y; ++i) push_back(h2_line());
   for (size_t i = b.size(); i < max_y; ++i) b.push_back(h2_line());
}
