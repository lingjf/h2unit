#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

SUITE(shared_ptr)
{
   Case(local) 
   {
      std::string* p = new std::string("h2unit");
      h2::h2_shared_ptr<std::string> s1(p);
   };

   Case(dual)
   {
      std::string* p = new std::string("h2unit");
      h2::h2_shared_ptr<std::string> s1(p);
      h2::h2_shared_ptr<std::string> s2 = s1;
   };

   Case(malloc)
   {
      char* p = (char*)malloc(100);
      h2::h2_shared_ptr<char> s1(p);
   };

   Case(new)
   {
      char* p = (char*)new char;
      h2::h2_shared_ptr<char> s1(p);
   };
}
