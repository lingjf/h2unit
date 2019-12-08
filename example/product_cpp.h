#ifndef _PRODUCT_CPP_H_
#define _PRODUCT_CPP_H_

class Animal
{
 private:
   int x, y;

 public:
   Animal()
     : x(0), y(0) {}

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

class Vivipara : public Animal
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

class Ovipara : public Animal
{
 public:
   int cry()
   {
      return 0;
   }
};

class Dog : public Vivipara
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

class Cat : public Vivipara
{
 public:
   Cat(Animal* mother, Animal* father) {}

   const char* say()
   {
      return "miao";
   }
};

class Bird : public Ovipara
{
 public:
   const char* say()
   {
      return "jiji";
   }
};

class Centipede : public Ovipara
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
             int feet9) {}

   const char* say()
   {
      return "";
   }
};

#endif
