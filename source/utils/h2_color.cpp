
struct h2_colorful {
   h2_singleton(h2_colorful);

   char current[8][32];
   int default_attribute;
#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
   HANDLE console_handle;
#else
#   define FOREGROUND_INTENSITY 0
#   define COMMON_LVB_UNDERSCORE 0
#   define COMMON_LVB_REVERSE_VIDEO 0
#   define FOREGROUND_RED 0
#   define FOREGROUND_GREEN 0
#   define FOREGROUND_BLUE 0
#   define BACKGROUND_RED 0
#   define BACKGROUND_GREEN 0
#   define BACKGROUND_BLUE 0
#endif

   h2_colorful()
   {
      memset(current, 0, sizeof(current));
      default_attribute = 0;
#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
      console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(console_handle, &csbi);
      default_attribute = csbi.wAttributes;
#endif
   }

   void clear_style()
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         current[i][0] = '\0';
   }

   void push_style(const char* style, int length)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (current[i][0] == '\0') {
            strncpy(current[i], style, length);
            current[i][length] = '\0';
            break;
         }
   }

   void pop_style(const char* style, int length)
   {
      for (int i = 0; i < sizeof(current) / sizeof(current[0]); ++i)
         if (!strncmp(current[i], style, length) && strlen(current[i]) == length)
            current[i][0] = '\0';
   }

   void change()
   {
#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
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

   void parse(const char* style)
   {
      const char* p = style + 2;
      char s = '+';
      if (*p == '+' || *p == '-') s = *p++;

      for (;;) {
         int l = strcspn(p, ",}");
         s == '-' ? pop_style(p, l) : push_style(p, l);
         if (!strncmp("reset", p, l)) clear_style();
         if (*(p + l) == '}' || *(p + l) == '\0') break;
         p += l + 1;
      }
   }

   void print(const char* str)
   {
      if (h2_color::isctrl(str)) {
         if (h2_option::I().colorful) I().parse(str), I().change();
      } else {
         h2_libc::write(fileno(stdout), str, strlen(str));
      }
   }

   int style2value(const char* style)
   {
      static struct st {
         const char* name;
         int value;
         int attribute;
      } K[] = {
        {"reset", 0, default_attribute},
        {"bold", 1, FOREGROUND_INTENSITY},
        {"italics", 3, 0},
        {"underline", 4, COMMON_LVB_UNDERSCORE},
        {"inverse", 7, COMMON_LVB_REVERSE_VIDEO},
        {"strikethrough", 9, 0},
        {"black", 30, 0},
        {"red", 31, FOREGROUND_RED},
        {"green", 32, FOREGROUND_GREEN},
        {"yellow", 33, FOREGROUND_RED | FOREGROUND_GREEN},
        {"blue", 34, FOREGROUND_BLUE},
        {"purple", 35, FOREGROUND_RED | FOREGROUND_BLUE},
        {"cyan", 36, FOREGROUND_BLUE | FOREGROUND_GREEN},
        {"gray", 37, 0},
        {"default", 39, 0},
        {"dark gray", 90, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"light red", 91, FOREGROUND_RED | FOREGROUND_INTENSITY},
        {"light green", 92, FOREGROUND_GREEN | FOREGROUND_INTENSITY},
        {"light yellow", 93, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY},
        {"light blue", 94, FOREGROUND_BLUE | FOREGROUND_INTENSITY},
        {"light purple", 95, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
        {"light cyan", 96, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE},
        {"white", 97, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE},
        {"bg_black", 40, 0},
        {"bg_red", 41, BACKGROUND_RED},
        {"bg_green", 42, BACKGROUND_GREEN},
        {"bg_yellow", 43, BACKGROUND_RED | BACKGROUND_GREEN},
        {"bg_blue", 44, BACKGROUND_BLUE},
        {"bg_purple", 45, BACKGROUND_RED | BACKGROUND_BLUE},
        {"bg_cyan", 46, BACKGROUND_BLUE | BACKGROUND_GREEN},
        {"bg_white", 47, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE},
        // {"bg_default", 49}
      };

      for (int i = 0; i < sizeof(K) / sizeof(K[0]); ++i)
         if (!strcmp(K[i].name, style))
#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
            return K[i].attribute;
#else
            return K[i].value;
#endif

      return default_attribute;
   }
};

h2_inline void h2_color::prints(const char* style, const char* format, ...)
{
   if (style && strlen(style)) {
      char t[128];
      sprintf(t, "\033{%s}", style);
      h2_colorful::I().print(t);
   }

   char* alloca_str;
   h2_sprintf(alloca_str, format);
   h2_colorful::I().print(alloca_str);

   if (style && strlen(style)) h2_colorful::I().print("\033{reset}");
}

h2_inline void h2_color::printl(const h2_row& row)
{
   for (auto& word : row) h2_colorful::I().print(word.c_str());
   h2_colorful::I().print("\n");
}

h2_inline void h2_color::printl(const h2_rows& rows)
{
   for (auto& row : rows) printl(row);
}
