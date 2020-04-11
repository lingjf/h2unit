
struct h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
   h2_string() : basic_string() {}
   h2_string(const h2_string& __str) : basic_string(__str.c_str()) {}
   h2_string(const char* __s) : basic_string(__s) {}
   h2_string(const std::string& __s) : basic_string(__s.c_str()) {}
   h2_string(size_t __n, char __c) : basic_string(__n, __c) {}
   h2_string(const char* __s, size_t __n) : basic_string(__s, __n) {}
   h2_string(const unsigned char* __s) : basic_string((const char*)__s) {}

   h2_string& operator=(const h2_string& __str) { return assign(__str.c_str()), *this; }
   h2_string& operator=(const char* __s) { return assign(__s), *this; }
   h2_string& operator=(const std::string& __str) { return assign(__str.c_str()), *this; }
   h2_string& operator=(char __c) { return assign(1, __c), *this; }
   h2_string& operator=(const unsigned char* __s) { return assign((const char*)__s), *this; }

   h2_string& operator+=(const h2_string& __str) { return append(__str.c_str()), *this; }
   h2_string& operator+=(const char* __s) { return append(__s), *this; }
   h2_string& operator+=(const std::string& __str) { return append(__str.c_str()), *this; }
   h2_string& operator+=(char __c) { return push_back(__c), *this; }

   bool equals(h2_string __str, bool caseless = false) const;
   bool contains(h2_string __substr, bool caseless = false) const;
   bool startswith(h2_string __prefix, bool caseless = false) const;
   bool endswith(h2_string __suffix, bool caseless = false) const;

   h2_string& tolower();
   static h2_string tolower(h2_string from) { return from.tolower(); }
   h2_string acronym(int atmost = 16) const;
   h2_string& center(int width);
   h2_string& sprintf(const char* format, ...);
};

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }
