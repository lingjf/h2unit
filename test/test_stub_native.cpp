#include "../source/h2_unit.cpp"

char* h2_native_tojson(h2::h2_native* native, char* b)
{
   unsigned char saved[32];
   void* origin_fp;
   int reference_count = 0;
   sprintf(b, "{"
              "\"reference_count\": %d"
              "}",
           native->reference_count);
   return b;
}

char* h2_natives_tojson(h2::h2_natives& natives, char* b)
{
   sprintf(b, "[");
   h2_list_for_each_entry (p, i, natives.natives, h2::h2_native, x) {
      if (i) sprintf(b + strlen(b), ",");
      h2_native_tojson(p, b + strlen(b));
   }
   sprintf(b + strlen(b), "]");
   return b;
}

int foobar__test_stub_native(int a, int b)
{
   return 1;
}
static int STUB_foobar(int a, int b)
{
   return -1;
}

SUITE(natives)
{
   char t1[1024];
   h2::h2_natives pool;

   Case(init)
   {
      JE("[]", h2_natives_tojson(pool, t1));
      OK(IsNull, pool.get((void*)foobar__test_stub_native));
   }

   Case(add del)
   {
      pool += ((void*)foobar__test_stub_native);
      JE("[{'reference_count':1}]", h2_natives_tojson(pool, t1));
      pool += ((void*)foobar__test_stub_native);
      JE("[{'reference_count':2}]", h2_natives_tojson(pool, t1));

      auto ret = pool.get((void*)foobar__test_stub_native);
      OK(NotNull, ret);
      JE("{'reference_count':2}", h2_native_tojson(ret, t1));

      pool -= ((void*)foobar__test_stub_native);
      JE("[{'reference_count':1}]", h2_natives_tojson(pool, t1));
      pool -= ((void*)foobar__test_stub_native);
      JE("[]", h2_natives_tojson(pool, t1));
   }
}

CASE(stub temporary restore)
{
   OK(1, foobar__test_stub_native(0, 0));
   STUB(foobar__test_stub_native, STUB_foobar);
   {
      OK(-1, foobar__test_stub_native(0, 0));
      h2::h2_stub_temporary_restore t((void*)foobar__test_stub_native);
      OK(1, foobar__test_stub_native(0, 0));
   }
   OK(-1, foobar__test_stub_native(0, 0));
}
