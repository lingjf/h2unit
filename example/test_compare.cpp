#include "h2unit.h"
#include <stdlib.h>

extern "C" {
#include "product_c.h"
}

H2UNIT (getEven) {
};

H2CASE(getEven, "getEvent with odd value")
{
   H2EQ(4, getEven(7));
}

H2CASE(getEven, "getEven with even value")
{
   H2EQ(6, getEven(6));
}

H2UNIT (getCeil) {
   /*
    * setup() and teardown() can be omitted. The default is doing nothing
    */
};

H2CASE(getCeil, "test for float type number")
{
   H2EQ(6.0, getCeil(6.2));
   H2EQ(6.0, getCeil(6.4));
   H2EQ(3.14159, 2.34159);
}

H2CASE(getCeil, "test for great than")
{
   H2EQ(Gt(3.15), 3.14159);
}

H2UNIT (Point) {
};

/*
 * h2unit H2EQ can be used to verify Case sensitive String.
 */
H2CASE(Point, string equal)
{
   Point p1 = {0, 0};
   H2EQ("Point(0, 0)", Point_toString(&p1));
   Point p2 = {1, 2};
   H2EQ("Point(0, 0)", Point_toString(&p2));
}

H2CASE(Point, demo HasSubstr)
{
   Point p1 = {0, 0};
   H2EQ(HasSubstr("(0, 0)"), Point_toString(&p1));
   Point p2 = {1, 2};
   H2EQ(HasSubstr("(1, 1)"), Point_toString(&p2));
}

H2CASE(Point, demo StartsWith)
{
   Point p1 = {0, 0};
   H2EQ(StartsWith("Point"), Point_toString(&p1));
   H2EQ(StartsWith("Boint"), Point_toString(&p1));
}

H2CASE(Point, demo EndsWith)
{
   Point p1 = {0, 0};
   H2EQ(EndsWith("(0, 0)"), Point_toString(&p1));
   H2EQ(EndsWith("(1, 1)"), Point_toString(&p1));
}

/*
 * h2unit WILDCARD/We can be used to verify string by WildCard. support:
 *  -- ? any one char
 *  -- * any char(s)
 */
H2CASE(Point, wildcard string)
{
   Point p1 = {0, 0};
   H2EQ(We("Point(0, ?)"), Point_toString(&p1));
   Point p2 = {1, 2};
   H2EQ(We("Point(*]"), Point_toString(&p2));
}

/*
 * h2unit REGEX/Re can be used to verify string by Regular express.
 * http://www.cplusplus.com/reference/regex/ECMAScript/
 */
H2CASE(Point, regex string)
{
   Point p1 = {0, 0};
   H2EQ(Re("Point\\(0, 0\\)"), Point_toString(&p1));
   Point p2 = {1, 2};
   H2EQ(Re("Point\\(."), Point_toString(&p2));
   Point p3 = {3, 4};
   H2EQ(Re("Point\\(.*\\)"), Point_toString(&p3));
   Point p4 = {5, 6};
   H2EQ(Re("^Point\\(.*\\)$"), Point_toString(&p4));
   Point p5 = {7, 8};
   H2EQ(Re("Point\\([789], [0-9]\\)"), Point_toString(&p5));
   Point p6 = {11, 88};
   H2EQ(Re("Point\\([0-9]*, [^0-5]*\\)"), Point_toString(&p6));
   Point p7 = {11, 88};
   H2EQ(Re("Point\\([0-9]+, [0-9]+\\)"), Point_toString(&p7));
}

/*
 * Case-Less string compare
 */

H2CASE(Point, caseless string equal)
{
   Point p1 = {0, 0};
   H2EQ(CaseLess("point(0, 0)"), Point_toString(&p1));
   Point p2 = {1, 2};
   H2EQ(CaseLess("POINT(1, 1)"), Point_toString(&p2));
}

H2CASE(Point, caseless starts / end with)
{
   Point p1 = {0, 0};
   H2EQ(CaseLess(StartsWith("point(0, 0)")), Point_toString(&p1));
   Point p2 = {1, 2};
   H2EQ(CaseLess(EndsWith("P(1, 2)")), Point_toString(&p2));
}

H2UNIT_CASE(string equal)
{
   H2EQ("a7af3452a5eee24a149f8993e48ce81b77b200b212b23c2402c221ddc4260a608cf328a2133386477eb7a65958893726741cb9223419a7a0224fe7af2866944197ee4ab18e25913cae8507bb306a284f",
        "a7af3452a5eeX24a149f8993e48ce81b77b200b212b23c2402c2Y1ddc4260a608cf328a2133386477eb7a6595889372674Zcb9223419a7a0224fe7af2866944197ee4ab18e25913cae8507bb306a284f");
}
/*
 * h2unit Me (Memory equal) can be used to verify memory block.
 */
H2UNIT_CASE(memory equal)
{
   const unsigned char e00[] = "abcdefghijklmnopqrstuvwxyz";
   H2EQ(Me(e00, sizeof(e00)), "abcdefghijklmnopqrstuvwxyz");
   H2EQ(Me(e00, sizeof(e00)), "abcdEfghijklmnopqrsTuvwxyz");
}

/*
 * h2unit Pointee can be used to verify the data pointer point to.
 */
H2UNIT_CASE(point to)
{
   int x = 1;
   H2EQ(PointeeEq(1), &x);
   H2EQ(Pe(2), &x);
}
