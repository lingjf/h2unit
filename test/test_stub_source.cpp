#include "../source/h2_unit.cpp"
#include "test_types.hpp"

static h2::h2_string h2_source_tojson(h2::h2_source* s)
{
   h2::h2_string out;
   out.sprintf("{"
               "\"reference_count\": %d"
               "}",
               s->reference_count);
   return out;
}

static h2::h2_string h2_sources_tojson(h2::h2_sources& a)
{
   h2::h2_string out;
   out.sprintf("[");
   h2_list_for_each_entry (p, i, a.sources, h2::h2_source, x) {
      if (i) out += ",";
      out += h2_source_tojson(p);
   }
   out.sprintf("]");
   return out;
}

SUITE(sources)
{
   h2::h2_sources pool;

   Case(init)
   {
      JE("[]", h2_sources_tojson(pool));
      OK(IsNull, pool.get((void*)foobar2));
   }

   Case(add del)
   {
      auto a = pool.add((void*)foobar2, "foobar2", H2_FILINE);
      OK(NotNull, a);
      JE("[{'reference_count':1}]", h2_sources_tojson(pool));
      auto b = pool.add((void*)foobar2, "foobar2", H2_FILINE);
      OK(NotNull, b);
      JE("[{'reference_count':2}]", h2_sources_tojson(pool));

      auto ret = pool.get((void*)foobar2);
      OK(NotNull, ret);
      JE("{'reference_count':2}", h2_source_tojson(ret));

      pool.del(ret);
      JE("[{'reference_count':1}]", h2_sources_tojson(pool));
      pool.del(ret);
      JE("[]", h2_sources_tojson(pool));
   }
}
