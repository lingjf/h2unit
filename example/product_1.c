#include "product_1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getEven(int var)
{
   return var & (~1);
}

double getCeil(double var)
{
   return (double) ((int) var);
}

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

