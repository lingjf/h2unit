#ifndef _DEMO_CLANGUAGE_H_
#define _DEMO_CLANGUAGE_H_

int getEven(int var);

double getCeil(double var);

typedef struct Point {
   int x;
   int y;
} Point;

char* Point_toString(Point* point);

char* Point_tojson(Point* point);


int orig_foo(int a);

char* orig_varg(const char* format, ...);

int getSum(int x);

int isLegal(const char* str);

#endif
