
struct h2_matches {
   virtual h2_line expection(bool caseless, bool dont) const = 0;
};

static inline h2_string DS(bool match)
{
   return match ? "should not match" : "";
}

static inline h2_line CD(const h2_line& s, bool caseless, bool dont, const char* dsym = "!")
{
   h2_line line;
   if (dont) line.push_back(dsym);
   if (caseless) line.push_back("~");
   line += s;
   return line;
}

template <typename T>
inline auto h2_matches_expection(const T& e, bool caseless, bool dont) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_line>::type
{
   return e.expection(caseless, dont);
}
template <typename T>
inline auto h2_matches_expection(const T& e, bool caseless, bool dont) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_line>::type
{
   return CD(h2_representify(e), caseless, dont);
}

#define H2_MATCHES_T2V2E(t_matchers)                                                                                                                                              \
                                                                                                                                                                                  \
   template <typename T>                                                                                                                                                          \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                                                                \
   template <typename T, std::size_t I>                                                                                                                                           \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                                                   \
   {                                                                                                                                                                              \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                                                              \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                                                      \
   }                                                                                                                                                                              \
   template <typename T>                                                                                                                                                          \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }                                   \
                                                                                                                                                                                  \
   h2_line t2e(bool caseless, bool dont, std::integral_constant<std::size_t, 0>) const { return {}; }                                                                             \
   template <std::size_t I>                                                                                                                                                       \
   h2_line t2e(bool caseless, bool dont, std::integral_constant<std::size_t, I>) const                                                                                            \
   {                                                                                                                                                                              \
      return t2e(caseless, dont, std::integral_constant<size_t, I - 1>()) + (1 < I ? gray(", ") : h2_line()) + h2_matches_expection(std::get<I - 1>(t_matchers), caseless, dont); \
   }                                                                                                                                                                              \
   h2_line t2e(bool caseless, bool dont) const { return t2e(caseless, dont, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
