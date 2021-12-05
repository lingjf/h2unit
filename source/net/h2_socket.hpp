struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(size_, data_){};
};

struct h2_sock : h2_once {
   h2_sock();
   ~h2_sock();
   static void clear();
   static void inject(const void* packet, size_t size, const char* attributes = "");  // from=1.2.3.4:5678, to=4.3.2.1:8765

   template <typename M1 = h2_polymorphic_matcher<h2_matches_any>, typename M2 = h2_polymorphic_matcher<h2_matches_any>, typename M3 = h2_polymorphic_matcher<h2_matches_any>, typename M4 = h2_polymorphic_matcher<h2_matches_any>>
   static void check(const char* filine, const char* e, M1 from = Any, M2 to = Any, M3 payload = Any, M4 size = Any);
};

#define __H2SOCK(Q) for (h2::h2_sock Q; Q;)
#define H2SOCK(...) __H2SOCK(H2PP_UNIQUE())

#define Ptx(...) h2::h2_sock::check(H2_FILINE, h2::ss(#__VA_ARGS__), __VA_ARGS__)
#define Pij(Packet_, Size_, ...) h2::h2_sock::inject(Packet_, Size_, h2::ss(#__VA_ARGS__))
