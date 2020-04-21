
struct h2_dns : h2_libc {
   h2_list x, y;
   const char* name;
   int count;
   char array[32][128];
   h2_dns(const char* name_) : name(name_), count(0) {}
   static void setaddrinfo(int count, ...);
   static void initialize();
};

struct h2_dnses {
   h2_list dnses;
   void add(h2_dns* dns);
   void clear();
};
