#define __H2MATCHES(name, args, message)                                                      \
   const char* matcher_name = name;                                                           \
   template <typename A> bool __matches(const A& a) const;                                    \
   template <typename A> h2::h2_fail* matches(const A& a, const h2::C& c) const               \
   {                                                                                          \
      if (c.fit(__matches(a))) return nullptr;                                                \
      h2::h2_fail* fail = h2::h2_fail::new_unexpect(expection(c), h2::h2_stringify(a, true)); \
      h2::h2_ostringstream t;                                                                 \
      t << H2PP_REMOVE_PARENTHESES(message);                                                  \
      fail->user_explain = t.str().c_str();                                                   \
      return fail;                                                                            \
   }                                                                                          \
   virtual h2::h2_line expection(const h2::C& c) const override { return c.pre() + matcher_name + h2::gray("(") + args + h2::gray(")"); }

#define H2MATCHER0(name, message)                                                                                                                   \
   struct h2_##name##_matches : h2::h2_matches {                                                                                                    \
      explicit h2_##name##_matches() {}                                                                                                             \
      __H2MATCHES(#name, "", message)                                                                                                               \
   };                                                                                                                                               \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches> name() { return h2::h2_polymorphic_matcher<h2_##name##_matches>(h2_##name##_matches()); } \
   template <typename A> bool h2_##name##_matches::__matches(const A& a) const

#define H2MATCHER1(name, e1, message)                                                                                                                                                                 \
   template <typename E1> struct h2_##name##_matches : h2::h2_matches {                                                                                                                               \
      const E1 e1;                                                                                                                                                                                    \
      explicit h2_##name##_matches(const E1& _e1) : e1(_e1) {}                                                                                                                                        \
      __H2MATCHES(#name, h2::h2_stringify(e1, true), message)                                                                                                                                         \
   };                                                                                                                                                                                                 \
   template <typename E1> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1>> name(const E1 _e1) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1>>(h2_##name##_matches<E1>(_e1)); } \
   template <typename E1> template <typename A> bool h2_##name##_matches<E1>::__matches(const A& a) const

#define H2MATCHER2(name, e1, e2, message)                                                                                                                                                                                                         \
   template <typename E1, typename E2> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                              \
      const E1 e1;                                                                                                                                                                                                                                \
      const E2 e2;                                                                                                                                                                                                                                \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2) : e1(_e1), e2(_e2) {}                                                                                                                                                            \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true), message)                                                                                                                                       \
   };                                                                                                                                                                                                                                             \
   template <typename E1, typename E2> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>> name(const E1 _e1, const E2 _e2) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>>(h2_##name##_matches<E1, E2>(_e1, _e2)); } \
   template <typename E1, typename E2> template <typename A> bool h2_##name##_matches<E1, E2>::__matches(const A& a) const

#define H2MATCHER3(name, e1, e2, e3, message)                                                                                                                                                                                                                                                 \
   template <typename E1, typename E2, typename E3> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                                                             \
      const E1 e1;                                                                                                                                                                                                                                                                            \
      const E2 e2;                                                                                                                                                                                                                                                                            \
      const E3 e3;                                                                                                                                                                                                                                                                            \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3) : e1(_e1), e2(_e2), e3(_e3) {}                                                                                                                                                                                \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true) + h2::gray(", ") + h2::h2_stringify(e3, true), message)                                                                                                                                     \
   };                                                                                                                                                                                                                                                                                         \
   template <typename E1, typename E2, typename E3> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>> name(const E1 _e1, const E2 _e2, const E3 _e3) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>>(h2_##name##_matches<E1, E2, E3>(_e1, _e2, _e3)); } \
   template <typename E1, typename E2, typename E3> template <typename A> bool h2_##name##_matches<E1, E2, E3>::__matches(const A& a) const

#define H2MATCHER4(name, e1, e2, e3, e4, message)                                                                                                                                                                                                                                                                                         \
   template <typename E1, typename E2, typename E3, typename E4> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                                                                                            \
      const E1 e1;                                                                                                                                                                                                                                                                                                                        \
      const E2 e2;                                                                                                                                                                                                                                                                                                                        \
      const E3 e3;                                                                                                                                                                                                                                                                                                                        \
      const E4 e4;                                                                                                                                                                                                                                                                                                                        \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4) : e1(_e1), e2(_e2), e3(_e3), e4(_e4) {}                                                                                                                                                                                                    \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true) + h2::gray(", ") + h2::h2_stringify(e3, true) + h2::gray(", ") + h2::h2_stringify(e4, true), message)                                                                                                                                   \
   };                                                                                                                                                                                                                                                                                                                                     \
   template <typename E1, typename E2, typename E3, typename E4> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>>(h2_##name##_matches<E1, E2, E3, E4>(_e1, _e2, _e3, _e4)); } \
   template <typename E1, typename E2, typename E3, typename E4> template <typename A> bool h2_##name##_matches<E1, E2, E3, E4>::__matches(const A& a) const

#define H2MATCHER5(name, e1, e2, e3, e4, e5, message)                                                                                                                                                                                                                                                                                                                                 \
   template <typename E1, typename E2, typename E3, typename E4, typename E5> struct h2_##name##_matches : h2::h2_matches {                                                                                                                                                                                                                                                           \
      const E1 e1;                                                                                                                                                                                                                                                                                                                                                                    \
      const E2 e2;                                                                                                                                                                                                                                                                                                                                                                    \
      const E3 e3;                                                                                                                                                                                                                                                                                                                                                                    \
      const E4 e4;                                                                                                                                                                                                                                                                                                                                                                    \
      const E5 e5;                                                                                                                                                                                                                                                                                                                                                                    \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4, const E5& _e5) : e1(_e1), e2(_e2), e3(_e3), e4(_e4), e5(_e5) {}                                                                                                                                                                                                                        \
      __H2MATCHES(#name, h2::h2_stringify(e1, true) + h2::gray(", ") + h2::h2_stringify(e2, true) + h2::gray(", ") + h2::h2_stringify(e3, true) + h2::gray(", ") + h2::h2_stringify(e4, true) + h2::gray(", ") + h2::h2_stringify(e5, true), message)                                                                                                                                 \
   };                                                                                                                                                                                                                                                                                                                                                                                 \
   template <typename E1, typename E2, typename E3, typename E4, typename E5> inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4, const E5 _e5) { return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>>(h2_##name##_matches<E1, E2, E3, E4, E5>(_e1, _e2, _e3, _e4, _e5)); } \
   template <typename E1, typename E2, typename E3, typename E4, typename E5> template <typename A> bool h2_##name##_matches<E1, E2, E3, E4, E5>::__matches(const A& a) const

#define H2MATCHER_1(name) H2MATCHER0(name, (""))

#define H2MATCHER_2(name, t) H2PP_CAT(H2MATCHER_2_, H2PP_IS_PARENTHESIS(t))(name, t)
#define H2MATCHER_2_0(name, e1) H2MATCHER1(name, e1, (""))
#define H2MATCHER_2_1(name, message) H2MATCHER0(name, message)

#define H2MATCHER_3(name, e1, t) H2PP_CAT(H2MATCHER_3_, H2PP_IS_PARENTHESIS(t))(name, e1, t)
#define H2MATCHER_3_0(name, e1, e2) H2MATCHER2(name, e1, e2, (""))
#define H2MATCHER_3_1(name, e1, message) H2MATCHER1(name, e1, message)

#define H2MATCHER_4(name, e1, e2, t) H2PP_CAT(H2MATCHER_4_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, t)
#define H2MATCHER_4_0(name, e1, e2, e3) H2MATCHER3(name, e1, e2, e3, (""))
#define H2MATCHER_4_1(name, e1, e2, message) H2MATCHER2(name, e1, e2, message)

#define H2MATCHER_5(name, e1, e2, e3, t) H2PP_CAT(H2MATCHER_5_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, e3, t)
#define H2MATCHER_5_0(name, e1, e2, e3, e4) H2MATCHER4(name, e1, e2, e3, e4, (""))
#define H2MATCHER_5_1(name, e1, e2, e3, message) H2MATCHER3(name, e1, e2, e3, message)

#define H2MATCHER_6(name, e1, e2, e3, e4, t) H2PP_CAT(H2MATCHER_6_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, e3, e4, t)
#define H2MATCHER_6_0(name, e1, e2, e3, e4, e5) H2MATCHER5(name, e1, e2, e3, e4, e5, (""))
#define H2MATCHER_6_1(name, e1, e2, e3, e4, message) H2MATCHER4(name, e1, e2, e3, e4, message)

#define H2MATCHER_7(name, e1, e2, e3, e4, e5, t) H2PP_CAT(H2MATCHER_7_, H2PP_IS_PARENTHESIS(t))(name, e1, e2, e3, e4, e5, t)
#define H2MATCHER_7_0(name, e1, e2, e3, e4, e5, e6)
#define H2MATCHER_7_1(name, e1, e2, e3, e4, e5, message) H2MATCHER5(name, e1, e2, e3, e4, e5, message)

#define H2MATCHER(...) H2PP_VCALL(H2MATCHER_, __VA_ARGS__)
