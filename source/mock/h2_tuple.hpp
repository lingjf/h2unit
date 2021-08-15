
static inline void h2_assert_g();

template <typename MatcherTuple, typename ArgumentTuple, std::size_t I>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, I>)
{
   h2_fail* fails = matches(matchers, arguments, std::integral_constant<std::size_t, I - 1>());
   h2_fail* fail = std::get<I - 1>(matchers).matches(std::get<I - 1>(arguments));
   if (fail) fail->seqno = I - 1;
   h2_fail::append_subling(fails, fail);
   h2_assert_g();
   return fails;
}

template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& arguments, std::integral_constant<std::size_t, 0>)
{
   return nullptr;
}

template <typename MatcherTuple, typename ArgumentTuple>
inline h2_fail* h2_tuple_matches(MatcherTuple& matchers, ArgumentTuple& arguments)
{
   return matches(matchers, arguments, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}

template <typename ArgumentTuple>
void h2_tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, 0>) {}
template <typename ArgumentTuple, std::size_t I>
void h2_tuple_types(h2_vector<h2_string>& names, std::integral_constant<std::size_t, I>)
{
   h2_tuple_types<ArgumentTuple>(names, std::integral_constant<std::size_t, I - 1>());
   names.push_back(h2_type_name<typename std::tuple_element<I - 1, ArgumentTuple>::type>((char*)alloca(256)));
}
template <typename ArgumentTuple>
void h2_tuple_types(h2_vector<h2_string>& names)
{
   return h2_tuple_types<ArgumentTuple>(names, std::integral_constant<std::size_t, std::tuple_size<ArgumentTuple>::value>());
}
