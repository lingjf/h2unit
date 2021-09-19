#include "../source/h2_unit.cpp"
#include "test_types.hpp"

char* h2_source_tojson(h2::h2_source* s, char* b)
{
   sprintf(b, "{"
              "\"reference_count\": %d"
              "}",
           s->reference_count);
   return b;
}

char* h2_sources_tojson(h2::h2_sources& a, char* b)
{
   sprintf(b, "[");
   h2_list_for_each_entry (p, i, a.sources, h2::h2_source, x) {
      if (i) sprintf(b + strlen(b), ",");
      h2_source_tojson(p, b + strlen(b));
   }
   sprintf(b + strlen(b), "]");
   return b;
}

SUITE(sources)
{
   char t1[1024];
   h2::h2_sources pool;

   Case(init)
   {
      JE("[]", h2_sources_tojson(pool, t1));
      OK(IsNull, pool.get((void*)foobar2));
   }

   Case(add del)
   {
      auto a = pool.add((void*)foobar2, "foobar2", __FILE__, __LINE__);
      OK(NotNull, a);
      JE("[{'reference_count':1}]", h2_sources_tojson(pool, t1));
      auto b = pool.add((void*)foobar2, "foobar2", __FILE__, __LINE__);
      OK(NotNull, b);
      JE("[{'reference_count':2}]", h2_sources_tojson(pool, t1));

      auto ret = pool.get((void*)foobar2);
      OK(NotNull, ret);
      JE("{'reference_count':2}", h2_source_tojson(ret, t1));

      pool.del(ret);
      JE("[{'reference_count':1}]", h2_sources_tojson(pool, t1));
      pool.del(ret);
      JE("[]", h2_sources_tojson(pool, t1));
   }
}
