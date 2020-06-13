
h2_inline h2_fail* h2_matchee_regex::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, expects(a, caseless, dont));
}

h2_inline h2_string h2_matchee_regex::expects(const h2_string& a, bool caseless, bool dont) const
{
   return CD("Re(" + h2_stringify(e) + ")", caseless, dont);
}

h2_inline h2_fail* h2_matchee_wildcard::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, expects(a, caseless, dont));
}

h2_inline h2_string h2_matchee_wildcard::expects(const h2_string& a, bool caseless, bool dont) const
{
   return CD("We(" + h2_stringify(e) + ")", caseless, dont);
}

h2_inline h2_fail* h2_matchee_strcmp::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.equals(e, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(e, a, expects(a, caseless, dont));
}

h2_inline h2_string h2_matchee_strcmp::expects(const h2_string& a, bool caseless, bool dont) const
{
   return CD("\"" + h2_stringify(e) + "\"", caseless, dont);
}

h2_inline h2_fail* h2_matchee_substr::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.contains(substring, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(substring, a, expects(a, caseless, dont));
}

h2_inline h2_string h2_matchee_substr::expects(const h2_string& a, bool caseless, bool dont) const
{
   return CD("Substr(\"" + h2_stringify(substring) + "\")", caseless, dont);
}

h2_inline h2_fail* h2_matchee_startswith::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(prefix_string, a, expects(a, caseless, dont));
}

h2_inline h2_string h2_matchee_startswith::expects(const h2_string& a, bool caseless, bool dont) const
{
   return CD("StartsWith(\"" + h2_stringify(prefix_string) + "\")", caseless, dont);
}

h2_inline h2_fail* h2_matchee_endswith::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   return new h2_fail_strfind(suffix_string, a, expects(a, caseless, dont));
}

h2_inline h2_string h2_matchee_endswith::expects(const h2_string& a, bool caseless, bool dont) const
{
   return CD("EndsWith(\"" + h2_stringify(suffix_string) + "\")", caseless, dont);
}

h2_inline h2_fail* h2_matchee_json::matches(const h2_string& a, bool caseless, bool dont) const
{
   if ((h2_json::match(e, a)) == !dont) return nullptr;
   return new h2_fail_json(e, a, expects(a, caseless, dont));
}

h2_inline h2_string h2_matchee_json::expects(const h2_string& a, bool caseless, bool dont) const
{
   return CD("Je(\"" + h2_stringify(e) + "\")", caseless, dont);
}

static inline h2_string readable_size(int width, int nbits)
{
   char t[64];
   switch (width) {
   case 1: sprintf(t, "%d bit%s", nbits, nbits > 1 ? "s" : ""); break;
   case 8: sprintf(t, "%d byte%s", nbits / 8, nbits / 8 > 1 ? "s" : ""); break;
   case 16: sprintf(t, "%d word%s", nbits / 16, nbits / 16 > 1 ? "s" : ""); break;
   case 32: sprintf(t, "%d dword%s", nbits / 32, nbits / 32 > 1 ? "s" : ""); break;
   case 64: sprintf(t, "%d qword%s", nbits / 64, nbits / 64 > 1 ? "s" : ""); break;
   default: sprintf(t, "?"); break;
   }
   return h2_string(t);
}

h2_inline h2_fail* h2_matchee_memcmp::matches(const void* a, bool caseless, bool dont) const
{
   bool result = false;

   const unsigned char* _e[3];
   int _nbits[3];
   int _width[3];
   int i, gc = 0;

   if (width == 0) /* guess width */ {
      // Case1: binary string descript memory
      if (h2_numeric::is_bin_string((const char*)e)) {
         _e[gc] = (const unsigned char*)alloca(strlen((const char*)e));
         _nbits[gc] = h2_numeric::bin_to_bits((const char*)e, (unsigned char*)_e[gc]);
         _width[gc] = 1;
         ++gc;
      }
      // Case2: hex string descript memory
      if (h2_numeric::is_hex_string((const char*)e)) {
         _e[gc] = (const unsigned char*)alloca(strlen((const char*)e));
         _nbits[gc] = h2_numeric::hex_to_bits((const char*)e, (unsigned char*)_e[gc]);
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
         result = h2_numeric::bits_equal((const unsigned char*)_e[i], (const unsigned char*)a, _nbits[i]);
      else
         result = memcmp(_e[i], a, _nbits[i] / 8) == 0;

      if (result) break;
   }

   if (result == !dont) return nullptr;
   int j = result ? i : 0;
   return new h2_fail_memcmp((const unsigned char*)_e[j], (const unsigned char*)a, _width[j], _nbits[j], "", h2_stringify(a), "memcmp " + readable_size(_width[j], _nbits[j]));
}

h2_inline h2_string h2_matchee_memcmp::expects(const void* a, bool caseless, bool dont) const
{
   return CD("Me()", caseless, dont);
}
