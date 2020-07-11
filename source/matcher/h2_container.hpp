
template <typename EK, typename EV>
struct h2_pair_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(EK k_, EV v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, bool caseless = false, bool dont = false) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      using AK = typename std::decay<decltype(a.first)>::type;
      using AV = typename std::decay<decltype(a.second)>::type;
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<AK>(k).matches(a.first, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<AV>(v).matches(a.second, caseless, false));
      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect("", h2_stringify(a), expects(h2_type<A>(), caseless, dont));
      }
      return fail;
   }

   template <typename A>
   auto matches(const A& a, bool caseless = false, bool dont = false) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect("", h2_stringify(a), expects(h2_type<A>(), caseless, dont));
   }

   template <typename A>
   auto expects(h2_type<A>, bool caseless = false, bool dont = false) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_string>::type
   {
      using AK = typename std::decay<typename std::decay<A>::type::first_type>::type;
      using AV = typename std::decay<typename std::decay<A>::type::second_type>::type;
      h2_string s1 = h2_matcher_cast<AK>(k).expects(caseless, false);
      h2_string s2 = h2_matcher_cast<AV>(v).expects(caseless, false);
      return CD("(" + s1 + ", " + s2 + ")", false, dont);
   }
   template <typename A>
   auto expects(h2_type<A>, bool caseless = false, bool dont = false) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_string>::type
   {
      return "Pair()";
   }
};

template <typename... Matchers>
struct h2_listof_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   auto matches(A a, bool caseless = false, bool dont = false) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      using value_type = typename std::decay<decltype(*a.begin())>::type;
      h2_fail* fails = nullptr;
      auto v_matchers = t2v<value_type, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, c = 0;
         for (auto& k : a) {
            if (j++ == i) {
               ++c;
               fail = v_matchers[i].matches(k, caseless, false);
               break;
            }
         }
         if (c == 0) {
            h2_string t2 = v_matchers[i].expects(caseless, false);
            fail = h2_fail::new_unexpect("", "[missing]", t2);
         }
         if (fail) fail->no = h2_stringify(i);
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", h2_stringify(a), expects(h2_type<A>(), caseless, dont), "fails");
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto matches(A a, bool caseless = false, bool dont = false) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      /* c/c++ array */
      h2_fail* fails = nullptr;
      auto v_matchers = t2v<typename std::decay<decltype(a[0])>::type, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         h2_fail* fail = v_matchers[i].matches(a[i], caseless, false);
         if (fail) fail->no = h2_stringify(i);
         h2_fail::append_subling(fails, fail);
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", h2_stringify(a), expects(h2_type<A>(), caseless, dont), "fails");
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   auto expects(h2_type<A>, bool caseless = false, bool dont = false) const -> typename std::enable_if<h2_is_container<typename std::decay<A>::type>::value, h2_string>::type
   {
      using value_type = typename std::decay<typename std::decay<A>::type::value_type>::type;
      return __expects(h2_type<value_type>(), caseless, dont);
   }

   template <typename A>
   auto expects(h2_type<A>, bool caseless = false, bool dont = false) const -> typename std::enable_if<!h2_is_container<typename std::decay<A>::type>::value, h2_string>::type
   {
      using value_type = typename std::decay<decltype(std::declval<A>()[0])>::type;
      return __expects(h2_type<value_type>(), caseless, dont);
   }

   template <typename value_type>
   h2_string __expects(h2_type<value_type>, bool caseless = false, bool dont = false) const
   {
      h2_string ret;
      auto v_matchers = t2v<value_type, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         ret += Comma[!!i] + v_matchers[i].expects(caseless, false);
      }
      return CD("ListOf(" + ret + ")", caseless, dont);
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_has_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_has_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      using type = typename A::value_type;
      auto v_matchers = t2v<type, 0>();

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         bool found = false;
         for (auto& j : a) {
            h2_fail* fail = v_matchers[i].matches(j, caseless, false);
            if (!fail) {
               found = true;
               break;
            }
         }
         if (!found) {
            h2_string t2 = v_matchers[i].expects(caseless, false);
            h2_fail* fail = h2_fail::new_normal(nullptr, 0, nullptr, "Not has %s", t2.c_str());
            if (fail) fail->no = h2_stringify(i);
            h2_fail::append_subling(fails, fail);
         }
      }
      if (!fails == !dont) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect("", h2_stringify(a), expects(h2_type<A>(), caseless, dont), "fails");
      h2_fail::append_child(fail, fails);
      return fail;
   }

   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      h2_string ret;
      using value_type = typename std::decay<A>::type::value_type;
      auto v_matchers = t2v<value_type, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         ret += Comma[!!i] + v_matchers[i].expects(caseless, false);
      }
      return CD("Has(" + ret + ")", caseless, dont);
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_in_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_in_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, caseless, false);
         if (!fail) ++s;
         if (fail) delete fail;
      }

      if (0 < s == !dont) return nullptr;
      return h2_fail::new_unexpect("", h2_stringify(a), expects(h2_type<A>(), caseless, dont));
   }

   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      h2_string ret;
      auto v_matchers = t2v<A, 0>();
      for (size_t i = 0; i < v_matchers.size(); ++i) {
         ret += Comma[!!i] + v_matchers[i].expects(caseless, false);
      }

      return CD("In(" + ret + ")", caseless, dont);
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename MK, typename MV>
inline h2_polymorphic_matcher<h2_pair_matches<MK, MV>> Pair(const MK mk, const MV mv)
{
   return h2_polymorphic_matcher<h2_pair_matches<MK, MV>>(h2_pair_matches<MK, MV>(mk, mv));
}

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>> Has(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_has_matches<typename std::decay<const Matchers&>::type...>>(h2_has_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>> In(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_in_matches<typename std::decay<const Matchers&>::type...>>(h2_in_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
