
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
   if (O.memory_check) {
      if (!O.debug) h2_crash::install();
      h2_exempt::setup();
      h2_overrides::I().set();
   }
   stack::root();
}
h2_inline void h2_memory::finalize()
{
   if (O.memory_check) h2_stack::I().at_exit = true;
}
h2_inline void h2_memory::overrides()
{
   if (O.memory_check) h2_overrides::I().set();
}
h2_inline void h2_memory::restores()
{
   if (O.memory_check) h2_overrides::I().reset();
}

h2_inline void h2_memory::stack::root()
{
   h2_stack::I().push("", "root", __FILE__, __LINE__);
}
h2_inline void h2_memory::stack::push(const char* file, int line)
{
   h2_stack::I().push("", "case", file, line);
}
h2_inline h2_fail* h2_memory::stack::pop()
{
   return h2_stack::I().pop();
}
h2_inline long long h2_memory::stack::footprint()
{
   return h2_stack::I().top()->footprint;
}

h2_inline h2_memory::stack::block::block(const char* attributes, const char* file, int line)
{
   h2_stack::I().push(attributes, "block", file, line);
}
h2_inline h2_memory::stack::block::~block()
{
   h2_fail_g(h2_stack::I().pop());
}
