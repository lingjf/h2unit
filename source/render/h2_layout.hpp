struct h2_layout {
   static h2_paragraph split(const h2_paragraph& left_paragraph, const h2_paragraph& right_paragraph, const char* left_title, const char* right_title, size_t step, char scale, size_t width);
   static h2_paragraph unified(const h2_sentence& up_sentence, const h2_sentence& down_sentence, const char* up_title, const char* down_title, size_t width);
   static h2_paragraph seperate(const h2_sentence& up_sentence, const h2_sentence& down_sentence, const char* up_title, const char* down_title, size_t width);
};
