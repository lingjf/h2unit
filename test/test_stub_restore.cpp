#include "../source/h2_unit.cpp"
#include "test_types.hpp"

CASE(stub temporary restore)
{
   OK(2, foobar2(0, ""));
   STUB(foobar2, foobar2_fake);
   {
      OK(-2, foobar2(0, ""));
      h2::h2_stub_temporary_restore t((void*)foobar2);
      OK(2, foobar2(0, ""));
   }
   OK(-2, foobar2(0, ""));
}
