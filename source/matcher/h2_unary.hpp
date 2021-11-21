struct h2_matches_any : h2_matches {
   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const { return nullptr; }
   virtual h2_line expection(h2_mc c) const override { return "Any"; }
};

struct h2_matches_null : h2_matches {
   const bool reverse;
   explicit h2_matches_null(bool reverse_) : reverse(reverse_) {}
   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      bool result = reverse ? nullptr != (const void*)a : nullptr == (const void*)a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify((const void*)a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return reverse != c.negative /*XOR ^*/ ? "NotNull" : "IsNull";
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
   h2_fail* matches(const A& a, h2_mc c) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;
      return h2_matcher_cast<Pointee>(m).matches(*a, c.update_n(0));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return h2_matches_expection(m, c);
   }
};

const h2_polymorphic_matcher<h2_matches_any> _{h2_matches_any()};
const h2_polymorphic_matcher<h2_matches_any> Any{h2_matches_any()};

inline h2_polymorphic_matcher<h2_matches_null> _IsNull() { return h2_polymorphic_matcher<h2_matches_null>(h2_matches_null(false)); }
inline h2_polymorphic_matcher<h2_matches_null> _NotNull() { return h2_polymorphic_matcher<h2_matches_null>(h2_matches_null(true)); }

template <typename M>
inline h2_polymorphic_matcher<h2_pointee_matches<M>> Pointee(M m) { return h2_polymorphic_matcher<h2_pointee_matches<M>>(h2_pointee_matches<M>(m)); }
