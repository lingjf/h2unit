
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

#define H2_MATCHER_T2V2E(t_matchers)                                                                                                                           \
                                                                                                                                                               \
   template <typename T>                                                                                                                                       \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                                             \
   template <typename T, std::size_t I>                                                                                                                        \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                                \
   {                                                                                                                                                           \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                                           \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                                   \
   }                                                                                                                                                           \
   template <typename T>                                                                                                                                       \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }                \
                                                                                                                                                               \
   h2_string t2e(bool caseless, bool dont, std::integral_constant<std::size_t, 0>) const { return ""; }                                                        \
   template <std::size_t I>                                                                                                                                    \
   h2_string t2e(bool caseless, bool dont, std::integral_constant<std::size_t, I>) const                                                                       \
   {                                                                                                                                                           \
      return t2e(caseless, dont, std::integral_constant<size_t, I - 1>()) + comma_if(1 < I) + h2_matches_expects(std::get<I - 1>(t_matchers), caseless, dont); \
   }                                                                                                                                                           \
   h2_string t2e(bool caseless, bool dont) const { return t2e(caseless, dont, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
