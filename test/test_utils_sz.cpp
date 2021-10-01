#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(sz help)
{
   Case(type)
   {
      auto file = __FILE__;
      auto line = __LINE__;

      OK("char const" _pointer, h2::h2_cxa::demangle(typeid(decltype(file)).name()));
      OK("int", h2::h2_cxa::demangle(typeid(decltype(line)).name()));
   }

   Case(basename abc.cpp)
   {
      h2::h2_sz sz("abc.cpp");
      OK("abc.cpp", sz.basefile());
   }

   Case(basename "test/abc.cpp")
   {
      h2::h2_sz sz("test/abc.cpp");
      OK("abc.cpp", sz.basefile());
   }

   Case(basename "proj/test/abc.cpp")
   {
      h2::h2_sz sz("proj/test/abc.cpp");
      OK("abc.cpp", sz.basefile());
   }

   Case(basename "proj/test\\abc.cpp")
   {
      h2::h2_sz sz("proj/test\\abc.cpp");
      OK("abc.cpp", sz.basefile());
   }
}

h2::h2_string sz_tojson(const h2::h2_sz& sz)
{
   h2::h2_string out;
   out.sprintf("{");
   if (sz.file)
      out.sprintf("'file': '%s',", sz.file);
   else
      out.sprintf("'file': null,");

   out.sprintf("'line': %d,", sz.line);

   if (sz.func)
      out.sprintf("'func': '%s',", sz.func);
   else
      out.sprintf("'func': null,");

   out.sprintf("}");
   return out;
}

SUITE(sz)
{
   Case(default constructor)
   {
      h2::h2_sz sz;
      JE("{                \
          'file': null,    \
          'line': 0,       \
          'func': null     \
        }",
         sz_tojson(sz));
   }

   Case(file line)
   {
      h2::h2_sz sz("abc.cpp", 123);
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         sz_tojson(sz));
   }

   Case(copy constructor)
   {
      h2::h2_sz sz("abc.cpp", 123);
      h2::h2_sz sz2(sz);
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         sz_tojson(sz2));
   }

   Case(assigment copy constructor)
   {
      h2::h2_sz sz("abc.cpp", 123);
      h2::h2_sz sz2 = sz;
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         sz_tojson(sz2));
   }

   Case(assigment copy)
   {
      h2::h2_sz sz("abc.cpp", 123);
      h2::h2_sz sz2;

      sz2 = sz;
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         sz_tojson(sz2));
   }
}

static void f1(const h2::h2_sz& sz = h2::h2_sz())
{
   JE("{                         \
          'file': null,          \
          'line': 0,             \
          'func': null           \
        }",
      sz_tojson(sz));
}
static void f2(const h2::h2_sz& sz = h2::h2_sz())
{
   JE("{                         \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
      sz_tojson(sz));
}

CASE(sz)
{
   f1();
   f2({"abc.cpp", 123});
}