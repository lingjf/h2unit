struct h2_row : h2_vector<h2_string> {
   h2_row() {}
   h2_row(const char* a) : h2_vector<h2_string>({a}) {}
   h2_row(const h2_string& a) : h2_vector<h2_string>({a}) {}
   h2_row(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   unsigned width(bool ignore_indent = false) const;
   h2_row& indent(int n, const char c = ' ');
   h2_row& padding(int n, const char c = ' ');

   h2_row& printf(const char* style, const char* format, ...);
   h2_row& operator+=(const h2_row& row);
   h2_row& brush(const char* style);

   bool enclosed(const char c) const;
   h2_row gray_quote() const;
   h2_row acronym(int width = 16, int tail = 0) const;
   h2_string string() const;

   void samesizify(h2_row& b);
};

inline h2_row operator+(const h2_row& a, const h2_row& b)  // implicit conversion const char* / h2_string
{
   h2_row row;
   row.insert(row.end(), a.begin(), a.end());
   row.insert(row.end(), b.begin(), b.end());
   return row;
}

static inline const h2_row color(const h2_row& s, const char* style) { return ("\033{" + h2_string(style) + "}") + s + "\033{reset}"; }
static inline const h2_row delta(const h2_row& s, const char* style) { return ("\033{+" + h2_string(style) + "}") + s + ("\033{-" + h2_string(style) + "}"); }
static inline const h2_row gray(const h2_row& s) { return delta(s, "dark gray"); }

struct h2_rows : h2_vector<h2_row> {
   h2_rows() {}
   h2_rows(std::initializer_list<h2_row> il) : h2_vector<h2_row>(il) {}

   h2_rows& operator+=(const h2_rows& rows);

   unsigned width() const;
   bool foldable(unsigned width = 20);
   h2_row folds();

   h2_string string() const;

   void sequence(unsigned indent = 0, int start = 0);
   void samesizify(h2_rows& b);
};
