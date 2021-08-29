struct h2_matches_bytecmp : h2_matches {
   const int width;
   const void* e;
   const bool isstring;
   const int nbytes;
   explicit h2_matches_bytecmp(const int width_, const void* e_, const bool isstring_, const int nbytes_) : width(width_), e(e_), isstring(isstring_), nbytes(nbytes_) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

struct h2_matches_bitcmp : h2_matches {
   const void* e;
   const bool isstring;
   const int nbits;
   explicit h2_matches_bitcmp(const void* e_, const bool isstring_, const int nbits_) : e(e_), isstring(isstring_), nbits(nbits_) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const;
   virtual h2_row expection(bool caseless, bool dont) const override;
};

template <typename E>
struct h2_matches_memcmp : h2_matches {
   const E e;
   const int length;
   explicit h2_matches_memcmp(const E e_, const int length_) : e(e_), length(length_) {}
   h2_fail* matches(const void* a, int n, bool caseless, bool dont) const
   {
      h2_fail* fail = (h2_fail*)1;

      if (std::is_convertible<E, h2_string>::value) { /* deduce */
         if (h2_numeric::is_bin_string((const char*)e)) {
            h2_matches_bitcmp t((const void*)e, true, length);
            fail = t.matches(a, n, false, false);
         }
      }

      if (fail) {
         h2_matches_bytecmp t(h2_sizeof_pointee<E>::value * 8, e, std::is_convertible<E, h2_string>::value, length * h2_sizeof_pointee<E>::value);
         fail = t.matches(a, n, false, false);
      }

      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect(expection(caseless, dont), h2_stringify(a));
      }
      return fail;
   }
   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD("Me()", caseless, dont);
   }
};

template <typename E, typename T = typename std::decay<E>::type>
inline h2_polymorphic_matcher<h2_matches_memcmp<T>> Me(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_memcmp<T>>(h2_matches_memcmp<T>((T)expect, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bitcmp> M1e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bitcmp>(h2_matches_bitcmp((const void*)expect, std::is_convertible<E, h2_string>::value, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M8e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(8, (const void*)expect, std::is_convertible<E, h2_string>::value, length));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M16e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(16, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 2));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M32e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(32, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 4));
}
template <typename E>
inline h2_polymorphic_matcher<h2_matches_bytecmp> M64e(const E expect, const int length = 0)
{
   return h2_polymorphic_matcher<h2_matches_bytecmp>(h2_matches_bytecmp(64, (const void*)expect, std::is_convertible<E, h2_string>::value, length * 8));
}
