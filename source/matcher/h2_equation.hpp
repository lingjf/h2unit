template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, size_t, h2_mc c) const
   {
      if (c.fit(a == e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD(h2_representify(e), c.update_caseless(false), "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& e_, const long double = 0) : e(h2_string(e_)) {}

   h2_fail* matches(const h2_string& a, size_t, h2_mc c) const
   {
      h2_string _e = e, _a = a;
      if (c.spaceless) _e = e.squash(), _a = a.squash();
      if (c.fit(_a.equals(_e, c.caseless))) return nullptr;
      if (c.fit(h2_pattern::wildcard_match(_e.c_str(), _a.c_str(), c.caseless))) return nullptr;
      // if (c.fit(h2_pattern::regex_match(_e.c_str(), _a.c_str(), c.caseless))) return nullptr;
      return h2_fail::new_strcmp(_e, a, c.caseless, expection(c));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return CD(h2_representify(c.spaceless ? e.squash() : e), c, "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& e_, const long double epsilon_ = 0) : e(e_), epsilon(epsilon_) {}

   template <typename A>
   h2_fail* matches(const A& a, size_t, h2_mc c) const
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
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_representify(a));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      h2_line t = h2_representify(e);
      if (epsilon != 0) {
         h2_ostringstream oss;
         oss << "±" << std::fixed << epsilon;
         t += oss.str().c_str();
      }
      return CD(t, c.update_caseless(false), "≠");
   }
};

template <typename T, typename E = typename h2_decay<T>::type>
inline h2_polymorphic_matcher<h2_equation<E>> Eq(const T& expect, const long double epsilon = 0)
{
   return h2_polymorphic_matcher<h2_equation<E>>(h2_equation<E>(expect, epsilon));
}
