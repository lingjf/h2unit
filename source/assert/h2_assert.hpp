struct h2_defer_failure : h2_once {
   const char* assert_type, *assert_op = ",";
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

template <typename M, typename A>
struct h2_expr2 {
   M m;
   A a;
   const char* op;
   h2_expr2(M m_, A a_, const char* op_) : m(m_), a(a_), op(op_) {}
   h2_ostringstream& ok(h2_defer_failure* d)
   {
      d->assert_type = "Ok2";
      d->assert_op = op;
      h2_fail* fail = h2::h2_matcher_cast<A>(m).matches(a, 0, false, false, true);
      d->fails = fail;
      if (fail && fail->subling_next) {
         d->fails = h2_fail::new_unexpect();
         h2_fail::append_child(d->fails, fail);
      }
      h2_assert_g();
      return d->oss;
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
struct h2_expr1 {
   T t;
   explicit h2_expr1(T t_) : t(t_) {}
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_expr2<decltype(Eq((E)t)), A> operator==(const U& u) const { return {Eq((E)t), (A)u, "=="}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_expr2<decltype(Nq((E)t)), A> operator!=(const U& u) const { return {Nq((E)t), (A)u, "!="}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_expr2<decltype(Lt((E)t)), A> operator>(const U& u) const { return {Lt((E)t), (A)u, ">"}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_expr2<decltype(Gt((E)t)), A> operator<(const U& u) const { return {Gt((E)t), (A)u, "<"}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_expr2<decltype(Le((E)t)), A> operator>=(const U& u) const { return {Le((E)t), (A)u, ">="}; }
   template <typename U, typename A = typename h2_decay<U>::type>
   h2_expr2<decltype(Ge((E)t)), A> operator<=(const U& u) const { return {Ge((E)t), (A)u, "<="}; }

   template <typename dummy>
   struct false_type {
      static constexpr bool value = false;
   };
   template <typename U>
   void operator&&(const U& u) const { static_assert(false_type<U>::value, "&& is not supported inside Ok, wrap the expression inside parentheses, or using OK instead of Ok"); }
   template <typename U>
   void operator||(const U& u) const { static_assert(false_type<U>::value, "|| is not supported inside Ok, wrap the expression inside parentheses, or using OK instead of Ok"); }
};

struct h2_expr12 {
   template <typename T>
   h2_expr1<const T&> operator>(const T& t) { return h2_expr1<const T&>{t}; }
};

template <typename T>
static inline h2_ostringstream& h2_ok12(h2_defer_failure* d, h2_expr1<T> a) { return h2_ok1(d, static_cast<bool>(a.t)); }
template <typename E, typename A>
static inline h2_ostringstream& h2_ok12(h2_defer_failure* d, h2_expr2<E, A> ea) { return ea.ok(d); }

/* clang-format off */
#define H2Ok(...) __H2Ok(H2PP_UNIQUE(), (#__VA_ARGS__), __VA_ARGS__)
#define __H2Ok(Q, expression, ...) \
   for (h2::h2_defer_failure Q("", expression, __FILE__, __LINE__); Q;) h2::h2_ok12(&Q, h2::h2_expr12() > __VA_ARGS__)

#define H2OK(_1, ...) H2PP_CAT(__H2OK, H2PP_IS_EMPTY(__VA_ARGS__)) (H2PP_UNIQUE(), #_1, (#__VA_ARGS__), _1, __VA_ARGS__)
#define __H2OK1(Q, a_expression, _dummy, actual, ...) \
   for (h2::h2_defer_failure Q("", a_expression, __FILE__, __LINE__); Q;) h2::h2_ok1(&Q, actual)
#define __H2OK0(Q, e_expression, a_expression, expect, ...) \
   for (h2::h2_defer_failure Q(e_expression, a_expression, __FILE__, __LINE__); Q;) h2::h2_ok2(&Q, expect, __VA_ARGS__)

#define H2JE(...) H2PP_VARIADIC_CALL(__H2JE, H2PP_UNIQUE(), __VA_ARGS__)
#define __H2JE3(Q, expect, actual) \
   for (h2::h2_defer_failure Q(#expect, #actual, __FILE__, __LINE__); Q;) h2::h2_je(&Q, expect, actual, "")
#define __H2JE4(Q, expect, actual, selector) \
   for (h2::h2_defer_failure Q(#expect, #actual, __FILE__, __LINE__); Q;) h2::h2_je(&Q, expect, actual, selector)
