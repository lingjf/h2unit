
static inline void h2_check_g();

struct h2_mock : h2_libc {
   h2_list x;
   void *origin_fp, *substitute_fp;
   const char* origin_fn;
   const char* file;
   int line;

   h2_vector<h2_attendance> attendance_array;
   int attendance_index = 0;

   h2_mock(void* origin_fp_, void* substitute_fp_, const char* origin_fn_, const char* file_, int line_)
     : origin_fp(origin_fp_), substitute_fp(substitute_fp_), origin_fn(origin_fn_), file(file_), line(line_) {}

   virtual void reset() = 0;

   h2_fail* times_check();
};

static inline void h2_mock_g(h2_mock*);

template <size_t I, typename T, typename... Args>
struct __nth_type_impl {
   using type = typename __nth_type_impl<I - 1, Args...>::type;
};
template <typename T, typename... Args>
struct __nth_type_impl<0, T, Args...> {
   using type = T;
};
template <size_t Index, typename... Args>
struct h2_nth_type {
   using type = typename __nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;
};

template <size_t Index, typename... Args>
using h2_nth_decay = typename h2_decay<typename h2_nth_type<Index, Args...>::type>::type;

template <size_t N>
struct h2_tuple_match {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, const char* file, int line, const char* func)
   {
      h2_fail* fail = h2_tuple_match<N - 1>::matches(matchers, arguments, file, line, func);
      h2_fail* f = std::get<N - 1>(matchers).matches(std::get<N - 1>(arguments));
      if (f) f->set_locate(file, line, func, N - 1);
      h2_fail::append_subling(fail, f);
      h2_check_g();
      return fail;
   }
};

template <>
struct h2_tuple_match<0> {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, const char* file, int line, const char* func)
   {
      return nullptr;
   }
};

template <int Counter, int Lineno, typename Class, typename F>
class h2_mocker;

template <int Counter, int Lineno, typename Class, typename Return, typename... Args>
class h2_mocker<Counter, Lineno, Class, Return(Args...)> : h2_mock {
 private:
   h2_mocker(void* origin_fp, const char* origin_fn, const char* file, int line)
     : h2_mock(origin_fp, std::is_same<std::false_type, Class>::value ? (void*)normal_function_stub : (void*)member_function_stub, origin_fn, file, line) {}

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

   using ArgumentTuple = std::tuple<Args..., int>;
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

   h2_vector<MatcherTuple> matchers_array;
   h2_vector<h2_function<Class, Return(Args...)>> function_array;

   static Return normal_function_stub(Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      if (!I().function_array[index].empty)
         return I().function_array[index](nullptr, std::forward<Args>(args)...);

      h2::h2_stub_temporary_restore t(I().origin_fp);
      I().function_array[index].origin_function = I().origin_fp;
      return I().function_array[index](nullptr, std::forward<Args>(args)...);
   }

   static Return member_function_stub(Class* that, Args... args)
   {
      int index = I().matches(std::forward<Args>(args)...);
      if (!I().function_array[index].empty)
         return I().function_array[index](that, std::forward<Args>(args)...);

      h2::h2_stub_temporary_restore t(I().origin_fp);
      I().function_array[index].origin_function = I().origin_fp;
      return I().function_array[index](that, std::forward<Args>(args)...);
   }

   h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments)
   {
      if (1 == std::tuple_size<ArgumentTuple>::value) return nullptr;
      return h2_tuple_match<std::tuple_size<ArgumentTuple>::value>::matches(matchers, arguments, file, line, origin_fn);
   }

   int matches(Args... args)
   {
      ArgumentTuple at = std::forward_as_tuple(std::forward<Args>(args)..., 0);
      int attendance_offset = -1;
      for (int i = attendance_index; i < attendance_array.size(); ++i) {
         h2_fail* fail = matches(matchers_array[i], at);
         if (fail) {
            if (attendance_array[i].is_not_enough()) h2_fail_g(fail, false);
            if (attendance_array[i].is_satisfied()) delete fail; /* continue; try next h2_attendance */
         } else {
            ++attendance_array[attendance_offset = i];
            if (attendance_array[i].is_saturated()) attendance_index += 1;
            break;
         }
      }
      if (-1 == attendance_offset) {
         h2_fail_g(new h2_fail_call(origin_fn, "", "exceed", file, line), false);
      }
      return attendance_offset;
   }

   void reset() override
   {
      attendance_array.clear();
      matchers_array.clear();
      function_array.clear();
      attendance_index = 0;
   }

 public:
   static h2_mocker& I(void* origin_fp = nullptr, const char* origin_fn = nullptr, const char* file = nullptr, int line = 0)
   {
      static h2_mocker* i = nullptr;
      if (!i) i = new h2_mocker(origin_fp, origin_fn, file, line);
      if (origin_fp && file) {
         i->reset();
         h2_mock_g(i);
      }
      return *i;
   }

   h2_mocker& once(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      attendance_array.push_back(h2_attendance(1, 1));
      matchers_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(h2_function<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& twice(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      attendance_array.push_back(h2_attendance(2, 2));
      matchers_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(h2_function<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& times(int count)
   {
      attendance_array.push_back(h2_attendance(count, count));
      matchers_array.push_back(MatcherTuple());
      function_array.push_back(h2_function<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& any(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      attendance_array.push_back(h2_attendance(0, INT_MAX));
      matchers_array.push_back(std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9));
      function_array.push_back(h2_function<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atleast(int count)
   {
      attendance_array.push_back(h2_attendance(count, INT_MAX));
      matchers_array.push_back(MatcherTuple());
      function_array.push_back(h2_function<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atmost(int count)
   {
      attendance_array.push_back(h2_attendance(0, count));
      matchers_array.push_back(MatcherTuple());
      function_array.push_back(h2_function<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& between(int left, int right)
   {
      attendance_array.push_back(h2_attendance(left, right));
      matchers_array.push_back(MatcherTuple());
      function_array.push_back(h2_function<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& with(MATCHER_Any_0_1_2_3_4_5_6_7_8_9)
   {
      if (!matchers_array.empty()) matchers_array.back() = std::forward_as_tuple(FORWARD_Matcher_0_1_2_3_4_5_6_7_8_9);
      return *this;
   }

   /* clang-format off */
   h2_mocker& th1(h2_matcher<h2_nth_decay<0, Args...>> e = Any) { if (!matchers_array.empty()) std::get<0>(matchers_array.back()) = e; return *this; }
   h2_mocker& th2(h2_matcher<h2_nth_decay<1, Args...>> e = Any) { if (!matchers_array.empty()) std::get<1>(matchers_array.back()) = e; return *this; }
   h2_mocker& th3(h2_matcher<h2_nth_decay<2, Args...>> e = Any) { if (!matchers_array.empty()) std::get<2>(matchers_array.back()) = e; return *this; }
   h2_mocker& th4(h2_matcher<h2_nth_decay<3, Args...>> e = Any) { if (!matchers_array.empty()) std::get<3>(matchers_array.back()) = e; return *this; }
   h2_mocker& th5(h2_matcher<h2_nth_decay<4, Args...>> e = Any) { if (!matchers_array.empty()) std::get<4>(matchers_array.back()) = e; return *this; }
   h2_mocker& th6(h2_matcher<h2_nth_decay<5, Args...>> e = Any) { if (!matchers_array.empty()) std::get<5>(matchers_array.back()) = e; return *this; }
   h2_mocker& th7(h2_matcher<h2_nth_decay<6, Args...>> e = Any) { if (!matchers_array.empty()) std::get<6>(matchers_array.back()) = e; return *this; }
   h2_mocker& th8(h2_matcher<h2_nth_decay<7, Args...>> e = Any) { if (!matchers_array.empty()) std::get<7>(matchers_array.back()) = e; return *this; }
   h2_mocker& th9(h2_matcher<h2_nth_decay<8, Args...>> e = Any) { if (!matchers_array.empty()) std::get<8>(matchers_array.back()) = e; return *this; }
   /* clang-format on */

   h2_mocker& returns()
   {
      if (!function_array.empty()) {
         function_array.back().empty = false;
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

struct h2_mocks {
   h2_list mocks;
   bool add(h2_mock* mock);
   h2_fail* clear();
};

#ifdef _WIN32
#   define __H2_LINE__ 0
#else
#   define __H2_LINE__ __LINE__
#endif

#define __H2MOCK2(BeFunc, Signature) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, std::false_type, Signature>::I((void*)BeFunc, #BeFunc, __FILE__, __LINE__)

#define __H2MOCK3(Class, Method, Signature) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, Class, Signature>::I(h2::h2_mfp<Class, Signature>::A(&Class::Method, "MOCK", "", #Class, #Method, #Signature, __FILE__, __LINE__), #Class "::" #Method, __FILE__, __LINE__)

#define __H2MOCK4(Class, Method, Signature, Instance) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, Class, Signature>::I(h2::h2_mfp<Class, Signature>::A(&Class::Method, Instance), #Class "::" #Method, __FILE__, __LINE__)

#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)
