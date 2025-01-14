;
;  Copyright (c) 2020 Phillip Stevens
;
;  This Source Code Form is subject to the terms of the Mozilla Public
;  License, v. 2.0. If a copy of the MPL was not distributed with this
;  file, You can obtain one at http://mozilla.org/MPL/2.0/.
;
;  feilipu, August 2020
;
;-------------------------------------------------------------------------
;  asm_am9511_pushl - am9511 APU push long
;-------------------------------------------------------------------------
; 
;  Load long into Am9511 APU stack
;
;-------------------------------------------------------------------------

SECTION code_fp_am9511

EXTERN __IO_APU_STATUS, __IO_APU_DATA

PUBLIC asm_am9511_pushl
PUBLIC asm_am9511_pushl_fastcall


.asm_am9511_pushl

    ; float primitive
    ; push a long into Am9511 stack.
    ;
    ; enter : stack = long, ret1, ret0
    ;
    ; exit  : stack = long, ret1
    ; 
    ; uses  : af, bc', hl'

;   in a,(__IO_APU_STATUS)      ; read the APU status register
;   rlca                        ; busy? __IO_APU_STATUS_BUSY
;   jr C,asm_am9511_pushl_hl

    exx                         ; preserve dehl

    ld hl,4
    add hl,sp

    ld bc,__IO_APU_DATA         ; the address of the APU data port in bc
    outi                        ; load LSW into APU
    outi
    outi                        ; load MSW into APU
    outi

    exx
    ret


.asm_am9511_pushl_fastcall

    ; float primitive
    ; push a long into Am9511 stack.
    ;
    ; enter : stack = ret1, ret0
    ;       :  dehl = long
    ;
    ; exit  : stack = ret1
    ; 
    ; uses  : af, bc, de, hl

;   in a,(__IO_APU_STATUS)      ; read the APU status register
;   rlca                        ; busy? __IO_APU_STATUS_BUSY
;   jr C,asm_am9511_pushl_fastcall

    ld bc,__IO_APU_DATA         ; the address of the APU data port in bc
    out (c),l                   ; load LSW into APU
    out (c),h
    out (c),e                   ; load MSW into APU
    out (c),d
    ret

