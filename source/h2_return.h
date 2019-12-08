
#ifndef ___H2_RETURN__H___
#define ___H2_RETURN__H___

template <typename Class, typename F>
class h2_return;

template <typename Class, typename Return, typename... Args>
class h2_return<Class, Return(Args...)>
{
 private:
   Return _r;
   std::function<Return(Args...)> _f1;
   std::function<Return(Class*, Args...)> _f2;

 public:
   h2_return()
     : _r(), _f1(), _f2() {}
   h2_return(Return r)
     : _r(r), _f1(), _f2() {}
   h2_return(std::function<Return(Args...)> f)
     : _f1(f) {}
   h2_return(std::function<Return(Class*, Args...)> f)
     : _f2(f) {}

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
class h2_return<Class, void(Args...)>
{
 private:
   std::function<void(Args...)> _f1;
   std::function<void(Class*, Args...)> _f2;

 public:
   h2_return()
     : _f1(), _f2() {}
   h2_return(std::function<void(Args...)> f)
     : _f1(f) {}
   h2_return(std::function<void(Class*, Args...)> f)
     : _f2(f) {}

   void operator()(Class* that, Args... args)
   {
      if (_f2)
         _f2(that, args...);
      else if (_f1)
         _f1(args...);
      else
         ;
      return;
   }
};

#endif
