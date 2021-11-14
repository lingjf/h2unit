static inline bool match_names(const std::vector<const char*>& patterns, const char* subject)
{
   for (auto pattern : patterns) {
      if (strcasestr(subject, pattern)) return true;
      if (strcspn(pattern, "?*[]") < strlen(pattern) && h2_pattern::wildcard_match(pattern, subject, true)) return true;
   }
   return false;
}

static inline bool match_tags(const std::vector<const char*>& patterns, h2_describe& describe)
{
   for (auto pattern : patterns)
      if (describe.has_tag(pattern)) return true;
   return false;
}

static inline bool h2_filter_case(h2_suite* s, h2_case* c)
{
   if (O.tags_filter) {
      if (!O.includes.empty())
         if (!match_tags(O.includes, s->describe) && !match_tags(O.includes, c->describe))
            return true;
      if (!O.excludes.empty())
         if (match_tags(O.excludes, s->describe) || match_tags(O.excludes, c->describe))
            return true;
   } else {
      if (!O.includes.empty())
         if (!match_names(O.includes, s->describe.name) && !match_names(O.includes, c->describe.name) && !match_names(O.includes, c->filine))
            return true;
      if (!O.excludes.empty())
         if (match_names(O.excludes, s->describe.name) || match_names(O.excludes, c->describe.name) || match_names(O.excludes, c->filine))
            return true;
   }
   return false;
}

static inline bool h2_filter_suite(h2_suite* s)
{
   if (O.tags_filter) {
      if (!O.includes.empty())
         if (!match_tags(O.includes, s->describe))
            return true;
      if (!O.excludes.empty())
         if (match_tags(O.includes, s->describe))
            return true;
   } else {
      if (!O.includes.empty())
         if (!match_names(O.includes, s->describe.name) && !match_names(O.includes, s->filine))
            return true;
      if (!O.excludes.empty())
         if (match_names(O.excludes, s->describe.name) || match_names(O.excludes, s->filine))
            return true;
   }
   return false;
}

static inline void h2_filter(h2_suite* s, h2_case* c = nullptr)
{
   if (c) {
      c->filtered = h2_filter_case(s, c);
      if (!c->filtered) s->filtered = false;
   } else {
      s->filtered = h2_filter_suite(s);
   }
}
