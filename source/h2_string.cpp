
h2_inline bool h2_string::equals(h2_string __str, bool caseless) const {
   if (!caseless) return *this == __str;
   return tolower(c_str()) == tolower(__str);
}

h2_inline bool h2_string::contains(h2_string __substr, bool caseless) const {
   if (!caseless) return find(__substr) != h2_string::npos;
   return tolower(c_str()).find(tolower(__substr)) != h2_string::npos;
}

h2_inline bool h2_string::startswith(h2_string __prefix, bool caseless) const {
   if (!caseless) return find(__prefix) == 0;
   return tolower(c_str()).find(tolower(__prefix)) == 0;
}

h2_inline bool h2_string::endswith(h2_string __suffix, bool caseless) const {
   if (!caseless) return rfind(__suffix) == length() - __suffix.length();
   return tolower(c_str()).rfind(tolower(__suffix)) == length() - __suffix.length();
}

h2_inline bool h2_string::wildcard_match(h2_string __pattern, bool caseless) const {
   h2_wildcard_match(__pattern.c_str(), c_str());
   if (!caseless) return h2_wildcard_match(__pattern.c_str(), c_str());
   return h2_wildcard_match(tolower(__pattern).c_str(), tolower(c_str()).c_str());
}

h2_inline bool h2_string::regex_match(h2_string __pattern, bool caseless) const {
   bool result = false;
   try {
      std::regex re1(__pattern.c_str());
      std::regex re2(__pattern.c_str(), std::regex::icase);
      result = std::regex_match(c_str(), caseless ? re2 : re1);
   }
   catch (const std::regex_error& e) {
      result = false;
   }
   return result;
}

h2_inline h2_string& h2_string::tolower() {
   for (auto& c : *this) c = ::tolower(c);
   return *this;
}

h2_inline h2_string h2_string::acronym(int atmost) const {
   char t[256];
   strncpy(t, c_str(), atmost);
   strcpy(t + atmost, O.colorable ? "\033[90m...\033[0m" : "...");
   return h2_string(t);
}

h2_inline h2_string& h2_string::center(int width) {
   int left = (width - length()) / 2, right = width - left - length();
   insert(0, left, ' ');
   append(right, ' ');
   return *this;
}

h2_inline h2_string& h2_string::sprintf(const char* format, ...) {
   va_list a;
   va_start(a, format);
#if defined _WIN32
   int length = _vscprintf(format, a);
#else
   int length = vsnprintf(nullptr, 0, format, a);
#endif
   va_end(a);
   char* t = (char*)alloca(length + 1);
   va_start(a, format);
   vsprintf(t, format, a);
   va_end(a);
   append(t);
   return *this;
}