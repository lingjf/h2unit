
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

h2_inline h2_line& h2_line::operator+=(const h2_line& line)
{
   insert(end(), line.begin(), line.end());
   return *this;
}

h2_inline h2_line& h2_line::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
   return *this;
}

h2_inline bool h2_line::enclosed(const char c) const
{
   bool f = false, ff = false, b = false;
   for (auto& word : *this) {
      if (!h2_color::is_ctrl(word.c_str())) {
         if (!ff) f = word.front() == c;
         ff = true;
         b = word.back() == c;
      }
   }

   return f && b;
}

h2_inline h2_line h2_line::gray_quote() const
{
   if (!enclosed('\"') && !enclosed('\'')) return *this;

   h2_line line;
   unsigned w = width();
   unsigned i = 0;

   for (auto& word : *this) {
      if (h2_color::is_ctrl(word.c_str())) {
         line.push_back(word);
      } else {
         h2_string h, m, t;
         for (auto& c : word) {
            if (i == 0) {
               h.push_back(c);
            } else if (i == w - 1) {
               t.push_back(c);
            } else {
               m.push_back(c);
            }
            ++i;
         }
         if (h.size()) line += gray(h);
         if (m.size()) line.push_back(m);
         if (t.size()) line += gray(t);
      }
   }

   return line;
}

h2_inline h2_line h2_line::acronym(int width, int tail) const
{
   h2_line l1;
   for (auto& word : *this) {
      if (h2_color::is_ctrl(word.c_str())) {
         l1.push_back(word);
      } else {
         l1.push_back(word.escape());
      }
   }

   int l1_width = l1.width();
   if (l1_width <= width) return l1;

   h2_line l2;
   int i = 0;
   for (auto& word : l1) {
      if (h2_color::is_ctrl(word.c_str())) {
         l2.push_back(word);
      } else {
         h2_string h, m, t;
         for (auto& c : word) {
            if (i < width - 3 - tail) {
               h.push_back(c);
            } else if (i == width - 3 - tail) {
               m = "...";
            } else if (l1_width - tail <= i) {
               t.push_back(c);
            }
            ++i;
         }
         if (h.size()) l2.push_back(h);
         if (m.size()) l2 += gray(m);
         if (t.size()) l2.push_back(t);
      }
   }

   return l2;
}

h2_inline h2_string h2_line::string() const
{
   h2_string s;
   for (auto& word : *this)
      if (!h2_color::is_ctrl(word.c_str()))
         s += word;
   return s;
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
            sum += word.size();

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

h2_inline h2_string h2_lines::string() const
{
   h2_string s;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace() && !h2_color::is_ctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_lines::sequence(unsigned indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = gray(h2_string("%zu. ", i + start)) + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_lines::samesizify(h2_lines& b)
{
   size_t max_y = std::max(size(), b.size());
   for (size_t i = size(); i < max_y; ++i) push_back(h2_line());
   for (size_t i = b.size(); i < max_y; ++i) b.push_back(h2_line());
}
