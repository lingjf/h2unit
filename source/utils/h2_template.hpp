template <typename T> struct h2_type_identity { typedef T type; }; // std::type_identity
template <typename...> struct h2_true_type : std::true_type {};

template <typename T, typename = void>
struct h2_is_smart_ptr : std::false_type {};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::shared_ptr<typename T::element_type>>::value>::type> : std::true_type {};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::unique_ptr<typename T::element_type>>::value>::type> : std::true_type {};
template <typename T>
struct h2_is_smart_ptr<T, typename std::enable_if<std::is_same<typename std::remove_cv<T>::type, std::weak_ptr<typename T::element_type>>::value>::type> : std::true_type {};

template <typename U, typename = void>
struct h2_decay_impl : h2_type_identity<U> {};
template <>
struct h2_decay_impl<char*> : h2_type_identity<const char*> {};
template <typename U>
struct h2_decay_impl<U, typename std::enable_if<std::is_enum<U>::value>::type> : h2_type_identity<typename std::underlying_type<U>::type> {};
template <typename T>
struct h2_decay : h2_decay_impl<typename std::decay<T>::type> {};

template <std::size_t I, typename T, typename... S>
struct h2_nth_type_impl : h2_type_identity<typename h2_nth_type_impl<I - 1, S...>::type> {};
template <typename T, typename... S>
struct h2_nth_type_impl<0, T, S...> : h2_type_identity<T> {};
template <std::size_t I, typename... S>
struct h2_nth_type : h2_type_identity<typename h2_nth_type_impl<I, S..., int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type> {};

template <std::size_t I, typename... S>
using h2_nth_decay = typename h2_decay<typename h2_nth_type<I, S...>::type>::type;

template <typename T, typename = void>
struct h2_sizeof_pointee : std::integral_constant<std::size_t, sizeof(typename std::remove_pointer<T>::type)> {};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<std::is_void<typename std::remove_pointer<T>::type>::value>::type> : std::integral_constant<std::size_t, 1> {};
template <typename T>
struct h2_sizeof_pointee<T, typename std::enable_if<h2_is_smart_ptr<T>::value>::type> : std::integral_constant<std::size_t, sizeof(typename T::element_type)> {};  // smart ptr not hold void*

template <typename T>
struct h2_pointee_type : h2_type_identity<typename T::element_type> {};
template <typename T>
struct h2_pointee_type<T*> : h2_type_identity<T> {};

template <typename T>
inline T* h2_pointer_if(T* a) { return a; }
template <typename T>
inline auto h2_pointer_if(T& a) -> typename std::enable_if<h2_is_smart_ptr<T>::value, typename T::element_type*>::type { return a.get(); }
template <typename T>
inline auto h2_pointer_if(T& a) -> typename std::enable_if<!h2_is_smart_ptr<T>::value, T*>::type { return &a; }

template <typename T>
struct h2_is_pair : std::false_type {};
template <typename K, typename V>
struct h2_is_pair<std::pair<K, V>> : std::true_type {};

template <typename T, typename = void>
struct h2_is_iterable : std::false_type {};
template <typename T>
struct h2_is_iterable<T,
                      typename std::enable_if<h2_true_type<typename T::value_type,
                                                           typename T::size_type,
                                                           typename T::iterator,
                                                           typename T::const_iterator,
                                                           decltype(std::declval<T>().begin()),
                                                           decltype(std::declval<T>().end()),
                                                           decltype(std::declval<T>().cbegin()),
                                                           decltype(std::declval<T>().cend())>::value>::type> : std::true_type {};

template <typename T, typename = void>
struct h2_has_substr : std::false_type {};
template <typename T>
struct h2_has_substr<T, typename std::enable_if<h2_true_type<decltype(std::declval<T>().substr())>::value>::type> : std::true_type {};

template <typename T> struct h2_is_container : std::conditional<h2_is_iterable<T>::value && !h2_has_substr<T>::value, std::true_type, std::false_type>::type {};
template <typename T> struct h2_is_string : std::conditional<h2_is_iterable<T>::value && h2_has_substr<T>::value, std::true_type, std::false_type>::type {};

template <typename T, typename = void>
struct h2_is_map : std::false_type {};
template <typename T>
struct h2_is_map<T, typename std::enable_if<h2_is_iterable<T>::value && h2_true_type<typename T::mapped_type>::value>::type> : std::true_type {};

template <typename T, typename = void>
struct h2_is_container_adaptor : std::false_type {};
template <typename T>
struct h2_is_container_adaptor<T, typename std::enable_if<h2_true_type<typename T::value_type, typename T::size_type, typename T::container_type>::value>::type> : std::true_type {};

template <typename ContainerAdaptor>
const typename ContainerAdaptor::container_type& underlying_container(const ContainerAdaptor& ca)
{
   struct AntiProtected : ContainerAdaptor {
      static const typename ContainerAdaptor::container_type& get(const ContainerAdaptor& ca) { return ca.*&AntiProtected::c; }
   };
   return AntiProtected::get(ca);
}

#if __cplusplus >= 201402L || (defined _MSVC_LANG && _MSVC_LANG >= 201402L)
#define h2_index_sequence std::index_sequence
#define h2_make_index_sequence std::make_index_sequence
#else
template <std::size_t...> struct h2_index_sequence {};
template <std::size_t N, std::size_t... S> struct h2_make_index_sequence : h2_make_index_sequence<N - 1, N - 1, S...> {};
template <std::size_t... S> struct h2_make_index_sequence<0, S...> : h2_index_sequence<S...> {};
#endif
