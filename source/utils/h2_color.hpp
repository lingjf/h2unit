struct h2_color {
   static void prints(const char* style, const char* format, ...);
   static void printl(const h2_sentence& sentence, bool cr = true);
   static void printl(const h2_paragraph& paragraph, bool cr = true);
   static bool isctrl(const char* s) { return s[0] == '\033' && s[1] == '{'; };
};
