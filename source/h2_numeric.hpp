
struct h2_numeric {
   static int hex_to_byte(char c);
   static bool is_bin_string(const char* s);
   static bool is_hex_string(const char* s);
   static int extract_hex_string(const char* s, bool ignore_space = false);

   static int bin_to_bits(const char* bin, unsigned char* bytes);

   static int hex_to_bits(const char* hex, unsigned char* bytes);
   static int hex_to_bytes(const char* hex, unsigned char* bytes);

   static bool bits_equal(const unsigned char* b1, const unsigned char* b2, int nbits);

   static const char* sequence_number(int sequence, int shift = 1);

   static long long parse_int_after_equal(const char* s);
};
