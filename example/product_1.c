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

int orig_func(int a)
{
   return a + 1;
}

int isLegal(int var, const char* str)
{
   return orig_func(var) + atoi(str);
}

