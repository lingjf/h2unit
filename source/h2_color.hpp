
struct h2_color {
   static void printf(const char* style, const char* format, ...);
   static void printf(const h2_line& line);
   static void printf(const h2_lines& lines);

   static bool is_ctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
