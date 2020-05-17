
struct h2_line : public h2_vector<h2_string> {
   h2_line() {}
   h2_line(h2_string a) : h2_vector<h2_string>({a}) {}
   h2_line(std::initializer_list<h2_string> il) : h2_vector<h2_string>(il) {}

   int length() const;
   void indent(int n_space);
   void padding(int n_space);

   void printf(const char* style, const char* format, ...);
   void concat_back(const char* style, h2_line& line);
   void concat_front(const char* style, h2_line& line);

   void fold(h2_vector<h2_line>& lines);
   void samesizify(h2_line& b);
};

struct h2_lines : public h2_vector<h2_line> {
   h2_lines() {}
   h2_lines(std::initializer_list<h2_line> il) : h2_vector<h2_line>(il) {}

   void concat_back(h2_lines& lines);
   void concat_front(h2_lines& lines);

   int max_length() const;
   void samesizify(h2_lines& b);
   bool foldable();

   void sequence(int indent, int start = 0);
};