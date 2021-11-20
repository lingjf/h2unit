
static inline void __split_describe(const char* describe, char* name_buf, char* tags_buf)
{
   strcpy(name_buf, describe);
   strcpy(tags_buf, "");

   const char* p = describe;
   for (bool quote = false; *p; ++p) {
      if (*p == '\"') quote = !quote;
      if (!quote && *p == '[') break;
   }
   if (*p != '[') return;  // left brace
   const char* q = p;
   for (bool quote = false; *q; ++q) {
      if (*q == '\"') quote = !quote;
      if (!quote && *q == ']') break;
   }
   if (*q != ']') return;  // right brace

   strncpy(tags_buf, p + 1, (size_t)(q - p - 1));
   tags_buf[(size_t)(q - p - 1)] = '\0';

   strcpy(&name_buf[(size_t)(p - describe)], q + 1);
}

h2_inline h2_test::h2_test(const char* filine_, const char* file_, int line_, const char* describe_) : filine(filine_), file(h2_basefile(file_)), line(line_), describe(describe_)
{
   if (describe) {
      __split_describe(describe, name_buf, tags_buf);

      name = name_buf;
      while (*name && ::isspace(*name)) name++;  // strip left space

      int count = 0;
      for (char* t = strtok(tags_buf, " ,"); t; t = strtok(nullptr, " ,"))
         if (count < sizeof(tags) / sizeof(tags[0]) - 1) tags[count++] = t;
      tags[count] = nullptr;
   }
}

h2_inline bool h2_test::tagged(const char* tag_pattern) const
{
   for (int i = 0; tags[i]; ++i) {
      if (!strcasecmp(tags[i], tag_pattern)) return true;
      if (strcspn(tag_pattern, "?*[]") < strlen(tag_pattern) && h2_pattern::wildcard_match(tag_pattern, tags[i], true)) return true;
   }
   return false;
}
