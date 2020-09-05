
h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expection(caseless, dont));
}
h2_inline h2_line h2_matches_regex::expection(bool caseless, bool dont) const
{
   return CD("Re" + gray("(") + h2_stringify(e) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expection(caseless, dont));
}
h2_inline h2_line h2_matches_wildcard::expection(bool caseless, bool dont) const
{
   return CD("We" + gray("(") + h2_stringify(e) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.equals(e, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expection(caseless, dont));
}
h2_inline h2_line h2_matches_strcmp::expection(bool caseless, bool dont) const
{
   return CD(h2_representify(e), caseless, dont, "≠");
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.contains(substring, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(substring, a, expection(caseless, dont));
}
h2_inline h2_line h2_matches_substr::expection(bool caseless, bool dont) const
{
   return CD("Substr" + gray("(") + h2_representify(substring) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expection(caseless, dont));
}
h2_inline h2_line h2_matches_startswith::expection(bool caseless, bool dont) const
{
   return CD("StartsWith" + gray("(") + h2_representify(prefix_string) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, int n, bool caseless, bool dont) const
{
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expection(caseless, dont));
}
h2_inline h2_line h2_matches_endswith::expection(bool caseless, bool dont) const
{
   return CD("EndsWith" + gray("(") + h2_representify(suffix_string) + gray(")"), caseless, dont);
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, int, bool caseless, bool dont) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, caseless);
   int ret = h2_json::match(e, _a, caseless);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(caseless, dont), caseless, "illformed json");
   if ((ret == 0) == !dont) return nullptr;
   return h2_fail::new_json(e, _a, expection(caseless, dont), caseless, DS(dont));
}
h2_inline h2_line h2_matches_json::expection(bool caseless, bool dont) const
{
   return CD(h2_stringify(e), caseless, dont, "≠");
}
