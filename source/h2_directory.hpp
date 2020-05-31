
struct h2_directory {
   h2_singleton(h2_directory);

   h2_list suites;

   static void drop_last_order();
   static void sort();
   static int count();
};
