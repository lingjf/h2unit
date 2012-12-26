#include "h2unit.h"
#include <stdlib.h>
#include <string.h>

#include "product_2.h"

H2UNIT(wstring)
{
};

H2CASE(wstring, "Constructor")
{
   wstring w;
   H2EQUAL_STRCMP("wstring(len=1,cap=256,dat=)", w.tuString());
}

H2CASE(wstring, "fromInt(123)")
{
   wstring w;
   w.fromInt(123);
   H2EQUAL_REGEX("wstring(len=[0-9],cap=??,dat=*)", w.tuString());
}

H2CASE(wstring, "fromInt(12345)")
{
   wstring w;
   w.fromInt(12345);
   unsigned char exp_buf[] = { 0x32, '\0', 0x32, '\0', 0x33, '\0', 0x34, '\0', 0x35, '\0' };
   H2EQUAL_MEMCMP(exp_buf, w.getData(), sizeof(exp_buf));
}

H2CASE_IGNORE(wstring, "show compare format")
{
   H2EQUAL_STRCMP(
               "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ",
               "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

H2CASE(wstring, "test catch none")
{
   wstring w;
   H2CATCH_NONE(w.fromInt(8));
   H2CATCH_NONE(w.fromInt(-8));
}

H2CASE(wstring, "test catch something")
{
   wstring w;
   H2CATCH_THROW(w.fromInt(-8), int);
   H2CATCH_THROW(w.fromInt(-8), float);
}

H2UNIT(memory_leak)
{
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(memory_leak, "test memory leak")
{
   allocation();
}

H2CASE(memory_leak, "test memory faulty injection")
{
   H2_FAULTY_INJECT_MEMORY(10);
   H2CHECK(NULL == malloc(11));
}

