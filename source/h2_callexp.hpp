
struct h2_callexp {
   int call, least, most;

   h2_callexp(int _least, int _most) : call(0), least(_least), most(_most) {}

   void operator++() { call += 1; }

   bool is_not_enough() const { return call < least; }
   bool is_satisfied() const { return least <= call && call <= most; }
   bool is_saturated() const { return call == most; }
   bool is_overmuch() const { return most < call; }

   h2_fail* check(const char* func, const char* file, int line);
   const char* actual();
   const char* expect();
};
