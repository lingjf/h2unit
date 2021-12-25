template <typename Matcher>
struct h2_not_matches : h2_matches {
   const Matcher m; /* h2_polymorphic_matcher or immediate value or h2_matches */
   explicit h2_not_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<A>(m).matches(a, c.update_negative(!c.negative));
   }
   virtual h2_line expection(const C& c) const override
   {
      return h2_matches_expection(m, c.update_negative(!c.negative));
   }
};

template <typename MatcherT, typename MatcherF>
struct h2_conditional_matches : h2_matches {
   const bool cond;
   const MatcherT mt;
   const MatcherF mf;
   explicit h2_conditional_matches(const bool cond_, const MatcherT& mt_, const MatcherF& mf_) : cond(cond_), mt(mt_), mf(mf_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return cond ? h2_matcher_cast<A>(mt).matches(a, c) : h2_matcher_cast<A>(mf).matches(a, c);
   }
   virtual h2_line expection(const C& c) const override
   {
      return cond ? h2_matches_expection(mt, c) : h2_matches_expection(mf, c);
   }
};

template <typename MatcherL, typename MatcherR>
struct h2_and_matches : h2_matches {
   const MatcherL ml;
   const MatcherR mr;
   explicit h2_and_matches(const MatcherL& ml_, const MatcherR& mr_) : ml(ml_), mr(mr_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<A>(ml).matches(a, c.update_negative(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<A>(mr).matches(a, c.update_negative(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      auto sl = h2_matches_expection(ml, c.update_negative(false));
      if (sl.has(" or ")) sl = gray("(") + sl + gray(")");
      auto sr = h2_matches_expection(mr, c.update_negative(false));
      if (sr.has(" and ") || sr.has(" or ")) sr = gray("(") + sr + gray(")");
      h2_line s = sl + " and " + sr;
      if (c.negative) s = gray("(") + s + gray(")");
      return c.update_caseless(false).pre() + s;
   }
};

template <typename MatcherL, typename MatcherR>
struct h2_or_matches : h2_matches {
   const MatcherL ml;
   const MatcherR mr;
   explicit h2_or_matches(const MatcherL& ml_, const MatcherR& mr_) : ml(ml_), mr(mr_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(ml).matches(a, c.update_negative(false));
      h2_fail* f2 = h2_matcher_cast<A>(mr).matches(a, c.update_negative(false));
      if (c.fit(!f1 || !f2)) {
         if (f1) delete f1;
         if (f2) delete f2;
         return nullptr;
      }
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, f1);
      h2_fail::append_subling(fails, f2);

      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      auto sl = h2_matches_expection(ml, c.update_negative(false));
      auto sr = h2_matches_expection(mr, c.update_negative(false));
      if (sr.has(" or ")) sr = gray("(") + sr + gray(")");
      h2_line s = sl + " or " + sr;
      if (c.negative) s = gray("(") + s + gray(")");
      return c.update_caseless(false).pre() + s;
   }
};

template <typename... Matchers>
struct h2_allof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_allof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_vector<h2_matcher<A>> matchers;
      e.matchers(matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = matchers[i].matches(a, c.update_negative(false));  // dont not transfer down
         if (fail) fail->seqno = (int)i;
         h2_fail::append_subling(fails, fail);
      }

      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + "AllOf" + gray("(") + e.expection(c.update_negative(false)) + gray(")");
   }
};

template <typename... Matchers>
struct h2_anyof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_anyof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_vector<h2_matcher<A>> matchers;
      e.matchers(matchers);

      int count = 0;
      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = matchers[i].matches(a, c.update_negative(false));
         if (!fail) {
            count++;
            break;
         }
         if (fail) fail->seqno = (int)i;
         h2_fail::append_subling(fails, fail);
      }

      if (c.fit(0 < count)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + "AnyOf" + gray("(") + e.expection(c.update_negative(false)) + gray(")");
   }
};

template <typename... Matchers>
struct h2_noneof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_noneof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      h2_vector<h2_matcher<A>> matchers;
      e.matchers(matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = matchers[i].matches(a, c.update_negative(false));
         if (fail) delete fail;
         else {
            fail = h2_fail::new_normal("should not match " + matchers[i].expection(c).brush("green"));
            fail->seqno = (int)i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(const C& c) const override
   {
      return c.update_caseless(false).pre() + "NoneOf" + gray("(") + e.expection(c.update_negative(false)) + gray(")");
   }
};

template <typename T, typename P = h2_polymorphic_matcher<h2_not_matches<T>>>
inline P Not(T expect) { return P(h2_not_matches<T>(expect)); }

template <typename T, typename F, typename P = h2_polymorphic_matcher<h2_conditional_matches<T, F>>>
inline P Conditional(bool cond, T true_expect, F false_expect) { return P(h2_conditional_matches<T, F>(cond, true_expect, false_expect)); }

template <typename... T, typename M = h2_allof_matches<typename std::decay<const T&>::type...>, typename P = h2_polymorphic_matcher<M>>
inline P AllOf(const T&... expects) { return P(M(expects...)); }
template <typename... T, typename M = h2_anyof_matches<typename std::decay<const T&>::type...>, typename P = h2_polymorphic_matcher<M>>
inline P AnyOf(const T&... expects) { return P(M(expects...)); }
template <typename... T, typename M = h2_noneof_matches<typename std::decay<const T&>::type...>, typename P = h2_polymorphic_matcher<M>>
inline P NoneOf(const T&... expects) { return P(M(expects...)); }

template <typename T1, typename T2, typename M = h2_and_matches<h2_polymorphic_matcher<T1>, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator&&(const h2_polymorphic_matcher<T1>& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(expect1, expect2)); }
template <typename T1, typename T2, typename E2 = h2_matcher<typename h2_decay<T2>::type>, typename M = h2_and_matches<h2_polymorphic_matcher<T1>, E2>, typename P = h2_polymorphic_matcher<M>>
inline P operator&&(const h2_polymorphic_matcher<T1>& expect1, const T2& expect2) { return P(M(expect1, E2(expect2))); }
template <typename T1, typename T2, typename E1 = h2_matcher<typename h2_decay<T1>::type>, typename M = h2_and_matches<E1, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator&&(const T1& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(E1(expect1), expect2)); }

template <typename T1, typename T2, typename M = h2_or_matches<h2_polymorphic_matcher<T1>, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator||(const h2_polymorphic_matcher<T1>& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(expect1, expect2)); }
template <typename T1, typename T2, typename E2 = h2_matcher<typename h2_decay<T2>::type>, typename M = h2_or_matches<h2_polymorphic_matcher<T1>, E2>, typename P = h2_polymorphic_matcher<M>>
inline P operator||(const h2_polymorphic_matcher<T1>& expect1, const T2& expect2) { return P(M(expect1, E2(expect2))); }
template <typename T1, typename T2, typename E1 = h2_matcher<typename h2_decay<T1>::type>, typename M = h2_or_matches<E1, h2_polymorphic_matcher<T2>>, typename P = h2_polymorphic_matcher<M>>
inline P operator||(const T1& expect1, const h2_polymorphic_matcher<T2>& expect2) { return P(M(E1(expect1), expect2)); }
