template <typename Matcher>
struct h2_not_matches : h2_matches {
   const Matcher m; /* h2_polymorphic_matcher or immediate value or h2_matches */
   explicit h2_not_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      return h2_matcher_cast<A>(m).matches(a, n, c.update_dont(!c.dont));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return h2_matches_expection(m, c.update_dont(!c.dont));
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_and_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_and_matches(const Matcher1& m1_, const Matcher2& m2_) : m1(m1_), m2(m2_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m1).matches(a, n, c.update_dont(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<A>(m2).matches(a, n, c.update_dont(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);

      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.dont ? gray("(") : h2_line()) + h2_matches_expection(m1, c.update_dont(false)) + " and " + h2_matches_expection(m2, c.update_dont(false)) + (c.dont ? gray(")") : h2_line()), c.update_caseless(false));
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_or_matches : h2_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_or_matches(const Matcher1& m1_, const Matcher2& m2_) : m1(m1_), m2(m2_) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, n, c.update_dont(false));
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, n, c.update_dont(false));
      if (c.fit(!f1 || !f2)) {
         if (f1) delete f1;
         if (f2) delete f2;
         return nullptr;
      }
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, f1);
      h2_fail::append_subling(fails, f2);

      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD((c.dont ? gray("(") : h2_line()) + h2_matches_expection(m1, c.update_dont(false)) + " or " + h2_matches_expection(m2, c.update_dont(false)) + (c.dont ? gray(")") : h2_line()), c.update_caseless(false));
   }
};

template <typename... Matchers>
struct h2_allof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, c.update_dont(false));  // dont not transfer down
         if (fail) fail->seqno = (int)i;
         h2_fail::append_subling(fails, fail);
      }

      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (c.dont) {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a), "should not match all");
      } else {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("AllOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      int count = 0;
      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, c.update_dont(false));
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
      h2_fail* fail = nullptr;
      if (c.dont) {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a), "should not match any one");
      } else {
         fail = h2_fail::new_unexpect(expection(c), h2_representify(a), "not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("AnyOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   h2_fail* matches(const A& a, int n, h2_mc c) const
   {
      h2_vector<h2_matcher<A>> v_matchers;
      t2v(v_matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, n, c.update_dont(false));
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
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("NoneOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
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

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> operator&&(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> operator&&(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> operator&&(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> operator||(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> operator||(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> operator||(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}
