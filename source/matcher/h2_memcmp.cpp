
h2_inline bool h2_memcmp_util::is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; p++)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

h2_inline bool h2_memcmp_util::is_bin_string(const char* s)
{
   for (const char* p = s; *p; p++)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

h2_inline size_t h2_memcmp_util::bin_to_bits(const char* bin, unsigned char* bytes)
{
   memset(bytes, 0, strlen(bin));
   size_t c = 0;
   for (const char* p = bin; *p; p++) {
      if (*p == ' ') continue;
      size_t i = c / 8, j = 7 - c % 8;
      ++c;
      unsigned char ebit = *p == '1' ? 1 : 0;
      bytes[i] = bytes[i] | (ebit << j);
   }
   return c;
}

h2_inline size_t h2_memcmp_util::hex_to_bytes(const char* hex, unsigned char* bytes) { return hex2bytes(hex, bytes); }

h2_inline bool h2_memcmp_util::bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits)
{
   for (size_t k = 0; k < nbits; ++k) {
      size_t i = k / 8, j = 7 - k % 8;
      if (((b1[i] >> j) & 1) != ((b2[i] >> j) & 1)) return false;
   }
   return true;
}