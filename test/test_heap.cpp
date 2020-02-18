#include "../source/h2_unit.h"

using namespace h2;

class C7
{
 public:
   int a;
   double b;
   char c[100];

   C7(){};
};

CASE(malloc / free)
{
   char* p = (char*)h2_raw::malloc(100);
   OK(p != NULL);
   strcpy(p, "Hello World brk mmap");
   h2_raw::free(p);
}

CASE(piece malloc / free)
{
   h2_backtrace bt;
   h2_piece* m = h2_piece::allocate(100, 0, bt);
   h2_piece::release(m);
}

CASE(monitered malloc / free)
{
   free(calloc(10, 10));
   free(realloc(malloc(10), 100));
   void* p = NULL;
   int ret = posix_memalign(&p, 32, 1024);
   OK(0, ret);
   OK(NULL != p);
   free(p);
   free(strdup("hello leak"));
   free(strndup("hello leak", 8));
}

CASE(C++ new and delete)
{
   C7* c = new C7();
   delete c;
}

CASE(test memory leak block)
{
   void *c1, *c2, *c3, *c4;

   c1 = malloc(1);

   BLOCK()
   {
      c2 = malloc(3);
      free(c2);
   }

   BLOCK()
   {
      c3 = malloc(5);
      BLOCK()
      {
         c4 = malloc(7);
         free(c4);
      }
      free(c3);
   }

   free(c1);
}

CASE(sys calls)
{
   char t[1024];
   sprintf(t, "%g%g%g", 1.0 / 3.0, 1.0 / 7.0, 1.0 / 13.0);
   srand(0);
   rand();
   close(socket(AF_INET, SOCK_DGRAM, 0));
   fclose(fopen("/dev/null", "r"));
   struct timeval tv;
   struct timezone tz;
   gettimeofday(&tv, &tz);
   time(NULL);
   // localtime(&t3);

   strerror(ENOMEM);
   double t4 = sqrt(42.0);
   sprintf(t, "%g", t4);

   struct addrinfo hints, *res;
   memset(&hints, 0, sizeof(hints));
   hints.ai_family = PF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   getaddrinfo("localhost", NULL, &hints, &res);
   freeaddrinfo(res);
}

SUITE(leak)
{
   void* p = NULL;

   Setup()
   {
      p = malloc(32 * 864);
   };

   Teardown()
   {
      free(p);
   };

   Case(malloc free in setup and teardown)
   {
   };
}
