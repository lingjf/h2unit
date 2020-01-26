
struct h2_mock {
   h2_list x;
   void *befp, *tofp;
   const char* befn;
   const char* file;
   int line;

   h2_vector<h2_callx> c_array;
   int c_index;

   h2_mock(void* befp_, void* tofp_, const char* befn_, const char* file_, int line_)
     : befp(befp_), tofp(tofp_), befn(befn_), file(file_), line(line_), c_index(0) {}

   virtual void reset() = 0;

   h2_fail* times_check() {
      h2_fail* fail = nullptr;
      for (auto it = c_array.begin(); it != c_array.end(); it++) h2_append_y_fail(fail, (*it).check());
      if (fail) fail->locate(file, line, befn);
      return fail;
   }
};

static inline bool h2_mock_g(h2_mock*);

template <size_t I, typename T, typename... Args>
struct h2_nth_type_impl {
   using type = typename h2_nth_type_impl<I - 1, Args...>::type;
};

template <typename T, typename... Args>
struct h2_nth_type_impl<0, T, Args...> {
   using type = T;
};

template <size_t Index, typename... Args>
using h2_nth_type = typename h2_nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;

template <size_t Index, typename... Args>
using h2_nth_type_decay = typename std::decay<h2_nth_type<Index, Args...>>::type;

template <size_t N>
struct h2_tuple_match {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args, const char* file, int line, const char* func) {
      h2_fail* f0 = h2_tuple_match<N - 1>::matches(matchers, args, file, line, func);
      h2_fail* f1 = std::get<N - 1>(matchers).matches(std::get<N - 1>(args));
      if (f1) f1->locate(file, line, func, N - 1);
      h2_append_x_fail(f0, f1);
      return f0;
   }
};

template <>
struct h2_tuple_match<0> {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args, const char* file, int line, const char* func) {
      return nullptr;
   }
};

template <int Counter, int Lineno, typename Class, typename F>
class h2_mocker;

template <int Counter, int Lineno, typename Class, typename Return, typename... Args>
class h2_mocker<Counter, Lineno, Class, Return(Args...)> : h2_mock {
 private:
   h2_mocker(void* befp, const char* befn, const char* file, int line)
     : h2_mock(befp, std::is_same<std::false_type, Class>::value ? (void*)normal_function_stub : (void*)member_function_stub, befn, file, line) {}

/* clang-format off */
#define __H2_MATCHER_TYPE_LIST                \
   h2_matcher<h2_nth_type_decay<0, Args...>>, \
   h2_matcher<h2_nth_type_decay<1, Args...>>, \
   h2_matcher<h2_nth_type_decay<2, Args...>>, \
   h2_matcher<h2_nth_type_decay<3, Args...>>, \
   h2_matcher<h2_nth_type_decay<4, Args...>>, \
   h2_matcher<h2_nth_type_decay<5, Args...>>, \
   h2_matcher<h2_nth_type_decay<6, Args...>>, \
   h2_matcher<h2_nth_type_decay<7, Args...>>, \
   h2_matcher<h2_nth_type_decay<8, Args...>>, \
   h2_matcher<h2_nth_type_decay<9, Args...>>

#define __H2_MATCHER_PARAMETER_DEFAULT_LIST           \
   h2_matcher<h2_nth_type_decay<0, Args...>> a_0 = _, \
   h2_matcher<h2_nth_type_decay<1, Args...>> a_1 = _, \
   h2_matcher<h2_nth_type_decay<2, Args...>> a_2 = _, \
   h2_matcher<h2_nth_type_decay<3, Args...>> a_3 = _, \
   h2_matcher<h2_nth_type_decay<4, Args...>> a_4 = _, \
   h2_matcher<h2_nth_type_decay<5, Args...>> a_5 = _, \
   h2_matcher<h2_nth_type_decay<6, Args...>> a_6 = _, \
   h2_matcher<h2_nth_type_decay<7, Args...>> a_7 = _, \
   h2_matcher<h2_nth_type_decay<8, Args...>> a_8 = _, \
   h2_matcher<h2_nth_type_decay<9, Args...>> a_9 = _

#define __H2_MATCHER_ARGUMENT_LIST \
   a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7, a_8, a_9

   /* clang-format on */

   typedef std::tuple<Args..., int> argument_tuple;
   typedef std::tuple<__H2_MATCHER_TYPE_LIST> matcher_tuple;

   h2_vector<matcher_tuple> m_array;
   h2_vector<h2_routine<Class, Return(Args...)>> r_array;

   static Return normal_function_stub(Args... args) {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](nullptr, std::move(args)...);
   }

   static Return member_function_stub(Class* that, Args... args) {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](that, std::move(args)...);
   }

   h2_fail* matches(matcher_tuple& matchers, argument_tuple& args) {
      if (1 == std::tuple_size<argument_tuple>::value) return nullptr;
      return h2_tuple_match<std::tuple_size<argument_tuple>::value>::matches(matchers, args, file, line, befn);
   }

   int matches(Args... args) {
      argument_tuple a_tuple = std::make_tuple(args..., 0);
      int c_offset = -1;
      for (int i = c_index; i < (int)c_array.size(); ++i) {
         h2_fail* fail = matches(m_array[i], a_tuple);
         if (fail) {
            if (c_array[i].is_shortage()) h2_fail_g(fail);
            if (c_array[i].is_satisfied()) delete fail; /* continue; try next h2_callx */
         } else {
            ++c_array[c_offset = i];
            if (c_array[i].is_saturated()) c_index += 1;
            break;
         }
      }
      if (-1 == c_offset)
         h2_fail_g(new h2_fail_normal(file, line, befn, "unexpect call"));

      return c_offset;
   }

   void reset() override { c_array.clear(), m_array.clear(), r_array.clear(), c_index = 0; }

   h2_mocker& register_and_return_reference() {
      if (!h2_mock_g(this)) reset();
      return *this;
   }

 public:
   static h2_mocker& I(void* befp = nullptr, const char* befn = nullptr, const char* file = nullptr, int line = 0) {
      static h2_mocker* I = nullptr;
      if (!I) I = new (h2_raw::malloc(sizeof(h2_mocker))) h2_mocker(befp, befn, file, line);
      return I->register_and_return_reference();
   }

   h2_mocker& once(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      c_array.push_back(h2_callx(1, 1));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& twice(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      c_array.push_back(h2_callx(2, 2));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& times(int count) {
      c_array.push_back(h2_callx(count, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& any(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      c_array.push_back(h2_callx(0, INT_MAX));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& atleast(int count) {
      c_array.push_back(h2_callx(count, INT_MAX));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& atmost(int count) {
      c_array.push_back(h2_callx(0, count));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& between(int left, int right) {
      c_array.push_back(h2_callx(left, right));
      m_array.push_back(matcher_tuple());
      r_array.push_back(h2_routine<Class, Return(Args...)>());
      return register_and_return_reference();
   }

   h2_mocker& with(__H2_MATCHER_PARAMETER_DEFAULT_LIST) {
      if (!m_array.empty()) m_array.back() = std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST);
      return register_and_return_reference();
   }

   /* clang-format off */
   h2_mocker& th1(h2_matcher<h2_nth_type_decay<0, Args...>> a_=_) { if (!m_array.empty()) std::get<0>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th2(h2_matcher<h2_nth_type_decay<1, Args...>> a_=_) { if (!m_array.empty()) std::get<1>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th3(h2_matcher<h2_nth_type_decay<2, Args...>> a_=_) { if (!m_array.empty()) std::get<2>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th4(h2_matcher<h2_nth_type_decay<3, Args...>> a_=_) { if (!m_array.empty()) std::get<3>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th5(h2_matcher<h2_nth_type_decay<4, Args...>> a_=_) { if (!m_array.empty()) std::get<4>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th6(h2_matcher<h2_nth_type_decay<5, Args...>> a_=_) { if (!m_array.empty()) std::get<5>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th7(h2_matcher<h2_nth_type_decay<6, Args...>> a_=_) { if (!m_array.empty()) std::get<6>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th8(h2_matcher<h2_nth_type_decay<7, Args...>> a_=_) { if (!m_array.empty()) std::get<7>(m_array.back()) = a_; return register_and_return_reference(); }
   h2_mocker& th9(h2_matcher<h2_nth_type_decay<8, Args...>> a_=_) { if (!m_array.empty()) std::get<8>(m_array.back()) = a_; return register_and_return_reference(); }
   /* clang-format on */

   h2_mocker& returns(h2_routine<Class, Return(Args...)> r) {
      if (!r_array.empty()) r_array.back() = r;
      return register_and_return_reference();
   }

   h2_mocker& does(std::function<Return(Args...)> f) {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return register_and_return_reference();
   }

   h2_mocker& does(std::function<Return(Class*, Args...)> f) {
      if (!r_array.empty()) r_array.back() = h2_routine<Class, Return(Args...)>(f);
      return register_and_return_reference();
   }

   h2_mocker& operator=(std::function<Return(Args...)> f) { return does(f); }
   h2_mocker& operator=(std::function<Return(Class*, Args...)> f) { return does(f); }
};
