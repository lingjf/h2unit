
struct h2_matches {
   virtual h2_string expects(bool caseless, bool dont) const = 0;
};

static inline h2_string DS(bool match)
{
   return match ? "should not match" : "";
}

static inline h2_string CD(const h2_string& s, bool caseless = false, bool dont = false)
{
   h2_string z = s;
   if (dont) z = "!" + z;
   if (caseless) z = "~" + z;
   return z;
}

template <typename T>
inline auto h2_matches_expects(const T& a, bool caseless, bool dont) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_string>::type
{
   return a.expects(caseless, dont);
}
template <typename T>
inline auto h2_matches_expects(const T& a, bool caseless, bool dont) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_string>::type
{
   return CD(h2_stringify(a), caseless, dont);
}

#define H2_MATCHER_T2V2E(t_matchers)                                                                                                                                       \
                                                                                                                                                                           \
   template <typename T>                                                                                                                                                   \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>) const                                                          \
   {                                                                                                                                                                       \
   }                                                                                                                                                                       \
                                                                                                                                                                           \
   template <typename T>                                                                                                                                                   \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, typename std::conditional<sizeof...(Matchers) != 0, std::integral_constant<std::size_t, 0>, std::nullptr_t>::type) const \
   {                                                                                                                                                                       \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<0>(t_matchers)));                                                                                                   \
      t2v(v_matchers, std::integral_constant<std::size_t, 1>());                                                                                                           \
   }                                                                                                                                                                       \
                                                                                                                                                                           \
   template <typename T, std::size_t I>                                                                                                                                    \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                                            \
   {                                                                                                                                                                       \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                                                                                   \
      t2v(v_matchers, std::integral_constant<std::size_t, I + 1>());                                                                                                       \
   }                                                                                                                                                                       \
                                                                                                                                                                           \
   h2_string t2e(bool caseless, bool dont, std::integral_constant<std::size_t, sizeof...(Matchers)>) const                                                                 \
   {                                                                                                                                                                       \
      return "";                                                                                                                                                           \
   }                                                                                                                                                                       \
                                                                                                                                                                           \
   h2_string t2e(bool caseless, bool dont, typename std::conditional<sizeof...(Matchers) != 0, std::integral_constant<std::size_t, 0>, std::nullptr_t>::type) const        \
   {                                                                                                                                                                       \
      return h2_matches_expects(std::get<0>(t_matchers), caseless, dont) + t2e(caseless, dont, std::integral_constant<size_t, 1>());                                       \
   }                                                                                                                                                                       \
                                                                                                                                                                           \
   template <std::size_t I>                                                                                                                                                \
   h2_string t2e(bool caseless, bool dont, std::integral_constant<std::size_t, I>) const                                                                                   \
   {                                                                                                                                                                       \
      return Comma[!!I] + h2_matches_expects(std::get<I>(t_matchers), caseless, dont) + t2e(caseless, dont, std::integral_constant<size_t, I + 1>());                      \
   }
