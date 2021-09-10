template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      if ((a == e) == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      return CD(h2_representify(e), false, dont, "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& e_, const long double = 0) : e(h2_string(e_)) {}

   h2_fail* matches(const h2_string& a, int, bool caseless, bool dont) const
   {
      if (a.equals(e, caseless) == !dont) return nullptr;
      if (h2_pattern::wildcard_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;
      if (h2_pattern::regex_match(e.c_str(), a.c_str(), caseless) == !dont) return nullptr;

      return h2_fail::new_strcmp(e, a, caseless, expection(caseless, dont));
   }
   virtual h2_row expection(bool caseless, bool dont) const override
   {
      return CD(h2_representify(e), caseless, dont, "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& e_, const long double epsilon_ = 0) : e(e_), epsilon(epsilon_) {}

   template <typename A>
   h2_fail* matches(const A& a, int, bool caseless, bool dont) const
   {
      bool result;
      if (std::is_floating_point<E>::value || std::is_floating_point<A>::value) {
         //
         // the machine epsilon has to be scaled to the magnitude of the values used
         // and multiplied by the desired precision in ULPs (units in the last place)
         // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
         //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
         long double _epsilon = epsilon;
         if (_epsilon == 0) _epsilon = 0.00001;
         result = std::fabs((const long double)a - (const long double)e) < _epsilon;
      } else {
         result = a == e;
      }
      if (result == !dont) return nullptr;
      return h2_fail::new_unexpect(expection(caseless, dont), h2_representify(a));
   }
   virtual h2_row expection(bool, bool dont) const override
   {
      h2_row t = h2_representify(e);
      if (epsilon != 0) {
         h2_ostringstream oss;
         oss << "±" << std::fixed << epsilon;
         t += oss.str().c_str();
      }
      return CD(t, false, dont, "≠");
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_equation<E>> Eq(const T& expect, const long double epsilon = 0)
{
   return h2_polymorphic_matcher<h2_equation<E>>(h2_equation<E>(expect, epsilon));
}
