#include "h2unit.h"
#include <stdlib.h>

#include "demo_basic.h"


int gv;

/*
 * H2UNIT is a unit test Suite, which contains several test cases.
 */
H2UNIT(getEven)
{
   int uv;

   /*
    * setup() will be executed before every test case belong to this suite.
    * Typically it is used to prepare preconditions.
    */
   void setup()
   {
      uv = 2;
      gv = 3;
   }
   /*
    * teardown() will be executed after every test case belong to this suite.
    * whatever test case passed or failed.
    * Typically it is used to release resource.
    */
   void teardown()
   {
      uv = 0;
      gv = 1;
   }
};

/*
 * H2CASE is a unit test case
 */
H2CASE(getEven,"test for variable and setup")
{
   H2EQ_TRUE(2 == uv);
   H2EQ_TRUE(4 == gv);
}

H2CASE(getEven,"getEvent with odd value")
{
   H2EQ_MATH(4, getEven(7));
}

H2CASE(getEven,"getEven with even value")
{
   H2EQ_MATH(6, getEven(6));
   H2NE_MATH(6, getEven(6));
}

H2CASE(getEven,"different type integer")
{
   unsigned char e1 = 2;
   unsigned char a1 = 2;
   H2EQ_MATH(e1, a1);

   char e2 = 2;
   char a2 = 2;
   H2EQ_MATH(e2, a2);

   short e3 = 2;
   short a3 = 2;
   H2EQ_MATH(e3, a3);

   unsigned short e4 = 2;
   unsigned short a4 = 2;
   H2EQ_MATH(e4, a4);

   long e5 = 2;
   long a5 = 2;
   H2EQ_MATH(e5, a5);

   unsigned long e6 = 2;
   unsigned long a6 = 2;
   H2EQ_MATH(e6, a6);

   long long e7 = 1234567890LL;
   long long a7 = 1234567890LL;
   H2EQ_MATH(e7, a7);

   unsigned long long e8 = 0x12345678900ULL;
   unsigned long long a8 = 12345678900ULL;
   H2EQ_MATH(e8, a8);
}


H2UNIT(getCeil)
{
  /*
   * setup() and teardown() can be omitted. The default is doing nothing
   */
};

H2CASE(getCeil, "test for float type number")
{
   H2EQ_MATH(6.0, getCeil(6.2));
   H2EQ_MATH(6.0, getCeil(6.4));
   H2EQ_MATH(3.14159, getCeil(3.14159));
}

H2CASE(getCeil, "test for number in range")
{
   H2EQ_RANGE(6, 7, getCeil(6.8));
   H2EQ_RANGE(7, 8, getCeil(6.8));
}

H2CASE(getCeil, "test for number not in range")
{
   H2NE_RANGE(7, 8, getCeil(6.8));
   H2NE_RANGE(6, 7, getCeil(6.8));
}

H2CASE(getCeil, "test for number in set")
{
   double t[] = {
      1.0, 2.0, 3.0
   };
   H2EQ_INSET(t, 3, getCeil(2.8));
   H2EQ_INSET(t, 3, getCeil(6.8));
}

H2CASE(getCeil, "test for number not in set")
{
   double t[] = {
      1.0, 2.0, 3.0
   };
   H2NE_INSET(t, 3, getCeil(6.8));
   H2NE_INSET(t, 3, getCeil(2.8));
}

H2UNIT(Point)
{
};

/*
 * h2unit H2EQ_STRCMP can be used to verify Case sensitive String.
 */
H2CASE(Point, "string equal")
{
   Point p1;
   H2EQ_STRCMP("Point(0, 0)", p1.tuString());
   Point p2(1,2);
   H2EQ_STRCMP("Point(1, 3)", p2.tuString());
}
/*
 * h2unit H2NE_STRCMP can be used to verify Case sensitive String.
 */
H2CASE(Point, "string not equal")
{
   Point p1;
   H2NE_STRCMP("Point(1, 0)", p1.tuString());
   Point p2(1,2);
   H2NE_STRCMP("Point(1, 2)", p2.tuString());
}

/*
 * h2unit H2EQ_WILDCARD can be used to verify string by WildCard. support:
 *  -- ? any one char
 *  -- * any char(s)
 *  -- [] specified char(s)
 *     -- [abc] a or b or c
 *     -- [a-z] a or b or ... or z
 *     -- [!abc] any char except a and b and c
 *     -- [^abc] any char except a and b and c
 */
H2CASE(Point, "wildcard string")
{
   Point p1;
   H2EQ_WILDCARD("Point(0, ?)", p1.tuString());
   Point p2(1,2);
   H2EQ_WILDCARD("Point(*)", p2.tuString());
   Point p3(3,4);
   H2EQ_WILDCARD("Point([123], [0-9])", p3.tuString());
   Point p4(6,12);
   H2EQ_WILDCARD("Point([!a-zA-Z], [^a-z][^a-z])", p4.tuString());

   H2EQ_WILDCARD("Point\\*Point\\?\\[1,2\\]", "Point*Point?[1,2]");
}

/*
 * h2unit H2EQ_REGEX can be used to verify string by Regular express. support:
 *  -- c any specified char
 *  -- . any char
 *  -- [] specified char(s)
 *     -- [abc] a or b or c
 *     -- [a-z] a or b or ... or z
 *     -- [!abc] any char except a and b and c
 *     -- [^abc] any char except a and b and c
 *  -- * any times appear of previous char
 *  -- + 1 and more times appear of previous char
 *  -- {} specified appear times of previous char
 *     -- {2} appear absolutely only 2 times
 *     -- {1,3} appear at least 1 times and at most 3 times
 *     -- {2,} appear at least 2 times
 *     -- {,3} appear at most 3 times
 *  -- ^ begin of
 *  -- $ end of
 */
H2CASE(Point, "regex string")
{
   Point p1;
   H2EQ_REGEX("Point(0, 0)", p1.tuString());
   Point p2(1,2);
   H2EQ_REGEX("Point(.", p2.tuString());
   Point p3(3,4);
   H2EQ_REGEX("Point(.*)", p3.tuString());
   Point p4(5,6);
   H2EQ_REGEX("^Point(.*)$", p4.tuString());
   Point p5(7,8);
   H2EQ_REGEX("Point([789], [0-9])", p5.tuString());
   Point p6(11,88);
   H2EQ_REGEX("Point([0-9]*, [!0-5]*)", p6.tuString());
   Point p7(11,88);
   H2EQ_REGEX("Point([0-9]+, [0-9]+)", p7.tuString());
   Point p8(11,56);
   H2EQ_REGEX("P{1,}oint(1{2},{,2} [0-9]{1,3})", p8.tuString());
}

/*
 * Case-Less strcmp
 */

H2CASE(Point, "caseless string equal")
{
   Point p1;
   H2EQ_STRCMP_NOCASE("point(0, 0)", p1.tuString());
   Point p2(1,2);
   H2EQ_STRCMP_NOCASE("POINT(1, 2)", p2.tuString());
}

/*
 * If no setup() and teardown(), H2UNIT can be omitted.
 * H2UNIT_CASE act H2UNIT plus H2CASE.
 *
 * h2unit H2EQ_MEMCMP can be used to verify memory block.
 */

H2UNIT_CASE("memory equal")
{
   Point p1;
   H2EQ_MEMCMP("Point(0, 0)", p1.tuString(), sizeof("Point(0, 0)"));
   H2EQ_MEMCMP("Point(1, 0)", p1.tuString(), sizeof("Point(1, 0)"));
}

/*
 * h2unit can take over assert(expr).
 */

H2UNIT(take_over_assert)
{
};

/*
 * H2TODO/H2UNIT_TODO is a case which will NOT be executed.
 * It can be used in TDD phase-1
 */
H2TODO(take_over_assert, "h2unit assert")
{
   getTail(NULL);
}

H2UNIT_TODO("h2unit assert")
{
   getTail((char*) "");
}


