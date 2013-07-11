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
    * teardown() will be executed after every test case belong to this suite
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
H2CASE(getEven,"test for h2unit varible and setup")
{
   H2EQUAL_TRUE(2 == uv);
}

H2CASE(getEven,"test for global varible and H2EQUAL[_TRUE]")
{
#if defined(__GNUC__) || (defined(_MSC_VER) && _MSC_VER >= 1400)
   H2EQUAL(4 == gv);
#else
   H2EQUAL_TRUE(4 == gv);
#endif
}

H2CASE(getEven,"test for H2EQUAL_INT")
{
   H2EQUAL_INT(4, getEven(7));
}

H2CASE(getEven,"test for general H2EQUAL[_INTEGER]")
{
   H2EQUAL(6, getEven(6));
}

H2CASE(getEven,"test for H2EQUAL_INT/H2EQUAL with byte")
{
   unsigned char e1 = 2;
   unsigned char a1 = 2;
   H2EQUAL_INT(e1, a1);

   char e2 = 2;
   char a2 = 2;
   H2EQUAL(e2, a2);
}

H2CASE(getEven,"test for H2EQUAL_INT/H2EQUAL with short")
{
   short e1 = 2;
   short a1 = 2;
   H2EQUAL_INT(e1, a1);

   unsigned short e2 = 2;
   unsigned short a2 = 2;
   H2EQUAL(e2, a2);
}

H2CASE(getEven,"test for H2EQUAL_INT/H2EQUAL with long")
{
   long e1 = 2;
   long a1 = 2;
   H2EQUAL_INT(e1, a1);

   unsigned long e2 = 2;
   unsigned long a2 = 2;
   H2EQUAL(e2, a2);
}

H2CASE(getEven,"test for H2EQUAL_INT/H2EQUAL with long long")
{
   long long e1 = 2;
   long long a1 = 2;
   H2EQUAL_INT(e1, a1);

   unsigned long long e2 = 0x12345678900ULL;
   unsigned long long a2 = 12345678900ULL;
   H2EQUAL(e2, a2);
}

H2TODO(getEven, "todo feature")
{

}


H2UNIT(getCeil)
{
  /*
   * setup() and teardown() can be omitted.
   */
};

H2CASE(getCeil, "test for general H2EQUAL")
{
   H2EQUAL_FLOAT(6.0, getCeil(6.2));
   H2EQUAL(6.0, getCeil(6.4));
}

H2CASE(getCeil, "test for H2EQUAL_FLOAT")
{
   H2EQUAL_FLOAT(6.8, getCeil(6.8));
}

H2CASE(getCeil, "test for H2EQUAL_RANGE")
{
   H2EQUAL_RANGE(6, 7, getCeil(6.8));
   H2EQUAL_RANGE(7, 8, getCeil(6.8));
}

H2CASE(getCeil, "test for H2EQUAL_RANGE")
{
   double t[] = {
      1.0, 2.0, 3.0
   };
   H2EQUAL_INSET(t, 3, getCeil(2.8));
   H2EQUAL_INSET(t, 3, getCeil(6.8));
}

H2UNIT(Point)
{
};

/*
 * h2unit H2EQUAL_STRCMP can be used to verify String.
 */
H2CASE(Point, "string equal")
{
   Point p1;
   H2EQUAL_STRCMP("Point(0, 0)", p1.tuString());
   Point p2(1,2);
   H2EQUAL_STRCMP("Point(1, 3)", p2.tuString());
}

/*
 * h2unit H2EQUAL_REGEX can be used to verify WildCard String.
 * Regular Express is not supported yet ,
 * due to import complex PCRE will break the h2unit design principle :
 *       Light-Weight just like hydrogen !
 */
H2CASE(Point, "wildcard equal")
{
   Point p1;
   H2EQUAL_WILDCARD("Point([0-9], *)", p1.tuString());
   Point p2(1,2);
   H2EQUAL_WILDCARD("Point([0-9], ?)", p2.tuString());
   Point p3(3,4);
   H2EQUAL_WILDCARD("Point(*, 5)", p3.tuString());
}

/*
 * Case-Less strcmp
 */

H2CASE(Point, "caseless string equal")
{
   Point p1;
   H2EQUAL_STRCMP_NOCASE("point(0, 0)", p1.tuString());
   Point p2(1,2);
   H2EQUAL_STRCMP_NOCASE("POINT(1, 2)", p2.tuString());
}

/*
 * h2unit H2EQUAL_MEMCMP can be used to verify memory block.
 */

H2CASE(Point, "memory equal")
{
   Point p1;
   H2EQUAL_MEMCMP("Point(0, 0)", p1.tuString(), 12);
   H2EQUAL_MEMCMP("Point(1, 0)", p1.tuString(), 12);
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

