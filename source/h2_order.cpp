
struct h2_order {
   static constexpr const char* last_order_file_path = ".last_order";

   static void drop_last_order() { ::remove(last_order_file_path); }

   static void read_last_order(std::vector<std::string>& list) {
      char suite_case_name[1024];
      h2_with f(fopen(last_order_file_path, "r"));
      if (f.f)
         while (1 == fscanf(f.f, "%[^\n]\n", suite_case_name))
            list.push_back(suite_case_name);
   }

   static void save_last_order(std::vector<h2_case*>& list) {
      h2_with f(fopen(last_order_file_path, "w"));
      if (f.f)
         for (auto c : list)
            fprintf(f.f, "%s[:]%s\n", c->suite->name, c->name);
   }

   static std::vector<h2_case*> case_list() {
      std::vector<h2_case*> case1_list, case2_list;
      std::vector<std::string> last_list;

      for (auto s : h2_suite::suites())
         for (auto c : s->cases())
            case1_list.push_back(c);

      read_last_order(last_list);

      if (0 < last_list.size()) {
         for (auto& k : last_list)
            for (auto it = case1_list.begin(); it != case1_list.end(); it++)
               if (k == (*it)->suite->name + std::string("[:]") + (*it)->name) {
                  case2_list.push_back(*it);
                  case1_list.erase(it);
                  break;
               }

         for (auto it = case1_list.begin(); it != case1_list.end(); it = case1_list.erase(it))
            case2_list.push_back(*it);

         return case2_list;
      }

      if (h2cfg().randomize) {
         shuffle(case1_list.begin(), case1_list.end(), std::default_random_engine{std::random_device()()});
         save_last_order(case1_list);
      }

      return case1_list;
   }

   static void print_list() {
      int ss = 0, cs = 0, t = h2cfg().listing;
      bool sb = t == 'a' || t == 'A' || t == 's' || t == 'S', cb = t == 'a' || t == 'A' || t == 'c' || t == 'C';

      for (auto s : h2_suite::suites()) {
         const char* sn = strlen(s->name) ? s->name : "(Anonymous)";
         if (t = 0, sb) {
            if (!h2cfg().filter(sn, "", "")) t++;
            for (auto c : s->cases())
               if (!h2cfg().filter(sn, cb ? c->name : "", "")) t++;
            if (t) printf("S%d. %s \n", ++ss, sn);
         }
         if (t = 0, cb)
            for (auto c : s->cases())
               if (!h2cfg().filter(sn, c->name, ""))
                  sb ? printf("C%d/S%d/%d. %s // %s \n", ++cs, ss, ++t, sn, c->name) : printf("C%d. %s // %s \n", ++cs, sn, c->name);
      }
   }

   static void list_then_exit() { print_list(), exit(0); }
};
