
#include "mini_disassembler.h"
#include "mini_disassembler_types.h"

const ModrmEntry MiniDisassembler::s_ia16_modrm_map_[] = {
// mod == 00
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, false, OS_ZERO },
  /* r/m == 101 */ { false, false, OS_ZERO },
  /* r/m == 110 */ { true, false, OS_WORD },
  /* r/m == 111 */ { false, false, OS_ZERO },
// mod == 01
  /* r/m == 000 */ { true, false, OS_BYTE },
  /* r/m == 001 */ { true, false, OS_BYTE },
  /* r/m == 010 */ { true, false, OS_BYTE },
  /* r/m == 011 */ { true, false, OS_BYTE },
  /* r/m == 100 */ { true, false, OS_BYTE },
  /* r/m == 101 */ { true, false, OS_BYTE },
  /* r/m == 110 */ { true, false, OS_BYTE },
  /* r/m == 111 */ { true, false, OS_BYTE },
// mod == 10
  /* r/m == 000 */ { true, false, OS_WORD },
  /* r/m == 001 */ { true, false, OS_WORD },
  /* r/m == 010 */ { true, false, OS_WORD },
  /* r/m == 011 */ { true, false, OS_WORD },
  /* r/m == 100 */ { true, false, OS_WORD },
  /* r/m == 101 */ { true, false, OS_WORD },
  /* r/m == 110 */ { true, false, OS_WORD },
  /* r/m == 111 */ { true, false, OS_WORD },
// mod == 11
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, false, OS_ZERO },
  /* r/m == 101 */ { false, false, OS_ZERO },
  /* r/m == 110 */ { false, false, OS_ZERO },
  /* r/m == 111 */ { false, false, OS_ZERO }
};

const ModrmEntry MiniDisassembler::s_ia32_modrm_map_[] = {
// mod == 00
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, true, OS_ZERO },
  /* r/m == 101 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 110 */ { false, false, OS_ZERO },
  /* r/m == 111 */ { false, false, OS_ZERO },
// mod == 01
  /* r/m == 000 */ { true, false, OS_BYTE },
  /* r/m == 001 */ { true, false, OS_BYTE },
  /* r/m == 010 */ { true, false, OS_BYTE },
  /* r/m == 011 */ { true, false, OS_BYTE },
  /* r/m == 100 */ { true, true, OS_BYTE },
  /* r/m == 101 */ { true, false, OS_BYTE },
  /* r/m == 110 */ { true, false, OS_BYTE },
  /* r/m == 111 */ { true, false, OS_BYTE },
// mod == 10
  /* r/m == 000 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 001 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 010 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 011 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 100 */ { true, true, OS_DOUBLE_WORD },
  /* r/m == 101 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 110 */ { true, false, OS_DOUBLE_WORD },
  /* r/m == 111 */ { true, false, OS_DOUBLE_WORD },
// mod == 11
  /* r/m == 000 */ { false, false, OS_ZERO },
  /* r/m == 001 */ { false, false, OS_ZERO },
  /* r/m == 010 */ { false, false, OS_ZERO },
  /* r/m == 011 */ { false, false, OS_ZERO },
  /* r/m == 100 */ { false, false, OS_ZERO },
  /* r/m == 101 */ { false, false, OS_ZERO },
  /* r/m == 110 */ { false, false, OS_ZERO },
  /* r/m == 111 */ { false, false, OS_ZERO },
};
