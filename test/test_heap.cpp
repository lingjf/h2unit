#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

class C7
{
 public:
   int a;
   double b;
   char c[100];

   C7(){};
};

CASE(piece malloc / free)
{
   h2::h2_backtrace bt;
   h2::h2_piece* m = new h2::h2_piece(100, 0, bt);
   delete m;
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

SUITE(BLOCK)
{
   void *c1, *c2, *c3, *c4;

   Case(block)
   {
      c1 = malloc(1);
      BLOCK()
      {
         c2 = malloc(3);
         free(c2);
      }
      free(c1);
   }

   Case(nested)
   {
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
   }

   Case(limited)
   {
      BLOCK(10)
      {
         c2 = malloc(8);
         OK(NotNull, c2);
         c3 = malloc(8);
         OK(IsNull, c3);
         free(c2);
         c4 = malloc(8);
         OK(NotNull, c4);
         free(c4);
      }
   }
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
   void* p = malloc(32 * 864);

   Cleanup()
   {
      free(p);
   }

   Case(malloc in setup and free in teardown)
   {
      strcpy((char*)p, "hello world");
      OK("hello world", (char*)p);
   };
}
