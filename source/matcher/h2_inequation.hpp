
template <typename E>
struct h2_matches_ge : h2_matches {
   const E e;
   explicit h2_matches_ge(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a >= e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD(">=" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_gt : h2_matches {
   const E e;
   explicit h2_matches_gt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a > e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD(">" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_le : h2_matches {
   const E e;
   explicit h2_matches_le(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a <= e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD("<=" + h2_stringify(e), false, dont);
   }
};

template <typename E>
struct h2_matches_lt : h2_matches {
   const E e;
   explicit h2_matches_lt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a < e) == !dont) return nullptr;
      return h2_fail::new_unexpect(h2_stringify(e), h2_stringify(a), expects(caseless, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return CD("<" + h2_stringify(e), false, dont);
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_not_matches<E>> Nq(const T& expect) { return h2_polymorphic_matcher<h2_not_matches<E>>(h2_not_matches<E>(expect)); }

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_ge<E>> Ge(const T& expect) { return h2_polymorphic_matcher<h2_matches_ge<E>>(h2_matches_ge<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_gt<E>> Gt(const T& expect) { return h2_polymorphic_matcher<h2_matches_gt<E>>(h2_matches_gt<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_le<E>> Le(const T& expect) { return h2_polymorphic_matcher<h2_matches_le<E>>(h2_matches_le<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_matches_lt<E>> Lt(const T& expect) { return h2_polymorphic_matcher<h2_matches_lt<E>>(h2_matches_lt<E>(expect)); }
