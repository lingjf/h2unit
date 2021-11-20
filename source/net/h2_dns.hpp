struct h2_dns {
   static void initialize();
   static void setaddrinfo(int count, ...);
};

struct h2_dnses {
   static void add(h2_list& dnses, h2_list& name);
   static void clear(h2_list& dnses);
};

#define H2DNS(...) h2::h2_dns::setaddrinfo(H2PP_NARG(__VA_ARGS__), __VA_ARGS__)
