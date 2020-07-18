
struct h2_line : h2_vector<h2_string> {
   h2_line() {}
   h2_line(const char* a) : h2_vector<h2_string>({a}) {}
   h2_line(h2_string a) : h2_vector<h2_string>({a}) {}
   h2_line(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   unsigned width(bool ignore_indent = false) const;
   h2_line& indent(int n, const char c = ' ');
   h2_line& padding(int n, const char c = ' ');

   h2_line& printf(const char* style, const char* format, ...);
   h2_line& concat_back(const h2_line& line, const char* style = nullptr);
   h2_line& concat_front(const h2_line& line_or_word, const char* style = nullptr);

   void brush(const char* style);
   void samesizify(h2_line& b);
};

struct h2_lines : h2_vector<h2_line> {
   h2_lines() {}
   h2_lines(std::initializer_list<h2_line> il) : h2_vector<h2_line>(il) {}

   h2_lines& operator+=(const h2_lines& lines);
   h2_lines& concat_front(const h2_lines& lines);
   h2_lines& concat_front(const h2_line& line);

   unsigned width() const;
   bool foldable(unsigned width = 20);
   h2_line folds();

   void sequence(unsigned indent = 0, int start = 0);
   void samesizify(h2_lines& b);
};
