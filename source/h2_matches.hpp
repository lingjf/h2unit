
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
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return h2_matcher_cast<A>(m).expects(a, caseless, !dont);
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
      if (!fail == !dont) return nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
      }
      return fail;
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<A>(m1).expects(a, caseless, false);
      h2_string s2 = h2_matcher_cast<A>(m2).expects(a, caseless, false);
      return CD(s1 + "&&" + s1, caseless, dont);
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
      if (result == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_string s1 = h2_matcher_cast<A>(m1).expects(a, caseless, false);
      h2_string s2 = h2_matcher_cast<A>(m2).expects(a, caseless, false);
      return CD(s1 + "||" + s1, caseless, dont);
   }
};

template <typename Matcher>
struct h2_pointee_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher _m) : m(_m) {}

   template <typename SmartPointer>
   struct PointeeOf {
      typedef typename SmartPointer::element_type type;
   };
   template <typename T>
   struct PointeeOf<T*> {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, caseless, dont);
   }
   template <typename A>
   h2_string expects(const A a, bool caseless = false, bool dont = false) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).expects(*a, caseless, dont);
   }
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return m.matches(a, true, dont); }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return m.expects(a, true, dont); }
};

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
      if (!fail == !dont) return nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", "{" + h2_stringify(a.first) + ", " + h2_stringify(a.second) + "}", expects(a, caseless, dont));
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

      if (!fails == !dont) return nullptr;
      h2_fail* fail = nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont), "Should not match all");
      } else {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

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

      if ((0 < c) == !dont) return nullptr;
      h2_fail* fail = nullptr;
      if (dont) {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont), "Should not match any one");
      } else {
         fail = new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont), "Not match any one");
         h2_fail::append_child(fail, fails);
      }
      return fail;
   }

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

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
      }
      if ((c == 0) == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
   }

   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return ""; }

   H2_MATCHER_T2V(t_matchers)
};

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
      }

      if (0 < s == !dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify(a), expects(a, caseless, dont));
   }
   template <typename A>
   h2_string expects(A a, bool caseless = false, bool dont = false) const { return "In"; }

   H2_MATCHER_T2V(t_matchers)
};

const h2_polymorphic_matcher<h2_matchee_any> _{h2_matchee_any()};
const h2_polymorphic_matcher<h2_matchee_any> Any{h2_matchee_any()};
const h2_polymorphic_matcher<h2_matchee_null> IsNull{h2_matchee_null(false)};
const h2_polymorphic_matcher<h2_matchee_null> NotNull{h2_matchee_null(true)};
const h2_polymorphic_matcher<h2_matchee_boolean<true>> IsTrue{h2_matchee_boolean<true>()};
const h2_polymorphic_matcher<h2_matchee_boolean<false>> IsFalse{h2_matchee_boolean<false>()};

template <typename E>
inline h2_polymorphic_matcher<h2_equals<E>> Eq(const E expect) { return h2_polymorphic_matcher<h2_equals<E>>(h2_equals<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_not_matches<E>> Nq(const E expect) { return h2_polymorphic_matcher<h2_not_matches<E>>(h2_not_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_ge<E>> Ge(const E expect) { return h2_polymorphic_matcher<h2_matchee_ge<E>>(h2_matchee_ge<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_gt<E>> Gt(const E expect) { return h2_polymorphic_matcher<h2_matchee_gt<E>>(h2_matchee_gt<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_le<E>> Le(const E expect) { return h2_polymorphic_matcher<h2_matchee_le<E>>(h2_matchee_le<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_matchee_lt<E>> Lt(const E expect) { return h2_polymorphic_matcher<h2_matchee_lt<E>>(h2_matchee_lt<E>(expect)); }

inline h2_polymorphic_matcher<h2_matchee_memcmp> Me(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(len ? 8 : 0, (const void*)ptr, len * 8)); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M1e(const char* str) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(0, (const void*)str, strlen(str))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M8e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(8, (const void*)ptr, len ? len * 8 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M16e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(16, (const void*)ptr, len ? len * 16 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M32e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(32, (const void*)ptr, len ? len * 32 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_matchee_memcmp> M64e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_matchee_memcmp>(h2_matchee_memcmp(64, (const void*)ptr, len ? len * 64 : strlen((const char*)ptr))); }

inline h2_polymorphic_matcher<h2_matchee_regex> Re(const h2_string& regex_pattern) { return h2_polymorphic_matcher<h2_matchee_regex>(h2_matchee_regex(regex_pattern)); }
inline h2_polymorphic_matcher<h2_matchee_wildcard> We(const h2_string& wildcard_pattern) { return h2_polymorphic_matcher<h2_matchee_wildcard>(h2_matchee_wildcard(wildcard_pattern)); }
inline h2_polymorphic_matcher<h2_matchee_strcmp> Se(const h2_string& expect) { return h2_polymorphic_matcher<h2_matchee_strcmp>(h2_matchee_strcmp(expect)); }
inline h2_polymorphic_matcher<h2_matchee_substr> Substr(const h2_string& substring) { return h2_polymorphic_matcher<h2_matchee_substr>(h2_matchee_substr(substring)); }
inline h2_polymorphic_matcher<h2_matchee_startswith> StartsWith(const h2_string& prefix_string) { return h2_polymorphic_matcher<h2_matchee_startswith>(h2_matchee_startswith(prefix_string)); }
inline h2_polymorphic_matcher<h2_matchee_endswith> EndsWith(const h2_string& suffix_string) { return h2_polymorphic_matcher<h2_matchee_endswith>(h2_matchee_endswith(suffix_string)); }
inline h2_polymorphic_matcher<h2_matchee_json> Je(const h2_string& expect) { return h2_polymorphic_matcher<h2_matchee_json>(h2_matchee_json(expect)); }

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(M m) { return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(h2_matcher<h2_string>(m))); }
#ifndef _WIN32
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> operator~(M m)
{
   return CaseLess(m);
}
#endif
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
