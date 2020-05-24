
static constexpr const char* last_order_file_path = ".last_order";

static inline void save_last_order()
{
   h2_with f(fopen(last_order_file_path, "w"));
   if (f.f)
      h2_list_for_each_entry(s, &h2_directory::I().sorted_suites, h2_suite, sorted)
        h2_list_for_each_entry(c, &s->sorted_cases, h2_case, sorted)
          fprintf(f.f, "%s\n%s\n", s->name, c->name);
}

static inline bool mark_sequence(char* suitename, char* casename)
{
   static long long seq = INT_MIN;

   h2_list_for_each_entry(s, &h2_directory::I().registered_suites, h2_suite, registered)
   {
      if (strcmp(suitename, s->name) == 0) {
         h2_list_for_each_entry(c, &s->registered_cases, h2_case, registered)
         {
            if (strcmp(casename, c->name) == 0) {
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

static inline void insert_suite_sort(h2_list* suite_list, h2_suite* s)
{
   h2_list_for_each_entry(p, suite_list, h2_suite, sorted)
   {
      if (s->seq < p->seq) {
         p->sorted.add_before(&s->sorted);
         return;
      }
   }
   suite_list->add_tail(&s->sorted);
}

static inline void insert_case_sort(h2_list* case_list, h2_case* c)
{
   h2_list_for_each_entry(p, case_list, h2_case, sorted)
   {
      if (c->seq < p->seq) {
         p->sorted.add_before(&c->sorted);
         return;
      }
   }
   case_list->add_tail(&c->sorted);
}

h2_inline int h2_directory::sort()
{
   int count = 0;
   int last = read_last_order();
   srand(clock());
   if (O.randomize && last == 0)
      h2_list_for_each_entry(s, &h2_directory::I().registered_suites, h2_suite, registered)
        h2_list_for_each_entry(c, &s->registered_cases, h2_case, registered)
          s->seq = c->seq = rand();

   h2_list_for_each_entry(s, &h2_directory::I().registered_suites, h2_suite, registered)
   {
      h2_list_for_each_entry(c, &s->registered_cases, h2_case, registered)
      {
         insert_case_sort(&s->sorted_cases, c);
         count += 1;
      }
      insert_suite_sort(&h2_directory::I().sorted_suites, s);
   }

   if (O.randomize && last == 0)
      save_last_order();

   return count;
}

h2_inline void h2_directory::drop_last_order() { ::remove(last_order_file_path); }
