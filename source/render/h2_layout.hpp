struct h2_layout {
   static h2_rows split(const h2_rows& left_rows, const h2_rows& right_rows, const char* left_title, const char* right_title, int step, char scale, unsigned width);
   static h2_rows unified(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width);
   static h2_rows seperate(const h2_row& up_row, const h2_row& down_row, const char* up_title, const char* down_title, unsigned width);
};
