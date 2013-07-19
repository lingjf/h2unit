#ifndef _DEMO_BASIC_H_
#define _DEMO_BASIC_H_

int getEven(int var);

double getCeil(double var);

class Point {
private:
   int m_x, m_y;
public:
   Point();
   Point(int x, int y);
   ~Point();
   char* tuString();
};

class Parser {
public:
   Parser();
   int fromInt(const char* str);
};

char* getTail(char* str);

#endif
