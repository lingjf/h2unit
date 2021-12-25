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
   const size_t size, length, width;
   explicit h2_matches_memcmp(const E buffer_, const size_t size_, const size_t length_, const size_t width_) : buffer(buffer_), size(size_), length(length_), width(width_) {}
   h2_fail* matches(const void* a, const C& c) const
   {
      unsigned char* e = (unsigned char*)buffer;
      size_t l = length, w = width;
      bool result = true;
      do {
         if (!w && !l && std::is_convertible<E, h2_string>::value) { /* deduce by string format */
            l = strlen((const char*)buffer);
            w = 8;
            if (!strcmp((const char*)buffer, (const char*)a)) break;
            if (h2_memcmp_util::is_bin_string((const char*)buffer)) {
               e = (unsigned char*)alloca(l);
               l = h2_memcmp_util::bin_to_bits((const char*)buffer, e);
               w = 1;
            } else if (h2_memcmp_util::is_hex_string((const char*)buffer)) {
               e = (unsigned char*)alloca(l);
               l = h2_memcmp_util::hex_to_bytes((const char*)buffer, e);
               w = 8;
            }
         }
         if (!w) w = h2_sizeof_pointee<E>::value * 8; /* deduce by data type */
         if (!l) l = size;                            /* deduce by array size */
         if (!l || !w) return h2_fail::new_normal(color("length", "red") + " not specified " + gray("in ") + color("Me(buffer, ", "cyan") + color("length", "red") + gray(", width") + color(")", "cyan"));
         result = h2_memcmp_util::bits_equal(e, (const unsigned char*)a, l * w);
      } while (0);
      if (c.fit(result)) return nullptr;
      return h2_fail::new_memcmp((const unsigned char*)e, (const unsigned char*)a, l, w);
   }
   virtual h2_line expection(const C& c) const override { return c.pre() + "Me()"; }
};

template <typename T, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_memcmp<E>>>
inline P _Me(const T buffer, const size_t size, const size_t length = 0, const size_t width = 0) { return P(h2_matches_memcmp<E>((E)buffer, size, length, width)); }

#define H2Me(buffer, ...) H2PP_CAT(__H2Me, H2PP_IS_EMPTY(__VA_ARGS__))(buffer, std::extent<decltype(buffer)>::value, __VA_ARGS__)
#define __H2Me1(buffer, size, ...) h2::_Me(buffer, size)
#define __H2Me0(buffer, size, ...) h2::_Me(buffer, size, __VA_ARGS__)
