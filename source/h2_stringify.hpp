
template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_line print(T a, bool represent = false) { return "?"; }
};

#define H2_STRINGIFY_IMPL_TOSTRING(member)                                                                                    \
   template <typename T>                                                                                                      \
   struct h2_stringify_impl<T, typename std::enable_if<std::is_member_function_pointer<decltype(&T::member)>::value>::type> { \
      static h2_line print(const T& a, bool represent = false) { return const_cast<T&>(a).member(); }                         \
   }

/* tostring() may not be mark const, remove cast const in T a */

H2_STRINGIFY_IMPL_TOSTRING(tostring);
H2_STRINGIFY_IMPL_TOSTRING(toString);
H2_STRINGIFY_IMPL_TOSTRING(Tostring);
H2_STRINGIFY_IMPL_TOSTRING(ToString);
H2_STRINGIFY_IMPL_TOSTRING(to_string);

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false) { return ostream_print(a, represent); }

   template <typename U>
   static h2_line ostream_print(const U& a, bool represent)
   {
      h2_ostringstream os;
      os << std::boolalpha;
      os << const_cast<U&>(a);
      if (represent) {
         const char* quote = nullptr;
         if (std::is_same<char, U>::value) quote = "'";
         if (std::is_convertible<U, h2_string>::value) quote = "\"";
         if (quote) return gray(quote) + os.str().c_str() + gray(quote);
      }
      return {os.str().c_str()};
   }

   static h2_line ostream_print(unsigned char a, bool represent)
   {  // https://en.cppreference.com/w/cpp/string/byte/isprint
      return ostream_print<unsigned int>(static_cast<unsigned int>(a), represent);
   }
};

template <typename K, typename V>
struct h2_stringify_impl<std::pair<K, V>> {
   static h2_line print(const std::pair<K, V>& a, bool represent = false)
   {
      return gray("(") + h2_stringify_impl<K>::print(a.first, represent) + gray(", ") + h2_stringify_impl<V>::print(a.second, represent) + gray(")");
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value && !std::is_convertible<T, h2_string>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      h2_line line;
      line += gray("[");
      for (auto it = a.begin(); it != a.end(); it++) {
         if (it != a.begin()) line += gray(", ");
         line += h2_stringify_impl<typename T::value_type>::print(*it, represent);
      }
      line += gray("]");
      return line;
   }
};

template <typename... Args>
struct h2_stringify_impl<std::tuple<Args...>> {
   static h2_line print(const std::tuple<Args...>& a, bool represent = false)
   {
      return gray("(") + tuple_print(a, represent, std::integral_constant<std::size_t, sizeof...(Args)>()) + gray(")");
   }

   static h2_line tuple_print(const std::tuple<Args...>& a, bool represent, std::integral_constant<std::size_t, 0>) { return {}; }
   template <std::size_t I>
   static h2_line tuple_print(const std::tuple<Args...>& a, bool represent, std::integral_constant<std::size_t, I>)
   {
      return tuple_print(a, represent, std::integral_constant<std::size_t, I - 1>()) + (1 < I ? gray(", ") : h2_line()) + h2_stringify_impl<typename std::decay<decltype(std::get<I - 1>(a))>::type>::print(std::get<I - 1>(a), represent);
   }
};

template <>
struct h2_stringify_impl<std::nullptr_t> {
   static h2_line print(std::nullptr_t a, bool represent = false) { return "nullptr"; }
};

template <typename T>
inline h2_line h2_stringify(const T& a, bool represent = false)
{
   return h2_stringify_impl<T>::print(a, represent);
}

template <typename T>
inline h2_line h2_stringify(T a, size_t n, bool represent)
{
   if (n == 0) return h2_stringify(a, represent);

   h2_line line;
   line += gray("[");
   for (size_t i = 0; i < n; ++i) {
      if (i) line += gray(", ");
      line += h2_stringify(a[i], represent);
   }
   line += gray("]");
   return line;
}

template <typename T>
inline h2_line h2_representify(const T& a) { return h2_stringify(a, true); }

template <typename T>
inline h2_line h2_representify(T a, size_t n) { return h2_stringify(a, n, true); }
