
h2_inline h2_fail* h2_string_equal_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (r) dont = !dont;
   if (a.equals(e, caseless) == !dont) return nullptr;
   if (h2_wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   if (h2_regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;

   return new h2_fail_strcmp(e, a, dont, caseless);
}

h2_inline h2_fail* h2_float_equal_matches::matches(const long double a, bool caseless, bool dont) const
{
   if (r) dont = !dont;
   // the machine epsilon has to be scaled to the magnitude of the values used
   // and multiplied by the desired precision in ULPs (units in the last place)
   // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
   //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
   bool result = std::fabs(a - e) < 0.00001;
   if (result == !dont) return nullptr;
   return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), dont, false, "");
}

static inline bool __is_bin_string(const char* s)
{
   for (const char* p = s; *p; p++)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

static inline bool __is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; p++)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

static inline int __hex_to_byte(char c)
{
   return '0' <= c && c <= '9' ? c - '0' : ('A' <= c && c <= 'F' ? c - 'A' + 10 : ('a' <= c && c <= 'f' ? c - 'a' + 10 : -1));
}

static inline int __hex_to_bytes(const char* hex, unsigned char* bytes)
{
   memset(bytes, 0, strlen(hex));
   if (hex[0] == '0' && ::tolower(hex[1]) == 'x') hex += 2;
   char b;
   int c = 0;
   for (const char* p = hex; *p; p++) {
      if (::isxdigit(*p)) {
         if (++c % 2 == 0)
            bytes[c / 2 - 1] = (unsigned char)((__hex_to_byte(b) << 4) + __hex_to_byte(*p));
         else
            b = *p;
      }
   }
   return 8 * c / 2;
}

static inline int __bin_to_bytes(const char* bin, unsigned char* bytes)
{
   memset(bytes, 0, strlen(bin));
   int c = 0;
   for (const char* p = bin; *p; p++) {
      if (*p == ' ') continue;
      int i = c / 8, j = 7 - c % 8;
      ++c;
      unsigned char ebit = *p == '1' ? 1 : 0;
      bytes[i] = bytes[i] | (ebit << j);
   }
   return c;
}

static inline bool __bits_equal(const unsigned char* e, const unsigned char* a, int nbits)
{
   for (int k = 0; k < nbits; ++k) {
      int i = k / 8, j = 7 - k % 8;
      if (((e[i] >> j) & 1) != ((a[i] >> j) & 1)) return false;
   }
   return true;
}

static inline char* __print_size(int width, int nbits)
{
   static char t[64];
   switch (width) {
   case 1: sprintf(t, "%d bit%s", nbits, nbits > 1 ? "s" : ""); break;
   case 8: sprintf(t, "%d byte%s", nbits / 8, nbits / 8 > 1 ? "s" : ""); break;
   case 16: sprintf(t, "%d word%s", nbits / 16, nbits / 16 > 1 ? "s" : ""); break;
   case 32: sprintf(t, "%d dword%s", nbits / 32, nbits / 32 > 1 ? "s" : ""); break;
   case 64: sprintf(t, "%d qword%s", nbits / 64, nbits / 64 > 1 ? "s" : ""); break;
   default: sprintf(t, "?"); break;
   }
   return t;
}

h2_inline h2_fail* h2_memcmp_matches::matches(const void* a, bool caseless, bool dont) const
{
   bool result = false;

   const unsigned char* _e[3];
   int _nbits[3];
   int _width[3];
   int i, gc = 0;

   if (width == 0) /* guess width */ {
      // Case1: binary string descript memory
      if (__is_bin_string((const char*)e)) {
         _e[gc] = (const unsigned char*)alloca(strlen((const char*)e));
         _nbits[gc] = __bin_to_bytes((const char*)e, (unsigned char*)_e[gc]);
         _width[gc] = 1;
         ++gc;
      }
      // Case2: hex string descript memory
      if (__is_hex_string((const char*)e)) {
         _e[gc] = (const unsigned char*)alloca(strlen((const char*)e));
         _nbits[gc] = __hex_to_bytes((const char*)e, (unsigned char*)_e[gc]);
         _width[gc] = 8;
         ++gc;
      }
      // Case3: string filled memory
      _e[gc] = (const unsigned char*)e;
      _nbits[gc] = strlen((const char*)e) * 8;
      _width[gc] = 8;
      ++gc;
   } else {
      _e[gc] = (const unsigned char*)e;
      _nbits[gc] = nbits;
      _width[gc] = width;
      ++gc;
   }

   for (i = 0; i < gc; ++i) {
      if (_width[i] == 1)
         result = __bits_equal((const unsigned char*)_e[i], (const unsigned char*)a, _nbits[i]);
      else
         result = memcmp(_e[i], a, _nbits[i] / 8) == 0;

      if (result) break;
   }

   if (result == !dont) return nullptr;
   int j = result ? i : 0;
   return new h2_fail_memcmp((const unsigned char*)_e[j], (const unsigned char*)a, _width[j], _nbits[j], dont, __print_size(_width[j], _nbits[j]), h2_stringify(a));
}

h2_inline h2_fail* h2_regex_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, dont, caseless, "Re");
}

h2_inline h2_fail* h2_wildcard_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, dont, caseless, "We");
}

h2_inline h2_fail* h2_strcmp_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.equals(e, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, dont, caseless, "");
}

h2_inline h2_fail* h2_contains_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.contains(substring, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(substring, a, dont, caseless, "Contains");
}

h2_inline h2_fail* h2_startswith_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(prefix_string, a, dont, caseless, "StartsWith");
}

h2_inline h2_fail* h2_endswith_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(suffix_string, a, dont, caseless, "EndsWith");
}

h2_inline h2_fail* h2_json_matches::matches(const h2_string& a, bool caseless, bool dont) const
{
   if ((h2_json::match(e, a)) == !dont) return nullptr;
   return new h2_fail_json(e, a, dont, caseless);
}
