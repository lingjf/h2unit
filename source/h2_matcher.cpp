
h2_inline h2_fail* h2_string_equal_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (r) dont = !dont;
   if (a.equals(e, caseless) == !dont) return nullptr;
   if (a.wildcard_match(e, caseless) == !dont) return nullptr;
   if (a.regex_match(e, caseless) == !dont) return nullptr;

   h2_fail* fail;
   if (dont) {
      fail = (h2_fail*)new h2_fail_unexpect();
      fail->mprintf("should not %sequals", caseless ? "caseless " : "");
   } else {
      fail = (h2_fail*)new h2_fail_strcmp(e, a, caseless);
      fail->mprintf("not %sequals", caseless ? "caseless " : "");
   }
   fail->eprintf("\"%s\"", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   return fail;
}

h2_inline h2_fail* h2_float_equal_matches::matches(const long double a, bool caseless, bool dont) const {
   if (r) dont = !dont;
   // the machine epsilon has to be scaled to the magnitude of the values used
   // and multiplied by the desired precision in ULPs (units in the last place)
   // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
   //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
   bool result = std::fabs(a - e) < 0.00001;
   if (result == !dont) return nullptr;
   return h2_common_unexpect(a, e, dont, "equals");
}

h2_inline h2_fail* h2_memcmp_matches::matches(const void* a, bool caseless, bool dont) const {
   int sz = size ? size : strlen((const char*)e);
   if ((memcmp(e, a, sz) == 0) == !dont) return nullptr;
   h2_fail_memcmp* fail = new h2_fail_memcmp((const unsigned char*)e, (const unsigned char*)a, sz);
   if (dont)
      fail->mprintf("shoud not %d bytes equals", sz);
   else
      fail->mprintf("not %d bytes equals", sz);
   return fail;
}

h2_inline h2_fail* h2_regex_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.regex_match(e, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("/%s/", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not Regex matches");
   else
      fail->mprintf("not Regex matches");

   return fail;
}

h2_inline h2_fail* h2_wildcard_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.wildcard_match(e, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("/%s/", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not Wildcard matches");
   else
      fail->mprintf("not Wildcard matches");

   return fail;
}

h2_inline h2_fail* h2_strcmp_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.equals(e, caseless) == !dont) return nullptr;
   h2_fail* fail;
   if (dont) {
      fail = (h2_fail*)new h2_fail_unexpect();
      fail->mprintf("should not %sequals", caseless ? "caseless " : "");
   } else {
      fail = (h2_fail*)new h2_fail_strcmp(e, a, caseless);
      fail->mprintf("not %sequals", caseless ? "caseless " : "");
   }
   fail->eprintf("\"%s\"", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   return fail;
}

h2_inline h2_fail* h2_contains_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.contains(substring, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("\"%s\"", substring.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not %shas substr", caseless ? "caseless " : "");
   else
      fail->mprintf("not %shas substr", caseless ? "caseless " : "");

   return fail;
}

h2_inline h2_fail* h2_startswith_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("\"%s\"", prefix_string.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not %sstarts with", caseless ? "caseless " : "");
   else
      fail->mprintf("not %sstarts with", caseless ? "caseless " : "");

   return fail;
}

h2_inline h2_fail* h2_endswith_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("\"%s\"", suffix_string.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not %sends with", caseless ? "caseless " : "");
   else
      fail->mprintf("not %sends with", caseless ? "caseless " : "");

   return fail;
}

h2_inline h2_fail* h2_json_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if ((h2_json::match(e, a)) == !dont) return nullptr;
   h2_fail_json* fail = new h2_fail_json(e, a);
   if (dont)
      fail->mprintf("should not equals");
   else
      fail->mprintf("not equals");

   return fail;
}
