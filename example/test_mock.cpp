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

H2UNIT (Mock_Function) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(Mock_Function, once)
{
   H2MOCK(foobar, int(int, const char*)).once(1, "A").returns(11);
   H2EQ(11, foobar(2, "B"));
}

H2CASE(Mock_Function, twice)
{
   H2MOCK(foobar, int(int, const char*)).twice(Eq(1), _).returns(11);
   H2EQ(11, foobar(1, "A"));
   H2EQ(22, foobar(1, "BC"));
}

H2CASE(Mock_Function, 3 times)
{
   H2MOCK(foobar, int(int, const char*)).times(3).with(Ge(1)).returns(11);
   H2EQ(11, foobar(1, "A"));

   H2MOCK(time, time_t(time_t*)).once();
}

H2CASE(Mock_Function, any 0)
{
   H2MOCK(foobar, int(int, const char*)).any(1, "A");
}

H2CASE(Mock_Function, any 1)
{
   H2MOCK(foobar, int(int, const char*)).any().with(1, "A").returns(11);
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, any 2)
{
   H2MOCK(foobar, int(int, const char*)).any().with(1, "A").returns(11);
   H2EQ(11, foobar(1, "A"));
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, atleast 2)
{
   H2MOCK(foobar, int(int, const char*)).atleast(2).with(1, "A").returns(11);
   H2EQ(11, foobar(1, "A"));
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, atleast 3)
{
   H2MOCK(foobar, int(int, const char*)).atleast(2).with(1, "A").returns(11);
   H2EQ(11, foobar(1, "A"));
   H2EQ(11, foobar(1, "A"));
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, atmost 1)
{
   H2MOCK(foobar, int(int, const char*)).atmost(2).with(1, "A").returns(11);
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, atmost 2)
{
   H2MOCK(foobar, int(int, const char*)).atmost(2).with(1, "A").returns(11);
   H2EQ(11, foobar(1, "A"));
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, between)
{
   H2MOCK(foobar, int(int, const char*)).between(2, 4).with(1, "A").returns(11);
   H2EQ(11, foobar(1, "A"));
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, multi - line)
{
   H2MOCK(foobar, int(int, const char*)).once(1, "A").returns(11).once().with(2, "B").returns(22).twice(3, "C").returns(33);

   H2EQ(11, foobar(1, "A"));
   H2EQ(22, foobar(2, "B"));
   H2EQ(33, foobar(3, "C"));
   H2EQ(33, foobar(3, "C"));
}

H2CASE(Mock_Function, th0)
{
   H2MOCK(foobar, int(int, const char*)).once().th0(1);
   foobar(1, "A");
}

H2CASE(Mock_Function, th1)
{
   H2MOCK(foobar, int(int, const char*)).once().th1("A");
   foobar(1, "A");
}

H2CASE(Mock_Function, Null Matcher)
{
   H2MOCK(foobar, int(int, const char*)).once(1, Null()).returns(11);
   H2EQ(11, foobar(1, NULL));
}

H2CASE(Mock_Function, HasSubstr Matcher)
{
   H2MOCK(foobar, int(int, const char*)).once(1, HasSubstr("BC")).returns(11);
   H2EQ(11, foobar(1, "ABCD"));
}

H2CASE(Mock_Function, Json Matcher)
{
   H2MOCK(foobar, int(int, const char*)).once(1, Je("{}")).returns(11);
   H2EQ(11, foobar(1, "[]"));
}

H2CASE(Mock_Function, Not Matcher)
{
   H2MOCK(foobar, int(int, const char*)).once(Not(2), _).returns(11);
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, AllOf Matcher)
{
   H2MOCK(foobar, int(int, const char*)).once(AllOf(1, Ge(1)), _).returns(11);
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, AnyOf Matcher)
{
   H2MOCK(foobar, int(int, const char*)).once(AnyOf(Le(1), Gt(2)), _).returns(11);
   H2EQ(11, foobar(1, "A"));
}

H2CASE(Mock_Function, NoneOf Matcher)
{
   H2MOCK(foobar, int(int, const char*)).once(NoneOf(1, Ge(1)), _).returns(11);
   H2EQ(11, foobar(0, "A"));
}

H2UNIT (Mock_Member) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(Mock_Member, static member function)
{
   H2MOCK(Animal::born, int(int)).once(1).returns(11);
   H2EQ(11, Animal::born(1));
}

H2CASE(Mock_Member, member function)
{
   H2MOCK(Dog, go, int(int, int)).once(1, 2).returns(11);
   H2MOCK(Dog, go, int(int)).once(1).returns(11);
   Dog dog(1);
   H2EQ(11, dog.go(1, 2));
   H2EQ(11, dog.go(1));
}

H2CASE(Mock_Member, member function lambda)
{
   H2MOCK(Dog, go, int(int, int)).once(1, 2) = [](int x, int y) { return x + y + 1; };
   H2MOCK(Dog, go, int(int)).once() = [](Dog* dog, int xy) { H2EQ(1, dog->age); return xy + 1; };
   Dog dog(1);
   H2EQ(4, dog.go(1, 2));
   H2EQ(3, dog.go(1)); /* Failed here */
}

H2CASE(Mock_Member, virtual member function)
{
   H2MOCK(Dog, say, const char*()).once().returns("www");
   Dog dog(1);
   H2EQ("www", dog.say());
   H2MOCK(Cat, say, const char*()).once() = []() { return "mmm..."; };
   Cat cat(nullptr, nullptr);
   H2EQ("mmm...", cat.say());

   H2MOCK(Vivipara, cry, int()).once() = []() { return 5; };
   H2EQ(5, dog.cry());

   H2MOCK(Ovipara, cry, int(), Bird()).once() = []() { return 6; };
   Bird bird;
   H2EQ(6, bird.cry());
}

H2CASE(Mock_Member, virtual member function failed)
{
   H2MOCK(Centipede, say, const char*()).once() = []() { return "..."; }; /* Failed here */
   Centipede centipede(1, 2, 3, 4, 5, 6, 7, 8, 9);
   H2EQ("...", centipede.say());
}

H2CASE(Mock_Member, abstract class)
{
   Bird bird;
   H2MOCK(Ovipara, cry, int(), Bird()).once() = []() { return 6; };
   H2EQ(6, bird.cry());

   H2EQ("jiji", bird.say());
   H2MOCK(Ovipara, say, const char*(), Bird()).once() = []() { return "j..."; };
   H2EQ("j...", bird.say());
}

H2CASE(Mock_Member, abstract class failed)
{
   H2MOCK(Ovipara, cry, int()).once() = []() { return 6; }; /* Failed here */
}
