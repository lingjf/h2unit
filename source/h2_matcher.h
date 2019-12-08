
#ifndef ___H2_MATCHER__H___
#define ___H2_MATCHER__H___

template <typename T>
class h2_matcher_mpl
{
 public:
   virtual h2_fail* matches(T a, bool caseless = false, bool dont = false) const = 0;

   virtual ~h2_matcher_mpl() {}
};

template <typename T>
class h2_matcher
{
 private:
   const h2_matcher_mpl<const T&>* impl_;

 public:
   explicit h2_matcher() {}

   h2_matcher(const h2_matcher_mpl<const T&>* _impl, const int placeholder)
   {
      impl_ = _impl;
   }

   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   h2_matcher(h2_matcher&&) = default;
   h2_matcher& operator=(h2_matcher&&) = default;

   virtual ~h2_matcher() {}

 public:
   h2_fail* matches(const T& a, bool caseless = false, bool dont = false) const
   {
      return impl_->matches(a, caseless, dont);
   }

   h2_matcher(T value);
   h2_matcher(std::function<bool(T)> f);
};

template <typename T, typename F>
class h2_lambda_matcher : public h2_matcher_mpl<T>
{
 private:
   F f_;

 public:
   h2_lambda_matcher(F f)
     : f_(f) {}
   h2_fail* matches(T a, bool caseless = false, bool dont = false) const
   {
      bool result = f_(a);
      if (result) {
         return nullptr;
      }
      h2_fail_normal* fail = new h2_fail_normal();
      fail->kprintf("lambda failed");
      return fail;
   }

   static void* operator new(std::size_t sz)
   {
      return h2_alloc::I().malloc(sz);
   }
   static void operator delete(void* ptr)
   {
      h2_alloc::I().free(ptr);
   }
};

template <typename T>
inline h2_matcher<T>::h2_matcher(std::function<bool(T)> f)
{
   *this = h2_matcher<T>(new h2_lambda_matcher<const T&, std::function<bool(T)>>(f), 0);
}

template <typename Matches>
class h2_polymorphic_matcher
{
 public:
   explicit h2_polymorphic_matcher(const Matches& _matches)
     : matches_(_matches) {}

   template <typename T>
   operator h2_matcher<T>() const
   {
      return h2_matcher<T>(new internal_impl<const T&>(matches_), 0);
   }

 private:
   template <typename T>
   class internal_impl : public h2_matcher_mpl<T>
   {
    public:
      explicit internal_impl(const Matches& _matches)
        : matches_(_matches) {}

      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override
      {
         return matches_.matches(a, caseless, dont);
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }

    private:
      const Matches matches_;
   };

   Matches matches_;
};

template <typename Matches>
inline h2_polymorphic_matcher<Matches> MakePolymorphicMatcher(const Matches& matches)
{
   return h2_polymorphic_matcher<Matches>(matches);
}

template <typename E>
class h2_eq_matches
{
 public:
   explicit h2_eq_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a == e_;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(const float a, bool caseless = false, bool dont = false) const
   {
      bool result = std::fabs(a - e_) < 0.00001;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(const double a, bool caseless = false, bool dont = false) const
   {
      bool result = std::fabs(a - e_) < 0.00001;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const
   {
      bool result = std::fabs(a - e_) < 0.00001;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(char* a, bool caseless = false, bool dont = false) const
   {
      return matches(h2_string(a), caseless, dont);
   }
   // template <>
   h2_fail* matches(const char* a, bool caseless = false, bool dont = false) const
   {
      return matches(h2_string(a), caseless, dont);
   }
   // template <>
   h2_fail* matches(const h2_string a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(e_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }
      bool result = e2 == a2;
      if (result == !dont) return nullptr;
      if (dont) {
         h2_fail_unexpect* fail = new h2_fail_unexpect();
         fail->eprintf("\"%s\"", h2_acronym_string(h2_string(e_).c_str()));
         fail->aprintf("\"%s\"", h2_acronym_string(a.c_str()));
         fail->mprintf("should not %sequal to", caseless ? "caseless " : "");
         return fail;
      }
      else {
         if (h2_string(e_).length() < 35 && a.length() < 35) {
            h2_fail_unexpect* fail = new h2_fail_unexpect();
            fail->eprintf("\"%s\"", h2_string(e_).c_str());
            fail->aprintf("\"%s\"", a.c_str());
            fail->mprintf("not %sequal to", caseless ? "caseless " : "");
            return fail;
         }
         else {
            h2_fail_strcmp* fail = new h2_fail_strcmp();
            fail->add(e_, a, caseless);
            return fail;
         }
      }
   }

   template <typename A>
   h2_fail* desc(A a, bool dont) const
   {
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      h2_ostringstream ose, osa;
      ose << std::boolalpha << e_;
      osa << std::boolalpha << a;
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("should not equal to");
      }
      else {
         fail->mprintf("not equal to");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename T>
inline h2_matcher<T>::h2_matcher(T value)
{
   *this = MakePolymorphicMatcher(h2_eq_matches<T>(value));
}

// This general version is used when MatcherCast()'s argument is a
// polymorphic matcher (i.e. something that can be converted to a
// Matcher but is not one yet; for example, Eq(value)) or a value (for
// example, "hello").
template <typename T, typename M>
class MatcherCastImpl
{
 public:
   static h2_matcher<T> Cast(const M& polymorphic_matcher_or_value)
   {
      // M can be a polymorphic matcher, in which case we want to use
      // its conversion operator to create Matcher<T>.  Or it can be a value
      // that should be passed to the Matcher<T>'s constructor.
      //
      // We can't call Matcher<T>(polymorphic_matcher_or_value) when M is a
      // polymorphic matcher because it'll be ambiguous if T has an implicit
      // constructor from M (this usually happens when T has an implicit
      // constructor from any type).
      //
      // It won't work to unconditionally implict_cast
      // polymorphic_matcher_or_value to Matcher<T> because it won't trigger
      // a user-defined conversion from M to T if one exists (assuming M is
      // a value).
      return CastImpl(polymorphic_matcher_or_value,
                      std::is_convertible<M, h2_matcher<T>>{},
                      std::is_convertible<M, T>{});
   }

 private:
   template <bool Ignore>
   static h2_matcher<T> CastImpl(const M& polymorphic_matcher_or_value,
                                 std::true_type /* convertible_to_matcher */,
                                 std::integral_constant<bool, Ignore>)
   {
      // M is implicitly convertible to Matcher<T>, which means that either
      // M is a polymorphic matcher or Matcher<T> has an implicit constructor
      // from M.  In both cases using the implicit conversion will produce a
      // matcher.
      //
      // Even if T has an implicit constructor from M, it won't be called because
      // creating Matcher<T> would require a chain of two user-defined conversions
      // (first to create T from M and then to create Matcher<T> from T).
      return polymorphic_matcher_or_value;
   }

   template <typename To>
   static To ImplicitCast_(To x)
   {
      return x;
   }

   // M can't be implicitly converted to Matcher<T>, so M isn't a polymorphic
   // matcher. It's a value of a type implicitly convertible to T. Use direct
   // initialization to create a matcher.
   static h2_matcher<T> CastImpl(const M& value,
                                 std::false_type /* convertible_to_matcher */,
                                 std::true_type /* convertible_to_T */)
   {
      return h2_matcher<T>(ImplicitCast_<T>(value));
   }

   // M can't be implicitly converted to either Matcher<T> or T. Attempt to use
   // polymorphic matcher Eq(value) in this case.
   //
   // Note that we first attempt to perform an implicit cast on the value and
   // only fall back to the polymorphic Eq() matcher afterwards because the
   // latter calls bool operator==(const Lhs& lhs, const Rhs& rhs) in the end
   // which might be undefined even when Rhs is implicitly convertible to Lhs
   // (e.g. std::pair<const int, int> vs. std::pair<int, int>).
   //
   static h2_matcher<T> CastImpl(const M& value,
                                 std::false_type /* convertible_to_matcher */,
                                 std::false_type /* convertible_to_T */)
   {
      return MakePolymorphicMatcher(h2_eq_matches<M>(value));
   }
};

// This more specialized version is used when MatcherCast()'s argument
// is already a Matcher.  This only compiles when type T can be
// statically converted to type U.
template <typename T, typename U>
class MatcherCastImpl<T, h2_matcher<U>>
{
 public:
   static h2_matcher<T> Cast(const h2_matcher<U>& source_matcher)
   {
      return h2_matcher<T>(new Impl(source_matcher));
   }

 private:
   class Impl : public h2_matcher_mpl<T>
   {
    public:
      explicit Impl(const h2_matcher<U>& source_matcher)
        : source_matcher_(source_matcher) {}

      // We delegate the matching logic to the source matcher.
      h2_fail* matches(T x, bool caseless, bool dont) const override
      {
         using FromType = typename std::remove_cv<typename std::remove_pointer<
           typename std::remove_reference<T>::type>::type>::type;
         using ToType = typename std::remove_cv<typename std::remove_pointer<
           typename std::remove_reference<U>::type>::type>::type;
         // Do not allow implicitly converting base*/& to derived*/&.
         static_assert(
           // Do not trigger if only one of them is a pointer. That implies a
           // regular conversion and not a down_cast.
           (std::is_pointer<typename std::remove_reference<T>::type>::value !=
            std::is_pointer<typename std::remove_reference<U>::type>::value) ||
             std::is_same<FromType, ToType>::value ||
             !std::is_base_of<FromType, ToType>::value,
           "Can't implicitly convert from <base> to <derived>");

         return source_matcher_.matches(static_cast<U>(x), caseless, dont);
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }

    private:
      const h2_matcher<U> source_matcher_;

      void operator=(Impl const&) = delete;
   };
};

// This even more specialized version is used for efficiently casting
// a matcher to its own type.
template <typename T>
class MatcherCastImpl<T, h2_matcher<T>>
{
 public:
   static h2_matcher<T> Cast(const h2_matcher<T>& matcher)
   {
      return matcher;
   }
};

// In order to be safe and clear, casting between different matcher
// types is done explicitly via MatcherCast<T>(m), which takes a
// matcher m and returns a Matcher<T>.  It compiles only when T can be
// statically converted to the argument type of m.
template <typename T, typename M>
inline h2_matcher<T> MatcherCast(const M& matcher)
{
   return MatcherCastImpl<T, M>::Cast(matcher);
}

struct h2_any_matches
{
   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      return nullptr;
   }
};

struct h2_null_matches
{
   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = nullptr == reinterpret_cast<const void*>(a);
      if (result == !dont) return nullptr;
      h2_fail_normal* fail = new h2_fail_normal();
      if (dont) {
         fail->kprintf("shoud not be null");
      }
      else {
         fail->kprintf("is not null %p", reinterpret_cast<const void*>(a));
      }
      return fail;
   }
};

template <typename E>
class h2_ge_matches
{
 public:
   explicit h2_ge_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a >= e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud >=");
      }
      else {
         fail->mprintf("not >=");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename E>
class h2_gt_matches
{
 public:
   explicit h2_gt_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a > e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud >");
      }
      else {
         fail->mprintf("not >");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename E>
class h2_le_matches
{
 public:
   explicit h2_le_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a <= e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud <=");
      }
      else {
         fail->mprintf("not <=");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename E>
class h2_lt_matches
{
 public:
   explicit h2_lt_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a < e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud <");
      }
      else {
         fail->mprintf("not <");
      }
      return fail;
   }

 private:
   E e_;
};

class h2_me_matches
{
 public:
   explicit h2_me_matches(const void* e, const int size)
     : e_(e), size_(size)
   {
      if (size == 0) {
         size_ = strlen((const char*)e);
      }
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = memcmp(e_, (const void*)a, size_) == 0;
      if (result == !dont) return nullptr;
      h2_fail_memcmp* fail = new h2_fail_memcmp();
      fail->add(e_, (const void*)a, size_);
      return fail;
   }

 private:
   const void* e_;
   int size_;
};

template <typename M>
class h2_pe_matches
{
 public:
   explicit h2_pe_matches(M matcher)
     : inner_matcher_(matcher) {}

   // PointeeOf<Pointer>::type is the type of a value pointed to by a
   // Pointer, which can be either a smart pointer or a raw pointer.
   template <typename Pointer>
   struct PointeeOf
   {
      // Smart pointer classes define type element_type as the type of
      // their pointees.
      typedef typename Pointer::element_type type;
   };
   // This specialization is for the raw pointer case.
   template <typename T>
   struct PointeeOf<T*>
   {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;

      return MatcherCast<Pointee>(inner_matcher_).matches(*a, caseless, dont);
   }

 private:
   M inner_matcher_;
};

class h2_regex_matches
{
 public:
   explicit h2_regex_matches(const h2_string& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a);
      bool result;
      try {
         std::regex re(e_);
         result = std::regex_match(a2, re);
      }
      catch (const std::regex_error& e) {
         result = false;
      }
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e_.c_str());
      fail->aprintf("\"%s\"", a2.c_str());
      if (dont) {
         fail->mprintf("shoud matches Regex");
      }
      else {
         fail->mprintf("not matches Regex");
      }
      return fail;
   }

 private:
   const h2_string e_;
};

class h2_wildcard_matches
{
 public:
   explicit h2_wildcard_matches(const h2_string& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a);
      bool result = h2_wildcard_match(e_.c_str(), a2.c_str());
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e_.c_str());
      fail->aprintf("\"%s\"", a2.c_str());
      if (dont) {
         fail->mprintf("shoud matches Wildcard");
      }
      else {
         fail->mprintf("not matches Wildcard");
      }
      return fail;
   }

 private:
   const h2_string e_;
};

class h2_hassubstr_matches
{
 public:
   explicit h2_hassubstr_matches(const h2_string& substring)
     : substring_(substring) {}

   // Accepts pointer types, particularly:
   //   const char*
   //   char*
   //   const wchar_t*
   //   wchar_t*

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(substring_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }
      bool result = a2.find(e2) != h2_string::npos;
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", substring_.c_str());
      fail->aprintf("\"%s\"", h2_string(a).c_str());
      if (dont) {
         fail->mprintf("shoud not %shas substr", caseless ? "caseless " : "");
      }
      else {
         fail->mprintf("not %shas substr", caseless ? "caseless " : "");
      }
      return fail;
   }

 private:
   const h2_string substring_;
};

class h2_startswith_matches
{
 public:
   explicit h2_startswith_matches(const h2_string& prefix_string)
     : prefix_string_(prefix_string) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(prefix_string_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }

      bool result = a2.length() >= e2.length() && a2.substr(0, e2.length()) == e2;
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", prefix_string_.c_str());
      fail->aprintf("\"%s\"", h2_string(a).c_str());
      if (dont) {
         fail->mprintf("shoud not %sstarts with", caseless ? "caseless " : "");
      }
      else {
         fail->mprintf("not %sstarts with", caseless ? "caseless " : "");
      }
      return fail;
   }

 private:
   const h2_string prefix_string_;
};

class h2_endswith_matches
{
 public:
   explicit h2_endswith_matches(const h2_string& suffix_string)
     : suffix_string_(suffix_string) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(suffix_string_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }
      bool result = a2.length() >= e2.length() && a2.substr(a2.length() - e2.length()) == e2;
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", suffix_string_.c_str());
      fail->aprintf("\"%s\"", h2_string(a).c_str());
      if (dont) {
         fail->mprintf("shoud not %sends with", caseless ? "caseless " : "");
      }
      else {
         fail->mprintf("not %sends with", caseless ? "caseless " : "");
      }
      return fail;
   }

 private:
   const h2_string suffix_string_;
};

class h2_je_matches
{
 public:
   explicit h2_je_matches(const h2_string& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = h2json::match(e_.c_str(), (const char*)a);
      if (result == !dont) return nullptr;
      h2_fail_json* fail = new h2_fail_json();
      if (dont) {
         fail->kprintf("JSON should not equals");
      }
      else {
         fail->kprintf("JSON not equals");
      }
      fail->add(e_.c_str(), (const char*)a);
      return fail;
   }

 private:
   const h2_string e_;
};

template <typename M>
class h2_caseless_matches
{
 public:
   explicit h2_caseless_matches(M matcher)
     : inner_matcher_(matcher) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      return MatcherCast<const h2_string>(inner_matcher_).matches(a, true, dont);
   }

 private:
   M inner_matcher_;
};

template <typename M>
class h2_not_matches
{
 public:
   explicit h2_not_matches(M matcher)
     : inner_matcher_(matcher) {}
   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      return MatcherCast<const A&>(inner_matcher_).matches(a, caseless, !dont);
   }

 private:
   M inner_matcher_;
};

template <typename... Matchers>
class h2_allof_matches
{
 public:
   explicit h2_allof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      std::vector<h2_matcher<A>, h2_allocator<h2_matcher<A>>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (auto i = matchers.begin(); i != matchers.end(); i++) {
         auto m = *i;
         auto f = m.matches(a, caseless, false);
         if (f) {
            if (!fail) {
               fail = f;
            }
            else {
               fail->appendy(f);
            }
         }
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_anyof_matches
{
 public:
   explicit h2_anyof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      std::vector<h2_matcher<A>, h2_allocator<h2_matcher<A>>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      for (auto i = matchers.begin(); i != matchers.end(); i++) {
         auto m = *i;
         auto f = m.matches(a, caseless, false);
         if (!f) {
            return nullptr;
         }
      }
      h2_ostringstream osa;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("\"%s\"", osa.str().c_str());
      if (dont) {
         //TODO
      }
      else {
         fail->mprintf("not matches any of");
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_noneof_matches
{
 public:
   explicit h2_noneof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      std::vector<h2_matcher<A>, h2_allocator<h2_matcher<A>>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (auto i = matchers.begin(); i != matchers.end(); i++) {
         auto m = *i;
         auto f = m.matches(a, caseless, true);
         if (f) {
            if (!fail) {
               fail = f;
            }
            else {
               fail->appendy(f);
            }
         }
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_listof_matches
{
 public:
   explicit h2_listof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      typedef decltype(a[0]) B;
      std::vector<h2_matcher<B>, h2_allocator<h2_matcher<B>>> matchers;
      tuple_to_vector_matcher<B>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (int i = 0; i < (int)matchers.size(); ++i) {
         auto f = matchers[i].matches(a[i], caseless, false);
         if (f) {
            if (!fail) {
               fail = f;
            }
            else {
               fail->appendy(f);
            }
         }
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

/* clang-format off */

            const h2_polymorphic_matcher<h2_any_matches> 
_                                   { h2_any_matches() };
            inline h2_polymorphic_matcher<h2_any_matches> 
Any()                               { return MakePolymorphicMatcher(h2_any_matches()); }
            inline h2_polymorphic_matcher<h2_null_matches> 
Null()                              { return MakePolymorphicMatcher(h2_null_matches()); }
            template <typename E> inline h2_polymorphic_matcher<h2_eq_matches<E>> 
Eq(const E expect)                  { return MakePolymorphicMatcher(h2_eq_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_ge_matches<E>> 
Ge(const E expect)                  { return MakePolymorphicMatcher(h2_ge_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_gt_matches<E>> 
Gt(const E expect)                  { return MakePolymorphicMatcher(h2_gt_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_le_matches<E>> 
Le(const E expect)                  { return MakePolymorphicMatcher(h2_le_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_lt_matches<E>> 
Lt(const E expect)                  { return MakePolymorphicMatcher(h2_lt_matches<E>(expect)); }
            inline h2_polymorphic_matcher<h2_me_matches> 
Me(const void *buf, const int size = 0)   { return MakePolymorphicMatcher(h2_me_matches(buf, size)); }
            template <typename Matcher_or_Value> inline h2_polymorphic_matcher<h2_pe_matches<Matcher_or_Value>> 
Pe(Matcher_or_Value expect)               { return MakePolymorphicMatcher(h2_pe_matches<Matcher_or_Value>(expect)); }
            template <typename Matcher_or_Value> inline h2_polymorphic_matcher<h2_pe_matches<Matcher_or_Value>> 
PointeeEq(Matcher_or_Value expect)        { return MakePolymorphicMatcher(h2_pe_matches<Matcher_or_Value>(expect)); }
            inline h2_polymorphic_matcher<h2_regex_matches> 
Re(const h2_string& regex_pattern)        { return MakePolymorphicMatcher(h2_regex_matches(regex_pattern)); }
            inline h2_polymorphic_matcher<h2_wildcard_matches> 
We(const h2_string& wildcard_pattern)     { return MakePolymorphicMatcher(h2_wildcard_matches(wildcard_pattern)); }
            inline h2_polymorphic_matcher<h2_hassubstr_matches> 
HasSubstr(const h2_string& substring)     { return MakePolymorphicMatcher(h2_hassubstr_matches(substring)); }
            inline h2_polymorphic_matcher<h2_startswith_matches> 
StartsWith(const h2_string& prefix_string)  { return MakePolymorphicMatcher(h2_startswith_matches(prefix_string)); }
            inline h2_polymorphic_matcher<h2_endswith_matches> 
EndsWith(const h2_string& suffix_string)    { return MakePolymorphicMatcher(h2_endswith_matches(suffix_string)); }
            template <typename Matcher_or_String> inline h2_polymorphic_matcher<h2_caseless_matches<Matcher_or_String>> 
CaseLess(Matcher_or_String expect)          { return MakePolymorphicMatcher(h2_caseless_matches<Matcher_or_String>(expect)); }
            inline h2_polymorphic_matcher<h2_je_matches> 
JsonEq(const h2_string& expect)   { return MakePolymorphicMatcher(h2_je_matches(expect)); }
            inline h2_polymorphic_matcher<h2_je_matches> 
Je(const h2_string& expect)       { return MakePolymorphicMatcher(h2_je_matches(expect)); }
            template <typename Matcher_or_Value> inline h2_polymorphic_matcher<h2_not_matches<Matcher_or_Value>> 
Not(Matcher_or_Value matcher)     { return MakePolymorphicMatcher(h2_not_matches<Matcher_or_Value>(matcher)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> 
AllOf(const Matchers&... matchers)    { return MakePolymorphicMatcher(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> 
AnyOf(const Matchers&... matchers)    { return MakePolymorphicMatcher(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> 
NoneOf(const Matchers&... matchers)   { return MakePolymorphicMatcher(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> 
ListOf(const Matchers&... matchers)   { return MakePolymorphicMatcher(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }

/* clang-format on */

#endif
