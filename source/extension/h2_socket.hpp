
struct h2_packet : h2_libc {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(size_, data_){};
};

struct h2_socks : h2_libc {
   h2_list x, y;
   h2_stubs stubs;

   struct socket {
      int fd;
      h2_string from, to;
      socket(int fd_, const char* from_, const char* to_) : fd(fd_), from(from_), to(to_) {}
   };

   h2_vector<socket> sockets;

   h2_socks();
   ~h2_socks();

   void put_outgoing(const char* from, const char* to, const char* data, size_t size);
   void put_outgoing(int fd, const char* data, size_t size);
   void put_incoming(const char* from, const char* to, const char* data, size_t size);

   char last_to[128];
   h2_list incoming, outgoing;
};

template <typename M1, typename M2, typename M3, typename M4>
struct h2_packet_matches : h2_matches {
   const M1 from;
   const M2 to;
   const M3 data;
   const M4 size;
   explicit h2_packet_matches(M1 from_, M2 to_, M3 data_, M4 size_) : from(from_), to(to_), data(data_), size(size_) {}

   h2_fail* matches(h2_packet* a, int, bool caseless = false, bool dont = false) const
   {
      h2_fail* fails = nullptr;
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(from).matches(a->from.c_str(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const char*>(to).matches(a->to.c_str(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const unsigned char*>(data).matches((unsigned char*)a->data.data(), 0, caseless, dont));
      h2_fail::append_subling(fails, h2_matcher_cast<const int>(size).matches(a->data.size(), 0, caseless, dont));
      return fails;
   }

   virtual h2_row expection(bool, bool) const override { return ""; }
};

template <typename M1, typename M2, typename M3, typename M4>
inline h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>> PktEq(M1 from = Any, M2 to = Any, M3 data = Any, M4 size = Any)
{
   return h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>>(h2_packet_matches<M1, M2, M3, M4>(from, to, data, size));
}

struct h2_sock {
   static h2_packet* start_and_fetch();
   static void inject_received(const void* packet, size_t size, const char* attributes); // from=1.2.3.4:5678, to=4.3.2.1:8765
};

/* clang-format off */
#define __H2SOCK0(_Packet, _Size, ...) h2::h2_sock::inject_received(_Packet, _Size, #__VA_ARGS__)
#define __H2SOCK1(...) h2::h2_sock::start_and_fetch()
#define H2SOCK(...) H2PP_CAT(__H2SOCK, H2PP_IS_EMPTY(__VA_ARGS__)) (__VA_ARGS__)
