#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

static int dns_use_getaddrinfo(const char* hostname, char* ips, char* cnames) {
   strcpy(ips, "");
   strcpy(cnames, "");

   addrinfo hints, *res;

   memset(&hints, 0, sizeof(addrinfo));
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_family = AF_INET;

   int err = getaddrinfo(hostname, NULL, &hints, &res);
   if (err == 0) {
      for (auto i = res; i; i = i->ai_next) {
         if (i->ai_addr) {
            sprintf(ips + strlen(ips), "%s;", inet_ntoa(((sockaddr_in*)(i->ai_addr))->sin_addr));
         }
         if (i->ai_canonname) {
            sprintf(cnames + strlen(cnames), "%s;", i->ai_canonname);
         }
      }
      freeaddrinfo(res);
   }

   return err;
}

static void* dns_use_gethostbyname(const char* hostname, char* ips, char* cnames) {
   strcpy(ips, "");
   strcpy(cnames, "");

   struct hostent* host = gethostbyname(hostname);
   if (!host) {
      return NULL;
   }

   for (int i = 0; host->h_aliases[i]; i++) {
      sprintf(cnames + strlen(cnames), "%s;", host->h_aliases[i]);
   }

   for (int i = 0; host->h_addr_list[i]; i++) {
      sprintf(ips + strlen(ips), "%s;", inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
   }
   return host;
}

GlobalSetup() {
   DNS("127.0.0.1");
};

GlobalTeardown() {
};

SUITE(getaddrinfo) 
{
   int ret;
   char t1[1024] = "", t2[1024] = "";

   Case(global) {
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("127.0.0.1;", t1);
   };

   Case(DNS 1) 
   {
      DNS("192.168.1.123");
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("192.168.1.123;", t1);
   };

   Case(DNS 2) 
   {
      DNS("dns.google.com", "192.168.1.123", "1.2.3.4");
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("192.168.1.123;1.2.3.4;", t1);
   };

   Case(DNS cname) 
   {
      DNS("dns.google.com", "1.dns.google.com", "1.2.3.4");
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("1.2.3.4;", t1);
      OK("1.dns.google.com;", t2);
   };
}

SUITE(gethostbyname) 
{
   void* ret;
   char t1[1024] = "", t2[1024] = "";

   //    Case(system) {
   //       ret = dns_use_gethostbyname("dns.google.com", t1, t2);
   //       OK(ret);
   //       OK(Contains("8.8.8.8;"), t1);
   //    };

   Case(DNS 1)
   {
      DNS("192.168.1.123");
      ret = dns_use_gethostbyname("dns.google.com", t1, t2);
      OK(ret);
      OK("192.168.1.123;", t1);
   };

   Case(DNS 2)
   {
      DNS("dns.google.com", "192.168.1.123", "1.2.3.4");
      ret = dns_use_gethostbyname("dns.google.com", t1, t2);
      OK(ret);
      OK("192.168.1.123;1.2.3.4;", t1);
   };

   Case(DNS cname)
   {
      DNS("dns.google.com", "1.dns.google.com", "1.2.3.4");
      ret = dns_use_gethostbyname("dns.google.com", t1, t2);
      OK(ret);
      OK("1.2.3.4;", t1);
      OK("1.dns.google.com;", t2);
   };
}

H2MATCHER(IPeq, expect, ("not equal"))
{
   char s[256];
   struct sockaddr_in* b = (struct sockaddr_in*)a;
   sprintf(s, "%s:%d", inet_ntoa(b->sin_addr), ntohs(b->sin_port));
   return h2::h2_wildcard_match(expect, s);
}

SUITE(SOCK)
{
   struct sockaddr_in local = {0};
   local.sin_family = AF_INET;
   local.sin_addr.s_addr = htonl(INADDR_ANY);
   local.sin_port = htons(9527);

   struct sockaddr_in remote = {0};
   remote.sin_family = AF_INET;
   remote.sin_addr.s_addr = ntohl(inet_addr("8.8.8.8"));
   remote.sin_port = htons(8888);

   struct sockaddr_in c;
   socklen_t l;

   char buffer[1024 * 8];
   unsigned char b2[1024];

   Case(UDP sendto recvfrom) 
   {
      H2SOCK();
      int sock = socket(AF_INET, SOCK_DGRAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));

      int sent = sendto(sock, "1234567890", 10, 0, (struct sockaddr*)&remote, sizeof(remote));
      OK(10, sent);
      OK(PktEq("0.0.0.0:9527", "8.8.8.8:8888", Me("1234567890", 10), 10), H2SOCK());

      H2UDP((const unsigned char*)"9876543210", 10, "4.4.4.4:4444");
      l = sizeof(struct sockaddr);
      int r1 = recvfrom(sock, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&c, &l);
      OK(10, r1);
      OK(Me("9876543210", 10), buffer);
      OK(IPeq("4.4.4.4:4444"), &c);

      H2UDP((const unsigned char*)"9876543210", 10);
      l = sizeof(struct sockaddr);
      int r2 = recvfrom(sock, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&c, &l);
      OK(10, r2);
      OK(Me("9876543210", 10), buffer);
      OK(IPeq("8.8.8.8:8888"), &c);

      close(sock);
   }

   Case(UDP sendmsg recvmsg)
   {
      H2SOCK();
      int sock = socket(AF_INET, SOCK_DGRAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));

      struct iovec iov1;     /* Data array */
      struct msghdr msg1;    /* Message header */
      iov1.iov_base = buffer;
      iov1.iov_len = sizeof(buffer);
      msg1.msg_name = (void*)&c;
      msg1.msg_namelen = sizeof(c);
      msg1.msg_iov = &iov1;
      msg1.msg_iovlen = 1;
      msg1.msg_flags = 0;
      msg1.msg_control = b2;
      msg1.msg_controllen = sizeof(b2);

      H2UDP((const unsigned char*)"9876543210", 10, "4.4.4.4:4444");
      ssize_t r1 = recvmsg(sock, &msg1, 0);
      OK(10, r1);
      OK(IPeq("4.4.4.4:4444"), &c);

      struct iovec iov2;     /* Data array */
      struct msghdr msg2;    /* Message header */
      iov2.iov_base = (void *)"1234567890";
      iov2.iov_len = 10;
      msg2.msg_name = (void*)&remote;
      msg2.msg_namelen = sizeof(remote);
      msg2.msg_iov = &iov2;
      msg2.msg_iovlen = 1;
      msg2.msg_flags = 0;
      msg2.msg_control = b2;
      msg2.msg_controllen = sizeof(b2);
      ssize_t r2 = sendmsg(sock, &msg2, 0);
      OK(10, r2);
      OK(PktEq("*:9527", "8.8.8.8:8888", Me("1234567890", 10), 10), H2SOCK());

      close(sock);
   }

   Case(TCP server) {
      H2SOCK();
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));

      listen(sock, 10);

      H2TCP((const unsigned char*)"", 0, "4.4.4.4:4444");
      l = sizeof(c);
      int sock2 = accept(sock, (struct sockaddr*)&c, &l);
      OK(Ge(0), sock2);
      OK(IPeq("4.4.4.4:4444"), &c);

      int sent = send(sock2, "1234567890", 10, 0);
      OK(10, sent);
      OK(PktEq("*:9527", "4.4.4.4:4444", Me("1234567890", 10), 10), H2SOCK());

      H2TCP((const unsigned char*)"9876543210", 10, "4.4.4.4:4444");
      int r1 = recv(sock2, (char*)buffer, sizeof(buffer), 0);
      OK(10, r1);
      OK(Me("9876543210", 10), buffer);

      H2TCP((const unsigned char*)"0123456789", 10, "4.4.4.4:4444");
      H2TCP((const unsigned char*)"ABCDEFGHIJ", 10, "4.4.4.4:4444");
      int r2 = recv(sock2, (char*)buffer, sizeof(buffer), 0);
      OK(20, r2);
      OK(Me("0123456789ABCDEFGHIJ", 10), buffer);

      close(sock2);
      close(sock);
   }

   Case(TCP client) {
      H2SOCK();
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));
  
      H2TCP((const unsigned char*)"", 0, "8.8.8.8:8888");
      connect(sock, (struct sockaddr *)&remote, sizeof(remote));

      int sent = send(sock, "1234567890", 10, 0);
      OK(10, sent);
      OK(PktEq("*:9527", "8.8.8.8:8888", Me("1234567890", 10), 10), H2SOCK());

      H2TCP((const unsigned char*)"9876543210", 10, "8.8.8.8:8888");
      int r1 = recv(sock, (char*)buffer, sizeof(buffer), 0);
      OK(10, r1);
      OK(Me("9876543210", 10), buffer);

      H2TCP((const unsigned char*)"0123456789", 10, "8.8.8.8:8888");
      H2TCP((const unsigned char*)"ABCDEFGHIJ", 10, "8.8.8.8:8888");
      int r2 = recv(sock, (char*)buffer, sizeof(buffer), 0);
      OK(20, r2);
      OK(Me("0123456789ABCDEFGHIJ", 10), buffer);

      close(sock);
   }
}
