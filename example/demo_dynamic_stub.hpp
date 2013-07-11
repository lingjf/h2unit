
#ifndef _DEMO_DYNAMIC_STUB_HPP_
#define _DEMO_DYNAMIC_STUB_HPP_

int orig_dog(int a, double b);

class Shape {
public:
   Shape();
   virtual ~Shape();
   virtual int getEdge();
};

class Rect : public Shape {
private:
   int m_a, m_b, m_x, m_y;
public:
   Rect();
   Rect(int a, int b, int x, int y);
   virtual ~Rect();
   int getPage(int v);
   virtual int getEdge();
};

class Circle : public Shape {
private:
   int m_r;
public:
   Circle(int r);
   virtual ~Circle();
   virtual int getEdge();
   void enlarge(int d);
   char* tuString();
};



#endif
