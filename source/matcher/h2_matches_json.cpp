h2_inline h2_fail* h2_matches_json::matches(const h2_string& a, C c) const
{
   h2_string _a = a;
   if (selector.size()) _a = h2_json::select(a, selector, c.case_insensitive);
   int ret = h2_json::match(e, _a, c.case_insensitive);
   if (ret < 0) return h2_fail::new_json(e, _a, expection(c), c.case_insensitive, "illformed json");
   if (c.fit(ret == 0)) return nullptr;
   return h2_fail::new_json(e, _a, expection(c), c.case_insensitive);
}
h2_inline h2_line h2_matches_json::expection(C c) const
{
   return c.pre("≠") + h2_stringify(e);
}
