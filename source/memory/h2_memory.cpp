struct h2_overrides {
   h2_singleton(h2_overrides);
   h2_override_stdlib stdlib;
   h2_override_platform platform;

   void set()
   {
      stdlib.set();
      platform.set();
   }
   void reset()
   {
      platform.reset();
      stdlib.reset();
   }
};

h2_inline void h2_memory::initialize()
{
   if (O.memory_check) h2_exempt::setup();
   hook();
   stack::root();
}
h2_inline void h2_memory::finalize()
{
   if (O.memory_check) h2_stack::I().at_exit = true;
}
h2_inline void h2_memory::hook(bool overrides)
{
   static bool s_overrides = false;
   if (O.memory_check) {
      if (overrides) {
         if (!s_overrides) h2_overrides::I().set();
      } else {
         if (s_overrides) h2_overrides::I().reset();
      }
      s_overrides = overrides;
   }
}

h2_inline void h2_memory::stack::root()
{
   h2_stack::I().push("", "root", {__FILE__, __LINE__});
}
h2_inline void h2_memory::stack::push(const h2_sz& sz)
{
   h2_stack::I().push("", "case", sz);
}
h2_inline h2_fail* h2_memory::stack::pop()
{
   return h2_stack::I().pop();
}
h2_inline long long h2_memory::stack::footprint()
{
   return h2_stack::I().top()->footprint;
}

h2_inline h2_memory::stack::block::block(const char* attributes, const h2_sz& sz)
{
   unmem = h2_extract::has(attributes, "unmem");
   if (unmem) h2_memory::hook(false);
   h2_stack::I().push(attributes, "block", sz);
}
h2_inline h2_memory::stack::block::~block()
{
   h2_fail_g(h2_stack::I().pop());
   if (unmem) h2_memory::hook();
}
