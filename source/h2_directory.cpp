
static constexpr const char* last_order_file_path = ".last_order";

static inline bool isfit(const char opt, const char e) { return opt == 'a' || opt == 'A' || ::tolower(opt) == ::tolower(e); }

h2_inline void h2_directory::drop_last_order() { ::remove(last_order_file_path); }

static inline void read_last_order(std::vector<std::pair<std::string, std::string>>& list)
{
   char suitename[512], casename[512];
   h2_with f(fopen(last_order_file_path, "r"));
   if (f.f)
      while (1 == fscanf(f.f, "%[^\n]\n", suitename) && 1 == fscanf(f.f, "%[^\n]\n", casename))
         list.push_back(std::pair<std::string, std::string>(suitename, casename));
}

static inline void save_last_order()
{
   h2_with f(fopen(last_order_file_path, "w"));
   if (f.f)
      for (auto& s : h2_directory::I().suites)
         for (auto& c : s->cases())
            fprintf(f.f, "%s\n%s\n", s->name, c->name);
}

static inline void find_and_sequent(std::pair<std::string, std::string>& suite_case)
{
   static long long seq = INT_MIN;
   for (auto& s : h2_directory::I().suites)
      if (suite_case.first == s->name)
         for (auto& c : s->cases())
            if (suite_case.second == c->name)
               s->seq = c->seq = ++seq;
}

h2_inline int h2_directory::count()
{
   int count = 0;
   for (auto& s : h2_directory::I().suites)
      count += s->cases().size();
   return count;
}

h2_inline int h2_directory::sort()
{
   std::vector<std::pair<std::string, std::string>> last_list;
   read_last_order(last_list);
   if (0 < last_list.size()) {
      for (auto& k : last_list) find_and_sequent(k);
      for (auto& s : h2_directory::I().suites)
         std::sort(s->cases().begin(), s->cases().end(), [](h2_case* a, h2_case* b) { return a->seq < b->seq; });
      std::sort(h2_directory::I().suites.begin(), h2_directory::I().suites.end(), [](h2_suite* a, h2_suite* b) { return a->seq < b->seq; });
   } else {
      if (isfit(O.randomize, 'c'))
         for (auto& s : h2_directory::I().suites)
            shuffle(s->cases().begin(), s->cases().end(), std::default_random_engine{std::random_device()()});
      if (isfit(O.randomize, 's')) shuffle(h2_directory::I().suites.begin(), h2_directory::I().suites.end(), std::default_random_engine{std::random_device()()});
      if (O.randomize) save_last_order();
   }
   return count();
}

h2_inline void h2_directory::print_list()
{
   int ss = 0, cs = 0, t;

   for (auto& s : h2_directory::I().suites) {
      if (t = 0, isfit(O.listing, 's')) {
         if (!O.filter(s->name, "", "")) t++;
         for (auto& c : s->cases())
            if (!O.filter(s->name, isfit(O.listing, 'c') ? c->name : "", "")) t++;
         if (t) ::printf("S%d. %s \\\\ %s:%d\n", ++ss, s->name, basename((char*)s->file), s->line);
      }
      if (t = 0, isfit(O.listing, 'c'))
         for (auto& c : s->cases())
            if (!O.filter(s->name, c->name, ""))
               isfit(O.listing, 's') ? ::printf("C%d/S%d-%d. %s // %s \\\\ %s:%d\n", ++cs, ss, ++t, s->name, c->name, basename((char*)c->file), c->line) : ::printf("C%d. %s // %s \\\\ %s:%d\n", ++cs, s->name, c->name, basename((char*)c->file), c->line);
   }
}

h2_inline void h2_directory::list_then_exit()
{
   sort();
   print_list();
   exit(0);
}
