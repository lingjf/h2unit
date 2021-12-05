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
struct h2_pointee_type {
   typedef typename T::element_type type;
};
template <typename T>
struct h2_pointee_type<T*> {
   typedef T type;
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

template <typename...>
struct h2_valid_t {
};

template <typename T, typename = void>
struct h2_is_string : std::false_type {
};

template <typename T>
struct h2_is_string<T,
                    typename std::conditional<false,
                                              h2_valid_t<typename T::value_type,
                                                         typename T::size_type,
                                                         typename T::iterator,
                                                         typename T::const_iterator,
                                                         decltype(std::declval<T>().substr()),
                                                         decltype(std::declval<T>().begin()),
                                                         decltype(std::declval<T>().end()),
                                                         decltype(std::declval<T>().cbegin()),
                                                         decltype(std::declval<T>().cend())>,
                                              void>::type> : std::true_type {
};

template <typename T, typename = void>
struct h2_is_iterable : std::false_type {
};

template <typename T>
struct h2_is_iterable<T,
                      typename std::conditional<false,
                                                h2_valid_t<typename T::value_type,
                                                           typename T::size_type,
                                                           typename T::iterator,
                                                           typename T::const_iterator,
                                                           decltype(std::declval<T>().begin()),
                                                           decltype(std::declval<T>().end()),
                                                           decltype(std::declval<T>().cbegin()),
                                                           decltype(std::declval<T>().cend())>,
                                                void>::type> : std::true_type {
};

template <typename T, typename = void>
struct h2_is_map : std::false_type {
};

template <typename T>
struct h2_is_map<T,
                 typename std::conditional<false,
                                           h2_valid_t<typename T::value_type,
                                                      typename T::size_type,
                                                      typename T::mapped_type,
                                                      typename T::iterator,
                                                      typename T::const_iterator,
                                                      decltype(std::declval<T>().begin()),
                                                      decltype(std::declval<T>().end()),
                                                      decltype(std::declval<T>().cbegin()),
                                                      decltype(std::declval<T>().cend())>,
                                           void>::type> : std::true_type {
};

template <typename T>
struct h2_is_container : std::conditional<h2_is_iterable<T>::value && !h2_is_string<T>::value, std::true_type, std::false_type>::type {
};

template <typename T, typename = void>
struct h2_is_container_adaptor : std::false_type {
};

template <typename T>
struct h2_is_container_adaptor<T,
                               typename std::conditional<false,
                                                         h2_valid_t<typename T::value_type,
                                                                    typename T::size_type,
                                                                    typename T::container_type>,
                                                         void>::type> : std::true_type {
};

template <typename T, typename = void>
struct h2_is_sizable : std::false_type {
};

template <typename T>
struct h2_is_sizable<T,
                     typename std::conditional<false,
                                               h2_valid_t<decltype(std::declval<T>().size())>,
                                               void>::type> : public std::true_type {
};

template <typename ContainerAdaptor>
const typename ContainerAdaptor::container_type& underlying_container(const ContainerAdaptor& ca)
{
   struct AntiProtected : ContainerAdaptor {
      static const typename ContainerAdaptor::container_type& get(const ContainerAdaptor& ca)
      {
         return ca.*&AntiProtected::c;
      }
   };
   return AntiProtected::get(ca);
}
