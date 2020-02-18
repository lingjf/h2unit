
template <typename T>
struct h2_matcher_impl {
   virtual h2_fail* matches(T a, bool caseless = false, bool dont = false) const = 0;
   virtual ~h2_matcher_impl() {}
};

template <typename T>
struct h2_matcher_base {
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl;

   h2_matcher_base() {}
   explicit h2_matcher_base(const h2_matcher_impl<const T&>* impl_, const int placeholder) : impl(impl_) {}
   h2_matcher_base(const h2_matcher_base&) = default;
   h2_matcher_base& operator=(const h2_matcher_base&) = default;
   virtual ~h2_matcher_base() {}
   h2_fail* matches(const T& a, bool caseless = false, bool dont = false) const { return impl->matches(a, caseless, dont); }
};

template <typename T>
struct h2_matcher : public h2_matcher_base<T> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl, const int placeholder) : h2_matcher_base<T>(impl, placeholder) {}
   h2_matcher(T value);
};

template <>
struct h2_matcher<const char*> : public h2_matcher_base<const char*> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const char* const&>* impl, const int placeholder) : h2_matcher_base<const char*>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<const std::string&> : public h2_matcher_base<const std::string&> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* impl, const int placeholder) : h2_matcher_base<const std::string&>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<std::string> : public h2_matcher_base<std::string> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* impl, const int placeholder) : h2_matcher_base<std::string>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <typename Matches>
struct h2_polymorphic_matcher {
   const Matches m;
   explicit h2_polymorphic_matcher(const Matches& matches_) : m(matches_) {}

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(m), 0); }

   template <typename T>
   struct internal_impl : public h2_matcher_impl<T> {
      const Matches m;
      explicit internal_impl(const Matches& matches_) : m(matches_) {}
      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override { return m.matches(a, caseless, dont); }
      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };
};

template <typename A, typename E>
inline h2_fail* h2_desc(const A& a, const E& e, bool dont, const char* op) {
   h2_ostringstream ose, osa;
   ose << std::boolalpha << e;
   osa << std::boolalpha << a;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("%s", ose.str().c_str());
   fail->aprintf("%s", osa.str().c_str());
   if (dont)
      fail->mprintf("shoud not %s", op);
   else
      fail->mprintf("not %s", op);

   return fail;
}

struct h2_string_eq_matches {
   const h2_string e;
   h2_string_eq_matches(const h2_string& _e) : e(_e) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.equals(e, caseless) == !dont) return nullptr;
      if (dont) {
         h2_fail_unexpect* fail = new h2_fail_unexpect();
         fail->eprintf("\"%s\"", h2_acronym_string(e.c_str()));
         fail->aprintf("\"%s\"", h2_acronym_string(a.c_str()));
         fail->mprintf("should not %sequal to", caseless ? "caseless " : "");
         return fail;
      } else {
         if (35 < e.length() && 35 < a.length()) return new h2_fail_strcmp(e.c_str(), a.c_str(), caseless);
         h2_fail_unexpect* fail = new h2_fail_unexpect();
         fail->eprintf("\"%s\"", e.c_str());
         fail->aprintf("\"%s\"", a.c_str());
         fail->mprintf("not %sequal to", caseless ? "caseless " : "");
         return fail;
      }
   }
};

struct h2_float_eq_matches {
   const long double e;
   explicit h2_float_eq_matches(const long double _e) : e(_e) {}

   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const {
      // the machine epsilon has to be scaled to the magnitude of the values used
      // and multiplied by the desired precision in ULPs (units in the last place)
      // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
      //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
      bool result = std::fabs(a - e) < 0.00001;
      if (result == !dont) return nullptr;
      return h2_desc(a, e, dont, "equal to");
   }
};

template <typename E>
struct h2_eq_matches {
   const E e;
   explicit h2_eq_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a == e) == !dont) return nullptr;
      return h2_desc(a, e, dont, "equal to");
   }
};

template <>
struct h2_eq_matches<char*> : public h2_string_eq_matches {
   explicit h2_eq_matches(const char* e) : h2_string_eq_matches(h2_string(e)) {}
};
template <>
struct h2_eq_matches<const char*> : public h2_string_eq_matches {
   explicit h2_eq_matches(const char* e) : h2_string_eq_matches(h2_string(e)) {}
};
template <>
struct h2_eq_matches<h2_string> : public h2_string_eq_matches {
   explicit h2_eq_matches(const h2_string e) : h2_string_eq_matches(e) {}
};
template <>
struct h2_eq_matches<std::string> : public h2_string_eq_matches {
   explicit h2_eq_matches(const std::string e) : h2_string_eq_matches(h2_string(e)) {}
};

template <>
struct h2_eq_matches<float> : public h2_float_eq_matches {
   explicit h2_eq_matches(const float e) : h2_float_eq_matches(e) {}
};
template <>
struct h2_eq_matches<double> : public h2_float_eq_matches {
   explicit h2_eq_matches(const double e) : h2_float_eq_matches(e) {}
};
template <>
struct h2_eq_matches<long double> : public h2_float_eq_matches {
   explicit h2_eq_matches(const long double e) : h2_float_eq_matches(e) {}
};

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = h2_polymorphic_matcher<h2_eq_matches<T>>(h2_eq_matches<T>(value)); }

inline h2_matcher<const char*>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<const char*>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_eq_matches>(h2_string_eq_matches(value)); }

template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from) { return do_cast(from, std::is_convertible<M, h2_matcher<T>>{}, std::is_convertible<M, T>{}); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type) { return h2_polymorphic_matcher<h2_eq_matches<M>>(h2_eq_matches<M>(from)); }
};

template <typename T, typename U>
struct h2_matcher_cast_impl<T, h2_matcher<U>> {
   static h2_matcher<T> cast(const h2_matcher<U>& from) { return h2_matcher<T>(new internal_impl(from)); }

   struct internal_impl : public h2_matcher_impl<T> {
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}

      // Delegate the matching logic to the source h2_matcher.
      h2_fail* matches(T a, bool caseless, bool dont) const override {
         using FromType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::type;
         using ToType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<U>::type>::type>::type;
         // Do not allow implicitly converting base*/& to derived*/&.
         static_assert((std::is_pointer<typename std::remove_reference<T>::type>::value != std::is_pointer<typename std::remove_reference<U>::type>::value) || std::is_same<FromType, ToType>::value || !std::is_base_of<FromType, ToType>::value, "Can't implicitly convert from <base> to <derived>");

         return from.matches(static_cast<U>(a), caseless, dont);
      }
      const h2_matcher<U> from;
      void operator=(internal_impl const&) = delete;

      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };
};

template <typename T>
struct h2_matcher_cast_impl<T, h2_matcher<T>> {
   static h2_matcher<T> cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> h2_matcher_cast(const M& from) { return h2_matcher_cast_impl<T, M>::cast(from); }

struct h2_any_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
};

struct h2_null_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((nullptr == reinterpret_cast<const void*>(a)) == !dont) return nullptr;
      if (dont)
         return new h2_fail_normal(nullptr, 0, nullptr, "shoud not be null");
      else
         return new h2_fail_normal(nullptr, 0, nullptr, "is not null %p", reinterpret_cast<const void*>(a));
   }
};

template <typename E>
struct h2_ge_matches {
   const E e;
   explicit h2_ge_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a >= e) == !dont) return nullptr;
      return h2_desc(a, e, dont, ">=");
   }
};

template <typename E>
struct h2_gt_matches {
   const E e;
   explicit h2_gt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a > e) == !dont) return nullptr;
      return h2_desc(a, e, dont, ">");
   }
};

template <typename E>
struct h2_le_matches {
   const E e;
   explicit h2_le_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a <= e) == !dont) return nullptr;
      return h2_desc(a, e, dont, "<=");
   }
};

template <typename E>
struct h2_lt_matches {
   const E e;
   explicit h2_lt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((a < e) == !dont) return nullptr;
      return h2_desc(a, e, dont, "<");
   }
};

struct h2_me_matches {
   const void* e;
   const int size;
   explicit h2_me_matches(const void* _e, const int _size) : e(_e), size(_size) {}

   h2_fail* matches(const void* a, bool caseless = false, bool dont = false) const {
      int sz = size ? size : strlen((const char*)e);
      if ((memcmp(e, a, sz) == 0) == !dont) return nullptr;
      return new h2_fail_memcmp(e, a, sz);
   }
};

template <typename Matcher>
struct h2_pe_matches {
   const Matcher m;
   explicit h2_pe_matches(Matcher matcher_) : m(matcher_) {}

   template <typename SmartPointer>
   struct PointeeOf { typedef typename SmartPointer::element_type type; };
   template <typename T>
   struct PointeeOf<T*> { typedef T type; };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;

      return h2_matcher_cast<Pointee>(m).matches(*a, caseless, dont);
   }
};

struct h2_regex_matches {
   const h2_string e;
   explicit h2_regex_matches(const h2_string& _e) : e(_e) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      bool result;
      try {
         std::regex re(e);
         result = std::regex_match(a, re);
      }
      catch (const std::regex_error&) {
         result = false;
      }
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud matches Regex");
      else
         fail->mprintf("not matches Regex");

      return fail;
   }
};

struct h2_wildcard_matches {
   const h2_string e;
   explicit h2_wildcard_matches(const h2_string& _e) : e(_e) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (h2_wildcard_match(e.c_str(), a.c_str()) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud matches Wildcard");
      else
         fail->mprintf("not matches Wildcard");

      return fail;
   }
};

struct h2_contains_matches {
   const h2_string substring;
   explicit h2_contains_matches(const h2_string& substring_) : substring(substring_) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.contains(substring, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", substring.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %shas substr", caseless ? "caseless " : "");
      else
         fail->mprintf("not %shas substr", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_startswith_matches {
   const h2_string prefix_string;
   explicit h2_startswith_matches(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", prefix_string.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %sstarts with", caseless ? "caseless " : "");
      else
         fail->mprintf("not %sstarts with", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_endswith_matches {
   const h2_string suffix_string;
   explicit h2_endswith_matches(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", suffix_string.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %sends with", caseless ? "caseless " : "");
      else
         fail->mprintf("not %sends with", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_je_matches {
   const h2_string e;
   explicit h2_je_matches(const h2_string& _e) : e(_e) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if ((h2_json::match(e.c_str(), a.c_str())) == !dont) return nullptr;
      if (dont)
         return new h2_fail_json("JSON should not equals", e.c_str(), a.c_str());
      else
         return new h2_fail_json("JSON not equals", e.c_str(), a.c_str());
   }
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      return m.matches(a, true, dont);
   }
};

template <typename Matcher>
struct h2_not_matches {
   const Matcher m;
   explicit h2_not_matches(Matcher matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      return h2_matcher_cast<A>(m).matches(a, caseless, !dont);
   }
};

#define H2_MATCHER_T2V(t_matchers)                                                                          \
   template <typename T, size_t I>                                                                          \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), \
                                h2_vector<h2_matcher<T>> v_matchers = {}) const {                           \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                    \
      return t2v<T>(std::integral_constant<size_t, I + 1>(), v_matchers);                                   \
   }                                                                                                        \
   template <typename T>                                                                                    \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, sizeof...(Matchers)>,                        \
                                h2_vector<h2_matcher<T>> v_matchers = {}) const { return v_matchers; }

template <typename... Matchers>
struct h2_allof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<A, 0>();

      h2_fail* fail = nullptr;
      for (auto& m : v_matchers)
         h2_append_y_fail(fail, m.matches(a, caseless, false));

      if (!fail == !dont) return nullptr;

      if (dont)
         return new h2_fail_normal(nullptr, 0, nullptr, "should not matches any of matcher");
      else
         return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* fail = m.matches(a, caseless, false);
         if (!fail) s++;
         if (fail) delete fail;
      }

      if ((0 < s) == !dont) return nullptr;

      h2_ostringstream osa;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("\"%s\"", osa.str().c_str());
      if (dont)
         fail->mprintf("should matches none of matcher");
      else
         fail->mprintf("not matches any of matcher");
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<A, 0>();

      h2_fail* fail = nullptr;
      for (auto& m : v_matchers)
         h2_append_y_fail(fail, m.matches(a, caseless, true));

      if (!fail == !dont) return nullptr;

      if (dont)
         return new h2_fail_normal(nullptr, 0, nullptr, "should matches any of matcher");
      else
         return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_listof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      auto v_matchers = t2v<decltype(a[0]), 0>();

      h2_fail* fail = nullptr;
      for (size_t i = 0; i < v_matchers.size(); ++i)
         h2_append_y_fail(fail, v_matchers[i].matches(a[i], caseless, dont));

      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

const h2_polymorphic_matcher<h2_any_matches> Any__{h2_any_matches()};

inline h2_polymorphic_matcher<h2_null_matches> Null__() {
   return h2_polymorphic_matcher<h2_null_matches>(h2_null_matches());
}
template <typename E>
inline h2_polymorphic_matcher<h2_eq_matches<E>> Eq__(const E expect) {
   return h2_polymorphic_matcher<h2_eq_matches<E>>(h2_eq_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_ge_matches<E>> Ge__(const E expect) {
   return h2_polymorphic_matcher<h2_ge_matches<E>>(h2_ge_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_gt_matches<E>> Gt__(const E expect) {
   return h2_polymorphic_matcher<h2_gt_matches<E>>(h2_gt_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_le_matches<E>> Le__(const E expect) {
   return h2_polymorphic_matcher<h2_le_matches<E>>(h2_le_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_lt_matches<E>> Lt__(const E expect) {
   return h2_polymorphic_matcher<h2_lt_matches<E>>(h2_lt_matches<E>(expect));
}
inline h2_polymorphic_matcher<h2_me_matches> Me__(const void* buf, const int size = 0) {
   return h2_polymorphic_matcher<h2_me_matches>(h2_me_matches(buf, size));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_pe_matches<Matcher>> Pe__(Matcher expect) {
   return h2_polymorphic_matcher<h2_pe_matches<Matcher>>(h2_pe_matches<Matcher>(expect));
}
inline h2_polymorphic_matcher<h2_regex_matches> Re__(const h2_string& regex_pattern) {
   return h2_polymorphic_matcher<h2_regex_matches>(h2_regex_matches(regex_pattern));
}
inline h2_polymorphic_matcher<h2_wildcard_matches> We__(const h2_string& wildcard_pattern) {
   return h2_polymorphic_matcher<h2_wildcard_matches>(h2_wildcard_matches(wildcard_pattern));
}
inline h2_polymorphic_matcher<h2_contains_matches> Contains__(const h2_string& substring) {
   return h2_polymorphic_matcher<h2_contains_matches>(h2_contains_matches(substring));
}
inline h2_polymorphic_matcher<h2_startswith_matches> StartsWith__(const h2_string& prefix_string) {
   return h2_polymorphic_matcher<h2_startswith_matches>(h2_startswith_matches(prefix_string));
}
inline h2_polymorphic_matcher<h2_endswith_matches> EndsWith__(const h2_string& suffix_string) {
   return h2_polymorphic_matcher<h2_endswith_matches>(h2_endswith_matches(suffix_string));
}
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess__(h2_matcher<h2_string> expect) {
   return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(expect));
}
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess__(h2_string expect) {
   return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(expect));
}
inline h2_polymorphic_matcher<h2_je_matches> Je__(const h2_string& expect) {
   return h2_polymorphic_matcher<h2_je_matches>(h2_je_matches(expect));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not__(Matcher expect) {
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(expect));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> AllOf__(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>>(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> AnyOf__(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>>(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> NoneOf__(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>>(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf__(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
