h2_inline h2_describe::h2_describe(const char* describe) : desc(describe)
{
   if (desc) {
      strcpy(nbuf, desc);
      strcpy(tbuf, "");
      split();

      name = nbuf;
      while (*name && ::isspace(*name)) name++;  // strip left space

      int count = 0;
      for (char* t = strtok(tbuf, " ,"); t; t = strtok(nullptr, " ,"))
         if (count < sizeof(tags) / sizeof(tags[0]) - 1) tags[count++] = t;
      tags[count] = nullptr;
   }
}

h2_inline void h2_describe::split()
{
   const char* p = desc;
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

   strncpy(tbuf, p + 1, (size_t)(q - p - 1));
   tbuf[(size_t)(q - p - 1)] = '\0';

   strcpy(&nbuf[(size_t)(p - desc)], q + 1);
}

h2_inline bool h2_describe::has_tag(const char* pattern)
{
   for (int i = 0; tags[i]; ++i) {
      if (!strcasecmp(tags[i], pattern)) return true;
      if (strcspn(pattern, "?*[]") < strlen(pattern) && h2_pattern::wildcard_match(pattern, tags[i], true)) return true;
   }
   return false;
}
