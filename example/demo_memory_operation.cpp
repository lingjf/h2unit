#include <stdio.h>
#include <string.h>

void allocation()
{
   char *p1 = (char*) malloc(3);
   sprintf(p1, "ab");
   char *p2 = (char*) malloc(9);
   sprintf(p2, "cd");
   free(p1);

   char *p3 = (char*) calloc(1, 3);
   sprintf(p3, "ab");
   char *p4 = (char*) calloc(1, 9);
   sprintf(p4, "cd");
   free(p3);

   char *p5 = (char*) malloc(3);
   sprintf(p5, "ab");
   char *p6 = (char*) realloc(p5, 9);
   sprintf(p6, "cd");

   char *m1 = NULL;
   posix_memalign((void**)&m1, 4, 7);
   sprintf(m1, "xyz");

   char *m2 = (char*)memalign(4, 7);
   sprintf(m2, "xyz123");

   char *p7 = strdup("abc");
   sprintf(p7, "ab");
   char *p8 = strndup("abcd", 3);
   sprintf(p8, "cd");
   free(p7);

   int *p9 = new int(2);
   *p9 = 3;
   int *p0 = new int[2];
   *p0 = 3;

   delete p0;
}
