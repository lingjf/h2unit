#include "../source/h2_unit.cpp"

H2MATCHER(IPeq, expect, ("not equal"))
{
   char s[256];
   struct sockaddr_in* b = (struct sockaddr_in*)a;
   sprintf(s, "%s:%d", inet_ntoa(b->sin_addr), ntohs(b->sin_port));
   return h2::h2_pattern::wildcard_match(expect, s);
}

SUITE(socket utils)
{
   char t[1024];
   struct sockaddr_in a;

   Case(iport parse)
   {
      h2::h2__socket::iport_parse("1.2.3.4", &a);
      OK(IPeq("1.2.3.4:0"), &a);
   }

   Case(iport parse with port)
   {
      h2::h2__socket::iport_parse("1.2.3.4:9527", &a);
      OK(IPeq("1.2.3.4:9527"), &a);
   }

   Case(iport tostring)
   {
      a.sin_family = AF_INET;
      a.sin_addr.s_addr = htonl(INADDR_ANY);
      a.sin_port = htons(9527);
      OK("0.0.0.0:9527", h2::h2__socket::iport_tostring(&a, t));
   }

   Case(iport tostring with port)
   {
      a.sin_family = AF_INET;
      a.sin_addr.s_addr = inet_addr("1.2.3.4");
      a.sin_port = htons(9527);
      OK("1.2.3.4:9527", h2::h2__socket::iport_tostring(&a, t));
   }

   Case(extract_iport_after_equal = 1.2.3.4 : 4444)
   {
      char from[64] = {0};
      h2::extract_iport_after_equal("=1.2.3.4:4444", from);
      OK("1.2.3.4:4444", from);
   }

   Case(extract_iport_after_equal = "1.2.3.4:4444")
   {
      char from[64] = {0};
      h2::extract_iport_after_equal(" = \"1.2.3.4:4444\"", from);
      OK("1.2.3.4:4444", from);
   }

   Case(extract_iport_after_equal = 1.2.3.4 : 4444, )
   {
      char from[64] = {0};
      h2::extract_iport_after_equal(" = 1.2.3.4 : 4444,", from);
      OK("1.2.3.4:4444", from);
   }

   Case(extract_iport_after_equal = 1.2.3.4 : *, )
   {
      char from[64] = {0};
      h2::extract_iport_after_equal(" = 1.2.3.4 : *,", from);
      OK("1.2.3.4:*", from);
   }

   Case(extract_iport_after_equal = *: *, )
   {
      char from[64] = {0};
      h2::extract_iport_after_equal(" = 1.2.3.4 : *,", from);
      OK("*:*", from);
   }
}

SUITE(SOCK)
{
   struct sockaddr_in local = {0};
   local.sin_family = AF_INET;
   local.sin_addr.s_addr = htonl(INADDR_ANY);
   local.sin_port = htons(9527);

   struct sockaddr_in remote = {0};
   remote.sin_family = AF_INET;
   remote.sin_addr.s_addr = inet_addr("1.2.3.4");
   remote.sin_port = htons(8888);

   struct sockaddr_in c;
   socklen_t l;

   char buffer[1024 * 8];
   unsigned char b2[1024];

   Case(UDP sendto recvfrom)
   {
      SOCK();  // Start Hook Socket API
      int sock = socket(AF_INET, SOCK_DGRAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));

      int sent = sendto(sock, "1234567890", 10, 0, (struct sockaddr*)&remote, sizeof(remote));
      OK(10, sent);
      // Fetch outgoing packet
      OK(PktEq("0.0.0.0:9527", "1.2.3.4:8888", Me("1234567890", 10), 10), SOCK());

      // Inject as received packet from 4.3.2.1:4444
      SOCK("9876543210", 10, from = 4.3.2.1 : 4444);
      l = sizeof(struct sockaddr);
      int r1 = recvfrom(sock, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&c, &l);
      OK(10, r1);
      OK(Me("9876543210", 10), buffer);
      OK(IPeq("4.3.2.1:4444"), &c);

      SOCK("9876543210", 10);  // Inject as received packet from last peer
      l = sizeof(struct sockaddr);
      int r2 = recvfrom(sock, (char*)buffer, sizeof(buffer), 0, (struct sockaddr*)&c, &l);
      OK(10, r2);
      OK(Me("9876543210", 10), buffer);
      OK(IPeq("1.2.3.4:8888"), &c);

      close(sock);
   }

   Case(UDP sendmsg recvmsg)
   {
      SOCK();
      int sock = socket(AF_INET, SOCK_DGRAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));

      struct iovec iov1;  /* Data array */
      struct msghdr msg1; /* Message header */
      iov1.iov_base = buffer;
      iov1.iov_len = sizeof(buffer);
      msg1.msg_name = (void*)&c;
      msg1.msg_namelen = sizeof(c);
      msg1.msg_iov = &iov1;
      msg1.msg_iovlen = 1;
      msg1.msg_flags = 0;
      msg1.msg_control = b2;
      msg1.msg_controllen = sizeof(b2);

      SOCK("9876543210",
           10,
           from = "4.3.2.1:4444");  // Inject as received packet from 4.3.2.1:4444
      ssize_t r1 = recvmsg(sock, &msg1, 0);
      OK(10, r1);
      OK(IPeq("4.3.2.1:4444"), &c);

      struct iovec iov2;  /* Data array */
      struct msghdr msg2; /* Message header */
      iov2.iov_base = (void*)"1234567890";
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
      OK(PktEq("*:9527", "1.2.3.4:8888", Me("1234567890", 10), 10), SOCK());

      close(sock);
   }

   Case(TCP server)
   {
      SOCK();
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));

      listen(sock, 10);

      SOCK("", 0, from = "1.2.3.4:8888");  // Inject as Connected Signal from 1.2.3.4:8888
      l = sizeof(c);
      int sock2 = accept(sock, (struct sockaddr*)&c, &l);
      OK(Ge(0), sock2);
      OK(IPeq("1.2.3.4:8888"), &c);

      int sent = send(sock2, "1234567890", 10, 0);
      OK(10, sent);
      OK(PktEq("*:9527", "1.2.3.4:8888", Me("1234567890", 10), 10), SOCK());

      SOCK("9876543210",
           10,
           from = "1.2.3.4:8888");  // Inject as received packet from 4.3.2.1:8888
      int r1 = recv(sock2, (char*)buffer, sizeof(buffer), 0);
      OK(10, r1);
      OK(Me("9876543210", 10), buffer);

      close(sock2);
      close(sock);
   }

   Case(TCP client)
   {
      SOCK();
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      bind(sock, (struct sockaddr*)&local, sizeof(local));

      SOCK("", 0, from = "1.2.3.4:8888");
      connect(sock, (struct sockaddr*)&remote, sizeof(remote));

      int sent = send(sock, "1234567890", 10, 0);
      OK(10, sent);
      OK(PktEq("*:9527", "1.2.3.4:8888", Me("1234567890", 10), 10), SOCK());

      SOCK("9876543210", 10, from = "1.2.3.4:8888");
      int r1 = recv(sock, (char*)buffer, sizeof(buffer), 0);
      OK(10, r1);
      OK(Me("9876543210", 10), buffer);

      close(sock);
   }
}
