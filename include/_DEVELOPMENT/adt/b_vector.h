
#ifndef _ADT_B_VECTOR_H
#define _ADT_B_VECTOR_H

#include <stddef.h>

// DATA STRUCTURES

typedef struct b_vector_s
{

   void       *data;
   size_t      size;
   size_t      capacity;
   size_t      max_size;

} b_vector_t;

#ifdef __SDCC

// SDCC

extern size_t      b_vector_append(b_vector_t *v, int c);
extern void       *b_vector_append_block(b_vector_t *v, size_t n);
extern size_t      b_vector_append_n(b_vector_t *v, size_t n, int c);
extern int         b_vector_at(b_vector_t *v, size_t idx);
extern int         b_vector_back(b_vector_t *v);
extern size_t      b_vector_capacity(b_vector_t *v);
extern void        b_vector_clear(b_vector_t *v);
extern void       *b_vector_data(b_vector_t *v);
extern void        b_vector_destroy(b_vector_t *v);
extern int         b_vector_empty(b_vector_t *v);
extern size_t      b_vector_erase(b_vector_t *v, size_t idx);
extern size_t      b_vector_erase_block(b_vector_t *v, size_t idx, size_t n);
extern size_t      b_vector_erase_range(b_vector_t *v, size_t idx_first, size_t idx_last);
extern int         b_vector_front(b_vector_t *v);
extern b_vector_t  b_vector_init(void *p, size_t capacity, size_t max_size);
extern size_t      b_vector_insert(b_vector_t *v, size_t idx, int c);
extern void       *b_vector_insert_block(b_vector_t *v, size_t idx, size_t n);
extern size_t      b_vector_insert_n(b_vector_t *v, size_t idx, size_t n, int c);
extern size_t      b_vector_max_size(b_vector_t *v);
extern int         b_vector_pop_back(b_vector_t *v);
extern size_t      b_vector_push_back(b_vector_t *v, int c);
extern size_t      b_vector_read_block(void *dst, size_t n, b_vector_t *v, size_t idx);
extern int         b_vector_reserve(b_vector_t *v, size_t n);
extern int         b_vector_resize(b_vector_t *v, size_t n);
extern int         b_vector_shrink_to_fit(b_vector_t *);
extern size_t      b_vector_size(b_vector_t *v);
extern size_t      b_vector_write_block(void *src, size_t n, b_vector_t *v, size_t idx);

#ifndef __SDCC_DISABLE_FASTCALL

// SDCC FASTCALL LINKAGE

extern int         b_vector_back_fastcall(b_vector_t *v) __z88dk_fastcall;
extern size_t      b_vector_capacity_fastcall(b_vector_t *v) __z88dk_fastcall;
extern void        b_vector_clear_fastcall(b_vector_t *v) __z88dk_fastcall;
extern void       *b_vector_data_fastcall(b_vector_t *v) __z88dk_fastcall;
extern void        b_vector_destroy_fastcall(b_vector_t *v) __z88dk_fastcall;
extern int         b_vector_empty_fastcall(b_vector_t *v) __z88dk_fastcall;
extern int         b_vector_front_fastcall(b_vector_t *v) __z88dk_fastcall;
extern size_t      b_vector_max_size_fastcall(b_vector_t *v) __z88dk_fastcall;
extern int         b_vector_pop_back_fastcall(b_vector_t *v) __z88dk_fastcall;
extern int         b_vector_shrink_to_fit_fastcall(b_vector_t *) __z88dk_fastcall;
extern size_t      b_vector_size_fastcall(b_vector_t *v) __z88dk_fastcall;

// SDCC MAKE FASTCALL LINKAGE THE DEFAULT

#define b_vector_back(a)                b_vector_back_fastcall(a)
#define b_vectory_capacity(a)           b_vector_capacity_fastcall(a)
#define b_vector_clear(a)               b_vector_clear_fastcall(a)
#define b_vector_data(a)                b_vector_data_fastcall(a)
#define b_vector_destroy(a)             b_vector_destroy_fastcall(a)
#define b_vector_empty(a)               b_vector_empty_fastcall(a)
#define b_vector_front(a)               b_vector_front_fastcall(a)
#define b_vector_max_size(a)            b_vector_max_size_fastcall(a)
#define b_vector_pop_back(a)            b_vector_pop_back_fastcall(a)
#define b_vector_shrink_to_fit(a)       b_vector_shrink_to_fit_fastcall(a)
#define b_vector_size(a)                b_vector_size_fastcall(a)

#endif

#ifndef __SDCC_DISABLE_CALLEE

// SDCC CALLEE LINKAGE

extern size_t                           b_vector_append_callee(b_vector_t *v, int c) __z88dk_callee;
extern void                            *b_vector_append_block_callee(b_vector_t *v, size_t n) __z88dk_callee;
extern size_t                           b_vector_append_n_callee(b_vector_t *v, size_t n, int c) __z88dk_callee;
extern int                              b_vector_at_callee(b_vector_t *v, size_t idx) __z88dk_callee;
extern size_t                           b_vector_erase_callee(b_vector_t *v, size_t idx) __z88dk_callee;
extern size_t                           b_vector_erase_block_callee(b_vector_t *v, size_t idx, size_t n) __z88dk_callee;
extern size_t                           b_vector_erase_range_callee(b_vector_t *v, size_t idx_first, size_t idx_last) __z88dk_callee;
extern b_vector_t                       b_vector_init_callee(void *p, size_t capacity, size_t max_size) __z88dk_callee;
extern size_t                           b_vector_insert_callee(b_vector_t *v, size_t idx, int c) __z88dk_callee;
extern void                            *b_vector_insert_block_callee(b_vector_t *v, size_t idx, size_t n) __z88dk_callee;
extern size_t                           b_vector_insert_n_callee(b_vector_t *v, size_t idx, size_t n, int c) __z88dk_callee;
extern size_t                           b_vector_push_back_callee(b_vector_t *v, int c) __z88dk_callee;
extern size_t                           b_vector_read_block_callee(void *dst, size_t n, b_vector_t *v, size_t idx) __z88dk_callee;
extern int                              b_vector_reserve_callee(b_vector_t *v, size_t n) __z88dk_callee;
extern int                              b_vector_resize_callee(b_vector_t *v, size_t n) __z88dk_callee;
extern size_t                           b_vector_write_block_callee(void *src, size_t n, b_vector_t *v, size_t idx) __z88dk_callee;

// SDCC MAKE CALLEE LINKAGE THE DEFAULT

#define b_vector_append(a,b)                     b_vector_append_callee(a,b)
#define b_vector_append_block(a,b)               b_vector_append_block_callee(a,b)
#define b_vector_append_n(a,b,c)                 b_vector_append_n_callee(a,b,c)
#define b_vector_at(a,b)                         b_vector_at_callee(a,b)
#define b_vector_erase(a,b)                      b_vector_erase_callee(a,b)
#define b_vector_erase_block(a,b,c)              b_vector_erase_block_callee(a,b,c)
#define b_vector_erase_range(a,b,c)              b_vector_erase_range_callee(a,b,c)
#define b_vector_init(a,b,c)                     b_vector_init_callee(a,b,c)
#define b_vector_insert(a,b,c)                   b_vector_insert_callee(a,b,c)
#define b_vector_insert_block(a,b,c)             b_vector_insert_block_callee(a,b,c)
#define b_vector_insert_n(a,b,c,d)               b_vector_insert_n_callee(a,b,c,d)
#define b_vector_push_back(a,b)                  b_vector_push_back_callee(a,b)
#define b_vector_read_block(a,b,c,d)             b_vector_read_block_callee(a,b,c,d)
#define b_vector_reserve(a,b)                    b_vector_reserve_callee(a,b)
#define b_vector_resize(a,b)                     b_vector_resize_callee(a,b)
#define b_vector_write_block(a,b,c,d)            b_vector_write_block_callee(a,b,c,d)

#endif

#else

// SCCZ80

extern size_t     __LIB__               b_vector_append(b_vector_t *v, int c);
extern void       __LIB__              *b_vector_append_block(b_vector_t *v, size_t n);
extern size_t     __LIB__               b_vector_append_n(b_vector_t *v, size_t n, int c);
extern int        __LIB__               b_vector_at(b_vector_t *v, size_t idx);
extern int        __LIB__ __FASTCALL__  b_vector_back(b_vector_t *v);
extern size_t     __LIB__ __FASTCALL__  b_vector_capacity(b_vector_t *v);
extern void       __LIB__ __FASTCALL__  b_vector_clear(b_vector_t *v);
extern void       __LIB__ __FASTCALL__ *b_vector_data(b_vector_t *v);
extern void       __LIB__ __FASTCALL__  b_vector_destroy(b_vector_t *v);
extern int        __LIB__ __FASTCALL__  b_vector_empty(b_vector_t *v);
extern size_t     __LIB__               b_vector_erase(b_vector_t *v, size_t idx);
extern size_t     __LIB__               b_vector_erase_block(b_vector_t *v, size_t idx, size_t n);
extern size_t     __LIB__               b_vector_erase_range(b_vector_t *v, size_t idx_first, size_t idx_last);
extern int        __LIB__ __FASTCALL__  b_vector_front(b_vector_t *v);
extern b_vector_t __LIB__               b_vector_init(void *p, size_t capacity, size_t max_size);
extern size_t     __LIB__               b_vector_insert(b_vector_t *v, size_t idx, int c);
extern void       __LIB__              *b_vector_insert_block(b_vector_t *v, size_t idx, size_t n);
extern size_t     __LIB__               b_vector_insert_n(b_vector_t *v, size_t idx, size_t n, int c);
extern size_t     __LIB__ __FASTCALL__  b_vector_max_size(b_vector_t *v);
extern int        __LIB__ __FASTCALL__  b_vector_pop_back(b_vector_t *v);
extern size_t     __LIB__               b_vector_push_back(b_vector_t *v, int c);
extern size_t     __LIB__               b_vector_read_block(void *dst, size_t n, b_vector_t *v, size_t idx);
extern int        __LIB__               b_vector_reserve(b_vector_t *v, size_t n);
extern int        __LIB__               b_vector_resize(b_vector_t *v, size_t n);
extern int        __LIB__ __FASTCALL__  b_vector_shrink_to_fit(b_vector_t *);
extern size_t     __LIB__ __FASTCALL__  b_vector_size(b_vector_t *v);
extern size_t     __LIB__               b_vector_write_block(void *src, size_t n, b_vector_t *v, size_t idx);

// SCCZ80 CALLEE LINKAGE

extern size_t     __LIB__ __CALLEE__    b_vector_append_callee(b_vector_t *v, int c);
extern void       __LIB__ __CALLEE__   *b_vector_append_block_callee(b_vector_t *v, size_t n);
extern size_t     __LIB__ __CALLEE__    b_vector_append_n_callee(b_vector_t *v, size_t n, int c);
extern int        __LIB__ __CALLEE__    b_vector_at_callee(b_vector_t *v, size_t idx);
extern size_t     __LIB__ __CALLEE__    b_vector_erase_callee(b_vector_t *v, size_t idx);
extern size_t     __LIB__ __CALLEE__    b_vector_erase_block_callee(b_vector_t *v, size_t idx, size_t n);
extern size_t     __LIB__ __CALLEE__    b_vector_erase_range_callee(b_vector_t *v, size_t idx_first, size_t idx_last);
extern b_vector_t __LIB__ __CALLEE__    b_vector_init_callee(void *p, size_t capacity, size_t max_size);
extern size_t     __LIB__ __CALLEE__    b_vector_insert_callee(b_vector_t *v, size_t idx, int c);
extern void       __LIB__ __CALLEE__   *b_vector_insert_block_callee(b_vector_t *v, size_t idx, size_t n);
extern size_t     __LIB__ __CALLEE__    b_vector_insert_n_callee(b_vector_t *v, size_t idx, size_t n, int c);
extern size_t     __LIB__ __CALLEE__    b_vector_push_back_callee(b_vector_t *v, int c);
extern size_t     __LIB__ __CALLEE__    b_vector_read_block_callee(void *dst, size_t n, b_vector_t *v, size_t idx);
extern int        __LIB__ __CALLEE__    b_vector_reserve_callee(b_vector_t *v, size_t n);
extern int        __LIB__ __CALLEE__    b_vector_resize_callee(b_vector_t *v, size_t n);
extern size_t     __LIB__ __CALLEE__    b_vector_write_block_callee(void *src, size_t n, b_vector_t *v, size_t idx);

// SCCZ80 MAKE CALLEE LINKAGE THE DEFAULT

#define b_vector_append(a,b)                     b_vector_append_callee(a,b)
#define b_vector_append_block(a,b)               b_vector_append_block_callee(a,b)
#define b_vector_append_n(a,b,c)                 b_vector_append_n_callee(a,b,c)
#define b_vector_at(a,b)                         b_vector_at_callee(a,b)
#define b_vector_erase(a,b)                      b_vector_erase_callee(a,b)
#define b_vector_erase_block(a,b,c)              b_vector_erase_block_callee(a,b,c)
#define b_vector_erase_range(a,b,c)              b_vector_erase_range_callee(a,b,c)
#define b_vector_init(a,b,c)                     b_vector_init_callee(a,b,c)
#define b_vector_insert(a,b,c)                   b_vector_insert_callee(a,b,c)
#define b_vector_insert_block(a,b,c)             b_vector_insert_block_callee(a,b,c)
#define b_vector_insert_n(a,b,c,d)               b_vector_insert_n_callee(a,b,c,d)
#define b_vector_push_back(a,b)                  b_vector_push_back_callee(a,b)
#define b_vector_read_block(a,b,c,d)             b_vector_read_block_callee(a,b,c,d)
#define b_vector_reserve(a,b)                    b_vector_reserve_callee(a,b)
#define b_vector_resize(a,b)                     b_vector_resize_callee(a,b)
#define b_vector_write_block(a,b,c,d)            b_vector_write_block_callee(a,b,c,d)

#endif

#endif
