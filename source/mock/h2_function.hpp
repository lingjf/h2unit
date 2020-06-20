
template <typename Return>
struct h2_return : h2_libc {
   Return value;
   h2_return() = delete;
   explicit h2_return(Return _value) : value(_value){};
};

template <typename Class, typename F>
struct h2_function;

template <typename Class, typename Return, typename... Args>
struct h2_function<Class, Return(Args...)> {
   std::function<Return(Args...)> normal_function = {};  // functional alignment issue
   std::function<Return(Class*, Args...)> member_function = {};
   h2_shared_ptr<h2_return<Return>> return_value;

   h2_function(Return r) : return_value(new h2_return<Return>(r)) {}
   h2_function(std::function<Return(Args...)> f) : normal_function(f) {}
   h2_function(std::function<Return(Class*, Args...)> f) : member_function(f) {}

   Return operator()(Class* that, Args... args)
   {
      if (member_function) {
         return member_function(that, args...);
      } else if (normal_function) {
         return normal_function(args...);
      } else if (return_value) {
         return return_value->value;
      }
      /* return uninitialized value */
   }
   void clear()
   {
      normal_function = nullptr;
      member_function = nullptr;
      return_value.reset();
   }
};

template <typename Class, typename... Args>
struct h2_function<Class, void(Args...)> {
   std::function<void(Args...)> normal_function = {};
   std::function<void(Class*, Args...)> member_function = {};

   h2_function() {}
   h2_function(std::function<void(Args...)> f) : normal_function(f) {}
   h2_function(std::function<void(Class*, Args...)> f) : member_function(f) {}

   void operator()(Class* that, Args... args)
   {
      if (member_function) {
         member_function(that, args...);
      } else if (normal_function) {
         normal_function(args...);
      }
   }
   void clear()
   {
      normal_function = nullptr;
      member_function = nullptr;
   }
};
