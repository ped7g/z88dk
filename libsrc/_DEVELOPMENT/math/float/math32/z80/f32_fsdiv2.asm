;
;  feilipu, 2019 June
;
;  This Source Code Form is subject to the terms of the Mozilla Public
;  License, v. 2.0. If a copy of the MPL was not distributed with this
;  file, You can obtain one at http://mozilla.org/MPL/2.0/.
;
;-------------------------------------------------------------------------
; m32_fsdiv2 - z80, z180, z80-zxn floating point divide by 2
;-------------------------------------------------------------------------
; 
; Division by 2 is decrementing the exponent. An easy optimisation.
;
;-------------------------------------------------------------------------
; FIXME clocks
;-------------------------------------------------------------------------

SECTION code_fp_math32

EXTERN m32_fszero_fastcall
EXTERN m32_fsmin_fastcall

PUBLIC m32_fsdiv2_fastcall
PUBLIC _m32_div2f


._m32_div2f
.m32_fsdiv2_fastcall
    sla e                       ; get exponent in d
    rl d                        ; put sign in C
    rr e                        ; save sign in e[7]

    xor a
    and d
    jp Z,m32_fszero_fastcall

    dec d                       ; divide by 2
    jp Z,m32_fsmin_fastcall     ; capture underflow

    sla e                       ; restore sign to C
    rr d                        ; return sign and exponent
    rr e
    ret                         ; return IEEE DEHL
