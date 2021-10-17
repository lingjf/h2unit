struct h2_numeric {
   static bool is_hex_string(const char* s);
   static bool is_bin_string(const char* s);
   static size_t hex_to_bytes(const char* hex, unsigned char* bytes);
   static size_t bin_to_bits(const char* bin, unsigned char* bytes);

   static bool bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits);

   static const char* sequence_number(size_t sequence, size_t shift = 1);
};
