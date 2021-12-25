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

template <typename T, typename Class, typename Data, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_data_matches<E, Class, Data>>>
inline P Member_member(T expect, Data Class::*data) { return P(h2_member_data_matches<E, Class, Data>(expect, data)); }

#if __cplusplus >= 201402L || (defined _MSVC_LANG && _MSVC_LANG >= 201402L)
template <typename Matcher, typename Method, typename... Args>
struct h2_member_method_matches : h2_matches {
   const Matcher m;
   Method method;
   std::tuple<Args...> args;
   explicit h2_member_method_matches(Matcher m_, Method method_, Args... args_) : m(m_), method(method_), args(args_...) {}

   // apply1/2 => std::apply since C++17
   template <typename A, std::size_t... S>
   auto apply2(A& a, h2_index_sequence<S...>) const { return (a.*method)(std::get<S>(args)...); }
   template <typename A>
   auto apply1(A& a) const { return apply2(a, h2_make_index_sequence<sizeof...(Args)>()); }

   template <typename A>
   h2_fail* matches(const A& a, const C& c) const
   {
      auto p = (typename std::add_pointer<typename std::remove_pointer<typename std::decay<A>::type>::type>::type)h2_pointer_if(a);
      return h2_matcher_cast<decltype(apply1(*p))>(m).matches(apply1(*p), c);
   }
   virtual h2_line expection(const C& c) const override { return h2_matches_expection(m, c); }
};

template <typename Class, typename... Args>
struct mem_fn2 {
   template <typename ReturnType> static auto get(ReturnType (Class::*f)(Args...)) { return f; }
   template <typename ReturnType> static auto get(ReturnType (Class::*f)(Args...) const) { return f; }
};  // mem_fn1/2 improve std::mem_fn for overloading
template <typename Class, typename... Args> auto mem_fn1(Args... args) { return mem_fn2<Class, Args...>(); }

template <typename T, typename Method, typename... Args, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, Method, Args...>>>
inline P Member_method(T expect, Method method, Args... args) { return P(h2_member_method_matches<E, Method, Args...>(expect, method, args...)); }

template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)()>>>
inline P Member_member(T expect, ReturnType (Class::*method)()) { return P(h2_member_method_matches<E, ReturnType (Class::*)()>(expect, method)); }
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() const>>>
inline P Member_member(T expect, ReturnType (Class::*method)() const) { return P(h2_member_method_matches<E, ReturnType (Class::*)() const>(expect, method)); }
#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L) /* clang-format off */ // The noexcept-specification is a part of the function type since C++17
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() noexcept>>>
inline P Member_member(T expect, ReturnType (Class::*method)() noexcept) { return P(h2_member_method_matches<E, ReturnType (Class::*)() noexcept>(expect, method)); }
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() const noexcept>>>
inline P Member_member(T expect, ReturnType (Class::*method)() const noexcept) { return P(h2_member_method_matches<E, ReturnType (Class::*)() const noexcept>(expect, method)); }
#endif
#endif

#define H2Member(expect, Class, member, ...) H2PP_CAT2(H2Member, H2PP_IS_EMPTY(__VA_ARGS__)) (expect, Class, member, __VA_ARGS__)
#define H2Member1(expect, Class, member, ...) h2::Member_member(expect, &Class::member)
// #define H2Member0(expect, Class, member, ...) h2::Member_method(expect, decltype(h2::mem_fn1<Class>(__VA_ARGS__))::get(&Class::member), __VA_ARGS__)  // without arguments braced
#define H2Member0(...) H2PP_NCALL(H2Member0, 4, __VA_ARGS__)
#define H2Member00(expect, Class, member, ...) h2::Member_method(expect, decltype(h2::mem_fn1<Class>(__VA_ARGS__))::get(&Class::member), __VA_ARGS__)
#define H2Member01(expect, Class, member, ...) H2PP_CAT2(H2Member01, H2PP_IS_BEGIN_PARENTHESIS(__VA_ARGS__)) (expect, Class, member, __VA_ARGS__)
#define H2Member010(expect, Class, member, ...) h2::Member_method(expect, decltype(h2::mem_fn1<Class>(__VA_ARGS__))::get(&Class::member), __VA_ARGS__)
#define H2Member011(expect, Class, member, ...) H2PP_CAT2(H2Member011, H2PP_IS_EMPTY __VA_ARGS__) (expect, Class, member, H2PP_REMOVE_PARENTHESES(__VA_ARGS__))
#define H2Member0111(expect, Class, member, ...) h2::Member_method(expect, decltype(h2::mem_fn1<Class>())::get(&Class::member))
#define H2Member0110(expect, Class, member, ...) h2::Member_method(expect, decltype(h2::mem_fn1<Class>(__VA_ARGS__))::get(&Class::member), __VA_ARGS__)
