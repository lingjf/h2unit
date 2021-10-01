struct h2_layout {
   static h2_lines split(const h2_lines& left_lines, const h2_lines& right_lines, const char* left_title, const char* right_title, size_t step, char scale, size_t width);
   static h2_lines unified(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width);
   static h2_lines seperate(const h2_line& up_line, const h2_line& down_line, const char* up_title, const char* down_title, size_t width);
};
