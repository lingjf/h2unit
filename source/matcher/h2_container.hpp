
template <typename KMatcher, typename VMatcher>
struct h2_pair_matches {
   const KMatcher k;
   const VMatcher v;
   explicit h2_pair_matches(KMatcher k_, VMatcher v_) : k(k_), v(v_) {}

   template <typename AK, typename AV>
   operator h2_pair_matches<AK, AV>();

   template <typename AK, typename AV>
   h2_fail* matches(const std::pair<AK, AV>& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* fail = nullptr;
      h2_fail::append_subling(fail, h2_matcher_cast<AK>(k).matches(a.first, caseless, false));
      h2_fail::append_subling(fail, h2_matcher_cast<AV>(v).matches(a.second, caseless, false));
      if (!fail == !dont) {
         if (fail) delete fail;
         return nullptr;
      }
      if (dont) {
         fail = h2_fail::new_unexpect("", "{" + h2_stringify(a.first) + ", " + h2_stringify(a.second) + "}", expects(a, caseless, dont));
      }
      return fail;
   }
   template <typename AK, typename AV>
   h2_string expects(const std::pair<AK, AV>& a, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<AK>(k).expects(a.first, caseless, false);
      h2_string s2 = h2_matcher_cast<AV>(v).expects(a.second, caseless, false);
      return CD("{" + s1 + ", " + s2 + "}", false, dont);
   }
};

template <typename KMatcher, typename VMatcher>
template <typename AK, typename AV>
inline h2_pair_matches<KMatcher, VMatcher>::operator h2_pair_matches<AK, AV>()
{
   return h2_pair_matches<AK, AV>(h2_matcher_cast<AK>(k), h2_matcher_cast<AV>(v));
}

template <typename M1, typename M2>
inline h2_pair_matches<M1, M2> Pair(const M1 m1, const M2 m2)
{
   return h2_pair_matches<M1, M2>(m1, m2);
}

#define H2_MATCHER_T2V(t_matchers)                                                                                                                          \
   template <typename T, size_t I>                                                                                                                          \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), h2_vector<h2_matcher<T>> v_matchers = {}) const \
   {                                                                                                                                                        \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                                                                    \
      return t2v<T>(std::integral_constant<size_t, I + 1>(), v_matchers);                                                                                   \
   }                                                                                                                                                        \
   template <typename T>                                                                                                                                    \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, sizeof...(Matchers)>, h2_vector<h2_matcher<T>> v_matchers = {}) const { return v_matchers; }

#define H2_MATCHER_T4V(t_matchers)                                                                                                                                          \
   template <typename K, typename V, size_t I>                                                                                                                              \
   h2_vector<h2_pair_matches<K, V>> t4v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), h2_vector<h2_pair_matches<K, V>> v_matchers = {}) const \
   {                                                                                                                                                                        \
      v_matchers.push_back((h2_pair_matches<K, V>)std::get<I>(t_matchers));                                                                                                 \
      return t4v<K, V>(std::integral_constant<size_t, I + 1>(), v_matchers);                                                                                                \
   }                                                                                                                                                                        \
   template <typename K, typename V>                                                                                                                                        \
   h2_vector<h2_pair_matches<K, V>> t4v(std::integral_constant<size_t, sizeof...(Matchers)>, h2_vector<h2_pair_matches<K, V>> v_matchers = {}) const { return v_matchers; }

template <typename... Matchers>
struct h2_listof_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::vector<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::deque<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::list<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::forward_list<A> a, bool caseless = false, bool dont = false) const;

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_has_matches {
   std::tuple<Matchers...> t_matchers;
   explicit h2_has_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const;
   template <typename A, size_t N>
   h2_fail* matches(const std::array<A, N> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::vector<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::deque<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::list<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::forward_list<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::set<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::multiset<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::unordered_set<A> a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_fail* matches(const std::unordered_multiset<A> a, bool caseless = false, bool dont = false) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::map<AK, AV> a, bool caseless, bool dont) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::multimap<AK, AV> a, bool caseless, bool dont) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::unordered_map<AK, AV> a, bool caseless, bool dont) const;
   template <typename AK, typename AV>
   h2_fail* matches(const std::unordered_multimap<AK, AV> a, bool caseless, bool dont) const;

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

   H2_MATCHER_T2V(t_matchers)
   H2_MATCHER_T4V(t_matchers)
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
      return h2_fail::new_unexpect("", h2_stringify(a), expects(a, caseless, dont));
   }
   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return "In"; }

   H2_MATCHER_T2V(t_matchers)
};

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
