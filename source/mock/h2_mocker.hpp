struct h2_mocker_base : h2_libc {
   h2_list x;
   void *srcfp, *dstfp;
   const char *srcfn, *filine;
   char return_type[512];
   h2_vector<h2_string> argument_types;
   bool greed_mode = true;

   h2_line argument(int seq, const char* def = "") const;
   h2_line signature() const;

   h2_vector<h2_checkin> checkin_array;
   int checkin_index = 0;
   h2_fail* check() const;

   virtual void reset() = 0;
   void mock();

   void failing(h2_fail* fail, int checkin_offset) const;
};

namespace {

template <int Counter, typename Class, typename Signature> class h2_mocker;
template <int Counter, typename Class, typename ReturnType, typename... Args>
class h2_mocker<Counter, Class, ReturnType(Args...)> : h2_mocker_base {
   using ArgumentTuple = std::tuple<Args...>;

#define H2_Typedef_Matcher(__, i) h2_matcher<h2_nth_decay<i, Args...>>
   using MatcherTuple = std::tuple<H2PP_REPEAT((, ), H2_Typedef_Matcher, , 20)>;
#undef H2_Typedef_Matcher

   h2_vector<MatcherTuple> matcher_array;
   h2_vector<h2_routine<Class, ReturnType(Args...)>> routine_array;
   h2_routine<Class, ReturnType(Args...)> original;

   static ReturnType function_stub(Class* This, Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().srcfp);
      if (index == -1 || !I().routine_array[index])
         return I().original(This, std::forward<Args>(args)...);
      return I().routine_array[index](This, std::forward<Args>(args)...);
   }

   struct member_function_stub {  // wrap for calling conversions
      ReturnType fx(Args... args) { return function_stub((Class*)this, std::forward<Args>(args)...); }
   };

   static ReturnType normal_function_stub(Args... args) { return function_stub(nullptr, std::forward<Args>(args)...); }

   int matches(Args... args)
   {
      ArgumentTuple at = std::forward_as_tuple(std::forward<Args>(args)...);
      int checkin_offset = -1;
      for (int i = checkin_index; i < (int)checkin_array.size(); ++i) {
         h2_fail* fails = tuple_matches(matcher_array[i], at);
         if (fails) {
            if (checkin_offset != -1) break;
            if (checkin_array[i].is_satisfied()) { /* try next h2_checkin */
               delete fails;
               continue;
            }
            failing(fails, i);
         } else {
            checkin_index = i;
            checkin_offset = i;
            if (checkin_array[i].is_saturated()) continue;
            if (checkin_array[i].insufficient()) break;
            /* satisfied */
            if (greed_mode) break;
            /* continue */
         }
      }
      if (checkin_offset != -1) checkin_array[checkin_offset].call += 1;
      if (checkin_offset == -1) {
         h2_fail* fail = h2_fail::new_normal(signature(), filine);
         h2_fail* f = h2_fail::new_normal(srcfn + h2_stringify(at, true) + color(" unexpectedly", "red,bold") + " called");
         h2_fail::append_child(fail, f);
         h2_runner::failing(fail);
      }
      return checkin_offset;
   }

   void reset() override
   {
      checkin_array.clear();
      matcher_array.clear();
      routine_array.clear();
      checkin_index = 0;
      greed_mode = true;
   }

 public:
   static h2_mocker& I()
   {
      static h2_mocker* i = nullptr;
      if (!i) {
         i = new h2_mocker();
         h2_cxa::type_name<ReturnType>(i->return_type, sizeof(i->return_type));
         tuple_types<ArgumentTuple>(i->argument_types);
      }
      return *i;
   }

   static h2_mocker& I(void* srcfp, const char* srcfn, const char* filine)
   {
      if (std::is_same<std::false_type, Class>::value) {
         I().dstfp = (void*)normal_function_stub;
         I().original.fp = (ReturnType(*)(Args...))srcfp;
      } else {
         I().dstfp = h2::h2_fp<member_function_stub, ReturnType(Args...)>::get(&member_function_stub::fx);
         I().original.mfp = (ReturnType(*)(Class*, Args...))srcfp;
      }
      I().srcfp = srcfp;
      I().srcfn = srcfn;
      I().filine = filine;
      I().reset();
      I().mock();
      return I();
   }

   h2_mocker& greed(bool mode)
   {
      greed_mode = mode;
      return *this;
   }

#define H2_Default_Matcher(__, i) h2_matcher<h2_nth_decay<i, Args...>> _##i = {}
#define H2_Forward_Matcher(__, i) std::forward<h2_matcher<h2_nth_decay<i, Args...>>>(_##i)

   h2_mocker& Once(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Once());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Twice(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Twice());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Times(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Times(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Any(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Any());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Atleast(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atleast(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Atmost(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atmost(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& Between(int left, int right, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Between(left, right));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<Class, ReturnType(Args...)>());
      return *this;
   }

   h2_mocker& With(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      if (checkin_array.empty()) Any();
      matcher_array.back() = std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20));
      return *this;
   }

#undef H2_Default_Matcher
#undef H2_Forward_Matcher

#define H2_Th_Matcher(__, i)                                     \
   h2_mocker& Th##i(h2_matcher<h2_nth_decay<i, Args...>> e = {}) \
   {                                                             \
      if (checkin_array.empty()) Any();                          \
      std::get<i>(matcher_array.back()) = e;                     \
      return *this;                                              \
   }
   H2PP_REPEAT(, H2_Th_Matcher, , 20);
#undef H2_Th_Matcher

   h2_mocker& Return(h2_routine<Class, ReturnType(Args...)> r)
   {
      if (checkin_array.empty()) Any();
      if (!routine_array.empty()) routine_array.pop_back();
      routine_array.push_back(r);
      return *this;
   }

   void operator=(ReturnType (*f)(Args...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.fp = f;
   }

   void operator=(ReturnType (*f)(Class*, Args...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.mfp = f;
   }
};
}
