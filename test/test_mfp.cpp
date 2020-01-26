#include "../source/h2_unit.h"

SUITE(mfp)
{
   Setup(){};

   Teardown(){};

   class Shape
   {
    public:
      int x, y;

      Shape()
        : x(0), y(0) {}

      static int fly(int x, int y)
      {
         return 0;
      }

      int go(int x, int y)
      {
         return 1;
      }

      int go(int xy)
      {
         return 2;
      }

      virtual int work(int x, int y)
      {
         return 3;
      }
   };

   class Rect : public Shape
   {
    public:
      Rect(int _x, double _y) {}
      virtual int work(int x, int y)
      {
         return 4;
      }
   };

   class Circle : public Shape
   {
    public:
      Circle(char* _x) {}
      virtual int work(int x, int y)
      {
         return 5;
      }
   };

   class Six : public Rect
   {
    public:
      Six(int _1, int _2, int _3, int _4, int _5, int _6)
        : Rect(0, 0) {}
      virtual int work(int x, int y)
      {
         return 6;
      }
   };

   Case(normal member function)
   {
      void* p1 = h2_mfp<Shape, int(int, int)>::A(&Shape::go, "H2MOCK", "", "Shape", "go", "int(int,int)", __FILE__, __LINE__);
      typedef int (*Shape_Go1)(int, int);
      OK(1, ((Shape_Go1)p1)(1, 1));

      void* p2 = h2_mfp<Shape, int(int)>::A(&Shape::go, "H2MOCK", "", "Shape", "go", "int(int,int)", __FILE__, __LINE__);
      typedef int (*Shape_Go2)(int);
      OK(2, ((Shape_Go2)p2)(1));

      void* p3 = h2_mfp<Rect, int(int, int)>::A(&Rect::go, "H2MOCK", "", "Rect", "go", "int(int,int)", __FILE__, __LINE__);
      typedef int (*Rect_Go1)(int, int);
      OK(1, ((Rect_Go1)p3)(1, 1));

      void* p4 = h2_mfp<Circle, int(int)>::A(&Circle::go, "H2MOCK", "", "Circle", "go", "int(int,int)", __FILE__, __LINE__);
      typedef int (*Circle_Go2)(int);
      OK(2, ((Circle_Go2)p4)(1));
   };

   Case(virtual member function)
   {
      void* p1 = h2_mfp<Shape, int(int, int)>::A(&Shape::work, "H2MOCK", "", "Shape", "work", "int(int,int)", __FILE__, __LINE__);
      typedef int (*Shape_Work)(int, int);
      OK(3, ((Shape_Work)p1)(1, 1));

      void* p2 = h2_mfp<Rect, int(int, int)>::A(&Rect::work, "H2MOCK", "", "Rect", "work", "int(int,int)", __FILE__, __LINE__);
      typedef int (*Rect_Work)(int, int);
      OK(4, ((Rect_Work)p2)(1, 1));

      void* p3 = h2_mfp<Circle, int(int, int)>::A(&Circle::work, "H2MOCK", "", "Shape", "work", "int(int,int)", __FILE__, __LINE__);
      typedef int (*Circle_Work)(int, int);
      OK(5, ((Circle_Work)p3)(1, 1));

      void* p4 = h2_mfp<Six, int(int, int)>::A(&Six::work, Six(1, 2, 3, 4, 5, 6));
      typedef int (*Six_Work)(int, int);
      OK(6, ((Six_Work)p4)(1, 1));
   };
}