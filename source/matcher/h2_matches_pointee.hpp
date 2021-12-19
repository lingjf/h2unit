template <typename Matcher>
struct h2_pointee_matches : h2_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<typename h2_pointee_type<A>::type>(m).matches(*a, c.clear_size());
   }
   virtual h2_line expection(const C& c) const override
   {
      return h2_matches_expection(m, c);
   }
};

template <typename T, typename P = h2_polymorphic_matcher<h2_pointee_matches<T>>>
inline P Pointee(T expect) { return P(h2_pointee_matches<T>(expect)); }
