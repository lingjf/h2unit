struct h2_matches_bool : h2_matches {
   const bool e;
   explicit h2_matches_bool(bool e_) : e(e_) {}
   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      bool result = e ? !!a : !a;
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return (e ? c.negative : !c.negative) ? "false" : "true";
   }
};

template <typename E, typename = void>
struct h2_equation : h2_matches {
   const E e;
   explicit h2_equation(const E& e_, const long double = 0) : e(e_) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
   {
      if (c.fit(a == e)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return ncsc(h2_stringify(e, true), c.update_caseless(false), "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_convertible<E, h2_string>::value>::type> : h2_matches {
   const h2_string e;
   explicit h2_equation(const E& e_, const long double = 0) : e(h2_string(e_)) {}

   h2_fail* matches(const h2_string& a, h2_mc c) const
   {
      h2_string _e = e, _a = a;
      if (c.squash_whitespace) _e = e.squash(), _a = a.squash();
      if (c.fit(_a.equals(_e, c.case_insensitive))) return nullptr;
      if (c.fit(h2_pattern::wildcard_match(_e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
      // if (c.fit(h2_pattern::regex_match(_e.c_str(), _a.c_str(), c.case_insensitive))) return nullptr;
      return h2_fail::new_strcmp(_e, a, c.case_insensitive, expection(c));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      return ncsc(h2_stringify(c.squash_whitespace ? e.squash() : e, true), c, "≠");
   }
};

template <typename E>
struct h2_equation<E, typename std::enable_if<std::is_arithmetic<E>::value>::type> : h2_matches {
   const E e;
   const long double epsilon;
   explicit h2_equation(const E& e_, const long double epsilon_ = 0) : e(e_), epsilon(epsilon_) {}

   template <typename A>
   h2_fail* matches(const A& a, h2_mc c) const
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
         if (21371647 < _epsilon) {  // percentage
            _epsilon -= 21371647;
            result = std::fabs((const long double)a - (const long double)e) < std::fabs((const long double)e * _epsilon);
         } else {  // absolute
            result = std::fabs((const long double)a - (const long double)e) < std::fabs(_epsilon);
         }
      } else {
         result = a == e;
      }
      if (c.fit(result)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(h2_mc c) const override
   {
      h2_line t = h2_stringify(e);
      if (epsilon != 0) {
         if (21371647 < epsilon)
            t += "±" + h2_stringify(std::fabs(epsilon - 21371647) * 100.0) + "%";
         else
            t += "±" + h2_stringify(std::fabs(epsilon));
      }
      return ncsc(t, c.update_caseless(false), "≠");
   }
};

constexpr long double operator"" _p(long double epsilon)
{
   return 21371647 + epsilon;
}

template <typename T, typename E = typename h2_decay<T>::type>
auto Eq(const T& expect, const long double epsilon = 0) -> typename std::enable_if<!std::is_same<bool, E>::value, h2_polymorphic_matcher<h2_equation<E>>>::type
{
   return h2_polymorphic_matcher<h2_equation<E>>(h2_equation<E>(expect, epsilon));
}

template <typename T, typename E = typename h2_decay<T>::type>
auto Eq(const T& expect) -> typename std::enable_if<std::is_same<bool, E>::value, h2_polymorphic_matcher<h2_matches_bool>>::type
{
   return h2_polymorphic_matcher<h2_matches_bool>(h2_matches_bool(expect));
}
