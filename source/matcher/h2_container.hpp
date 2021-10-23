template <typename EK, typename EV>
struct h2_pair_matches : h2_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(const EK& k_, const EV& v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, int, h2_mc c) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.first)>::type>(k).matches(a.first, 0, c.update_dont(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.second)>::type>(v).matches(a.second, 0, c.update_dont(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(const A& a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect(expection(c), h2_representify(a));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD(gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")"), c.update_caseless(false));
   }
};

template <typename Matcher>
struct h2_has_matches : h2_matches {
   Matcher m;
   explicit h2_has_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, int n, h2_mc c) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      bool found = false;
      for (auto& i : a) {
         h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(i, n, c.update_dont(false));
         if (!fail) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a), DS(found));
   }

   template <typename A>
   auto matches(A a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      bool found = false;
      for (int i = 0; i < n; ++i) {
         h2_fail* fail = h2_matcher_cast<typename std::decay<decltype(a[i])>::type>(m).matches(a[i], n, c.update_dont(false));
         if (!fail) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a, n), DS(found));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("Has" + gray("(") + h2_matches_expection(m, c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }
};

template <typename Matcher>
struct h2_countof_matches : h2_matches {
   Matcher m;
   explicit h2_countof_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, int n, h2_mc c) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      int count = 0;
      for (auto& c : a) count++; /* container size() is best, but forward_list haven't. iterator works all, regardless speed. https://en.cppreference.com/w/cpp/container */
      return __matches(count, h2_representify(a), c);
   }

   template <typename A>
   auto matches(A a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return __matches(n, h2_representify(a, n), c);
   }

   h2_fail* __matches(int count, h2_line&& represent, h2_mc c) const
   {
      h2_fail* fails = h2_matcher_cast<int>(m).matches(count, 0, c.update_dont(false));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      return h2_fail::new_unexpect(expection(c), represent + "/" + h2_stringify(count));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("CountOf" + gray("(") + h2_matches_expection(m, c.update_caseless(false).update_dont(false)) + gray(")"), c.update_caseless(false));
   }
};

template <typename... Matchers>
struct h2_listof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, int, h2_mc c) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;

      h2_vector<h2_matcher<typename A::value_type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, count = 0;
         for (auto& k : a) {
            if (j++ == i) {
               ++count;
               fail = v_matchers[i].matches(k, 0, c.update_dont(false));
               break;
            }
         }
         if (count == 0) {
            fail = h2_fail::new_unexpect(v_matchers[i].expection(c.update_dont(false)), "", "but out of range");
         }
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(A a, int n, h2_mc c) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ generic array */
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<typename std::decay<decltype(a[0])>::type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a[i], n, c.update_dont(false));
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_representify(a, n), DS(!fails));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return CD("ListOf" + gray("(") + t2e(c.update_dont(false)) + gray(")"), c.update_caseless(false));
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_pair_matches<EK, EV>> Pair(const MK& mk, const MV& mv)
{
   return h2_polymorphic_matcher<h2_pair_matches<EK, EV>>(h2_pair_matches<EK, EV>(mk, mv));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matcher&>::type>> Has(const Matcher& m)
{
   return h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matcher&>::type>>(h2_has_matches<typename std::decay<const Matcher&>::type>(m));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matcher&>::type>> CountOf(const Matcher& m)
{
   return h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matcher&>::type>>(h2_countof_matches<typename std::decay<const Matcher&>::type>(m));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
