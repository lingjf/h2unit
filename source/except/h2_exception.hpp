using nothrow = std::integral_constant<int, 32717>;

struct h2_exception {
   h2_singleton(h2_exception);
   jmp_buf catch_hole;
   bool catching = false;
   h2_backtrace last_bt;
   void* thrown_exception;
   std::type_info* type_info;
   char last_type[1024];

   static void initialize();

   static h2_once try_catch();
   static void try_catch_finally(h2_fail* fail, const char* what);

   template <typename T, typename M>
   static auto matches(T* a, M m) -> typename std::enable_if<std::is_base_of<std::exception, T>::value, h2_fail*>::type
   {
      return h2_matcher_cast<const char*>(m).matches(a->what(), {0});
   }

   template <typename T, typename M>
   static auto matches(T* a, M m) -> typename std::enable_if<!std::is_base_of<std::exception, T>::value, h2_fail*>::type
   {
      return h2_matcher_cast<T>(m).matches(*a, {0});
   }

   template <typename T>
   static auto what(T* a) -> typename std::enable_if<std::is_base_of<std::exception, T>::value, const char*>::type { return a->what(); }
   template <typename T>
   static auto what(T* a) -> typename std::enable_if<!std::is_base_of<std::exception, T>::value, const char*>::type { return nullptr; }

   template <typename T, typename M>
   static void check_catch(M m, const char* filine)
   {
      h2_fail* fail = nullptr;
      if (std::is_same<nothrow, T>::value) {  // no throw check
         if (h2_exception::I().thrown_exception)
            fail = h2_fail::new_exception("was thrown but expect no throw", h2_exception::I().last_type, h2_exception::I().last_bt, filine);
      } else {
         if (!h2_exception::I().thrown_exception) {
            fail = h2_fail::new_normal("expect exception " + color(h2_cxa::demangle(typeid(T).name()), "green") + " thrown but not", filine);
         } else {
            if (!(typeid(T) == *h2_exception::I().type_info)) {  // check type
               fail = h2_fail::new_exception("was thrown but expect type is " + color(h2_cxa::demangle(typeid(T).name()), "green"), h2_exception::I().last_type, h2_exception::I().last_bt, filine);
            } else {  // check value
               fail = matches((T*)h2_exception::I().thrown_exception, m);
               if (fail) {
                  fail->filine = filine;
                  h2_fail::append_child(fail, h2_fail::new_exception("was thrown", h2_exception::I().last_type, h2_exception::I().last_bt, filine));
               }
            }
         }
      }
      try_catch_finally(fail, what((T*)h2_exception::I().thrown_exception));
   }
};

#define __H2Catch0() for (auto _Q__ = h2::h2_exception::try_catch(); _Q__; h2::h2_exception::check_catch<nothrow>(_, H2_FILINE))
#define __H2Catch1(type) for (auto _Q__ = h2::h2_exception::try_catch(); _Q__; h2::h2_exception::check_catch<type>(_, H2_FILINE))
#define __H2Catch2(type, matcher) for (auto _Q__ = h2::h2_exception::try_catch(); _Q__; h2::h2_exception::check_catch<type>(matcher, H2_FILINE))
/* clang-format off */
#define H2Catch(...) H2PP_VCALL(__H2Catch, __VA_ARGS__) if (!::setjmp(h2::h2_exception::I().catch_hole))
