template <typename... Matchers>
struct h2_matcher_tuple {
   std::tuple<Matchers...> t_matchers;
   explicit h2_matcher_tuple(const Matchers&... matchers) : t_matchers(matchers...) {}

   template <typename T>
   void __matchers(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}
   template <typename T, std::size_t I>
   void __matchers(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const
   {
      __matchers(v_matchers, std::integral_constant<std::size_t, I - 1>());
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));
   }
   template <typename T>
   void matchers(h2_vector<h2_matcher<T>>& v_matchers) const { return __matchers(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }

   h2_line __expection(C c, std::integral_constant<std::size_t, 0>) const { return {}; }
   template <std::size_t I>
   h2_line __expection(C c, std::integral_constant<std::size_t, I>) const
   {
      return __expection(c, std::integral_constant<size_t, I - 1>()) + gray(comma_if(1 < I)) + h2_matches_expection(std::get<I - 1>(t_matchers), c);
   }
   h2_line expection(C c) const { return __expection(c, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
};

template <typename EK, typename EV>
struct h2_pair_matches : h2_matches {
   const EK k;
   const EV v;
   explicit h2_pair_matches(const EK& k_, const EV& v_) : k(k_), v(v_) {}

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.first)>::type>(k).matches(a.first, c.clear_size().update_negative(false)));
      h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(a.second)>::type>(v).matches(a.second, c.clear_size().update_negative(false)));
      if (c.fit(!fails)) {
         if (fails) delete fails;
         return nullptr;
      }
      h2_fail* fail = h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
      h2_fail::append_child(fail, fails);
      return fail;
   }
   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<!h2_is_pair<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(C c) const override
   {
      return c.update_caseless(false).pre() + gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")");
   }
};

template <typename T>
struct h2_is_pair_matches : std::false_type {};
template <typename MK, typename MV>
struct h2_is_pair_matches<h2_pair_matches<MK, MV>> : std::true_type {};

template <typename T, typename = void>
struct h2_is_polymorphic_matcher_pair_matches : std::false_type {};
template <typename T>
struct h2_is_polymorphic_matcher_pair_matches<T, typename std::enable_if<h2_is_polymorphic_matcher<T>::value && h2_is_pair_matches<typename T::matches_type>::value>::type> : std::true_type {};

#define H2_MATCHES_CONTAINER1()                                                                                                                                                                                \
   template <typename A>                                                                                                                                                                                       \
   auto matches(const A& a, C c) const->typename std::enable_if<h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type                                                                  \
   {                                                                                                                                                                                                           \
      return matches(underlying_container(a), c);                                                                                                                                                              \
   }                                                                                                                                                                                                           \
   template <typename A>                                                                                                                                                                                       \
   auto matches(A a, C c) const->typename std::enable_if<!h2_is_iterable<typename std::decay<A>::type>::value && !h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type                \
   { /* native c/c++ array or pointer */                                                                                                                                                                       \
      return matches(h2_array<decltype(a[0])>(a, c.array_size == -1 ? (c.range_end - c.range_start < Range::end - Range::start ? c.range_end - c.range_start : Range::end - Range::start) : c.array_size), c); \
   }

#define H2_MATCHES_CONTAINER2(name)                                                                                                                                 \
   H2_MATCHES_CONTAINER1()                                                                                                                                          \
   virtual h2_line expection(C c) const override                                                                                                                    \
   {                                                                                                                                                                \
      return c.update_caseless(false).pre() + (name) + gray("(") + h2_matches_expection(m, c.update_caseless(false).update_negative(false)) + gray(")") + c.post(); \
   }                                                                                                                                                                \
   bool range_in(const C& c, const int i) const { return c.in(i) && Range::in(i); }

#define H2_MATCHES_STATS(value, prev_result)                                                                       \
   bool result = (prev_result);                                                                                    \
   if (result) {                                                                                                   \
      h2_fail* fail = h2_matcher_cast<decltype(value)>(m).matches((value), c.clear_size().update_negative(false)); \
      if (fail) delete fail, result = false;                                                                       \
   }                                                                                                               \
   if (c.fit(result)) return nullptr;                                                                              \
   return h2_fail::new_unexpect(expection(c), ((prev_result) ? h2_stringify(value) : h2_line()) + h2_stringify(a, true));

template <typename Range, typename Matcher>  // [Start, End)
struct h2_every1_matches : h2_matches {
   Matcher m;
   explicit h2_every1_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      int i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) {
            h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(ia, c.clear_size().update_negative(false));
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
   H2_MATCHES_CONTAINER2("Every")
};

template <typename Range, typename EK, typename EV, typename M = h2_pair_matches<EK, EV>, typename P = h2_polymorphic_matcher<M>>
struct h2_every2_matches : h2_every1_matches<Range, P> {
   EK k;
   EV v;
   explicit h2_every2_matches(const EK& k_, const EV& v_) : h2_every1_matches<Range, P>(P(M(k_, v_))), k(k_), v(v_) {}
   virtual h2_line expection(C c) const override
   {
      return c.update_caseless(false).pre() + "Every" + gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")");
   }
};

template <typename Range, typename Matcher>
struct h2_has1_matches : h2_matches {
   Matcher m;
   explicit h2_has1_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto __matches(const A& a, C c) const -> typename std::enable_if<h2_is_map<typename std::decay<A>::type>::value && !h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // HasKey scenario
      int found = 0, i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) {
            h2_fail* fail = h2_matcher_cast<typename std::decay<decltype(ia.first)>::type>(m).matches(ia.first, c.clear_size().update_negative(false));
            if (!fail) {
               if (++found >= c.times) break;
            } else
               delete fail;
         }
      if (c.fit(found >= c.times)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto __matches(const A& a, C c) const -> typename std::enable_if<!h2_is_map<typename std::decay<A>::type>::value || h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // Normal scenario
      int found = 0, i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) {
            h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(ia, c.clear_size().update_negative(false));
            if (!fail) {
               if (++found >= c.times) break;
            } else
               delete fail;
         }
      if (c.fit(found >= c.times)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type { return __matches(a, c); }

   H2_MATCHES_CONTAINER2("Has")
};

template <typename EK, typename EV, typename M = h2_pair_matches<EK, EV>, typename P = h2_polymorphic_matcher<M>>
struct h2_has2_matches : h2_has1_matches<h2_range<0, 5413722>, P> {
   const EK k;
   const EV v;
   const char* type;
   explicit h2_has2_matches(const EK& k_, const EV& v_, const char* type_ = "Has") : h2_has1_matches<h2_range<0, 5413722>, P>(P(M(k_, v_))), k(k_), v(v_), type(type_) {}

   virtual h2_line expection(C c) const override
   {
      h2_line t;
      if (strcmp("HasValue", type)) t += h2_matches_expection(k, c.update_negative(false));
      if (!strcmp("Has", type)) t += ", ";
      if (strcmp("HasKey", type)) t += h2_matches_expection(v, c.update_negative(false));
      return c.update_caseless(false).pre() + type + gray("(") + t + gray(")") + c.post();
   }
};

template <typename Range, typename... Matchers>
struct h2_listof_matches : h2_matches {
   h2_matcher_tuple<Matchers...> e;
   explicit h2_listof_matches(const Matchers&... matchers) : e(matchers...) {}

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_vector<h2_matcher<typename A::value_type>> matchers;
      e.matchers(matchers);

      h2_fail* fails = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i) {
         h2_fail* fail = nullptr;
         size_t j = 0, count = 0;
         for (auto const& ia : a) {
            if (j++ == i + (c.range_start > Range::start ? c.range_start : Range::start)) {
               ++count;
               fail = matchers[i].matches(ia, c.clear_size().update_negative(false));
               break;
            }
         }
         if (count == 0) fail = h2_fail::new_unexpect(matchers[i].expection(c.update_negative(false)), "", "but out of range");
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
   virtual h2_line expection(C c) const override
   {
      return c.update_caseless(false).pre() + gray("[") + e.expection(c.update_negative(false)) + gray("]");
   }
   H2_MATCHES_CONTAINER1()
};

template <typename Matcher, typename Range = h2_range<0, 5413722>>
struct h2_countof_matches : h2_matches {
   Matcher m;
   explicit h2_countof_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      size_t count = 0;  // std::forward_list no size()
      for (auto it = a.cbegin(); it != a.cend(); ++it) count++;
      H2_MATCHES_STATS(count, true);
   }
   H2_MATCHES_CONTAINER2("CountOf")
};

template <typename Range, typename Matcher>
struct h2_maxmin_matches : h2_matches {
   const bool is_max;
   Matcher m;
   explicit h2_maxmin_matches(const bool is_max_, const Matcher& m_) : is_max(is_max_), m(m_) {}

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      int i = 0, count = 0;
      typename A::value_type value;
      for (auto const& ia : a)  // std::max_element / std::max_element
         if (range_in(c, i++)) {
            if (++count == 1) value = ia;
            if (is_max ? (ia > value) : (ia < value)) value = ia;
         }
      H2_MATCHES_STATS(value, count != 0);
   }
   H2_MATCHES_CONTAINER2(is_max ? "MaxOf" : "MinOf")
};

template <typename Range, typename Matcher>
struct h2_avg_matches : h2_matches {
   Matcher m;
   explicit h2_avg_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      double sum = 0, count = 0;
      int i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++)) sum += ia, ++count;
      H2_MATCHES_STATS(count ? sum / count : 0, count != 0);
   }
   H2_MATCHES_CONTAINER2("AvgOf")
};

template <typename Range, typename Matcher>
struct h2_median_matches : h2_matches {
   Matcher m;
   explicit h2_median_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      std::vector<typename A::value_type> b;
      int i = 0;
      for (auto const& ia : a)
         if (range_in(c, i++))
            for (auto it = b.begin();; ++it)  // std::sort(b.begin(), b.end());  // std::nth_element /// avoid include <algorithm> for compile time
               if (it == b.end() || ia < *it) {
                  b.insert(it, ia);
                  break;
               }
      double value = 0;
      if (b.size()) value = b.size() % 2 ? b[b.size() / 2] : (b[b.size() / 2] + b[b.size() / 2 + 1]) / 2;
      H2_MATCHES_STATS(value, b.size() != 0);
   }
   H2_MATCHES_CONTAINER2("MedianOf")
};

template <typename TK, typename TV, typename EK = typename h2_decay<TK>::type, typename EV = typename h2_decay<TV>::type, typename P = h2_polymorphic_matcher<h2_pair_matches<EK, EV>>>
inline P Pair(const TK& expect_key, const TV& expect_value) { return P(h2_pair_matches<EK, EV>(expect_key, expect_value)); }

template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_has1_matches<h2_range<Rs, Re>, E>>>
inline P Has(const T& expect) { return P(h2_has1_matches<h2_range<Rs, Re>, E>(expect)); }
template <typename TK, typename TV, typename EK = typename h2_decay<TK>::type, typename EV = typename h2_decay<TV>::type, typename P = h2_polymorphic_matcher<h2_has2_matches<EK, EV>>>
inline P Has(const TK& expect_key, const TV& expect_value) { return P(h2_has2_matches<EK, EV>(expect_key, expect_value)); }
template <typename TK, typename EK = typename h2_decay<TK>::type, typename EV = h2_polymorphic_matcher<h2_matches_any>, typename P = h2_polymorphic_matcher<h2_has2_matches<EK, EV>>>
inline P HasKey(const TK& expect_key) { return P(h2_has2_matches<EK, EV>(expect_key, _, "HasKey")); }
template <typename TV, typename EV = typename h2_decay<TV>::type, typename EK = h2_polymorphic_matcher<h2_matches_any>, typename P = h2_polymorphic_matcher<h2_has2_matches<EK, EV>>>
inline P HasValue(const TV& expect_value) { return P(h2_has2_matches<EK, EV>(_, expect_value, "HasValue")); }

template <int Rs = 5413722, int Re = -1, typename... T, typename P = h2_polymorphic_matcher<h2_listof_matches<h2_range<Rs, Re>, typename std::decay<const T&>::type...>>>
inline P ListOf(const T&... expects) { return P(h2_listof_matches<h2_range<Rs, Re>, typename std::decay<const T&>::type...>(expects...)); }

template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_every1_matches<h2_range<Rs, Re>, E>>>
inline P Every(const T& expect) { return P(h2_every1_matches<h2_range<Rs, Re>, E>(expect)); }
template <int Rs = 5413722, int Re = -1, typename T1, typename T2, typename E1 = typename std::decay<const T1&>::type, typename E2 = typename std::decay<const T2&>::type, typename P = h2_polymorphic_matcher<h2_every2_matches<h2_range<Rs, Re>, E1, E2>>>
inline P Every(const T1& expect1, const T2& expect2) { return P(h2_every2_matches<h2_range<Rs, Re>, E1, E2>(expect1, expect2)); }

template <typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_countof_matches<E>>>
inline P CountOf(const T& expect) { return P(h2_countof_matches<E>(expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_maxmin_matches<h2_range<Rs, Re>, E>>>
inline P MaxOf(const T& expect) { return P(h2_maxmin_matches<h2_range<Rs, Re>, E>(true, expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_maxmin_matches<h2_range<Rs, Re>, E>>>
inline P MinOf(const T& expect) { return P(h2_maxmin_matches<h2_range<Rs, Re>, E>(false, expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_avg_matches<h2_range<Rs, Re>, E>>>
inline P AvgOf(const T& expect) { return P(h2_avg_matches<h2_range<Rs, Re>, E>(expect)); }
template <int Rs = 5413722, int Re = -1, typename T, typename E = typename std::decay<const T&>::type, typename P = h2_polymorphic_matcher<h2_median_matches<h2_range<Rs, Re>, E>>>
inline P MedianOf(const T& expect) { return P(h2_median_matches<h2_range<Rs, Re>, E>(expect)); }
