
struct h2__socket {
   h2_singleton(h2__socket);
   h2_list socks;

   static void iport_parse(const char* str, struct sockaddr_in* addr) {
      char temp[1024];
      strcpy(temp, str);

      addr->sin_family = AF_INET;
      addr->sin_port = 0;
      char* colon = strchr(temp, ':');
      if (colon) {
         *colon = '\0';
         addr->sin_port = htons(atoi(colon + 1));
      }
      addr->sin_addr.s_addr = ::inet_addr(temp);
   }

   static const char* iport_tostring(struct sockaddr_in* addr, char* str) {
      sprintf(str, "%s:%d", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
      return str;
   }

   struct temporary_noblock : h2_once {
      int sockfd, flags;
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

   static h2_packet* read_incoming(int sockfd) {
      bool block = !(fcntl(sockfd, F_GETFL) & O_NONBLOCK);
      const char* local = getsockname(sockfd, (char*)alloca(64));
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);

      do {
         h2_list_for_each_entry(p, &sock->incoming, h2_packet, x) if (h2_wildcard_match(p->to.c_str(), local)) {
            p->x.out();
            return p;
         }
         if (block) h2_sleep(100);
      } while (block);
      return nullptr;
   }

   static int accept(int socket, struct sockaddr* address, socklen_t* address_len) {
      h2_packet* tcp = read_incoming(socket);
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
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      sock->sockets.push_back({fd, c, tcp->from.c_str()});
      if (tcp->data.length())
         sock->incoming.push(&tcp->x);
      else
         delete tcp;

      return fd;
   }

   static int connect(int socket, const struct sockaddr* address, socklen_t address_len) {
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      sock->sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = read_incoming(socket);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      if (tcp->data.length())
         sock->incoming.push(&tcp->x);
      else
         delete tcp;
      return 0;
   }

   static ssize_t send(int socket, const void* buffer, size_t length, int flags) {
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      if (sock) sock->put_outgoing(socket, (const char*)buffer, length);
      return length;
   }
   static ssize_t sendmsg(int socket, const struct msghdr* message, int flags) {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
   static ssize_t sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len) {
      h2_sock* sock = h2_list_top_entry(&I().socks, h2_sock, y);
      if (sock) sock->put_outgoing(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return length;
   }
   static ssize_t recv(int socket, void* buffer, size_t length, int flags) {
      ssize_t ret = 0;
      h2_packet* tcp = read_incoming(socket);
      if (tcp) {
         ret = tcp->data.copy((char*)buffer, tcp->data.length(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len) {
      ssize_t ret = 0;
      h2_packet* udp = read_incoming(socket);
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
};

h2_inline h2_sock::h2_sock() {
   stubs.add((void*)::sendto, (void*)h2__socket::sendto, "", "", __FILE__, __LINE__);
   stubs.add((void*)::recvfrom, (void*)h2__socket::recvfrom, "", "", __FILE__, __LINE__);
   stubs.add((void*)::sendmsg, (void*)h2__socket::sendmsg, "", "", __FILE__, __LINE__);
   stubs.add((void*)::recvmsg, (void*)h2__socket::recvmsg, "", "", __FILE__, __LINE__);
   stubs.add((void*)::send, (void*)h2__socket::send, "", "", __FILE__, __LINE__);
   stubs.add((void*)::recv, (void*)h2__socket::recv, "", "", __FILE__, __LINE__);
   stubs.add((void*)::accept, (void*)h2__socket::accept, "", "", __FILE__, __LINE__);
   stubs.add((void*)::connect, (void*)h2__socket::connect, "", "", __FILE__, __LINE__);
   strcpy(last_to, "0.0.0.0:0");
}

h2_inline h2_sock::~h2_sock() {
   x.out(), y.out();
   stubs.clear();
}

h2_inline void h2_sock::put_outgoing(const char* from, const char* to, const char* data, size_t size) {
   strcpy(last_to, to);
   outgoing.push_back(&(new h2_packet(from, to, data, size))->x);
}

h2_inline void h2_sock::put_outgoing(int fd, const char* data, size_t size) {
   char from[128] = "", to[128] = "";
   for (auto& t : sockets)
      if (t.fd == fd) {
         strcpy(from, t.from.c_str());
         strcpy(to, t.to.c_str());
         break;
      }
   put_outgoing(from, to, data, size);
}

h2_inline void h2_sock::put_incoming(const char* from, const char* to, const char* data, size_t size) {
   incoming.push_back(&(new h2_packet(from ? from : last_to, to, data, size))->x);
}

h2_inline h2_packet* h2_socket::start_and_fetch() {
   if (!h2_task::I().current_case) return nullptr;

   h2_sock* sock = h2_task::I().current_case->sock;
   if (!sock) {
      sock = h2_task::I().current_case->sock = new h2_sock();
      h2__socket::I().socks.push(&sock->y);
   }

   return h2_list_pop_entry(&sock->outgoing, h2_packet, x);
}

h2_inline void h2_socket::inject_received(const void* packet, size_t size, const char* from, const char* to) {
   h2_sock* sock = h2_list_top_entry(&h2__socket::I().socks, h2_sock, y);
   if (sock) sock->put_incoming(from, to, (const char*)packet, size);
}
