h2_inline h2_fail* h2_matches_strcmp::matches(const h2_string& a, const C& c) const
{
   h2_string _e = e, _a = a;
   if (c.squash_whitespace) _e = e.squash(), _a = a.squash();
   if (c.fit(_a.equals(_e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strcmp(_e, a, c.case_insensitive, expection(c));
}
h2_inline h2_line h2_matches_strcmp::expection(const C& c) const
{
   return c.pre("≠") + h2_stringify(c.squash_whitespace ? e.squash() : e, true);
}

h2_inline h2_line h2_matches_string::expection(const C& c) const
{
   return c.pre() + name + gray("(") + h2_stringify(e, true) + gray(")");
}

h2_inline h2_fail* h2_matches_regex::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::regex_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_wildcard::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(h2_pattern::wildcard_match(e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_substr::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.contains(e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_startswith::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.startswith(e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}

h2_inline h2_fail* h2_matches_endswith::matches(const h2_string& a, const C& c) const
{
   h2_string _a = a;
   if (c.squash_whitespace) _a = a.squash();
   if (c.fit(_a.endswith(e, c.case_insensitive))) return nullptr;
   return h2_fail::new_strfind(e, a, expection(c));
}
