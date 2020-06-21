
#define __H2_LISTOF_COMMON(type)                                                       \
   h2_fail* fails = nullptr;                                                           \
   auto v_matchers = t2v<type, 0>();                                                   \
   for (int i = 0; i < v_matchers.size(); ++i) {                                       \
      h2_fail* fail = nullptr;                                                         \
      int j = 0, c = 0;                                                                \
      for (auto& k : a) {                                                              \
         if (j++ == i) {                                                               \
            ++c;                                                                       \
            fail = v_matchers[i].matches(k, caseless, false);                          \
            break;                                                                     \
         }                                                                             \
      }                                                                                \
      if (c == 0) {                                                                    \
         type t1;                                                                      \
         h2_string t2 = v_matchers[i].expects(t1, caseless, false);                    \
         fail = h2_fail::new_unexpect("", "[missing]", t2);                            \
      }                                                                                \
      if (fail) fail->no = h2_stringify(i);                                            \
      h2_fail::append_subling(fails, fail);                                            \
   }                                                                                   \
   if (!fails == !dont) {                                                              \
      if (fails) delete fails;                                                         \
      return nullptr;                                                                  \
   }                                                                                   \
   h2_fail* fail = h2_fail::new_unexpect("", "", expects(a, caseless, dont), "fails"); \
   h2_fail::append_child(fail, fails);                                                 \
   return fail

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_listof_matches<Matchers...>::matches(A a, bool caseless, bool dont) const
{ /* c/c++ array and std::array */
   h2_fail* fails = nullptr;
   auto v_matchers = t2v<decltype(a[0]), 0>();
   for (int i = 0; i < v_matchers.size(); ++i) {
      h2_fail* fail = v_matchers[i].matches(a[i], caseless, false);
      if (fail) fail->no = h2_stringify(i);
      h2_fail::append_subling(fails, fail);
   }
   if (!fails == !dont) {
      if (fails) delete fails;
      return nullptr;
   }
   h2_fail* fail = h2_fail::new_unexpect("", "", expects(a, caseless, dont), "fails");
   h2_fail::append_child(fail, fails);
   return fail;
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_listof_matches<Matchers...>::matches(const std::vector<A> a, bool caseless, bool dont) const
{
   __H2_LISTOF_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_listof_matches<Matchers...>::matches(const std::deque<A> a, bool caseless, bool dont) const
{
   __H2_LISTOF_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_listof_matches<Matchers...>::matches(const std::list<A> a, bool caseless, bool dont) const
{
   __H2_LISTOF_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_listof_matches<Matchers...>::matches(const std::forward_list<A> a, bool caseless, bool dont) const
{
   __H2_LISTOF_COMMON(A);
}

#define __H2_HAS_COMMON(type)                                                                \
   h2_fail* fails = nullptr;                                                                 \
   for (int i = 0; i < v_matchers.size(); ++i) {                                             \
      bool found = false;                                                                    \
      for (auto& j : a) {                                                                    \
         h2_fail* fail = v_matchers[i].matches(j, caseless, false);                          \
         if (!fail) {                                                                        \
            found = true;                                                                    \
            break;                                                                           \
         }                                                                                   \
      }                                                                                      \
      if (!found) {                                                                          \
         type t1;                                                                            \
         h2_string t2 = v_matchers[i].expects(t1, caseless, false);                          \
         h2_fail* fail = h2_fail::new_normal(nullptr, 0, nullptr, "Not has %s", t2.c_str()); \
         if (fail) fail->no = h2_stringify(i);                                               \
         h2_fail::append_subling(fails, fail);                                               \
      }                                                                                      \
   }                                                                                         \
   if (!fails == !dont) {                                                                    \
      if (fails) delete fails;                                                               \
      return nullptr;                                                                        \
   }                                                                                         \
   h2_fail* fail = h2_fail::new_unexpect("", "", expects(a, caseless, dont), "fails");       \
   h2_fail::append_child(fail, fails);                                                       \
   return fail

template <typename... Matchers>
template <typename A, size_t N>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::array<A, N> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}
template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::vector<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::deque<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::list<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::forward_list<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::set<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::multiset<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::unordered_set<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename A>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::unordered_multiset<A> a, bool caseless, bool dont) const
{
   auto v_matchers = t2v<A, 0>();
   __H2_HAS_COMMON(A);
}

template <typename... Matchers>
template <typename AK, typename AV>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::map<AK, AV> a, bool caseless, bool dont) const
{
   using type = std::pair<AK, AV>;
   auto v_matchers = t4v<AK, AV, 0>();
   __H2_HAS_COMMON(type);
}

template <typename... Matchers>
template <typename AK, typename AV>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::multimap<AK, AV> a, bool caseless, bool dont) const
{
   using type = std::pair<AK, AV>;
   auto v_matchers = t4v<AK, AV, 0>();
   __H2_HAS_COMMON(type);
}

template <typename... Matchers>
template <typename AK, typename AV>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::unordered_map<AK, AV> a, bool caseless, bool dont) const
{
   using type = std::pair<AK, AV>;
   auto v_matchers = t4v<AK, AV, 0>();
   __H2_HAS_COMMON(type);
}

template <typename... Matchers>
template <typename AK, typename AV>
inline h2_fail* h2_has_matches<Matchers...>::matches(const std::unordered_multimap<AK, AV> a, bool caseless, bool dont) const
{
   using type = std::pair<AK, AV>;
   auto v_matchers = t4v<AK, AV, 0>();
   __H2_HAS_COMMON(type);
}