struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool, bool) const { return nullptr; }
   virtual h2_sentence expection(bool, bool, bool) const override { return "Any"; }
};

struct h2_matches_null : h2_matches {
   const bool reverse;
   explicit h2_matches_null(bool reverse_) : reverse(reverse_) {}
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool dont, bool ncop) const
   {
      bool _dont = reverse ? !dont : dont;
      if ((nullptr == (const void*)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect(expection(false, dont, ncop), h2_stringify((const void*)a));
   }
   virtual h2_sentence expection(bool, bool dont, bool ncop) const override
   {
      return (reverse ? !dont : dont) ? "NotNull" : "IsNull";
   }
};

template <bool E>
struct h2_matches_boolean : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, int, bool, bool dont, bool ncop) const
   {
      bool _dont = E ? dont : !dont;
      if (((bool)a) == !_dont) return nullptr;
      return h2_fail::new_unexpect(expection(false, dont, ncop), a ? "true" : "false");
   }
   virtual h2_sentence expection(bool, bool dont, bool ncop) const override
   {
      return (E ? dont : !dont) ? "false" : "true";
   }
};

template <typename Matcher>
struct h2_pointee_matches : h2_matches {
   const Matcher m;
   explicit h2_pointee_matches(Matcher m_) : m(m_) {}

   template <typename SmartPointer>
   struct PointeeOf {
      typedef typename SmartPointer::element_type type;
   };
   template <typename T>
   struct PointeeOf<T*> {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, int, bool caseless, bool dont, bool ncop) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, 0, caseless, dont, ncop);
   }
   virtual h2_sentence expection(bool caseless, bool dont, bool ncop) const override
   {
      return h2_matches_expection(m, caseless, dont, ncop);
   }
};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_any> Any{h2_matches_any()};

inline h2_polymorphic_matcher<h2_matches_null> Is_Null() { return h2_polymorphic_matcher<h2_matches_null>(h2_matches_null(false)); }
inline h2_polymorphic_matcher<h2_matches_null> Not_Null() { return h2_polymorphic_matcher<h2_matches_null>(h2_matches_null(true)); }
inline h2_polymorphic_matcher<h2_matches_boolean<true>> Is_True() { return h2_polymorphic_matcher<h2_matches_boolean<true>>(h2_matches_boolean<true>()); }
inline h2_polymorphic_matcher<h2_matches_boolean<false>> Is_False() { return h2_polymorphic_matcher<h2_matches_boolean<false>>(h2_matches_boolean<false>()); }

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
