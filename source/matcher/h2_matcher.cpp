
template <typename T>
inline h2_matcher<T>::h2_matcher()
{
   *this = Any;
}

template <typename T>
inline h2_matcher<T>::h2_matcher(T value)
{
   *this = Eq(value);
}
