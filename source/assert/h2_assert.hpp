struct h2_defer_failure : h2_once {
   const char* assert_type;
   const char *e_expression, *a_expression;
   const char* file;
   int line;
   h2_fail* fails{nullptr};
   h2_ostringstream oss;

   h2_defer_failure(const char* e_expression_, const char* a_expression_, const char* file_, int line_) : e_expression(e_expression_), a_expression(a_expression_), file(file_), line(line_) {}
   ~h2_defer_failure();
};

template <typename E, typename A>
static inline h2_ostringstream& h2_ok2(h2_defer_failure* d, E e, A a, int n = 0)
{
   d->assert_type = "OK2";
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   h2_fail* fail = m.matches((typename h2_decay<A>::type)a, n);
   d->fails = fail;
   if (fail && fail->subling_next) {
      d->fails = h2_fail::new_unexpect();
      h2_fail::append_child(d->fails, fail);
   }
   h2_assert_g();
   return d->oss;
}

static inline h2_ostringstream& h2_ok1(h2_defer_failure* d, bool a)
{
   d->assert_type = "OK1";
   if (!a) d->fails = h2_fail::new_unexpect("true", "false");
   h2_assert_g();
   return d->oss;
}

static inline h2_ostringstream& h2_je(h2_defer_failure* d, h2_string e, h2_string a, h2_string selector)
{
   d->assert_type = "JE";
   h2::h2_matcher<h2_string> m = Je(e, selector);
   d->fails = m.matches(a);
   h2_assert_g();
   return d->oss;
}

#define H2OK(_arg, ...) H2PP_CAT(__H2OK, H2PP_IS_EMPTY(__VA_ARGS__)) \
(H2PP_UNIQUE(), #_arg, (#__VA_ARGS__), _arg, __VA_ARGS__)
#define __H2OK1(Q, expression, _0, a, ...) \
   for (h2::h2_defer_failure Q("", expression, __FILE__, __LINE__); Q;) h2::h2_ok1(&Q, a)
#define __H2OK0(Q, e_expression, a_expression, e, ...) \
   for (h2::h2_defer_failure Q(e_expression, a_expression, __FILE__, __LINE__); Q;) h2::h2_ok2(&Q, e, __VA_ARGS__)

#define H2JE(...) H2PP_VARIADIC_CALL(__H2JE, H2PP_UNIQUE(), __VA_ARGS__)
#define __H2JE3(Q, expect, actual) \
   for (h2::h2_defer_failure Q(#expect, #actual, __FILE__, __LINE__); Q;) h2::h2_je(&Q, expect, actual, "")

#define __H2JE4(Q, expect, actual, selector) \
   for (h2::h2_defer_failure Q(#expect, #actual, __FILE__, __LINE__); Q;) h2::h2_je(&Q, expect, actual, selector)
