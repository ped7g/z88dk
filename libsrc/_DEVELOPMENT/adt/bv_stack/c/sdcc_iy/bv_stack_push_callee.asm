
; int bv_stack_push_callee(bv_stack_t *s, int c)

SECTION code_adt_bv_stack

PUBLIC _bv_stack_push_callee

EXTERN _b_vector_append_callee

defc _bv_stack_push_callee = _b_vector_append_callee

INCLUDE "adt/bv_stack/z80/asm_bv_stack_push.asm"
