struct h2_matches_strcmp : h2_matches {
   const h2_string e;
   explicit h2_matches_strcmp(const h2_string& e_) : e(e_) {}
   h2_fail* matches(const h2_string& a, C c) const;
   virtual h2_line expection(C c) const override;
};

struct h2_matches_string : h2_matches {
   const char* name;
   const h2_string e;
   explicit h2_matches_string(const char* name_, const h2_string& e_) : name(name_), e(e_) {}
   virtual h2_line expection(C c) const override;
};

struct h2_matches_regex : h2_matches_string {
   explicit h2_matches_regex(const h2_string& e_) : h2_matches_string("Re", e_) {}
   h2_fail* matches(const h2_string& a, C c) const;
};

struct h2_matches_wildcard : h2_matches_string {
   explicit h2_matches_wildcard(const h2_string& e_) : h2_matches_string("We", e_) {}
   h2_fail* matches(const h2_string& a, C c) const;
};

struct h2_matches_substr : h2_matches_string {
   explicit h2_matches_substr(const h2_string& substring) : h2_matches_string("Substr", substring) {}
   h2_fail* matches(const h2_string& a, C c) const;
};

struct h2_matches_startswith : h2_matches_string {
   explicit h2_matches_startswith(const h2_string& prefix_string) : h2_matches_string("StartsWith", prefix_string) {}
   h2_fail* matches(const h2_string& a, C c) const;
};

struct h2_matches_endswith : h2_matches_string {
   explicit h2_matches_endswith(const h2_string& suffix_string) : h2_matches_string("EndsWith", suffix_string) {}
   h2_fail* matches(const h2_string& a, C c) const;
};

struct h2_caseless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(const h2_matcher<h2_string>& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, C c) const { return m.matches(a, c.update_caseless(true)); }
   virtual h2_line expection(C c) const override { return m.expection(c.update_caseless(true)); }
};

struct h2_spaceless_matches : h2_matches {
   const h2_matcher<h2_string> m;
   explicit h2_spaceless_matches(const h2_matcher<h2_string>& m_) : m(m_) {}

   template <typename A>
   h2_fail* matches(const A& a, C c) const { return m.matches(a, c.update_spaceless(true)); }
   virtual h2_line expection(C c) const override { return m.expection(c.update_spaceless(true)); }
};

inline auto Se(const h2_string& expect) -> h2_polymorphic_matcher<h2_matches_strcmp> { return h2_polymorphic_matcher<h2_matches_strcmp>(h2_matches_strcmp(expect)); }
inline auto Re(const h2_string& regex_pattern) -> h2_polymorphic_matcher<h2_matches_regex> { return h2_polymorphic_matcher<h2_matches_regex>(h2_matches_regex(regex_pattern)); }
inline auto We(const h2_string& wildcard_pattern) -> h2_polymorphic_matcher<h2_matches_wildcard> { return h2_polymorphic_matcher<h2_matches_wildcard>(h2_matches_wildcard(wildcard_pattern)); }
inline auto Substr(const h2_string& substring) -> h2_polymorphic_matcher<h2_matches_substr> { return h2_polymorphic_matcher<h2_matches_substr>(h2_matches_substr(substring)); }
inline auto StartsWith(const h2_string& prefix_string) -> h2_polymorphic_matcher<h2_matches_startswith> { return h2_polymorphic_matcher<h2_matches_startswith>(h2_matches_startswith(prefix_string)); }
inline auto EndsWith(const h2_string& suffix_string) -> h2_polymorphic_matcher<h2_matches_endswith> { return h2_polymorphic_matcher<h2_matches_endswith>(h2_matches_endswith(suffix_string)); }

template <typename T, typename P = h2_polymorphic_matcher<h2_caseless_matches>>
inline P CaseLess(const T& expect) { return P(h2_caseless_matches(h2_matcher<h2_string>(expect))); }
template <typename T, typename P = h2_polymorphic_matcher<h2_spaceless_matches>>
inline P SpaceLess(const T& expect) { return P(h2_spaceless_matches(h2_matcher<h2_string>(expect))); }
