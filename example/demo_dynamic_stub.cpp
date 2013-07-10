#include "demo_dynamic_stub.hpp"


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

int Rect::getCode()
{
   return 0;
}

int Rect::getPage(int v)
{
   return v;
}

int Rect::getEdge()
{
   return 4;
}

char* Rect::tuString()
{
   static char buffer[1024];
   sprintf(buffer, "1");
   return buffer;
}


Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

int Triangle::getEdge()
{
   return 3;
}
