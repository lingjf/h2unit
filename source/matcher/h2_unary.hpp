
struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool, bool) const { return nullptr; }
   virtual h2_string expects(bool, bool) const override { return "Any"; }
};

struct h2_matches_null : h2_matches {
   const bool reverse;
   explicit h2_matches_null(bool _reverse) : reverse(_reverse) {}
   template <typename A>
   h2_fail* matches(const A& a, bool, bool dont) const
   {
      bool _dont = reverse ? !dont : dont;
      if ((nullptr == (const void*)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect("", h2_stringify((const void*)a), expects(false, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return (reverse ? !dont : dont) ? "NotNull" : "IsNull";
   }
};

template <bool E>
struct h2_matches_boolean : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, bool, bool dont) const
   {
      bool _dont = E ? dont : !dont;
      if (((bool)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect("", a ? "true" : "false", expects(false, dont));
   }
   virtual h2_string expects(bool, bool dont) const override
   {
      return (E ? dont : !dont) ? "false" : "true";
   }
};

template <typename Matcher>
struct h2_pointee_matches : h2_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher _m) : m(_m) {}

   template <typename SmartPointer>
   struct PointeeOf {
      typedef typename SmartPointer::element_type type;
   };
   template <typename T>
   struct PointeeOf<T*> {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, bool caseless, bool dont) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, caseless, dont);
   }
   virtual h2_string expects(bool caseless, bool dont) const override
   {
      return h2_matches_expects(m, caseless, dont);
   }
};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_any> Any{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_null> IsNull{h2_matches_null(false)};
const h2_polymorphic_matcher<h2_matches_null> NotNull{h2_matches_null(true)};
const h2_polymorphic_matcher<h2_matches_boolean<true>> IsTrue{h2_matches_boolean<true>()};
const h2_polymorphic_matcher<h2_matches_boolean<false>> IsFalse{h2_matches_boolean<false>()};

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
