
template <typename ReturnType>
struct h2_return : h2_libc {
   ReturnType value;
   h2_return() = delete;
   explicit h2_return(ReturnType _value) : value(_value){};
};

template <typename Class, typename Signature>
struct h2_routine;

template <typename Class, typename ReturnType, typename... Args>
struct h2_routine<Class, ReturnType(Args...)> {
   ReturnType (*nfp)(Args...) = nullptr; // normal function pointer
   ReturnType (*mfp)(Class*, Args...) = nullptr; // member function pointer
   h2_shared_ptr<h2_return<ReturnType>> return_value;

   h2_routine() {}
   h2_routine(ReturnType r) : return_value(new h2_return<ReturnType>(r)) {}
   h2_routine(ReturnType (*f)(Args...)) : nfp(f) {}
   h2_routine(ReturnType (*f)(Class*, Args...)) : mfp(f) {}

   ReturnType operator()(Class* This, Args... args)
   {
      if (mfp) {
         return mfp(This, args...);
      } else if (nfp) {
         return nfp(args...);
      } else if (return_value) {
         return return_value->value;
      }
      /* return uninitialized value */
   }
   operator bool()
   {
      return nfp || mfp || return_value;
   }
   void clear()
   {
      nfp = nullptr;
      mfp = nullptr;
      return_value.reset();
   }
};

template <typename Class, typename... Args>
struct h2_routine<Class, void(Args...)> {
   void (*nfp)(Args...) = nullptr;
   void (*mfp)(Class*, Args...) = nullptr;

   h2_routine() {}
   h2_routine(void (*f)(Args...)) : nfp(f) {}
   h2_routine(void (*f)(Class*, Args...)) : mfp(f) {}

   void operator()(Class* This, Args... args)
   {
      if (mfp) {
         mfp(This, args...);
      } else if (nfp) {
         nfp(args...);
      }
   }
   operator bool()
   {
      return nfp || mfp;
   }
   void clear()
   {
      nfp = nullptr;
      mfp = nullptr;
   }
};
