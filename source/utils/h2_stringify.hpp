template <typename T, typename = void>
struct h2_stringify_impl {
   static h2_line print(const T&, bool = false) { return "?"; }
};

#define H2_TOSTRING_ABLE(tostring)                                                                            \
   template <typename T>                                                                                      \
   struct h2_##tostring##_able {                                                                              \
      template <typename U>                                                                                   \
      static auto return_type(U* u) -> decltype(u->tostring());                                               \
      template <typename U>                                                                                   \
      static void return_type(...);                                                                           \
      static constexpr bool value = std::is_convertible<decltype(return_type<T>(nullptr)), h2_string>::value; \
   };

H2_TOSTRING_ABLE(tostring);
H2_TOSTRING_ABLE(toString);
H2_TOSTRING_ABLE(Tostring);
H2_TOSTRING_ABLE(ToString);
H2_TOSTRING_ABLE(to_string);

/* tostring() may not be mark const, remove cast const in T a; fix multi-tostring */
template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2::h2_tostring_able<T>::value || h2::h2_toString_able<T>::value || h2::h2_Tostring_able<T>::value || h2::h2_ToString_able<T>::value || h2::h2_to_string_able<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      if (represent) return gray("\"") + print__tostring(a) + gray("\"");
      return print__tostring(a);
   }
   template <typename U>
   static auto print__tostring(const U& a) -> typename std::enable_if<h2::h2_tostring_able<U>::value, h2_string>::type { return const_cast<U&>(a).tostring(); }
   template <typename U>
   static auto print__tostring(const U& a) -> typename std::enable_if<!h2::h2_tostring_able<U>::value, h2_string>::type { return print__toString(a); }
   template <typename U>
   static auto print__toString(const U& a) -> typename std::enable_if<h2::h2_toString_able<U>::value, h2_string>::type { return const_cast<U&>(a).toString(); }
   template <typename U>
   static auto print__toString(const U& a) -> typename std::enable_if<!h2::h2_toString_able<U>::value, h2_string>::type { return print__Tostring(a); }
   template <typename U>
   static auto print__Tostring(const U& a) -> typename std::enable_if<h2::h2_Tostring_able<U>::value, h2_string>::type { return const_cast<U&>(a).toString(); }
   template <typename U>
   static auto print__Tostring(const U& a) -> typename std::enable_if<!h2::h2_Tostring_able<U>::value, h2_string>::type { return print__ToString(a); }
   template <typename U>
   static auto print__ToString(const U& a) -> typename std::enable_if<h2::h2_ToString_able<U>::value, h2_string>::type { return const_cast<U&>(a).ToString(); }
   template <typename U>
   static auto print__ToString(const U& a) -> typename std::enable_if<!h2::h2_ToString_able<U>::value, h2_string>::type { return print__to_string(a); }
   template <typename U>
   static auto print__to_string(const U& a) -> typename std::enable_if<h2::h2_to_string_able<U>::value, h2_string>::type { return const_cast<U&>(a).to_string(); }
   template <typename U>
   static auto print__to_string(const U& a) -> typename std::enable_if<!h2::h2_to_string_able<U>::value, h2_string>::type { return ""; }
};

template <typename T>
struct h2_is_ostreamable {
   template <typename U>
   static auto test(U* u) -> decltype(std::declval<std::ostream&>() << *u, std::true_type());
   template <typename U>
   static auto test(...) -> std::false_type;
   static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_ostreamable<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false) { return ostream_print(a, represent); }

   template <typename U>
   static auto ostream_print(const U& a, bool) -> typename std::enable_if<std::is_arithmetic<U>::value, h2_line>::type
   {
      auto str = std::to_string(a);
      if (str.find_first_of('.') != std::string::npos) {
         str.erase(str.find_last_not_of("0") + 1);
         str.erase(str.find_last_not_of(".") + 1);
      }
      return {str.c_str()};
   }

   template <typename U>
   static auto ostream_print(const U& a, bool represent) -> typename std::enable_if<!std::is_arithmetic<U>::value, h2_line>::type
   {
      h2_ostringstream oss;
      oss << a;
      if (represent && std::is_convertible<U, h2_string>::value)
         return gray("\"") + oss.str().c_str() + gray("\"");
      return {oss.str().c_str()};
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
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      h2_line line;
      for (auto it = a.begin(); it != a.end(); ++it)
         line += (it != a.begin() ? gray(", ") : h2_line()) + h2_stringify_impl<typename T::value_type>::print(*it, represent);
      return gray("[") + line + gray("]");
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_container_adaptor<T>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      auto _a = underlying_container(a);
      return h2_stringify_impl<decltype(_a)>::print(_a, represent);
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
   static h2_line print(std::nullptr_t a, bool = false) { return "nullptr"; }
};

template <>
struct h2_stringify_impl<bool> {
   static h2_line print(const bool a, bool = false) { return a ? "true" : "false"; }
};

template <>
struct h2_stringify_impl<const char*> {
   static h2_line print(const char* a, bool represent) { return a ? h2_stringify_impl<h2_string>::print(h2_string{a}, represent) : h2_line{"(null)"}; }
};

template <>
struct h2_stringify_impl<char*> {
   static h2_line print(char* a, bool represent) { return h2_stringify_impl<const char*>::print(a, represent); }
};

template <>
struct h2_stringify_impl<unsigned char> {  // https://en.cppreference.com/w/cpp/string/byte/isprint
   static h2_line print(unsigned char a, bool) { return h2_stringify_impl<unsigned int>::print(static_cast<unsigned int>(a), false); }
};

template <>
struct h2_stringify_impl<char> {
   static h2_line print(char a, bool represent)
   {
      h2_string str(1, a);
      if (represent) return gray("'") + str + gray("'");
      return {str};
   }
};

template <typename T>
struct h2_stringify_impl<T, typename std::enable_if<h2_is_string<T>::value && std::is_same<wchar_t, typename T::value_type>::value>::type> {
   static h2_line print(const T& a, bool represent = false)
   {
      h2_string s;
      for (auto c : a) s += (c <= 0xff) ? static_cast<char>(c) : '?';
      return h2_stringify_impl<h2_string>::print(s, represent);
   }
};

template <>
struct h2_stringify_impl<const wchar_t*> {
   static h2_line print(const wchar_t* a, bool represent) { return a ? h2_stringify_impl<std::wstring>::print(std::wstring{a}, represent) : h2_line{"(null)"}; }
};

template <>
struct h2_stringify_impl<wchar_t*> {
   static h2_line print(wchar_t* a, bool represent) { return h2_stringify_impl<const wchar_t*>::print(a, represent); }
};

template <>
struct h2_stringify_impl<wchar_t> {
   static h2_line print(wchar_t a, bool represent) { return h2_stringify_impl<char>::print((a <= 0xff) ? static_cast<char>(a) : '?', represent); }
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
   for (size_t i = 0; i < n; ++i)
      line += (i ? gray(", ") : h2_line()) + h2_stringify(a[i], represent);
   return gray("[") + line + gray("]");
}
