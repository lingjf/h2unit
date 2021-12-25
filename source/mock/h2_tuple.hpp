template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* __tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, 0>) { return nullptr; }
template <typename MatcherTuple, typename ArgumentTuple, std::size_t I>
inline h2_fail* __tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, I>)
{
   h2_fail* fails = __tuple_matches(matchers, arguments, std::integral_constant<std::size_t, I - 1>());
   h2_fail* fail = std::get<I - 1>(matchers).matches(std::get<I - 1>(arguments));
   if (fail) fail->seqno = I - 1;
   h2_fail::append_subling(fails, fail);
   h2_runner::asserts();
   return fails;
}
template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments)
{
   return __tuple_matches(matchers, arguments, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}

template <typename ArgumentTuple>
inline void __tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, 0>) {}
template <typename ArgumentTuple, std::size_t I>
inline void __tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, I>)
{
   __tuple_types<ArgumentTuple>(names, std::integral_constant<std::size_t, I - 1>());
   names.push_back(h2_cxa::type_name<typename std::tuple_element<I - 1, ArgumentTuple>::type>());
}
template <typename ArgumentTuple>
inline void tuple_types(h2_vector<h2_string>& names)
{
   return __tuple_types<ArgumentTuple>(names, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}
