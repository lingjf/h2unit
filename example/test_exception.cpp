#include "h2unit.h"
#include <stdlib.h>

#include "demo_exception.h"


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


