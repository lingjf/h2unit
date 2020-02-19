
struct h2_addr {
   h2_list x, y;
   const char* hostname;
   int count;
   struct sockaddr_storage array[32];

   h2_addr(const char* hostname_, int count_, va_list& args) : hostname(hostname_), count(0) {
      for (int i = 0, s1, s2, s3, s4; i < count_ && i < 32; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&array[count++];
         memset(b, 0, sizeof(struct sockaddr_storage));
         const char* p = va_arg(args, const char*);
         if (4 == ::sscanf(p, "%d.%d.%d.%d", &s1, &s2, &s3, &s4)) {
            b->sin_family = AF_INET;
            b->sin_addr.s_addr = inet_addr(p);
         } else
            strcpy((char*)&b->sin_addr, p);
      }
   }

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
};

static inline void h2_addr_g(h2_addr* addr);

struct h2_dns {
   h2_list addrs;

   h2_addr* find(const char* hostname) {
      h2_list_for_each_entry(p, &addrs, h2_addr, y) if (strlen(p->hostname) == 0 || strcmp(hostname, p->hostname) == 0) return p;
      return nullptr;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res) {
      h2_addr* addr = I().find(hostname);
      if (!addr) return -1;

      static struct addrinfo addrinfos[32];
      memset(addrinfos, 0, sizeof(addrinfos));

      struct addrinfo** pp = res;
      for (int i = 0; i < addr->count; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&addr->array[i];
         struct addrinfo* a = &addrinfos[i];
         if (b->sin_family == AF_INET) {
            a->ai_addr = (struct sockaddr*)b;
            a->ai_addrlen = sizeof(struct sockaddr_in);
         } else
            a->ai_canonname = (char*)&b->sin_addr;
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
      h2_addr* addr = I().find(name);
      if (!addr) return nullptr;

      static char* h_aliases[32];
      static char* h_addr_list[32];
      static struct hostent h;

      h.h_name = name;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      memset(h_aliases, 0, sizeof(h_aliases));
      memset(h_addr_list, 0, sizeof(h_addr_list));

      for (int i = 0, a = 0, c = 0; i < addr->count; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&addr->array[i];
         if (b->sin_family == AF_INET)
            h_addr_list[a++] = (char*)&b->sin_addr;
         else
            h_aliases[c++] = (char*)&b->sin_addr;
      }
      return &h;
   }

   /* clang-format off */
   static h2_dns& I() { static h2_dns __; return __; }
   /* clang-format on */

   h2_stub getaddrinfo_stub;
   h2_stub freeaddrinfo_stub;
   h2_stub gethostbyname_stub;

   h2_dns() : getaddrinfo_stub((void*)::getaddrinfo), freeaddrinfo_stub((void*)::freeaddrinfo), gethostbyname_stub((void*)::gethostbyname) {}

   void init() {
      getaddrinfo_stub.replace((void*)getaddrinfo);
      freeaddrinfo_stub.replace((void*)freeaddrinfo);
      gethostbyname_stub.replace((void*)gethostbyname);
   }

   void exit() {
      getaddrinfo_stub.restore();
      freeaddrinfo_stub.restore();
      gethostbyname_stub.restore();
   }

   static void setaddrinfo(const char* hostname, int count, ...) {
      va_list a;
      va_start(a, count);
      h2_addr* addr = new h2_addr(hostname, count, a);
      I().addrs.push(&addr->y);
      h2_addr_g(addr);
      va_end(a);
   }
};
