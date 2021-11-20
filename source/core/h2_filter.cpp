static inline bool match_names(const std::vector<const char*>& patterns, const char* subject)
{
   for (auto pattern : patterns) {
      if (strcasestr(subject, pattern)) return true;
      if (strcspn(pattern, "?*[]") < strlen(pattern) && h2_pattern::wildcard_match(pattern, subject, true)) return true;
   }
   return false;
}

static inline bool match_tags(const std::vector<const char*>& patterns, const h2_test* test)
{
   for (auto pattern : patterns)
      if (test->tagged(pattern)) return true;
   return false;
}

static inline bool __filter(const h2_test* s, const h2_test* c, const std::vector<const char*>& includes, const std::vector<const char*>& excludes, bool tags_filter)
{
   if (tags_filter) {
      if (!includes.empty())
         if (!(match_tags(includes, s) || c && match_tags(includes, c))) // priority && > ||
            return true;
      if (!excludes.empty())
         if (match_tags(excludes, s) || c && match_tags(excludes, c))
            return true;
   } else {
      if (!includes.empty())
         if (!(match_names(includes, s->name) || c && match_names(includes, c->name) || match_names(includes, c ? c->filine : s->filine)))
            return true;
      if (!excludes.empty())
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
