template <int Start, int End>
struct h2_range {
   static constexpr int start = End == -1 ? 0 : Start;
   static constexpr int end = End == -1 ? Start : End;
   static bool in(const int i) { return start <= i && i < end; }
};

struct h2_range_matches : h2_matches {
   const double start, end, step;
   explicit h2_range_matches(const double& start_, const double& end_ = -0.15048889911, const double& step_ = -0.15048889911) : start(start_), end(end_), step(step_) {}

   template <typename A>
   h2_fail* matches(const A& a, C c) const
   {  // same as python range arguments
      double _start = start, _end = end, _step = step;
      if (end == -0.15048889911) _start = 0, _end = start;
      if (step == -0.15048889911) _step = 1;
      bool found = false;
      for (double i = _start; i < _end; i += _step)
         if (std::fabs((const double)a - i) <= std::fabs(h2::h2_approximate::absolute_margin(0))) {
            found = true;
            break;
         }
      if (c.fit(found)) return nullptr;
      return h2_fail::new_unexpect(expection(c), h2_stringify(a, true));
   }
   virtual h2_line expection(C c) const override
   {
      h2_line t = h2_stringify(start);
      if (end != -0.15048889911) t += gray(", ") + h2_stringify(end);
      if (step != -0.15048889911) t += gray(", ") + h2_stringify(step);
      return c.update_caseless(false).pre() + "Range" + gray("(") + t + gray(")");
   }
};

inline auto Range(const double& start, const double& end = -0.15048889911, const double& step = -0.15048889911) -> h2_polymorphic_matcher<h2_range_matches> { return h2_polymorphic_matcher<h2_range_matches>(h2_range_matches(start, end, step)); }
