
h2_inline h2_fail* h2_checkin::check(const char* func, const char* file, int line)
{
   if (is_satisfied() || is_saturated()) return nullptr;
   return h2_fail::new_normal({func, "\033{+dark gray}", "()", "\033{-dark gray}", " expected ", "\033{green}", expect(), "\033{reset}", " but actually ", "\033{red,bold}", actual(), "\033{reset}", " called"}, file, line);
}

h2_inline const char* h2_checkin::actual()
{
   static char st[64];
   if (call > 0)
      sprintf(st, "%d times", call);
   else
      sprintf(st, "never");
   return st;
}

h2_inline const char* h2_checkin::expect()
{
   static char st[128];
   if (least == 0) {
      if (most == 0)
         sprintf(st, "never called");
      else if (most == INT_MAX)
         sprintf(st, "any number of times");
      else
         sprintf(st, "at most %d times", most);
   } else if (least == most)
      sprintf(st, "exactly %d times", least);
   else if (most == INT_MAX)
      sprintf(st, "at least %d times", least);
   else  // 0 < least < most < INT_MAX
      sprintf(st, "between %d and %d times", least, most);

   return st;
}
