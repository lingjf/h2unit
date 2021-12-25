h2_inline h2_fail* h2_checkin::check(size_t index, size_t total, const char* srcfn) const
{
   if (is_satisfied() || is_saturated()) return nullptr;
   h2_line t = srcfn + gray("()") + " expected " + delta(expect(), "green") + " but actually " + delta(actual(), "red,bold") + " called";
   if (1 < total) t += gray(" when ") + index_th(index) + " " + color(expr, "cyan");
   return h2_fail::new_normal(t);
}

h2_inline const char* h2_checkin::actual() const
{
   static char st[64];
   if (call > 0) sprintf(st, "%d times", call);
   else sprintf(st, "never");
   return st;
}

h2_inline const char* h2_checkin::expect() const
{
   static char st[128];
   if (least == 0) {
      if (most == 0) sprintf(st, "never called");
      else if (most == 0x7fffffff) sprintf(st, "any number of times");
      else sprintf(st, "at most %d times", most);
   } else if (least == most) sprintf(st, "exactly %d times", least);
   else if (most == 0x7fffffff) sprintf(st, "at least %d times", least);
   else sprintf(st, "between %d and %d times", least, most);  // 0 < least < most < 0x7fffffff
   return st;
}
