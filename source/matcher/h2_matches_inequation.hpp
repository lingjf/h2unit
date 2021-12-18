template <typename E>
struct h2_matches_ge : h2_matches {
   const E e;
   explicit h2_matches_ge(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, C c) const
   {
      if (c.fit(a >= e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(C c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : "≥") + h2_stringify(e, true);
   }
};

template <typename E>
struct h2_matches_gt : h2_matches {
   const E e;
   explicit h2_matches_gt(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, C c) const
   {
      if (c.fit(a > e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(C c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : ">") + h2_stringify(e);
   }
};

template <typename E>
struct h2_matches_le : h2_matches {
   const E e;
   explicit h2_matches_le(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, C c) const
   {
      if (c.fit(a <= e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(C c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : "≤") + h2_stringify(e);
   }
};

template <typename E>
struct h2_matches_lt : h2_matches {
   const E e;
   explicit h2_matches_lt(const E& e_) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, C c) const
   {
      if (c.fit(a < e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a));
   }
   virtual h2_line expection(C c) const override
   {
      return c.update_caseless(false).pre() + (c.no_compare_operator ? "" : "<") + h2_stringify(e);
   }
};

template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_not_matches<E>>>
inline P Nq(const T& expect) { return P(h2_not_matches<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_ge<E>>>
inline P Ge(const T& expect) { return P(h2_matches_ge<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_gt<E>>>
inline P Gt(const T& expect) { return P(h2_matches_gt<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_le<E>>>
inline P Le(const T& expect) { return P(h2_matches_le<E>(expect)); }
template <typename T, typename E = typename h2_decay<T>::type, typename P = h2_polymorphic_matcher<h2_matches_lt<E>>>
inline P Lt(const T& expect) { return P(h2_matches_lt<E>(expect)); }
