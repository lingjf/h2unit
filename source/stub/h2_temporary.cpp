
h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* _srcfp) : srcfp(_srcfp)
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) {
      source->save(current_opcode);
      source->reset();
   }
}

h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) source->reset(current_opcode);
}
