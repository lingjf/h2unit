
struct h2_layout {
   static h2_lines split(h2_lines& left_lines, h2_lines& right_lines, const char* left_title, const char* right_title);
   static h2_lines unified(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title);
   static h2_lines seperate(h2_line& up_line, h2_line& down_line, const char* up_title, const char* down_title);
};
