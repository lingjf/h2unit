
struct h2_directory {
   static constexpr const char* last_order_file_path = ".last_order";

   static void drop_last_order() { ::remove(last_order_file_path); }

   static void read_last_order(std::vector<std::string>& list) {
      char suite_case[1024];
      h2_with f(fopen(last_order_file_path, "r"));
      if (f.f)
         while (1 == fscanf(f.f, "%[^\n]\n", suite_case))
            list.push_back(suite_case);
   }

   static void save_last_order(std::vector<h2_case*>& list) {
      h2_with f(fopen(last_order_file_path, "w"));
      if (f.f)
         for (auto c : list)
            fprintf(f.f, "%s/*//*/%s\n", c->suite->name, c->name);
   }

   static std::vector<h2_case*> cases() {
      std::vector<h2_case*> source_list, retest_list;
      std::vector<std::string> last_list;

      for (auto s : h2_suite::suites())
         for (auto c : s->cases())
            source_list.push_back(c);

      read_last_order(last_list);

      if (0 < last_list.size()) {
         for (auto& k : last_list)
            for (auto it = source_list.begin(); it != source_list.end(); it++)
               if (k == (*it)->suite->name + std::string("/*//*/") + (*it)->name) {
                  retest_list.push_back(*it);
                  source_list.erase(it);
                  break;
               }

         for (auto it = source_list.begin(); it != source_list.end(); it = source_list.erase(it))
            retest_list.push_back(*it);

         return retest_list;
      }

      if (O().randomize) {
         shuffle(source_list.begin(), source_list.end(), std::default_random_engine{std::random_device()()});
         save_last_order(source_list);
      }

      return source_list;
   }

   static void print_list() {
      int ss = 0, cs = 0, t = O().listing;
      bool sb = t == 'a' || t == 'A' || t == 's' || t == 'S', cb = t == 'a' || t == 'A' || t == 'c' || t == 'C';

      for (auto s : h2_suite::suites()) {
         if (t = 0, sb) {
            if (!O().filter(s->name, "", "")) t++;
            for (auto c : s->cases())
               if (!O().filter(s->name, cb ? c->name : "", "")) t++;
            if (t) printf("S%d. %s \\\\ %s:%d\n", ++ss, s->name, basename((char*)s->file), s->line);
         }
         if (t = 0, cb)
            for (auto c : s->cases())
               if (!O().filter(s->name, c->name, ""))
                  sb ? printf("C%d/S%d-%d. %s // %s \\\\ %s:%d\n", ++cs, ss, ++t, s->name, c->name, basename((char*)c->file), c->line) :
                       printf("C%d. %s // %s \\\\ %s:%d\n", ++cs, s->name, c->name, basename((char*)c->file), c->line);
      }
   }

   static void list_then_exit() { print_list(), exit(0); }
};
