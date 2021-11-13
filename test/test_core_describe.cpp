#include "../source/h2_unit.cpp"

h2::h2_string h2_describe_tojson(h2::h2_describe* d)
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
   out.sprintf("'name': '%s'", d->name);
   out.sprintf("}");
   return out;
}

SUITE(h2_describe)
{
   Case(nullptr)
   {
      h2::h2_describe d(nullptr);
      JE("{                \
          'tags': [],      \
          'name': ''       \
        }",
         h2_describe_tojson(&d));
   }

   Case(empty)
   {
      h2::h2_describe d("");
      JE("{              \
          'tags': [],    \
          'name': ''     \
        }",
         h2_describe_tojson(&d));
   }

   Case(spaces)
   {
      h2::h2_describe d("  \t");
      JE("{                \
          'tags': [],      \
          'name': ''       \
        }",
         h2_describe_tojson(&d));
   }

   Case(no tags)
   {
      h2::h2_describe d("this is a case");
      JE("{                         \
          'tags': [],               \
          'name': 'this is a case'  \
        }",
         h2_describe_tojson(&d));
   }

   Case(1 tags)
   {
      h2::h2_describe d("[atag] this is a case");
      JE("{                         \
          'tags': ['atag'],         \
          'name': 'this is a case'  \
        }",
         h2_describe_tojson(&d));
   }

   Case(2 tags)
   {
      h2::h2_describe d(" [atag,btag]this is a case");
      JE("{                            \
          'tags': ['atag', 'btag'],    \
          'name': 'this is a case'     \
        }",
         h2_describe_tojson(&d));
   }

   Case(3 tags)
   {
      h2::h2_describe d(" [atag, btag ctag] this is a case");
      JE("{                                  \
          'tags': ['atag', 'btag', 'ctag'],  \
          'name': 'this is a case'           \
        }",
         h2_describe_tojson(&d));
   }

   Case(3 tags tail)
   {
      h2::h2_describe d("this is a case[atag, btag ctag]");
      JE("{                                  \
          'tags': ['atag', 'btag', 'ctag'],  \
          'name': 'this is a case'           \
        }",
         h2_describe_tojson(&d));
   }

   Case(3 tags inside)
   {
      h2::h2_describe d("this is a [atag, btag ctag]case");
      JE("{                                  \
          'tags': ['atag', 'btag', 'ctag'],  \
          'name': 'this is a case'           \
        }",
         h2_describe_tojson(&d));
   }

   Case(not tags in string)
   {
      h2::h2_describe d("this \"is a [atag, btag ctag]case\"");
      JE("{                                              \
          'tags': [],                                    \
          'name': 'this \"is a [atag, btag ctag]case\"'  \
        }",
         h2_describe_tojson(&d));
   }
}
