
/* 考勤 check on work attendance ; 函数被调次数期望 */
struct h2_attendance {
   int call = 0; /* 就地初始化成员变量 */
   int least, most;

   h2_attendance(int _least, int _most) : least(_least), most(_most) {}

   void operator++() { call += 1; }

   bool is_not_enough(/*不够*/) const { return call < least; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_overmuch(/*过多*/) const { return most < call; }

   h2_fail* check(const char* func, const char* file, int line);
   const char* actual();
   const char* expect();
};
