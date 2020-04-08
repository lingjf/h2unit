
struct h2_dns : h2_libc {
   h2_list x, y;
   const char* hostname;
   int count;
   char array[32][128];
   h2_dns(const char* hostname_) : hostname(hostname_), count(0) {}
};

struct h2_dnses {
   h2_list s;
   void add(h2_dns* dns);
   void clear();
};

struct h2_ns {
   static void setaddrinfo(int count, ...);
};
