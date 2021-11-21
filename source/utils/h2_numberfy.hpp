
template <typename T, typename = void>
struct h2_numberfy_impl {
   static std::uintptr_t cast(T a) { return a; }
};
template <typename T>
struct h2_numberfy_impl<T, typename std::enable_if<std::is_pointer<T>::value>::type> {
   static std::uintptr_t cast(T a) { return reinterpret_cast<std::uintptr_t>(a); }  // https://stackoverflow.com/questions/153065/converting-a-pointer-into-an-integer
};
template <typename T>
struct h2_numberfy_impl<T, typename std::enable_if<std::is_member_pointer<T>::value>::type> {  // non-static member object or non-static member function
   static std::uintptr_t cast(T a)
   {
      union {
         T a;
         std::uintptr_t b;
      } u;
      u.a = a;
      return u.b;
   }
};
template <typename T>
struct h2_numberfy_impl<T, typename std::enable_if<h2_is_smart_ptr<T>::value>::type> {
   static std::uintptr_t cast(T a) { return h2_numberfy_impl<typename T::element_type*>::cast(a.get()); }
};
template <>
struct h2_numberfy_impl<std::nullptr_t> {
   static std::uintptr_t cast(std::nullptr_t) { return 0; }
};

template <typename R, typename F>
static R h2_numberfy(F f) { return (R)h2_numberfy_impl<F>::cast(f); }
// template function can't partial specialization, redirect to template class partial specialization
