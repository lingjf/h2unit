template <typename Matcher>
struct h2_not_matches : h2_matches {
   const Matcher m; /* h2_polymorphic_matcher or immediate value or h2_matches */
   explicit h2_not_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      return h2_matcher_cast<A>(m).matches(a, c.update_negative(!c.negative));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return h2_matches_expection(m, c.update_negative(!c.negative));
   }
};

template <typename MatcherL, typename MatcherR>
struct h2_and_matches : h2_matches {
   const MatcherL ml;
   const MatcherR mr;
   explicit h2_and_matches(const MatcherL& ml_, const MatcherR& mr_) : ml(ml_), mr(mr_) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
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

   virtual h2_line expection(h2_mc c) const override
   {
      auto sl = h2_matches_expection(ml, c.update_negative(false));
      if (sl.has(" or ")) sl = gray("(") + sl + gray(")");
      auto sr = h2_matches_expection(mr, c.update_negative(false));
      if (sr.has(" and ") || sr.has(" or ")) sr = gray("(") + sr + gray(")");
      h2_line s = sl + " and " + sr;
      if (c.negative) s = gray("(") + s + gray(")");
      return ncsc(s, c.update_caseless(false));
   }
};

template <typename MatcherL, typename MatcherR>
struct h2_or_matches : h2_matches {
   const MatcherL ml;
   const MatcherR mr;
   explicit h2_or_matches(const MatcherL& ml_, const MatcherR& mr_) : ml(ml_), mr(mr_) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
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

   virtual h2_line expection(h2_mc c) const override
   {
      auto sl = h2_matches_expection(ml, c.update_negative(false));
      auto sr = h2_matches_expection(mr, c.update_negative(false));
      if (sr.has(" or ")) sr = gray("(") + sr + gray(")");
      h2_line s = sl + " or " + sr;
      if (c.negative) s = gray("(") + s + gray(")");
      return ncsc(s, c.update_caseless(false));
   }
};

template <typename... Matchers>
struct h2_allof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, c.update_negative(false));  // dont not transfer down
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

   virtual h2_line expection(h2_mc c) const override
   {
      return ncsc("AllOf" + gray("(") + t2e(c.update_negative(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int count = 0;
      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, c.update_negative(false));
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

   virtual h2_line expection(h2_mc c) const override
   {
      return ncsc("AnyOf" + gray("(") + t2e(c.update_negative(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, c.update_negative(false));
         if (fail)
            delete fail;
         else {
            fail = h2_fail::new_normal("should not match " + v_matchers[i].expection(c).brush("green"));
            fail->seqno = (int)i;
            h2_fail::append_subling(fails, fail);
         }
      }
      if (c.fit(!fails)) {
         delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return ncsc("NoneOf" + gray("(") + t2e(c.update_negative(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher m)
{
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(m));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> AllOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>>(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> AnyOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>>(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> NoneOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>>(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename MatcherL, typename MatcherR>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<MatcherL>, h2_polymorphic_matcher<MatcherR>>> operator&&(const h2_polymorphic_matcher<MatcherL>& ml, const h2_polymorphic_matcher<MatcherR>& mr)
{
   h2_and_matches<h2_polymorphic_matcher<MatcherL>, h2_polymorphic_matcher<MatcherR>> a(ml, mr);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<MatcherL>, h2_polymorphic_matcher<MatcherR>>> b(a);
   return b;
}
template <typename MatcherL, typename MatcherR>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<MatcherL>, h2_matcher<typename h2_decay<MatcherR>::type>>> operator&&(const h2_polymorphic_matcher<MatcherL>& ml, const MatcherR& mr)
{
   h2_matcher<typename h2_decay<MatcherR>::type> a(mr);
   h2_and_matches<h2_polymorphic_matcher<MatcherL>, h2_matcher<typename h2_decay<MatcherR>::type>> b(ml, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<MatcherL>, h2_matcher<typename h2_decay<MatcherR>::type>>> c(b);
   return c;
}
template <typename MatcherL, typename MatcherR>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<MatcherL>::type>, h2_polymorphic_matcher<MatcherR>>> operator&&(const MatcherL& ml, const h2_polymorphic_matcher<MatcherR>& mr)
{
   h2_matcher<typename h2_decay<MatcherL>::type> a(ml);
   h2_and_matches<h2_matcher<typename h2_decay<MatcherL>::type>, h2_polymorphic_matcher<MatcherR>> b(a, mr);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<MatcherL>::type>, h2_polymorphic_matcher<MatcherR>>> c(b);
   return c;
}

template <typename MatcherL, typename MatcherR>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<MatcherL>, h2_polymorphic_matcher<MatcherR>>> operator||(const h2_polymorphic_matcher<MatcherL>& ml, const h2_polymorphic_matcher<MatcherR>& mr)
{
   h2_or_matches<h2_polymorphic_matcher<MatcherL>, h2_polymorphic_matcher<MatcherR>> a(ml, mr);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<MatcherL>, h2_polymorphic_matcher<MatcherR>>> b(a);
   return b;
}
template <typename MatcherL, typename MatcherR>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<MatcherL>, h2_matcher<typename h2_decay<MatcherR>::type>>> operator||(const h2_polymorphic_matcher<MatcherL>& ml, const MatcherR& mr)
{
   h2_matcher<typename h2_decay<MatcherR>::type> a(mr);
   h2_or_matches<h2_polymorphic_matcher<MatcherL>, h2_matcher<typename h2_decay<MatcherR>::type>> b(ml, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<MatcherL>, h2_matcher<typename h2_decay<MatcherR>::type>>> c(b);
   return c;
}
template <typename MatcherL, typename MatcherR>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<MatcherL>::type>, h2_polymorphic_matcher<MatcherR>>> operator||(const MatcherL& ml, const h2_polymorphic_matcher<MatcherR>& mr)
{
   h2_matcher<typename h2_decay<MatcherL>::type> a(ml);
   h2_or_matches<h2_matcher<typename h2_decay<MatcherL>::type>, h2_polymorphic_matcher<MatcherR>> b(a, mr);
   h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<MatcherL>::type>, h2_polymorphic_matcher<MatcherR>>> c(b);
   return c;
}
