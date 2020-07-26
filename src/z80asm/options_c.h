//-----------------------------------------------------------------------------
// Z88DK Z80 Module Assembler
// Parse command line options
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#pragma once

#include "legacy.h"

#include "strutil.h"
#include "types.h"
#include "utarray.h"
#include "utstring.h"

/*-----------------------------------------------------------------------------
*   Assembler standard library
*----------------------------------------------------------------------------*/
#define Z80ASM_LIB	"z80asm-%s-%s.lib"
#define SWAP_IX_IY_NAME	(SwapIxIy() ? "ixiy" : "")

/*-----------------------------------------------------------------------------
*   APPMAKE type
*----------------------------------------------------------------------------*/
typedef enum { APPMAKE_NONE, APPMAKE_ZX81, APPMAKE_ZX } appmake_t;

/*-----------------------------------------------------------------------------
*   singleton opts
*----------------------------------------------------------------------------*/
#define OPT_VAR(type, name, default)	type name;
typedef struct Opts
{
#include "options_def.h"
}
Opts;

extern Opts opts;

/*-----------------------------------------------------------------------------
*   Parse command line, set options, including opts.files with list of
*	input files, including parsing of '@' lists
*----------------------------------------------------------------------------*/
extern void parse_argv( int argc, char *argv[] );

/*-----------------------------------------------------------------------------
*   Change extension of given file name, return pointer to file name in
*	strpool
*	Extensions may be changed by options.
*----------------------------------------------------------------------------*/
extern const char *get_asm_filename(const char *filename );
extern const char *get_list_filename(const char *filename );
extern const char *get_obj_filename(const char *filename );
extern const char *get_def_filename(const char *filename );
extern const char *get_err_filename(const char *filename );
extern const char *get_bin_filename(const char *filename );
extern const char *get_lib_filename(const char *filename );
extern const char *get_sym_filename(const char *filename );
extern const char *get_map_filename(const char *filename);
extern const char *get_reloc_filename(const char *filename);

/*-----------------------------------------------------------------------------
*   Call appmake if requested in options
*----------------------------------------------------------------------------*/
extern void checkrun_appmake(void);

#define ZX_ORIGIN		 23760		/* origin for unexpanded ZX Spectrum */
#define ZX_ORIGIN_S		"23760"
#define ZX_ORIGIN_MIN	 23760
#define ZX_ORIGIN_MAX	 0xFFFF
#define ZX_APP_EXT		".tap"		/* ZX Spectrum TAP file */

#define ZX81_ORIGIN		 16514		/* origin for ZX 81 */
#define ZX81_ORIGIN_S	"16514"
#define ZX81_ORIGIN_MIN	 16514
#define ZX81_ORIGIN_MAX  16514
#define ZX81_APP_EXT	".P"		/* ZX81 .P file */
