#include "../source/h2_unit.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>

H2UNIT (h2_leak) {
   void setup() {}

   void teardown() {}
};

H2CASE(h2_leak, malloc / free)
{
   h2_alloc alloc;
   char* p = (char*)alloc.malloc(100);
   H2EQ(p != NULL);
   strcpy(p, "Hello World brk mmap");
   alloc.free(p);
}

H2CASE(h2_leak, mm malloc / free)
{
   h2_backtrace bt;
   h2_mm* mm = h2_mm::allocate(100, 0, bt);
   void* mm_same = h2_mm::container_of(mm->ptr);
   H2EQ(mm == mm_same);
   h2_mm::release(mm);
}

H2CASE(h2_leak, monitered malloc / free)
{
   free(malloc(8));
   free(calloc(10, 10));
   free(realloc(malloc(10), 100));
   void* p = NULL;
   int ret = posix_memalign(&p, 32, 1024);
   H2EQ(0, ret);
   H2EQ(NULL != p);
   free(p);
   free(strdup("hello leak"));
   free(strndup("hello leak", 8));
}

H2CASE(h2_leak, sys calls)
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
   time_t t3 = time(NULL);
   localtime(&t3);
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
