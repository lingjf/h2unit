
struct h2_extra {
   static struct sockaddr_storage* get_sockaddrs() {
      static struct sockaddr_storage sockaddrs[100] = {{0}};
      return sockaddrs;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res) {
      static struct addrinfo addrinfos[100];
      struct sockaddr_storage* sockaddrs = get_sockaddrs();

      struct addrinfo** pp = res;

      for (int i = 0; i < 100; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
         if (0 == b->sin_addr.s_addr) {
            break;
         }

         struct addrinfo* a = &addrinfos[i];
         a->ai_addr = (struct sockaddr*)&sockaddrs[i];
         a->ai_addrlen = sizeof(struct sockaddr_in);
         a->ai_family = AF_INET;
         a->ai_socktype = SOCK_STREAM;
         a->ai_protocol = IPPROTO_TCP;
         a->ai_canonname = nullptr;
         a->ai_next = nullptr;
         if (hints) {
            a->ai_family = hints->ai_family;
            a->ai_socktype = hints->ai_socktype;
            a->ai_protocol = hints->ai_protocol;
         }
         *pp = a;
         pp = &a->ai_next;
      }
      *pp = nullptr;
      return 0;
   }

   static void freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* gethostbyname(const char* name) {
      static char* h_aliases[1] = {0};
      static char* h_addr_list[100];
      static struct hostent h;
      h.h_name = (char*)name;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      struct sockaddr_storage* sockaddrs = get_sockaddrs();
      for (int i = 0; i < 100; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
         if (0 == b->sin_addr.s_addr) break;
         h_addr_list[i] = (char*)&b->sin_addr.s_addr;
      }

      return &h;
   }

   /* clang-format off */
   static h2_extra& I() { static h2_extra I; return I; }
   /* clang-format on */

   h2_extra() : getaddrinfo_stub((void*)::getaddrinfo), freeaddrinfo_stub((void*)::freeaddrinfo) {}

   h2_stub getaddrinfo_stub;
   h2_stub freeaddrinfo_stub;

   void dohook() {
      getaddrinfo_stub.replace((void*)h2_extra::getaddrinfo);
      freeaddrinfo_stub.replace((void*)h2_extra::freeaddrinfo);
   }

   void unhook() {
      getaddrinfo_stub.restore();
      freeaddrinfo_stub.restore();
   }
};

static inline void addrinfos(int count, ...) {
   struct sockaddr_storage* sockaddrs = h2_extra::get_sockaddrs();

   int i = 0;
   va_list a;
   va_start(a, count);
   for (; i < count; ++i) {
      const char* p = va_arg(a, const char*);
      struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
      memset(b, 0, sizeof(struct sockaddr_in));
      b->sin_family = AF_INET;
      b->sin_addr.s_addr = inet_addr(p);
   }
   va_end(a);

   memset(&sockaddrs[i], 0, sizeof(struct sockaddr));
}

#define H2DNS(...) h2::addrinfos(H2PP_NARGS(__VA_ARGS__), __VA_ARGS__)
