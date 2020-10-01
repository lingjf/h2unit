
#include "../source/h2_unit.cpp"

char* h2_libc_malloc_tostring(h2::h2_libc_malloc* m, char* s)
{
   strcpy(s, "[");
   int c1 = 0;
   for (auto* p = m->next; p; p = p->next) {
      sprintf(s + strlen(s), "%s", c1++ ? "," : "");
      sprintf(s + strlen(s), "{ size : %lld, buddies: [", p->size);
      h2_list_for_each_entry (b, p->buddies, h2::h2_libc_malloc::buddy, x) {
         sprintf(s + strlen(s), "%s%lld", li ? "," : "", b->size);
      }
      strcat(s, "]");
      strcat(s, "}");
   }
   strcat(s, "]");

   return s;
}

CASE(libc_malloc)
{
   h2::h2_libc_malloc M;

   char t1[1024];
   JE("[]", h2_libc_malloc_tostring(&M, t1));

   size_t* p1 = (size_t*)M.malloc(88);
   OK(NotNull, p1);
   OK(88 + 8, *(p1 - 1));
   JE("[                                              \
          {                                           \
            \"size\": 4096 * 257,                     \
            \"buddies\": [4096 * 257 - 32 - (88+8)]   \
          }                                           \
        ]",
      h2_libc_malloc_tostring(&M, t1));

   size_t* p2 = (size_t*)M.malloc(1001);
   OK(NotNull, p2);
   OK(1008 + 8, *(p2 - 1));
   JE("[                                                          \
          {                                                       \
            \"size\": 4096 * 257,                                 \
            \"buddies\": [4096 * 257 - 32 - (88+8) - (1008+8)]    \
          }                                                       \
        ]",
      h2_libc_malloc_tostring(&M, t1));

   size_t* p3 = (size_t*)M.malloc(201);
   OK(NotNull, p3);
   OK(208 + 8, *(p3 - 1));
   JE("[                                                                   \
          {                                                                \
            \"size\": 4096 * 257,                                          \
            \"buddies\": [4096 * 257 - 32 - (88+8) - (1008+8) - (208+8)]   \
          }                                                                \
        ]",
      h2_libc_malloc_tostring(&M, t1));

   M.free(p2);
   JE("[                                                                            \
          {                                                                         \
            \"size\": 4096 * 257,                                                   \
            \"buddies\": [4096 * 257 - 32 - (88+8) - (1008+8) - (208+8), (1008+8)]  \
          }                                                                         \
        ]",
      h2_libc_malloc_tostring(&M, t1));

   M.free(p1);
   JE("[                                                                                     \
          {                                                                                  \
            \"size\": 4096 * 257,                                                            \
            \"buddies\": [4096 * 257 - 32 - (88+8) - (1008+8) - (208+8), (1008+8)+(88+8)]    \
          }                                                                                  \
        ]",
      h2_libc_malloc_tostring(&M, t1));

#if 0
   M.free(p3);
   JE("[                                     \
          {                                  \
            \"size\": 4096 * 257,            \
            \"buddies\": [4096 * 257 - 32]   \
          }                                  \
        ]",
      h2_libc_malloc_tostring(&M, t1));
#endif      
}

CASE(libc_malloc repeat)
{
   struct A {
      h2::h2_list x;
      char value[4];
   };

   h2::h2_libc_malloc M;
   h2::h2_list as;

   for (int i = 0; i < 1024 * 32; i++) {
      A* a1 = (A*)M.malloc(sizeof(A) + rand() % 5000);
      as.push_back(a1->x);
      A* a2 = (A*)M.malloc(sizeof(A) + rand() % 50000);
      as.push_back(a2->x);

      A* a = h2_list_pop_entry(as, A, x);
      M.free(a);
   }

   h2_list_for_each_entry (p, as, A, x) {
      M.free(p);
   }
}
