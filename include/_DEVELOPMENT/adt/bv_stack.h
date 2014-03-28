
#ifndef _ADT_BV_STACK_H
#define _ADT_BV_STACK_H

#include <_DEVELOPMENT/stddef.h>

// DATA STRUCTURES

typedef struct bv_stack_s
{

   void       *data;
   size_t      size;
   size_t      capacity;
   size_t      max_size;

} bv_stack_t;

// SCCZ80

extern size_t     __LIB__ __FASTCALL__  bv_stack_capacity(bv_stack_t *s);
extern void       __LIB__ __FASTCALL__  bv_stack_clear(bv_stack_t *s);
extern void       __LIB__ __FASTCALL__  bv_stack_destroy(bv_stack_t *s);
extern int        __LIB__ __FASTCALL__  bv_stack_empty(bv_stack_t *s);
extern bv_stack_t __LIB__              *bv_stack_init(void *p, size_t capacity, size_t max_size);
extern size_t     __LIB__ __FASTCALL__  bv_stack_max_size(bv_stack_t *s);
extern int        __LIB__ __FASTCALL__  bv_stack_pop(bv_stack_t *s);
extern int        __LIB__               bv_stack_push(bv_stack_t *s, int c);
extern int        __LIB__               bv_stack_reserve(bv_stack_t *s, size_t n);
extern int        __LIB__ __FASTCALL__  bv_stack_shrink_to_fit(bv_stack_t *s);
extern size_t     __LIB__ __FASTCALL__  bv_stack_size(bv_stack_t *s);
extern int        __LIB__ __FASTCALL__  bv_stack_top(bv_stack_t *s);

// SCCZ80 CALLEE LINKAGE

extern bv_stack_t __LIB__ __CALLEE__   *bv_stack_init_callee(void *p, size_t capacity, size_t max_size);
extern int        __LIB__ __CALLEE__    bv_stack_push_callee(bv_stack_t *s, int c);
extern int        __LIB__ __CALLEE__    bv_stack_reserve_callee(bv_stack_t *s, size_t n);

// SCCZ80 MAKE CALLEE LINKAGE THE DEFAULT

#define bv_stack_init(a,b,c)                     bv_stack_init_callee(a,b,c)
#define bv_stack_push(a,b)                       bv_stack_push_callee(a,b)
#define bv_stack_reserve(a,b)                    bv_stack_reserve_callee(a,b)

#endif
