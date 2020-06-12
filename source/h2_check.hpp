
struct h2_defer_fail : h2_once {
   const char* check_type;
   const char *e_expression, *a_expression, *expression;
   const char* file;
   int line;
   h2_fail* fail{nullptr};
   h2_ostringstream oss;

   h2_defer_fail(const char* check_type_, const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_);
   ~h2_defer_fail();
};

static inline h2_ostringstream& h2_OK1(bool a, h2_defer_fail* d)
{
   if (!a) d->fail = new h2_fail_unexpect("true", "false");
   return d->oss;
}

template <typename E, typename A>
static inline h2_ostringstream& h2_OK2(E e, A a, h2_defer_fail* d)
{
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   h2_fail* fail = m.matches((typename h2_decay<A>::type)a);
   d->fail = fail;
   if (fail && fail->subling_next) {
      d->fail = new h2_fail_unexpect();
      h2_fail::append_child(d->fail, fail);
   }
   return d->oss;
}

static inline h2_ostringstream& h2_JE(h2_string e, h2_string a, h2_defer_fail* d)
{
   h2::h2_matcher<h2_string> m = Je(e);
   d->fail = m.matches(a);
   return d->oss;
}

#define __H2OK3(condition, expression, Qt) \
   for (h2::h2_defer_fail Qt("OK1", "", #condition, H2PP_REMOVE_PARENTHESES(expression), __FILE__, __LINE__); Qt;) h2::h2_OK1(condition, &Qt)

#define __H2OK4(expect, actual, expression, Qt) \
   for (h2::h2_defer_fail Qt("OK2", #expect, #actual, H2PP_REMOVE_PARENTHESES(expression), __FILE__, __LINE__); Qt;) h2::h2_OK2(expect, actual, &Qt)

#define __H2JE(expect, actual, Qt) \
   for (h2::h2_defer_fail Qt("JE", #expect, #actual, "", __FILE__, __LINE__); Qt;) h2::h2_JE(expect, actual, &Qt)

#define H2OK(...) H2PP_VARIADIC_CALL(__H2OK, __VA_ARGS__, (#__VA_ARGS__), H2Q(t_defer_fail))

#define H2JE(expect, actual) __H2JE(expect, actual, H2Q(t_defer_fail))
