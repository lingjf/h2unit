struct h2_sentence : h2_vector<h2_string> {
   h2_sentence() {}
   h2_sentence(const char* a) : h2_vector<h2_string>({a}) {}
   h2_sentence(const h2_string& a) : h2_vector<h2_string>({a}) {}
   h2_sentence(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   size_t width(bool ignore_indent = false) const;
   h2_sentence& indent(size_t n, const char c = ' ');
   h2_sentence& padding(size_t n, const char c = ' ');

   h2_sentence& printf(const char* style, const char* format, ...);
   h2_sentence& operator+=(const h2_sentence& sentence);
   h2_sentence& brush(const char* style);

   bool enclosed(const char c) const;
   h2_sentence gray_quote() const;
   h2_sentence acronym(size_t width = 16, size_t tail = 0) const;
   h2_string string() const;

   static void samesizify(h2_sentence& a, h2_sentence& b);
};

inline h2_sentence operator+(const h2_sentence& a, const h2_sentence& b)  // implicit conversion const char* / h2_string
{
   h2_sentence sentence;
   sentence.insert(sentence.end(), a.begin(), a.end());
   sentence.insert(sentence.end(), b.begin(), b.end());
   return sentence;
}

static inline const h2_sentence color(const h2_sentence& sentence, const char* style) { return ("\033{" + h2_string(style) + "}") + sentence + "\033{reset}"; }
static inline const h2_sentence delta(const h2_sentence& sentence, const char* style) { return ("\033{+" + h2_string(style) + "}") + sentence + ("\033{-" + h2_string(style) + "}"); }
static inline const h2_sentence gray(const h2_sentence& sentence) { return delta(sentence, "dark gray"); }
