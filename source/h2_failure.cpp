
#define _H2_XPRINTF(value, format)           \
   do {                                      \
      char t[1024 * 8];                      \
      va_list args;                          \
      va_start(args, format);                \
      vsnprintf(t, sizeof(t), format, args); \
      va_end(args);                          \
      value = value + t;                     \
   } while (0)

h2_inline void h2_fail::append_x(h2_fail*& fail, h2_fail* n)
{
   if (!fail)
      fail = n;
   else {
      h2_fail** pp = &fail->x_next;
      while (*pp) pp = &(*pp)->x_next;
      *pp = n;
   }
}

h2_inline void h2_fail::append_y(h2_fail*& fail, h2_fail* n)
{
   if (!fail)
      fail = n;
   else {
      h2_fail** pp = &fail->y_next;
      while (*pp) pp = &(*pp)->y_next;
      *pp = n;
   }
}

h2_inline h2_fail::h2_fail(const char* file_, int line_, const char* func_, int argi_)
  : x_next(nullptr), y_next(nullptr), file(file_), line(line_), func(func_), argi(argi_), pad(0), w_type(0) {}

h2_inline h2_fail::~h2_fail()
{
   if (y_next) delete y_next;
   if (x_next) delete x_next;
}

h2_inline void h2_fail::locate(const char* file_, int line_, const char* func_, int argi_)
{
   file = file_, line = line_, func = func_, argi = argi_;
   if (y_next) y_next->locate(file_, line_, func_, argi_);
}

h2_inline void h2_fail::set_w_type(int w_type_)
{
   w_type = w_type_;
   if (y_next) y_next->set_w_type(w_type_);
}

h2_inline void h2_fail::set_e_expr(const char* e_expr_)
{
   e_expr = e_expr_;
   if (y_next) y_next->set_e_expr(e_expr_);
}

h2_inline void h2_fail::set_a_expr(const char* a_expr_)
{
   a_expr = a_expr_;
   if (y_next) y_next->set_a_expr(a_expr_);
}

h2_inline void h2_fail::kprintf(const char* format, ...) { _H2_XPRINTF(_k, format); }
h2_inline void h2_fail::hprintf(const char* format, ...) { _H2_XPRINTF(_h, format); }
h2_inline void h2_fail::eprintf(const char* format, ...) { _H2_XPRINTF(_e, format); }
h2_inline void h2_fail::aprintf(const char* format, ...) { _H2_XPRINTF(_a, format); }
h2_inline void h2_fail::mprintf(const char* format, ...) { _H2_XPRINTF(_m, format); }
h2_inline void h2_fail::uprintf(const char* format, ...) { _H2_XPRINTF(_u, format); }

h2_inline void h2_fail::print_locate()
{
   static constexpr const char* a9 = "1st\0002nd\0003rd\0004th\0005th\0006th\0007th\0008th\0009th";
   if (func && strlen(func)) h2_printf(", in %s(%s)", func, 0 <= argi && argi < 9 ? a9 + argi * 4 : "");
   if (file && strlen(file) && 0 < line) h2_printf(", at %s:%d", file, line);
   h2_printf("\n");
}

h2_inline void h2_fail::print() { _k.size() && h2_printf("%s%s", PAD(++pad), _k.c_str()); }
h2_inline void h2_fail::print(FILE* fp) { fprintf(fp, "%s", _k.c_str()); }

h2_inline h2_fail_normal::h2_fail_normal(const char* file_, int line_, const char* func_, const char* format, ...)
  : h2_fail(file_, line_, func_) { _H2_XPRINTF(_k, format); }

h2_inline void h2_fail_normal::print() { h2_fail::print(), print_locate(); }

h2_inline h2_fail_unexpect::h2_fail_unexpect(const char* file_, int line_) : h2_fail(file_, line_) {}

h2_inline void h2_fail_unexpect::print_OK1()
{
   h2_printf(" OK(%s) is %s", a_expr.c_str(), SF("bold,red", "false"));
}

static inline bool __both_inset(const char* set[], int n, const char* a, const char* b)
{
   int ca = 0, cb = 0;
   for (int i = 0; i < n; ++i) {
      if (!strcmp(set[i], a)) ca++;
      if (!strcmp(set[i], b)) cb++;
   }
   return ca > 0 && cb > 0;
}
static inline bool __is_synonym(const char* a, const char* b)
{
   static const char* set1[] = {"NULL", "__null", "((void *)0)", "(nil)", "nullptr", "0", "0x0"};
   static const char* set2[] = {"true", "1"};
   static const char* set3[] = {"false", "0"};
   return __both_inset(set1, sizeof(set1) / sizeof(set1[0]), a, b) ||
          __both_inset(set2, sizeof(set2) / sizeof(set2[0]), a, b) ||
          __both_inset(set3, sizeof(set3) / sizeof(set3[0]), a, b);
}

h2_inline void h2_fail_unexpect::print_OK2()
{
   char t1[256] = {0}, t2[256] = {0};
   if (e_expr == _e) /* OK(1, ret) */
      strcpy(t1, SF("green", "%s", _e.acronym().c_str()));
   else /* OK(Eq(1), ret) */
      strcpy(t1, e_expr.acronym().c_str());

   if (a_expr == _a)
      strcpy(t2, SF("bold,red", "%s", _a.acronym().c_str()));
   else {
      if (_a.length() && !__is_synonym(a_expr.c_str(), _a.c_str())) sprintf(t2, "%s%s", SF("bold,red", "%s", _a.acronym().c_str()), SF("dark gray", "<=="));
      strcpy(t2 + strlen(t2), a_expr.acronym().c_str());
   }

   h2_printf(" OK(%s, %s)", t1, t2);
   if (_m.length()) h2_printf(" actual %s", _m.c_str());
}

h2_inline void h2_fail_unexpect::print_JE()
{
   h2_printf(" JE(%s, %s)", e_expr.acronym().c_str(), a_expr.acronym().c_str());
   if (_m.length()) h2_printf(" actual %s expect", _m.c_str());
}

h2_inline void h2_fail_unexpect::print_MOCK()
{
   h2_printf(" actual %s", SF("green", "%s", _a.acronym().c_str()));
   if (_m.length()) h2_printf(" %s", _m.c_str());
   if (_e.length()) h2_printf(" %s", SF("bold,red", "%s", _e.acronym().c_str()));
}

h2_inline void h2_fail_unexpect::print()
{
   h2_fail::print(); /* nothing */
   if (w_type == 0) print_MOCK();
   if (w_type == 1) print_OK1();
   if (w_type == 2) print_OK2();
   if (w_type == 3) print_JE();
   if (_u.length()) h2_printf(", %s", _u.c_str());
   print_locate();
}

h2_inline h2_fail_strcmp::h2_fail_strcmp(const h2_string& expect_, const h2_string& actual_, bool caseless_, const char* file_, int line_)
  : h2_fail_unexpect(file_, line_), expect(expect_), actual(actual_), caseless(caseless_) {}

h2_inline void h2_fail_strcmp::print()
{
   h2_fail_unexpect::print();

   int columns = h2_term_size() - 12;
   int rows = ::ceil(std::max(expect.length(), actual.length()) / (double)columns);
   for (int i = 0; i < rows; ++i) {
      char eline[1024 * 32], aline[1024 * 32], *ep = eline, *ap = aline;
      if (i * columns <= expect.length()) ep += sprintf(ep, "%s%s ", SF("dark gray", "expect"), SF("green", ">"));
      if (i * columns <= actual.length()) ap += sprintf(ap, "%s%s ", SF("dark gray", "actual"), SF("red", ">"));
      for (int j = 0; j < columns; ++j) {
         char ec = i * columns + j <= expect.length() ? expect[i * columns + j] : ' ';
         char ac = i * columns + j <= actual.length() ? actual[i * columns + j] : ' ';

         bool eq = caseless ? ::tolower(ec) == ::tolower(ac) : ec == ac;
         ep = fmt_char(ec, eq, "green", ep);
         ap = fmt_char(ac, eq, "red,bold", ap);
      }
      if (i * columns <= expect.length()) h2_printf("%s\n", eline);
      if (i * columns <= actual.length()) h2_printf("%s\n", aline);
   }
}

h2_inline char* h2_fail_strcmp::fmt_char(char c, bool eq, const char* style, char* p)
{
   char st[64] = "", cc = c;
   if (!eq) strcpy(st, style);
   if (c == '\n') cc = 'n', strcat(st, ",inverse");
   if (c == '\r') cc = 'r', strcat(st, ",inverse");
   if (c == '\t') cc = 't', strcat(st, ",inverse");
   if (c == '\0') cc = ' ', eq || strcat(st, ",inverse");
   return p += sprintf(p, "%s", SF(st, "%c", cc));
}

h2_inline h2_fail_json::h2_fail_json(const h2_string& expect_, const h2_string& actual_, const char* file_, int line_)
  : h2_fail_unexpect(file_, line_), expect(expect_), actual(actual_) { _e = expect_, _a = actual_; }

h2_inline void h2_fail_json::print()
{
   h2_fail_unexpect::print();
   h2_string str;
   int side_width = h2_json::diff(expect, actual, h2_term_size(), str);

   h2_printf("%s\n", SF("dark gray", "%s│%s", h2_string("expect").center(side_width).c_str(), h2_string("actual").center(side_width).c_str()));
   h2_printf("%s", str.c_str());
}

h2_inline h2_fail_memcmp::h2_fail_memcmp(const unsigned char* expect_, const unsigned char* actual_, int width_, int nbits_, const char* file_, int line_)
  : h2_fail_unexpect(file_, line_), expect(expect_, expect_ + (nbits_ + 7) / 8), actual(actual_, actual_ + (nbits_ + 7) / 8), width(width_), nbits(nbits_) {}

h2_inline void h2_fail_memcmp::print()
{
   h2_fail_unexpect::print();
   switch (width) {
   case 1: print_bits(); break;
   case 8: print_bytes(); break;
   case 16: print_int16(); break;
   case 32: print_int32(); break;
   case 64: print_int64(); break;
   default: break;
   }
}

h2_inline char* h2_fail_memcmp::fmt_bit(unsigned char c, unsigned char t, const char* style, char* p)
{
   return p += sprintf(p, "%s", SF(c != t ? style : "", "%1d", c));
}

h2_inline void h2_fail_memcmp::print_bits()
{
   h2_printf("%s \n", SF("dark gray", "%s  │  %s", h2_string("expect").center(npr_1b * 9).c_str(), h2_string("actual").center(npr_1b * 9).c_str()));
   int bytes = expect.size(), rows = ::ceil(bytes * 1.0 / npr_1b);
   for (int i = 0; i < rows; ++i) {
      char eline[1024], aline[1024], *ep = eline, *ap = aline;
      for (int j = 0; j < npr_1b; ++j) {
         for (int k = 0; k < 8; ++k) {
            if (nbits <= (i * npr_1b + j) * 8 + k)
               ep += sprintf(ep, " "), ap += sprintf(ap, " ");
            else {
               unsigned char ebit = (expect[i * npr_1b + j] >> (7 - k)) & 0x1;
               unsigned char abit = (actual[i * npr_1b + j] >> (7 - k)) & 0x1;
               ep = fmt_bit(ebit, abit, "green", ep);
               ap = fmt_bit(abit, ebit, "bold,red", ap);
            }
         }
         ep += sprintf(ep, " "), ap += sprintf(ap, " ");
      }
      h2_printf("%s  %s  %s \n", eline, SF("dark gray", "│"), aline);
   }
}

h2_inline char* h2_fail_memcmp::fmt_byte(unsigned char c, unsigned char t, int j, const char* style, char* p)
{
   return p += sprintf(p, "%s", SF(c != t ? style : "", j < 8 ? "%02X " : " %02X", c));
}

h2_inline void h2_fail_memcmp::print_bytes()
{
   h2_printf("%s \n", SF("dark gray", "%s  │  %s", h2_string("expect").center(npr_8b * 3).c_str(), h2_string("actual").center(npr_8b * 3).c_str()));
   int bytes = expect.size(), rows = ::ceil(bytes * 1.0 / npr_8b);
   for (int i = 0; i < rows; ++i) {
      char eline[1024], aline[1024], *ep = eline, *ap = aline;
      for (int j = 0; j < npr_8b; ++j) {
         if (bytes <= i * npr_8b + j)
            ep += sprintf(ep, "   "), ap += sprintf(ap, "   ");
         else {
            ep = fmt_byte(expect[i * npr_8b + j], actual[i * npr_8b + j], j, "green", ep);
            ap = fmt_byte(actual[i * npr_8b + j], expect[i * npr_8b + j], j, "bold,red", ap);
         }
      }
      h2_printf("%s  %s  %s \n", eline, SF("dark gray", "│"), aline);
   }
}

h2_inline char* h2_fail_memcmp::fmt_int16(unsigned short c, unsigned short t, const char* style, char* p)
{
   return p += sprintf(p, "%s", SF(c != t ? style : "", "%04X ", c));
}

h2_inline void h2_fail_memcmp::print_int16()
{
   h2_printf("%s \n", SF("dark gray", "%s │  %s", h2_string("expect").center(npr_16b * 5).c_str(), h2_string("actual").center(npr_16b * 5).c_str()));
   int ints = expect.size() / 2, rows = ::ceil(ints * 1.0 / npr_16b);
   for (int i = 0; i < rows; ++i) {
      char eline[1024], aline[1024], *ep = eline, *ap = aline;
      for (int j = 0; j < npr_16b; ++j) {
         if (ints <= i * npr_16b + j)
            ep += sprintf(ep, "     "), ap += sprintf(ap, "     ");
         else {
            unsigned char e16[2] = {expect[(i * npr_16b + j) * 2 + 0], expect[(i * npr_16b + j) * 2 + 1]};
            unsigned char a16[2] = {actual[(i * npr_16b + j) * 2 + 0], actual[(i * npr_16b + j) * 2 + 1]};
            ep = fmt_int16(*(unsigned char*)e16, *(unsigned char*)a16, "green", ep);
            ap = fmt_int16(*(unsigned char*)a16, *(unsigned char*)e16, "bold,red", ap);
         }
      }
      h2_printf("%s %s  %s \n", eline, SF("dark gray", "│"), aline);
   }
}

h2_inline char* h2_fail_memcmp::fmt_int32(unsigned long c, unsigned long t, const char* style, char* p)
{
   return p += sprintf(p, "%s", SF(c != t ? style : "", "%08X ", c));
}

h2_inline void h2_fail_memcmp::print_int32()
{
   h2_printf("%s \n", SF("dark gray", "%s │  %s", h2_string("expect").center(npr_32b * 9).c_str(), h2_string("actual").center(npr_32b * 9).c_str()));
   int ints = expect.size() / 4, rows = ::ceil(ints * 1.0 / npr_32b);
   for (int i = 0; i < rows; ++i) {
      char eline[1024], aline[1024], *ep = eline, *ap = aline;
      for (int j = 0; j < npr_32b; ++j) {
         if (ints <= i * npr_32b + j)
            ep += sprintf(ep, "         "), ap += sprintf(ap, "         ");
         else {
            unsigned char e32[4] = {expect[(i * npr_32b + j) * 4 + 0], expect[(i * npr_32b + j) * 4 + 1], expect[(i * npr_32b + j) * 4 + 2], expect[(i * npr_32b + j) * 4 + 3]};
            unsigned char a32[4] = {actual[(i * npr_32b + j) * 4 + 0], actual[(i * npr_32b + j) * 4 + 1], actual[(i * npr_32b + j) * 4 + 2], actual[(i * npr_32b + j) * 4 + 3]};
            ep = fmt_int32(*(unsigned char*)e32, *(unsigned char*)a32, "green", ep);
            ap = fmt_int32(*(unsigned char*)a32, *(unsigned char*)e32, "bold,red", ap);
         }
      }
      h2_printf("%s %s  %s \n", eline, SF("dark gray", "│"), aline);
   }
}

h2_inline char* h2_fail_memcmp::fmt_int64(unsigned long long c, unsigned long long t, const char* style, char* p)
{
   return p += sprintf(p, "%s", SF(c != t ? style : "", "%016X ", c));
}

h2_inline void h2_fail_memcmp::print_int64()
{
   h2_printf("%s \n", SF("dark gray", "%s │  %s", h2_string("expect").center(npr_64b * 17).c_str(), h2_string("actual").center(npr_64b * 17).c_str()));
   int int64s = expect.size() / 8, rows = ::ceil(int64s * 1.0 / npr_64b);
   for (int i = 0; i < rows; ++i) {
      char eline[1024], aline[1024], *ep = eline, *ap = aline;
      for (int j = 0; j < npr_64b; ++j) {
         if (int64s <= i * npr_64b + j)
            ep += sprintf(ep, "                 "), ap += sprintf(ap, "                 ");
         else {
            unsigned char e64[8] = {expect[(i * npr_64b + j) * 8 + 0], expect[(i * npr_64b + j) * 8 + 1], expect[(i * npr_64b + j) * 8 + 2], expect[(i * npr_64b + j) * 8 + 3], expect[(i * npr_64b + j) * 8 + 4], expect[(i * npr_64b + j) * 8 + 5], expect[(i * npr_64b + j) * 8 + 6], expect[(i * npr_64b + j) * 8 + 7]};
            unsigned char a64[8] = {actual[(i * npr_64b + j) * 8 + 0], actual[(i * npr_64b + j) * 8 + 1], actual[(i * npr_64b + j) * 8 + 2], actual[(i * npr_64b + j) * 8 + 3], actual[(i * npr_64b + j) * 8 + 4], actual[(i * npr_64b + j) * 8 + 5], actual[(i * npr_64b + j) * 8 + 6], actual[(i * npr_64b + j) * 8 + 7]};
            ep = fmt_int64(*(unsigned long long*)e64, *(unsigned long long*)a64, "green", ep);
            ap = fmt_int64(*(unsigned long long*)a64, *(unsigned long long*)e64, "bold,red", ap);
         }
      }
      h2_printf("%s %s  %s \n", eline, SF("dark gray", "│"), aline);
   }
}

h2_inline h2_fail_memoverflow::h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_, int line_)
  : h2_fail(file_, line_), ptr((unsigned char*)ptr_), offset(offset_), magic(magic_), spot(((unsigned char*)ptr_) + offset_, ((unsigned char*)ptr_) + offset_ + size), bt0(bt0_), bt1(bt1_)
{
   kprintf("Memory overflow malloc %p %+d (%p) ", ptr, offset, ptr + offset);
}

h2_inline void h2_fail_memoverflow::print()
{
   h2_fail::print();

   for (int i = 0; i < spot.size(); ++i)
      h2_printf("%s ", SF(magic[i] == spot[i] ? "green" : "bold,red", "%02X", spot[i]));

   print_locate();
   if (0 < bt1.count) h2_printf("%s%p trampled at backtrace:\n", PAD(++pad), ptr + offset), bt1.print(pad + 1);
   if (0 < bt0.count) h2_printf("%swhich allocated at backtrace:\n", PAD(++pad)), bt0.print(pad + 1);
}

h2_inline h2_fail_memleak::h2_fail_memleak(const char* file_, int line_, const char* where_)
  : h2_fail(file_, line_), where(where_), bytes(0), times(0) {}

h2_inline void h2_fail_memleak::add(void* ptr, int size, h2_backtrace& bt)
{
   bytes += size, times += 1;
   for (auto& c : places) {
      if (c.bt == bt) {
         c.ptr2 = ptr, c.size2 = size, c.bytes += size, c.times += 1;
         return;
      }
   }
   places.push_back(P(ptr, size, bt));
}

h2_inline void h2_fail_memleak::print()
{
   char t_places[64] = "", t_times[64] = "", t_bytes[64] = "";
   if (1 < places.size()) sprintf(t_places, "%s places ", SF("bold,red", "%d", (int)places.size()));
   if (1 < times) sprintf(t_times, "%s times ", SF("bold,red", "%lld", times));
   if (0 < bytes) sprintf(t_bytes, "%s bytes", SF("bold,red", "%lld", bytes));

   kprintf("Memory Leaked %s%s%s in %s totally", t_places, t_times, t_bytes, where);
   h2_fail::print(), print_locate();
   ++pad;
   for (auto& c : places) {
      c.times <= 1 ? h2_printf("%s%p Leaked %s bytes, at backtrace\n", PAD(pad), c.ptr, SF("bold,red", "%lld", c.bytes)) :
                     h2_printf("%s%p, %p ... Leaked %s times %s bytes (%s, %s ...), at backtrace\n", PAD(pad), c.ptr, c.ptr2, SF("bold,red", "%lld", c.times), SF("bold,red", "%lld", c.bytes), SF("bold,red", "%lld", c.size), SF("bold,red", "%lld", c.size2));
      c.bt.print(pad + 1);
   }
}

h2_inline h2_fail_free::h2_fail_free(void* ptr_, const char* desc, h2_backtrace& bt_alloc_, h2_backtrace& bt_free_, const char* file_, int line_)
  : h2_fail(file_, line_), bt_alloc(bt_alloc_), bt_free(bt_free_) { kprintf("%p %s", ptr_, desc); }

h2_inline void h2_fail_free::print()
{
   h2_fail::print(), h2_printf(" at backtrace:\n");
   bt_free.print(pad + 1);
   if (0 < bt_alloc.count) h2_printf("%swhich allocated at backtrace:\n", PAD(++pad)), bt_alloc.print(pad + 1);
}

h2_inline h2_fail_instantiate::h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_abstract_, const char* file_, int line_)
  : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why_abstract(why_abstract_)
{
   why_abstract ? kprintf("Instantiate 'class %s' is a abstract class", class_type) :
                  kprintf("Instantiate 'class %s' don't know initialize arguments", class_type);
}

h2_inline void h2_fail_instantiate::print()
{
   h2_fail::print(), print_locate();

   h2_printf("You may take following solutions to fix it: \n");
   if (why_abstract)
      h2_printf("1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s, %s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                SF("bold,yellow", "Derived_%s(...)", class_type));
   else {
      h2_printf("1. Define default constructor in class %s, or \n", class_type);
      h2_printf("2. Add parameterized construction in %s(%s%s%s, %s, %s, %s) \n",
                action_type,
                strlen(return_type) ? return_type : "",
                strlen(return_type) ? ", " : "",
                class_type, method_name, return_args,
                SF("bold,yellow", "%s(...)", class_type));
   }
}
