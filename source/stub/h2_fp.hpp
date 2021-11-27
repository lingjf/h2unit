/* clang-format off */
template <typename T> static void h2_destructible(...) {}
template <typename T> static auto h2_destructible(T* o) -> typename std::enable_if<std::is_destructible<T>::value && !std::is_abstract<T>::value, void>::type { o->~T(); }

template <typename T, int I> struct h2_constructible_error { static T* O(void* m) { return static_cast<T*>(m = (void*)I); } };
template <typename T> struct h2_constructible0 : std::true_type { static T* O(void* m) { return new (m) T(); } };

template <typename, typename> struct h2_constructible1_impl : std::false_type {};
template <typename, typename> struct h2_constructible2_impl : std::false_type {};
template <typename, typename> struct h2_constructible3_impl : std::false_type {};
template <typename, typename> struct h2_constructible4_impl : std::false_type {};
template <typename, typename> struct h2_constructible5_impl : std::false_type {};
template <typename, typename> struct h2_constructible6_impl : std::false_type {};
template <typename, typename> struct h2_constructible7_impl : std::false_type {};
template <typename, typename> struct h2_constructible8_impl : std::false_type {};
template <typename, typename> struct h2_constructible9_impl : std::false_type {};

#if !(defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)) // not gcc4
template <typename T> struct h2_constructible1_impl<T, decltype(T({}))> : std::true_type { static T* O(void* m) { return new (m) T({}); } };
template <typename T> struct h2_constructible2_impl<T, decltype(T({}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}); } };
template <typename T> struct h2_constructible3_impl<T, decltype(T({}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}); } };
template <typename T> struct h2_constructible4_impl<T, decltype(T({}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}); } };
template <typename T> struct h2_constructible5_impl<T, decltype(T({}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible6_impl<T, decltype(T({}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible7_impl<T, decltype(T({}, {}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible8_impl<T, decltype(T({}, {}, {}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}); } };
template <typename T> struct h2_constructible9_impl<T, decltype(T({}, {}, {}, {}, {}, {}, {}, {}, {}))> : std::true_type { static T* O(void* m) { return new (m) T({}, {}, {}, {}, {}, {}, {}, {}, {}); } };
#endif

template <typename T> using h2_constructible1 = h2_constructible1_impl<T, T>;
template <typename T> using h2_constructible2 = h2_constructible2_impl<T, T>;
template <typename T> using h2_constructible3 = h2_constructible3_impl<T, T>;
template <typename T> using h2_constructible4 = h2_constructible4_impl<T, T>;
template <typename T> using h2_constructible5 = h2_constructible5_impl<T, T>;
template <typename T> using h2_constructible6 = h2_constructible6_impl<T, T>;
template <typename T> using h2_constructible7 = h2_constructible7_impl<T, T>;
template <typename T> using h2_constructible8 = h2_constructible8_impl<T, T>;
template <typename T> using h2_constructible9 = h2_constructible9_impl<T, T>;

template <typename T>
using h2_constructible = 
   typename std::conditional<std::is_abstract<T>::value,
      h2_constructible_error<T, 1>,
      typename std::conditional<std::is_default_constructible<T>::value,
         h2_constructible0<T>,
         typename std::conditional<h2_constructible1<T>::value,
            h2_constructible1<T>,
            typename std::conditional<h2_constructible2<T>::value,
               h2_constructible2<T>,
               typename std::conditional<h2_constructible3<T>::value,
                  h2_constructible3<T>,
                  typename std::conditional<h2_constructible4<T>::value,
                     h2_constructible4<T>,
                     typename std::conditional<h2_constructible5<T>::value,
                        h2_constructible5<T>,
                        typename std::conditional<h2_constructible6<T>::value,
                           h2_constructible6<T>,
                           typename std::conditional<h2_constructible7<T>::value,
                              h2_constructible7<T>,
                              typename std::conditional<h2_constructible8<T>::value,
                                 h2_constructible8<T>,
                                 typename std::conditional<h2_constructible9<T>::value,
                                    h2_constructible9<T>,
                                    h2_constructible_error<T, 2>
                                 >::type
                              >::type
                           >::type
                        >::type
                     >::type
                  >::type
               >::type
            >::type
         >::type
      >::type
   >::type;

/* clang-format on */

static constexpr const char* const constructible_errors[] = {"", "abstract ", "non default constructor "};

template <typename = void, typename = void>
struct h2_fp {
   template <typename T>
   static void* A(T fp) { return h2_numberfy<void*>(fp); }
   static void* A(const char* fn)
   {
      h2_symbol* res[16];
      int n = h2_nm::get_by_name(fn, res, 16);
      if (n == 1) return h2_load::addr_to_ptr(res[0]->addr);
      h2_vector<h2_string> candidates;
      for (int i = 0; i < n; ++i) candidates.push_back(res[i]->name());
      h2_runner::failing(h2_fail::new_symbol(fn, candidates));
      return nullptr;
   }
};

template <typename ReturnType, typename... ArgumentTypes>
struct h2_fp<ReturnType(ArgumentTypes...)> {
   static void* A(ReturnType (*fp)(ArgumentTypes...)) { return h2_numberfy<void*>(fp); }
   static void* A(const char* fn) { return h2_fp<>::A(fn); }
};

template <typename ClassType, typename ReturnType, typename... ArgumentTypes>
struct h2_fp<ClassType, ReturnType(ArgumentTypes...)> {
   // static member function
   static void* A(ReturnType (*f)(ArgumentTypes...)) { return (void*)f; }
   static void* B(ClassType* o, ReturnType (*f)(ArgumentTypes...)) { return (void*)f; }

#if defined _MSC_VER
   // https://github.com/microsoft/Detours
   // &C::f1 ILT+165(??_9C$BAAA) 00007FF7987210AA
   // &C::f2 ILT+410(??_9C$B7AA) 00007FF79872119F
   // follow_jmp &C::f1 C::`vcall'{0}' 00007FF798721F2C
   // follow_jmp &C::f2 C::`vcall'{8}' 00007FF798721F24
   static void* A(ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      long offset;
      if (!is_virtual_mfp(f, offset)) return h2_numberfy<void*>(f);

      ClassType* o = h2_constructible<ClassType>::O(alloca(sizeof(ClassType)));
      if (0 == (unsigned long long)o || 1 == (unsigned long long)o || 2 == (unsigned long long)o) {
         h2_runner::failing(h2_fail::new_symbol("vtable of " + h2_string(constructible_errors[(unsigned long long)o]) + h2_cxa::type_name<ClassType>(), {}, "  try: " + color("STUB/MOCK(", "yellow") + color("Object", "bold,red") + color(", Class, Method, ...)", "yellow")));
         return nullptr;
      }
      void* t = get_virtual_mfp(o, offset);
      h2_destructible<ClassType>(o);
      return t;
   }
   static void* B(ClassType* o, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      long offset;
      if (!is_virtual_mfp(f, offset)) return h2_numberfy<void*>(f);
      return get_virtual_mfp(o, offset);
   }

   static void* get_virtual_mfp(ClassType* o, long offset)
   {
      void** vtable = *(void***)o;
      return vtable[offset / sizeof(void*)];
   }
   static bool is_virtual_mfp(ReturnType (ClassType::*f)(ArgumentTypes...), long& offset)
   {
      h2_symbol* symbol = h2_nm::get_by_addr((unsigned long long)h2_cxa::follow_jmp(h2_numberfy<void*>(f)));
      if (!symbol) return false;
      char* p = strstr(symbol->name(), "::`vcall'{");
      if (!p) return false;  // not virtual member function
      offset = strtol(p + 10, nullptr, 10);
      return true;
   }
#else
   //  https://itanium-cxx-abi.github.io/cxx-abi/
   //  &ClassType::Method has separate representations for non-virtual and virtual functions.
   //  For non-virtual functions, it is the address of the function.
   //  For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
   //  The least-significant bit therefore discriminates between virtual and non-virtual functions.
   static void* A(ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      if (!is_virtual_mfp(f)) return h2_numberfy<void*>(f);
      ClassType* o = h2_constructible<ClassType>::O(alloca(sizeof(ClassType)));
      if (2 < (unsigned long long)o) {
         void* t = get_virtual_mfp(o, f);
         h2_destructible<ClassType>(o);
         return t;
      }
      char vtable_symbol[1024];
      sprintf(vtable_symbol, "_ZTV%s", typeid(ClassType).name());  // mangle for "vtable for ClassType"
      unsigned long long relative_vtable = h2_nm::get_mangle(vtable_symbol);
      if (!relative_vtable) {
         h2_runner::failing(h2_fail::new_symbol("vtable of " + h2_string(constructible_errors[(unsigned long long)o]) + h2_cxa::type_name<ClassType>(), {}, "  try: " + color("STUB/MOCK(", "yellow") + color("Object", "bold,red") + color(", Class, Method, ...)", "yellow")));
         return nullptr;
      }
      return get_virtual_mfp((void**)h2_load::vtable_to_ptr(relative_vtable), f);
   }
   static void* B(ClassType* o, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      if (!is_virtual_mfp(f)) return h2_numberfy<void*>(f);
      return get_virtual_mfp(*(void***)o, f);
   }

   static void* get_virtual_mfp(ClassType* o, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      return get_virtual_mfp(*(void***)o, f);
   }
   static void* get_virtual_mfp(void** vtable, ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      return vtable[(h2_numberfy<unsigned long long>(f) & ~1ULL) / sizeof(void*)];
   }
   static bool is_virtual_mfp(ReturnType (ClassType::*f)(ArgumentTypes...))
   {
      if (h2_numberfy<unsigned long long>(&h2_vtable_test::dummy) & 1)
         return (h2_numberfy<unsigned long long>(f) & 1) && (h2_numberfy<unsigned long long>(f) - 1) % sizeof(void*) == 0 && h2_numberfy<unsigned long long>(f) < 1000 * sizeof(void*);
      else
         return h2_numberfy<unsigned long long>(f) % sizeof(void*) == 0 && h2_numberfy<unsigned long long>(f) < 100 * sizeof(void*);
   }
#endif
};
