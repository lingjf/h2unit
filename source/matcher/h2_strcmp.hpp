
struct h2_matches_regex {
   const h2_string e;
   explicit h2_matches_regex(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return CD("Re(" + h2_stringify(e) + ")", caseless, dont);
   }
};

struct h2_matches_wildcard {
   const h2_string e;
   explicit h2_matches_wildcard(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return CD("We(" + h2_stringify(e) + ")", caseless, dont);
   }
};

struct h2_matches_strcmp {
   const h2_string e;
   explicit h2_matches_strcmp(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return CD("\"" + h2_stringify(e) + "\"", caseless, dont);
   }
};

struct h2_matches_substr {
   const h2_string substring;
   explicit h2_matches_substr(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return CD("Substr(\"" + h2_stringify(substring) + "\")", caseless, dont);
   }
};

struct h2_matches_startswith {
   const h2_string prefix_string;
   explicit h2_matches_startswith(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return CD("StartsWith(\"" + h2_stringify(prefix_string) + "\")", caseless, dont);
   }
};

struct h2_matches_endswith {
   const h2_string suffix_string;
   explicit h2_matches_endswith(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return CD("EndsWith(\"" + h2_stringify(suffix_string) + "\")", caseless, dont);
   }
};

struct h2_matches_json {
   const h2_string e;
   explicit h2_matches_json(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const
   {
      return CD("Je(\"" + h2_stringify(e) + "\")", caseless, dont);
   }
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return m.matches(a, true, dont); }
   template <typename A>
   h2_string expects(h2_type<A>, bool caseless = false, bool dont = false) const { return m.expects(true, dont); }
};

inline h2_polymorphic_matcher<h2_matches_regex> Re(const h2_string& regex_pattern) { return h2_polymorphic_matcher<h2_matches_regex>(h2_matches_regex(regex_pattern)); }
inline h2_polymorphic_matcher<h2_matches_wildcard> We(const h2_string& wildcard_pattern) { return h2_polymorphic_matcher<h2_matches_wildcard>(h2_matches_wildcard(wildcard_pattern)); }
inline h2_polymorphic_matcher<h2_matches_strcmp> Se(const h2_string& expect) { return h2_polymorphic_matcher<h2_matches_strcmp>(h2_matches_strcmp(expect)); }
inline h2_polymorphic_matcher<h2_matches_substr> Substr(const h2_string& substring) { return h2_polymorphic_matcher<h2_matches_substr>(h2_matches_substr(substring)); }
inline h2_polymorphic_matcher<h2_matches_startswith> StartsWith(const h2_string& prefix_string) { return h2_polymorphic_matcher<h2_matches_startswith>(h2_matches_startswith(prefix_string)); }
inline h2_polymorphic_matcher<h2_matches_endswith> EndsWith(const h2_string& suffix_string) { return h2_polymorphic_matcher<h2_matches_endswith>(h2_matches_endswith(suffix_string)); }
inline h2_polymorphic_matcher<h2_matches_json> Je(const h2_string& expect) { return h2_polymorphic_matcher<h2_matches_json>(h2_matches_json(expect)); }

template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(M m) { return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(h2_matcher<h2_string>(m))); }
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> operator~(M m) { return CaseLess(m); }
