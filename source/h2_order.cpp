
struct h2_order {
   static constexpr const char* last_order_file_path = ".last_order";

   static std::vector<h2_case*> case_list() {
      std::vector<h2_case*> case1_list, case2_list;
      std::vector<std::string> last_list;

      for (auto i = h2_suite::G().begin(); i != h2_suite::G().end(); i++)
         for (auto j = (*i)->cases().begin(); j != (*i)->cases().end(); j++)
            case1_list.push_back(*j);

      //read_last_order
      char suite_case_name[1024];
      h2_file rfp(fopen(last_order_file_path, "r"));
      if (rfp.fp)
         while (1 == fscanf(rfp.fp, "%[^\n]\n", suite_case_name))
            last_list.push_back(suite_case_name);

      if (0 < last_list.size()) {
         for (auto i = last_list.begin(); i != last_list.end(); i++)
            for (auto j = case1_list.begin(); j != case1_list.end(); j++)
               if ((*i) == ((*j)->suite->name + std::string("[:]") + (*j)->name)) {
                  case2_list.push_back(*j), case1_list.erase(j);
                  break;
               }

         for (auto it = case1_list.begin(); it != case1_list.end(); it = case1_list.erase(it))
            case2_list.push_back(*it);

         return case2_list;
      }

      if (h2_cfg().randomize) {
         std::random_device rd;
         std::mt19937 g(rd());
         shuffle(case1_list.begin(), case1_list.end(), g);

         //save_last_order
         h2_file sfp(fopen(last_order_file_path, "w"));
         if (sfp.fp)
            for (auto it = case1_list.begin(); it != case1_list.end(); it++)
               fprintf(sfp.fp, "%s[:]%s\n", (*it)->suite->name, (*it)->name);
      }

      return case1_list;
   }

   static void drop_last_order() { remove(last_order_file_path); }

   static void print_list() {
      int ts = 0, ss = 0;
      for (auto i = h2_suite::G().begin(); i != h2_suite::G().end(); i++) {
         printf("    %4d. (Suite)%s \n", ++ss, strlen((*i)->name) ? (*i)->name : "(Anonymous)");
         for (size_t j = 0; j < (*i)->cases().size(); ++j)
            printf("%4d.    %4zu. %s \n", ++ts, j + 1, (*i)->cases()[j]->name);
      }
   }
};
