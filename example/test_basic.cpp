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

H2CASE(getEven,"test for byte type integer")
{
   unsigned char e1 = 2;
   unsigned char a1 = 2;
   H2EQ_MATH(e1, a1);

   char e2 = 2;
   char a2 = 2;
   H2EQ_MATH(e2, a2);
}

H2CASE(getEven,"test for short type integer")
{
   short e1 = 2;
   short a1 = 2;
   H2EQ_MATH(e1, a1);

   unsigned short e2 = 2;
   unsigned short a2 = 2;
   H2EQ_MATH(e2, a2);
}

H2CASE(getEven,"test for long type integer")
{
   long e1 = 2;
   long a1 = 2;
   H2EQ_MATH(e1, a1);

   unsigned long e2 = 2;
   unsigned long a2 = 2;
   H2EQ_MATH(e2, a2);
}

H2CASE(getEven,"test for long long type integer")
{
   long long e1 = 1234567890LL;
   long long a1 = 1234567890LL;
   H2EQ_MATH(e1, a1);

   unsigned long long e2 = 0x12345678900ULL;
   unsigned long long a2 = 12345678900ULL;
   H2EQ_MATH(e2, a2);
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
   Point p5(8,10);
   H2EQ_WILDCARD("Point(8, 10?)", p5.tuString());
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
 * h2unit H2EQ_MEMCMP can be used to verify memory block.
 */

H2CASE(Point, "memory equal")
{
   Point p1;
   H2EQ_MEMCMP("Point(0, 0)", p1.tuString(), sizeof("Point(0, 0)"));
   H2EQ_MEMCMP("Point(1, 0)", p1.tuString(), sizeof("Point(1, 0)"));
}


/*
 * h2unit can check the C++ exception throw out.
 */

H2UNIT(Parser)
{
};

H2CASE(Parser, "test catch none")
{
   Parser p;
   H2CATCH_NONE(p.fromInt("8"));
   H2CATCH_NONE(p.fromInt("-8"));
}

H2CASE(Parser, "test catch something")
{
   Parser p;
   H2CATCH_THROW(p.fromInt("-8"), int);
   H2CATCH_THROW(p.fromInt("-8"), float);
}

/*
 * h2unit can take over assert(expr).
 */

H2UNIT(take_over_assert)
{
};

H2TODO(take_over_assert, "h2unit assert")
{
   getTail(NULL);
}


