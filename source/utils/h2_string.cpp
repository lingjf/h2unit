static inline size_t utf8len(const char* s)
{
   if (0xf0 == (0xf8 & *(const unsigned char*)s)) return 4;  // 4-byte utf8 code point (began with 0b11110xxx)
   if (0xe0 == (0xf0 & *(const unsigned char*)s)) return 3;  // 3-byte utf8 code point (began with 0b1110xxxx)
   if (0xc0 == (0xe0 & *(const unsigned char*)s)) return 2;  // 2-byte utf8 code point (began with 0b110xxxxx)
   return 1;                                                 // 1-byte ascii (began with 0b0xxxxxxx)
}

h2_inline h2_string& h2_string::sprintf(const char* fmt, ...)
{
   char* alloca_str;
   h2_sprintf(alloca_str, fmt);
   append(alloca_str);
   return *this;
}

h2_inline h2_string& h2_string::replace_all(const char* from, const char* to)
{
   size_t start_pos = 0, from_length = strlen(from), to_length = strlen(to);
   while ((start_pos = find(from, start_pos)) != h2_string::npos) {
      replace(start_pos, from_length, to);
      start_pos += to_length;  // where 'to' is a substring of 'from'
   }
   return *this;
}

h2_inline size_t h2_string::width(size_t fat) const  // wcwidth()/wcswidth()
{
   size_t w = 0, n = 0;
   for (const char* p = c_str(); *p != '\0'; p += n) {
      n = utf8len(p);
      w += (n == 1 ? 1 : fat);
   }
   return w;
}

h2_inline bool h2_string::equals(const h2_string& fulstr, bool caseless) const
{
   if (!caseless) return *this == fulstr;
   return tolower() == fulstr.tolower();
}

h2_inline bool h2_string::contains(const h2_string& substr, bool caseless) const
{
   if (!caseless) return find(substr) != h2_string::npos;
   return tolower().find(substr.tolower()) != h2_string::npos;
}

h2_inline bool h2_string::startswith(const h2_string& prefix, bool caseless) const
{
   if (size() < prefix.size()) return false;
   if (!caseless) return find(prefix) == 0;
   return tolower().find(prefix.tolower()) == 0;
}

h2_inline bool h2_string::endswith(const h2_string& suffix, bool caseless) const
{
   if (size() < suffix.size()) return false;
   if (!caseless) return rfind(suffix) == size() - suffix.size();
   return tolower().rfind(suffix.tolower()) == size() - suffix.size();
}

h2_inline bool h2_string::enclosed(char left, char right) const
{
   if (right == '\0') right = left;
   return front() == left && back() == right;
}

h2_inline h2_string h2_string::unenclose(char left, char right) const
{
   if (!enclosed(left, right)) return *this;
   return h2_string(size() - 2, c_str() + 1);
}

h2_inline h2_string h2_string::escape(bool utf8) const
{
   h2_string s;
   for (auto& c : *this)
      switch (c) {
         case '\n': s.append(utf8 ? "␍" : "\\n"); break;
         case '\r': s.append(utf8 ? "␊" : "\\r"); break;
         case '\t': s.append(utf8 ? "␉" : "\\t"); break;
         case '\0': s.append(utf8 ? "␀" : "\0"); break;
         default: s.push_back(c); break;
      }
   return s;
}

h2_inline h2_string h2_string::unescape() const
{
   h2_string s = *this;
   s.replace_all("\\b", "\b");
   s.replace_all("\\f", "\f");
   s.replace_all("\\n", "\n");
   s.replace_all("\\r", "\r");
   s.replace_all("\\t", "\t");
   s.replace_all("\\\"", "\"");
   s.replace_all("\\\\", "\\");
   return s;
}

h2_inline h2_string h2_string::trim() const
{
   const auto a = find_first_not_of("\t\n ");
   if (a == h2_string::npos) return "";
   const auto b = find_last_not_of("\t\n ");
   return substr(a, b - a + 1).c_str();
}

h2_inline h2_string h2_string::squash(bool quote) const
{
   h2_string s;
   bool quote1 = false, quote2 = false;
   int spaces = 0;
   for (char c : trim()) {
      if (c == '\t' || c == '\n' || c == ' ') c = ' ';
      if (c != ' ') spaces = 0;
      if (!quote && c == '\'') quote1 = !quote1;
      if (!quote && c == '\"') quote2 = !quote2;
      if (!quote1 && !quote2 && c == ' ' && spaces++) continue;
      s.push_back(c);
   }
   return s;
}

h2_inline h2_string h2_string::tolower() const
{
   h2_string s;
   for (auto& c : *this) s.push_back(::tolower(c));
   return s;
}

h2_inline h2_string h2_string::centre(size_t width) const
{
   if (width <= size()) return *this;
   size_t left = (width - size()) / 2, right = width - left - size();
   h2_string s;
   s.append(left, ' ');
   s.append(*this);
   s.append(right, ' ');
   return s;
}

h2_inline h2_vector<h2_string> h2_string::disperse() const
{
   h2_vector<h2_string> chars;
   for (const char* p = c_str(); *p != '\0';) {
      auto n = utf8len(p);
      chars.push_back(h2_string(n, p));
      p += n;
   }
   return chars;
}
