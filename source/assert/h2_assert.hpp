struct h2_assert : h2_once {
   h2_fail* fails = nullptr;
   h2_ostringstream oss;
   h2_ostringstream& stash(h2_fail* fail, const char* assert_type, const char* assert_op = ",");
   void failing(const char* e_expression, const char* a_expression, const char* filine);
};

static inline h2_ostringstream& h2_je(h2_assert* d, h2_string e, h2_string a, h2_string selector)
{
   h2::h2_matcher<h2_string> m = Je(e, selector);
   h2_fail* fail = m.matches(a);
   return d->stash(fail, "JE");
}

template <typename E, typename A>
static inline h2_ostringstream& h2_ok2(h2_assert* d, E e, const A& a, std::false_type, int)
{
   h2_fail* fail = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e).matches(a, {});
   return d->stash(fail, "OK2");
}

template <typename E, typename A>
static inline h2_ostringstream& h2_ok2(h2_assert* d, E e, const A a, std::true_type, int n)
{
   h2_fail* fail = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e).matches((typename h2_decay<A>::type)a, {n});
   return d->stash(fail, "OK2");
}

template <typename M, typename A>
struct h2_2cp {
   M m;
   A a;
   const char* op;
   h2_2cp(M m_, A a_, const char* op_) : m(m_), a(a_), op(op_) {}
};

template <typename U, typename E = typename h2_decay<U>::type>
struct h2_1cp {
   U a;
   explicit h2_1cp(U u) : a(u) {}
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Equals((E)a)), A> operator==(const V& v) const { return {Equals((E)a), (A)v, "=="}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Nq((E)a)), A> operator!=(const V& v) const { return {Nq((E)a), (A)v, "!="}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Lt((E)a)), A> operator>(const V& v) const { return {Lt((E)a), (A)v, ">"}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Gt((E)a)), A> operator<(const V& v) const { return {Gt((E)a), (A)v, "<"}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Le((E)a)), A> operator>=(const V& v) const { return {Le((E)a), (A)v, ">="}; }
   template <typename V, typename A = typename h2_decay<V>::type>
   h2_2cp<decltype(Ge((E)a)), A> operator<=(const V& v) const { return {Ge((E)a), (A)v, "<="}; }
};

struct h2_0cp {
   template <typename T>
   h2_1cp<const T&> operator>(const T& t) const { return h2_1cp<const T&>{t}; }
};

template <typename E, typename A>
static inline h2_ostringstream& h2_ok1(h2_assert* d, h2_2cp<E, A> c2)
{
   h2_fail* fail = h2::h2_matcher_cast<A>(c2.m).matches(c2.a, {-1, true});
   return d->stash(fail, "OK2", c2.op);
}

template <typename A>
static inline h2_ostringstream& h2_ok1(h2_assert* d, h2_1cp<A> c1)
{
   h2_fail* fail = nullptr;
   if (!c1.a) fail = h2_fail::new_unexpect("true", h2_stringify(c1.a, true));
   return d->stash(fail, "OK1");
}

#define H2OK(_1, ...) H2PP_CAT(H2OK_, H2PP_IS_EMPTY(__VA_ARGS__))(H2PP_UNIQUE(), #_1, (#__VA_ARGS__), _1, __VA_ARGS__)
#define H2OK_1(Q, expression, _, actual, ...) \
   for (h2::h2_assert Q; Q; Q.failing("", expression, H2_FILINE)) h2::h2_ok1(&Q, h2::h2_0cp() > actual)
#define H2OK_0(Q, e_expression, a_expression, expect, actual) \
   for (h2::h2_assert Q; Q; Q.failing(e_expression, a_expression, H2_FILINE)) h2::h2_ok2(&Q, expect, actual, std::is_array<decltype(actual)>{}, std::extent<decltype(actual)>::value)

#define H2JE(...) H2PP_VCALL(H2JE_, H2PP_UNIQUE(), __VA_ARGS__)
#define H2JE_3(Q, expect, actual) \
   for (h2::h2_assert Q; Q; Q.failing(#expect, #actual, H2_FILINE)) h2::h2_je(&Q, expect, actual, "")
#define H2JE_4(Q, expect, actual, selector) \
   for (h2::h2_assert Q; Q; Q.failing(#expect, #actual, H2_FILINE)) h2::h2_je(&Q, expect, actual, selector)
