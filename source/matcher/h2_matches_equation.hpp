template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<!std::is_pointer<E>::value && !std::is_pointer<A>::value, h2_fail*>::type
   {
      if (c.fit(a == e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<std::is_pointer<E>::value || std::is_pointer<A>::value, h2_fail*>::type
   {
      if (!e) return h2_matches_null().matches(a, c);
      if (c.fit((void*)a == (void*)e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return c.update_caseless(false).pre("≠") + h2_stringify(e, true);
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      h2_string _e = e, _a(a);
      if (c.squash_whitespace) _e = e.squash(), _a = _a.squash();
      if (c.fit(_a.equals(_e, c.case_insensitive))) return nullptr;
      // if (c.fit(h2_pattern::wildcard_match(_e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
      return h2_fail::new_strcmp(_e, a, c.case_insensitive, expection(c));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return c.pre("≠") + h2_stringify(c.squash_whitespace ? e.squash() : e, true);
   }
};

template <>
struct h2_equation<const char*> : h2_matches {
   const char* e;
   explicit h2_equation(const char* e_, const long double = 0) : e(e_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<std::is_convertible<A, h2_string>::value, h2_fail*>::type
   {
      if (!e) return h2_matches_null().matches(a, c);
      h2_equation<h2_string> string_m(e);
      return string_m.matches(a, c);
   }
   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<!std::is_convertible<A, h2_string>::value, h2_fail*>::type
   {
      if (!e) return h2_matches_null().matches(a, c);
      if (c.fit((void*)a == (void*)e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      if (!e) return h2_matches_null().expection(c);
      return c.pre("≠") + h2_stringify(c.squash_whitespace ? h2_string(e).squash() : h2_string(e), true);
   }
};

template <>
struct h2_equation<char*> : h2_equation<const char*> {
   explicit h2_equation(const char* e_, const long double = 0) : h2_equation<const char*>((const char*)e_) {}
};

struct h2_approximate {
   static constexpr unsigned long long A = 100000000000000ull;
   static constexpr unsigned long long B = 1000000000000000000ull;

   unsigned long long operator*(const double& epsilon) const
   {
      return (unsigned long long)(fabs(epsilon) * A);
   }

   static bool is_scale(const long double epsilon)
   {
      if (epsilon == 0) return false;
      return epsilon < B;
   }
   static const long double absolute_margin(const long double epsilon)
   {
      if (epsilon == 0) return 0.00001;
      return (epsilon - B) / A;
   }
   static const long double percentage_scale(const long double epsilon)
   {
      return (epsilon / A) / 100;
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& e_, const long double epsilon_ = 0) : e(e_), epsilon(epsilon_) {}

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<std::is_floating_point<E>::value || std::is_floating_point<A>::value, h2_fail*>::type
   {
      // the machine epsilon has to be scaled to the magnitude of the values used
      // and multiplied by the desired precision in ULPs (units in the last place)
      // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
      //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
      bool result;
      if (h2::h2_approximate::is_scale(epsilon))  // percentage/scale
         result = std::fabs((const long double)a - (const long double)e) <= std::fabs(e * h2::h2_approximate::percentage_scale(epsilon));
      else  // absolute/margin
         result = std::fabs((const long double)a - (const long double)e) <= std::fabs(h2::h2_approximate::absolute_margin(epsilon));
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }

   template <typename A>
   auto matches(const A& a, h2_mc c) const -> typename std::enable_if<!std::is_floating_point<E>::value && !std::is_floating_point<A>::value, h2_fail*>::type
   {
      bool result = h2_numberfy<std::uintptr_t>(e) == h2_numberfy<std::uintptr_t>(a);
      if (c.fit(result)) return nullptr;
      if (!e && (std::is_pointer<A>::value || h2_is_smart_ptr<A>::value))
         return h2_fail::new_unexpect(c.negative ? "!NULL" : "NULL", h2_stringify(a, true));
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      h2_line t = h2_stringify(e);
      if (epsilon != 0) {
         if (h2::h2_approximate::is_scale(epsilon))  // percentage/scale
            t += "±" + h2_stringify(h2::h2_approximate::percentage_scale(epsilon) * 100.0) + "%";
         else  // absolute/margin
            t += "±" + h2_stringify(std::fabs(h2::h2_approximate::absolute_margin(epsilon)));
      }
      return c.update_caseless(false).pre("≠") + t;
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
auto _Eq(const T& expect, const long double epsilon = 0) -> typename std::enable_if<!std::is_same<std::nullptr_t, E>::value && !std::is_same<bool, E>::value, h2_polymorphic_matcher<h2_equation<E>>>::type
{
   return h2_polymorphic_matcher<h2_equation<E>>(h2_equation<E>(expect, epsilon));
}

template <typename T, typename E = typename h2_decay<T>::type>
auto _Eq(const T&, const long double = 0) -> typename std::enable_if<std::is_same<std::nullptr_t, E>::value, h2_polymorphic_matcher<h2_matches_null>>::type
{
   return h2_polymorphic_matcher<h2_matches_null>(h2_matches_null());
}

template <typename T, typename E = typename h2_decay<T>::type>
auto _Eq(const T& expect, const long double = 0) -> typename std::enable_if<std::is_same<bool, E>::value, h2_polymorphic_matcher<h2_matches_bool>>::type
{
   return h2_polymorphic_matcher<h2_matches_bool>(h2_matches_bool(expect));
}

#define H2Eq(expect, ...) H2PP_CAT(__H2Eq, H2PP_IS_EMPTY(__VA_ARGS__))(expect, __VA_ARGS__)
#define __H2Eq1(expect, ...) h2::_Eq(expect, 0)
#define __H2Eq0(expect, ...) h2::_Eq(expect, h2::h2_approximate() * __VA_ARGS__ + h2::h2_approximate::B)
