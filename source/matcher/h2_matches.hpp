struct h2_mc {
   bool caseless = false, dont = false, ncop = false, spaceless = false;
   h2_mc(bool caseless_ = false, bool dont_ = false, bool ncop_ = false, bool spaceless_ = false) : caseless(caseless_), dont(dont_), ncop(ncop_), spaceless(spaceless_) {}

   bool fit(bool result) const { return result == !dont; }
   h2_mc update_caseless(bool caseless_ = false) const { return {caseless_, dont, ncop, spaceless}; }
   h2_mc update_dont(bool dont_ = false) const { return {caseless, dont_, ncop, spaceless}; }
   h2_mc update_spaceless(bool spaceless_ = false) const { return {caseless, dont, ncop, spaceless_}; }
};
struct h2_matches {
   virtual h2_line expection(h2_mc c) const = 0;
};

static inline h2_string DS(bool match) { return match ? "should not match" : ""; }

static inline h2_line CD(const h2_line& s, h2_mc c, const char* dsym = "!")
{
   h2_line t;
   if (!c.ncop && c.dont) t.push_back(dsym);
   if (c.caseless) t.push_back("~");
   if (c.spaceless) t.push_back("*");
   t += s;
   return t;
}

template <typename T>
inline auto h2_matches_expection(const T& e, h2_mc c) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_line>::type { return e.expection(c); }
template <typename T>
inline auto h2_matches_expection(const T& e, h2_mc c) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_line>::type { return CD(h2_representify(e), c); }

#define H2_MATCHES_T2V2E(t_matchers)                                                                                                                    \
   template <typename T>                                                                                                                                \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                                      \
   template <typename T, std::size_t I>                                                                                                                 \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                         \
   {                                                                                                                                                    \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                                    \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                            \
   }                                                                                                                                                    \
   template <typename T>                                                                                                                                \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }         \
   h2_line t2e(h2_mc c, std::integral_constant<std::size_t, 0>) const { return {}; }                                                                    \
   template <std::size_t I>                                                                                                                             \
   h2_line t2e(h2_mc c, std::integral_constant<std::size_t, I>) const                                                                                   \
   {                                                                                                                                                    \
      return t2e(c, std::integral_constant<size_t, I - 1>()) + (1 < I ? gray(", ") : h2_line()) + h2_matches_expection(std::get<I - 1>(t_matchers), c); \
   }                                                                                                                                                    \
   h2_line t2e(h2_mc c) const { return t2e(c, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
