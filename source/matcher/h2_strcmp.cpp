
h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expects(h2_type<h2_string>(), caseless, dont));
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expects(h2_type<h2_string>(), caseless, dont));
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.equals(e, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(e, a, expects(h2_type<h2_string>(), caseless, dont));
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.contains(substring, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(substring, a, expects(h2_type<h2_string>(), caseless, dont));
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expects(h2_type<h2_string>(), caseless, dont));
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, bool caseless, bool dont) const
{
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expects(h2_type<h2_string>(), caseless, dont));
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, bool caseless, bool dont) const
{
   if ((h2_json::match(e, a)) == !dont) return nullptr;
   return h2_fail::new_json(e, a, expects(h2_type<h2_string>(), caseless, dont));
}
