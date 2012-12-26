#ifndef PRODUCT2_H
#define PRODUCT2_H

class wstring
{
private:
   int len;
   int cap;
   unsigned short* dat;
public:
   wstring();
   ~wstring();
   void fromInt(int value);
   unsigned char* getData();
   char* tuString();
};

void allocation();

#endif
