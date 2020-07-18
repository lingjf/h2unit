
template <int Counter, int Lineno, typename Class, typename Signature>
class h2_mocker;

template <int Counter, int Lineno, typename Class, typename Return, typename... Args>
class h2_mocker<Counter, Lineno, Class, Return(Args...)> : h2_mock {
   /* clang-format off */
#define MATCHER_Any_0_1_2_3_4_5_6_7_8_9           \
   h2_matcher<h2_nth_decay<0, Args...>> _0 = Any, \
   h2_matcher<h2_nth_decay<1, Args...>> _1 = Any, \
   h2_matcher<h2_nth_decay<2, Args...>> _2 = Any, \
   h2_matcher<h2_nth_decay<3, Args...>> _3 = Any, \
   h2_matcher<h2_nth_decay<4, Args...>> _4 = Any, \
   h2_matcher<h2_nth_decay<5, Args...>> _5 = Any, \
   h2_matcher<h2_nth_decay<6, Args...>> _6 = Any, \
   h2_matcher<h2_nth_decay<7, Args...>> _7 = Any, \
   h2_matcher<h2_nth_decay<8, Args...>> _8 = Any, \
   h2_matcher<h2_nth_decay<9, Args...>> _9 = Any

// #define FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9

#define FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9                \
   std::forward<h2_matcher<h2_nth_decay<0, Args...>>>(_0), \
   std::forward<h2_matcher<h2_nth_decay<1, Args...>>>(_1), \
   std::forward<h2_matcher<h2_nth_decay<2, Args...>>>(_2), \
   std::forward<h2_matcher<h2_nth_decay<3, Args...>>>(_3), \
   std::forward<h2_matcher<h2_nth_decay<4, Args...>>>(_4), \
   std::forward<h2_matcher<h2_nth_decay<5, Args...>>>(_5), \
   std::forward<h2_matcher<h2_nth_decay<6, Args...>>>(_6), \
   std::forward<h2_matcher<h2_nth_decay<7, Args...>>>(_7), \
   std::forward<h2_matcher<h2_nth_decay<8, Args...>>>(_8), \
   std::forward<h2_matcher<h2_nth_decay<9, Args...>>>(_9)
   /* clang-format on */

   using ArgumentTuple = std::tuple<Args...>;
   using MatcherTuple = std::tuple<h2_matcher<h2_nth_decay<0, Args...>>,
                                   h2_matcher<h2_nth_decay<1, Args...>>,
                                   h2_matcher<h2_nth_decay<2, Args...>>,
                                   h2_matcher<h2_nth_decay<3, Args...>>,
                                   h2_matcher<h2_nth_decay<4, Args...>>,
                                   h2_matcher<h2_nth_decay<5, Args...>>,
                                   h2_matcher<h2_nth_decay<6, Args...>>,
                                   h2_matcher<h2_nth_decay<7, Args...>>,
                                   h2_matcher<h2_nth_decay<8, Args...>>,
                                   h2_matcher<h2_nth_decay<9, Args...>>>;

   h2_vector<MatcherTuple> matcher_array;
   h2_vector<h2_function<Class, Return(Args...)>> function_array;
   bool greed_mode = true;

   static Return normal_function_stub(Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().origin_fp);
      return I().function_array[index](nullptr, std::forward<Args>(args)...);
   }

   static Return member_function_stub(Class* that, Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      h2::h2_stub_temporary_restore t(I().origin_fp);
      return I().function_array[index](that, std::forward<Args>(args)...);
   }

   int matches(Args... args)
   {
      ArgumentTuple at = std::forward_as_tuple(std::forward<Args>(args)...);
      int checkin_offset = -1;
      for (int i = checkin_index; i < checkin_array.size(); ++i) {
         h2_fail* fail = h2_tuple_matches(matcher_array[i], at);
         if (fail) {
            if (checkin_offset != -1) {
               break;
            }
            if (checkin_array[i].is_satisfied()) {
               if (i < checkin_array.size() - 1) { /* try next h2_checkin */
                  delete fail;
                  continue;
               }
            }
            fail->foreach([this](h2_fail* f, int, int) { f->file = file, f->line = line, f->func = origin_fn; });
            h2_fail_g(fail, false);
         } else {
            checkin_index = i;
            checkin_offset = i;
            if (checkin_array[i].is_saturated()) {
               continue;
            }
            if (checkin_array[i].is_not_enough()) {
               break;
            }
            /* satisfied */
            if (greed_mode) {
               break;
            }
            /* continue */
         }
      }
      if (checkin_offset != -1) {
         ++checkin_array[checkin_offset];
      }
      if (checkin_offset == -1) {
         h2_fail_g(h2_fail::new_call(origin_fn, "", "unexpect", {}, file, line), false);
      }
      return checkin_offset;
   }

   void reset() override
   {
      checkin_array.clear();
      matcher_array.clear();
      function_array.clear();
      checkin_index = 0;
      greed_mode = true;
   }

   h2_function<Class, Return(Args...)> origin()
   {
      if (std::is_same<std::false_type, Class>::value)
         return h2_function<Class, Return(Args...)>(std::function<Return(Args...)>((Return(*)(Args...))origin_fp));
      else
         return h2_function<Class, Return(Args...)>(std::function<Return(Class*, Args...)>((Return(*)(Class*, Args...))origin_fp));
   }

 public:
   static h2_mocker& I()
   {
      static h2_mocker* i = nullptr;
      if (!i) i = new h2_mocker();
      return *i;
   }

   static h2_mocker& I(void* origin_fp, const char* origin_fn, const char* file, int line)
   {
      I().origin_fp = origin_fp;
      I().substitute_fp = std::is_same<std::false_type, Class>::value ? (void*)normal_function_stub : (void*)member_function_stub;
      I().origin_fn = origin_fn;
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

   h2_mocker& once(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      checkin_array.push_back(h2_checkin(1, 1));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& twice(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      checkin_array.push_back(h2_checkin(2, 2));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& times(int count)
   {
      checkin_array.push_back(h2_checkin(count, count));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& any(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      checkin_array.push_back(h2_checkin(0, INT_MAX));
      matcher_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& atleast(int count)
   {
      checkin_array.push_back(h2_checkin(count, INT_MAX));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& atmost(int count)
   {
      checkin_array.push_back(h2_checkin(0, count));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& between(int left, int right)
   {
      checkin_array.push_back(h2_checkin(left, right));
      matcher_array.push_back(MatcherTuple());
      function_array.push_back(origin());
      return *this;
   }

   h2_mocker& with(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      if (!matcher_array.empty()) {
         matcher_array.back() = std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9);
      }
      return *this;
   }

   /* clang-format off */
   h2_mocker& th1(h2_matcher<h2_nth_decay<0, Args...>> e = Any) { if (!matcher_array.empty()) std::get<0>(matcher_array.back()) = e; return *this; }
   h2_mocker& th2(h2_matcher<h2_nth_decay<1, Args...>> e = Any) { if (!matcher_array.empty()) std::get<1>(matcher_array.back()) = e; return *this; }
   h2_mocker& th3(h2_matcher<h2_nth_decay<2, Args...>> e = Any) { if (!matcher_array.empty()) std::get<2>(matcher_array.back()) = e; return *this; }
   h2_mocker& th4(h2_matcher<h2_nth_decay<3, Args...>> e = Any) { if (!matcher_array.empty()) std::get<3>(matcher_array.back()) = e; return *this; }
   h2_mocker& th5(h2_matcher<h2_nth_decay<4, Args...>> e = Any) { if (!matcher_array.empty()) std::get<4>(matcher_array.back()) = e; return *this; }
   h2_mocker& th6(h2_matcher<h2_nth_decay<5, Args...>> e = Any) { if (!matcher_array.empty()) std::get<5>(matcher_array.back()) = e; return *this; }
   h2_mocker& th7(h2_matcher<h2_nth_decay<6, Args...>> e = Any) { if (!matcher_array.empty()) std::get<6>(matcher_array.back()) = e; return *this; }
   h2_mocker& th8(h2_matcher<h2_nth_decay<7, Args...>> e = Any) { if (!matcher_array.empty()) std::get<7>(matcher_array.back()) = e; return *this; }
   h2_mocker& th9(h2_matcher<h2_nth_decay<8, Args...>> e = Any) { if (!matcher_array.empty()) std::get<8>(matcher_array.back()) = e; return *this; }
   /* clang-format on */

   h2_mocker& returns()
   {
      if (!function_array.empty()) {
         function_array.back().clear();
      }
      return *this;
   }

   h2_mocker& returns(h2_function<Class, Return(Args...)> r)
   {
      if (!function_array.empty()) {
         function_array.pop_back();
         function_array.push_back(r);
      }
      return *this;
   }

   h2_mocker& does(std::function<Return(Args...)> f)
   {
      if (!function_array.empty()) {
         function_array.pop_back();
         function_array.push_back(h2_function<Class, Return(Args...)>(f));
      }
      return *this;
   }

   h2_mocker& does(std::function<Return(Class*, Args...)> f)
   {
      if (!function_array.empty()) {
         function_array.pop_back();
         function_array.push_back(h2_function<Class, Return(Args...)>(f));
      }
      return *this;
   }

   h2_mocker& operator=(std::function<Return(Args...)> f) { return does(f); }
   h2_mocker& operator=(std::function<Return(Class*, Args...)> f) { return does(f); }
};
