
struct h2_directory {
   h2_singleton(h2_directory);

   std::vector<h2_suite*> suites;

   static void drop_last_order();
   static int count();
   static int sort();
   static void print_list();
   static void list_then_exit();
};
