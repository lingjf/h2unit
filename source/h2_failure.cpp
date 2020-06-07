
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

h2_inline void h2_fail::set_locate(const char* file_, int line_, const char* func_, int argi_)
{
   file = file_, line = line_, func = func_, argi = argi_;
}

h2_inline const char* h2_fail::get_locate()
{
   static constexpr const char* a9 = "1st\0002nd\0003rd\0004th\0005th\0006th\0007th\0008th\0009th";
   static char st[1024];
   char* p = st;
   strcpy(p, "");
   if (func && strlen(func)) p += sprintf(p, " in %s(%s)", func, 0 <= argi && argi < 9 ? a9 + argi * 4 : "");
   if (file && strlen(file) && 0 < line) p += sprintf(p, " at %s:%d", file, line);
   return st;
}

h2_inline void h2_fail::foreach(std::function<void(h2_fail*, int, int)> cb, int subling_index, int child_index)
{
   cb(this, subling_index, child_index);
   if (child_next) child_next->foreach(cb, 0, child_index + 1);
   if (subling_next) subling_next->foreach(cb, subling_index + 1, child_index);
}

h2_inline h2_fail_normal::h2_fail_normal(const char* file_, int line_, const char* func_, const char* format, ...) : h2_fail(file_, line_, func_)
{
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   explain = alloca_str;
}

h2_inline void h2_fail_normal::print(int subling_index, int child_index)
{
   h2_line line;
   line.indent(child_index * 2 + 1);
   if (no.size()) line.printf("dark gray", "%s. ", no.c_str());
   line.printf("", "%s,%s", explain.c_str(), get_locate());
   h2_color::printf(line);
}

h2_inline void h2_fail_unexpect::print_OK1(h2_line& line)
{
   line.push_back("OK( ");
   line.printf("cyan", "%s", a_expression.acronym(O.verbose ? 10000 : 30).c_str());
   line.push_back(" ) is ");
   line.printf("bold,red", "false");
}

static inline bool is_synonym(h2_string& a, h2_string& b)
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

h2_inline void h2_fail_unexpect::print_OK2(h2_line& line)
{
   line.push_back("OK( ");

   if (!expection.size() || e_expression == expection || is_synonym(e_expression, expection)) {
      line.printf("green", e_expression.acronym(O.verbose ? 10000 : 30, 1).c_str());
   } else {
      line.printf("cyan", e_expression.acronym(O.verbose ? 10000 : 16).c_str());
      line.printf("dark gray", "==>");
      line.printf("green", expection.acronym(O.verbose ? 10000 : 30, 1).c_str());
   }

   line.push_back(", ");

   if (!a_represent.size() || a_expression == a_represent || is_synonym(a_expression, a_represent)) {
      line.printf("bold,red", a_expression.acronym(O.verbose ? 10000 : 30, 1).c_str());
   } else {
      line.printf("bold,red", a_represent.acronym(O.verbose ? 10000 : 30, 1).c_str());
      line.printf("dark gray", "<==");
      line.printf("cyan", a_expression.acronym(O.verbose ? 10000 : 16).c_str());
   }

   line.push_back(" )");
}

h2_inline void h2_fail_unexpect::print_JE(h2_line& line)
{
   line.push_back("JE( ");
   line.printf("cyan", "%s", e_expression.acronym(O.verbose ? 10000 : 30).c_str());
   line.push_back(", ");
   line.printf("bold,red", "%s", a_expression.acronym(O.verbose ? 10000 : 30).c_str());
   line.push_back(" )");
}

h2_inline void h2_fail_unexpect::print_Inner(h2_line& line)
{
   if (no.size()) line.printf("dark gray", "%s. ", no.c_str());
   line.push_back("expect is ");
   line.printf("green", expection.acronym(O.verbose ? 10000 : 30, 1).c_str());
   line.push_back(", actual is ");
   line.printf("bold,red", a_represent.acronym(O.verbose ? 10000 : 30, 1).c_str());
}

h2_inline void h2_fail_unexpect::print(int subling_index, int child_index)
{
   h2_line line;
   line.indent(child_index * 2 + 1);
   if (usage == 0) print_Inner(line);
   if (usage == 1) print_OK1(line);
   if (usage == 2) print_OK2(line);
   if (usage == 3) print_JE(line);
   if (explain.size()) line.printf("", " %s,", explain.c_str());
   if (user_explain.size()) line.printf("", " %s,", user_explain.c_str());
   line.push_back(get_locate());
   h2_color::printf(line);
}

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

h2_inline void h2_fail_strcmp::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);

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

   h2_lines lines = h2_layout::unified(e_line, a_line, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_strfind::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);

   h2_line e_line, a_line;
   for (size_t i = 0; i < e_value.size(); ++i)
      fmt_char(e_value[i], true, "", e_line);

   for (size_t i = 0; i < a_value.size(); ++i)
      fmt_char(a_value[i], true, "", a_line);

   h2_lines lines = h2_layout::seperate(e_line, a_line, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_json::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);
   h2_lines e_lines, a_lines;
   h2_json::diff(e_value, a_value, e_lines, a_lines);
   h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_memcmp::print(int subling_index, int child_index)
{
   h2_fail_unexpect::print(subling_index, child_index);
   h2_lines e_lines, a_lines;
   switch (width) {
   case 1: print_bits(e_lines, a_lines); break;
   case 8: print_bytes(e_lines, a_lines); break;
   case 16: print_int16(e_lines, a_lines); break;
   case 32: print_int32(e_lines, a_lines); break;
   case 64: print_int64(e_lines, a_lines); break;
   default: break;
   }
   h2_lines lines = h2_layout::split(e_lines, a_lines, "expect", "actual");
   h2_color::printf(lines);
}

h2_inline void h2_fail_memcmp::print_bits(h2_lines& e_lines, h2_lines& a_lines)
{
   int rows = ::ceil(e_value.size() * 1.0 / npr_1b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_1b; ++j) {
         for (int k = 0; k < 8; ++k) {
            if ((i * npr_1b + j) * 8 + k < nbits) {
               unsigned char e_val = (e_value[i * npr_1b + j] >> (7 - k)) & 0x1;
               unsigned char a_val = (a_value[i * npr_1b + j] >> (7 - k)) & 0x1;
               if (e_val == a_val) {
                  e_line.push_back(h2_string(e_val ? "1" : "0"));
                  a_line.push_back(h2_string(a_val ? "1" : "0"));
               } else {
                  e_line.printf("green", e_val ? "1" : "0");
                  a_line.printf("bold,red", a_val ? "1" : "0");
               }
            }
         }
         e_line.push_back(" ");
         a_line.push_back(" ");
      }
      e_lines.push_back(e_line);
      a_lines.push_back(a_line);
   }
}

h2_inline void h2_fail_memcmp::print_bytes(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size(), rows = ::ceil(count * 1.0 / npr_8b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_8b; ++j) {
         if (i * npr_8b + j < count) {
            char e_str[8], a_str[8];
            sprintf(e_str, j < 8 ? "%02X " : " %02X", e_value[i * npr_8b + j]);
            sprintf(a_str, j < 8 ? "%02X " : " %02X", a_value[i * npr_8b + j]);
            if (e_value[i * npr_8b + j] == a_value[i * npr_8b + j]) {
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

h2_inline void h2_fail_memcmp::print_int16(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size() / 2, rows = ::ceil(count * 1.0 / npr_16b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_16b; ++j) {
         if (i * npr_16b + j < count) {
            unsigned short e_val = *(unsigned short*)(e_value.data() + (i * npr_16b + j) * 2);
            unsigned short a_val = *(unsigned short*)(a_value.data() + (i * npr_16b + j) * 2);
            char e_str[32], a_str[32];
            sprintf(e_str, "%04X ", e_val);
            sprintf(a_str, "%04X ", a_val);
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

h2_inline void h2_fail_memcmp::print_int32(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size() / 4, rows = ::ceil(count * 1.0 / npr_32b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_32b; ++j) {
         if (i * npr_32b + j < count) {
            unsigned int e_val = *(unsigned int*)(e_value.data() + (i * npr_32b + j) * 4);
            unsigned int a_val = *(unsigned int*)(a_value.data() + (i * npr_32b + j) * 4);
            char e_str[32], a_str[32];
            sprintf(e_str, "%08X ", e_val);
            sprintf(a_str, "%08X ", a_val);
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

h2_inline void h2_fail_memcmp::print_int64(h2_lines& e_lines, h2_lines& a_lines)
{
   int count = e_value.size() / 8, rows = ::ceil(count * 1.0 / npr_64b);
   for (int i = 0; i < rows; ++i) {
      h2_line e_line, a_line;
      for (int j = 0; j < npr_64b; ++j) {
         if (i * npr_64b + j < count) {
            unsigned long long e_val = *(unsigned long long*)(e_value.data() + (i * npr_64b + j) * 8);
            unsigned long long a_val = *(unsigned long long*)(a_value.data() + (i * npr_64b + j) * 8);
            char e_str[32], a_str[32];
            sprintf(e_str, "%016llX ", e_val);
            sprintf(a_str, "%016llX ", a_val);
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

h2_inline void h2_fail_memory_leak::print(int subling_index, int child_index)
{
   h2_color::printf("", " %p", ptr);
   h2_color::printf("bold,red", " memory leak");
   h2_color::printf("red", " %d", size);
   h2_color::printf("", " bytes in %s totally%s\n", where, get_locate());
   h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
}

h2_inline void h2_fail_double_free::print(int subling_index, int child_index)
{
   h2_color::printf("", " %p", ptr);
   h2_color::printf("bold,red", " double free");
   h2_color::printf("", " at backtrace:\n", ptr), bt_double_free.print(2);
   h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   h2_color::printf("", "  already free at backtrace:\n"), bt_release.print(3);
}

h2_inline void h2_fail_asymmetric_free::print(int subling_index, int child_index)
{
   h2_color::printf("", " %p allocate with ", ptr);
   h2_color::printf("bold,red", "%s", who_allocate);
   h2_color::printf("", ", release by ");
   h2_color::printf("bold,red", "%s", who_release);
   h2_color::printf("", " asymmetrically at backtrace:\n"), bt_release.print(2);
   if (0 < bt_allocate.count) h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
}

h2_inline void h2_fail_overflow::print(int subling_index, int child_index)
{
   int offset = ptr < addr ? (long long)addr - ((long long)ptr + size) : (long long)addr - (long long)ptr;
   h2_color::printf("", " %p %+d (%p)", ptr, offset, addr);
   h2_color::printf("bold,red", " %s", action);
   h2_color::printf("", " %s ", offset >= 0 ? "overflow" : "underflow");

   for (int i = 0; i < spot.size(); ++i)
      h2_color::printf("bold,red", "%02X ", spot[i]);

   h2_color::printf("", ",%s\n", get_locate());
   if (bt_trample.count) h2_color::printf("", "  trampled at backtrace:\n"), bt_trample.print(3);
   h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
}

h2_inline void h2_fail_use_after_free::print(int subling_index, int child_index)
{
   h2_color::printf("", " %p %+d (%p)", ptr, (long long)addr - (long long)ptr, addr);
   h2_color::printf("bold,red", " %s after free", action);
   h2_color::printf("", " at backtrace:\n"), bt_use.print(2);
   h2_color::printf("", "  which allocate at backtrace:\n"), bt_allocate.print(3);
   h2_color::printf("", "  and free at backtrace:\n"), bt_release.print(3);
}

h2_inline void h2_fail_call::print(int subling_index, int child_index)
{
   h2_line line = {" ", e_who, "() "};
   if (e_call.size()) {
      line.push_back("expected ");
      line.printf("green", e_call.c_str());
      line.push_back(" calls but ");
   }
   line.printf("red,bold", a_call.c_str());
   line.push_back(" called");
   if (e_call.size()) line.push_back(" actually");
   line.push_back(get_locate());
   h2_color::printf(line);
}

h2_inline void h2_fail_instantiate::print(int subling_index, int child_index)
{
   why_abstract ? h2_color::printf("", "Instantiate 'class %s' is a abstract class%s\n", class_type, get_locate()) :
                  h2_color::printf("", "Instantiate 'class %s' don't know initialize arguments%s\n", class_type, get_locate());

   h2_color::printf("", "You may take following solutions to fix it: \n");
   if (why_abstract)
      h2_color::printf("", "1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s, Derived %s(...)) \n",
                       action_type,
                       strlen(return_type) ? return_type : "",
                       strlen(return_type) ? ", " : "",
                       class_type, method_name, return_args, class_type);
   else {
      h2_color::printf("", "1. Define default constructor in class %s, or \n", class_type);
      h2_color::printf("", "2. Add parameterized construction in %s(%s%s%s, %s, %s, %s(...)) \n",
                       action_type,
                       strlen(return_type) ? return_type : "",
                       strlen(return_type) ? ", " : "",
                       class_type, method_name, return_args, class_type);
   }
}
