
h2_inline void h2_memory::initialize()
{
   if (O.memory_check && !O.debug) h2_crash::install_segment_fault_handler();
   stack::root();
   if (O.memory_check) h2_override::I().overrides();
}
h2_inline void h2_memory::finalize()
{
   if (O.memory_check) h2_override::I().restores();
}
h2_inline void h2_memory::overrides()
{
   if (O.memory_check) h2_override::I().overrides();
}
h2_inline void h2_memory::restores()
{
   if (O.memory_check) h2_override::I().restores();
}

h2_inline void h2_memory::stack::root()
{
   h2_stack::I().push(LLONG_MAX / 2, sizeof(void*), nullptr, 0, false, "root", __FILE__, __LINE__);
}
h2_inline void h2_memory::stack::push(const char* file, int lino)
{
   h2_stack::I().push(LLONG_MAX / 2, sizeof(void*), nullptr, 0, false, "case", file, lino);
}
h2_inline h2_fail* h2_memory::stack::pop()
{
   return h2_stack::I().pop();
}
h2_inline long long h2_memory::stack::footprint()
{
   return h2_stack::I().top()->footprint;
}

static inline void parse_block_attributes(const char* attributes, long long& n_limit, int& n_align, unsigned char s_fill[32], int& n_fill, bool& noleak)
{
   n_limit = LLONG_MAX / 2;
   n_align = sizeof(void*);
   n_fill = 0;
   noleak = false;

   const char* p_noleak = strcasestr(attributes, "noleak");
   if (p_noleak) {
      noleak = true;
   }

   const char* p_limit = strcasestr(attributes, "limit");
   if (p_limit) {
      n_limit = h2_numeric::parse_int_after_equal(p_limit);
   }

   const char* p_align = strcasestr(attributes, "align");
   if (p_align) {
      n_align = (int)h2_numeric::parse_int_after_equal(p_align);
   }

   const char* p_fill = strcasestr(attributes, "fill");
   if (p_fill) {
      const char* p = strchr(p_fill, '=');
      if (p) {
         for (p += 1; *p && ::isspace(*p);) p++;  // strip left space

         if (p[0] == '0' && ::tolower(p[1]) == 'x') {
            n_fill = h2_numeric::hex_to_bytes(p + 2, s_fill);
         } else {
            long long v = strtoll(p, (char**)NULL, 10);
            if (v <= 0xFFU)
               n_fill = 1, *((unsigned char*)s_fill) = (unsigned char)v;
            else if (v <= 0xFFFFU)
               n_fill = 2, *((unsigned short*)s_fill) = (unsigned short)v;
            else if (v <= 0xFFFFFFFFU)
               n_fill = 4, *((unsigned int*)s_fill) = (unsigned int)v;
            else
               n_fill = 8, *((unsigned long long*)s_fill) = (unsigned long long)v;
         }
      }
   }
}

h2_inline h2_memory::stack::block::block(const char* attributes, const char* file, int lino)
{
   long long n_limit;
   int n_align;
   unsigned char s_fill[32];
   int n_fill;
   bool noleak;

   parse_block_attributes(attributes, n_limit, n_align, s_fill, n_fill, noleak);

   h2_stack::I().push(n_limit, n_align, s_fill, n_fill, noleak, "block", file, lino);
}
h2_inline h2_memory::stack::block::~block()
{
   h2_fail_g(h2_stack::I().pop(), false);
}
