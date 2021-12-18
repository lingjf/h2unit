struct h2_matches_json : h2_matches {
   const h2_string e, selector;
   explicit h2_matches_json(const h2_string& e_, const h2_string& selector_) : e(e_), selector(selector_) {}
   h2_fail* matches(const h2_string& a, C c) const;
   virtual h2_line expection(C c) const override;
};

inline auto Je(const h2_string& expect, const h2_string& selector = "") -> h2_polymorphic_matcher<h2_matches_json> { return h2_polymorphic_matcher<h2_matches_json>(h2_matches_json(expect, selector)); }
