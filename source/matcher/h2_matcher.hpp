template <typename T>
struct h2_matcher_impl : h2_matches {
   virtual h2_fail* matches(const T& a, C c) const = 0;
   virtual h2_line expection(C c) const override { return ""; }
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher : h2_matches {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher();         // Any matcher
   h2_matcher(T value);  // Converting constructor
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl_, const int /*placeholder*/) : impl(impl_) {}
   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   virtual ~h2_matcher() {}
   h2_fail* matches(const T& a, C c = {}) const { return impl->matches(a, c); }
   virtual h2_line expection(C c = {}) const { return impl->expection(c); };
};

template <typename Matches>
struct h2_polymorphic_matcher : h2_matches {
   using matches_type = Matches;
   const Matches m;
   int range_start = -1, range_end = -1, times = 1;
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
   h2_polymorphic_matcher& operator*(int times_)
   {
      times = times_;
      return *this;
   }
   h2_polymorphic_matcher& operator()(int start, int end = -1)  // [Start, End)
   {
      range_start = start, range_end = end;
      if (end == -1) range_start = 0, range_end = start;
      return *this;
   }
   h2_polymorphic_matcher& operator/(int end)
   {
      range_start = 0, range_end = end;
      return *this;
   }
#define H2_MATCHES_CONFIGURE c.array_size, c.no_compare_operator, negative != c.negative, /*XOR ^*/ case_insensitive || c.case_insensitive, squash_whitespace || c.squash_whitespace, range_start != -1 && range_end != -1 ? range_start : c.range_start, range_start != -1 && range_end != -1 ? range_end : c.range_end, c.times* times

   template <typename T>
   operator h2_matcher<T>() const
   {
      return h2_matcher<T>(new internal_impl<const T&>(m, range_start, range_end, times, negative, case_insensitive, squash_whitespace), 0);
   }
   virtual h2_line expection(C c = {}) const override { return h2_matches_expection(m, {H2_MATCHES_CONFIGURE}); }

   template <typename T>
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      int range_start, range_end, times;
      bool negative, case_insensitive, squash_whitespace;
      explicit internal_impl(const Matches& m_, int range_start_, int range_end_, int times_, bool negative_, bool case_insensitive_, bool squash_whitespace_) : m(m_), range_start(range_start_), range_end(range_end_), times(times_), negative(negative_), case_insensitive(case_insensitive_), squash_whitespace(squash_whitespace_) {}
      h2_fail* matches(const T& a, C c = {}) const override { return m.matches(a, {H2_MATCHES_CONFIGURE}); }
      h2_line expection(C c) const override { return m.expection({H2_MATCHES_CONFIGURE}); }
   };
};

template <typename T, typename = void>
struct h2_is_polymorphic_matcher : std::false_type {};
template <typename T>
struct h2_is_polymorphic_matcher<T, typename std::conditional<false, h2_valid_t<typename T::matches_type>, void>::type> : std::true_type {};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
static inline h2_polymorphic_matcher<h2_matches_any> Any() { return h2_polymorphic_matcher<h2_matches_any>(h2_matches_any()); }
