
#define __Matches_Common(message)                                                     \
   template <typename A>                                                              \
   bool __matches(const A& a) const;                                                  \
   template <typename A>                                                              \
   h2::h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const { \
      h2::h2_fail_unexpect* fail = new h2::h2_fail_unexpect();                        \
      if (__matches(a) == !dont) return nullptr;                                      \
      if (dont) {                                                                     \
         fail->mprintf("should not matches");                                         \
      } else {                                                                        \
         h2::h2_ostringstream osm;                                                    \
         osm << std::boolalpha << H2PP_REMOVE_PARENTHESES(message);                   \
         fail->mprintf("%s", osm.str().c_str());                                      \
      }                                                                               \
      return fail;                                                                    \
   }

#define H2MATCHER0(name, message)                                                     \
   struct h2_##name##_matches {                                                       \
      explicit h2_##name##_matches() {}                                               \
      __Matches_Common(message)                                                       \
   };                                                                                 \
   const h2::h2_polymorphic_matcher<h2_##name##_matches> name{h2_##name##_matches()}; \
   template <typename A>                                                              \
   bool h2_##name##_matches::__matches(const A& a) const

#define H2MATCHER1(name, e1, message)                                                           \
   template <typename E1>                                                                       \
   struct h2_##name##_matches {                                                                 \
      const E1 e1;                                                                              \
      explicit h2_##name##_matches(const E1& _e1) : e1(_e1) {}                                  \
      __Matches_Common(message)                                                                 \
   };                                                                                           \
   template <typename E1>                                                                       \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1>> name(const E1 _e1) {              \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1>>(h2_##name##_matches<E1>(_e1)); \
   }                                                                                            \
   template <typename E1>                                                                       \
   template <typename A>                                                                        \
   bool h2_##name##_matches<E1>::__matches(const A& a) const

#define H2MATCHER2(name, e1, e2, message)                                                                    \
   template <typename E1, typename E2>                                                                       \
   struct h2_##name##_matches {                                                                              \
      const E1 e1;                                                                                           \
      const E2 e2;                                                                                           \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2) : e1(_e1), e2(_e2) {}                       \
      __Matches_Common(message)                                                                              \
   };                                                                                                        \
   template <typename E1, typename E2>                                                                       \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>> name(const E1 _e1, const E2 _e2) {         \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2>>(h2_##name##_matches<E1, E2>(_e1, _e2)); \
   }                                                                                                         \
   template <typename E1, typename E2>                                                                       \
   template <typename A>                                                                                     \
   bool h2_##name##_matches<E1, E2>::__matches(const A& a) const

#define H2MATCHER3(name, e1, e2, e3, message)                                                                             \
   template <typename E1, typename E2, typename E3>                                                                       \
   struct h2_##name##_matches {                                                                                           \
      const E1 e1;                                                                                                        \
      const E2 e2;                                                                                                        \
      const E3 e3;                                                                                                        \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3) : e1(_e1), e2(_e2), e3(_e3) {}            \
      __Matches_Common(message)                                                                                           \
   };                                                                                                                     \
   template <typename E1, typename E2, typename E3>                                                                       \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>> name(const E1 _e1, const E2 _e2, const E3 _e3) {    \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3>>(h2_##name##_matches<E1, E2, E3>(_e1, _e2, _e3)); \
   }                                                                                                                      \
   template <typename E1, typename E2, typename E3>                                                                       \
   template <typename A>                                                                                                  \
   bool h2_##name##_matches<E1, E2, E3>::__matches(const A& a) const

#define H2MATCHER4(name, e1, e2, e3, e4, message)                                                                                        \
   template <typename E1, typename E2, typename E3, typename E4>                                                                         \
   struct h2_##name##_matches {                                                                                                          \
      const E1 e1;                                                                                                                       \
      const E2 e2;                                                                                                                       \
      const E3 e3;                                                                                                                       \
      const E4 e4;                                                                                                                       \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4) : e1(_e1), e2(_e2), e3(_e3), e4(_e4) {}   \
      __Matches_Common(message)                                                                                                          \
   };                                                                                                                                    \
   template <typename E1, typename E2, typename E3, typename E4>                                                                         \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4) { \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4>>(h2_##name##_matches<E1, E2, E3, E4>(_e1, _e2, _e3, _e4));   \
   }                                                                                                                                     \
   template <typename E1, typename E2, typename E3, typename E4>                                                                         \
   template <typename A>                                                                                                                 \
   bool h2_##name##_matches<E1, E2, E3, E4>::__matches(const A& a) const

#define H2MATCHER5(name, e1, e2, e3, e4, e5, message)                                                                                                          \
   template <typename E1, typename E2, typename E3, typename E4, typename E5>                                                                                  \
   struct h2_##name##_matches {                                                                                                                                \
      const E1 e1;                                                                                                                                             \
      const E2 e2;                                                                                                                                             \
      const E3 e3;                                                                                                                                             \
      const E4 e4;                                                                                                                                             \
      const E5 e5;                                                                                                                                             \
      explicit h2_##name##_matches(const E1& _e1, const E2& _e2, const E3& _e3, const E4& _e4, const E5& _e5) : e1(_e1), e2(_e2), e3(_e3), e4(_e4), e5(_e5) {} \
      __Matches_Common(message)                                                                                                                                \
   };                                                                                                                                                          \
   template <typename E1, typename E2, typename E3, typename E4, typename E5>                                                                                  \
   inline h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>> name(const E1 _e1, const E2 _e2, const E3 _e3, const E4 _e4, const E5 _e5) {     \
      return h2::h2_polymorphic_matcher<h2_##name##_matches<E1, E2, E3, E4, E5>>(h2_##name##_matches<E1, E2, E3, E4, E5>(_e1, _e2, _e3, _e4, _e5));            \
   }                                                                                                                                                           \
   template <typename E1, typename E2, typename E3, typename E4, typename E5>                                                                                  \
   template <typename A>                                                                                                                                       \
   bool h2_##name##_matches<E1, E2, E3, E4, E5>::__matches(const A& a) const

/* clang-format off */

#define _H2MATCHER1(name) H2MATCHER0(name, (""))

#define _H2MATCHER20(name, e1) H2MATCHER1(name, e1, (""))
#define _H2MATCHER21(name, message) H2MATCHER0(name, message)
#define _H2MATCHER2(name, t) H2PP_CAT2(_H2MATCHER2, H2PP_IS_BEGIN_PARENTHESES(t)) (name, t)

#define _H2MATCHER30(name, e1, e2) H2MATCHER2(name, e1, e2, (""))
#define _H2MATCHER31(name, e1, message) H2MATCHER1(name, e1, message)
#define _H2MATCHER3(name, e1, t) H2PP_CAT2(_H2MATCHER3, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, t)

#define _H2MATCHER40(name, e1, e2, e3) H2MATCHER3(name, e1, e2, e3, (""))
#define _H2MATCHER41(name, e1, e2, message) H2MATCHER2(name, e1, e2, message)
#define _H2MATCHER4(name, e1, e2, t) H2PP_CAT2(_H2MATCHER4, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, t)

#define _H2MATCHER50(name, e1, e2, e3, e4) H2MATCHER4(name, e1, e2, e3, e4, (""))
#define _H2MATCHER51(name, e1, e2, e3, message) H2MATCHER3(name, e1, e2, e3, message)
#define _H2MATCHER5(name, e1, e2, e3, t) H2PP_CAT2(_H2MATCHER5, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, e3, t)

#define _H2MATCHER60(name, e1, e2, e3, e4, e5) H2MATCHER5(name, e1, e2, e3, e4, e5, (""))
#define _H2MATCHER61(name, e1, e2, e3, e4, message) H2MATCHER4(name, e1, e2, e3, e4, message)
#define _H2MATCHER6(name, e1, e2, e3, e4, t) H2PP_CAT2(_H2MATCHER6, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, e3, e4, t)

#define _H2MATCHER70(name, e1, e2, e3, e4, e5, e6)
#define _H2MATCHER71(name, e1, e2, e3, e4, e5, message) H2MATCHER5(name, e1, e2, e3, e4, e5, message)
#define _H2MATCHER7(name, e1, e2, e3, e4, e5, t) H2PP_CAT2(_H2MATCHER7, H2PP_IS_BEGIN_PARENTHESES(t)) (name, e1, e2, e3, e4, e5, t)

/* clang-format on */

#define H2MATCHER(...) H2PP_VARIADIC_CALL(_H2MATCHER, __VA_ARGS__)
