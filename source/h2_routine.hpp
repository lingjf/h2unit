
template <typename Class, typename F>
struct h2_routine;

template <typename Class, typename Return, typename... Args>
struct h2_routine<Class, Return(Args...)> {
   std::function<Return(Args...)> normal_function = {}; // functional alignment issue
   std::function<Return(Class*, Args...)> member_function = {};
   void* origin_function = nullptr;
   Return return_value;
   bool empty = false;

   h2_routine() { empty = true; }
   h2_routine(Return r) : return_value(r) {}
   h2_routine(std::function<Return(Args...)> f) : normal_function(f) {}
   h2_routine(std::function<Return(Class*, Args...)> f) : member_function(f) {}

   Return operator()(Class* that, Args... args)
   {
      if (origin_function) {
         if (std::is_same<std::false_type, Class>::value)
            return ((Return(*)(Args...))origin_function)(args...);
         else
            return ((Return(*)(Class*, Args...))origin_function)(that, args...);
      } else if (member_function) {
         return member_function(that, args...);
      } else if (normal_function) {
         return normal_function(args...);
      } else {
         return return_value;
      }
   }
};

template <typename Class, typename... Args>
struct h2_routine<Class, void(Args...)> {
   std::function<void(Args...)> normal_function = {};
   std::function<void(Class*, Args...)> member_function = {};
   void* origin_function = nullptr;
   bool empty = false;

   h2_routine() { empty = true; }
   h2_routine(std::function<void(Args...)> f) : normal_function(f) {}
   h2_routine(std::function<void(Class*, Args...)> f) : member_function(f) {}

   void operator()(Class* that, Args... args)
   {
      if (origin_function) {
         if (std::is_same<std::false_type, Class>::value)
            return ((void (*)(Args...))origin_function)(args...);
         else
            return ((void (*)(Class*, Args...))origin_function)(that, args...);
      } else if (member_function) {
         member_function(that, args...);
      } else if (normal_function) {
         normal_function(args...);
      }
   }
};
