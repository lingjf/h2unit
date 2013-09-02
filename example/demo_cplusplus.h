#ifndef _DEMO_CPLUSPLUS_H_
#define _DEMO_CPLUSPLUS_H_

////////////////// Dynamic Stub

int getResult(int a, double b);

class Shape {
public:
   Shape();
   virtual ~Shape();
   virtual double getArea();
};

class Rect : public Shape {
private:
   int m_width, m_height;
public:
   Rect();
   Rect(int width, int height);
   virtual ~Rect();
   int getEdge();
   virtual double getArea();
};

class Circle : public Shape {
private:
   int m_radius;
public:
   Circle(int radius);
   virtual ~Circle();
   virtual double getArea();
   void enLarge(int d);
   char* toString();
};

////////////////// Exception

class Parser {
public:
   Parser();
   int fromInt(const char* str);
};


////////////////// Memory

void allocate_lots_of_thing();



#endif
