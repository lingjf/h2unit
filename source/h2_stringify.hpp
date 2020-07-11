
#define H2_HAS_MEMBER(member)                                    \
   template <typename T>                                         \
   struct h2_has_##member {                                      \
      template <typename U>                                      \
      static std::true_type test(decltype(&U::member));          \
      template <typename U>                                      \
      static std::false_type test(...);                          \
      static constexpr bool value = decltype(test<T>(0))::value; \
   }

H2_HAS_MEMBER(tostring);
H2_HAS_MEMBER(toString);
H2_HAS_MEMBER(Tostring);
H2_HAS_MEMBER(ToString);

template <typename T>
struct h2_is_ostreamable {
   template <typename U>
   static auto test(int) -> decltype(std::declval<std::ostream&>() << std::declval<U&>(), std::true_type());
   template <typename U>
   static auto test(...) -> std::false_type;
   static constexpr bool value = decltype(test<T>(0))::value;
};

template <typename T>
struct h2_is_pair {
   template <typename U>
   struct test : std::false_type {
   };
   template <typename K, typename V>
   struct test<std::pair<K, V>> : std::true_type {
   };
   static constexpr bool value = test<T>::value;
};

template <typename T>
struct has_const_iterator_begin_end {
   template <typename U>
   static std::true_type test(typename U::const_iterator*,
                              typename std::enable_if<
                                std::is_same<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::begin)),
                                             typename U::const_iterator (U::*)() const>::value>::type*,
                              typename std::enable_if<
                                std::is_same<decltype(static_cast<typename U::const_iterator (U::*)() const>(&U::end)),
                                             typename U::const_iterator (U::*)() const>::value,
                                void>::type*);

   template <typename U>
   static std::false_type test(...);

   static constexpr bool value = decltype(test<T>(nullptr, nullptr, nullptr))::value;
};

template <typename T>
struct h2_is_container : public std::integral_constant<bool, has_const_iterator_begin_end<T>::value> {
};

template <>
struct h2_is_container<std::string> : std::false_type {
};

template <>
struct h2_is_container<h2_string> : std::false_type {
};

template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_string print(T a)
   {
      return "";
   }
};

#define H2_STRINGIFY_IMPL_TOSTRING(name)                                                 \
   template <typename T>                                                                 \
   struct h2_stringify_impl<T, typename std::enable_if<h2_has_##name<T>::value>::type> { \
      static h2_string print(T a)                                                        \
      {                                                                                  \
         return a.name();                                                                \
      }                                                                                  \
   }

H2_STRINGIFY_IMPL_TOSTRING(tostring);
H2_STRINGIFY_IMPL_TOSTRING(toString);
H2_STRINGIFY_IMPL_TOSTRING(Tostring);
H2_STRINGIFY_IMPL_TOSTRING(ToString);

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_string print(T a)
   {
      return stream_print(a);
   }

   template <typename U>
   static h2_string stream_print(U a)
   {
      h2_ostringstream os;
      os << std::boolalpha;
      os << a;
      return h2_string(os.str().c_str());
   }

   // https://en.cppreference.com/w/cpp/string/byte/isprint
   static h2_string stream_print(unsigned char a)
   {
      return stream_print<unsigned int>(static_cast<unsigned int>(a));
   }
};

template <typename K, typename V>
struct h2_stringify_impl<std::pair<K, V>> {
   static h2_string print(const std::pair<K, V>& a)
   {
      return h2_stringify_impl<typename std::decay<K>::type>::print(a.first) + ": " + h2_stringify_impl<typename std::decay<V>::type>::print(a.second);
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value>::type> {
   static h2_string print(const T& a)
   {
      bool pair = h2_is_pair<typename std::decay<decltype(*a.begin())>::type>::value;
      h2_string ret = pair ? "{" : "[";
      for (auto it = a.begin(); it != a.end(); it++) {
         if (it != a.begin()) ret += ", ";
         ret += h2_stringify_impl<typename std::decay<decltype(*it)>::type>::print(*it);
      }
      return ret + (pair ? "}" : "]");
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
   {
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
inline h2_string h2_stringify(T a)
{
   return h2_stringify_impl<T>::print(a);
}

template <typename T>
inline h2_string h2_stringify(T a, int n)
{
   h2_string ret = "[";
   for (int i = 0; i < n; ++i) {
      if (i) ret += ", ";
      ret += h2_stringify(a[i]);
   }
   return ret + "]";
}

template <typename T>
inline h2_string h2_quote_stringfiy(const T& a)
{
   return (std::is_convertible<T, h2_string>::value ? "\"" : "") + h2_stringify(a) + (std::is_convertible<T, h2_string>::value ? "\"" : "");
}
