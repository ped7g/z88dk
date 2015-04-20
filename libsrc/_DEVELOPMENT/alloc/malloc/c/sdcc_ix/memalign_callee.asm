
; void *memalign_callee(size_t alignment, size_t size)

INCLUDE "clib_cfg.asm"

SECTION code_alloc_malloc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IF __CLIB_OPT_MULTITHREAD & $01
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _memalign_callee

EXTERN _aligned_alloc_callee

defc _memalign_callee = _aligned_alloc_callee

INCLUDE "alloc/malloc/z80/asm_memalign.asm"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ELSE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _memalign_callee

EXTERN _memalign_unlocked_callee

defc _memalign_callee = _memalign_unlocked_callee
   
INCLUDE "alloc/malloc/z80/asm_memalign.asm"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ENDIF
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
