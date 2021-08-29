struct h2_stub_temporary_restore : h2_once {
   unsigned char current_opcode[32];
   void* srcfp;
   h2_stub_temporary_restore(void* srcfp);
   ~h2_stub_temporary_restore();
};
