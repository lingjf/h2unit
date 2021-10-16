#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(fs help)
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
      h2::h2_fs fs("abc.cpp");
      OK("abc.cpp", fs.basefile());
   }

   Case(basename "test/abc.cpp")
   {
      h2::h2_fs fs("test/abc.cpp");
      OK("abc.cpp", fs.basefile());
   }

   Case(basename "proj/test/abc.cpp")
   {
      h2::h2_fs fs("proj/test/abc.cpp");
      OK("abc.cpp", fs.basefile());
   }

   Case(basename "proj/test\\abc.cpp")
   {
      h2::h2_fs fs("proj/test\\abc.cpp");
      OK("abc.cpp", fs.basefile());
   }
}

h2::h2_string fs_tojson(const h2::h2_fs& fs)
{
   h2::h2_string out;
   out.sprintf("{");
   if (fs.file)
      out.sprintf("'file': '%s',", fs.file);
   else
      out.sprintf("'file': null,");

   out.sprintf("'line': %d,", fs.line);

   if (fs.func)
      out.sprintf("'func': '%s',", fs.func);
   else
      out.sprintf("'func': null,");

   out.sprintf("}");
   return out;
}

SUITE(fs)
{
   Case(default constructor)
   {
      h2::h2_fs fs;
      JE("{                \
          'file': null,    \
          'line': 0,       \
          'func': null     \
        }",
         fs_tojson(fs));
   }

   Case(file line)
   {
      h2::h2_fs fs("abc.cpp", 123);
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         fs_tojson(fs));
   }

   Case(copy constructor)
   {
      h2::h2_fs fs("abc.cpp", 123);
      h2::h2_fs fs2(fs);
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         fs_tojson(fs2));
   }

   Case(assigment copy constructor)
   {
      h2::h2_fs fs("abc.cpp", 123);
      h2::h2_fs fs2 = fs;
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         fs_tojson(fs2));
   }

   Case(assigment copy)
   {
      h2::h2_fs fs("abc.cpp", 123);
      h2::h2_fs fs2;

      fs2 = fs;
      JE("{                      \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
         fs_tojson(fs2));
   }
}

static void f1(const h2::h2_fs& fs = h2::h2_fs())
{
   JE("{                         \
          'file': null,          \
          'line': 0,             \
          'func': null           \
        }",
      fs_tojson(fs));
}
static void f2(const h2::h2_fs& fs = h2::h2_fs())
{
   JE("{                         \
          'file': 'abc.cpp',     \
          'line': 123,           \
          'func': null           \
        }",
      fs_tojson(fs));
}

CASE(fs)
{
   f1();
   f2({"abc.cpp", 123});
}
