
H2MATCHER(IsEven) { return a % 2 == 0; }
H2MATCHER(IsOdd) { return a % 2 != 0; }
H2MATCHER(IsDivable, expect) { return a % expect == 0; }
H2MATCHER(IsInteger) { return ::floor(a) == a; }
H2MATCHER(IsNaN) { return std::isnan(a); }
