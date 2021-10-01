h2_inline size_t h2_line::width(bool ignore_indent) const
{
   size_t w = 0;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         if (!ignore_indent || !word.isspace())
            w += word.width();
   return w;
}

h2_inline h2_line& h2_line::indent(size_t n, const char c)
{
   if (n) insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_line& h2_line::padding(size_t n, const char c)
{
   if (n) push_back(h2_string(n, c));
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
      if (!h2_color::isctrl(word.c_str())) {
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
   size_t i = 0, w = width();

   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         line.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i == 0) {
            h.append(c.c_str());
         } else if (i == w - 1) {
            t.append(c.c_str());
         } else {
            m.append(c.c_str());
         }
         i += c.width();
      }
      if (h.size()) line += gray(h);
      if (m.size()) line.push_back(m);
      if (t.size()) line += gray(t);
   }

   return line;
}

h2_inline h2_line h2_line::acronym(size_t width, size_t tail) const
{
   h2_line line1, line2;
   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str()))
         line1.push_back(word);
      else
         line1.push_back(word.escape());
   }

   size_t i = 0, line1_width = line1.width();
   if (line1_width <= width) return line1;

   for (auto& word : line1) {
      if (h2_color::isctrl(word.c_str())) {
         line2.push_back(word);
         continue;
      }
      h2_string h, m, t;
      for (auto& c : word.disperse()) {
         if (i < width - 3 - tail) {
            h.append(c.c_str());
         } else if (i == width - 3 - tail) {
            m = "...";
         } else if (line1_width - tail <= i) {
            t.append(c.c_str());
         }
         i += c.width();
      }
      if (h.size()) line2.push_back(h);
      if (m.size()) line2 += gray(m);
      if (t.size()) line2.push_back(t);
   }

   return line2;
}

h2_inline h2_string h2_line::string() const
{
   h2_string s;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         s += word;
   return s;
}

h2_inline void h2_line::samesizify(h2_line& a, h2_line& b)
{
   size_t a_w = a.width(), b_w = b.width();
   a.padding(std::max(a_w, b_w) - a_w);
   b.padding(std::max(a_w, b_w) - b_w);
}

///// h2_lines
h2_inline h2_lines& h2_lines::operator+=(const h2_lines& lines)
{
   insert(end(), lines.begin(), lines.end());
   return *this;
}

h2_inline size_t h2_lines::width() const
{
   size_t m = 0;
   for (auto& line : *this)
      m = std::max(m, line.width());
   return m;
}

h2_inline bool h2_lines::foldable(size_t width) const
{
   size_t sum = 0;
   for (auto& line : *this)
      for (auto& word : line)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.size();

   return sum < width;
}

h2_inline h2_line h2_lines::folds() const
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
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_lines::sequence(size_t indent, size_t start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = gray(h2_string("%d. ", (int)(i + start))) + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_lines::samesizify(h2_lines& a, h2_lines& b)
{
   auto size = std::max(a.size(), b.size());
   for (auto i = a.size(); i < size; ++i) a.push_back(h2_line());
   for (auto i = b.size(); i < size; ++i) b.push_back(h2_line());
}
