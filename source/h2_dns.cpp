
struct h2_resolver {
   h2_singleton(h2_resolver);
   h2_list dnses;

   static bool inet_addr(const char* str, struct sockaddr_in* addr)
   {
      struct sockaddr_in a;
      if (!addr) addr = &a;
      return 1 == inet_pton(AF_INET, str, &addr->sin_addr);
   }

   h2_dns* find(const char* hostname)
   {
      h2_list_for_each_entry (p, &dnses, h2_dns, y)
         if (!strcmp("*", p->name) || !strcmp(hostname, p->name))
            return p;
      return nullptr;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res)
   {
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

   static struct hostent* gethostbyname(char* name)
   {
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

   h2_stubs stubs;
   h2_resolver()
   {
      stubs.add((void*)::getaddrinfo, (void*)getaddrinfo);
      stubs.add((void*)::freeaddrinfo, (void*)freeaddrinfo);
      stubs.add((void*)::gethostbyname, (void*)gethostbyname);
   }
   ~h2_resolver() { stubs.clear(); }
};

h2_inline void h2_dnses::add(h2_dns* dns) { dnses.push(&dns->x); }
h2_inline void h2_dnses::clear()
{
   h2_list_for_each_entry (p, &dnses, h2_dns, x) {
      p->x.out(), p->y.out();
      delete p;
   }
}

h2_inline void h2_dns::setaddrinfo(int n, ...)
{
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
      if (!h2_resolver::inet_addr(array[i], nullptr))
         if (strlen(hostname) < 2 || strlen(array[i]) < strlen(hostname))
            hostname = array[i];

   h2_dns* dns = new h2_dns(hostname);
   for (int i = 0; i < count; ++i)
      if (strcmp(hostname, array[i]))
         strcpy(dns->array[dns->count++], array[i]);

   h2_resolver::I().dnses.push(&dns->y);
   if (h2_task::I().current_case) h2_task::I().current_case->dnses.add(dns);
}

h2_inline void h2_dns::initialize()
{
   setaddrinfo(1, "127.0.0.1");
}
