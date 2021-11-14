struct h2_color {
   h2_singleton(h2_color);
   char current[8][32];

   h2_color() { memset(current, 0, sizeof(current)); }

   void clear_style()
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         current[i][0] = '\0';
   }
   void push_style(const char* style, size_t length)
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] == '\0') {
            strncpy(current[i], style, length);
            current[i][length] = '\0';
            break;
         }
   }
   void pop_style(const char* style, size_t length)
   {
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (!strncmp(current[i], style, length) && strlen(current[i]) == length)
            current[i][0] = '\0';
   }
   void change()
   {
      char a[256];
      sprintf(a, "\033[%d;", style2value("reset"));
      for (size_t i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            sprintf(a + strlen(a), "%d;", style2value(current[i]));
      a[strlen(a) - 1] = 'm';
      LIBC__write(-20072009, a, strlen(a));
   }
   void parse(const char* style)
   {
      const char* p = style + 2;
      char s = '+';
      if (*p == '+' || *p == '-') s = *p++;

      for (;;) {
         size_t l = strcspn(p, ",}");
         s == '-' ? pop_style(p, l) : push_style(p, l);
         if (!strncmp("reset", p, l)) clear_style();
         if (*(p + l) == '}' || *(p + l) == '\0') break;
         p += l + 1;
      }
   }
   void print(const char* str)
   {
      /* Windows PowerShell works, but CMD not, refer to v5.11 SetConsoleTextAttribute */
      if (h2_console::isctrl(str)) {
         if (h2_option::I().colorful) {
            I().parse(str);
            I().change();
         }
      } else {
         LIBC__write(-20072009, str, strlen(str));
      }
   }
   int style2value(const char* style)  // https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_sequences
   {
      if (!strcmp(style, "reset")) return 0;
      if (!strcmp(style, "bold")) return 1;
      if (!strcmp(style, "italics")) return 3;
      if (!strcmp(style, "underline")) return 4;
      if (!strcmp(style, "inverse")) return 7;
      if (!strcmp(style, "strikethrough")) return 9;
      if (!strcmp(style, "black")) return 30;
      if (!strcmp(style, "red")) return 31;
      if (!strcmp(style, "green")) return 32;
      if (!strcmp(style, "yellow")) return 33;
      if (!strcmp(style, "blue")) return 34;
      if (!strcmp(style, "purple")) return 35;
      if (!strcmp(style, "cyan")) return 36;
      if (!strcmp(style, "gray")) return 37;
      if (!strcmp(style, "default")) return 39;
      if (!strcmp(style, "dark gray")) return 90;
      if (!strcmp(style, "light red")) return 91;
      if (!strcmp(style, "light green")) return 92;
      if (!strcmp(style, "light yellow")) return 93;
      if (!strcmp(style, "light blue")) return 94;
      if (!strcmp(style, "light purple")) return 95;
      if (!strcmp(style, "light cyan")) return 96;
      if (!strcmp(style, "white")) return 97;
      if (!strcmp(style, "bg_black")) return 40;
      if (!strcmp(style, "bg_red")) return 41;
      if (!strcmp(style, "bg_green")) return 42;
      if (!strcmp(style, "bg_yellow")) return 43;
      if (!strcmp(style, "bg_blue")) return 44;
      if (!strcmp(style, "bg_purple")) return 45;
      if (!strcmp(style, "bg_cyan")) return 46;
      if (!strcmp(style, "bg_white")) return 47;
      return 0;
   }
};

h2_inline size_t h2_console::width()
{
   static size_t s_width = 0;
   if (s_width == 0) {
      s_width = 120;
#if defined _WIN32
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
      auto columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
      // auto rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
      if (16 < columns) s_width = columns;
#else
      struct winsize w;
      if (-1 != ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
         if (16 < w.ws_col) s_width = w.ws_col;
#endif
   }
   return s_width;
}

h2_inline void h2_console::prints(const char* style, const char* format, ...)
{
   if (style && strlen(style)) {
      char t[128];
      sprintf(t, "\033{%s}", style);
      h2_color::I().print(t);
   }

   char* alloca_str;
   h2_sprintf(alloca_str, format);
   h2_color::I().print(alloca_str);

   if (style && strlen(style)) h2_color::I().print("\033{reset}");
}

h2_inline void h2_console::printl(const h2_line& line, bool cr)
{
   for (auto& word : line) h2_color::I().print(word.c_str());
   if (cr) h2_color::I().print("\n");
}

h2_inline void h2_console::printl(const h2_lines& lines, bool cr)
{
   for (auto& line : lines) printl(line, cr);
}