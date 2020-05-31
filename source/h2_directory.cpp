
static constexpr const char* last_order_file_path = ".last_order";

static inline void save_last_order()
{
   h2_with f(fopen(last_order_file_path, "w"));
   if (f.f)
      h2_list_for_each_entry (s, &h2_directory::I().suites, h2_suite, x)
         h2_list_for_each_entry (c, &s->cases, h2_case, x)
            fprintf(f.f, "%s\n%s\n", s->name, c->name);
}

static inline bool mark_sequence(char* suitename, char* casename)
{
   static int seq = INT_MIN / 4;

   h2_list_for_each_entry (s, &h2_directory::I().suites, h2_suite, x) {
      if (!strcmp(suitename, s->name)) {
         h2_list_for_each_entry (c, &s->cases, h2_case, x) {
            if (!strcmp(casename, c->name)) {
               s->seq = c->seq = ++seq;
               return true;
            }
         }
      }
   }
   return false;
}

static inline int read_last_order()
{
   int count = 0;
   char suitename[512], casename[512];
   h2_with f(fopen(last_order_file_path, "r"));
   if (f.f)
      while (1 == fscanf(f.f, "%[^\n]\n", suitename) && 1 == fscanf(f.f, "%[^\n]\n", casename))
         if (mark_sequence(suitename, casename))
            count += 1;
   return count;
}

h2_inline void h2_directory::sort()
{
   int last = read_last_order();
   srand(h2_now());
   if (O.shuffle && last == 0)
      h2_list_for_each_entry (s, &h2_directory::I().suites, h2_suite, x)
         h2_list_for_each_entry (c, &s->cases, h2_case, x)
            s->seq = c->seq = rand();

   h2_directory::I().suites.sort([](h2_list* a, h2_list* b) {
      return h2_list_entry(a, h2_suite, x)->seq - h2_list_entry(b, h2_suite, x)->seq;
   });
   h2_list_for_each_entry (s, &h2_directory::I().suites, h2_suite, x) {
      s->cases.sort([](h2_list* a, h2_list* b) {
         return h2_list_entry(a, h2_case, x)->seq - h2_list_entry(b, h2_case, x)->seq;
      });
   }

   if (O.shuffle && last == 0)
      save_last_order();
}

h2_inline int h2_directory::count()
{
   int count = 0;
   h2_list_for_each_entry (s, &h2_directory::I().suites, h2_suite, x)
      count += s->cases.count();
   return count;
}

h2_inline void h2_directory::drop_last_order() { ::remove(last_order_file_path); }
