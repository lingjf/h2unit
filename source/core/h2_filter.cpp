static inline bool match_names(const char* const patterns[], const char* subject)
{
   for (int i = 0; patterns[i]; ++i) {
      if (strcasestr(subject, patterns[i])) return true;
      if (strcspn(patterns[i], "?*[]") < strlen(patterns[i]) && h2_pattern::wildcard_match(patterns[i], subject, true)) return true;
   }
   return false;
}

static inline bool match_tags(const char* const patterns[], const h2_test* test)
{
   for (int i = 0; patterns[i]; ++i)
      if (test->tagged(patterns[i])) return true;
   return false;
}

static inline bool __filter(const h2_test* s, const h2_test* c, const char* const includes[], const char* const excludes[], const char* const file_includes[], const char* const file_excludes[], const char* const suite_includes[], const char* const suite_excludes[], const char* const case_includes[], const char* const case_excludes[], const char* const tags_includes[], const char* const tags_excludes[])
{
   if (includes[0] || file_includes[0] || suite_includes[0] || case_includes[0] || tags_includes[0]) {
      bool m = false;
      if (includes[0]) m = m || (match_names(includes, s->filine) || c && match_names(includes, c->filine) || match_names(includes, s->name) || c && match_names(includes, c->name) || match_tags(includes, s) || c && match_tags(includes, c));
      if (file_includes[0]) m = m || (match_names(file_includes, s->filine) || c && match_names(file_includes, c->filine));
      if (suite_includes[0]) m = m || match_names(suite_includes, s->name);
      if (case_includes[0]) m = m || c && match_names(case_includes, c->name);  // priority && > ||
      if (tags_includes[0]) m = m || (match_tags(tags_includes, s) || c && match_tags(tags_includes, c));
      if (!m) return true;
   }
   if (excludes[0])
      if ((match_names(excludes, s->filine) || c && match_names(excludes, c->filine)) ||
          (match_names(excludes, s->name) || c && match_names(excludes, c->name)) ||
          (match_tags(excludes, s) || c && match_tags(excludes, c))) return true;
   if (file_excludes[0] && (match_names(file_excludes, s->filine) || c && match_names(file_excludes, c->filine))) return true;
   if (suite_excludes[0] && match_names(suite_excludes, s->name)) return true;
   if (case_excludes[0] && c && match_names(case_excludes, c->name)) return true;
   if (tags_excludes[0] && (match_tags(tags_excludes, s) || c && match_tags(tags_excludes, c))) return true;
   return false;
}

static inline void h2_filter_suite(h2_suite* s)
{
   s->filtered = __filter(s, nullptr, O.includes, O.excludes, O.file_includes, O.file_excludes, O.suite_includes, O.suite_excludes, O.case_includes, O.case_excludes, O.tags_includes, O.tags_excludes);
}

static inline void h2_filter_case(h2_suite* s, h2_case* c)
{
   c->filtered = __filter(s, c, O.includes, O.excludes, O.file_includes, O.file_excludes, O.suite_includes, O.suite_excludes, O.case_includes, O.case_excludes, O.tags_includes, O.tags_excludes);
   if (!c->filtered) s->filtered = false;
}
