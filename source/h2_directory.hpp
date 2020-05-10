
struct h2_directory {
   h2_singleton(h2_directory);

   h2_list registered_suites, sorted_suites;

   static void drop_last_order();
   static int sort();
};
