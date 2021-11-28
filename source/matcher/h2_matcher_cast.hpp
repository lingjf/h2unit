template <typename T, typename M>
struct h2_matcher_cast_impl {
   static h2_matcher<T> cast(const M& from)
   {
      return do_cast(from,
                     std::integral_constant < bool, std::is_same<bool, M>::value || std::is_same<std::nullptr_t, M>::value > {},
                     std::is_convertible<M, h2_matcher<T>>{} /* h2_matcher::h2_matcher(T value) Converting constructor | h2_polymorphic_matcher::operator h2_matcher<T>() */,
                     std::is_convertible<M, T>{});
   }

   template <bool Ignore1, bool Ignore2>
   static h2_matcher<T> do_cast(const M& from, std::true_type, std::integral_constant<bool, Ignore1>, std::integral_constant<bool, Ignore2>) { return _Eq(from); }

   template <bool Ignore>
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::true_type, std::integral_constant<bool, Ignore>) { return from; }

   template <typename To>
   static To implicit_cast(To x) { return x; }
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type, std::true_type) { return h2_matcher<T>(implicit_cast<T>(from)); }
   static h2_matcher<T> do_cast(const M& from, std::false_type, std::false_type, std::false_type) { return _Eq(from); }
};

template <typename T, typename U>
struct h2_matcher_cast_impl<T, h2_matcher<U>> {
   static h2_matcher<T> cast(const h2_matcher<U>& from) { return h2_matcher<T>(new internal_impl(from), 0); }

   struct internal_impl : h2_matcher_impl<const T&>, h2_libc {
      const h2_matcher<U> from;
      explicit internal_impl(const h2_matcher<U>& from_) : from(from_) {}
      h2_fail* matches(const T& a, h2_mc c) const override { return from.matches(static_cast<U>(a), c); }
      void operator=(internal_impl const&) = delete;
   };
};

template <typename T>
struct h2_matcher_cast_impl<T, h2_matcher<T>> {
   static h2_matcher<T> cast(const h2_matcher<T>& from) { return from; }
};

template <typename T, typename M>
inline h2_matcher<T> h2_matcher_cast(const M& from) { return h2_matcher_cast_impl<T, M>::cast(from); }
