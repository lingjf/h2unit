template <typename T>
inline h2_matcher<T>::h2_matcher() { *this = h2_polymorphic_matcher<h2_matches_any>(h2_matches_any()); }

template <typename T>
inline h2_matcher<T>::h2_matcher(T value) { *this = _Eq(value); }
