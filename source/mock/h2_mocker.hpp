
struct h2_mocker_base : h2_libc {
   h2_list x;
   void *srcfp, *dstfp;
   const char* srcfn;
   char return_type[512];
   h2_vector<h2_string> argument_types;
   const char* file;
   int line;
   bool greed_mode = true;

   h2_row argument(int seq = -1);
   h2_row signature();

   h2_vector<h2_checkin> checkin_array;
   int checkin_index = 0;
   h2_fail* check();

   virtual void reset() = 0;
   void mock();
};

namespace {

template <int Counter, typename ClassType, typename Signature>
class h2_mocker;

template <int Counter, typename ClassType, typename ReturnType, typename... ArgumentTypes>
class h2_mocker<Counter, ClassType, ReturnType(ArgumentTypes...)> : h2_mocker_base {
   using ArgumentTypeTuple = std::tuple<ArgumentTypes...>;

#define H2_Typedef_Matcher(__, i) h2_matcher<h2_nth_decay<i, ArgumentTypes...>>
   using MatcherTuple = std::tuple<H2PP_REPEAT((, ), H2_Typedef_Matcher, , 20)>;
#undef H2_Typedef_Matcher

   h2_vector<MatcherTuple> matcher_array;
   h2_vector<h2_routine<ClassType, ReturnType(ArgumentTypes...)>> routine_array;
   h2_routine<ClassType, ReturnType(ArgumentTypes...)> original;

   static ReturnType member_function_stub(ClassType* This, ArgumentTypes... arguments)
   {
      int index = I().matches(std::forward<ArgumentTypes>(arguments)...);
      h2::h2_stub_temporary_restore t(I().srcfp);
      if (index == -1 || !I().routine_array[index])
         return I().original(This, std::forward<ArgumentTypes>(arguments)...);
      return I().routine_array[index](This, std::forward<ArgumentTypes>(arguments)...);
   }

   static ReturnType normal_function_stub(ArgumentTypes... arguments)
   {
      return member_function_stub(nullptr, std::forward<ArgumentTypes>(arguments)...);
   }

   int matches(ArgumentTypes... arguments)
   {
      ArgumentTypeTuple at = std::forward_as_tuple(std::forward<ArgumentTypes>(arguments)...);
      int checkin_offset = -1;
      for (int i = checkin_index; i < checkin_array.size(); ++i) {
         h2_fail* fails = h2_tuple_matches(matcher_array[i], at);
         if (fails) {
            if (checkin_offset != -1) break;
            if (checkin_array[i].is_satisfied()) { /* try next h2_checkin */
               delete fails;
               continue;
            }
            fails->foreach([this, i](h2_fail* f, int, int) {
               f->explain += gray("on ") + (srcfn + argument(f->seqno));
               if (1 < checkin_array.size()) f->explain += gray(" when ") + h2_numeric::sequence_number(i) + " " + color(checkin_array[i].expr, "cyan");
            });
            h2_fail* fail = h2_fail::new_normal(signature(), file, line);
            h2_fail::append_child(fail, fails);
            h2_fail_g(fail);
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
         h2_fail* fail = h2_fail::new_normal(signature(), file, line);
         h2_fail* f = h2_fail::new_normal(srcfn + h2_representify(at) + color(" unexpectedly", "red,bold") + " called");
         h2_fail::append_child(fail, f);
         h2_fail_g(fail);
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
         h2_tuple_types<ArgumentTypeTuple>(i->argument_types);
      }
      return *i;
   }

   static h2_mocker& I(void* srcfp, const char* srcfn, const char* file, int line)
   {
      if (std::is_same<std::false_type, ClassType>::value) {
         I().dstfp = (void*)normal_function_stub;
         I().original.fp = (ReturnType(*)(ArgumentTypes...))srcfp;
      } else {
         I().dstfp = (void*)member_function_stub;
         I().original.mfp = (ReturnType(*)(ClassType*, ArgumentTypes...))srcfp;
      }
      I().srcfp = srcfp;
      I().srcfn = srcfn;
      I().file = file;
      I().line = line;
      I().reset();
      I().mock();
      return I();
   }

   h2_mocker& greed(bool mode)
   {
      greed_mode = mode;
      return *this;
   }

#define H2_Default_Matcher(__, i) h2_matcher<h2_nth_decay<i, ArgumentTypes...>> _##i = {}
#define H2_Forward_Matcher(__, i) std::forward<h2_matcher<h2_nth_decay<i, ArgumentTypes...>>>(_##i)

   h2_mocker& Once(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Once());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Twice(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Twice());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Times(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Times(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Any(H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Any());
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Atleast(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atleast(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Atmost(int count, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Atmost(count));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
      return *this;
   }

   h2_mocker& Between(int left, int right, H2PP_REPEAT((, ), H2_Default_Matcher, , 20))
   {
      checkin_array.push_back(h2_checkin::Between(left, right));
      matcher_array.push_back(std::forward_as_tuple(H2PP_REPEAT((, ), H2_Forward_Matcher, , 20)));
      routine_array.push_back(h2_routine<ClassType, ReturnType(ArgumentTypes...)>());
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

#define H2_Th_Matcher(__, i)                                              \
   h2_mocker& Th##i(h2_matcher<h2_nth_decay<i, ArgumentTypes...>> e = {}) \
   {                                                                      \
      if (checkin_array.empty()) Any();                                   \
      std::get<i>(matcher_array.back()) = e;                              \
      return *this;                                                       \
   }
   H2PP_REPEAT(, H2_Th_Matcher, , 20);
#undef H2_Th_Matcher

   h2_mocker& Return(h2_routine<ClassType, ReturnType(ArgumentTypes...)> r)
   {
      if (checkin_array.empty()) Any();
      if (!routine_array.empty()) routine_array.pop_back();
      routine_array.push_back(r);
      return *this;
   }

   void operator=(ReturnType (*f)(ArgumentTypes...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.fp = f;
   }

   void operator=(ReturnType (*f)(ClassType*, ArgumentTypes...))
   {
      if (checkin_array.empty()) Any();
      for (auto& a : routine_array)
         if (!a) a.mfp = f;
   }
};

}  // namespace
