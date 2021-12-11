template <typename EK, typename EV>
struct h2_pair_matches : h2_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(const EK& k_, const EV& v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.first)>::type>(k).matches(a.first, c.update_n(0).update_negative(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.second)>::type>(v).matches(a.second, c.update_n(0).update_negative(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return c.update_caseless(false).pre() + gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")");
   }
};

template <typename T>
struct h2_is_pair_matches : std::false_type {
};
template <typename MK, typename MV>
struct h2_is_pair_matches<h2_pair_matches<MK, MV>> : std::true_type {
};

template <typename T, typename = void>
struct h2_is_polymorphic_matcher_pair_matches : std::false_type {
};
template <typename T>
struct h2_is_polymorphic_matcher_pair_matches<T, typename std::enable_if<h2_is_polymorphic_matcher<T>::value && h2_is_pair_matches<typename T::matches_type>::value>::type> : std::true_type {
};

template <typename Matcher>
struct h2_has1_matches : h2_matches {
   Matcher m;
   explicit h2_has1_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto __matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_map<typename std::decay<A>::type>::value && !h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // HasKey scenario
      bool found = false;
      for (auto const& i : a) {
         h2_fail* fail = h2_matcher_cast<typename std::decay<decltype(i.first)>::type>(m).matches(i.first, c.update_n(0).update_negative(false));
         if (!fail) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto __matches(const A& a, h2_mc c) const -> typename std::enable_if<!h2_is_map<typename std::decay<A>::type>::value || h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // Normal scenario
      bool found = false;
      for (auto const& i : a) {
         h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(i, c.update_n(0).update_negative(false));
         if (!fail) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return __matches(a, c);
   }

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return matches(underlying_container(a), c);
   }

   template <typename A>
   auto matches(A a, h2_mc c) const -> typename std::enable_if<!h2_is_iterable<typename std::decay<A>::type>::value && !h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      bool found = false;
      for (size_t i = 0; i < c.n; ++i) {
         h2_fail* fail = h2_matcher_cast<typename std::decay<decltype(a[i])>::type>(m).matches(a[i], c.update_n(0).update_negative(false));
         if (!fail) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, c.n, true));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return c.update_caseless(false).pre() + "Has" + gray("(") + h2_matches_expection(m, c.update_negative(false)) + gray(")");
   }
};

template <typename EK, typename EV>
struct h2_has2_matches : h2_matches {
   const EK k;
   const EV v;
   const char* type;
   explicit h2_has2_matches(const EK& k_, const EV& v_, const char* type_ = "Has") : k(k_), v(v_), type(type_) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      bool found = false;
      for (auto const& i : a) {
         h2_fail* fails = nullptr;
         h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(i.first)>::type>(k).matches(i.first, c.update_n(0).update_negative(false)));
         h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(i.second)>::type>(v).matches(i.second, c.update_n(0).update_negative(false)));
         if (!fails) {
            found = true;
            break;
         }
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      h2_line t;
      if (strcmp("HasValue", type)) t += h2_matches_expection(k, c.update_negative(false));
      if (!strcmp("Has", type)) t += ", ";
      if (strcmp("HasKey", type)) t += h2_matches_expection(v, c.update_negative(false));
      return c.update_caseless(false).pre() + type + gray("(") + t + gray(")");
   }
};

template <typename Matcher>
struct h2_countof_matches : h2_matches {
   Matcher m;
   explicit h2_countof_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_sizable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return __matches(a.size(), h2_stringify(a, true), c);
   }

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<!h2_is_sizable<typename std::decay<A>::type>::value && h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {  // std::forward_list no size()
      size_t count = 0;
      for (auto it = a.cbegin(); it != a.cend(); ++it) count++;
      return __matches(count, h2_stringify(a, true), c);
   }

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<!h2_is_sizable<typename std::decay<A>::type>::value && !h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return __matches(c.n, h2_stringify(a, c.n, true), c);
   }

   h2_fail* __matches(size_t count, h2_line&& represent, h2_mc c) const
   {
      h2_fail* fails = h2_matcher_cast<size_t>(m).matches(count, c.update_n(0).update_negative(false));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      return h2_fail::new_unexpect(expection(c), represent + "/" + h2_stringify(count));
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return c.update_caseless(false).pre() + "CountOf" + gray("(") + h2_matches_expection(m, c.update_caseless(false).update_negative(false)) + gray(")");
   }
};

template <typename... Matchers>
struct h2_listof_matches : h2_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;

      h2_vector<h2_matcher<typename A::value_type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, count = 0;
         for (auto const& k : a) {
            if (j++ == i) {
               ++count;
               fail = v_matchers[i].matches(k, c.update_n(0).update_negative(false));
               break;
            }
         }
         if (count == 0) {
            fail = h2_fail::new_unexpect(v_matchers[i].expection(c.update_negative(false)), "", "but out of range");
         }
         if (fail) fail->seqno = i;
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

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return matches(underlying_container(a), c);
   }

   template <typename A> /* c/c++ generic array */
   auto matches(A a, h2_mc c) const -> typename std::enable_if<!h2_is_iterable<typename std::decay<A>::type>::value && !h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      h2_vector<h2_matcher<typename std::decay<decltype(a[0])>::type>> v_matchers;
      t2v(v_matchers);

      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a[i], c.update_n(0).update_negative(false));
         if (fail) fail->seqno = i;
         h2_fail::append_subling(fails, fail);
      }
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, c.n, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }

   virtual h2_line expection(h2_mc c) const override
   {
      return c.update_caseless(false).pre() + gray("[") + t2e(c.update_negative(false)) + gray("]");
   }

   H2_MATCHES_T2V2E(t_matchers)
};

template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_pair_matches<EK, EV>> Pair(const MK& mk, const MV& mv)
{
   return h2_polymorphic_matcher<h2_pair_matches<EK, EV>>(h2_pair_matches<EK, EV>(mk, mv));
}

template <typename Matcher>
inline h2_polymorphic_matcher<h2_has1_matches<typename std::decay<const Matcher&>::type>> Has(const Matcher& m)
{
   return h2_polymorphic_matcher<h2_has1_matches<typename std::decay<const Matcher&>::type>>(h2_has1_matches<typename std::decay<const Matcher&>::type>(m));
}

template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_has2_matches<EK, EV>> Has(const MK& mk, const MV& mv)
{
   return h2_polymorphic_matcher<h2_has2_matches<EK, EV>>(h2_has2_matches<EK, EV>(mk, mv));
}

template <typename MK, typename EK = typename h2_decay<MK>::type, typename EV = h2_polymorphic_matcher<h2_matches_any>>
inline h2_polymorphic_matcher<h2_has2_matches<EK, EV>> HasKey(const MK& mk)
{
   return h2_polymorphic_matcher<h2_has2_matches<EK, EV>>(h2_has2_matches<EK, EV>(mk, _, "HasKey"));
}
template <typename MV, typename EV = typename h2_decay<MV>::type, typename EK = h2_polymorphic_matcher<h2_matches_any>>
inline h2_polymorphic_matcher<h2_has2_matches<EK, EV>> HasValue(const MV& mv)
{
   return h2_polymorphic_matcher<h2_has2_matches<EK, EV>>(h2_has2_matches<EK, EV>(_, mv, "HasValue"));
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
