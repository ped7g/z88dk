
; void _ldiv__callee(ldiv_t *ld, long numer, long denom)

SECTION code_stdlib

PUBLIC __ldiv__callee

__ldiv__callee:

   pop af
   pop bc
   exx
   pop hl
   pop de
   exx
   pop hl
   pop de
   push af
   
   INCLUDE "stdlib/z80/asm__ldiv.asm"
