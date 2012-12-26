#include "product_2.h"

#include <stdio.h>
#include <string.h>

wstring::wstring()
{
   len = 0;
   cap = 256;
   dat = (unsigned short*) malloc(sizeof(unsigned short) * cap);
}

wstring::~wstring()
{
   free(dat);
}

void wstring::fromInt(int value)
{
   if (value < 0) {
      throw 2;
      return;
   }
   char buf[64];
   char *p = &buf[64];
   *(--p) = '\0';
   do {
      int b = value % 10;
      value = value / 10;
      *(--p) = (char) ('0' + b);
   } while (value > 0);

   unsigned char *q = (unsigned char*) dat;
   while (*p) {
      q[2 * len] = (unsigned char) *p;
      q[2 * len + 1] = 0;
      len++;
      p++;
   }
}

unsigned char* wstring::getData()
{
   return (unsigned char*) dat;
}

char* wstring::tuString()
{
   static char buffer[1024];
   char *p = buffer;
   p += sprintf(p, "wstring(len=%d,cap=%d,dat=", len, cap);
   unsigned char *q = (unsigned char*) dat;
   for (int i = 0; i < len * 2; i++) {
      char c = '_';
      if (q[i] != '\0') c = (char) q[i];
      p += sprintf(p, "%c", c);
   }
   p += sprintf(p, ")");
   return buffer;
}

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

   char *p7 = strdup("abc");
   sprintf(p7, "ab");
   char *p8 = strndup("abcd", 3);
   sprintf(p8, "cd");
   free(p7);

   int *p9 = new int(2);
   *p9 = 3;
   int *p0 = new int[2];
   *p0 = 3;
   wstring *w1 = new wstring();
   w1->fromInt(0);
   wstring *w2 = new wstring();
   w2->fromInt(0);
   delete w1;
   delete p0;
}

