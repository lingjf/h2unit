
struct h2_callx {
   int call_;
   int min_, max_;

   h2_callx(int _min, int _max) : call_(0), min_(_min), max_(_max) {}

   void operator++() { call_ += 1; }

   bool is_shortage(/*不够*/) { return call_ < min_; }
   bool is_satisfied(/*满足*/) { return min_ <= call_ && call_ <= max_; }
   bool is_saturated(/*饱和*/) { return call_ == max_; }
   bool is_overfill(/*过多*/) { return max_ < call_; }

   h2_fail* check() {
      if (is_satisfied() || is_saturated()) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->hamet("", actual(), "called but expect", expect(), "");
      return fail;
   }

   const char* actual() {
      static char t[64];
      if (call_ > 0)
         sprintf(t, "%d times", call_);
      else
         sprintf(t, "never");
      return t;
   }

   const char* expect() {
      static char t[128];
      if (min_ == 0)
         if (max_ == 0)
            sprintf(t, "never called");
         else if (max_ == INT_MAX)
            sprintf(t, "any number of times");
         else
            sprintf(t, "at most %d times", max_);
      else if (min_ == max_)
         sprintf(t, "exactly %d times", min_);
      else if (max_ == INT_MAX)
         sprintf(t, "at least %d times", min_);
      else  // 0 < min_ < max_ < INT_MAX
         sprintf(t, "between %d and %d times", min_, max_);

      return t;
   }
};
