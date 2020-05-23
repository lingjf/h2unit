
h2_inline int h2_line::width() const
{
   int w = 0;
   for (auto& word : *this)
      if (!h2_color::is_ctrl(word.c_str()))
         w += word.size();
   return w;
}

h2_inline void h2_line::indent(int n_space)
{
   insert(begin(), h2_string(n_space, ' '));
}

h2_inline void h2_line::padding(int n_space)
{
   push_back(h2_string(n_space, ' '));
}

h2_inline void h2_line::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
}

h2_inline void h2_line::concat_back(const char* style, h2_line& line)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   insert(end(), line.begin(), line.end());
   if (style && strlen(style)) push_back("\033{reset}");
}

h2_inline void h2_line::concat_front(const char* style, h2_line& line)
{
   if (style && strlen(style)) insert(begin(), "\033{reset}");
   insert(begin(), line.begin(), line.end());
   if (style && strlen(style)) insert(begin(), "\033{" + h2_string(style) + "}");
}

h2_inline void h2_line::fold(h2_vector<h2_line>& lines)
{
   for (size_t i = 0; i < lines.size(); i++)
      for (auto& word : lines[i])
         if (!word.isspace())  // drop indent
            push_back(word);
}

h2_inline void h2_line::samesizify(h2_line& b)
{
   int w = width(), b_w = b.width();
   padding(std::max(w, b_w) - w);
   b.padding(std::max(w, b_w) - b_w);
}

h2_inline void h2_lines::concat_back(h2_lines& lines)
{
   insert(end(), lines.begin(), lines.end());
}
h2_inline void h2_lines::concat_front(h2_lines& lines)
{
   insert(begin(), lines.begin(), lines.end());
}

h2_inline int h2_lines::max_width() const
{
   int m = 0;
   for (size_t i = 0; i < size(); ++i)
      m = std::max(m, at(i).width());
   return m;
}

h2_inline void h2_lines::samesizify(h2_lines& b)
{
   int max_y = std::max(size(), b.size());
   for (size_t i = size(); i < max_y; ++i) push_back(h2_line());
   for (size_t i = b.size(); i < max_y; ++i) b.push_back(h2_line());
}

h2_inline bool h2_lines::foldable()
{
   int sum = 0;
   for (size_t i = 0; i < size(); ++i)
      for (auto& word : at(i))
         if (!word.isspace())  // ignore indent
            sum += word.length();

   return sum < 20;
}

h2_inline void h2_lines::sequence(int indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      h2_line sequence;
      sequence.printf("dark gray", "%lu. ", i + start);
      at(i).concat_front("", sequence);
      at(i).indent(indent);
   }
}
