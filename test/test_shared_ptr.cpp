#include "../source/h2_unit.h"

using namespace h2;

SUITE(shared_ptr) 
{
   Case(local) 
   {
      std::string* p = new std::string("h2unit");
      h2_shared_ptr<std::string> s1(p);
   };

   Case(dual) 
   {
      std::string* p = new std::string("h2unit");
      h2_shared_ptr<std::string> s1(p);
      h2_shared_ptr<std::string> s2 = s1;
   };

   Case(malloc)
   {
      char* p = (char*)malloc(100);
      h2_shared_ptr<char> s1(p);
   };

   Case(new)
   {
      char* p = (char*)new char;
      h2_shared_ptr<char> s1(p);
   };
}
