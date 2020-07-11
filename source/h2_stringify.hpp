
template <typename T>
struct h2_is_ostreamable {
   template <typename U>
   static auto test(U* u) -> decltype(std::declval<std::ostream&>() << *u, std::true_type());
   template <typename U>
   static auto test(...) -> std::false_type;
   static constexpr bool value = decltype(test<T>(nullptr))::value;
};

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
   static std::true_type has_begin(typename std::enable_if<
                                   std::is_same<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::begin)),
                                                typename U::const_iterator (U::*)() const>::value>::type*);
   template <typename U>
   static std::false_type has_begin(...);

   template <typename U>
   static auto has_end(U* u) -> typename std::enable_if<std::is_member_function_pointer<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::end))>::value, std::true_type>::type;
   template <typename U>
   static std::false_type has_end(...);

   static constexpr bool value = decltype(has_const_iterator<T>(nullptr))::value &&
                                 decltype(has_begin<T>(nullptr))::value &&
                                 decltype(has_end<T>(nullptr))::value;
};

template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_string print(T a)
   {
      return "?";
   }
};

#define H2_STRINGIFY_IMPL_TOSTRING(member)                                                                                    \
   template <typename T>                                                                                                      \
   struct h2_stringify_impl<T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::member)>::value>::type> { \
      static h2_string print(const T& a)                                                                                      \
      {                                                                                                                       \
         return const_cast<T&>(a).member(); /* member may not be mark const, remove cast const in T a */                      \
      }                                                                                                                       \
   }

H2_STRINGIFY_IMPL_TOSTRING(tostring);
H2_STRINGIFY_IMPL_TOSTRING(toString);
H2_STRINGIFY_IMPL_TOSTRING(Tostring);
H2_STRINGIFY_IMPL_TOSTRING(ToString);

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_string print(const T& a)
   {
      return ostream_print(a);
   }

   template <typename U>
   static h2_string ostream_print(const U& a)
   {
      h2_ostringstream os;
      os << std::boolalpha;
      os << const_cast<U&>(a);
      return h2_string(os.str().c_str());
   }

   // https://en.cppreference.com/w/cpp/string/byte/isprint
   static h2_string ostream_print(unsigned char a)
   {
      return ostream_print<unsigned int>(static_cast<unsigned int>(a));
   }
};

template <typename K, typename V>
struct h2_stringify_impl<std::pair<K, V>> {
   static h2_string print(const std::pair<K, V>& a)
   {
      return "(" + h2_stringify_impl<K>::print(a.first) + ", " + h2_stringify_impl<V>::print(a.second) + ")";
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value && !std::is_convertible<T, h2_string>::value>::type> {
   static h2_string print(const T& a)
   {
      h2_string ret = "[";
      for (auto it = a.begin(); it != a.end(); it++) {
         ret += Comma[it != a.begin()] + h2_stringify_impl<typename T::value_type>::print(*it);
      }
      return ret + "]";
   }
};

template <typename... Args>
struct h2_stringify_impl<std::tuple<Args...>> {
   static h2_string print(const std::tuple<Args...>& a)
   {
      return "(" + tuple_print(a, std::integral_constant<std::size_t, 0>()) + ")";
   }

   static h2_string tuple_print(const std::tuple<Args...>& a, std::integral_constant<std::size_t, sizeof...(Args)>)
   {
      return "";
   }

   static h2_string tuple_print(const std::tuple<Args...>& a, typename std::conditional<sizeof...(Args) != 0, std::integral_constant<std::size_t, 0>, std::nullptr_t>::type)
   {  // decay inside type, because std::get<0>(a) is lvalue reference
      return h2_stringify_impl<typename std::decay<decltype(std::get<0>(a))>::type>::print(std::get<0>(a)) + tuple_print(a, std::integral_constant<std::size_t, 1>());
   }

   template <std::size_t N>
   static h2_string tuple_print(const std::tuple<Args...>& a, std::integral_constant<std::size_t, N>)
   {
      return ", " + h2_stringify_impl<typename std::decay<decltype(std::get<N>(a))>::type>::print(std::get<N>(a)) + tuple_print(a, std::integral_constant<std::size_t, N + 1>());
   }
};

template <>
struct h2_stringify_impl<std::nullptr_t> {
   static h2_string print(std::nullptr_t a)
   {
      return "nullptr";
   }
};

template <typename T>
inline h2_string h2_stringify(const T& a)
{
   return h2_stringify_impl<T>::print(a);
}

template <typename T>
inline h2_string h2_stringify(const T& a, int n)
{
   h2_string ret = "[";
   for (int i = 0; i < n; ++i) {
      ret += Comma[!!i] + h2_stringify(a[i]);
   }
   return ret + "]";
}

template <typename T>
inline h2_string h2_quote_stringfiy(const T& a)
{
   return (std::is_convertible<T, h2_string>::value ? "\"" : "") + h2_stringify(a) + (std::is_convertible<T, h2_string>::value ? "\"" : "");
}
