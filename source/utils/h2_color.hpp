
struct h2_color {
   static void prints(const char* style, const char* format, ...);
   static void printl(const h2_line& line);
   static void printl(const h2_lines& lines);

   static bool isctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
