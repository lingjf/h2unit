#include "h2unit.h"

extern "C" {
#include "product_c.h"
}

H2UNIT (Logic) {
};

H2CASE(Logic, Not)
{
   H2EQ(Not(5), getEven(7));
   H2EQ(Not(6), getEven(7));
}

H2CASE(Logic, Not string equal)
{
   H2EQ(Not("a7af3452a5eee24a149f8993e48ce81b77b200b212b23c2402c221ddc4260a608cf328a2133386477eb7a65958893726741cb9223419a7a0224fe7af2866944197ee4ab18e25913cae8507bb306a284f"),
        "a7af3452a5eee24a149f8993e48ce81b77b200b212b23c2402c221ddc4260a608cf328a2133386477eb7a65958893726741cb9223419a7a0224fe7af2866944197ee4ab18e25913cae8507bb306a284f");
}

H2CASE(Logic, Not Not)
{
   H2EQ(Not(Not(5)), getEven(7));
}

H2CASE(Logic, AllOf)
{
   Point p1 = {0, 0};
   H2EQ(AllOf("Point(0, 0)", StartsWith("Point")), Point_toString(&p1));

   Point p2 = {1, 2};
   H2EQ(AllOf("Point(0, 0)", EndsWith("P(1, 2)")), Point_toString(&p2));
}

H2CASE(Logic, AnyOf)
{
   Point p1 = {0, 0};
   H2EQ(AnyOf("Point(0, 0)", StartsWith("Point")), Point_toString(&p1));
   H2EQ(AnyOf("Point(0, 1)", StartsWith("Point")), Point_toString(&p1));

   Point p2 = {1, 2};
   H2EQ(AnyOf("Point(0, 0)", EndsWith("P(1, 2)")), Point_toString(&p2));
}

H2CASE(Logic, NoneOf)
{
   Point p1 = {0, 0};
   H2EQ(NoneOf("Point(0, 1)", EndsWith("P(1, 2)")), Point_toString(&p1));

   H2EQ(NoneOf("Point(0, 1)", StartsWith("Point")), Point_toString(&p1));
}
