struct h2_paragraph : h2_vector<h2_sentence> {
   h2_paragraph() {}
   h2_paragraph(std::initializer_list<h2_sentence> il) : h2_vector<h2_sentence>(il) {}

   h2_paragraph& operator+=(const h2_paragraph& paragraph);

   size_t width() const;
   bool foldable(size_t width = 20);
   h2_sentence folds();

   h2_string string() const;

   void sequence(unsigned indent = 0, int start = 0);
   static void samesizify(h2_paragraph& a, h2_paragraph& b);
};
