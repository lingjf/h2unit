
template <typename Matcher>
struct h2_not_matches {
   const Matcher m;
   explicit h2_not_matches(Matcher _m) : m(_m) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      return h2_matcher_cast<A>(m).matches(a, caseless, !dont);
   }
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return h2_matcher_cast<typename std::decay<A>::type>(m).expects(caseless, !dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_and_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_and_matches(Matcher1 _m1, Matcher2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m1).matches(a, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<A>(m2).matches(a, caseless, false));
      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(h2_type<A>(), caseless, dont));
      }
      return fail;
   }
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<typename std::decay<A>::type>(m1).expects(caseless, false);
      h2_string s2 = h2_matcher_cast<typename std::decay<A>::type>(m2).expects(caseless, false);
      return CD(s1 + " && " + s2, false, dont);
   }
};

template <typename Matcher1, typename Matcher2>
struct h2_or_matches {
   const Matcher1 m1;
   const Matcher2 m2;
   explicit h2_or_matches(Matcher1 _m1, Matcher2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, caseless, false);
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, caseless, false);
      bool result = !f1 || !f2;
      if (result == !dont) {
         if (f1) delete f1;
         if (f2) delete f2;
         return nullptr;
      }
      return h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(h2_type<A>(), caseless, dont));
   }
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<typename std::decay<A>::type>(m1).expects(caseless, false);
      h2_string s2 = h2_matcher_cast<typename std::decay<A>::type>(m2).expects(caseless, false);
      return CD(s1 + " || " + s2, false, dont);
   }
};

template <typename... Matchers>
struct h2_allof_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, caseless, false);
         if (fail) fail->no = h2_stringify(i);
         h2_fail::append_subling(fails, fail);
      }

      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(h2_type<A>(), caseless, dont), "Should not match all");
      } else {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(h2_type<A>(), caseless, dont));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      h2_string ret;
      using value_type = typename std::decay<A>::type;
      auto v_matchers = t2v<value_type, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         ret += Comma[!!i] + v_matchers[i].expects(caseless, false);
      }
      return CD("AllOf(" + ret + ")", false, dont);
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      int c = 0;
      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a, caseless, false);
         if (!fail) {
            c++;
            break;
         }
         if (fail) fail->no = h2_stringify(i);
         h2_fail::append_subling(fails, fail);
      }

      if ((0 < c) == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = nullptr;
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(h2_type<A>(), caseless, dont), "Should not match any one");
      } else {
         fail = h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(h2_type<A>(), caseless, dont), "Not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      h2_string ret;
      using value_type = typename std::decay<A>::type;
      auto v_matchers = t2v<value_type, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         ret += Comma[!!i] + v_matchers[i].expects(caseless, false);
      }
      return CD("AnyOf(" + ret + ")", false, dont);
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();
      int c = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, caseless, false);
         if (!fail) ++c;
         if (fail) delete fail;
      }
      if ((c == 0) == !dont) return nullptr;
      return h2_fail::new_unexpect("", h2_quote_stringfiy(a), expects(h2_type<A>(), caseless, dont));
   }

   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      h2_string ret;
      using value_type = typename std::decay<A>::type;
      auto v_matchers = t2v<value_type, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         ret += Comma[!!i] + v_matchers[i].expects(caseless, false);
      }
      return CD("NoneOf(" + ret + ")", false, dont);
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher m)
{
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(m));
}
template <typename Matches>
inline h2_polymorphic_matcher<h2_not_matches<h2_polymorphic_matcher<Matches>>> operator!(const h2_polymorphic_matcher<Matches>& m) { return Not(m); }

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
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator&&(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator||(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}
