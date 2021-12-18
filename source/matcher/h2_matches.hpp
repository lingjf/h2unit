struct C {
   int array_size, range_start, range_end, times;
   bool no_compare_operator, negative, case_insensitive, squash_whitespace;
   C(int array_size_ = -1, bool no_compare_operator_ = false, bool negative_ = false, bool case_insensitive_ = false, bool squash_whitespace_ = false, int range_start_ = 0, int range_end_ = 5413722, int times_ = 1) : array_size(array_size_), range_start(range_start_), range_end(range_end_), times(times_), no_compare_operator(no_compare_operator_), negative(negative_), case_insensitive(case_insensitive_), squash_whitespace(squash_whitespace_) {}

   bool in(const int i) const { return range_start <= i && i < range_end; }
   bool fit(bool result) const { return result == !negative; }
   C clear_size() const { return {-1, no_compare_operator, negative, case_insensitive, squash_whitespace, 0, 5413722, times}; }
   C update_negative(bool target = false) const { return {array_size, no_compare_operator, target, case_insensitive, squash_whitespace, range_start, range_end, times}; }
   C update_caseless(bool target = false) const { return {array_size, no_compare_operator, negative, target, squash_whitespace, range_start, range_end, times}; }
   C update_spaceless(bool target = false) const { return {array_size, no_compare_operator, negative, case_insensitive, target, range_start, range_end, times}; }

   h2_line pre(const char* ns = "!") const
   {
      h2_line t;
      if (!no_compare_operator && negative) t.push_back(ns);
      if (case_insensitive) t.push_back("~");
      if (squash_whitespace) t.push_back("*");
      return t;
   }
   h2_line post() const { return times == 1 ? h2_line() : " * " + h2_stringify(times); }
};

struct h2_matches {
   virtual h2_line expection(C c) const = 0;
};

struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, C c) const
   {
      if (c.fit(true)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(C c) const override
   {
      return c.negative ? "!Any" : "Any";
   }
};

struct h2_matches_null : h2_matches {
   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<std::is_pointer<A>::value || h2_is_smart_ptr<A>::value || std::is_integral<A>::value, h2_fail*>::type
   {
      bool result = !a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   template <typename A>
   auto matches(const A& a, C c) const -> typename std::enable_if<!std::is_pointer<A>::value && !h2_is_smart_ptr<A>::value && !std::is_integral<A>::value, h2_fail*>::type
   {
      bool result = std::is_same<std::nullptr_t, typename std::decay<decltype(a)>::type>::value;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(C c) const override
   {
      return c.negative ? "!NULL" : "NULL";
   }
};

struct h2_matches_bool : h2_matches {
   const bool e;
   explicit h2_matches_bool(bool e_) : e(e_) {}
   template <typename A>
   h2_fail* matches(const A& a, C c) const
   {
      bool result = e ? !!a : !a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(C c) const override
   {
      return (e ? c.negative : !c.negative) ? "false" : "true";
   }
};

template <typename T>
inline auto h2_matches_expection(const T& e, C c) -> typename std::enable_if<std::is_base_of<h2_matches, T>::value, h2_line>::type { return e.expection(c); }
template <typename T>
inline auto h2_matches_expection(const T& e, C c) -> typename std::enable_if<!std::is_base_of<h2_matches, T>::value, h2_line>::type { return h2_stringify(e, true); }
