
template <typename E>
struct h2_matches_ge {
   const E e;
   explicit h2_matches_ge(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a >= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(">=" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matches_gt {
   const E e;
   explicit h2_matches_gt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a > e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(">" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matches_le {
   const E e;
   explicit h2_matches_le(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a <= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD("<=" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matches_lt {
   const E e;
   explicit h2_matches_lt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a < e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD("<" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
inline h2_polymorphic_matcher<h2_not_matches<E>> Nq(const E expect) { return h2_polymorphic_matcher<h2_not_matches<E>>(h2_not_matches<E>(expect)); }

template <typename E>
inline h2_polymorphic_matcher<h2_matches_ge<E>> Ge(const E expect) { return h2_polymorphic_matcher<h2_matches_ge<E>>(h2_matches_ge<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matches_gt<E>> Gt(const E expect) { return h2_polymorphic_matcher<h2_matches_gt<E>>(h2_matches_gt<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matches_le<E>> Le(const E expect) { return h2_polymorphic_matcher<h2_matches_le<E>>(h2_matches_le<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matches_lt<E>> Lt(const E expect) { return h2_polymorphic_matcher<h2_matches_lt<E>>(h2_matches_lt<E>(expect)); }
