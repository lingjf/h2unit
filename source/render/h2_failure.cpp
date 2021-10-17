h2_inline void h2_fail::append_subling(h2_fail*& fail, h2_fail* nf)
{
   if (!fail) {
      fail = nf;
   } else {
      h2_fail** pp = &fail->subling_next;
      while (*pp) pp = &(*pp)->subling_next;
      *pp = nf;
   }
}

h2_inline void h2_fail::append_child(h2_fail*& fail, h2_fail* nf)
{
   if (!fail) {
      fail = nf;
   } else {
      h2_fail** pp = &fail->child_next;
      while (*pp) pp = &(*pp)->child_next;
      *pp = nf;
   }
}

h2_inline h2_fail::~h2_fail()
{
   if (child_next) delete child_next;
   if (subling_next) delete subling_next;
}

h2_inline h2_line h2_fail::locate()
{
   if (!fs.empty())
      return gray(" at ") + h2_string("%s:%d", fs.basefile(), fs.line);
   return {};
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, size_t, size_t)> cb, size_t si, size_t ci)
{
   cb(this, si, ci);
   if (child_next) child_next->foreach(cb, 0, ci + 1);
   if (subling_next) subling_next->foreach(cb, si + 1, ci);
}

struct h2_fail_normal : h2_fail {
   h2_fail_normal(const h2_line& explain_ = {}, const h2_fs& fs_ = h2_fs()) : h2_fail(explain_, fs_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      line += explain;
      h2_color::printl(line + locate());
   }
};

static inline bool is_synonym(const h2_string& a, const h2_string& b)
{
   static const char* s_null[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0", "00000000", "0000000000000000", nullptr};
   static const char* s_true[] = {"IsTrue", "true", "TRUE", "True", "1", nullptr};
   static const char* s_false[] = {"IsFalse", "false", "FALSE", "False", "0", nullptr};
   static const char** S[] = {s_null, s_true, s_false};
   h2_string a1 = a.escape(), b1 = b.escape();
   if (a1 == b1) return true;
   for (size_t i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
      if (h2_in(a1.c_str(), S[i]) && h2_in(b1.c_str(), S[i]))
         return true;
   return false;
}

struct h2_fail_unexpect : h2_fail {
   h2_line expection, represent;
   int c = 0;
   h2_fail_unexpect(const h2_line& expection_ = {}, const h2_line& represent_ = {}, const h2_line& explain_ = {}, const h2_fs& fs_ = h2_fs()) : h2_fail(explain_, fs_), expection(expection_), represent(represent_) {}
   void print_OK1(h2_line& line)
   {
      h2_line a = h2_line(a_expression).gray_quote().brush("cyan");
      line += "OK" + gray("(") + a + gray(")") + " is " + color("false", "bold,red");
   }
   void print_OK2_CP(h2_line& line, const char* assert_type)
   {
      h2_line e, a;
      if (!expection.width()) {
         e = h2_line(e_expression).abbreviate(10000, 3).gray_quote().brush("green");
      } else if (is_synonym(e_expression, expection.string())) {
         e = expection.abbreviate(10000, 3).brush("green");
      } else {
         e = h2_line(e_expression).abbreviate(O.verbose >= 4 ? 10000 : 120, 3).gray_quote().brush("cyan") + gray("==>") + expection.abbreviate(10000, 3).brush("green");
      }  // https://unicode-table.com/en/sets/arrow-symbols/

      if (!represent.width()) {
         a = h2_line(a_expression).abbreviate(10000, 3).gray_quote().brush("bold,red");
      } else if (is_synonym(a_expression, represent.string()) || !a_expression.length()) {
         a = represent.abbreviate(10000, 3).brush("bold,red");
      } else {
         a = represent.abbreviate(10000, 3).brush("bold,red") + gray("<==") + h2_line(a_expression).abbreviate(O.verbose >= 4 ? 10000 : 120, 3).gray_quote().brush("cyan");
      }

      line += assert_type + gray("(") + e + " " + assert_op + " " + a + gray(")");
   }
   void print_JE(h2_line& line)
   {
      h2_line e = h2_line(e_expression.unquote('\"').unquote('\'')).abbreviate(O.verbose >= 4 ? 10000 : 30, 2).brush("cyan");
      h2_line a = h2_line(a_expression.unquote('\"').unquote('\'')).abbreviate(O.verbose >= 4 ? 10000 : 30, 2).brush("bold,red");
      line += "JE" + gray("(") + e + ", " + a + gray(")");
   }
   void print_Inner(h2_line& line)
   {
      if (0 <= seqno) line.printf("dark gray", "%d. ", seqno);
      if (expection.width()) {
         line.printf("", "%sexpect is ", comma_if(c++));
         line += expection.abbreviate(O.verbose >= 4 ? 10000 : 120, 3).brush("green");
      }
      if (represent.width()) {
         line.printf("", "%sactual is ", comma_if(c++));
         line += represent.abbreviate(O.verbose >= 4 ? 10000 : 120, 3).brush("bold,red");
      }
   }

   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line;
      line.indent(ci * 2 + 1);
      if (!strcmp("Inner", assert_type)) print_Inner(line);
      if (!strcmp("OK1", assert_type)) print_OK1(line);
      if (!strcmp("OK", assert_type) || !strcmp("CP", assert_type)) print_OK2_CP(line, assert_type);
      if (!strcmp("JE", assert_type)) print_JE(line);
      if (explain.width()) line += comma_if(c++, ", ", " ") + explain;
      if (user_explain.size()) line += {comma_if(c++, ", ", " "), user_explain};
      h2_color::printl(line + locate());
   }
};

struct h2_fail_strcmp : h2_fail_unexpect {
   const bool caseless;
   const h2_string e_value, a_value;
   h2_fail_strcmp(const h2_string& e_value_, const h2_string& a_value_, bool caseless_, const h2_line& expection, const h2_line& explain = {}) : h2_fail_unexpect(expection, h2_representify(a_value_), explain), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   h2_line fmt_char(h2_string& c, bool eq, const char* style)
   {
      if (c.equals(" ") && O.colorful) return gray("‧");
      if (eq) return c.escape();
      return color(c.escape(), style);
   }
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (16 < e_value.width() || 16 < a_value.width()) {
         h2_line e_line, a_line;
         h2_vector<h2_string> e_chars = e_value.disperse(), a_chars = a_value.disperse();
         auto lcs = h2_LCS(e_chars, a_chars, caseless).lcs();
         for (size_t i = 0; i < lcs.first.size(); i++) e_line += fmt_char(e_chars[i], lcs.first[i], "green");
         for (size_t i = 0; i < lcs.second.size(); i++) a_line += fmt_char(a_chars[i], lcs.second[i], "red");
         h2_color::printl(h2_layout::unified(e_line, a_line, "expect", "actual", h2_shell::I().cww));
      }
   }
};

struct h2_fail_strfind : h2_fail_unexpect {
   const h2_string e_value, a_value;
   h2_fail_strfind(const h2_string& e_value_, const h2_string& a_value_, const h2_line& expection, const h2_line& explain) : h2_fail_unexpect(expection, h2_representify(a_value_), explain), e_value(e_value_), a_value(a_value_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_fail_unexpect::print(si, ci);

      if (16 < e_value.width() || 16 < a_value.width()) {
         h2_line e_line = e_value.escape(), a_line = a_value.escape();
         h2_color::printl(h2_layout::seperate(e_line, a_line, "expect", "actual", h2_shell::I().cww));
      }
   }
};

struct h2_fail_json : h2_fail_unexpect {
   const bool caseless;
   const h2_string e_value, a_value;
   h2_fail_json(const h2_string& e_value_, const h2_string& a_value_, const h2_line& expection_, bool caseless_, const h2_line& explain_) : h2_fail_unexpect(expection_, a_value_, explain_), caseless(caseless_), e_value(e_value_), a_value(a_value_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_lines e_lines, a_lines;
      h2_fail_unexpect::print(si, ci);
      if (O.copy_paste_json || !h2_json::diff(e_value, a_value, e_lines, a_lines, caseless)) {
         e_lines = h2_json::dump(e_value);
         a_lines = h2_json::dump(a_value);
         for (size_t i = 0; i < e_lines.size(); ++i)
            if (i) e_lines[i].indent(8);
         for (size_t i = 0; i < a_lines.size(); ++i)
            if (i) a_lines[i].indent(8);
         h2_color::prints("dark gray", "expect");
         h2_color::prints("green", "> ");
         h2_color::printl(e_lines);
         h2_color::prints("dark gray", "actual");
         h2_color::prints("red", "> ");
         h2_color::printl(a_lines);
      } else {
         h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual", 0, 'd', h2_shell::I().cww - 1);
         for (auto& line : lines) line.indent(1);
         h2_color::printl(lines);
      }
   }
};

struct h2_fail_memcmp : h2_fail_unexpect {
   const h2_vector<unsigned char> e_value, a_value;
   const size_t length, width;
   h2_fail_memcmp(const unsigned char* e_value_, const unsigned char* a_value_, const size_t length_, const size_t width_) : h2_fail_unexpect({}, {}, {}), e_value(e_value_, e_value_ + (length_ * width_ + 7) / 8), a_value(a_value_, a_value_ + (length_ * width_ + 7) / 8), length(length_), width(width_) {}
   void print(size_t si, size_t ci) override
   {
      expection.printf("", "memcmp %d %s", (int)length, format_width());
      h2_fail_unexpect::print(si, ci);
      h2_lines e_lines, a_lines;
      size_t bytes_per_row = 0;
      switch (width) {
         case 1: print_bits(e_lines, a_lines, bytes_per_row = 4); break;
         case 8: print_ints<unsigned char>(e_lines, a_lines, bytes_per_row = (h2_shell::I().cww < 108 ? 8 : 16)); break;
         case 16: print_ints<unsigned short>(e_lines, a_lines, bytes_per_row = 16); break;
         case 32: print_ints<unsigned int>(e_lines, a_lines, bytes_per_row = 16); break;
         case 64: print_ints<unsigned long long>(e_lines, a_lines, bytes_per_row = 16); break;
         default: break;
      }
      h2_color::printl(h2_layout::split(e_lines, a_lines, "expect", "actual", bytes_per_row * 8 / width, 'x', h2_shell::I().cww));
   }

   const char* format_width()
   {
      switch (width) {
         case 1: return "bits";
         case 8: return "bytes";
         case 16: return "uint16";
         case 32: return "uint32";
         case 64: return "uint64";
      }
      return "";
   }

   void print_bits(h2_lines& e_lines, h2_lines& a_lines, size_t bytes_per_row)
   {
      size_t rows = (size_t)::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (size_t i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (size_t j = 0; j < bytes_per_row; ++j) {
            if (j) e_line.push_back(" ");
            if (j) a_line.push_back(" ");
            for (size_t k = 0; k < 8; ++k) {
               if ((i * bytes_per_row + j) * 8 + k < length) {
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
   void print_ints(h2_lines& e_lines, h2_lines& a_lines, size_t bytes_per_row)
   {
      char fmt[32];
      sprintf(fmt, "%%s%%0%dX", (int)sizeof(T) * 2);

      size_t rows = (size_t)::ceil(e_value.size() * 1.0 / bytes_per_row);
      for (size_t i = 0; i < rows; ++i) {
         h2_line e_line, a_line;
         for (size_t j = 0; j < bytes_per_row; j += sizeof(T)) {
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
   const size_t size;
   const h2_backtrace bt_allocate, bt_release;
   h2_fail_memory(const void* ptr_, const size_t size_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_fs& fs_ = h2_fs()) : h2_fail({}, fs_), ptr(ptr_), size(size_), bt_allocate(bt_allocate_), bt_release(bt_release_) {}
};

struct h2_fail_memory_leak : h2_fail_memory {
   const h2_vector<std::pair<size_t, size_t>> sizes;
   const char* where;  // case or block
   h2_fail_memory_leak(const void* ptr_, const size_t size_, const h2_vector<std::pair<size_t, size_t>>& sizes_, const h2_backtrace& bt_allocate_, const char* where_, const h2_fs& fs_) : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace(), fs_), sizes(sizes_), where(where_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line line = h2_stringify(ptr) + color(" memory leak ", "bold,red") + h2_stringify(size).brush("red") + " ";
      size_t i = 0, c = 0, n = 3;
      h2_line sl;
      for (auto& p : sizes) {
         sl += gray(comma_if(i++));
         if (O.verbose <= 1 && n < i) {
            sl += gray("..." + h2_stringify(sizes.size() - n));
            break;
         }
         sl += h2_stringify(p.first);
         if (1 < p.second) sl += gray("x") + h2_stringify(p.second);
         c += p.second;
      }
      if (1 < c) line += gray("[") + sl + gray("] ");
      h2_color::printl(" " + line + "bytes in " + where + " totally" + locate());
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_double_free : h2_fail_memory {
   const h2_backtrace bt_double_free;
   h2_fail_double_free(const void* ptr_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_double_free_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), bt_double_free(bt_double_free_) {}
   void print(size_t si = 0, size_t ci = 0) override
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
   h2_fail_asymmetric_free(const void* ptr_, const char* who_allocate_, const char* who_release_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), who_allocate(who_allocate_), who_release(who_release_) {}
   void print(size_t si = 0, size_t ci = 0) override
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
   h2_fail_overflow(const void* ptr_, const size_t size_, const void* violate_ptr_, const char* action_, const h2_vector<unsigned char>& spot_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_trample_) : h2_fail_memory(ptr_, size_, bt_allocate_, h2_backtrace()), violate_ptr(violate_ptr_), action(action_), spot(spot_), bt_trample(bt_trample_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      long long offset = ptr < violate_ptr ? (long long)violate_ptr - ((long long)ptr + size) : (long long)violate_ptr - (long long)ptr;
      h2_line t = h2_stringify(ptr) + " " + color(h2_string("%+d", (int)offset), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + " " + (offset >= 0 ? "overflow" : "underflow") + " ";
      for (size_t i = 0; i < spot.size(); ++i) t.printf("bold,red", "%02X ", spot[i]);
      h2_color::printl(" " + t + locate() + (bt_trample.count ? " at backtrace:" : ""));
      if (bt_trample.count) bt_trample.print(3);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   }
};

struct h2_fail_use_after_free : h2_fail_memory {
   const void* violate_ptr;   /* 犯罪地点 */
   const char* action;        /* 犯罪行为 */
   const h2_backtrace bt_use; /* 犯罪过程 */
   h2_fail_use_after_free(const void* ptr_, const void* violate_ptr_, const char* action_, const h2_backtrace& bt_allocate_, const h2_backtrace& bt_release_, const h2_backtrace& bt_use_) : h2_fail_memory(ptr_, 0, bt_allocate_, bt_release_), violate_ptr(violate_ptr_), action(action_), bt_use(bt_use_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_line t = h2_stringify(ptr) + " " + color(h2_string("%+d", (long long)violate_ptr - (long long)ptr), "bold,red") + " " + gray("(") + h2_stringify(violate_ptr) + gray(")") + " " + color(action, "bold,red") + color(" after free", "bold,red");
      h2_color::printl(" " + t + " at backtrace:"), bt_use.print(2);
      h2_color::prints("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
      h2_color::prints("", "  and free at backtrace:\n"), bt_release.print(3);
   }
};

struct h2_fail_exception : h2_fail {
   const char* type;
   const h2_backtrace bt_throw;
   h2_fail_exception(const h2_line& explain_, const char* type_, const h2_backtrace& bt_throw_) : h2_fail(explain_, h2_fs()), type(type_), bt_throw(bt_throw_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_color::printl(" exception " + color(type, "red") + " " + explain + " at backtrace:");
      bt_throw.print(3);
   }
};

struct h2_fail_symbol : h2_fail {
   const h2_string symbol;
   const h2_vector<h2_string> candidates;
   h2_fail_symbol(const h2_string& symbol_, const h2_vector<h2_string>& candidates_, const h2_line& explain_) : h2_fail(explain_, h2_fs()), symbol(symbol_), candidates(candidates_) {}
   void print(size_t si = 0, size_t ci = 0) override
   {
      h2_color::printl(color(candidates.size() ? " Find multiple " : " Not found ", "yellow") + color(symbol, "bold,red"));
      for (size_t i = 0; i < candidates.size(); ++i)
         h2_color::printl("  " + gray(h2_stringify(i) + ". ") + color(candidates[i], "yellow"));
      if (explain.width()) h2_color::printl(explain);
   }
};

h2_inline h2_fail* h2_fail::new_normal(const h2_line& explain_, const h2_fs& fs_) { return new h2_fail_normal(explain_, fs_); }
h2_inline h2_fail* h2_fail::new_unexpect(const h2_line& expection_, const h2_line& represent_, const h2_line& explain_) { return new h2_fail_unexpect(expection_, represent_, explain_); }
h2_inline h2_fail* h2_fail::new_strcmp(const h2_string& e_value, const h2_string& a_value, bool caseless, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strcmp(e_value, a_value, caseless, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_strfind(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, const h2_line& explain_) { return new h2_fail_strfind(e_value, a_value, expection_, explain_); }
h2_inline h2_fail* h2_fail::new_json(const h2_string& e_value, const h2_string& a_value, const h2_line& expection_, bool caseless, const h2_line& explain_) { return new h2_fail_json(e_value, a_value, expection_, caseless, explain_); }
h2_inline h2_fail* h2_fail::new_memcmp(const unsigned char* e_value, const unsigned char* a_value, const size_t length, const size_t width) { return new h2_fail_memcmp(e_value, a_value, length, width); }
h2_inline h2_fail* h2_fail::new_memory_leak(const void* ptr, const size_t size, const h2_vector<std::pair<size_t, size_t>>& sizes, const h2_backtrace& bt_allocate, const char* where, const h2_fs& fs_) { return new h2_fail_memory_leak(ptr, size, sizes, bt_allocate, where, fs_); }
h2_inline h2_fail* h2_fail::new_double_free(const void* ptr, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_double_free) { return new h2_fail_double_free(ptr, bt_allocate, bt_release, bt_double_free); }
h2_inline h2_fail* h2_fail::new_asymmetric_free(const void* ptr, const char* who_allocate, const char* who_release, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release) { return new h2_fail_asymmetric_free(ptr, who_allocate, who_release, bt_allocate, bt_release); }
h2_inline h2_fail* h2_fail::new_overflow(const void* ptr, const size_t size, const void* violate_ptr, const char* action, const h2_vector<unsigned char>& spot, const h2_backtrace& bt_allocate, const h2_backtrace& bt_trample) { return new h2_fail_overflow(ptr, size, violate_ptr, action, spot, bt_allocate, bt_trample); }
h2_inline h2_fail* h2_fail::new_use_after_free(const void* ptr, const void* violate_ptr, const char* action, const h2_backtrace& bt_allocate, const h2_backtrace& bt_release, const h2_backtrace& bt_use) { return new h2_fail_use_after_free(ptr, violate_ptr, action, bt_allocate, bt_release, bt_use); }
h2_inline h2_fail* h2_fail::new_exception(const char* explain_, const char* type, const h2_backtrace& bt_throw) { return new h2_fail_exception(explain_, type, bt_throw); }
h2_inline h2_fail* h2_fail::new_symbol(const h2_string& symbol, const h2_vector<h2_string>& candidates, const h2_line& explain_) { return new h2_fail_symbol(symbol, candidates, explain_); };
