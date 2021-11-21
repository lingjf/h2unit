#include "../source/h2_unit.cpp"

h2::h2_string h2_test_tojson(h2::h2_test* d)
{
   h2::h2_string out;
   out.sprintf("{");
   out.sprintf("'tags':[");

   for (int i = 0; d->tags[i]; i++) {
      if (i) out.sprintf(",");
      out += "\"";
      out += d->tags[i];
      out += "\"";
   }
   out.sprintf("],");
   if (d->describe)
      out.sprintf("'describe': '%s',", d->describe);
   else
      out.sprintf("'describe': null,");
   out.sprintf("'name': '%s'", d->name);
   out.sprintf("}");
   return out;
}

SUITE(h2_test)
{
   Case(nullptr describe)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, nullptr);
      JE("{                   \
          'tags': [],         \
          'describe': null,   \
          'name': ''          \
        }",
         h2_test_tojson(&d));
   }

   Case(empty)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, "");
      JE("{                \
          'tags': [],      \
          'describe': '',  \
          'name': ''       \
        }",
         h2_test_tojson(&d));
   }

   Case(spaces)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, "  \t");
      JE("{                   \
          'tags': [],         \
          'describe': '  \t', \
          'name': ''          \
        }",
         h2_test_tojson(&d));
   }

   Case(no tags)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, "this is a case");
      JE("{                               \
          'tags': [],                     \
          'describe': 'this is a case',   \
          'name': 'this is a case'        \
        }",
         h2_test_tojson(&d));
   }

   Case(1 tags)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, "[atag] this is a case");
      JE("{                                     \
          'tags': ['atag'],                     \
          'describe': '[atag] this is a case',  \
          'name': 'this is a case'              \
        }",
         h2_test_tojson(&d));

      OK(d.tagged("atag"));
      OK(!d.tagged("btag"));
   }

   Case(2 tags)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, " [atag,btag]this is a case");
      JE("{                            \
          'tags': ['atag', 'btag'],    \
          'name': 'this is a case'     \
        }",
         h2_test_tojson(&d));

      OK(d.tagged("atag"));
      OK(d.tagged("btag"));
   }

   Case(3 tags)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, " [atag, btag ctag] this is a case");
      JE("{                                  \
          'tags': ['atag', 'btag', 'ctag'],  \
          'name': 'this is a case'           \
        }",
         h2_test_tojson(&d));

      OK(d.tagged("atag"));
      OK(d.tagged("btag"));
      OK(d.tagged("ctag"));
   }

   Case(3 tags tail)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, "this is a case[atag, btag ctag]");
      JE("{                                  \
          'tags': ['atag', 'btag', 'ctag'],  \
          'name': 'this is a case'           \
        }",
         h2_test_tojson(&d));

      OK(d.tagged("atag"));
      OK(d.tagged("btag"));
      OK(d.tagged("ctag"));
   }

   Case(3 tags inside)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, "this is a [atag, btag ctag]case");
      JE("{                                  \
          'tags': ['atag', 'btag', 'ctag'],  \
          'name': 'this is a case'           \
        }",
         h2_test_tojson(&d));

      OK(d.tagged("atag"));
      OK(d.tagged("btag"));
      OK(d.tagged("ctag"));
   }

   Case(not tags in string)
   {
      h2::h2_test d(H2_FILINE, __FILE__, __LINE__, "this \"is a [atag, btag ctag]case\"");
      JE("{                                              \
          'tags': [],                                    \
          'name': 'this \"is a [atag, btag ctag]case\"'  \
        }",
         h2_test_tojson(&d));

      OK(!d.tagged("atag"));
      OK(!d.tagged("btag"));
      OK(!d.tagged("ctag"));
   }
}
