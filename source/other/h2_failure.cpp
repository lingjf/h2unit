
h2_inline void h2_fail::append_subling(h2_fail*& fail, h2_fail* n)
{
   if (!fail) {
      fail = n;
   } else {
      h2_fail** pp = &fail->subling_next;
      while (*pp) pp = &(*pp)->subling_next;
      *pp = n;
   }
}

h2_inline void h2_fail::append_child(h2_fail*& fail, h2_fail* n)
{
   if (!fail) {
      fail = n;
   } else {
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

h2_inline h2_row h2_fail::locate()
{
   if (file && strlen(file) && line)
      return gray(" at ") + h2_string("%s:%d", h2_basename(file), line);
   return {};
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index, int child_index)
{
   cb(this, subling_index, child_index);
   if (child_next) child_next->foreach(cb, 0, child_index + 1);
   if (subling_next) subling_next->foreach(cb, subling_index + 1, child_index);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_row& explain_ = {}, const char* file_ = nullptr, int line_ = 0) : h2_fail(explain_, file_, line_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row;
      row.indent(child_index * 2 + 1);
      if (0 <= seqno) row.printf("dark gray", "%d. ", seqno);
      row += explain;
      h2_color::printl(row + locate());
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   static const char* s_null[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0", nullptr};
   static const char* s_true[] = {"IsTrue", "true", "1", nullptr};
   static const char* s_false[] = {"IsFalse", "false", "0", nullptr};
   static const char** S[] = {s_null, s_true, s_false};

   if (a == b) return true;
   for (int i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
      if (h2_in(a.c_str(), S[i]) && h2_in(b.c_str(), S[i]))
         return true;
   return false;
}

struct h2_fail_unexpect : h2_fail {
   h2_row expection, represent;
   int c = 0;
   h2_fail_unexpect(const h2_row& expection_ = {}, const h2_row& represent_ = {}, const h2_row& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail(explain_, file_, line_), expection(expection_), represent(represent_) {}

   void print_OK1(h2_row& row)
   {
      h2_row a = h2_row(a_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("cyan");
      row += "OK" + gray("(") + a + gray(")") + " is " + color("false", "bold,red");
   }
   void print_OK2(h2_row& row)
   {
      h2_row e, a;
      if (!expection.width()) {
         e = h2_row(e_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("green");
      } else if (is_synonym(e_expression, expection.string())) {
         e = expection.acronym(O.verbose ? 10000 : 30, 3).brush("green");
      } else {
         e = h2_row(e_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("cyan") + gray("==>") + expection.acronym(O.verbose ? 10000 : 30, 3).brush("green");
      }

      if (!represent.width()) {
         a = h2_row(a_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("bold,red");
      } else if (is_synonym(a_expression, represent.string()) || !a_expression.length()) {
         a = represent.acronym(O.verbose ? 10000 : 30, 3).brush("bold,red");
      } else {
         a = represent.acronym(O.verbose ? 10000 : 30, 3).brush("bold,red") + gray("<==") + h2_row(a_expression).acronym(O.verbose ? 10000 : 30, 3).gray_quote().brush("cyan");
      }

      row += "OK" + gray("(") + e + ", " + a + gray(")");
   }
   void print_JE(h2_row& row)
   {
      h2_row e = h2_row(e_expression.unquote('\"').unquote('\'')).acronym(O.verbose ? 10000 : 30, 2).brush("cyan");
      h2_row a = h2_row(a_expression.unquote('\"').unquote('\'')).acronym(O.verbose ? 10000 : 30, 2).brush("bold,red");
      row += "JE" + gray("(") + e + ", " + a + gray(")");
   }
   void print_Inner(h2_row& row)
   {
      if (0 <= seqno) row.printf("dark gray", "%d. ", seqno);
      if (expection.width()) {
         row.printf("", "%sexpect is ", comma_if(c++));
         row += expection.acronym(O.verbose ? 10000 : 30, 3).brush("green");
      }
      if (represent.width()) {
         row.printf("", "%sactual is ", comma_if(c++));
         row += represent.acronym(O.verbose ? 10000 : 30, 3).brush("bold,red");
      }
   }

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row;
      row.indent(child_index * 2 + 1);
      if (!strcmp("Inner", assert_type)) print_Inner(row);
      if (!strcmp("OK1", assert_type)) print_OK1(row);
      if (!strcmp("OK2", assert_type)) print_OK2(row);
      if (!strcmp("JE", assert_type)) print_JE(row);
      if (explain.width()) row += comma_if(c++, ", ", " ") + explain;
      if (user_explain.size()) row += {comma_if(c++, ", ", " "), user_explain};
      h2_color::printl(row + locate());
   }
};

static inline void fmt_char(char c, bool eq, const char* style, h2_row& row)
{
   char t_style[32] = "";
   if (!eq) strcpy(t_style, style);
   if (c == '\n') c = 'n', strcat(t_style, ",inverse");
   if (c == '\r') c = 'r', strcat(t_style, ",inverse");
   if (c == '\t') c = 't', strcat(t_style, ",inverse");
   if (c == '\0') c = ' ', eq || strcat(t_style, ",inverse");
   row.printf(t_style, "%c", c);
}

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_row& expection, const h2_row& explain = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(expection, h2_representify(a_value_), explain, file_, line_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}

   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
         h2_row e_row, a_row;
         for (size_t i = 0; i < e_value.size(); ++i) {
            char ac = i < a_value.size() ? a_value[i] : ' ';
            bool eq = caseless ? ::tolower(e_value[i]) == ::tolower(ac) : e_value[i] == ac;
            fmt_char(e_value[i], eq, "green", e_row);
         }
         for (size_t i = 0; i < a_value.size(); ++i) {
            char ec = i < e_value.size() ? e_value[i] : ' ';
            bool eq = caseless ? ::tolower(a_value[i]) == ::tolower(ec) : a_value[i] == ec;
            fmt_char(a_value[i], eq, "red", a_row);
         }

         h2_color::printl(h2_layout::unified(e_row, a_row, "expect", "actual", O.terminal_width));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_row& expection, const h2_row& explain, const char* file = nullptr, int line = 0)
     : h2_fail_unexpect(expection, h2_representify(a_value_), explain, file, line), e_value(e_value_), a_value(a_value_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_fail_unexpect::print(subling_index, child_index);

      if (12 < e_value.size() || 12 < a_value.size()) {  // omit short string unified compare layout
         h2_row e_row, a_row;
         for (size_t i = 0; i < e_value.size(); ++i) fmt_char(e_value[i], true, "", e_row);
         for (size_t i = 0; i < a_value.size(); ++i) fmt_char(a_value[i], true, "", a_row);
         h2_color::printl(h2_layout::seperate(e_row, a_row, "expect", "actual", O.terminal_width));
      }
   }
};

struct h2_fail_json : h2_fail_unexpect {
   h2_string e_value, a_value;
   const bool caseless;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_row& expection_, bool caseless_, const h2_row& explain_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect(expection_, a_value_, explain_, file_, line_), e_value(e_value_), a_value(a_value_), caseless(caseless_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_rows e_rows, a_rows;
      h2_fail_unexpect::print(subling_index, child_index);
      if (O.copy_paste_json || !h2_json::diff(e_value, a_value, e_rows, a_rows, caseless)) {
         e_rows = h2_json::format(e_value);
         a_rows = h2_json::format(a_value);
         for (size_t i = 0; i < e_rows.size(); ++i)
            if (i) e_rows[i].indent(8);
         for (size_t i = 0; i < a_rows.size(); ++i)
            if (i) a_rows[i].indent(8);
         h2_color::prints("dark gray", "expect");
         h2_color::prints("green", "> ");
         h2_color::printl(e_rows);
         h2_color::prints("dark gray", "actual");
         h2_color::prints("red", "> ");
         h2_color::printl(a_rows);
      } else {
         h2_rows rows = h2_layout::split(e_rows, a_rows, "expect", "actual", 0, 'd', O.terminal_width - 1);
         for (auto& row : rows) row.indent(1);
         h2_color::printl(rows);
      }
   }
};

struct h2_fail_memcmp : h2_fail_unexpect {
   h2_vector<unsigned char> e_value, a_value;
   const int width, nbits;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, int width_, int nbits_, const h2_string& represent_, const h2_row& explain_ = {}, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_unexpect({}, represent_, explain_, file_, line_), e_value(e_value_, e_value_ + (nbits_ + 7) / 8), a_value(a_value_, a_value_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}

   void print(int subling_index, int child_index) override
   {
      h2_fail_unexpect::print(subling_index, child_index);
      h2_rows e_rows, a_rows;
      int bytes_per_row = 0;
      switch (width) {
      case 1: print_bits(e_rows, a_rows, bytes_per_row = 4); break;
      case 8: print_ints<unsigned char>(e_rows, a_rows, bytes_per_row = (O.terminal_width < 108 ? 8 : 16)); break;
      case 16: print_ints<unsigned short>(e_rows, a_rows, bytes_per_row = 16); break;
      case 32: print_ints<unsigned int>(e_rows, a_rows, bytes_per_row = 16); break;
      case 64: print_ints<unsigned long long>(e_rows, a_rows, bytes_per_row = 16); break;
      default: break;
      }
      h2_color::printl(h2_layout::split(e_rows, a_rows, "expect", "actual", bytes_per_row * 8 / width, 'x', O.terminal_width));
   }

   void print_bits(h2_rows& e_rows, h2_rows& a_rows, int bytes_per_row)
   {
      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_row e_row, a_row;
         for (int j = 0; j < bytes_per_row; ++j) {
            if (j) e_row.push_back(" ");
            if (j) a_row.push_back(" ");
            for (int k = 0; k < 8; ++k) {
               if ((i * bytes_per_row + j) * 8 + k < nbits) {
                  unsigned char e_val = (e_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  unsigned char a_val = (a_value[i * bytes_per_row + j] >> (7 - k)) & 0x1;
                  if (e_val == a_val) {
                     e_row.push_back(h2_string(e_val ? "1" : "0"));
                     a_row.push_back(h2_string(a_val ? "1" : "0"));
                  } else {
                     e_row.printf("green", e_val ? "1" : "0");
                     a_row.printf("bold,red", a_val ? "1" : "0");
                  }
               }
            }
         }
         e_rows.push_back(e_row);
         a_rows.push_back(a_row);
      }
   }

   template <typename T>
   void print_ints(h2_rows& e_rows, h2_rows& a_rows, int bytes_per_row)
   {
      char fmt[32];
      sprintf(fmt, "%%s%%0%dX", (int)sizeof(T) * 2);

      int rows = ::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (int i = 0; i < rows; ++i) {
         h2_row e_row, a_row;
         for (int j = 0; j < bytes_per_row; j += sizeof(T)) {
            if (i * bytes_per_row + j < e_value.size()) {
               T e_val = *(T*)(e_value.data() + (i * bytes_per_row + j));
               T a_val = *(T*)(a_value.data() + (i * bytes_per_row + j));
               char e_str[32], a_str[32];
               sprintf(e_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", e_val);
               sprintf(a_str, fmt, j ? (j / sizeof(T) == 8 ? "  " : " ") : "", a_val);
               if (e_val == a_val) {
                  e_row.push_back(e_str);
                  a_row.push_back(a_str);
               } else {
                  e_row.printf("green", e_str);
                  a_row.printf("bold,red", a_str);
               }
            }
         }
         e_rows.push_back(e_row);
         a_rows.push_back(a_row);
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
   h2_vector<std::pair<int, int>> sizes;
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, int size_, const h2_vector<std::pair<int, int>>& sizes_, const h2_backtrace& bt_allocate_, const char* where_, const char* file_, int line_)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), sizes(sizes_), where(where_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row = h2_stringify(ptr) + color(" memory leak ", "bold,red") + h2_stringify(size).brush("red") + " ";
      int i = 0, c = 0, n = 3;
      h2_row l;
      for (auto& p : sizes) {
         l += gray(comma_if(i++));
         if (!O.verbose && n < i) {
            l += gray("..." + h2_stringify(sizes.size() - n));
            break;
         }
         l += h2_stringify(p.first);
         if (1 < p.second) l += gray("x") + h2_stringify(p.second);
         c += p.second;
      }
      if (1 < c) row += gray("[") + l + gray("] ");
      h2_color::printl(" " + row + "bytes in " + where + " totally" + locate());
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::prints("", " %p", ptr);
      h2_color::prints("bold,red", " double free");
      h2_color::prints("", " at backtrace:\n", ptr), bt_double_free.print(2);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::prints("", "  already free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_asymmetric_free : h2_fail_memory {
   const char *who_allocate, *who_release;
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_color::prints("", " %p allocate with ", ptr);
      h2_color::prints("bold,red", "%s", who_allocate);
      h2_color::prints("", ", release by ");
      h2_color::prints("bold,red", "%s", who_release);
      h2_color::prints("", " asymmetrically at backtrace:\n"), bt_release.print(2);
      if (0 < bt_allocate.count) h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_overflow : h2_fail_memory {
   const void* violate_ptr;             /* 犯罪地点 */
   const char* action;                  /* 犯罪行为 */
   const h2_vector<unsigned char> spot; /* 犯罪现场 */
   const h2_backtrace bt_trample;       /* 犯罪过程 */
   h2_fail_overflow(const void* ptr_, const int size_, const void* violate_ptr_, const char* action_, const h2_vector<unsigned char>& spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_, const char* file_ = nullptr, int line_ = 0)
     : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), file_, line_), violate_ptr(violate_ptr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      int offset = ptr < violate_ptr ? (long long)violate_ptr - ((long long)ptr + size) : (long long)violate_ptr - (long long)ptr;
      h2_row row = h2_stringify(ptr) + " " + color(h2_string("%+d", offset), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + " " + (offset >= 0 ? "overflow" : "underflow") + " ";
      for (int i = 0; i < spot.size(); ++i) row.printf("bold,red", "%02X ", spot[i]);
      h2_color::printl(" " + row + locate());
      if (bt_trample.count) h2_color::prints("", "  trampled at backtrace:\n"), bt_trample.print(3);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_use_after_free : h2_fail_memory {
   const void* violate_ptr;   /* 犯罪地点 */
   const char* action;        /* 犯罪行为 */
   const h2_backtrace bt_use; /* 犯罪过程 */
   h2_fail_use_after_free(const void* ptr_, const void* violate_ptr_, const char* action_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_use_)
     : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), violate_ptr(violate_ptr_), action(action_), bt_use(bt_use_) {}
   void print(int subling_index = 0, int child_index = 0) override
   {
      h2_row row = h2_stringify(ptr) + " " + color(h2_string("%+d", (long long)violate_ptr - (long long)ptr), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + color(" after free", "bold,red");
      h2_color::printl(" " + row + " at backtrace:"), bt_use.print(2);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::prints("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_row& explain, const char* file, int line)
{
   return new h2_fail_normal(explain, file, line);
}
h2_inline h2_fail* h2_fail::new_unexpect(const h2_row& expection, const h2_row& represent, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_unexpect(expection, represent, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_row& expection, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_strcmp(e_value, a_value, caseless, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_row& expection, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_strfind(e_value, a_value, expection, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_row& expection, bool caseless, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_json(e_value, a_value, expection, caseless, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, int width, int nbits, const h2_string& represent, const h2_row& explain, const char* file, int line)
{
   return new h2_fail_memcmp(e_value, a_value, width, nbits, represent, explain, file, line);
}
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, int size, const h2_vector<std::pair<int, int>>& sizes, const h2_backtrace& bt_allocate, const char* where, const char* file, int line)
{
   return new h2_fail_memory_leak(ptr, size, sizes, bt_allocate, where, file, line);
}
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free)
{
   return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free);
}
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release)
{
   return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release);
}
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const int size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample, const char* file, int line)
{
   return new h2_fail_overflow(ptr, size, violate_ptr, action, spot, bt_allocate, bt_trample, file, line);
}
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use)
{
   return new h2_fail_use_after_free(ptr, violate_ptr, action, bt_allocate, bt_release, bt_use);
}
