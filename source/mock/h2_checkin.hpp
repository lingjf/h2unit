
/* 考勤 ; 函数被调次数期望 */
struct h2_checkin {
   int call = 0;
   int least, most;

   h2_checkin(int _least, int _most) : least(_least), most(_most) {}

   void operator++() { call += 1; }

   bool is_not_enough(/*不够*/) const { return call < least; }
   bool is_minimal_satisfied(/*最小满足*/) const { return least == call; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_overmuch(/*过多*/) const { return most < call; }

   h2_fail* check(const char* func, const char* file, int line);
   const char* actual();
   const char* expect();
};
