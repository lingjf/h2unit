h2_inline unsigned h2_sentence::width(bool ignore_indent) const
{
   unsigned w = 0;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         if (!ignore_indent || !word.isspace())
            w += word.size();
   return w;
}

h2_inline h2_sentence& h2_sentence::indent(int n, const char c)
{
   insert(begin(), h2_string(n, c));
   return *this;
}

h2_inline h2_sentence& h2_sentence::padding(int n, const char c)
{
   push_back(h2_string(n, c));
   return *this;
}

h2_inline h2_sentence& h2_sentence::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) push_back("\033{" + h2_string(style) + "}");
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   push_back(alloca_str);
   if (style && strlen(style)) push_back("\033{reset}");
   return *this;
}

h2_inline h2_sentence& h2_sentence::operator+=(const h2_sentence& sentence)
{
   insert(end(), sentence.begin(), sentence.end());
   return *this;
}

h2_inline h2_sentence& h2_sentence::brush(const char* style)
{
   if (style && strlen(style)) {
      insert(begin(), "\033{" + h2_string(style) + "}");
      push_back("\033{reset}");
   }
   return *this;
}

h2_inline bool h2_sentence::enclosed(const char c) const
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

h2_inline h2_sentence h2_sentence::gray_quote() const
{
   if (!enclosed('\"') && !enclosed('\'')) return *this;

   h2_sentence sentence;
   unsigned w = width();
   unsigned i = 0;

   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         sentence.push_back(word);
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
         if (h.size()) sentence += gray(h);
         if (m.size()) sentence.push_back(m);
         if (t.size()) sentence += gray(t);
      }
   }

   return sentence;
}

h2_inline h2_sentence h2_sentence::acronym(int width, int tail) const
{
   h2_sentence s1;
   for (auto& word : *this) {
      if (h2_color::isctrl(word.c_str())) {
         s1.push_back(word);
      } else {
         s1.push_back(word.escape());
      }
   }

   int r1_width = s1.width();
   if (r1_width <= width) return s1;

   h2_sentence s2;
   int i = 0;
   for (auto& word : s1) {
      if (h2_color::isctrl(word.c_str())) {
         s2.push_back(word);
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
         if (h.size()) s2.push_back(h);
         if (m.size()) s2 += gray(m);
         if (t.size()) s2.push_back(t);
      }
   }

   return s2;
}

h2_inline h2_string h2_sentence::string() const
{
   h2_string s;
   for (auto& word : *this)
      if (!h2_color::isctrl(word.c_str()))
         s += word;
   return s;
}

h2_inline void h2_sentence::samesizify(h2_sentence& b)
{
   int w = width(), b_w = b.width();
   padding(std::max(w, b_w) - w);
   b.padding(std::max(w, b_w) - b_w);
}
