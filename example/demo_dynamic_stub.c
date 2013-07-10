#include "demo_dynamic_stub.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int orig_foo(int a)
{
   return a + 1;
}

static int orig_bar(int b)
{
   return b + 1;
}

int getSum(int x)
{
   return orig_foo(x) + orig_bar(x);
}

int isLegal(const char* str)
{
   return atoi(str);
}

