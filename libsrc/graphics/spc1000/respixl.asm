
		SECTION	code_clib
		PUBLIC	respixel

		EXTERN	res_MODE0
		EXTERN	res_MODE1
		EXTERN	res_MODE2

		EXTERN	__spc1000_mode
		INCLUDE	"target/spc1000/def/spc1000.def"


respixel:
		ld	a,(__spc1000_mode)
		cp	1
		jp	z,res_MODE1
		cp	2
		jp	z,res_MODE2
		and	a
		ret	nz
		jp	res_MODE0
