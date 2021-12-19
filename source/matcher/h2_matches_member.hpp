template <typename Matcher, typename Class, typename Data>
struct h2_member_data_matches : h2_matches {
   const Matcher m;
   const Data Class::*data;
   explicit h2_member_data_matches(Matcher m_, Data Class::*data_) : m(m_), data(data_) {}

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<Data>(m).matches(h2_pointer_if(a)->*data, c);
   }
   virtual h2_line expection(const C& c) const override { return h2_matches_expection(m, c); }
};

template <typename Matcher, typename Class, typename Return, typename Method, typename... Args>
struct h2_member_method_matches : h2_matches {
   const Matcher m;
   Method method;
   std::tuple<Args...> args;
   explicit h2_member_method_matches(Matcher m_, Method method_, Args... args_) : m(m_), method(method_), args(args_...) {}

   template <typename A>
   Return do_call(A& a) const { return do_call(a, h2_make_index_sequence<sizeof...(Args)>()); }
   template <typename A, std::size_t... S>
   Return do_call(A& a, h2_index_sequence<S...>) const { return (a.*method)(std::get<S>(args)...); }

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      auto p = (typename std::add_pointer<typename std::remove_pointer<typename std::decay<A>::type>::type>::type)h2_pointer_if(a);
      return h2_matcher_cast<Return>(m).matches(do_call(*p), c);
   }
   virtual h2_line expection(const C& c) const override { return h2_matches_expection(m, c); }
};

template <typename T, typename Class, typename Data, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_data_matches<E, Class, Data>>>
inline P Member(T expect, Data Class::*data) { return P(h2_member_data_matches<E, Class, Data>(expect, data)); }

template <typename T, typename Class, typename Return, typename... Args, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...), Args...>>>
inline P Member(T expect, Return (Class::*method)(Args...), Args... args) { return P(h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...), Args...>(expect, method, args...)); }
template <typename T, typename Class, typename Return, typename... Args, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...) const, Args...>>>
inline P Member(T expect, Return (Class::*method)(Args...) const, Args... args) { return P(h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...) const, Args...>(expect, method, args...)); }

#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)
template <typename T, typename Class, typename Return, typename... Args, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...) noexcept, Args...>>>
inline P Member(T expect, Return (Class::*method)(Args...) noexcept, Args... args) { return P(h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...) noexcept, Args...>(expect, method, args...)); }
template <typename T, typename Class, typename Return, typename... Args, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...) const noexcept, Args...>>>
inline P Member(T expect, Return (Class::*method)(Args...) const noexcept, Args... args) { return P(h2_member_method_matches<E, Class, Return, Return (Class::*)(Args...) const noexcept, Args...>(expect, method, args...)); }
#endif
