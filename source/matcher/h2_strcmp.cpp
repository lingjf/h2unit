h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, size_t n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(h2_pattern::regex_match(e.c_str(), _a.c_str(), c.caseless))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_regex::expection(h2_mc c) const
{
   return CD("Re" + gray("(") + h2_stringify(e) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, size_t n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(h2_pattern::wildcard_match(e.c_str(), _a.c_str(), c.caseless))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_wildcard::expection(h2_mc c) const
{
   return CD("We" + gray("(") + h2_stringify(e) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, size_t n, h2_mc c) const
{
   h2_string _e = e, _a = a;
   if (c.spaceless) _e = e.squash(), _a = a.squash();
   if (c.fit(_a.equals(_e, c.caseless))) return nullptr;
   return h2_fail::new_strfind(_e, a, expection(c));
}
h2_inline h2_line h2_matches_strcmp::expection(h2_mc c) const
{
   return CD(h2_representify(c.spaceless ? e.squash() : e), c, "≠");
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, size_t n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(_a.contains(substring, c.caseless))) return nullptr;
   return h2_fail::new_strfind(substring, a, expection(c));
}
h2_inline h2_line h2_matches_substr::expection(h2_mc c) const
{
   return CD("Substr" + gray("(") + h2_representify(substring) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, size_t n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(_a.startswith(prefix_string, c.caseless))) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expection(c));
}
h2_inline h2_line h2_matches_startswith::expection(h2_mc c) const
{
   return CD("StartsWith" + gray("(") + h2_representify(prefix_string) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, size_t n, h2_mc c) const
{
   h2_string _a = a;
   if (c.spaceless) _a = a.squash();
   if (c.fit(_a.endswith(suffix_string, c.caseless))) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expection(c));
}
h2_inline h2_line h2_matches_endswith::expection(h2_mc c) const
{
   return CD("EndsWith" + gray("(") + h2_representify(suffix_string) + gray(")"), c);
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, size_t, h2_mc c) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, c.caseless);
   int ret = h2_json::match(e, _a, c.caseless);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(c), c.caseless, "illformed json");
   if (c.fit(ret == 0)) return nullptr;
   return h2_fail::new_json(e, _a, expection(c), c.caseless, DS(c.dont));
}
h2_inline h2_line h2_matches_json::expection(h2_mc c) const
{
   return CD(h2_stringify(e), c, "≠");
}
