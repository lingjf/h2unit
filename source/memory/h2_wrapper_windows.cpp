
struct h2_wrapper_specific {
   h2_stubs stubs;

   h2_wrapper_specific()
   {
   }

   void overrides()
   {
      stubs.add((void*)::_aligned_malloc, (void*)h2_override::_aligned_malloc, "_aligned_malloc", __FILE__, __LINE__);
   }

   void restores()
   {
      stubs.clear();
   }
};
