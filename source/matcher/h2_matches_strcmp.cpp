h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::regex_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_regex::expection(h2_mc c) const
{
   return c.pre() + "Re" + gray("(") + h2_stringify(e, true) + gray(")");
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::wildcard_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
h2_inline h2_line h2_matches_wildcard::expection(h2_mc c) const
{
   return c.pre() + "We" + gray("(") + h2_stringify(e, true) + gray(")");
}

h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, h2_mc c) const
{
   h2_string _e = e, _a = a;
   if (c.squash_whitespace) _e = e.squash(), _a = a.squash();
   if (c.fit(_a.equals(_e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(_e, a, expection(c));
}
h2_inline h2_line h2_matches_strcmp::expection(h2_mc c) const
{
   return c.pre("≠") + h2_stringify(c.squash_whitespace ? e.squash() : e, true);
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.contains(substring, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(substring, a, expection(c));
}
h2_inline h2_line h2_matches_substr::expection(h2_mc c) const
{
   return c.pre() + "Substr" + gray("(") + h2_stringify(substring, true) + gray(")");
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.startswith(prefix_string, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(prefix_string, a, expection(c));
}
h2_inline h2_line h2_matches_startswith::expection(h2_mc c) const
{
   return c.pre() + "StartsWith" + gray("(") + h2_stringify(prefix_string, true) + gray(")");
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.endswith(suffix_string, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(suffix_string, a, expection(c));
}
h2_inline h2_line h2_matches_endswith::expection(h2_mc c) const
{
   return c.pre() + "EndsWith" + gray("(") + h2_stringify(suffix_string, true) + gray(")");
}

h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, h2_mc c) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, c.case_insensitive);
   int ret = h2_json::match(e, _a, c.case_insensitive);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(c), c.case_insensitive, "illformed json");
   if (c.fit(ret == 0)) return nullptr;
   return h2_fail::new_json(e, _a, expection(c), c.case_insensitive);
}
h2_inline h2_line h2_matches_json::expection(h2_mc c) const
{
   return c.pre("≠") + h2_stringify(e);
}
