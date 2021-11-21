struct h2_console {
   static size_t width()
   {
      static size_t s_width = 0;
      if (s_width == 0) {
         s_width = 120;
#if defined _WIN32
         CONSOLE_SCREEN_BUFFER_INFO csbi;
         GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
         auto columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
         if (16 < columns) s_width = columns;
#else
         struct winsize w;
         if (-1 != ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
            if (16 < w.ws_col) s_width = w.ws_col;
#endif
      }
      return s_width;
   }

   static void prints(const char* style, const char* format, ...)
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

   static void printl(const h2_line& line, bool cr = true)
   {
      for (auto& word : line) h2_color::I().print(word.c_str());
      if (cr) h2_color::I().print("\n");
   }

   static void printl(const h2_lines& lines, bool cr = true)
   {
      for (auto& line : lines) printl(line, cr);
   }
};
