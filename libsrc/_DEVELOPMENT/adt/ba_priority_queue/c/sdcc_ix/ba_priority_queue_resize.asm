
; int ba_priority_queue_resize(ba_priority_queue_t *q, size_t n)

SECTION code_adt_ba_priority_queue

PUBLIC _ba_priority_queue_resize

EXTERN l0_ba_priority_queue_resize_callee

_ba_priority_queue_resize:

   pop af
   pop hl
   pop de
   
   push de
   push hl
   push af

   jp l0_ba_priority_queue_resize_callee
