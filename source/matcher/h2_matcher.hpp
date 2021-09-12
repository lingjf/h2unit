template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(T a, int n, bool caseless, bool dont) const = 0;
   virtual h2_sentence expection(bool caseless, bool dont) const override { return ""; }
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
   h2_fail* matches(const T& a, int n = 0, bool caseless = false, bool dont = false) const { return impl->matches(a, n, caseless, dont); }
   virtual h2_sentence expection(bool caseless = false, bool dont = false) const { return impl->expection(caseless, dont); };
};

template <typename Matches>
struct h2_polymorphic_matcher : h2_matches {
   const Matches m;
   bool caseless = false, dont = false;
   explicit h2_polymorphic_matcher(const Matches& m_) : m(m_) {}
   h2_polymorphic_matcher& operator*()
   {
      caseless = true;
      return *this;
   }
   h2_polymorphic_matcher& operator~()
   {
      caseless = true;
      return *this;
   }
   h2_polymorphic_matcher& operator!()
   {
      dont = !dont;
      return *this;
   }
   h2_polymorphic_matcher& operator()() { return *this; }  // IsTrue/IsTrue() both works

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(m, caseless, dont), 0); }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      bool caseless, dont;
      explicit internal_impl(const Matches& m_, bool caseless_, bool dont_) : m(m_), caseless(caseless_), dont(dont_) {}
      h2_fail* matches(T a, int n = 0, bool caseless_ = false, bool dont_ = false) const override { return m.matches(a, n, caseless || caseless_, dont != dont_); }
      h2_sentence expection(bool caseless_, bool dont_) const override { return m.expection(caseless || caseless_, dont != dont_ /*XOR ^*/); }
   };

   virtual h2_sentence expection(bool caseless_ = false, bool dont_ = false) const override { return h2_matches_expection(m, caseless || caseless_, dont != dont_); }
};
