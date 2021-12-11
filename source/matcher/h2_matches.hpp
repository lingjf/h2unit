struct h2_mc {
   size_t n;
   bool negative, case_insensitive, squash_whitespace, no_compare_operator;
   h2_mc(size_t n_ = 0, bool negative_ = false, bool case_insensitive_ = false, bool squash_whitespace_ = false, bool no_compare_operator_ = false) : n(n_), negative(negative_), case_insensitive(case_insensitive_), squash_whitespace(squash_whitespace_), no_compare_operator(no_compare_operator_) {}

   bool fit(bool result) const { return result == !negative; }
   h2_mc update_n(size_t target = false) const { return {target, negative, case_insensitive, squash_whitespace, no_compare_operator}; }
   h2_mc update_negative(bool target = false) const { return {n, target, case_insensitive, squash_whitespace, no_compare_operator}; }
   h2_mc update_caseless(bool target = false) const { return {n, negative, target, squash_whitespace, no_compare_operator}; }
   h2_mc update_spaceless(bool target = false) const { return {n, negative, case_insensitive, target, no_compare_operator}; }

   h2_line pre(const char* ns = "!") const
   {
      h2_line t;
      if (!no_compare_operator && negative) t.push_back(ns);
      if (case_insensitive) t.push_back("~");
      if (squash_whitespace) t.push_back("*");
      return t;
   }
};

struct h2_matches {
   virtual h2_line expection(h2_mc c) const = 0;
};

struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      if (c.fit(true)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return c.negative ? "!Any" : "Any";
   }
};

struct h2_matches_null : h2_matches {
   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<std::is_pointer<A>::value || h2_is_smart_ptr<A>::value || std::is_integral<A>::value, h2_fail*>::type
   {
      bool result = !a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<!std::is_pointer<A>::value && !h2_is_smart_ptr<A>::value && !std::is_integral<A>::value, h2_fail*>::type
   {
      bool result = std::is_same<std::nullptr_t, typename std::decay<decltype(a)>::type>::value;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return c.negative ? "!NULL" : "NULL";
   }
};

struct h2_matches_bool : h2_matches {
   const bool e;
   explicit h2_matches_bool(bool e_) : e(e_) {}
   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      bool result = e ? !!a : !a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return (e ? c.negative : !c.negative) ? "false" : "true";
   }
};

template <typename T>
inline auto h2_matches_expection(const T& e, h2_mc c) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_line>::type { return e.expection(c); }
template <typename T>
inline auto h2_matches_expection(const T& e, h2_mc c) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_line>::type { return h2_stringify(e, true); }

#define H2_MATCHES_T2V2E(t_matchers)                                                                                                            \
   template <typename T>                                                                                                                        \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, 0>) const {}                                              \
   template <typename T, std::size_t I>                                                                                                         \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers, std::integral_constant<std::size_t, I>) const                                                 \
   {                                                                                                                                            \
      t2v(v_matchers, std::integral_constant<std::size_t, I - 1>());                                                                            \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I - 1>(t_matchers)));                                                                    \
   }                                                                                                                                            \
   template <typename T>                                                                                                                        \
   void t2v(h2_vector<h2_matcher<T>>& v_matchers) const { return t2v(v_matchers, std::integral_constant<std::size_t, sizeof...(Matchers)>()); } \
   h2_line t2e(h2_mc c, std::integral_constant<std::size_t, 0>) const { return {}; }                                                            \
   template <std::size_t I>                                                                                                                     \
   h2_line t2e(h2_mc c, std::integral_constant<std::size_t, I>) const                                                                           \
   {                                                                                                                                            \
      return t2e(c, std::integral_constant<size_t, I - 1>()) + gray(comma_if(1 < I)) + h2_matches_expection(std::get<I - 1>(t_matchers), c);    \
   }                                                                                                                                            \
   h2_line t2e(h2_mc c) const { return t2e(c, std::integral_constant<std::size_t, sizeof...(Matchers)>()); }
