struct h2_json_select {
   struct value {
      int index;
      h2_string key;
   };

   h2_vector<value> values;

   h2_json_select(const char* selector)
   {
      enum {
         st_idle,
         st_in_dot,
         st_in_bracket
      };
      int state = 0;
      const char *s = nullptr, *p = selector;
      do {
         switch (state) {
            case st_idle:
               if (*p == '.') {
                  state = st_in_dot;
                  s = p + 1;
               } else if (*p == '[') {
                  state = st_in_bracket;
                  s = p + 1;
               }
               break;
            case st_in_dot:
               if (*p == '.') {  // end a part
                  if (s < p) add(s, p, true);
                  // restart a new part
                  state = st_in_dot;
                  s = p + 1;
               } else if (*p == '[') {  // end a part
                  if (s < p) add(s, p, true);
                  // restart a new part
                  state = st_in_bracket;
                  s = p + 1;
               } else if (*p == '\0') {
                  if (s < p) add(s, p, true);
                  state = st_idle;
               }
               break;
            case st_in_bracket:
               if (*p == ']') {
                  if (s < p) add(s, p, false);
                  state = st_idle;
               }
               break;
         }
      } while (*p++);
   }

   void add(const char* start, const char* end, bool only_key)  // [start, end)
   {
      start = strip_left(start, end);
      end = strip_right(start, end);
      if (start < end) {
         if (!only_key) {
            if (strspn(start, "-0123456789") == (size_t)(end - start)) {
               values.push_back({atoi(start), ""});
               return;
            } else if ((*start == '\"' && *(end - 1) == '\"') || (*start == '\'' && *(end - 1) == '\'')) {
               ++start, --end;
            }
         }
         if (start < end) values.push_back({0, h2_string(end - start, start)});
      }
   }
};
