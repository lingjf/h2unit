#include "h2unit.h"
#include <stdlib.h>

extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

int foobar(int a, const char* b)
{
   return 0;
}

SUITE(Mock Function)
{
   Setup(){};

   Teardown(){};

   Case(once-- failure)
   {
      MOCK(foobar, int(int, const char*)).once(1, "A").returns(11);
      OK(11, foobar(2, "B"));
   };

   Case(twice-- failure)
   {
      MOCK(foobar, int(int, const char*)).twice(Eq(1), _).returns(11);
      OK(11, foobar(1, "A"));
      OK(22, foobar(1, "BC"));  //failure
   };

   Case(3 times-- failure)
   {
      MOCK(foobar, int(int, const char*)).times(3).with(Ge(1)).returns(11);
      OK(11, foobar(1, "A"));

      MOCK(time, time_t(time_t*)).once();
   };

   Case(any 0 --successful)
   {
      MOCK(foobar, int(int, const char*)).any(1, "A");
   };

   Case(any 1 --successful)
   {
      MOCK(foobar, int(int, const char*)).any().with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
   };

   Case(any 2 --successful)
   {
      MOCK(foobar, int(int, const char*)).any().with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   };

   Case(atleast 2 --successful)
   {
      MOCK(foobar, int(int, const char*)).atleast(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   };

   Case(atleast 3 --successful)
   {
      MOCK(foobar, int(int, const char*)).atleast(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   };

   Case(atmost 1 --successful)
   {
      MOCK(foobar, int(int, const char*)).atmost(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
   };

   Case(atmost 2 --successful)
   {
      MOCK(foobar, int(int, const char*)).atmost(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   };

   Case(between-- successful)
   {
      MOCK(foobar, int(int, const char*)).between(2, 4).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   };

   Case(multi - line-- successful)
   {
      MOCK(foobar, int(int, const char*)).once(1, "A").returns(11).once().with(2, "B").returns(22).twice(3, "C").returns(33);

      OK(11, foobar(1, "A"));
      OK(22, foobar(2, "B"));
      OK(33, foobar(3, "C"));
      OK(33, foobar(3, "C"));
   };

   Case(th1-- successful)
   {
      MOCK(foobar, int(int, const char*)).once().th1(1);
      foobar(1, "A");
   };

   Case(th2-- successful)
   {
      MOCK(foobar, int(int, const char*)).once().th2("A");
      foobar(1, "A");
   };

   Case(Null Matcher-- successful)
   {
      MOCK(foobar, int(int, const char*)).once(1, Null()).returns(11);
      OK(11, foobar(1, NULL));
   };

   Case(Contains Matcher-- successful)
   {
      MOCK(foobar, int(int, const char*)).once(1, Contains("BC")).returns(11);
      OK(11, foobar(1, "ABCD"));
   };

   Case(Json Matcher-- failure)
   {
      MOCK(foobar, int(int, const char*)).once(1, Je("{}")).returns(11);
      OK(11, foobar(1, "[]"));
   };

   Case(Not Matcher-- successful)
   {
      MOCK(foobar, int(int, const char*)).once(Not(2), _).returns(11);
      OK(11, foobar(1, "A"));
   };

   Case(AllOf Matcher-- successful)
   {
      MOCK(foobar, int(int, const char*)).once(AllOf(1, Ge(1)), _).returns(11);
      OK(11, foobar(1, "A"));
   };

   Case(AnyOf Matcher-- successful)
   {
      MOCK(foobar, int(int, const char*)).once(AnyOf(Le(1), Gt(2)), _).returns(11);
      OK(11, foobar(1, "A"));
   };

   Case(NoneOf Matcher-- successful)
   {
      MOCK(foobar, int(int, const char*)).once(NoneOf(1, Ge(1)), _).returns(11);
      OK(11, foobar(0, "A"));
   };
}

SUITE(Mock Member)
{
   Case(static member function-- successful)
   {
      MOCK(Animal::born, int(int)).once(1).returns(11);
      OK(11, Animal::born(1));
   };

   Case(member function-- successful)
   {
      MOCK(Dog, go, int(int, int)).once(1, 2).returns(11);
      MOCK(Dog, go, int(int)).once(1).returns(11);
      Dog dog(1);
      OK(11, dog.go(1, 2));
      OK(11, dog.go(1));
   };

   Case(member function lambda-- failure)
   {
      MOCK(Dog, go, int(int, int)).once(1, 2) = [](int x, int y) { return x + y + 1; };
      MOCK(Dog, go, int(int)).once() = [](Dog* dog, int xy) { OK(1, dog->age); return xy + 1; };
      Dog dog(1);
      OK(4, dog.go(1, 2));
      OK(3, dog.go(1));  //failure
   };

   Case(virtual member function-- successful)
   {
      MOCK(Dog, say, const char*()).once().returns("www");
      Dog dog(1);
      OK("www", dog.say());
      MOCK(Cat, say, const char*()).once() = []() { return "mmm..."; };
      Cat cat(nullptr, nullptr);
      OK("mmm...", cat.say());

      MOCK(Vivipara, cry, int()).once() = []() { return 5; };
      OK(5, dog.cry());

      MOCK(Ovipara, cry, int(), Bird()).once() = []() { return 6; };
      Bird bird;
      OK(6, bird.cry());
   };

   Case(virtual member function-- failure)
   {
      MOCK(Centipede, say, const char*()).once() = []() { return "..."; };  //failure
      Centipede centipede(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK("...", centipede.say());
   };

   Case(abstract class --successful)
   {
      Bird bird;
      MOCK(Ovipara, cry, int(), Bird()).once() = []() { return 6; };
      OK(6, bird.cry());

      OK("jiji", bird.say());
      MOCK(Ovipara, say, const char*(), Bird()).once() = []() { return "j..."; };
      OK("j...", bird.say());
   };

   Case(abstract class --failure)
   {
      MOCK(Ovipara, cry, int()).once() = []() { return 6; };  //failure
   };
}