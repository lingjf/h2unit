static inline h2_string readable_size(size_t width, size_t nbits)
{
   char t[64];
   switch (width) {
   case 1: sprintf(t, "%d bit%s", (int)nbits, nbits > 1 ? "s" : ""); break;
   case 8: sprintf(t, "%d byte%s", (int)(nbits / 8), nbits / 8 > 1 ? "s" : ""); break;
   case 16: sprintf(t, "%d word%s", (int)(nbits / 16), nbits / 16 > 1 ? "s" : ""); break;
   case 32: sprintf(t, "%d dword%s", (int)(nbits / 32), nbits / 32 > 1 ? "s" : ""); break;
   case 64: sprintf(t, "%d qword%s", (int)(nbits / 64), nbits / 64 > 1 ? "s" : ""); break;
   default: sprintf(t, "?"); break;
   }
   return h2_string(t);
}

h2_inline h2_fail* h2_matches_bytecmp::matches(const void* a, int n, bool caseless, bool dont, bool ncop) const
{
   bool result = false;
   size_t _nbytes;
   if (isstring) {
      if (is_hex_string((const char*)e)) {
         unsigned char* _e = (unsigned char*)alloca(strlen((const char*)e));
         size_t max_length = hex_to_bytes((const char*)e, _e);
         _nbytes = nbytes;
         if (nbytes == 0) _nbytes = max_length;
         if (_nbytes <= max_length) {
            result = memcmp(_e, a, _nbytes) == 0;
         }
      } else {
         _nbytes = strlen((const char*)e);
      }
   } else {
      if (nbytes == 0) {
         return h2_fail::new_normal("length required");
      }
   }
   if (!result) {
      _nbytes = nbytes;
      result = memcmp(e, a, _nbytes) == 0;
   }
   if (result == !dont) return nullptr;
   return h2_fail::new_memcmp((const unsigned char*)e, (const unsigned char*)a, width, _nbytes * 8, h2_stringify(a).string(), "memcmp " + readable_size(width, _nbytes * 8));
}

h2_inline h2_sentence h2_matches_bytecmp::expection(bool caseless, bool dont, bool ncop) const
{
   return CD("Me()", caseless, dont, ncop);
}

h2_inline h2_fail* h2_matches_bitcmp::matches(const void* a, int n, bool caseless, bool dont, bool ncop) const
{
   size_t max_length = INT_MAX;
   unsigned char* _e = (unsigned char*)e;
   if (isstring) {
      unsigned char* t = (unsigned char*)alloca(strlen((const char*)e));
      if (h2_numeric::is_bin_string((const char*)e)) {
         max_length = bin_to_bits((const char*)e, t);
         _e = t;
      } else if (is_hex_string((const char*)e)) {
         max_length = hex_to_bits((const char*)e, t);
         _e = t;
      } else {
         max_length = strlen((const char*)e) * 8;
      }
   } else {
      if (nbits == 0) {
         return h2_fail::new_normal("length required");
      }
   }
   size_t _nbits = nbits;
   if (nbits == 0) _nbits = max_length;
   if (max_length < _nbits) {
      return h2_fail::new_normal("length too loog");
   }
   bool result = bits_equal(_e, (const unsigned char*)a, _nbits);
   if (result == !dont) return nullptr;
   return h2_fail::new_memcmp(_e, (const unsigned char*)a, 1, _nbits, h2_stringify(a).string(), "memcmp " + readable_size(1, _nbits));
}

h2_inline h2_sentence h2_matches_bitcmp::expection(bool caseless, bool dont, bool ncop) const
{
   return CD("Me()", caseless, dont, ncop);
}
