
#ifndef ___H2_SUITE__H___
#define ___H2_SUITE__H___

class h2_case;

struct h2_suite
{
   const char* name;
   const char* file;
   const int line;

   int status_count[10];

   std::vector<h2_case*> case_list;

   h2_suite(const char* _name, const char* _file, const int _line)
     : name(_name), file(_file), line(_line)
   {
      memset(status_count, 0, sizeof(status_count));

      G()->push_back(this);
   }

   ~h2_suite() {}

   void install(h2_case* case_)
   {
      case_list.push_back(case_);
   }

   static h2_suite* A()
   {
      static h2_suite A("Anonymous", "", 0);
      return &A;
   }

   static std::vector<h2_suite*>* G()
   {
      static std::vector<h2_suite*> G;
      return &G;
   }
};

#endif
