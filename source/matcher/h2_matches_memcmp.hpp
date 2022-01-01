struct h2_memcmp_util {
   static bool bits_equal(const unsigned char* b1, const unsigned char* b2, size_t nbits);
   static bool is_hex_string(const char* s);
   static bool is_bin_string(const char* s);
   static size_t bin_to_bits(const char* bin, unsigned char* bytes);
   static size_t hex_to_bytes(const char* hex, unsigned char* bytes);
};

template <typename E>
struct h2_matches_memcmp : h2_matches {
   const E buffer;
   const size_t dimension, size, width;
   explicit h2_matches_memcmp(const E buffer_, const size_t dimension_, const size_t size_, const size_t width_) : buffer(buffer_), dimension(dimension_), size(size_), width(width_) {}
   h2_fail* matches(const void* a, const C& c) const
   {
      unsigned char* e = (unsigned char*)buffer;
      const char* b = (const char*)e;
      size_t z = size, w = width;
      bool result = true;
      do {
         if (!w && !z && std::is_convertible<E, h2_string>::value) { /* deduce by string format */
            z = strlen(b);
            w = 8;
            if (!strcmp(b, (const char*)a)) break;
            if (h2_memcmp_util::is_bin_string(b)) {
               e = (unsigned char*)alloca(z);
               z = h2_memcmp_util::bin_to_bits(b, e);
               w = 1;
            } else if (h2_memcmp_util::is_hex_string(b)) {
               e = (unsigned char*)alloca(z);
               z = h2_memcmp_util::hex_to_bytes(b, e);
               w = 8;
            }
         }
         if (!w) w = h2_sizeof_pointee<E>::value * 8; /* deduce by data type */
         if (!z) z = dimension;                       /* deduce by array dimension */
         if (!z || !w) return h2_fail::new_normal(color("size", "red") + " not specified " + gray("in ") + color("Me(buffer, ", "cyan") + color("size", "red") + gray(", width") + color(")", "cyan"));
         result = h2_memcmp_util::bits_equal(e, (const unsigned char*)a, z * w);
      } while (0);
      if (c.fit(result)) return nullptr;
      return h2_fail::new_memcmp(e, (const unsigned char*)a, z, w);
   }
   virtual h2_line expection(const C& c) const override { return c.pre() + "Me()"; }
};

template <typename T, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_memcmp<E>>>
inline P Memcmp(const T buffer, const size_t dimension, const size_t size = 0, const size_t width = 0) { return P(h2_matches_memcmp<E>((E)buffer, dimension, size, width)); }
inline auto Memcmp(const h2_string& buffer, const size_t dimension) -> h2_polymorphic_matcher<h2_matches_memcmp<h2_string>> { return h2_polymorphic_matcher<h2_matches_memcmp<h2_string>>(h2_matches_memcmp<h2_string>(buffer, dimension, buffer.size(), 8)); }

#define H2Me(buffer, ...) H2PP_CAT(H2Me_, H2PP_IS_EMPTY(__VA_ARGS__))(buffer, std::extent<decltype(buffer)>::value, __VA_ARGS__)
#define H2Me_1(buffer, dimension, ...) h2::Memcmp(buffer, dimension)
#define H2Me_0(buffer, dimension, ...) h2::Memcmp(buffer, dimension, __VA_ARGS__)
