template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(const T& a, size_t n, h2_mc c) const = 0;
   virtual h2_line expection(h2_mc c) const override { return ""; }
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher : h2_matches {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher();         // Any matcher
   h2_matcher(T value);  // Converting constructor
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl_, const int placeholder) : impl(impl_) {}
   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   virtual ~h2_matcher() {}
   h2_fail* matches(const T& a, size_t n = 0, h2_mc c = {}) const { return impl->matches(a, n, c); }
   virtual h2_line expection(h2_mc c = {}) const { return impl->expection(c); };
};

template <typename Matches>
struct h2_polymorphic_matcher : h2_matches {
   const Matches m;
   bool negative = false, case_insensitive = false, squash_whitespace = false;
   explicit h2_polymorphic_matcher(const Matches& m_) : m(m_) {}
   h2_polymorphic_matcher& operator!()
   {
      negative = !negative;
      return *this;
   }
   h2_polymorphic_matcher& operator~()
   {
      case_insensitive = true;
      return *this;
   }
   h2_polymorphic_matcher& operator*()
   {
      squash_whitespace = true;
      return *this;
   }
   h2_polymorphic_matcher& operator()() { return *this; }  // IsTrue/IsTrue() both works

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(m, negative, case_insensitive, squash_whitespace), 0); }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      bool negative, case_insensitive, squash_whitespace;
      explicit internal_impl(const Matches& m_, bool negative_, bool case_insensitive_, bool squash_whitespace_) : m(m_), negative(negative_), case_insensitive(case_insensitive_), squash_whitespace(squash_whitespace_) {}
      h2_fail* matches(const T& a, size_t n = 0, h2_mc c = {}) const override { return m.matches(a, n, {negative != c.negative, case_insensitive || c.case_insensitive, squash_whitespace || c.squash_whitespace, c.no_compare_operator}); }
      h2_line expection(h2_mc c) const override { return m.expection({negative != c.negative /*XOR ^*/, case_insensitive || c.case_insensitive, squash_whitespace || c.squash_whitespace, c.no_compare_operator}); }
   };

   virtual h2_line expection(h2_mc c = {}) const override { return h2_matches_expection(m, {negative != c.negative, case_insensitive || c.case_insensitive, squash_whitespace || c.squash_whitespace, c.no_compare_operator}); }
};
