
static inline h2_string CD(h2_string s, bool caseless = false, bool dont = false)
{
   h2_string z = s;
   if (dont) z = "!" + z;
   if (caseless) z = "~" + z;
   return z;
}

template <typename E, typename = void>
struct h2_equals {
   const E e;
   explicit h2_equals(const E& _e, const long double = 0) : e(_e) {}

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

template <typename E>
struct h2_equals<E, typename std::enable_if<h2_stringable<E>::value>::type> {
   const h2_string e;
   explicit h2_equals(const E& _e, const long double = 0) : e(h2_string(_e)) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const
   {
      if (a.equals(e, caseless) == !dont) return nullptr;
      if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
      if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;

      return new h2_fail_strcmp(e, a, caseless, expects(a, caseless, dont));
   }

   h2_string expects(const h2_string& a, bool caseless = false, bool dont = false) const
   {
      return CD("\"" + e + "\"", caseless, dont);
   }
};

template <typename E>
struct h2_equals<E, typename std::enable_if<std::is_floating_point<E>::value>::type> {
   const long double e, epsilon;
   explicit h2_equals(const E& _e, const long double _epsilon = 0) : e((long double)_e), epsilon(_epsilon) {}

   h2_fail* matches(const long double& a, bool caseless = false, bool dont = false) const
   {
      // the machine epsilon has to be scaled to the magnitude of the values used
      // and multiplied by the desired precision in ULPs (units in the last place)
      // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
      //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
      long double _epsilon = epsilon;
      if (_epsilon == 0) _epsilon = 0.00001;
      bool result = std::fabs(a - e) < _epsilon;
      if (result == !dont) return nullptr;
      return new h2_fail_unexpect(h2_stringify(e), h2_stringify(a), expects(a, false, dont));
   }

   h2_string expects(const long double& a, bool caseless = false, bool dont = false) const
   {
      return CD(h2_stringify(e), caseless, dont);
   }
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
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return (reverse ? !dont : dont) ? "NotNull" : "IsNull";
   }
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
   h2_string expects(const A& a, bool caseless = false, bool dont = false) const
   {
      return (E ? dont : !dont) ? "false" : "true";
   }
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
