
struct h2__color {
   h2_singleton(h2__color);
   h2__color() { memset(current, 0, sizeof(current)); }

   char current[8][32];

   void clear_style()
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         current[i][0] = '\0';
   }

   void push_style(const char* style)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] == '\0') {
            strcpy(current[i], style);
            break;
         }
   }

   void pop_style(const char* style)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (!strcmp(current[i], style))
            current[i][0] = '\0';
   }

   void change()
   {
#ifdef _WIN32
      SetConsoleTextAttribute(console_handle, style2value("reset"));
      WORD a = 0;
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            a |= style2value(current[i]);
      SetConsoleTextAttribute(console_handle, a);
#else
      char a[256];
      sprintf(a, "\033[%d;", style2value("reset"));
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] != '\0')
            sprintf(a + strlen(a), "%d;", style2value(current[i]));
      a[strlen(a) - 1] = 'm';
      h2_libc::write(1, a, strlen(a));
#endif
   }

   char* parse(const char* style)
   {
      char* end = strchr((char*)style, '}');
      assert(end);
      char t[128];
      strncpy(t, style, sizeof(t));
      *strchr(t, '}') = '\0';
      for (char* p = strtok(t + 2, ","); p; p = strtok(NULL, ",")) {
         if (p[0] == '-')
            pop_style(p + 1);
         else if (p[0] == '+')
            push_style(p + 1);
         else
            push_style(p);

         if (!strcmp("reset", p)) clear_style();
      }
      return end;
   }

   void print(const char* str)
   {
      for (const char* p = str; *p; p++) {
         if (h2_color::is_ctrl(p)) {
            p = I().parse(p);
            if (h2_option::I().colorable) I().change();
         } else {
            h2_libc::write(fileno(stdout), p, 1);
         }
      }
   }

   static int style2value(const char* style)
   {
      struct st {
         const char* name;
         int value;
      };

#ifdef _WIN32
      static HANDLE console_handle = NULL;
      static WORD default_attribute;
      if (console_handle == NULL) {
         console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
         CONSOLE_SCREEN_BUFFER_INFO csbi;
         GetConsoleScreenBufferInfo(console_handle, &csbi);
         default_attribute = csbi.wAttributes;
      }

      static struct st K[] = {
        // normal style
        {"reset", default_attribute},
        {"bold", FOREGROUND_INTENSITY},
        // { "italics", 3 },
        {"underline", COMMON_LVB_UNDERSCORE},
        {"inverse", COMMON_LVB_REVERSE_VIDEO},
        // { "strikethrough", 9 },
        // foreground color
        // { "black", 30 },
        {"red", FOREGROUND_RED},
        {"green", FOREGROUND_GREEN},
        {"yellow", FOREGROUND_RED | FOREGROUND_GREEN},
        {"blue", FOREGROUND_BLUE},
        {"purple", FOREGROUND_RED | FOREGROUND_BLUE},
        {"cyan", FOREGROUND_BLUE | FOREGROUND_GREEN},
        {"white", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"dark gray", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"default", 39},
        // background color
        // { "bg_black", 40 },
        {"bg_red", BACKGROUND_RED},
        {"bg_green", BACKGROUND_GREEN},
        {"bg_yellow", BACKGROUND_RED | BACKGROUND_GREEN},
        {"bg_blue", BACKGROUND_BLUE},
        {"bg_purple", BACKGROUND_RED | BACKGROUND_BLUE},
        {"bg_cyan", BACKGROUND_BLUE | BACKGROUND_GREEN},
        {"bg_white", BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE},
        // { "bg_default", 49 }
      };

#else
      static struct st K[] = {
        {"reset", 0},
        {"bold", 1},
        {"italics", 3},
        {"underline", 4},
        {"inverse", 7},
        {"strikethrough", 9},
        {"black", 30},
        {"red", 31},
        {"green", 32},
        {"yellow", 33},
        {"blue", 34},
        {"purple", 35},
        {"cyan", 36},
        {"gray", 37},
        {"default", 39},
        {"dark gray", 90},
        {"light red", 91},
        {"light green", 92},
        {"light yellow", 93},
        {"light blue", 94},
        {"light purple", 95},
        {"light cyan", 96},
        {"white", 97},
        {"bg_black", 40},
        {"bg_red", 41},
        {"bg_green", 42},
        {"bg_yellow", 43},
        {"bg_blue", 44},
        {"bg_purple", 45},
        {"bg_cyan", 46},
        {"bg_white", 47},
        // {"bg_default", 49}
      };
#endif

      for (int i = 0; i < sizeof(K) / sizeof(K[0]); ++i)
         if (!strcmp(K[i].name, style))
            return K[i].value;

      return K[0].value;  //reset
   }
};

h2_inline void h2_color::printf(const char* style, const char* format, ...)
{
   if (style && strlen(style)) {
      char t[128];
      sprintf(t, "\033{%s}", style);
      h2__color::I().print(t);
   }

   char* alloca_str;
   h2_sprintf(alloca_str, format);
   h2__color::I().print(alloca_str);

   if (style && strlen(style))
      h2__color::I().print("\033{reset}");
}

h2_inline void h2_color::printf(h2_line& line)
{
   for (auto& word : line)
      h2__color::I().print(word.c_str());
   h2__color::I().print("\n");
}

h2_inline void h2_color::printf(h2_lines& lines)
{
   for (auto& line : lines) printf(line);
}
