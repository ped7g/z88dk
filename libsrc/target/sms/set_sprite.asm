	SECTION code_clib	
	PUBLIC	set_sprite
	PUBLIC	_set_sprite

    EXTERN  CONSOLE_YOFFSET
    EXTERN  CONSOLE_XOFFSET

	include	"macros.inc"
;==============================================================
; set_sprite(int n, int x, int y, int tile)	
;==============================================================
; Loads the specified tileset into VRAM
;==============================================================
.set_sprite
._set_sprite
	ld	hl, 2
	add	hl, sp
	ld	d, (hl)		; Tile
	inc	hl
	inc	hl
	ld	a,+(CONSOLE_YOFFSET * 8)
	add (hl)		; Y
	ld	b,a
	inc	hl
	inc	hl
	ld	a,+(CONSOLE_XOFFSET * 8)
	add (hl)		; X
	ld	c,a
	inc	hl
	inc	hl
	ld	e, (hl)		; N
	
	;vdp set addr (Y table)
	setVRAM	e, $3f
	
	ld	a, b
	out	($be), a	; Set Y
	
	;vdp set addr (X/Tile table)
	ld	a, e
	add	a, a
	or	$80
	setVRAM	a, $3f

	ld a, c
	out ($be), a		; Set X
	nop			;4
	nop
	nop
	nop
	nop
	nop
	ld a, d			;4
	out ($be), a		; Set Tile

	ret
