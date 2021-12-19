template <typename Cast, typename Matcher>
struct h2_castof_matches : h2_matches {
   const Matcher m;
   explicit h2_castof_matches(Matcher m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<Cast>(m).matches((Cast)a, c);
   }
   virtual h2_line expection(const C& c) const override
   {
      return h2_matches_expection(m, c);
   }
};

template <typename D, typename T, typename P = h2_polymorphic_matcher<h2_castof_matches<D, T>>>
inline P CastOf(T expect) { return P(h2_castof_matches<D, T>(expect)); }
