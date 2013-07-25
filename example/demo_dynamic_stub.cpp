
#include "demo_dynamic_stub.hpp"

#include <stdio.h>

int orig_dog(int a, double b)
{
   return a + (int) b;
}

Shape::Shape()
{
}

Shape::~Shape()
{
}

int Shape::getEdge()
{
   return 0;
}


Rect::Rect()
{
}

Rect::Rect(int a, int b, int x, int y)
{
}

Rect::~Rect()
{
}

int Rect::getPage(int v)
{
   return v;
}

int Rect::getEdge()
{
   return 4;
}

Circle::Circle(int r)
{
   m_r = r;
}

Circle::~Circle()
{
}

int Circle::getEdge()
{
   return 1;
}

void Circle::enlarge(int d)
{
   m_r += d;
}

char* Circle::tuString()
{
   static char buffer[1024];
   sprintf(buffer, "Circle(%d)", m_r);
   return buffer;
}
