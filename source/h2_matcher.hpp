
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
struct h2_matcher : h2_matcher_base<T> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const T&>* impl, const int placeholder) : h2_matcher_base<T>(impl, placeholder) {}
   h2_matcher(T value);  // Converting constructor
};

template <>
struct h2_matcher<const char*> : h2_matcher_base<const char*> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const char* const&>* impl, const int placeholder) : h2_matcher_base<const char*>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<const std::string&> : h2_matcher_base<const std::string&> {
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* impl, const int placeholder) : h2_matcher_base<const std::string&>(impl, placeholder) {}
   h2_matcher(const std::string& value);
   h2_matcher(const char* value);
};

template <>
struct h2_matcher<std::string> : h2_matcher_base<std::string> {
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
   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      const Matches m;
      explicit internal_impl(const Matches& matches_) : m(matches_) {}
      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override { return m.matches(a, caseless, dont); }
   };
};

template <typename A, typename E>
inline h2_fail* h2_common_unexpect(const A& a, const E& e, bool dont, const char* op)
{
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

struct h2_string_equal_matches {
   const h2_string e;
   const bool r;
   h2_string_equal_matches(const h2_string& _e, bool _r = false) : e(_e), r(_r) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_float_equal_matches {
   const long double e;
   const bool r;
   explicit h2_float_equal_matches(const long double _e, bool _r) : e(_e), r(_r) {}
   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const;
};

template <typename E>
struct h2_equal_matches {
   const E e;
   const bool r;
   explicit h2_equal_matches(const E& _e, bool _r = false) : e(_e), r(_r) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if (r) dont = !dont;
      if ((a == e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, "equals");
   }
};

template <>
struct h2_equal_matches<char*> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<char* const> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<const char*> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<const char* const> : h2_string_equal_matches {
   explicit h2_equal_matches(const char* e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};
template <>
struct h2_equal_matches<h2_string> : h2_string_equal_matches {
   explicit h2_equal_matches(const h2_string e, bool r = false) : h2_string_equal_matches(e, r) {}
};
template <>
struct h2_equal_matches<std::string> : h2_string_equal_matches {
   explicit h2_equal_matches(const std::string e, bool r = false) : h2_string_equal_matches(h2_string(e), r) {}
};

template <>
struct h2_equal_matches<float> : h2_float_equal_matches {
   explicit h2_equal_matches(const float e, bool r = false) : h2_float_equal_matches(e, r) {}
};
template <>
struct h2_equal_matches<double> : h2_float_equal_matches {
   explicit h2_equal_matches(const double e, bool r = false) : h2_float_equal_matches(e, r) {}
};
template <>
struct h2_equal_matches<long double> : h2_float_equal_matches {
   explicit h2_equal_matches(const long double e, bool r = false) : h2_float_equal_matches(e, r) {}
};

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = h2_polymorphic_matcher<h2_equal_matches<T>>(h2_equal_matches<T>(value)); }

inline h2_matcher<const char*>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<const char*>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<const std::string&>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const std::string& value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }
inline h2_matcher<std::string>::h2_matcher(const char* value) { *this = h2_polymorphic_matcher<h2_string_equal_matches>(h2_string_equal_matches(value)); }

template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from) { return do_cast(from, std::is_convertible<M, h2_matcher<T>>{}, std::is_convertible<M, T>{}); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }

   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type) { return h2_polymorphic_matcher<h2_equal_matches<M>>(h2_equal_matches<M>(from)); }
};

template <typename T, typename U>
struct h2_matcher_cast_impl<T, h2_matcher<U>> {
   static h2_matcher<T> cast(const h2_matcher<U>& from) { return h2_matcher<T>(new internal_impl(from)); }

   struct internal_impl : h2_matcher_impl<T>, h2_libc {
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}

      // Delegate the matching logic to the source h2_matcher.
      h2_fail* matches(T a, bool caseless, bool dont) const override
      {
         using FromType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::type;
         using ToType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<U>::type>::type>::type;
         // Do not allow implicitly converting base*/& to derived*/&.
         static_assert((std::is_pointer<typename std::remove_reference<T>::type>::value != std::is_pointer<typename std::remove_reference<U>::type>::value) || std::is_same<FromType, ToType>::value || !std::is_base_of<FromType, ToType>::value, "Can't implicitly convert from <base> to <derived>");

         return from.matches(static_cast<U>(a), caseless, dont);
      }
      const h2_matcher<U> from;
      void operator=(internal_impl const&) = delete;
   };
};

template <typename T>
struct h2_matcher_cast_impl<T, h2_matcher<T>> {
   static h2_matcher<T> cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> h2_matcher_cast(const M& from) { return h2_matcher_cast_impl<T, M>::cast(from); }

template <typename Matcher>
struct h2_not_matches {
   const Matcher m;
   explicit h2_not_matches(Matcher matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      return h2_matcher_cast<A>(m).matches(a, caseless, !dont);
   }
};

template <typename M1, typename M2>
struct h2_and_matches {
   const M1 m1;
   const M2 m2;
   explicit h2_and_matches(M1 _m1, M2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* fail = nullptr;
      h2_fail::append_y(fail, h2_matcher_cast<A>(m1).matches(a, caseless, false));
      h2_fail::append_y(fail, h2_matcher_cast<A>(m2).matches(a, caseless, false));
      if (!fail == !dont) return nullptr;
      if (dont) {
         fail = new h2_fail_unexpect();
         fail->mprintf("should not both match");
      } else {
      }
      return fail;
   }
};

template <typename M1, typename M2>
struct h2_or_matches {
   const M1 m1;
   const M2 m2;
   explicit h2_or_matches(M1 _m1, M2 _m2) : m1(_m1), m2(_m2) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_fail* f1 = h2_matcher_cast<A>(m1).matches(a, caseless, false);
      h2_fail* f2 = h2_matcher_cast<A>(m2).matches(a, caseless, false);
      bool result = !f1 || !f2;
      if (result == !dont) return nullptr;
      h2_fail* fail = new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not match any");
      else
         fail->mprintf("not match any");

      return fail;
   }
};

struct h2_any_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
};

struct h2_null_matches {
   const bool r;
   explicit h2_null_matches(bool _r = false) : r(_r) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if (r) dont = !dont;
      if ((nullptr == (const void*)a) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("%p", (const void*)a);
      if (dont)
         fail->mprintf("should not null");
      else
         fail->mprintf("is not null");
      return fail;
   }
};

struct h2_boolean_matches {
   const bool e;
   explicit h2_boolean_matches(bool _e) : e(_e) {}
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((e == (bool)a) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf(a ? "true" : "false");
      if (dont)
         fail->mprintf("should not %s", e ? "true" : "false");
      else
         fail->mprintf("is not %s", e ? "true" : "false");
      return fail;
   }
};

template <typename E>
struct h2_ge_matches {
   const E e;
   explicit h2_ge_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a >= e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, ">=");
   }
};

template <typename E>
struct h2_gt_matches {
   const E e;
   explicit h2_gt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a > e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, ">");
   }
};

template <typename E>
struct h2_le_matches {
   const E e;
   explicit h2_le_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a <= e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, "<=");
   }
};

template <typename E>
struct h2_lt_matches {
   const E e;
   explicit h2_lt_matches(const E& _e) : e(_e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      if ((a < e) == !dont) return nullptr;
      return h2_common_unexpect(a, e, dont, "<");
   }
};

struct h2_memcmp_matches {
   const int width;
   const void* e;
   const int nbits;
   explicit h2_memcmp_matches(const int _width, const void* _e, const int _nbits) : width(_width), e(_e), nbits(_nbits) {}
   h2_fail* matches(const void* a, bool caseless = false, bool dont = false) const;
};

template <typename Matcher>
struct h2_pointee_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher matcher_) : m(matcher_) {}

   template <typename SmartPointer>
   struct PointeeOf {
      typedef typename SmartPointer::element_type type;
   };
   template <typename T>
   struct PointeeOf<T*> {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, caseless, dont);
   }
};

struct h2_regex_matches {
   const h2_string e;
   explicit h2_regex_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_wildcard_matches {
   const h2_string e;
   explicit h2_wildcard_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_strcmp_matches {
   const h2_string e;
   explicit h2_strcmp_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_contains_matches {
   const h2_string substring;
   explicit h2_contains_matches(const h2_string& substring_) : substring(substring_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_startswith_matches {
   const h2_string prefix_string;
   explicit h2_startswith_matches(const h2_string& prefix_string_) : prefix_string(prefix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_endswith_matches {
   const h2_string suffix_string;
   explicit h2_endswith_matches(const h2_string& suffix_string_) : suffix_string(suffix_string_) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_json_matches {
   const h2_string e;
   explicit h2_json_matches(const h2_string& _e) : e(_e) {}
   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const;
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> matcher_) : m(matcher_) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return m.matches(a, true, dont); }
};

#define H2_MATCHER_T2V(t_matchers)                                                                                                                          \
   template <typename T, size_t I>                                                                                                                          \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, I> _1 = std::integral_constant<size_t, 0>(), h2_vector<h2_matcher<T>> v_matchers = {}) const \
   {                                                                                                                                                        \
      v_matchers.push_back(h2_matcher_cast<T>(std::get<I>(t_matchers)));                                                                                    \
      return t2v<T>(std::integral_constant<size_t, I + 1>(), v_matchers);                                                                                   \
   }                                                                                                                                                        \
   template <typename T>                                                                                                                                    \
   h2_vector<h2_matcher<T>> t2v(std::integral_constant<size_t, sizeof...(Matchers)>, h2_vector<h2_matcher<T>> v_matchers = {}) const { return v_matchers; }

template <typename... Matchers>
struct h2_allof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_allof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* f = v_matchers[i].matches(a, caseless, false);
         if (f) f->kprintf(" %d. ", i);
         h2_fail::append_y(fails, f);
      }

      if (!fails == !dont) return nullptr;

      h2_fail* fail = new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not matches all of matchers");
      else {
         fail->mprintf("should matches all of matchers");
         h2_fail::append_x(fail, fails);
      }
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_anyof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_anyof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      h2_fail* fails = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i) {
         h2_fail* f = v_matchers[i].matches(a, caseless, false);
         if (!f) s++;
         if (f) f->kprintf(" %d. ", i);
         h2_fail::append_y(fails, f);
      }

      if ((0 < s) == !dont) return nullptr;

      h2_fail* fail = (h2_fail*)new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not matches any of matchers");
      else {
         fail->mprintf("not matches any of matchers");
         h2_fail::append_x(fail, fails);
      }
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_noneof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_noneof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<A, 0>();

      int s = 0;
      for (auto& m : v_matchers) {
         h2_fail* f = m.matches(a, caseless, false);
         if (!f) ++s;
      }

      if ((s == 0) == !dont) return nullptr;

      h2_fail_unexpect* fail = new h2_fail_unexpect();
      if (dont)
         fail->mprintf("should not matches none of matcher");
      else
         fail->mprintf("not matches none of matcher");
      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

template <typename... Matchers>
struct h2_listof_matches {
   std::tuple<Matchers...> t_matchers;

   explicit h2_listof_matches(const Matchers&... matchers) : t_matchers(matchers...) { static_assert(sizeof...(Matchers) > 0, "Must have at least one Matcher."); }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      auto v_matchers = t2v<decltype(a[0]), 0>();

      h2_fail* fail = nullptr;
      for (int i = 0; i < v_matchers.size(); ++i)
         h2_fail::append_y(fail, v_matchers[i].matches(a[i], caseless, dont));

      return fail;
   }

   H2_MATCHER_T2V(t_matchers)
};

const h2_polymorphic_matcher<h2_any_matches> _{h2_any_matches()};
const h2_polymorphic_matcher<h2_any_matches> Any{h2_any_matches()};
const h2_polymorphic_matcher<h2_null_matches> IsNull{h2_null_matches()};
const h2_polymorphic_matcher<h2_null_matches> NotNull{h2_null_matches(true)};
const h2_polymorphic_matcher<h2_boolean_matches> IsTrue{h2_boolean_matches(true)};
const h2_polymorphic_matcher<h2_boolean_matches> IsFalse{h2_boolean_matches(false)};

template <typename E>
inline h2_polymorphic_matcher<h2_equal_matches<E>> Eq(const E expect) { return h2_polymorphic_matcher<h2_equal_matches<E>>(h2_equal_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_equal_matches<E>> Nq(const E expect) { return h2_polymorphic_matcher<h2_equal_matches<E>>(h2_equal_matches<E>(expect, true)); }
template <typename E>
inline h2_polymorphic_matcher<h2_ge_matches<E>> Ge(const E expect) { return h2_polymorphic_matcher<h2_ge_matches<E>>(h2_ge_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_gt_matches<E>> Gt(const E expect) { return h2_polymorphic_matcher<h2_gt_matches<E>>(h2_gt_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_le_matches<E>> Le(const E expect) { return h2_polymorphic_matcher<h2_le_matches<E>>(h2_le_matches<E>(expect)); }
template <typename E>
inline h2_polymorphic_matcher<h2_lt_matches<E>> Lt(const E expect) { return h2_polymorphic_matcher<h2_lt_matches<E>>(h2_lt_matches<E>(expect)); }

inline h2_polymorphic_matcher<h2_memcmp_matches> Me(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(len ? 8 : 0, (const void*)ptr, len * 8)); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M1e(const char* str) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(0, (const void*)str, strlen(str))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M8e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(8, (const void*)ptr, len ? len * 8 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M16e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(16, (const void*)ptr, len ? len * 16 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M32e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(32, (const void*)ptr, len ? len * 32 : strlen((const char*)ptr))); }
inline h2_polymorphic_matcher<h2_memcmp_matches> M64e(const void* ptr, const int len = 0) { return h2_polymorphic_matcher<h2_memcmp_matches>(h2_memcmp_matches(64, (const void*)ptr, len ? len * 64 : strlen((const char*)ptr))); }

inline h2_polymorphic_matcher<h2_regex_matches> Re(const h2_string& regex_pattern) { return h2_polymorphic_matcher<h2_regex_matches>(h2_regex_matches(regex_pattern)); }
inline h2_polymorphic_matcher<h2_wildcard_matches> We(const h2_string& wildcard_pattern) { return h2_polymorphic_matcher<h2_wildcard_matches>(h2_wildcard_matches(wildcard_pattern)); }
inline h2_polymorphic_matcher<h2_strcmp_matches> Se(const h2_string& expect) { return h2_polymorphic_matcher<h2_strcmp_matches>(h2_strcmp_matches(expect)); }
inline h2_polymorphic_matcher<h2_contains_matches> Contains(const h2_string& substring) { return h2_polymorphic_matcher<h2_contains_matches>(h2_contains_matches(substring)); }
inline h2_polymorphic_matcher<h2_startswith_matches> StartsWith(const h2_string& prefix_string) { return h2_polymorphic_matcher<h2_startswith_matches>(h2_startswith_matches(prefix_string)); }
inline h2_polymorphic_matcher<h2_endswith_matches> EndsWith(const h2_string& suffix_string) { return h2_polymorphic_matcher<h2_endswith_matches>(h2_endswith_matches(suffix_string)); }
inline h2_polymorphic_matcher<h2_json_matches> Je(const h2_string& expect) { return h2_polymorphic_matcher<h2_json_matches>(h2_json_matches(expect)); }

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(M m) { return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(h2_matcher<h2_string>(m))); }
#ifndef _WIN32
template <typename M>
inline h2_polymorphic_matcher<h2_caseless_matches> operator~(M m)
{
   return CaseLess(m);
}
#endif
template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher m)
{
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(m));
}
template <typename Matches>
inline h2_polymorphic_matcher<h2_not_matches<h2_polymorphic_matcher<Matches>>> operator!(const h2_polymorphic_matcher<Matches>& m) { return Not(m); }

template <typename... Matchers>
inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> AllOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>>(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> AnyOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>>(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> NoneOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>>(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers)
{
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator&&(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_and_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator&&(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_and_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}

template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>> a(m1, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_polymorphic_matcher<M2>>> b(a);
   return b;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>>
operator||(const h2_polymorphic_matcher<M1>& m1, const M2& m2)
{
   h2_matcher<typename h2_decay<M2>::type> a(m2);
   h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>> b(m1, a);
   h2_polymorphic_matcher<h2_or_matches<h2_polymorphic_matcher<M1>, h2_matcher<typename h2_decay<M2>::type>>> c(b);
   return c;
}
template <typename M1, typename M2>
inline h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>>
operator||(const M1& m1, const h2_polymorphic_matcher<M2>& m2)
{
   h2_matcher<typename h2_decay<M1>::type> a(m1);
   h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>> b(a, m2);
   h2_polymorphic_matcher<h2_or_matches<h2_matcher<typename h2_decay<M1>::type>, h2_polymorphic_matcher<M2>>> c(b);
   return c;
}
