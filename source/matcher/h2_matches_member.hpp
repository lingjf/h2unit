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

#if __cplusplus >= 201402L || (defined _MSVC_LANG && _MSVC_LANG >= 201402L)  // Return type deduction / auto
// h2_apply => std::apply since C++17
template <typename A, typename M, typename T, std::size_t... S>
static inline auto h2_apply(A& a, M method, T args, h2_index_sequence<S...>) { return (a.*method)(std::get<S>(args)...); }
template <typename A, typename M, typename... Args>
static inline auto h2_apply(A& a, M method, std::tuple<Args...> args) { return h2_apply(a, method, args, h2_make_index_sequence<sizeof...(Args)>()); }

template <typename Matcher, typename Method, typename Args = std::tuple<>>
struct h2_member_method_matches : h2_matches {
   const Matcher m;
   Method method;
   Args args;
   explicit h2_member_method_matches(Matcher m_, Method method_, Args args_) : m(m_), method(method_), args(args_) {}

   template <typename A, typename P = typename std::add_pointer<typename std::remove_pointer<typename std::decay<A>::type>::type>::type>
   h2_fail* matches(const A& a, const C& c) const
   {
      return h2_matcher_cast<decltype(h2_apply(*(P) nullptr, method, args))>(m).matches(h2_apply(*(P)h2_pointer_if(a), method, args), c);
   }
   virtual h2_line expection(const C& c) const override { return h2_matches_expection(m, c); }
};

template <typename...> struct mfp {};  // mfp improve std::mem_fn for overloading
template <typename... Args> struct mfp<std::tuple<Args...>> {
   template <typename Class, typename ReturnType> static auto get(ReturnType (Class::*f)(Args...)) { return f; }
   template <typename Class, typename ReturnType> static auto get(ReturnType (Class::*f)(Args...) const) { return f; }
};

template <typename T, typename Method, typename Args = std::tuple<>, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, Method, Args>>>
inline P Member_method(T expect, Method method, Args args) { return P(h2_member_method_matches<E, Method, Args>(expect, method, args)); }

template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)()>>>
inline P Member_member(T expect, ReturnType (Class::*method)()) { return P(h2_member_method_matches<E, ReturnType (Class::*)()>(expect, method, std::tuple<>())); }
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() const>>>
inline P Member_member(T expect, ReturnType (Class::*method)() const) { return P(h2_member_method_matches<E, ReturnType (Class::*)() const>(expect, method, std::tuple<>())); }
#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)
// noexcept-specification is a part of the function type since C++17
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() noexcept>>>
inline P Member_member(T expect, ReturnType (Class::*method)() noexcept) { return P(h2_member_method_matches<E, ReturnType (Class::*)() noexcept>(expect, method, std::tuple<>())); }
template <typename T, typename Class, typename ReturnType, typename E = typename std::decay<T>::type, typename P = h2_polymorphic_matcher<h2_member_method_matches<E, ReturnType (Class::*)() const noexcept>>>
inline P Member_member(T expect, ReturnType (Class::*method)() const noexcept) { return P(h2_member_method_matches<E, ReturnType (Class::*)() const noexcept>(expect, method, std::tuple<>())); }
#endif
#endif

#define H2Member(expect, member, ...) H2PP_RESCAN(H2PP_CAT2(H2Member_, H2PP_IS_EMPTY(__VA_ARGS__))(expect, member, __VA_ARGS__))
#define H2Member_1(expect, member, ...) h2::Member_member(expect, member)
// #define H2Member_0(expect, Class, member, ...) h2::Member_method(expect, h2::mfp<decltype(std::make_tuple(__VA_ARGS__))>::get(&Class::member), std::make_tuple(__VA_ARGS__))  // without arguments braced
#define H2Member_0(...) H2PP_NCALL(H2Member_0_, 3, __VA_ARGS__)
#define H2Member_0_0(expect, member, ...) h2::Member_method(expect, h2::mfp<decltype(std::make_tuple(__VA_ARGS__))>::get(member), std::make_tuple(__VA_ARGS__))
#define H2Member_0_1(expect, member, ...) H2PP_RESCAN(H2PP_CAT2(H2Member_0_1_, H2PP_IS_PARENTHESIS(__VA_ARGS__))(expect, member, __VA_ARGS__))
#define H2Member_0_1_0(expect, member, ...) h2::Member_method(expect, h2::mfp<decltype(std::make_tuple(__VA_ARGS__))>::get(member), std::make_tuple(__VA_ARGS__))
#define H2Member_0_1_1(expect, member, ...) h2::Member_method(expect, h2::mfp<decltype(std::make_tuple __VA_ARGS__)>::get(member), std::make_tuple __VA_ARGS__)
