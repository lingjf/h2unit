
static inline void h2_check_g();

template <int N>
struct h2_tuple_matcher {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, const char* file, int line, const char* func)
   {
      h2_fail* fail = h2_tuple_matcher<N - 1>::matches(matchers, arguments, file, line, func);
      h2_fail* f = std::get<N - 1>(matchers).matches(std::get<N - 1>(arguments));
      if (f) f->set_locate(file, line, func, N - 1);
      h2_fail::append_subling(fail, f);
      h2_check_g();
      return fail;
   }
};

template <>
struct h2_tuple_matcher<0> {
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, const char* file, int line, const char* func)
   {
      return nullptr;
   }
};
