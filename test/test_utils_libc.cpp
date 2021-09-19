
#include "../source/h2_unit.cpp"

char* h2_libc_malloc_tostring(h2::h2_libc_malloc* m, char* s)
{
   strcpy(s, "[");
   int c1 = 0;
   for (auto* p = m->next; p; p = p->next) {
      sprintf(s + strlen(s), "%s", c1++ ? "," : "");
      sprintf(s + strlen(s), "{ bytes : %llu, buddies: [", p->bytes);
      h2_list_for_each_entry (b, i, p->buddies, h2::h2_libc_malloc::buddy, x) {
         sprintf(s + strlen(s), "%s%llu", i ? "," : "", b->size);
      }
      strcat(s, "]");
      strcat(s, "}");
   }
   strcat(s, "]");

   return s;
}

SUITE(libc_malloc)
{
   Case(normal)
   {
      h2::h2_libc_malloc M;

      char t1[1024], t2[1024];
      JE("[]", h2_libc_malloc_tostring(&M, t1));

      long long* p1 = (long long*)M.malloc(88);
      OK(NotNull, p1);
      OK(88 + 8, *(p1 - 1));

      sprintf(t2, "[                                              \
                     {                                            \
                        \"bytes\": 4*1024*1024,                   \
                        \"buddies\": [4*1024*1024 - %d - (88+8)]  \
                     }                                            \
                  ]",
              (int)sizeof(h2::h2_libc_malloc::block));
      JE(t2, h2_libc_malloc_tostring(&M, t1));

      long long* p2 = (long long*)M.malloc(1001);
      OK(NotNull, p2);
      OK(1008 + 8, *(p2 - 1));

      sprintf(t2, "[                                                          \
                     {                                                        \
                        \"bytes\": 4*1024*1024,                               \
                        \"buddies\": [4*1024*1024 - %d - (88+8) - (1008+8)]   \
                     }                                                        \
                  ]",
              (int)sizeof(h2::h2_libc_malloc::block));
      JE(t2, h2_libc_malloc_tostring(&M, t1));

      long long* p3 = (long long*)M.malloc(201);
      OK(NotNull, p3);
      OK(208 + 8, *(p3 - 1));

      sprintf(t2, "[                                                                   \
                     {                                                                 \
                        \"bytes\": 4*1024*1024,                                        \
                        \"buddies\": [4*1024*1024 - %d - (88+8) - (1008+8) - (208+8)]  \
                     }                                                                 \
                  ]",
              (int)sizeof(h2::h2_libc_malloc::block));
      JE(t2, h2_libc_malloc_tostring(&M, t1));

      M.free(p2);
      sprintf(t2, "[                                                                            \
                     {                                                                          \
                        \"bytes\": 4*1024*1024,                                                 \
                        \"buddies\": [4*1024*1024 - %d - (88+8) - (1008+8) - (208+8), (1008+8)] \
                     }                                                                          \
                  ]",
              (int)sizeof(h2::h2_libc_malloc::block));
      JE(t2, h2_libc_malloc_tostring(&M, t1));

      M.free(p1);
      sprintf(t2, "[                                                                                     \
                     {                                                                                   \
                        \"bytes\": 4*1024*1024,                                                          \
                        \"buddies\": [4*1024*1024 - %d - (88+8) - (1008+8) - (208+8), (1008+8)+(88+8)]   \
                     }                                                                                   \
                  ]",
              (int)sizeof(h2::h2_libc_malloc::block));

#if 0
   M.free(p3);
   sprintf(t2, "[                                     \
                  {                                   \
                     \"bytes\": 4*1024*1024,          \
                     \"buddies\": [4*1024*1024 - %d]  \
                  }                                   \
               ]",
           (int)sizeof(h2::h2_libc_malloc::block));
#endif
   }

   Case(repeat)
   {
      struct C0 {
         h2::h2_list x;
         char value[4];
      };

      h2::h2_libc_malloc m;
      h2::h2_list as;

      for (int i = 0; i < 1024 * 32; i++) {
         C0* a1 = (C0*)m.malloc(sizeof(C0) + rand() % 5000);
         as.push_back(a1->x);
         C0* a2 = (C0*)m.malloc(sizeof(C0) + rand() % 50000);
         as.push_back(a2->x);

         C0* a = h2_list_pop_entry(as, C0, x);
         m.free(a);
      }

      h2_list_for_each_entry (p, as, C0, x) {
         m.free(p);
      }
   }
}

SUITE(libc)
{
   struct C1 {
      h2::h2_list x;
      char* value;
      C1()
      {
         value = (char*)malloc(8);
      }
   };

   struct C2 : h2::h2_libc {
      h2::h2_list x;
      char* value;
      C2()
      {
         value = (char*)malloc(8);
      }
   };

   Case(not derived from libc)
   {
      // new C1;
      // should have leak 2 please
   }

   Case(derived from libc)
   {
      new C2;
      delete new C2;
   }
}
