
template <typename T>
class h2_matcher_impl {
 public:
   virtual h2_fail* matches(T a, bool caseless = false, bool dont = false) const = 0;
   virtual ~h2_matcher_impl() {}
};

// template <typename T>
// class h2_matcher {
//  private:
//    shared_ptr<const h2_matcher_impl<const T&>> impl_;

//  public:
//    h2_matcher() {}
//    h2_matcher(const h2_matcher_impl<const T&>* _impl, const int placeholder) : impl_(_impl) {}
//    h2_matcher(const h2_matcher&) = default;
//    h2_matcher& operator=(const h2_matcher&) = default;
//    virtual ~h2_matcher() {}
//    h2_fail* matches(const T& a, bool caseless = false, bool dont = false) const { return impl_->matches(a, caseless, dont); }
//    h2_matcher(T value);
// };

template <typename T>
class h2_matcher_base {
 private:
   h2_shared_ptr<const h2_matcher_impl<const T&>> impl_;

 public:
   h2_matcher_base() {}
   explicit h2_matcher_base(const h2_matcher_impl<const T&>* _impl, const int placeholder) : impl_(_impl) {}
   h2_matcher_base(const h2_matcher_base&) = default;
   h2_matcher_base& operator=(const h2_matcher_base&) = default;
   virtual ~h2_matcher_base() {}
   h2_fail* matches(const T& a, bool caseless = false, bool dont = false) const { return impl_->matches(a, caseless, dont); }
};

template <typename T>
class h2_matcher : public h2_matcher_base<T> {
 public:
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const T&>* _impl, const int placeholder) : h2_matcher_base<T>(_impl, placeholder) {}
   h2_matcher(T value);
};

template <>
class h2_matcher<const char*> : public h2_matcher_base<const char*> {
 public:
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const char* const&>* _impl, const int placeholder) : h2_matcher_base<const char*>(_impl, placeholder) {}
   h2_matcher(const std::string& s);
   h2_matcher(const char* s);
};

template <>
class h2_matcher<const std::string&> : public h2_matcher_base<const std::string&> {
 public:
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* _impl, const int placeholder) : h2_matcher_base<const std::string&>(_impl, placeholder) {}
   h2_matcher(const std::string& s);
   h2_matcher(const char* s);
};

template <>
class h2_matcher<std::string> : public h2_matcher_base<std::string> {
 public:
   h2_matcher() {}
   explicit h2_matcher(const h2_matcher_impl<const std::string&>* _impl, const int placeholder) : h2_matcher_base<std::string>(_impl, placeholder) {}
   h2_matcher(const std::string& s);
   h2_matcher(const char* s);
};

template <typename Matches>
class h2_polymorphic_matcher {
 public:
   explicit h2_polymorphic_matcher(const Matches& _matches) : matches_(_matches) {}

   template <typename T>
   operator h2_matcher<T>() const { return h2_matcher<T>(new internal_impl<const T&>(matches_), 0); }

 private:
   template <typename T>
   struct internal_impl : public h2_matcher_impl<T> {
      const Matches matches_;
      explicit internal_impl(const Matches& _matches) : matches_(_matches) {}
      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override { return matches_.matches(a, caseless, dont); }
      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }
   };

   Matches matches_;
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
         if (e.length() < 35 && a.length() < 35) {
            h2_fail_unexpect* fail = new h2_fail_unexpect();
            fail->eprintf("\"%s\"", e.c_str());
            fail->aprintf("\"%s\"", a.c_str());
            fail->mprintf("not %sequal to", caseless ? "caseless " : "");
            return fail;
         } else {
            h2_fail_strcmp* fail = new h2_fail_strcmp();
            fail->add(e.c_str(), a.c_str(), caseless);
            return fail;
         }
      }
   }
};

struct h2_float_eq_matches {
   const long double e;
   explicit h2_float_eq_matches(const long double _e) : e(_e) {}

   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const {
#if 0
      // the machine epsilon has to be scaled to the magnitude of the values used
      // and multiplied by the desired precision in ULPs (units in the last place)
      bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
            // unless the result is subnormal
            || std::fabs(a - e) < std::numeric_limits<double>::min();
#endif
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
#ifdef __GLIBC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wsign-compare"
#endif
      if ((a == e) == !dont) return nullptr;
#ifdef __GLIBC__
#   pragma GCC diagnostic pop
#endif
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
class MatcherCastImpl {
 public:
   static h2_matcher<T> Cast(const M& from) {
      return CastImpl(from, std::is_convertible<M, h2_matcher<T>>{}, std::is_convertible<M, T>{});
   }

 private:
   template <bool Ignore>
   static h2_matcher<T> CastImpl(const M& from, std::true_type, std::integral_constant<bool, Ignore>) {
      return from;
   }

   template <typename To>
   static To ImplicitCast_(To x) { return x; }

   static h2_matcher<T> CastImpl(const M& from, std::false_type, std::true_type) {
      return h2_matcher<T>(ImplicitCast_<T>(from));
   }

   static h2_matcher<T> CastImpl(const M& from, std::false_type, std::false_type) {
      return h2_polymorphic_matcher<h2_eq_matches<M>>(h2_eq_matches<M>(from));
   }
};

template <typename T, typename U>
class MatcherCastImpl<T, h2_matcher<U>> {
 public:
   static h2_matcher<T> Cast(const h2_matcher<U>& from) { return h2_matcher<T>(new Impl(from)); }

 private:
   class Impl : public h2_matcher_impl<T> {
    public:
      explicit Impl(const h2_matcher<U>& from) : _from(from) {}

      // delegate the matching logic to the source h2_matcher.
      h2_fail* matches(T x, bool caseless, bool dont) const override {
         using FromType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>::type;
         using ToType = typename std::remove_cv<typename std::remove_pointer<typename std::remove_reference<U>::type>::type>::type;
         // Do not allow implicitly converting base*/& to derived*/&.
         static_assert(
           // Do not trigger if only one of them is a pointer. That implies a
           // regular conversion and not a down_cast.
           (std::is_pointer<typename std::remove_reference<T>::type>::value !=
            std::is_pointer<typename std::remove_reference<U>::type>::value) ||
             std::is_same<FromType, ToType>::value || !std::is_base_of<FromType, ToType>::value,
           "Can't implicitly convert from <base> to <derived>");

         return _from.matches(static_cast<U>(x), caseless, dont);
      }

      static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
      static void operator delete(void* ptr) { h2_raw::free(ptr); }

    private:
      const h2_matcher<U> _from;

      void operator=(Impl const&) = delete;
   };
};

template <typename T>
class MatcherCastImpl<T, h2_matcher<T>> {
 public:
   static h2_matcher<T> Cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> MatcherCast(const M& from) { return MatcherCastImpl<T, M>::Cast(from); }

struct h2_any_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { return nullptr; }
};

struct h2_null_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      if ((nullptr == reinterpret_cast<const void*>(a)) == !dont) return nullptr;
      h2_fail_normal* fail = new h2_fail_normal();
      if (dont)
         fail->kprintf("shoud not be null");
      else
         fail->kprintf("is not null %p", reinterpret_cast<const void*>(a));

      return fail;
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
      h2_fail_memcmp* fail = new h2_fail_memcmp();
      fail->add(e, a, sz);
      return fail;
   }
};

template <typename Matcher>
struct h2_pe_matches {
   const Matcher m;
   explicit h2_pe_matches(Matcher _matcher) : m(_matcher) {}

   template <typename SmartPointer>
   struct PointeeOf { typedef typename SmartPointer::element_type type; };
   template <typename T>
   struct PointeeOf<T*> { typedef T type; };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;

      return MatcherCast<Pointee>(m).matches(*a, caseless, dont);
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
   const h2_string substring_;
   explicit h2_contains_matches(const h2_string& substring) : substring_(substring) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.contains(substring_, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", substring_.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %shas substr", caseless ? "caseless " : "");
      else
         fail->mprintf("not %shas substr", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_startswith_matches {
   const h2_string prefix_string_;
   explicit h2_startswith_matches(const h2_string& prefix_string) : prefix_string_(prefix_string) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.startswith(prefix_string_, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", prefix_string_.c_str());
      fail->aprintf("\"%s\"", a.c_str());
      if (dont)
         fail->mprintf("shoud not %sstarts with", caseless ? "caseless " : "");
      else
         fail->mprintf("not %sstarts with", caseless ? "caseless " : "");

      return fail;
   }
};

struct h2_endswith_matches {
   const h2_string suffix_string_;
   explicit h2_endswith_matches(const h2_string& suffix_string) : suffix_string_(suffix_string) {}

   h2_fail* matches(const h2_string& a, bool caseless = false, bool dont = false) const {
      if (a.endswith(suffix_string_, caseless) == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", suffix_string_.c_str());
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
      h2_fail_json* fail = new h2_fail_json();
      if (dont)
         fail->kprintf("JSON should not equals");
      else
         fail->kprintf("JSON not equals");

      fail->add(e.c_str(), a.c_str());
      return fail;
   }
};

struct h2_caseless_matches {
   const h2_matcher<h2_string> m;
   explicit h2_caseless_matches(h2_matcher<h2_string> _matcher) : m(_matcher) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      return m.matches(a, true, dont);
   }
};

template <typename Matcher>
struct h2_not_matches {
   const Matcher m;
   explicit h2_not_matches(Matcher _matcher) : m(_matcher) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const {
      return MatcherCast<A>(m).matches(a, caseless, !dont);
   }
};

template <typename... Matchers>
class h2_allof_matches {
 public:
   explicit h2_allof_matches(const Matchers&... matchers) : matchers_(matchers...) {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one h2_matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      h2_vector<h2_matcher<A>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (auto i = matchers.begin(); i != matchers.end(); i++)
         h2_append_y_fail(fail, (*i).matches(a, caseless, false));

      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>* matchers, std::integral_constant<size_t, I>) const {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_anyof_matches {
 public:
   explicit h2_anyof_matches(const Matchers&... matchers) : matchers_(matchers...) {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one h2_matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      h2_vector<h2_matcher<A>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      for (auto i = matchers.begin(); i != matchers.end(); i++) {
         h2_fail* fail = (*i).matches(a, caseless, false);
         if (!fail) return nullptr;
      }
      h2_ostringstream osa;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("\"%s\"", osa.str().c_str());
      if (dont) {
         //TODO
      } else {
         fail->mprintf("not matches any of");
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>* matchers, std::integral_constant<size_t, I>) const {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_noneof_matches {
 public:
   explicit h2_noneof_matches(const Matchers&... matchers) : matchers_(matchers...) {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one h2_matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      h2_vector<h2_matcher<A>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (auto i = matchers.begin(); i != matchers.end(); i++)
         h2_append_y_fail(fail, (*i).matches(a, caseless, true));

      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>* matchers, std::integral_constant<size_t, I>) const {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_listof_matches {
 public:
   explicit h2_listof_matches(const Matchers&... matchers) : matchers_(matchers...) {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one h2_matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const {
      typedef decltype(a[0]) B;
      h2_vector<h2_matcher<B>> matchers;
      tuple_to_vector_matcher<B>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (size_t i = 0; i < matchers.size(); ++i)
         h2_append_y_fail(fail, matchers[i].matches(a[i], caseless, false));

      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>* matchers, std::integral_constant<size_t, I>) const {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(h2_vector<h2_matcher<T>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

const h2_polymorphic_matcher<h2_any_matches> _{h2_any_matches()};

inline h2_polymorphic_matcher<h2_any_matches> Any() {
   return h2_polymorphic_matcher<h2_any_matches>(h2_any_matches());
}
inline h2_polymorphic_matcher<h2_null_matches> Null() {
   return h2_polymorphic_matcher<h2_null_matches>(h2_null_matches());
}
template <typename E>
inline h2_polymorphic_matcher<h2_eq_matches<E>> Eq(const E expect) {
   return h2_polymorphic_matcher<h2_eq_matches<E>>(h2_eq_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_ge_matches<E>> Ge(const E expect) {
   return h2_polymorphic_matcher<h2_ge_matches<E>>(h2_ge_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_gt_matches<E>> Gt(const E expect) {
   return h2_polymorphic_matcher<h2_gt_matches<E>>(h2_gt_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_le_matches<E>> Le(const E expect) {
   return h2_polymorphic_matcher<h2_le_matches<E>>(h2_le_matches<E>(expect));
}
template <typename E>
inline h2_polymorphic_matcher<h2_lt_matches<E>> Lt(const E expect) {
   return h2_polymorphic_matcher<h2_lt_matches<E>>(h2_lt_matches<E>(expect));
}
inline h2_polymorphic_matcher<h2_me_matches> Me(const void* buf, const int size = 0) {
   return h2_polymorphic_matcher<h2_me_matches>(h2_me_matches(buf, size));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_pe_matches<Matcher>> Pe(Matcher expect) {
   return h2_polymorphic_matcher<h2_pe_matches<Matcher>>(h2_pe_matches<Matcher>(expect));
}
inline h2_polymorphic_matcher<h2_regex_matches> Re(const h2_string& regex_pattern) {
   return h2_polymorphic_matcher<h2_regex_matches>(h2_regex_matches(regex_pattern));
}
inline h2_polymorphic_matcher<h2_wildcard_matches> We(const h2_string& wildcard_pattern) {
   return h2_polymorphic_matcher<h2_wildcard_matches>(h2_wildcard_matches(wildcard_pattern));
}
inline h2_polymorphic_matcher<h2_contains_matches> Contains(const h2_string& substring) {
   return h2_polymorphic_matcher<h2_contains_matches>(h2_contains_matches(substring));
}
inline h2_polymorphic_matcher<h2_startswith_matches> StartsWith(const h2_string& prefix_string) {
   return h2_polymorphic_matcher<h2_startswith_matches>(h2_startswith_matches(prefix_string));
}
inline h2_polymorphic_matcher<h2_endswith_matches> EndsWith(const h2_string& suffix_string) {
   return h2_polymorphic_matcher<h2_endswith_matches>(h2_endswith_matches(suffix_string));
}
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(h2_matcher<h2_string> expect) {
   return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(expect));
}
inline h2_polymorphic_matcher<h2_caseless_matches> CaseLess(h2_string expect) {
   return h2_polymorphic_matcher<h2_caseless_matches>(h2_caseless_matches(expect));
}
inline h2_polymorphic_matcher<h2_je_matches> Je(const h2_string& expect) {
   return h2_polymorphic_matcher<h2_je_matches>(h2_je_matches(expect));
}
template <typename Matcher>
inline h2_polymorphic_matcher<h2_not_matches<Matcher>> Not(Matcher expect) {
   return h2_polymorphic_matcher<h2_not_matches<Matcher>>(h2_not_matches<Matcher>(expect));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> AllOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>>(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> AnyOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>>(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> NoneOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>>(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
template <typename... Matchers>
inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> ListOf(const Matchers&... matchers) {
   return h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>>(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...));
}
