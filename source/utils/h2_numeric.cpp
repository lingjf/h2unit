static inline bool not2n(unsigned x)
{
   return x & (x - 1);
}

static inline unsigned mask2n(unsigned x)
{
   x |= (x >> 1);
   x |= (x >> 2);
   x |= (x >> 4);
   x |= (x >> 8);
   x |= (x >> 16);
   return x;
}

static inline int hex_to_byte(char c)
{
   return '0' <= c && c <= '9' ? c - '0' : ('A' <= c && c <= 'F' ? c - 'A' + 10 : ('a' <= c && c <= 'f' ? c - 'a' + 10 : -1));
}

static inline size_t bin_to_bits(const char* bin, unsigned char* bytes)
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

static inline size_t hex_to_bits(const char* hex, unsigned char* bytes)
{
   memset(bytes, 0, strlen(hex));
   if (hex[0] == '0' && ::tolower(hex[1]) == 'x') hex += 2;
   char b;
   size_t c = 0;
   for (const char* p = hex; *p; p++) {
      if (::isxdigit(*p)) {
         if (++c % 2 == 0)
            bytes[c / 2 - 1] = (unsigned char)((hex_to_byte(b) << 4) + hex_to_byte(*p));
         else
            b = *p;
      }
   }
   return c * 8 / 2;
}

static inline size_t hex_to_bytes(const char* hex, unsigned char* bytes)
{
   char b;
   size_t i = 0, c = 0;

   for (; ::isxdigit(hex[c]);) ++c;
   if (c % 2 == 1) {
      b = '0';
      i = 1;
      c += 1;
      hex = hex - 1;
   }
   for (; i < c; ++i) {
      if (i % 2 == 1)
         bytes[i / 2] = (unsigned char)((hex_to_byte(b) << 4) + hex_to_byte(hex[i]));
      b = hex[i];
   }
   return c / 2;
}

static inline bool bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits)
{
   for (size_t k = 0; k < nbits; ++k) {
      size_t i = k / 8, j = 7 - k % 8;
      if (((b1[i] >> j) & 1) != ((b2[i] >> j) & 1)) return false;
   }
   return true;
}

static inline size_t number_strlen(unsigned long long number, int base)
{
   unsigned long long _10000000 = 1;
   for (size_t i = 1;; ++i) {
      _10000000 *= base;
      if (number < _10000000) return i;
   }
}

static inline bool is_hex_string(const char* s)
{
   if (s[0] == '0' && ::tolower(s[1]) == 'x') return true;
   for (const char* p = s; *p; p++)
      if (!::isxdigit(*p) && !::isspace(*p))
         return false;
   return true;
}

h2_inline bool h2_numeric::is_bin_string(const char* s)
{
   for (const char* p = s; *p; p++)
      if (*p != '0' && *p != '1' && !::isspace(*p))
         return false;
   return true;
}

h2_inline const char* h2_numeric::sequence_number(size_t sequence, int shift)
{
   static const char* st[] = {"0th", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th"};
   static char ss[64];

   sequence += shift;
   if (sequence < sizeof(st) / sizeof(st[0])) return st[sequence];
   sprintf(ss, "%dth", (int)sequence);
   return ss;
}

static inline const char* format_duration(long long ms)
{
   static char st[128];
   if (ms < 100)
      sprintf(st, "%lld milliseconds", ms);
   else if (ms < 1000 * 60)
      sprintf(st, "%.2g second%s", ms / 1000.0, ms == 1000 ? "" : "s");
   else if (ms < 1000 * 60 * 60)
      sprintf(st, "%.2g minute%s", ms / 60000.0, ms == 60000 ? "" : "s");
   else
      sprintf(st, "%.2g hour%s", ms / 3600000.0, ms == 3600000 ? "" : "s");

   return st;
}

static inline const char* format_volume(long long footprint)
{
   static char st[128];
   if (footprint < 1024LL)
      sprintf(st, "%lld", footprint);
   else if (footprint < 1024LL * 1024LL)
      sprintf(st, "%.2gKB", footprint / 1024.0);
   else if (footprint < 1024LL * 1024LL * 1024LL)
      sprintf(st, "%.2gMB", footprint / (1024.0 * 1024.0));
   else
      sprintf(st, "%.2gGB", footprint / (1024.0 * 1024.0 * 1024.0));
   return st;
}
