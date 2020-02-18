
struct h2_callexp {
   int call;
   int least, most;

   h2_callexp(int _least, int _most) : call(0), least(_least), most(_most) {}

   void operator++() { call += 1; }

   bool is_shortage(/*不够*/) const { return call < least; }
   bool is_satisfied(/*满足*/) const { return least <= call && call <= most; }
   bool is_saturated(/*饱和*/) const { return call == most; }
   bool is_overfill(/*过多*/) const { return most < call; }

   h2_fail* check() {
      if (is_satisfied() || is_saturated()) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("%s", actual());
      fail->mprintf("called but expect");
      fail->eprintf("%s", expect());
      return fail;
   }

   const char* actual() {
      static char st[64];
      if (call > 0)
         sprintf(st, "%d times", call);
      else
         sprintf(st, "never");
      return st;
   }

   const char* expect() {
      static char st[128];
      if (least == 0)
         if (most == 0)
            sprintf(st, "never called");
         else if (most == INT_MAX)
            sprintf(st, "any number of times");
         else
            sprintf(st, "at most %d times", most);
      else if (least == most)
         sprintf(st, "exactly %d times", least);
      else if (most == INT_MAX)
         sprintf(st, "at least %d times", least);
      else  // 0 < least < most < INT_MAX
         sprintf(st, "between %d and %d times", least, most);

      return st;
   }
};
