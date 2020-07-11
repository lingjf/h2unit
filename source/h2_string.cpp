
h2_inline bool h2_string::equals(const h2_string& str, bool caseless) const
{
   if (!caseless) return *this == str;
   return tolower(c_str()) == tolower(str);
}

h2_inline bool h2_string::contains(const h2_string& substr, bool caseless) const
{
   if (!caseless) return find(substr) != h2_string::npos;
   return tolower(c_str()).find(tolower(substr)) != h2_string::npos;
}

h2_inline bool h2_string::startswith(const h2_string& prefix, bool caseless) const
{
   if (size() < prefix.size()) return false;
   if (!caseless) return find(prefix) == 0;
   return tolower(c_str()).find(tolower(prefix)) == 0;
}

h2_inline bool h2_string::endswith(const h2_string& suffix, bool caseless) const
{
   if (size() < suffix.size()) return false;
   if (!caseless) return rfind(suffix) == length() - suffix.length();
   return tolower(c_str()).rfind(tolower(suffix)) == length() - suffix.length();
}

h2_inline bool h2_string::isspace() const
{
   for (auto& c : *this)
      if (!::isspace(c)) return false;
   return true;
}

h2_inline bool h2_string::enclosed(char c) const
{
   return front() == c && back() == c;
}

h2_inline h2_string h2_string::unquote(char c) const
{
   if (!enclosed(c)) return *this;
   return h2_string(c_str() + 1, size() - 2);
}

h2_inline h2_string& h2_string::replace_all(const char* from, const char* to)
{
   size_t start_pos = 0, from_length = strlen(from), to_length = strlen(to);
   while ((start_pos = find(from, start_pos)) != h2_string::npos) {
      replace(start_pos, from_length, to);
      start_pos += to_length;  // handles case where 'to' is a substring of 'from'
   }
   return *this;
}

h2_inline h2_string& h2_string::tolower()
{
   for (auto& c : *this) c = ::tolower(c);
   return *this;
}

static inline void escape_char(const char c, h2_string& t)
{
   char d = c;
   switch (c) {
   case '\n': d = 'n'; break;
   case '\r': d = 'r'; break;
   case '\t': d = 't'; break;
   }
   if (d != c) t.push_back('\\');
   t.push_back(d);
}

h2_inline h2_string h2_string::acronym(int width, int tail) const
{
   h2_string t;
   if (size() <= width) {
      for (int i = 0; i < size(); ++i) escape_char(c_str()[i], t);
   } else {
      for (int i = 0; i < width - 3 - tail; ++i) escape_char(c_str()[i], t);
      t.append("\033{+dark gray}...\033{-dark gray}");
      for (int i = 0; i < tail; ++i) escape_char(c_str()[size() - tail + i], t);
   }
   return t;
}

h2_inline h2_string& h2_string::center(int width)
{
   int left = (width - length()) / 2, right = width - left - length();
   insert(0, left, ' ');
   append(right, ' ');
   return *this;
}

h2_inline h2_string& h2_string::sprintf(const char* format, ...)
{
   char* alloca_str;
   h2_sprintf(alloca_str, format);
   append(alloca_str);
   return *this;
}
