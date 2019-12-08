
#ifndef ___H2_MFP__H___
#define ___H2_MFP__H___

/* clang-format off */

template <typename...>
using h2_void_t = void;

template <typename T, int I>
struct h2_constructible_error
{ static T* O(void *o) { throw I; return (T*)o; } };

template <typename, typename T>
struct h2_constructible0_impl : std::false_type { };
template <typename T>
struct h2_constructible0_impl<h2_void_t<decltype(T())>, T> : std::true_type
{ static T* O(void* o) { return new (o) T(); } };

template <typename, typename T>
struct h2_constructible1_impl : std::false_type { };
template <typename T>
struct h2_constructible1_impl<h2_void_t<decltype(T({}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}); } };

template <typename, typename T>
struct h2_constructible2_impl : std::false_type { };
template <typename T>
struct h2_constructible2_impl<h2_void_t<decltype(T({}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}); } };

template <typename, typename T>
struct h2_constructible3_impl : std::false_type { };
template <typename T>
struct h2_constructible3_impl<h2_void_t<decltype(T({}, {}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}, {}); } };

template <typename, typename T>
struct h2_constructible4_impl : std::false_type { };
template <typename T>
struct h2_constructible4_impl<h2_void_t<decltype(T({}, {}, {}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}, {}, {}); } };

template <typename, typename T>
struct h2_constructible5_impl : std::false_type { };
template <typename T>
struct h2_constructible5_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}, {}, {}, {}); } };


template <typename T>
using h2_constructible0 = h2_constructible0_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible1 = h2_constructible1_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible2 = h2_constructible2_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible3 = h2_constructible3_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible4 = h2_constructible4_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible5 = h2_constructible5_impl<h2_void_t<>, T>;


template <typename T>
using h2_constructible = 
  typename std::conditional<
    h2_constructible0<T>::value,
    h2_constructible0<T>,
    typename std::conditional<
      h2_constructible1<T>::value,
      h2_constructible1<T>,
      typename std::conditional<
        h2_constructible2<T>::value,
        h2_constructible2<T>,
        typename std::conditional<
          h2_constructible3<T>::value,
          h2_constructible3<T>,
          typename std::conditional<
            h2_constructible4<T>::value,
            h2_constructible4<T>,
            typename std::conditional<
              h2_constructible5<T>::value,
              h2_constructible5<T>,
              typename std::conditional<
                std::is_abstract<T>::value,
                h2_constructible_error<T, 2>,
                h2_constructible_error<T, 1>
              >::type
            >::type
          >::type
        >::type
      >::type
    >::type
  >::type;

/* clang-format on */

/*    
    https://itanium-cxx-abi.github.io/cxx-abi/

    &Class::Method has separate representations for non-virtual and virtual functions.
    For non-virtual functions, it is the address of the function.  
    For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
    The least-significant bit therefore discriminates between virtual and non-virtual functions.
*/

static inline void h2_fail_g(h2_fail* fail);

template <typename Class, typename F>
struct h2_mfp;

template <typename Class, typename Return, typename... Args>
struct h2_mfp<Class, Return(Args...)>
{
   typedef Return (Class::*mfp_type)(Args...);
   typedef union
   {
      mfp_type f;
      void* p;
      intptr_t v;
   } cast_type;

   static inline bool is_virtual(cast_type& u)
   {
      return (u.v & 1) &&
             (u.v - 1) % sizeof(void*) == 0 &&
             (u.v - 1) / sizeof(void*) < 1000;
   }

   static inline void* get_vmfp(cast_type& u, Class* obj)
   {
      void** vtable = *(void***)obj;
      return vtable[(u.v - 1) / sizeof(void*)];
   }

   static void* A(mfp_type f,
                  const char* action_type,
                  const char* return_type,
                  const char* class_type,
                  const char* method_name,
                  const char* return_args,
                  const char* file,
                  int line)
   {
      cast_type u{f};

      if (is_virtual(u)) {
         void* p = nullptr;
         void* o = h2_alloc::I().malloc(sizeof(Class));
         try {
            p = get_vmfp(u, h2_constructible<Class>::O(o));
         }
         catch (int e) {
            h2_alloc::I().free(o);
            h2_fail_instantiate* fail = new h2_fail_instantiate(file, line);
            fail->add(action_type, return_type, class_type, method_name, return_args, e);
            h2_fail_g(fail);
         }
         h2_alloc::I().free(o);
         return p;
      }
      else {
         return u.p;
      }
   }

   template <typename Derived>
   static void* A(mfp_type f, Derived obj)
   {
      cast_type u{f};

      if (is_virtual(u)) {
         return get_vmfp(u, dynamic_cast<Class*>(&obj));
      }
      else {
         return u.p;
      }
   }
};

#endif
