h2_inline h2_paragraph& h2_paragraph::operator+=(const h2_paragraph& paragraph)
{
   insert(end(), paragraph.begin(), paragraph.end());
   return *this;
}

h2_inline size_t h2_paragraph::width() const
{
   size_t m = 0;
   for (auto& sentence : *this)
      m = std::max(m, sentence.width());
   return m;
}

h2_inline bool h2_paragraph::foldable(size_t width)
{
   size_t sum = 0;
   for (auto& sentence : *this)
      for (auto& word : sentence)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))  // ignore indent and \033m controller
            sum += word.size();

   return sum < width;
}

h2_inline h2_sentence h2_paragraph::folds()
{
   h2_sentence folded_sentence;
   for (auto& sentence : *this)
      for (auto& word : sentence)
         if (!word.isspace())  // drop indent
            folded_sentence.push_back(word);
   return folded_sentence;
}

h2_inline h2_string h2_paragraph::string() const
{
   h2_string s;
   for (auto& sentence : *this)
      for (auto& word : sentence)
         if (!word.isspace() && !h2_color::isctrl(word.c_str()))
            s += word;
   return s;
}

h2_inline void h2_paragraph::sequence(unsigned indent, int start)
{
   for (size_t i = 0; i < size(); ++i) {
      at(i) = gray(h2_string("%zu. ", i + start)) + at(i);
      if (indent) at(i).indent(indent);
   }
}

h2_inline void h2_paragraph::samesizify(h2_paragraph& a, h2_paragraph& b)
{
   size_t size = std::max(a.size(), b.size());
   for (size_t i = a.size(); i < size; ++i) a.push_back(h2_sentence());
   for (size_t i = b.size(); i < size; ++i) b.push_back(h2_sentence());
}
