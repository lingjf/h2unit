
struct h2_dns : h2_nohook {
   h2_list x, y;
   const char* hostname;
   int count;
   char array[32][128];

   h2_dns(const char* hostname_) : hostname(hostname_) {}
};

struct h2_packet : h2_nohook {
   h2_list x;
   h2_string from, to, data;
   h2_packet(const char* from_, const char* to_, const char* data_, size_t size_) : from(from_), to(to_), data(data_, size_){};
   bool match(const char* from_pattern, const char* to_pattern);
   bool can_recv(const char* local_iport);
};

struct h2_sock : h2_nohook {
   h2_list x, y;

   h2_stub sendto_stub;
   h2_stub recvfrom_stub;
   h2_stub sendmsg_stub;
   h2_stub recvmsg_stub;
   h2_stub send_stub;
   h2_stub recv_stub;
   h2_stub accept_stub;
   h2_stub connect_stub;

   struct socket {
      int fd;
      h2_string from, to;
      socket(int fd_, const char* from_, const char* to_) : fd(fd_), from(from_), to(to_) {}
   };

   h2_vector<socket> sockets;

   h2_sock();
   ~h2_sock();

   void put_outgoing_udp(const char* from, const char* to, const char* data, size_t size);
   void put_incoming_udp(const char* from, const char* to, const char* data, size_t size);
   void put_outgoing_tcp(int fd, const char* data, size_t size);
   void put_incoming_tcp(const char* from, const char* to, const char* data, size_t size);

   char last_to[128];
   h2_list incoming_udps;
   h2_list outgoing_udps;

   h2_list incoming_tcps;
   h2_list outgoing_tcps;
};

template <typename M1, typename M2, typename M3, typename M4>
struct h2_packet_matches {
   const M1 from;
   const M2 to;
   const M3 data;
   const M4 size;
   explicit h2_packet_matches(M1 from_, M2 to_, M3 data_, M4 size_) : from(from_), to(to_), data(data_), size(size_) {}

   h2_fail* matches(h2_packet* a, bool caseless = false, bool dont = false) const {
      h2_fail* fails = nullptr;
      h2_append_y_fail(fails, h2_matcher_cast<const char*>(from).matches(a->from.c_str(), caseless, dont));
      h2_append_y_fail(fails, h2_matcher_cast<const char*>(to).matches(a->to.c_str(), caseless, dont));
      h2_append_y_fail(fails, h2_matcher_cast<const unsigned char*>(data).matches((unsigned char*)a->data.data(), caseless, dont));
      h2_append_y_fail(fails, h2_matcher_cast<const int>(size).matches(a->data.length(), caseless, dont));
      return fails;
   }
};

template <typename M1, typename M2, typename M3, typename M4>
inline h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>> PktEq(M1 from, M2 to, M3 data, M4 size) {
   return h2_polymorphic_matcher<h2_packet_matches<M1, M2, M3, M4>>(h2_packet_matches<M1, M2, M3, M4>(from, to, data, size));
}

struct h2_network_exporter {
   static void setaddrinfo(int count, ...);
   static h2_packet* sock_start_and_fetch();
   static void udp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to);
   static void tcp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to);
};
