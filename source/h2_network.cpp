
static inline bool is_ipv4(const char* str) {
   int s1, s2, s3, s4;
   return 4 == ::sscanf(str, "%d.%d.%d.%d", &s1, &s2, &s3, &s4);
}

struct h2_network {
   h2_singleton(h2_network);

   h2_list dnss, socks;

   static bool inet_addr(const char* str, struct sockaddr_in* addr) {
      int s1, s2, s3, s4;
      if (4 == ::sscanf(str, "%d.%d.%d.%d", &s1, &s2, &s3, &s4)) {
         addr->sin_family = AF_INET;
         addr->sin_addr.s_addr = ::inet_addr(str);
         return true;
      }
      return false;
   }

   static bool iport_parse(const char* str, struct sockaddr_in* addr) {  // todo for simplify
      char* colon = NULL;
      char temp[1024];
      strcpy(temp, str);

      addr->sin_family = AF_INET;
      addr->sin_port = 0;
      colon = strchr(temp, ':');
      if (colon) {
         *colon = '\0';
         addr->sin_port = htons(atoi(colon + 1));
      }
      addr->sin_addr.s_addr = ::inet_addr(temp);
      return true;
   }

   static const char* iport_tostring(struct sockaddr_in* addr, char* str) {
      sprintf(str, "%s:%d", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
      return str;
   }

   static bool isblock(int sockfd) {
      return !(fcntl(sockfd, F_GETFL) & O_NONBLOCK);
   }

   struct temporary_noblock : h2_once {
      int sockfd;
      int flags;
      temporary_noblock(int sockfd_) : sockfd(sockfd_) {
         flags = fcntl(sockfd, F_GETFL);
         fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
      }
      ~temporary_noblock() { fcntl(sockfd, F_SETFL, flags); }
   };

   static const char* getsockname(int sockfd, char* s, struct sockaddr_in* a = nullptr) {
      struct sockaddr_in b;
      if (!a) a = &b;
      socklen_t l = sizeof(struct sockaddr_in);
      ::getsockname(sockfd, (struct sockaddr*)a, &l);
      return iport_tostring(a, s);
   }

   h2_dns* find(const char* hostname) {
      h2_list_for_each_entry(p, &dnss, h2_dns, y) if (streq("*", p->hostname) || streq(hostname, p->hostname)) return p;
      return nullptr;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res) {
      h2_dns* dns = I().find(hostname);
      if (!dns) return -1;

      static struct addrinfo addrinfos[32];
      static struct sockaddr_in sockaddrs[32];
      memset(addrinfos, 0, sizeof(addrinfos));
      memset(sockaddrs, 0, sizeof(sockaddrs));

      struct addrinfo** pp = res;
      for (int i = 0; i < dns->count; ++i) {
         struct addrinfo* a = &addrinfos[i];
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(dns->array[i], b)) {
            a->ai_addr = (struct sockaddr*)b;
            a->ai_addrlen = sizeof(struct sockaddr_in);
         } else
            a->ai_canonname = dns->array[i];
         if (hints) {
            a->ai_family = hints->ai_family;
            a->ai_socktype = hints->ai_socktype;
            a->ai_protocol = hints->ai_protocol;
         } else {
            a->ai_family = AF_INET;
            a->ai_socktype = SOCK_STREAM;
            a->ai_protocol = IPPROTO_TCP;
         }
         a->ai_next = nullptr;
         *pp = a, pp = &a->ai_next;
      }
      *pp = nullptr;
      return 0;
   }

   static void freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* gethostbyname(char* name) {
      h2_dns* dns = I().find(name);
      if (!dns) return nullptr;

      static struct sockaddr_in sockaddrs[32];
      static char* h_aliases[32];
      static char* h_addr_list[32];
      static struct hostent h;

      h.h_name = name;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      memset(sockaddrs, 0, sizeof(sockaddrs));
      memset(h_aliases, 0, sizeof(h_aliases));
      memset(h_addr_list, 0, sizeof(h_addr_list));

      for (int i = 0, a = 0, c = 0; i < dns->count; ++i) {
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(dns->array[i], b))
            h_addr_list[a++] = (char*)&b->sin_addr;
         else
            h_aliases[c++] = dns->array[i];
      }
      return &h;
   }

   static void read_incoming(int sockfd, h2_list& list, bool sync, h2_packet*& packet) {
      bool block = isblock(sockfd);
      const char* local = getsockname(sockfd, (char*)alloca(64));
      do {
         h2_list_for_each_entry(p, &list, h2_packet, x) {
            // return h2_wildcard_match(to.c_str(), local_iport);
            // printf("shit local=%s, packet_to=%s \n", local, p->to.c_str());
            if (p->can_recv(local) && (!sync || p->data.length() == 0)) {
               p->x.out();
               packet = p;
               return;
            }
         }
         if (block) h2_sleep(100);
      } while (block);
   }

   static int accept(int socket, struct sockaddr* address, socklen_t* address_len) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      h2_packet* tcp = nullptr;
      read_incoming(socket, sock->incoming_tcps, true, tcp);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      iport_parse(tcp->from.c_str(), (struct sockaddr_in*)address);
      *address_len = sizeof(struct sockaddr_in);

      int fd = ::socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in a;
      const char* c = getsockname(socket, (char*)alloca(64), &a);
      ::bind(fd, (struct sockaddr*)&a, sizeof(a));
      sock->sockets.push_back({fd, c, tcp->from.c_str()});
      delete tcp;

      return fd;
   }

   static int connect(int socket, const struct sockaddr* address, socklen_t address_len) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      sock->sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = nullptr;
      read_incoming(socket, sock->incoming_tcps, true, tcp);

      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      delete tcp;
      return 0;
   }

   static ssize_t send(int socket, const void* buffer, size_t length, int flags) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      if (sock) sock->put_outgoing_tcp(socket, (const char*)buffer, length);
      return length;
   }
   static ssize_t sendmsg(int socket, const struct msghdr* message, int flags) {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
   static ssize_t sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      if (sock) sock->put_outgoing_udp(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return length;
   }
   static ssize_t recv(int socket, void* buffer, size_t length, int flags) {
      ssize_t ret = 0;
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);

      h2_packet* tcp = nullptr;
      read_incoming(socket, sock->incoming_tcps, false, tcp);

      if (tcp) {
         ret = tcp->data.copy((char*)buffer, tcp->data.length(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len) {
      ssize_t ret = 0;
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);

      h2_packet* udp = nullptr;
      read_incoming(socket, sock->incoming_udps, false, udp);

      if (udp) {
         ret = udp->data.copy((char*)buffer, udp->data.length(), 0);
         iport_parse(udp->from.c_str(), (struct sockaddr_in*)address);
         *address_len = sizeof(struct sockaddr_in);
         delete udp;
      }
      return ret;
   }
   static ssize_t recvmsg(int socket, struct msghdr* message, int flags) {
      return recvfrom(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, &message->msg_namelen);
   }

   h2_stub getaddrinfo_stub;
   h2_stub freeaddrinfo_stub;
   h2_stub gethostbyname_stub;

   h2_network() : getaddrinfo_stub((void*)::getaddrinfo),
                  freeaddrinfo_stub((void*)::freeaddrinfo),
                  gethostbyname_stub((void*)::gethostbyname) {
      getaddrinfo_stub.replace((void*)getaddrinfo);
      freeaddrinfo_stub.replace((void*)freeaddrinfo);
      gethostbyname_stub.replace((void*)gethostbyname);
   }

   ~h2_network() {
      // getaddrinfo_stub.restore();
      // freeaddrinfo_stub.restore();
      // gethostbyname_stub.restore();
   }
};

h2_inline bool h2_packet::match(const char* from_pattern, const char* to_pattern) {
   return h2_wildcard_match(from_pattern, from.c_str()) &&
          h2_wildcard_match(to_pattern, to.c_str());
}
h2_inline bool h2_packet::can_recv(const char* local_iport) {
   return h2_wildcard_match(to.c_str(), local_iport);
}

h2_inline h2_sock::h2_sock() : sendto_stub((void*)::sendto),
                               recvfrom_stub((void*)::recvfrom),
                               sendmsg_stub((void*)::sendmsg),
                               recvmsg_stub((void*)::recvmsg),
                               send_stub((void*)::send),
                               recv_stub((void*)::recv),
                               accept_stub((void*)::accept),
                               connect_stub((void*)::connect) {
   sendto_stub.replace((void*)h2_network::sendto);
   recvfrom_stub.replace((void*)h2_network::recvfrom);
   sendmsg_stub.replace((void*)h2_network::sendmsg);
   recvmsg_stub.replace((void*)h2_network::recvmsg);
   send_stub.replace((void*)h2_network::send);
   recv_stub.replace((void*)h2_network::recv);
   accept_stub.replace((void*)h2_network::accept);
   connect_stub.replace((void*)h2_network::connect);
   strcpy(last_to, "0.0.0.0:0");
}

h2_inline h2_sock::~h2_sock() {
   x.out();
   y.out();
   sendto_stub.restore();
   recvfrom_stub.restore();
   sendmsg_stub.restore();
   recvmsg_stub.restore();
   send_stub.restore();
   recv_stub.restore();
   accept_stub.restore();
   connect_stub.restore();
}

h2_inline void h2_sock::put_outgoing_udp(const char* from, const char* to, const char* data, size_t size) {
   strcpy(last_to, to);
   h2_packet* udp = new h2_packet(from, to, data, size);
   outgoing_udps.push_back(&udp->x);
}

h2_inline void h2_sock::put_incoming_udp(const char* from, const char* to, const char* data, size_t size) {
   h2_packet* udp = new h2_packet(from ? from : last_to, to, data, size);
   incoming_udps.push_back(&udp->x);
}

h2_inline void h2_sock::put_outgoing_tcp(int fd, const char* data, size_t size) {
   char from[128], to[128];

   for (auto& t : sockets) {
      if (t.fd == fd) {
         strcpy(from, t.from.c_str());
         strcpy(to, t.to.c_str());
         break;
      }
   }

   strcpy(last_to, to);

   h2_packet* tcp = nullptr;
   h2_list_for_each_entry(p, &outgoing_tcps, h2_packet, x) {
      if (p->from == from && p->to == to) {
         tcp = p;
         break;
      }
   }
   if (tcp) {
      tcp->data.append(data, size);
   } else {
      tcp = new h2_packet(from, to, data, size);
      outgoing_tcps.push_back(&tcp->x);
   }
}

h2_inline void h2_sock::put_incoming_tcp(const char* from, const char* to, const char* data, size_t size) {
   from = from ? from : last_to;
   h2_packet* tcp = nullptr;
   h2_list_for_each_entry(p, &incoming_tcps, h2_packet, x) {
      if (p->from == from && p->to == to) {
         tcp = p;
         break;
      }
   }
   if (tcp) {
      tcp->data.append(data, size);
   } else {
      tcp = new h2_packet(from, to, data, size);
      incoming_tcps.push_back(&tcp->x);
   }
}

h2_inline void h2_network_exporter::setaddrinfo(int n, ...) {
   if (n == 0) return;

   const char* array[32];
   int count = 0;
   va_list a;
   va_start(a, n);
   for (int i = 0; i < n && i < 32; ++i)
      array[count++] = va_arg(a, const char*);
   va_end(a);

   const char* hostname = "*";
   for (int i = 0; i < count; ++i)
      if (!is_ipv4(array[i]))
         if (strlen(hostname) < 2 || strlen(array[i]) < strlen(hostname))
            hostname = array[i];

   h2_dns* dns = new h2_dns(hostname);

   for (int i = 0; i < count; ++i)
      if (!streq(hostname, array[i]))
         strcpy(dns->array[dns->count++], array[i]);

   h2_network::I().dnss.push(&dns->y);
   h2_dns_g(dns);
}

h2_inline h2_packet* h2_network_exporter::sock_start_and_fetch() {
   h2_sock* sock = h2_sock_g(nullptr);
   if (!sock) {
      sock = h2_sock_g(new h2_sock());
      h2_network::I().socks.push(&sock->y);
   }

   h2_packet* ret = nullptr;
   ret = h2_list_pop_entry(&sock->outgoing_udps, h2_packet, x);
   if (!ret)
      ret = h2_list_pop_entry(&sock->outgoing_tcps, h2_packet, x);
   return ret;
}

h2_inline void h2_network_exporter::udp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to) {
   h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
   if (sock) sock->put_incoming_udp(from, to, (const char*)packet, size);
}

h2_inline void h2_network_exporter::tcp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to) {
   h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
   if (sock) sock->put_incoming_tcp(from, to, (const char*)packet, size);
}
