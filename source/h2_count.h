
#ifndef ___H2_COUNT__H___
#define ___H2_COUNT__H___

#define h2_count_between(_min, _max) h2_count(_min, _max)
#define h2_count_atleast(n) h2_count(n, INT_MAX)
#define h2_count_atmost(n) h2_count(0, n)
#define h2_count_any() h2_count(0, INT_MAX)
#define h2_count_exactly(n) h2_count(n, n)

class h2_count
{
 private:
   int call_;
   int min_, max_;

 public:
   h2_count(int min, int max)
     : call_(0), min_(min), max_(max) {}

   void inc()
   {
      call_ += 1;
   }

   bool is_shortage(/*不够*/)
   {
      return call_ < min_;
   }
   bool is_satisfied(/*满足*/)
   {
      return min_ <= call_ && call_ <= max_;
   }
   bool is_saturated(/*饱和*/)
   {
      return call_ == max_;
   }
   bool is_overfill(/*过多*/)
   {
      return max_ < call_;
   }

   h2_fail* double_check()
   {
      if (is_satisfied() || is_saturated()) {
         return nullptr;
      }
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->hamet("", actual(), "called but expect", expect(), "");
      return fail;
   }

   const char* actual()
   {
      static char t[64];
      if (call_ > 0) {
         sprintf(t, "%d times", call_);
      }
      else {
         sprintf(t, "never");
      }
      return t;
   }

   const char* expect()
   {
      static char t[128];
      if (min_ == 0) {
         if (max_ == 0) {
            sprintf(t, "never called");
         }
         else if (max_ == INT_MAX) {
            sprintf(t, "any number of times");
         }
         else {
            sprintf(t, "at most %d times", max_);
         }
      }
      else if (min_ == max_) {
         sprintf(t, "exactly %d times", min_);
      }
      else if (max_ == INT_MAX) {
         sprintf(t, "at least %d times", min_);
      }
      else {
         // 0 < min_ < max_ < INT_MAX
         sprintf(t, "between %d and %d times", min_, max_);
      }

      return t;
   }
};

#endif
