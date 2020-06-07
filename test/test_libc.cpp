#include "../source/h2_unit.cpp"

static h2::h2_libc_malloc M;

char* h2_libc_malloc_tostring(h2::h2_libc_malloc* m, char* s)
{
   strcpy(s, "[");
   h2_list_for_each_entry (p, m->buddies, h2::h2_libc_malloc::buddy, x) {
      sprintf(s + strlen(s), "%s%lu", i ? "," : "", p->size);
   }
   strcat(s, "]");

   return s;
}

CASE(libc_malloc)
{
   OK(8, sizeof(size_t));
   char t1[1024];
   JE("[]", h2_libc_malloc_tostring(&M, t1));

   size_t* p1 = (size_t*)M.malloc(88);
   OK(NotNull, p1);
   OK(88 + 8, *(p1 - 1));
   JE("[1024 * 1024 * 100 - (88 + 8)]", h2_libc_malloc_tostring(&M, t1));

   size_t* p2 = (size_t*)M.malloc(1001);
   OK(NotNull, p2);
   OK(1008 + 8, *(p2 - 1));
   JE("[1024 * 1024 * 100 - (88 + 8) - (1008 + 8)]",
      h2_libc_malloc_tostring(&M, t1));

   size_t* p3 = (size_t*)M.malloc(201);
   OK(NotNull, p3);
   OK(208 + 8, *(p3 - 1));
   JE("[1024 * 1024 * 100 - (88 + 8) - (1008 + 8) - (208 + 8)]",
      h2_libc_malloc_tostring(&M, t1));

   M.free(p2);
   JE("[(1008 + 8),      \
       1024 * 1024 * 100 - (88 + 8) - (1008 + 8) - (208 + 8)]",
      h2_libc_malloc_tostring(&M, t1));

   size_t* p21 = (size_t*)M.malloc(100);
   OK(NotNull, p21);
   OK(104 + 8, *(p21 - 1));
   JE("[(1008 + 8 - (104 + 8)),    \
       1024 * 1024 * 100 - (88 + 8) - (1008 + 8) - (208 + 8)]",
      h2_libc_malloc_tostring(&M, t1));

   M.free(p21);
   JE("[(1008 + 8),    \
       1024 * 1024 * 100 - (88 + 8) - (1008 + 8) - (208 + 8)]",
      h2_libc_malloc_tostring(&M, t1));

   M.free(p1);
   JE("[(88 + 8) + (1008 + 8),     \
       1024 * 1024 * 100 - (88 + 8) - (1008 + 8) - (208 + 8)]",
      h2_libc_malloc_tostring(&M, t1));

   M.free(p3);
   JE("[1024 * 1024 * 100]", h2_libc_malloc_tostring(&M, t1));
}
