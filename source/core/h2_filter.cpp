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

static inline bool __filter(const h2_test* s, const h2_test* c, const char* const includes[], const char* const excludes[], bool tags_filter)
{
   if (tags_filter) {
      if (includes[0])
         if (!(match_tags(includes, s) || c && match_tags(includes, c)))  // priority && > ||
            return true;
      if (excludes[0])
         if (match_tags(excludes, s) || c && match_tags(excludes, c))
            return true;
   } else {
      if (includes[0])
         if (!(match_names(includes, s->name) || c && match_names(includes, c->name) || match_names(includes, c ? c->filine : s->filine)))
            return true;
      if (excludes[0])
         if (match_names(excludes, s->name) || c && match_names(excludes, c->name) || match_names(excludes, c ? c->filine : s->filine))
            return true;
   }
   return false;
}

static inline void h2_filter_suite(h2_suite* s)
{
   s->filtered = __filter(s, nullptr, O.includes, O.excludes, O.tags_filter);
}

static inline void h2_filter_case(h2_suite* s, h2_case* c)
{
   c->filtered = __filter(s, c, O.includes, O.excludes, O.tags_filter);
   if (!c->filtered) s->filtered = false;
}
