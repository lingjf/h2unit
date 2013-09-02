
#include "demo_cplusplus.h"

#include <cstdio>



int getResult(int a, double b)
{
   return a + (int) b;
}

Shape::Shape()
{
}

Shape::~Shape()
{
}

double Shape::getArea()
{
   return 0;
}


Rect::Rect()
{
   m_width = 0;
   m_height = 0;
}

Rect::Rect(int width, int height)
{
   m_width = width;
   m_height = height;
}

Rect::~Rect()
{
}

int Rect::getEdge()
{
   return 0;
}

double Rect::getArea()
{
   return m_width * m_height;
}

Circle::Circle(int radius)
{
   m_radius = radius;
}

Circle::~Circle()
{
}

double Circle::getArea()
{
   /* not implemented yet */
   return -1;
}

void Circle::enLarge(int d)
{
   m_radius += d;
}

char* Circle::toString()
{
   static char buffer[1024];
   sprintf(buffer, "Circle(%d)", m_radius);
   return buffer;
}


Parser::Parser()
{
}

int Parser::fromInt(const char* str)
{
   int s = 0;
   for(char* p = (char*)str; p && *p; p++) {
      if (*p == '-') {
         throw 1;
      }
      if (*p < '0' || *str > '9') {
         throw 2;
      }

      s = s * 10 + *p - '0';
   }
   return s;
}



void allocate_lots_of_thing()
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

