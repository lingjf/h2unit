struct h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
   h2_string() : basic_string() {}
   h2_string(const h2_string& s) : basic_string(s.c_str()) {}
   h2_string(const std::string& s) : basic_string(s.c_str()) {}
   template <typename... T>
   h2_string(const char* fmt, T... t) : basic_string() { sizeof...(T) ? sprintf(fmt, t...) : assign(fmt ? fmt : "(null)"); }
   h2_string(size_t n, const char* s) : basic_string(s, n) {}
   h2_string(size_t n, const char c) : basic_string(n, c) {}

   h2_string& operator=(const h2_string& s) { return assign(s.c_str()), *this; }
   h2_string& operator=(const std::string& s) { return assign(s.c_str()), *this; }
   h2_string& operator=(const char* s) { return assign(s ? s : "(null)"), *this; }
   h2_string& operator=(const char c) { return assign(1, c), *this; }

   h2_string& operator+=(const h2_string& s) { return append(s.c_str()), *this; }
   h2_string& operator+=(const std::string& s) { return append(s.c_str()), *this; }
   h2_string& operator+=(const char* s) { return append(s), *this; }
   h2_string& operator+=(const char c) { return push_back(c), *this; }

   h2_string& sprintf(const char* fmt, ...);
   h2_string& replace_all(const char* from, const char* to);

   size_t width(size_t fat = 2) const;
   bool equals(const h2_string& fulstr, bool caseless = false) const;
   bool contains(const h2_string& substr, bool caseless = false) const;
   bool startswith(const h2_string& prefix, bool caseless = false) const;
   bool endswith(const h2_string& suffix, bool caseless = false) const;

   bool enclosed(char left = '\"', char right = '\0') const;
   h2_string unenclose(char left = '\"', char right = '\0') const;

   h2_string escape(bool utf8 = false) const;
   h2_string unescape() const;
   h2_string trim() const;
   h2_string squash(bool quote = false) const;
   h2_string tolower() const;
   h2_string centre(size_t width) const;
   h2_vector<h2_string> disperse() const;
};

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }
