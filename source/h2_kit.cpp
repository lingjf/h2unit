
static inline bool h2_regex_match(const char* pattern, const char* subject, bool caseless = false) {
   bool result = false;
   try {  //C++11 support regex; gcc 4.8 start support regex, gcc 5.5 icase works.
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error& e) {
      result = false;
   }
   return result;
}

static inline bool h2_wildcard_match(const char* pattern, const char* subject, bool caseless = false) {
   return !fnmatch(pattern, subject, caseless ? FNM_CASEFOLD : 0);  // PathMatchSpec() for windows
}

static inline long long h2_now() {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

static inline void h2_sleep(long long milliseconds) {
   ::usleep(milliseconds * 1000);
}

static inline int h2_winsz() {
   struct winsize w;
   if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) return 80;
   return w.ws_col;
}

static inline const char* h2_style(const char* style, char* ascii_code) {
   static struct {
      const char *name, *value;
   } K[] = {
     {"reset", "0;"},
     {"bold", "1;"},
     {"italics", "3;"},
     {"underline", "4;"},
     {"inverse", "7;"},
     {"strikethrough", "9;"},
     {"black", "30;"},
     {"red", "31;"},
     {"green", "32;"},
     {"yellow", "33;"},
     {"blue", "34;"},
     {"purple", "35;"},
     {"cyan", "36;"},
     {"gray", "37;"},
     {"default", "39;"},
     {"dark gray", "90;"},
     {"light red", "91;"},
     {"light green", "92;"},
     {"light yellow", "93;"},
     {"light blue", "94;"},
     {"light purple", "95;"},
     {"light cyan", "96;"},
     {"white", "97;"},
     {"bg_black", "40;"},
     {"bg_red", "41;"},
     {"bg_green", "42;"},
     {"bg_yellow", "43;"},
     {"bg_blue", "44;"},
     {"bg_purple", "45;"},
     {"bg_cyan", "46;"},
     {"bg_white", "47;"},
     {"bg_default", "49;"}};

   char t[1024], *s = strcpy(t, style), *d = ascii_code + sprintf(ascii_code, "\033["), *q = d;
   for (char* p = strtok(s, ","); p; p = strtok(nullptr, ","))
      for (auto& k : K)
         if (!strcmp(k.name, p)) {
            q += sprintf(q, "%s", k.value);
            break;
         }
   return d == q ? strcpy(ascii_code, "") : (*(q - 1) = 'm', ascii_code);
}

static inline const char* PAD(int n) {
   static char st[1024];
   memset(st, ' ', n);
   st[n] = '\0';
   return st;
}

static inline char* SF(const char* style, const char* fmt, ...) {
   static char sb[1024 * 256], *sp;
   if (sp < sb || sb + sizeof(sb) / 2 < sp) sp = sb;
   char *s = sp, *p = s;

   p += sprintf(p, "%s", h2_option::I().style(style));
   va_list a;
   va_start(a, fmt);
   p += vsprintf(p, fmt, a);
   va_end(a);
   p += sprintf(p, "%s", h2_option::I().style("reset"));

   sp = p + 1;
   return s;
}
