
static inline h2_string CD(h2_string s, bool caseless = false, bool dont = false)
{
   h2_string z = s;
   if (dont) z = "!" + z;
   if (caseless) z = "~" + z;
   return z;
}

struct h2_equals_string {
   const h2_string e;
   h2_equals_string(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_equals_float {
   const long double e;
   explicit h2_equals_float(const long double _e) : e(_e) {}
   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const;
   h2_string expects(const long double a, bool caseless = false, bool dont = false) const;
};

template <typename E>
struct h2_equals {
   const E e;
   explicit h2_equals(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a == e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }

   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(h2_stringify(e), caseless, dont);
   }
};

template <>
struct h2_equals<char*> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<char* const> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<const char*> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<const char* const> : h2_equals_string {
   explicit h2_equals(const char* e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<h2_string> : h2_equals_string {
   explicit h2_equals(const h2_string e) : h2_equals_string(e) {}
};
template <>
struct h2_equals<std::string> : h2_equals_string {
   explicit h2_equals(const std::string e) : h2_equals_string(h2_string(e)) {}
};
template <>
struct h2_equals<float> : h2_equals_float {
   explicit h2_equals(const float e) : h2_equals_float(e) {}
};
template <>
struct h2_equals<double> : h2_equals_float {
   explicit h2_equals(const double e) : h2_equals_float(e) {}
};
template <>
struct h2_equals<long double> : h2_equals_float {
   explicit h2_equals(const long double e) : h2_equals_float(e) {}
};

struct h2_matchee_any {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return "Any"; }
};

struct h2_matchee_null {
   const bool reverse;
   explicit h2_matchee_null(bool _reverse) : reverse(_reverse) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      bool _dont = reverse ? !dont : dont;
      if ((nullptr == (const void*)a) == !_dont) return nullptr;
      return new h2_fail_unexpect("", h2_stringify((const void*)a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return (reverse ? !dont : dont) ? "NotNull" : "IsNull"; }
};

template <bool E>
struct h2_matchee_boolean {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      bool _dont = E ? dont : !dont;
      if (((bool)a) == !_dont) return nullptr;
      return new h2_fail_unexpect("", a ? "true" : "false", expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const { return (E ? dont : !dont) ? "false" : "true"; }
};

template <typename E>
struct h2_matchee_ge {
   const E e;
   explicit h2_matchee_ge(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a >= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(">=" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matchee_gt {
   const E e;
   explicit h2_matchee_gt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a > e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD(">" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matchee_le {
   const E e;
   explicit h2_matchee_le(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a <= e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD("<=" + h2_stringify(e), caseless, dont);
   }
};

template <typename E>
struct h2_matchee_lt {
   const E e;
   explicit h2_matchee_lt(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a < e) == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }
   template <typename A>
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return CD("<" + h2_stringify(e), caseless, dont);
   }
};

struct h2_matchee_regex {
   const h2_string e;
   explicit h2_matchee_regex(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_wildcard {
   const h2_string e;
   explicit h2_matchee_wildcard(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_strcmp {
   const h2_string e;
   explicit h2_matchee_strcmp(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_substr {
   const h2_string substring;
   explicit h2_matchee_substr(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_startswith {
   const h2_string prefix_string;
   explicit h2_matchee_startswith(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_endswith {
   const h2_string suffix_string;
   explicit h2_matchee_endswith(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_json {
   const h2_string e;
   explicit h2_matchee_json(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_matchee_memcmp {
   const int width;
   const void* e;
   const int nbits;
   explicit h2_matchee_memcmp(const int _width, const void* _e, const int _nbits) : width(_width), e(_e), nbits(_nbits) {}
   h2_fail* matches(const void* a, bool caseless = false, bool dont = false) const;
   h2_string expects(const void* a, bool caseless = false, bool dont = false) const;
};
