#ifndef _PRODUCT_CPP_H_
#define _PRODUCT_CPP_H_

class Shape {
 public:
   int x, y;
   const char* name;

 public:
   Shape(int _x, int _y, const char* _name) : x(_x), y(_y), name(_name) {}
   virtual ~Shape() {}

   static int born(int a)
   {
      return 0;
   }

   int move(int x, int y)
   {
      this->x += x;
      this->y += y;
      return 0;
   }

   int move(int xy);

   virtual const char* print() = 0;
};

class Rect : public Shape {
 public:
   int width, height;

 public:
   Rect(int left, int top, int _width, int _height) : Shape(left, top, "Rect"), width(_width), height(_height)
   {
   }

   void scale(double x)
   {
      width = (int)(width * x);
      height = (int)(height * x);
   }

   const char* print()
   {
      return "Rect";
   }
};

#endif
