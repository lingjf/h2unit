#ifndef _DEMO_DYNAMIC_STUB_HPP_
#define _DEMO_DYNAMIC_STUB_HPP_

class Shape {
public:
   Shape();
   ~Shape();
   virtual int getEdge();
};

class Rect : public Shape {
   int m_a, m_b, m_x, my;
public:
   Rect();
   Rect(int a, int b, int x, int y);
   ~Rect();
   int getCode();
   int getPage(int v);
   int getEdge();
   char* tuString();
};

class Triangle : public Shape {
public:
   Triangle();
   ~Triangle();
   int getEdge();
};



#endif
