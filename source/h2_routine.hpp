
template <typename Class, typename F>
struct h2_routine;

template <typename Class, typename Return, typename... Args>
struct h2_routine<Class, Return(Args...)> {
   Return _r;
   std::function<Return(Args...)> _f1;
   std::function<Return(Class*, Args...)> _f2;

   h2_routine() : _r(), _f1(), _f2() {}
   h2_routine(Return r) : _r(r), _f1(), _f2() {}
   h2_routine(std::function<Return(Args...)> f) : _f1(f) {}
   h2_routine(std::function<Return(Class*, Args...)> f) : _f2(f) {}

   Return operator()(Class* that, Args... args)
   {
      if (_f2)
         return _f2(that, args...);
      else if (_f1)
         return _f1(args...);
      else
         return _r;
   }
};

template <typename Class, typename... Args>
struct h2_routine<Class, void(Args...)> {
   std::function<void(Args...)> _f1;
   std::function<void(Class*, Args...)> _f2;

   h2_routine() : _f1(), _f2() {}
   h2_routine(std::function<void(Args...)> f) : _f1(f) {}
   h2_routine(std::function<void(Class*, Args...)> f) : _f2(f) {}

   void operator()(Class* that, Args... args)
   {
      if (_f2)
         _f2(that, args...);
      else if (_f1)
         _f1(args...);
   }
};
