#ifndef _PRODUCT_CPP_H_
#define _PRODUCT_CPP_H_

class Animal /* 动物 */
{
 private:
   int x, y;

 public:
   Animal()
     : x(0), y(0) {}
   virtual ~Animal() {}

   static int born(int a)
   {
      return 0;
   }

   int go(int x, int y)
   {
      this->x += x;
      this->y += y;
      return 0;
   }

   int go(int xy);

   virtual const char* say() = 0;
   virtual int cry() = 0;
};

class Vivipara /* 胎生动物 */ : public Animal
{
 public:
   const char* say()
   {
      return "";
   }

   int cry()
   {
      return 1;
   }
};

class Ovipara /* 卵生动物 */ : public Animal
{
 public:
   int cry()
   {
      return 0;
   }
};

class Dog /* 狗 */ : public Vivipara
{
 public:
   int age;
   Dog(int age_)
     : age(age_) {}

   const char* say()
   {
      return "wang";
   }

   void run() {}
};

class Cat /* 猫 */ : public Vivipara
{
 public:
   Cat(Animal* mother, Animal* father) {}

   const char* say()
   {
      return "miao";
   }
};

class Bird /* 鸟 */ : public Ovipara
{
 public:
   const char* say()
   {
      return "jiji";
   }
};

class Centipede /* 蜈蚣 */ : public Ovipara
{
 public:
   Centipede(int feet1,
             int feet2,
             int feet3,
             int feet4,
             int feet5,
             int feet6,
             int feet7,
             int feet8,
             int feet9,
             int feet10,
             int feet11) {}

   const char* say()
   {
      return "";
   }
};

#endif
