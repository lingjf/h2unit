
template <typename T = void>
struct h2_type {
   using type = T;
};

template <typename T>
struct h2_matcher_impl {
   virtual h2_fail* matches(T a, bool caseless = false, bool dont = false) const = 0;
   virtual h2_string expects(bool caseless = false, bool dont = false) const { return ""; }
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher() {}
   h2_matcher(T value);  // Converting constructor
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl_, const int placeholder) : impl(impl_) {}
   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   virtual ~h2_matcher() {}
   h2_fail* matches(const T& a, bool caseless = false, bool dont = false) const { return impl->matches(a, caseless, dont); }
   h2_string expects(bool caseless = false, bool dont = false) const { return impl->expects(caseless, dont); };
};

template <typename Matches>
struct h2_polymorphic_matcher {
   const Matches m;
   explicit h2_polymorphic_matcher(const Matches& _matches) : m(_matches) {}

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(m), 0); }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      explicit internal_impl(const Matches& _matches) : m(_matches) {}
      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override { return m.matches(a, caseless, dont); }
      h2_string expects(bool caseless = false, bool dont = false) const override { return m.expects(h2_type<T>(), caseless, dont); }
   };
};

static inline h2_string CD(const h2_string& s, bool caseless = false, bool dont = false)
{
   h2_string z = s;
   if (dont) z = "!" + z;
   if (caseless) z = "~" + z;
   return z;
}
