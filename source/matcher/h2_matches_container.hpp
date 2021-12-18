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

#define H2_MATCHES_CONTAINER1(Un)                                                                                                                                                                   \
   template <typename A>                                                                                                                                                                            \
   auto matches(const A& a, h2_mc c) const->typename std::enable_if<h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type                                                   \
   {                                                                                                                                                                                                \
      return matches(underlying_container(a), c);                                                                                                                                                   \
   }                                                                                                                                                                                                \
   template <typename A>                                                                                                                                                                            \
   auto matches(A a, h2_mc c) const->typename std::enable_if<!h2_is_iterable<typename std::decay<A>::type>::value && !h2_is_container_adaptor<typename std::decay<A>::type>::value, h2_fail*>::type \
   {                                                                                                                                                                                                \
      return matches(h2_array<decltype(a[0])>(a, c.n == -1 ? (Un != 0x7fffffff ? Un : 5413722) : c.n), c);                                                                                          \
   }

#define H2_MATCHES_CONTAINER2(Un, name)                                                                                                                  \
   H2_MATCHES_CONTAINER1(Un)                                                                                                                             \
   virtual h2_line expection(h2_mc c) const override                                                                                                     \
   {                                                                                                                                                     \
      return c.update_caseless(false).pre() + (name) + gray("(") + h2_matches_expection(m, c.update_caseless(false).update_negative(false)) + gray(")"); \
   }

#define H2_MATCHES_STATS(value, prev_result)                                                                      \
   bool result = (prev_result);                                                                                   \
   if (result) {                                                                                                  \
      h2_fail* fail = h2_matcher_cast<decltype(value)>(m).matches((value), c.update_n(0).update_negative(false)); \
      if (fail) {                                                                                                 \
         delete fail;                                                                                             \
         result = false;                                                                                          \
      }                                                                                                           \
   }                                                                                                              \
   if (c.fit(result)) return nullptr;                                                                             \
   return h2_fail::new_unexpect(expection(c), ((prev_result) ? h2_stringify(value) : h2_line()) + h2_stringify(a, true));

template <int Count, typename Matcher>
struct h2_every1_matches : h2_matches {
   Matcher m;
   explicit h2_every1_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      h2_fail* fails = nullptr;
      for (auto const& ia : a) {
         h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(ia, c.update_n(0).update_negative(false));
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

   H2_MATCHES_CONTAINER2(Count, "Every")
};

template <int Count, typename EK, typename EV, typename M = h2_pair_matches<EK, EV>, typename P = h2_polymorphic_matcher<M>>
struct h2_every2_matches : h2_every1_matches<Count, P> {
   EK k;
   EV v;
   explicit h2_every2_matches(const EK& k_, const EV& v_) : h2_every1_matches<Count, P>(P(M(k_, v_))), k(k_), v(v_) {}
   virtual h2_line expection(h2_mc c) const override
   {
      return c.update_caseless(false).pre() + "Every" + gray("(") + h2_matches_expection(k, c) + gray(", ") + h2_matches_expection(v, c) + gray(")");
   }
};

template <int Count, typename Matcher>
struct h2_has1_matches : h2_matches {
   Matcher m;
   explicit h2_has1_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto __matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_map<typename std::decay<A>::type>::value && !h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // HasKey scenario
      int found = 0, count = 0;
      for (auto const& ia : a) {
         if (++count > Count) break;
         h2_fail* fail = h2_matcher_cast<typename std::decay<decltype(ia.first)>::type>(m).matches(ia.first, c.update_n(0).update_negative(false));
         if (!fail) {
            found = 1;
            break;
         } else
            delete fail;
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto __matches(const A& a, h2_mc c) const -> typename std::enable_if<!h2_is_map<typename std::decay<A>::type>::value || h2_is_polymorphic_matcher_pair_matches<Matcher>::value, h2_fail*>::type
   {  // Normal scenario
      bool found = false;
      for (auto const& ia : a) {
         h2_fail* fail = h2_matcher_cast<typename A::value_type>(m).matches(ia, c.update_n(0).update_negative(false));
         if (!fail) {
            found = true;
            break;
         } else
            delete fail;
      }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type { return __matches(a, c); }

   H2_MATCHES_CONTAINER2(Count, "Has")
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
      for (auto const& ia : a) {
         h2_fail* fails = nullptr;
         h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(ia.first)>::type>(k).matches(ia.first, c.update_n(0).update_negative(false)));
         h2_fail::append_subling(fails, h2_matcher_cast<typename std::decay<decltype(ia.second)>::type>(v).matches(ia.second, c.update_n(0).update_negative(false)));
         if (!fails) {
            found = true;
            break;
         } else
            delete fails;
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
         for (auto const& ia : a) {
            if (j++ == i) {
               ++count;
               fail = v_matchers[i].matches(ia, c.update_n(0).update_negative(false));
               break;
            }
         }
         if (count == 0) fail = h2_fail::new_unexpect(v_matchers[i].expection(c.update_negative(false)), "", "but out of range");
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

   virtual h2_line expection(h2_mc c) const override
   {
      return c.update_caseless(false).pre() + gray("[") + t2e(c.update_negative(false)) + gray("]");
   }

   H2_MATCHES_CONTAINER1(0x7fffffff)
   H2_MATCHES_T2V2E(t_matchers)
};

template <typename Matcher>
struct h2_countof_matches : h2_matches {
   Matcher m;
   explicit h2_countof_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      size_t count = 0;  // std::forward_list no size()
      for (auto it = a.cbegin(); it != a.cend(); ++it) count++;
      H2_MATCHES_STATS(count, true);
   }
   H2_MATCHES_CONTAINER2(0x7fffffff, "CountOf")
};

template <int Count, typename Matcher>
struct h2_maxmin_matches : h2_matches {
   const bool is_max;
   Matcher m;
   explicit h2_maxmin_matches(const bool is_max_, const Matcher& m_) : is_max(is_max_), m(m_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      int count = 0;
      typename A::value_type value;
      for (auto const& ia : a) {  // std::max_element / std::max_element
         if (++count > Count) break;
         if (count == 1) value = ia;
         if (is_max ? (ia > value) : (ia < value)) value = ia;
      }
      H2_MATCHES_STATS(value, count != 0);
   }
   H2_MATCHES_CONTAINER2(Count, is_max ? "MaxOf" : "MinOf")
};

template <int Count, typename Matcher>
struct h2_avg_matches : h2_matches {
   Matcher m;
   explicit h2_avg_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      double avg = 0, count = 0;
      for (auto const& ia : a) {
         if (++count > Count) break;
         avg = (avg * (count - 1) + ia) / count;
      }
      H2_MATCHES_STATS(avg, count != 0);
   }
   H2_MATCHES_CONTAINER2(Count, "AvgOf")
};

template <int Count, typename Matcher>
struct h2_median_matches : h2_matches {
   Matcher m;
   explicit h2_median_matches(const Matcher& m_) : m(m_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<h2_is_iterable<typename std::decay<A>::type>::value, h2_fail*>::type
   {
      std::vector<typename A::value_type> b;
      int count = 0;
      for (auto const& ia : a) {
         if (++count > Count) break;
         for (auto it = b.begin();; ++it)  // std::sort(b.begin(), b.end());  // std::nth_element /// avoid include <algorithm> for compile time
            if (it == b.end() || ia < *it) {
               b.insert(it, ia);
               break;
            }
      }
      double value = 0;
      if (b.size()) value = b.size() % 2 ? b[b.size() / 2] : (b[b.size() / 2] + b[b.size() / 2 + 1]) / 2;
      H2_MATCHES_STATS(value, b.size() != 0);
   }
   H2_MATCHES_CONTAINER2(Count, "MedianOf")
};

template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_pair_matches<EK, EV>> Pair(const MK& mk, const MV& mv) { return h2_polymorphic_matcher<h2_pair_matches<EK, EV>>(h2_pair_matches<EK, EV>(mk, mv)); }

template <int Count = 0x7fffffff, typename Matcher>
inline h2_polymorphic_matcher<h2_has1_matches<Count, typename std::decay<const Matcher&>::type>> Has(const Matcher& m) { return h2_polymorphic_matcher<h2_has1_matches<Count, typename std::decay<const Matcher&>::type>>(h2_has1_matches<Count, typename std::decay<const Matcher&>::type>(m)); }
template <typename MK, typename MV, typename EK = typename h2_decay<MK>::type, typename EV = typename h2_decay<MV>::type>
inline h2_polymorphic_matcher<h2_has2_matches<EK, EV>> Has(const MK& mk, const MV& mv) { return h2_polymorphic_matcher<h2_has2_matches<EK, EV>>(h2_has2_matches<EK, EV>(mk, mv)); }
template <typename MK, typename EK = typename h2_decay<MK>::type, typename EV = h2_polymorphic_matcher<h2_matches_any>>
inline h2_polymorphic_matcher<h2_has2_matches<EK, EV>> HasKey(const MK& mk) { return h2_polymorphic_matcher<h2_has2_matches<EK, EV>>(h2_has2_matches<EK, EV>(mk, _, "HasKey")); }
template <typename MV, typename EV = typename h2_decay<MV>::type, typename EK = h2_polymorphic_matcher<h2_matches_any>>
inline h2_polymorphic_matcher<h2_has2_matches<EK, EV>> HasValue(const MV& mv) { return h2_polymorphic_matcher<h2_has2_matches<EK, EV>>(h2_has2_matches<EK, EV>(_, mv, "HasValue")); }

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers) { return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }

template <int Count = 0x7fffffff, typename Matcher>
inline h2_polymorphic_matcher<h2_every1_matches<Count, typename std::decay<const Matcher&>::type>> Every(const Matcher& expect) { return h2_polymorphic_matcher<h2_every1_matches<Count, typename std::decay<const Matcher&>::type>>(h2_every1_matches<Count, typename std::decay<const Matcher&>::type>(expect)); }
template <int Count = 0x7fffffff, typename MK, typename MV>
inline h2_polymorphic_matcher<h2_every2_matches<Count, typename std::decay<const MK&>::type, typename std::decay<const MV&>::type>> Every(const MK& expect1, const MV& expect2) { return h2_polymorphic_matcher<h2_every2_matches<Count, typename std::decay<const MK&>::type, typename std::decay<const MV&>::type>>(h2_every2_matches<Count, typename std::decay<const MK&>::type, typename std::decay<const MV&>::type>(expect1, expect2)); }

template <typename Matcher>
inline h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matcher&>::type>> CountOf(const Matcher& m) { return h2_polymorphic_matcher<h2_countof_matches<typename std::decay<const Matcher&>::type>>(h2_countof_matches<typename std::decay<const Matcher&>::type>(m)); }
template <int Count = 0x7fffffff, typename Matcher>
inline h2_polymorphic_matcher<h2_maxmin_matches<Count, typename std::decay<const Matcher&>::type>> MaxOf(const Matcher& m) { return h2_polymorphic_matcher<h2_maxmin_matches<Count, typename std::decay<const Matcher&>::type>>(h2_maxmin_matches<Count, typename std::decay<const Matcher&>::type>(true, m)); }
template <int Count = 0x7fffffff, typename Matcher>
inline h2_polymorphic_matcher<h2_maxmin_matches<Count, typename std::decay<const Matcher&>::type>> MinOf(const Matcher& m) { return h2_polymorphic_matcher<h2_maxmin_matches<Count, typename std::decay<const Matcher&>::type>>(h2_maxmin_matches<Count, typename std::decay<const Matcher&>::type>(false, m)); }
template <int Count = 0x7fffffff, typename Matcher>
inline h2_polymorphic_matcher<h2_avg_matches<Count, typename std::decay<const Matcher&>::type>> AvgOf(const Matcher& m) { return h2_polymorphic_matcher<h2_avg_matches<Count, typename std::decay<const Matcher&>::type>>(h2_avg_matches<Count, typename std::decay<const Matcher&>::type>(m)); }
template <int Count = 0x7fffffff, typename Matcher>
inline h2_polymorphic_matcher<h2_median_matches<Count, typename std::decay<const Matcher&>::type>> MedianOf(const Matcher& m) { return h2_polymorphic_matcher<h2_median_matches<Count, typename std::decay<const Matcher&>::type>>(h2_median_matches<Count, typename std::decay<const Matcher&>::type>(m)); }
