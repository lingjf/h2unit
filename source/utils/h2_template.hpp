template <typename T, typename = void>
struct h2_is_smart_ptr : std::false_type {
};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::shared_ptr<typename T::element_type>>::value>::type> : std::true_type {
};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::unique_ptr<typename T::element_type>>::value>::type> : std::true_type {
};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::weak_ptr<typename T::element_type>>::value>::type> : std::true_type {
};

template <typename U, typename = void>
struct h2_decay_impl {
   typedef U type;
};
template <>
struct h2_decay_impl<char*> {
   typedef const char* type;
};
template <typename U>
struct h2_decay_impl<U, typename std::enable_if<std::is_enum<U>::value>::type> {
   typedef int type;
};
template <typename T>
struct h2_decay {
   using type = typename h2_decay_impl<typename std::decay<T>::type>::type;
};

template <std::size_t I, typename T, typename... Args>
struct h2_nth_type_impl {
   using type = typename h2_nth_type_impl<I - 1, Args...>::type;
};
template <typename T, typename... Args>
struct h2_nth_type_impl<0, T, Args...> {
   using type = T;
};
template <std::size_t Index, typename... Args>
struct h2_nth_type {
   using type = typename h2_nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;
};

template <std::size_t Index, typename... Args>
using h2_nth_decay = typename h2_decay<typename h2_nth_type<Index, Args...>::type>::type;

template <typename T, typename = void>
struct h2_sizeof_pointee : std::integral_constant<std::size_t, sizeof(typename std::remove_pointer<T>::type)> {
};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<std::is_void<typename std::remove_pointer<T>::type>::value>::type> : std::integral_constant<std::size_t, 1> {
};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<h2_is_smart_ptr<T>::value>::type> : std::integral_constant<std::size_t, sizeof(typename T::element_type)> {  // smart ptr not hold void*
};

template <typename T>
inline T* h2_pointer_if(T* a) { return a; }
template <typename T>
inline auto h2_pointer_if(T& a) -> typename std::enable_if<h2_is_smart_ptr<T>::value, typename T::element_type*>::type { return a.get(); }
template <typename T>
inline auto h2_pointer_if(T& a) -> typename std::enable_if<!h2_is_smart_ptr<T>::value, T*>::type { return &a; }

template <typename T>
struct h2_is_pair : std::false_type {
};
template <typename K, typename V>
struct h2_is_pair<std::pair<K, V>> : std::true_type {
};

template <typename T>
struct h2_is_container {
   template <typename U>
   static std::true_type has_const_iterator(typename U::const_iterator*);
   template <typename U>
   static std::false_type has_const_iterator(...);

   template <typename U>
   static std::true_type has_begin(typename std::enable_if<std::is_same<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::begin)), typename U::const_iterator (U::*)() const>::value>::type*);
   template <typename U>
   static std::false_type has_begin(...);

   template <typename U>
   static auto has_end(U* u) -> typename std::enable_if<std::is_member_function_pointer<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::end))>::value, std::true_type>::type;
   template <typename U>
   static std::false_type has_end(...);

   static constexpr bool value = decltype(has_const_iterator<T>(nullptr))::value && decltype(has_begin<T>(nullptr))::value && decltype(has_end<T>(nullptr))::value;
};
