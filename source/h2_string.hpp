
struct h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
   h2_string() : basic_string() {}
   h2_string(const h2_string& str) : basic_string(str.c_str()) {}
   h2_string(const std::string& str) : basic_string(str.c_str()) {}
   h2_string(const char* s) : basic_string(s) {}
   h2_string(const char* s, size_t n) : basic_string(s, n) {}
   h2_string(size_t n, char c) : basic_string(n, c) {}

   h2_string& operator=(const h2_string& str) { return assign(str.c_str()), *this; }
   h2_string& operator=(const std::string& str) { return assign(str.c_str()), *this; }
   h2_string& operator=(const char* s) { return assign(s), *this; }
   h2_string& operator=(char c) { return assign(1, c), *this; }

   h2_string& operator+=(const h2_string& str) { return append(str.c_str()), *this; }
   h2_string& operator+=(const std::string& str) { return append(str.c_str()), *this; }
   h2_string& operator+=(const char* s) { return append(s), *this; }
   h2_string& operator+=(char c) { return push_back(c), *this; }

   bool equals(h2_string str, bool caseless = false) const;
   bool contains(h2_string substr, bool caseless = false) const;
   bool startswith(h2_string prefix, bool caseless = false) const;
   bool endswith(h2_string suffix, bool caseless = false) const;

   bool isspace() const;

   h2_string& tolower();
   static h2_string tolower(h2_string from) { return from.tolower(); }
   h2_string acronym(int width = 16, int tail = 0) const;
   h2_string& center(int width);
   h2_string& sprintf(const char* format, ...);
};

template <typename T>
h2_string h2_stringify(T a)
{
   h2_ostringstream os;
   os << std::boolalpha;
   os << a;
   return h2_string(os.str().c_str());
}

template <typename A, size_t N>
h2_string h2_stringify(const std::array<A, N> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::vector<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::deque<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::list<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::forward_list<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::set<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::multiset<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::unordered_set<T> a) { return ""; }
template <typename T>
h2_string h2_stringify(const std::unordered_multiset<T> a) { return ""; }

template <>
inline h2_string h2_stringify(std::nullptr_t a) { return "nullptr"; }

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }
