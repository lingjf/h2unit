#include "../source/h2_unit.cpp"

static int dns_use_getaddrinfo(const char* hostname, char* ips, char* cnames)
{
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
            sprintf(ips + strlen(ips),
                    "%s;",
                    inet_ntoa(((sockaddr_in*)(i->ai_addr))->sin_addr));
         }
         if (i->ai_canonname) {
            sprintf(cnames + strlen(cnames), "%s;", i->ai_canonname);
         }
      }
      freeaddrinfo(res);
   }

   return err;
}

static void* dns_use_gethostbyname(const char* hostname, char* ips, char* cnames)
{
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
      sprintf(ips + strlen(ips),
              "%s;",
              inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
   }
   return host;
}

GlobalSetup()
{
   DNS("127.0.0.1");
}

GlobalTeardown() {}

SUITE(getaddrinfo)
{
   int ret;
   char t1[1024] = "", t2[1024] = "";

   Case(global)
   {
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("127.0.0.1;", t1);
   }

   Case(DNS 1)
   {
      DNS("192.168.1.123");
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("192.168.1.123;", t1);
   }

   Case(DNS 2)
   {
      DNS("dns.google.com", "192.168.1.123", "1.2.3.4");
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("192.168.1.123;1.2.3.4;", t1);
   }

   Case(DNS cname)
   {
      DNS("dns.google.com", "1.dns.google.com", "1.2.3.4");
      ret = dns_use_getaddrinfo("dns.google.com", t1, t2);
      OK(0, ret);
      OK("1.2.3.4;", t1);
      OK("1.dns.google.com;", t2);
   }
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
   }

   Case(DNS 2)
   {
      DNS("dns.google.com", "192.168.1.123", "1.2.3.4");
      ret = dns_use_gethostbyname("dns.google.com", t1, t2);
      OK(ret);
      OK("192.168.1.123;1.2.3.4;", t1);
   }

   Case(DNS cname)
   {
      DNS("dns.google.com", "1.dns.google.com", "1.2.3.4");
      ret = dns_use_gethostbyname("dns.google.com", t1, t2);
      OK(ret);
      OK("1.2.3.4;", t1);
      OK("1.dns.google.com;", t2);
   }
}
