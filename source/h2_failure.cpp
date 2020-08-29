
static inline void escape_char(const char c, h2_string& t)
{
   char d = c;
   switch (c) {
   case '\n': d = 'n'; break;
   case '\r': d = 'r'; break;
   case '\t': d = 't'; break;
   }
   if (d != c) t.push_back('\\');
   t.push_back(d);
}

static inline h2_line h2_acronym(const h2_string& s, int width = 16, int tail = 0)
{
   if (s.size() <= width) {
      h2_string t;
      for (int i = 0; i < s.size(); ++i) escape_char(s.c_str()[i], t);
      return {t};
   } else {
      h2_string t1, t2;
      for (int i = 0; i < width - 3 - tail; ++i) escape_char(s.c_str()[i], t1);
      for (int i = 0; i < tail; ++i) escape_char(s.c_str()[s.size() - tail + i], t2);
      return {t1, "\033{+dark gray}", "...", "\033{-dark gray}", t2};
   }
}

h2_inline void h2_fail::append_subling(h2_fail*& fail, h2_fail* n)
{
   if (!fail)
      fail = n;
   else {
      h2_fail** pp = &fail->subling_next;
      while (*pp) pp = &(*pp)->subling_next;
      *pp = n;
   }
}

h2_inline void h2_fail::append_child(h2_fail*& fail, h2_fail* n)
{
   if (!fail)
      fail = n;
   else {
      h2_fail** pp = &fail->child_next;
      while (*pp) pp = &(*pp)->child_next;
      *pp = n;
   }
}

h2_inline h2_fail::~h2_fail()
{
   if (child_next) delete child_next;
   if (subling_next) delete subling_next;
}

h2_inline const char* h2_fail::locate()
{
   static char st[1024];
   if (!file || !strlen(file) || !line) return "";
   sprintf(st, "at %s:%d", file, line);
   return st;
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index, int child_index)
{
   cb(this, subling_index, child_index);
   if (child_next) child_next->foreach(cb, 0, child_index + 1);
   if (subling_next) subling_next->foreach(cb, subling_index + 1, child_index);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_line& explain_ = {}, const char* file_ = nullptr, int line_ = 0) : h2_fail(explain_, file_, line_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_line line;
      line.indent(child_index * 2 + 1);
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      line.concat_back(explain);
      if (strlen(locate())) line.printf("", ", %s", locate());
      h2_color::printf(line);
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   static const char* s_null[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0", nullptr};
   static const char* s_true[] = {"IsTrue", "true", "1", nullptr};
   static const char* s_false[] = {"IsFalse", "false", "0", nullptr};
   static const char** S[] = {s_null, s_true, s_false};

   for (int i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
      if (h2_in(a.c_str(), S[i]) && h2_in(b.c_str(), S[i]))
         return true;

   return false;
}

static inline void quote_acronym_print(h2_line& line, const h2_string& str, const char* style)
{
   if (str.enclosed('\"')) line.printf("dark gray", "\"");
   line.concat_back(h2_acronym(str.unquote('\"'), O.verbose ? 10000 : 30, str.enclosed('\"') ? 2 : 3), style);
   if (str.enclosed('\"')) line.printf("dark gray", "\"");
}

struct h2_fail_unexpect : h2_fail {
   h2_string e_represent, a_represent;
   h2_string expection;
   int c = 0;
   h2_fail_unexpect(const h2_string& e_represent_ = "", const h2_string& a_represent_ = "", const h2_string& expection_ = "", const h2_line& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(explain_, file_, line_), e_represent(e_represent_), a_represent(a_represent_), expection(expection_) {}

   void print_OK1(h2_line& line)
   {
      line.push_back("OK( ");
      line.concat_back(h2_acronym(a_expression, O.verbose ? 10000 : 30), "cyan");
      line.push_back(" ) is ");
      line.printf("bold,red", "false");
   }
   void print_OK2(h2_line& line)
   {
      line.push_back("OK( ");

      if (!expection.size() || e_expression == expection || is_synonym(e_expression, expection)) {
         quote_acronym_print(line, e_expression, "green");
      } else {
         quote_acronym_print(line, e_expression, "cyan");
         line.printf("dark gray", "==>");
         quote_acronym_print(line, expection, "green");
      }

      line.push_back(", ");

      if (!a_represent.size() || a_expression == a_represent || is_synonym(a_expression, a_represent)) {
         quote_acronym_print(line, a_expression, "bold,red");
      } else {
         quote_acronym_print(line, a_represent, "bold,red");
         line.printf("dark gray", "<==");
         quote_acronym_print(line, a_expression, "cyan");
      }

      line.push_back(" )");
   }
   void print_JE(h2_line& line)
   {
      line.push_back("JE( ");
      line.concat_back(h2_acronym(e_expression, O.verbose ? 10000 : 30, 2), "cyan");
      line.push_back(", ");
      line.concat_back(h2_acronym(a_expression, O.verbose ? 10000 : 30, 2), "bold,red");
      line.push_back(" )");
   }
   void print_Inner(h2_line& line)
   {
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      if (expection.size()) {
         line.printf("", "%sexpect is ", comma_if(c++));
         quote_acronym_print(line, expection, "green");
      }
      if (a_represent.size()) {
         line.printf("", "%sactual is ", comma_if(c++));
         quote_acronym_print(line, a_represent, "bold,red");
      }
   }

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_line line;
      line.indent(child_index * 2 + 1);
      if (!strcmp("Inner", check_type)) print_Inner(line);
      if (!strcmp("OK1", check_type)) print_OK1(line);
      if (!strcmp("OK2", check_type)) print_OK2(line);
      if (!strcmp("JE", check_type)) print_JE(line);
      line.push_back(" ");
      if (explain.width()) line.printf("", "%s", comma_if(c++)), line.concat_back(explain);
      if (user_explain.size()) line.printf("", "%s%s", comma_if(c++), user_explain.c_str());
      if (strlen(locate())) line.printf("", "%s%s", comma_if(c++), locate());
      h2_color::printf(line);
   }
};

static inline void fmt_char(char c, bool eq, const char* style, h2_line& line)
{
   char t_style[32] = "";
   if (!eq) strcpy(t_style, style);
   if (c == '\n') c = 'n', strcat(t_style, ",inverse");
   if (c == '\r') c = 'r', strcat(t_style, ",inverse");
   if (c == '\t') c = 't', strcat(t_style, ",inverse");
   if (c == '\0') c = ' ', eq || strcat(t_style, ",inverse");
   line.printf(t_style, "%c", c);
}

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_string& expection, const h2_line& explain = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(h2_representify(e_value_), h2_representify(a_value_), expection, explain, file_, line_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
         h2_line e_line, a_line;
         for (size_t i = 0; i < e_value.size(); ++i) {
            char ac = i < a_value.size() ? a_value[i] : ' ';
            bool eq = caseless ? ::tolower(e_value[i]) == ::tolower(ac) : e_value[i] == ac;
            fmt_char(e_value[i], eq, "green", e_line);
         }
         for (size_t i = 0; i < a_value.size(); ++i) {
            char ec = i < e_value.size() ? e_value[i] : ' ';
            bool eq = caseless ? ::tolower(a_value[i]) == ::tolower(ec) : a_value[i] == ec;
            fmt_char(a_value[i], eq, "red", a_line);
         }

         h2_color::printf(h2_layout::unified(e_line, a_line, "expect", "actual", O.term_size));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection, const h2_line& explain, const char* file = nullptr, int line = 0)
     : h2_fail_unexpect(h2_representify(e_value_), h2_representify(a_value_), expection, explain, file, line), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
         h2_line e_line, a_line;
         for (size_t i = 0; i < e_value.size(); ++i)
            fmt_char(e_value[i], true, "", e_line);

         for (size_t i = 0; i < a_value.size(); ++i)
            fmt_char(a_value[i], true, "", a_line);

         h2_color::printf(h2_layout::seperate(e_line, a_line, "expect", "actual", O.term_size));
      }
   }
};

struct h2_fail_json : h2_fail_unexpect {
   h2_string e_value, a_value;
   const bool caseless;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_string& expection_, bool caseless_, const h2_line& explain_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(e_value_, a_value_, expection_, explain_, file_, line_), e_value(e_value_), a_value(a_value_), caseless(caseless_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_lines e_lines, a_lines;
      bool illformed = !h2_json::diff(e_value, a_value, e_lines, a_lines, caseless);
      if (illformed) explain.push_back("illformed json");
      h2_fail_unexpect::print(subling_index, child_index);
      if (O.paste || illformed) {
         e_lines = h2_json::format(e_value);
         a_lines = h2_json::format(a_value);
         h2_color::printf("dark gray", "expect");
         h2_color::printf("bold,green", ">\n");
         h2_color::printf(e_lines);
         h2_color::printf("dark gray", "actual");
         h2_color::printf("bold,red", ">\n");
         h2_color::printf(a_lines);
      } else {
         h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual", (O.seq ? 1 : 0), 'd', O.term_size - 1);
         for (auto& line : lines) line.indent(1);
         h2_color::printf(lines);
      }
   }
};

struct h2_fail_memcmp : h2_fail_unexpect {
   h2_vector<unsigned char> e_value, a_value;
   const int width, nbits;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, const h2_string& expection_, const h2_string& a_represent_, const h2_line& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect("", a_represent_, expection_, explain_, file_, line_), e_value(e_value_, e_value_ + (nbits_ + 7) / 8), a_value(a_value_, a_value_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}

   void print(int subling_index, int child_index) override
   {
      h2_fail_unexpect::print(subling_index, child_index);
      h2_lines e_lines, a_lines;
      int bytes_per_row = 0;
      switch (width) {
      case 1: print_bits(e_lines, a_lines, bytes_per_row = 4); break;
      case 8: print_ints<unsigned char>(e_lines, a_lines, bytes_per_row = (O.term_size < 80 ? 8 : 16)); break;
      case 16: print_ints<unsigned short>(e_lines, a_lines, bytes_per_row = 16); break;
      case 32: print_ints<unsigned int>(e_lines, a_lines, bytes_per_row = 16); break;
      case 64: print_ints<unsigned long long>(e_lines, a_lines, bytes_per_row = 16); break;
      default: break;
      }
      h2_color::printf(h2_layout::split(e_lines, a_lines, "expect", "actual", bytes_per_row * 8 / width, 'x', O.term_size));
   }

   void print_bits(h2_lines& e_lines, h2_lines& a_lines, int bytes_per_row)
   {
      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (int j = 0; j < bytes_per_row; ++j) {
            if (j) e_line.push_back(" ");
            if (j) a_line.push_back(" ");
            for (int k = 0; k < 8; ++k) {
               if ((i * bytes_per_row + j) * 8 + k < nbits) {
                  unsigned char e_val = (e_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  unsigned char a_val = (a_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  if (e_val == a_val) {
                     e_line.push_back(h2_string(e_val ? "1" : "0"));
                     a_line.push_back(h2_string(a_val ? "1" : "0"));
                  } else {
                     e_line.printf("green", e_val ? "1" : "0");
                     a_line.printf("bold,red", a_val ? "1" : "0");
                  }
               }
            }
         }
         e_lines.push_back(e_line);
         a_lines.push_back(a_line);
      }
   }

   template <typename T>
   void print_ints(h2_lines& e_lines, h2_lines& a_lines, int bytes_per_row)
   {
      char fmt[32];
      sprintf(fmt, "%%s%%0%dX", (int)sizeof(T) * 2);

      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (int j = 0; j < bytes_per_row; j += sizeof(T)) {
            if (i * bytes_per_row + j < e_value.size()) {
               T e_val = *(T*)(e_value.data() + (i * bytes_per_row + j));
               T a_val = *(T*)(a_value.data() + (i * bytes_per_row + j));
               char e_str[32], a_str[32];
               sprintf(e_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", e_val);
               sprintf(a_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", a_val);
               if (e_val == a_val) {
                  e_line.push_back(e_str);
                  a_line.push_back(a_str);
               } else {
                  e_line.printf("green", e_str);
                  a_line.printf("bold,red", a_str);
               }
            }
         }
         e_lines.push_back(e_line);
         a_lines.push_back(a_line);
      }
   }
};

struct h2_fail_memory : h2_fail {
   const void* ptr;
   const int size;
   const h2_backtrace bt_allocate, bt_release;

   h2_fail_memory(const void* ptr_, const int size_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail({}, file_, line_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, int size_, const h2_backtrace& bt_allocate_, const char* where_, const char* file_, int line_)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), where(where_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::printf("", " %p", ptr);
      h2_color::printf("bold,red", " memory leak");
      h2_color::printf("red", " %d", size);
      h2_color::printf("", " bytes in %s totally %s\n", where, locate());
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::printf("", " %p", ptr);
      h2_color::printf("bold,red", " double free");
      h2_color::printf("", " at backtrace:\n", ptr), bt_double_free.print(2);
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::printf("", "  already free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::printf("", " %p allocate with ", ptr);
      h2_color::printf("bold,red", "%s", who_allocate);
      h2_color::printf("", ", release by ");
      h2_color::printf("bold,red", "%s", who_release);
      h2_color::printf("", " asymmetrically at backtrace:\n"), bt_release.print(2);
      if (0 < bt_allocate.count) h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_overflow : h2_fail_memory {
   const void* addr;                    /* 犯罪地点 */
   const char* action;                  /* 犯罪行为 */
   const h2_vector<unsigned char> spot; /* 犯罪现场 */
   const h2_backtrace bt_trample;       /* 犯罪过程 */
   h2_fail_overflow(const void* ptr_, const int size_, const void* addr_, const char* action_, const h2_vector<unsigned char>& spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), addr(addr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      int offset = ptr < addr ? (long long)addr - ((long long)ptr + size) : (long long)addr - (long long)ptr;
      h2_color::printf("", " %p %+d (%p)", ptr, offset, addr);
      h2_color::printf("bold,red", " %s", action);
      h2_color::printf("", " %s ", offset >= 0 ? "overflow" : "underflow");

      for (int i = 0; i < spot.size(); ++i)
         h2_color::printf("bold,red", "%02X ", spot[i]);

      if (locate()) h2_color::printf("", ", %s\n", locate());
      if (bt_trample.count) h2_color::printf("", "  trampled at backtrace:\n"), bt_trample.print(3);
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_use_after_free : h2_fail_memory {
   const void* addr;          /* 犯罪地点 */
   const char* action;        /* 犯罪行为 */
   const h2_backtrace bt_use; /* 犯罪过程 */
   h2_fail_use_after_free(const void* ptr_, const void* addr_, const char* action_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_use_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), addr(addr_), action(action_), bt_use(bt_use_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::printf("", " %p %+d (%p)", ptr, (long long)addr - (long long)ptr, addr);
      h2_color::printf("bold,red", " %s after free", action);
      h2_color::printf("", " at backtrace:\n"), bt_use.print(2);
      h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::printf("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_line& explain, const char* file, int line)
{
   return new h2_fail_normal(explain, file, line);
}
h2_inline h2_fail* h2_fail::new_unexpect(const h2_string& e_represent, const h2_string& a_represent, const h2_string& expection, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_unexpect(e_represent, a_represent, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_string& expection, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_strcmp(e_value, a_value, caseless, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_strfind(e_value, a_value, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_string& expection, bool caseless, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_json(e_value, a_value, expection, caseless, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& expection, const h2_string& a_represent, const h2_line& explain, const char* file, int line)
{
   return new h2_fail_memcmp(e_value, a_value, width, nbits, expection, a_represent, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, int size, const h2_backtrace& bt_allocate, const char* where, const char* file, int line)
{
   return new h2_fail_memory_leak(ptr, size, bt_allocate, where, file, line);
}
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free)
{
   return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free);
}
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release)
{
   return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release);
}
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const int size, const void* addr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file, int line)
{
   return new h2_fail_overflow(ptr, size, addr, action, spot, bt_allocate, bt_trample, file, line);
}
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* addr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use)
{
   return new h2_fail_use_after_free(ptr, addr, action, bt_allocate, bt_release, bt_use);
}
