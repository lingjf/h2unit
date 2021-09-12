struct h2_color {
   static void prints(const char* style, const char* format, ...);
   static void printl(const h2_row& row, bool cr = true);
   static void printl(const h2_rows& rows, bool cr = true);
   static bool isctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
