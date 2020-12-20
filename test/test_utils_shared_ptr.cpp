#include "../source/h2_unit.cpp"

struct Shared {
   int* a;
   Shared(int* _a) : a(_a) { *a = 1; }
   ~Shared() { *a = 0; }
};

SUITE(shared_ptr)
{
   Case(local)
   {
      std::string* p = new std::string("h2unit");
      h2::h2_shared_ptr<std::string> s1(p);
   }

   Case(dual)
   {
      std::string* p = new std::string("h2unit");
      h2::h2_shared_ptr<std::string> s1(p);
      h2::h2_shared_ptr<std::string> s2 = s1;
   }

   // shared_ptr can't store ptr which allocated by malloc()
   // Case(malloc)
   // {
   //    char* p = (char*)malloc(100);
   //    h2::h2_shared_ptr<char> s1(p);
   // }

   Case(free at deconstructor)
   {
      char* p = (char*)new char;
      h2::h2_shared_ptr<char> s1(p);
   }

   Case(reset)
   {
      int a = 0;
      h2::h2_shared_ptr<Shared> s1(new Shared(&a));
      OK(1, a);
      s1.reset();
      OK(0, a);
   }
}
