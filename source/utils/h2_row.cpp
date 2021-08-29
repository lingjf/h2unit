h2_inline unsigned h2_row::width(bool ignore_indent) const
{
   unsigned w = 0;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         if (!ignore_indent || !word.isspace())
            w += word.size();
   return w;
}

h2_inline h2_row& h2_row::indent(int n, const char c)
{
   insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_row& h2_row::padding(int n, const char c)
{
   push_back(h2_string(n, c));
   return *this;
}

h2_inline h2_row& h2_row::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_row& h2_row::operator+=(const h2_row& row)
{
   insert(end(), row.begin(), row.end());
   return *this;
}

h2_inline h2_row& h2_row::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
   return *this;
}

h2_inline bool h2_row::enclosed(const char c) const
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

h2_inline h2_row h2_row::gray_quote() const
{
   if (!enclosed('\"') && !enclosed('\'')) return *this;

   h2_row row;
   unsigned w = width();
   unsigned i = 0;

   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         row.push_back(word);
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
         if (h.size()) row += gray(h);
         if (m.size()) row.push_back(m);
         if (t.size()) row += gray(t);
      }
   }

   return row;
}

h2_inline h2_row h2_row::acronym(int width, int tail) const
{
   h2_row r1;
   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         r1.push_back(word);
      } else {
         r1.push_back(word.escape());
      }
   }

   int r1_width = r1.width();
   if (r1_width <= width) return r1;

   h2_row r2;
   int i = 0;
   for (auto& word : r1) {
      if (h2_color::isctrl(word.c_str())) {
         r2.push_back(word);
      } else {
         h2_string h, m, t;
         for (auto& c : word) {
            if (i < width - 3 - tail) {
               h.push_back(c);
            } else if (i == width - 3 - tail) {
               m = "...";
            } else if (r1_width - tail <= i) {
               t.push_back(c);
            }
            ++i;
         }
         if (h.size()) r2.push_back(h);
         if (m.size()) r2 += gray(m);
         if (t.size()) r2.push_back(t);
      }
   }

   return r2;
}

h2_inline h2_string h2_row::string() const
{
   h2_string s;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         s += word;
   return s;
}

h2_inline void h2_row::samesizify(h2_row& b)
{
   int w = width(), b_w = b.width();
   padding(std::max(w, b_w) - w);
   b.padding(std::max(w, b_w) - b_w);
}

h2_inline h2_rows& h2_rows::operator+=(const h2_rows& rows)
{
   insert(end(), rows.begin(), rows.end());
   return *this;
}

h2_inline unsigned h2_rows::width() const
{
   unsigned m = 0;
   for (auto& row : *this)
      m = std::max(m, row.width());
   return m;
}

h2_inline bool h2_rows::foldable(unsigned width)
{
   int sum = 0;
   for (auto& row : *this)
      for (auto& word : row)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.size();

   return sum < width;
}

h2_inline h2_row h2_rows::folds()
{
   h2_row folded_row;
   for (auto& row : *this)
      for (auto& word : row)
         if (!word.isspace())  // drop indent
            folded_row.push_back(word);
   return folded_row;
}

h2_inline h2_string h2_rows::string() const
{
   h2_string s;
   for (auto& row : *this)
      for (auto& word : row)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_rows::sequence(unsigned indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = gray(h2_string("%zu. ", i + start)) + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_rows::samesizify(h2_rows& b)
{
   size_t max_y = std::max(size(), b.size());
   for (size_t i = size(); i < max_y; ++i) push_back(h2_row());
   for (size_t i = b.size(); i < max_y; ++i) b.push_back(h2_row());
}
