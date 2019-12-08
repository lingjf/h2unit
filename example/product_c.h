#ifndef _PRODUCT_C_H_
#define _PRODUCT_C_H_

int getEven(int var);

double getCeil(double var);

typedef struct Point
{
   int x;
   int y;
} Point;

Point* create_point(int x, int y);

const char* Point_toString(Point* point);

const char* Point_tojson(Point* point);

int orig_foo(int a);
int orig_bar(int b);

char* orig_varg(const char* format, ...);

int getSum(int x);

int isLegal(const char* str);

#endif
